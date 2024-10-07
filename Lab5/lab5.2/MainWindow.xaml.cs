using System;
using System.Windows;
using System.IO;
using System.Threading;

namespace lab5._2
{
    public partial class MainWindow : Window
    {
        public MainWindow()
        {
            InitializeComponent();
        }
        class Logger
        {
            private readonly string _filePath;
            private Mutex _mutex;

            public Logger(string filePath)
            {
                _filePath = filePath;
                _mutex = new Mutex();
                File.WriteAllText(_filePath, string.Empty);
            }

            public void WriteLog(string logMessage)
            {
                _mutex.WaitOne();
                try
                {
                    File.AppendAllText(_filePath, logMessage + Environment.NewLine);
                }
                finally
                {
                    _mutex.ReleaseMutex();
                }
            }
        }

        private void StartThreads_Click(object sender, RoutedEventArgs e)
        {
            int threadCount = int.Parse(ThreadCountTextBox.Text);
            Logger logger = new Logger("log.txt");
            for (int i = 0; i < threadCount; i++)
            {
                int threadNum = i;
                new Thread(() =>
                {
                    Random random = new Random();
                    int delay = random.Next(800, 1200); // ~1 second delay
                    Thread.Sleep(delay);
                    string logMessage = $"Thread {threadNum}: {DateTime.Now:HH:mm:ss.fff}, Delay: {delay} ms";
                    logger.WriteLog(logMessage);
                    Dispatcher.Invoke(() => LogTextBox.AppendText(logMessage + "\n"));
                }).Start();
            }
        }
    }
}
