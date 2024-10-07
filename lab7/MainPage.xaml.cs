using System;
using System.Threading.Tasks;
using Windows.Storage;
using Windows.Storage.Pickers;
using Windows.Storage.AccessCache;
using Windows.UI.Xaml;
using Windows.UI.Xaml.Controls;
using TreeView = Microsoft.UI.Xaml.Controls.TreeView;
using TreeViewNode = Microsoft.UI.Xaml.Controls.TreeViewNode;
using Windows.UI.Xaml.Input;

namespace lab7
{
    public sealed partial class MainPage : Page
    {
        public MainPage()
        {
            this.InitializeComponent();
            FolderTreeView.Expanding += FolderTreeView_Expanding;
        }
        private async void PickFolderButton_Click(object sender, RoutedEventArgs e)
        {
            // Clear any previous error messages
            ErrorMessage.Visibility = Visibility.Collapsed;

            // Show loading spinner before opening folder picker
            LoadingIndicator.IsActive = true;
            LoadingIndicator.Visibility = Visibility.Visible;

            FolderPicker folderPicker = new FolderPicker
            {
                SuggestedStartLocation = PickerLocationId.Desktop
            };
            folderPicker.FileTypeFilter.Add("*"); // We need to add at least one filter, even if selecting folders.

            StorageFolder folder = await folderPicker.PickSingleFolderAsync();

            if (folder != null)
            {
                try
                {
                    // Clear any existing items in the TreeView.
                    FolderTreeView.RootNodes.Clear();

                    // Load the folder and subfolders into the TreeView.
                    ulong folderSize = await GetFolderSizeAsync(folder);
                    var rootNode = new TreeViewNode
                    {
                        Content = new FolderItem
                        {
                            Folder = folder,
                            DisplayName = folder.Name,
                            Size = FormatSize(folderSize)
                        },
                        HasUnrealizedChildren = true // Set this so that expanding triggers loading of children
                    };

                    FolderTreeView.RootNodes.Add(rootNode);

                    // Optionally load the contents of the root folder immediately
                    // await LoadFolderContentsAsync(rootNode, folder);
                }
                catch (UnauthorizedAccessException)
                {
                    // Show error message if access is denied
                    ErrorMessage.Text = "Access Denied to folder: " + folder.Name;
                    ErrorMessage.Visibility = Visibility.Visible;
                }
            }

            // Hide loading spinner after folder is selected and loaded
            LoadingIndicator.IsActive = false;
            LoadingIndicator.Visibility = Visibility.Collapsed;
        }

        private async Task LoadFolderContentsAsync(TreeViewNode node, StorageFolder folder)
        {
            try
            {
                // Get the subfolders and files
                var items = await folder.GetItemsAsync();
                foreach (var item in items)
                {
                    if (item is StorageFolder subFolder)
                    {
                        ulong folderSize = await GetFolderSizeAsync(subFolder);
                        var folderNode = new TreeViewNode
                        {
                            Content = new FolderItem { Folder = subFolder, DisplayName = subFolder.Name, Size = FormatSize(folderSize) },
                            HasUnrealizedChildren = true
                        };
                        node.Children.Add(folderNode);
                    }
                    else if (item is StorageFile file)
                    {
                        var fileNode = new TreeViewNode
                        {
                            Content = new FileItem { File = file, DisplayName = file.Name, Size = await GetFileSizeAsync(file) }
                        };
                        node.Children.Add(fileNode);
                    }
                }
            }
            catch (UnauthorizedAccessException)
            {
                // Show error message if access is denied
                ErrorMessage.Text = "Access Denied to folder: " + folder.Name;
                ErrorMessage.Visibility = Visibility.Visible;
            }
            catch (Exception ex)
            {
                System.Diagnostics.Debug.WriteLine($"Error loading folder contents: {ex.Message}");
            }
        }


        private async void FolderTreeView_Expanding(TreeView sender, Microsoft.UI.Xaml.Controls.TreeViewExpandingEventArgs args)
        {
            var expandingNode = args.Node;

            if (expandingNode.HasUnrealizedChildren)
            {
                expandingNode.Children.Clear(); // Clear placeholder nodes

                // Disable TreeView interaction and show loading spinner
                FolderTreeView.IsEnabled = false;
                LoadingIndicator.IsActive = true;
                LoadingIndicator.Visibility = Visibility.Visible;

                expandingNode.HasUnrealizedChildren = false;

                if (expandingNode.Content is FolderItem folderItem)
                {
                    // Load the folder contents asynchronously
                    await LoadFolderContentsAsync(expandingNode, folderItem.Folder);
                }

                // Re-enable TreeView interaction and hide loading spinner
                FolderTreeView.IsEnabled = true;
                LoadingIndicator.IsActive = false;
                LoadingIndicator.Visibility = Visibility.Collapsed;
            }
        }

