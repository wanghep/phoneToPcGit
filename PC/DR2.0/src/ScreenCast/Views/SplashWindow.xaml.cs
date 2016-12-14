using System;
using System.Windows.Input;
using ScreenCast.ViewModels;
using System.Windows.Controls;
using System.Windows.Media.Animation;


namespace ScreenCast.Views
{
    /// <summary>
    /// SplashWindow.xaml 的交互逻辑
    /// </summary>
    public partial class SplashWindow
    {
        public SplashWindow()
        {
            InitializeComponent();
            DataContext = new SplashViewModel(this);

            //var sb = FindResource("Storyboardflush") as Storyboard;
            //if (sb != null)
             //   sb.Begin();
        }

        private void Grid_MouseLeftButtonDown(object sender, MouseButtonEventArgs e)
        {
            DragMove();
        }

        private void MediaElement_MediaEnded(object sender, System.Windows.RoutedEventArgs e)
        {
            ((MediaElement)sender).Position = ((MediaElement)sender).Position.Add(TimeSpan.FromMilliseconds(1));
        }

    }
}
