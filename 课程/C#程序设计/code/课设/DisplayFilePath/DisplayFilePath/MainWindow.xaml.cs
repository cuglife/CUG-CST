using System.Windows;
using System.Windows.Forms;
using MessageBox = System.Windows.MessageBox;
using FileDialog = Microsoft.Win32.FileDialog;
using Path = System.IO.Path;
using Directory = System.IO.Directory;

namespace DisplayFilePath
{
    /// <summary>
    /// @author 191181 高天翔
    /// @time 2020.10.24
    /// 获取指定文件的信息：将选择的文件夹或文件的路径的根信息和父目录信息显示在文本框中
    /// </summary>
    public partial class MainWindow
    {
        private string _filePath = "/";

        public MainWindow()
        {
            InitializeComponent();
        }

        private void Button_FileFolderSelect_Click(object sender, RoutedEventArgs e)
        {
            var dialog = new FolderBrowserDialog();
            dialog.ShowDialog();
            _filePath = dialog.SelectedPath;
            FilePathBox.Text = _filePath;
        }

        private void Button_FileSelect_Click(object sender, RoutedEventArgs e)
        {
            FileDialog fileDialog = new Microsoft.Win32.OpenFileDialog();
            fileDialog.ShowDialog();
            _filePath = fileDialog.FileName;
            FilePathBox.Text = _filePath;
        }

        private void Button_ShowRootInfo_Click(object sender, RoutedEventArgs e)
        {
            _filePath = Path.GetPathRoot(_filePath);
            FilePathBox.Text = _filePath;
        }

        private void Button_ShowFatherMenu_Click(object sender, RoutedEventArgs e)
        {
            var parentPath = Directory.GetParent(_filePath);
            if (parentPath != null)
                _filePath = parentPath.FullName;
            else
                MessageBox.Show("已经到达根目录!", "通知");

            FilePathBox.Text = _filePath;
        }
    }
}