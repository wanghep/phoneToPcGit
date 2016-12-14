using System.Windows.Input;
using ScreenCast.Commands;

namespace ScreenCast.Views
{
    /// <summary>
    /// AboutWindow.xaml 的交互逻辑
    /// </summary>
    public partial class AboutWindow
    {
        public AboutWindow()
        {
            InitializeComponent();
        }

        private void AboutWindow_OnMouseLeftButtonDown(object sender, MouseButtonEventArgs e)
        {
            DragMove();
        }

        private void OnAboutExitClick(object sender, System.Windows.RoutedEventArgs e)
        {
            Hide();
        }
    }
}
