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

using ScreenCast.Commands;
using ScreenCast.Views;


namespace ScreenCast.Views
{
    /// <summary>
    /// Connecting.xaml 的交互逻辑
    /// </summary>
    public partial class Connecting 
    {


        public Connecting()
        {
            InitializeComponent();



        }


        private void OnMinClickMin(object sender, RoutedEventArgs e)  //最小化
        {
           Hide();
        }


        private void Grid_MouseLeftButtonDown(object sender, MouseButtonEventArgs e)
        {
            DragMove();
        }

    }
}
