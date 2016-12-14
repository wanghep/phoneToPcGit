using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Shapes;

namespace ScreenCast.Views
{
    /// <summary>
    /// HelpWindow.xaml 的交互逻辑
    /// </summary>
    public partial class WindowHelp : Window
    {
        public WindowHelp()
        {
            InitializeComponent();
        }

        private void OnClickExit(object sender, RoutedEventArgs e)
        {
            Hide();
        }

        private void WindowHelp_OnMouseLeftButtonDown(object sender, MouseButtonEventArgs e)
        {
            DragMove(); 
        }
    }
}