        private async void CopyButton_Click(object sender, RoutedEventArgs e)
        {
            if (FolderTreeView.SelectedNode != null)
            {
                if (FolderTreeView.SelectedNode.Content is FolderItem folderItem)
                {
                    await CopyFolderAsync(folderItem.Folder);
                }
                else if (FolderTreeView.SelectedNode.Content is FileItem fileItem)
                {
                    await CopyFileAsync(fileItem.File);
                }
            }
        }

        private async Task CopyFolderAsync(StorageFolder sourceFolder)
        {
            FolderPicker folderPicker = new FolderPicker
            {
                SuggestedStartLocation = PickerLocationId.Desktop
            };
            folderPicker.FileTypeFilter.Add("*");
            StorageFolder destinationFolder = await folderPicker.PickSingleFolderAsync();

            if (destinationFolder != null)
            {
                await CopyFolderContentsAsync(sourceFolder, destinationFolder);
            }
        }

        private async Task CopyFolderContentsAsync(StorageFolder sourceFolder, StorageFolder destinationFolder)
        {
            // Create a new folder with the same name in the destination
            StorageFolder newFolder = await destinationFolder.CreateFolderAsync(sourceFolder.Name, CreationCollisionOption.GenerateUniqueName);

            // Copy files
            foreach (var item in await sourceFolder.GetItemsAsync())
            {
                if (item is StorageFile file)
                {
                    await file.CopyAsync(newFolder);
                }
                else if (item is StorageFolder folder)
                {
                    await CopyFolderContentsAsync(folder, newFolder);
                }
            }
        }

        private async Task CopyFileAsync(StorageFile sourceFile)
        {
            FolderPicker folderPicker = new FolderPicker
            {
                SuggestedStartLocation = PickerLocationId.Desktop
            };
            folderPicker.FileTypeFilter.Add("*");
            StorageFolder destinationFolder = await folderPicker.PickSingleFolderAsync();

            if (destinationFolder != null)
            {
                await sourceFile.CopyAsync(destinationFolder, sourceFile.Name, NameCollisionOption.GenerateUniqueName);
            }
        }

        // New method for deleting folder or file
        private async void DeleteButton_Click(object sender, RoutedEventArgs e)
        {
            if (FolderTreeView.SelectedNode != null)
            {
                if (FolderTreeView.SelectedNode.Content is FolderItem folderItem)
                {
                    await DeleteFolderAsync(folderItem.Folder);
                }
                else if (FolderTreeView.SelectedNode.Content is FileItem fileItem)
                {
                    await DeleteFileAsync(fileItem.File);
                }
            }
        }

        private async Task DeleteFolderAsync(StorageFolder folder)
        {
            var dialog = new ContentDialog
            {
                Title = "Delete Folder",
                Content = $"Are you sure you want to delete the folder '{folder.Name}'?",
                PrimaryButtonText = "Delete",
                CloseButtonText = "Cancel"
            };

            var result = await dialog.ShowAsync();
            if (result == ContentDialogResult.Primary)
            {
                await folder.DeleteAsync();
            }
        }

        private async Task DeleteFileAsync(StorageFile file)
        {
            var dialog = new ContentDialog
            {
                Title = "Delete File",
                Content = $"Are you sure you want to delete the file '{file.Name}'?",
                PrimaryButtonText = "Delete",
                CloseButtonText = "Cancel"
            };

            var result = await dialog.ShowAsync();
            if (result == ContentDialogResult.Primary)
            {
                await file.DeleteAsync();
            }
        }

        // Helper to get file size in a human-readable format
        private async Task<string> GetFileSizeAsync(StorageFile file)
        {
            var basicProperties = await file.GetBasicPropertiesAsync();
            return FormatSize(basicProperties.Size);
        }

        // Helper to recursively calculate folder size
        private async Task<ulong> GetFolderSizeAsync(StorageFolder folder)
        {
            ulong totalSize = 0;
            var items = await folder.GetItemsAsync();
            foreach (var item in items)
            {
                if (item is StorageFile file)
                {
                    var basicProperties = await file.GetBasicPropertiesAsync();
                    totalSize += basicProperties.Size;
                }
                else if (item is StorageFolder subFolder)
                {
                    totalSize += await GetFolderSizeAsync(subFolder);
                }
            }
            return totalSize;
        }

        // Utility function to format the size to a readable format (Bytes, KB, MB, GB)
        private string FormatSize(ulong bytes)
        {
            string[] sizeSuffixes = { "Bytes", "KB", "MB", "GB", "TB" };
            int index = 0;
            double size = bytes;

            while (size >= 1024 && index < sizeSuffixes.Length - 1)
            {
                index++;
                size /= 1024;
            }

            return $"{size:0.##} {sizeSuffixes[index]}";
        }
    }

    public class FolderItem
    {
        public StorageFolder Folder { get; set; }
        public string DisplayName { get; set; }
        public string Size { get; set; }
    }

    public class FileItem
    {
        public StorageFile File { get; set; }
        public string DisplayName { get; set; }
        public string Size { get; set; }
    }
}