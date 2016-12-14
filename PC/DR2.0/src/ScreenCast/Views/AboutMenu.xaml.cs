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
    /// AboutMenu.xaml 的交互逻辑
    /// </summary>
    public partial class AboutMenu : Window
    {
        public AboutMenu g_AboutMenu = null;
        public AboutMenu()
        {
            InitializeComponent();

            g_AboutMenu = this;
        }

        private void AboutWindow_OnMouseLeftButtonDown(object sender, MouseButtonEventArgs e)
        {
            DragMove();
        }



        private void OnFocusableChanged(object sender, DependencyPropertyChangedEventArgs e)
        {
            Hide();
        }

        private void OnLostFocus(object sender, RoutedEventArgs e)
        {
            Hide();
        }

    }
}
