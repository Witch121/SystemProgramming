using System;
using System.Diagnostics;
using System.IO;
using System.IO.Compression;
using System.Threading;
using System.Windows;
using Microsoft.Win32;

namespace Lab5._1
{
    public partial class MainWindow : Window
    {
        public MainWindow()
        {
            InitializeComponent();
        }

        private void SelectFile_Click(object sender, RoutedEventArgs e)
        {
            // Open a file dialog to select a file
            OpenFileDialog openFileDialog = new OpenFileDialog();
            if (openFileDialog.ShowDialog() == true)
            {
                string selectedFile = openFileDialog.FileName;

                // Determine if the file is a .7z file for unpacking, otherwise pack the file
                if (System.IO.Path.GetExtension(selectedFile).ToLower() == ".7z")
                {
                    RunProcess($"x \"{selectedFile}\""); // Unpack the file
                }
                else
                {
                    RunProcess($"a \"{selectedFile}.7z\" \"{selectedFile}\""); // Pack the file
                }
            }
        }

        // Helper method to run the 7z process
        private void RunProcess(string arguments)
        {
            try
            {
                // Assuming you have placed 7z.exe in a folder named "tools" inside your project directory
                string pathTo7z = System.IO.Path.Combine(AppDomain.CurrentDomain.BaseDirectory, "tools", "7z.exe");

                ProcessStartInfo startInfo = new ProcessStartInfo
                {
                    FileName = pathTo7z,  // Path to 7z.exe inside the tools folder
                    Arguments = arguments,
                    RedirectStandardOutput = true,  // Capture standard output
                    RedirectStandardError = true,   // Capture error output
                    UseShellExecute = false,
                    CreateNoWindow = true           // No command window shown
                };

                // Start the process
                Process process = new Process { StartInfo = startInfo };
                process.OutputDataReceived += (sender, e) => Dispatcher.Invoke(() => LogTextBox.AppendText(e.Data + "\n"));
                process.ErrorDataReceived += (sender, e) => Dispatcher.Invoke(() => LogTextBox.AppendText(e.Data + "\n"));

                process.Start();
                process.BeginOutputReadLine();  // Start reading standard output
                process.BeginErrorReadLine();   // Start reading error output
                process.WaitForExit();          // Wait for process to finish
            }
            catch (Exception ex)
            {
                // Log any errors to the TextBox
                LogTextBox.AppendText($"Error: {ex.Message}\n");
            }
        }
    }
}