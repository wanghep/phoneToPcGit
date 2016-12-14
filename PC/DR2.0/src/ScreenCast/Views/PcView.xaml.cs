using System.Windows;
using System.Windows.Input;
using ScreenCast.ViewModels;

namespace ScreenCast.Views
{
    /// <summary>
    /// PcView.xaml 的交互逻辑
    /// </summary>
    public partial class PcView : IClientView
    {
        private int _isVertical = 0;
        public PcView()
        {
            InitializeComponent();
            DataContext = new PhoneViewModel(this);
        }

        #region IClientView
        public void FullScreen(bool full)
        {
            if (full)
            {
                Root.Margin = new Thickness(0);
                WindowState = WindowState.Maximized;
            }
            else
            {
                Root.Margin = new Thickness(20);
                WindowState = WindowState.Normal;
            }
        }

        public void OnBuffer(int width, int height, int isVertical, int bkSize)
        {
            if (_isVertical != isVertical)
            {
                _isVertical = isVertical;
            }
        }
        #endregion
        private void Grid_MouseLeftButtonDown(object sender, MouseButtonEventArgs e)
        {
            DragMove();
        }

        protected override void OnTextInput(TextCompositionEventArgs e)
        {
            base.OnTextInput(e);
            Player.OnText(e.Text);
        }
    }
}
