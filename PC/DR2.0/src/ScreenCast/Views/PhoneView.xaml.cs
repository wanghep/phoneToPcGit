using System;
using System.Diagnostics;
using System.Windows;
using System.Windows.Input;
using ScreenCast.ViewModels;
using ScreenCast.Client;



namespace ScreenCast.Views
{
    /// <summary>
    /// PhoneView.xaml 的交互逻辑
    /// </summary>
    public partial class PhoneView : IClientView
    {
        private IntPtr _rgn = IntPtr.Zero;
        private int _isVertical = 1;
        private bool _isFull = false;
        private readonly Resizer _resizer = new Resizer();

        private FloatingToolBars m_lft;
        //private FloatingToolBars m_flt;

        private PhoneViewModel m_PhoneViewModel;

        

        public PhoneView()
        {
            InitializeComponent();
            m_PhoneViewModel = new PhoneViewModel(this);
            DataContext = m_PhoneViewModel;
            SwitchOrientation();

        }



        #region IClientView
        public void FullScreen(bool full)
        {
            if (_isFull == full)
                return;
            _isFull = full;
            if (full)
            {
                Player.Margin = new Thickness(0);
                WindowState = WindowState.Maximized;
                //切换全屏的时候显示出来窗口 
                if (m_lft != null)
                {
                    // Debug.WriteLine("ppppp left:{0} ,top:{1} width:{2}", this.Left, this.Top ,this.Width);
                    m_lft.ShowMyBar(true ,this ,true);
                }
            }
            else
            {
                Player.Margin = new Thickness(30,30, 30, 30);
                WindowState = WindowState.Normal;

                if (m_lft != null)
                {
                    m_lft.ShowMyBar(true ,this ,false);
                }
                //m_lft.Left = this.Left + this.Width - 94;
                //m_lft.Top = this.Top ;
            }
        }
        public void OnBuffer(int width, int height, int isVertical, int bkSize)
        {
            if (_isVertical != isVertical && _isFull == false)
            {
                _isVertical = isVertical;
                if (Dispatcher.CheckAccess())
                {
                    SwitchOrientation();
                }
                else
                {
                    Dispatcher.BeginInvoke(new Action(SwitchOrientation));
                }
            }
        }

       public  bool nowstate()
        {
               Show();
               if(_isFull == true)
               {
                   WindowState = WindowState.Maximized;
               }
               else {
                   WindowState = WindowState.Normal;
               }
               Activate();
            return true;
        }


        void SwitchOrientation()
        {
            if (_isFull)
            {
                _isVertical = -1;
                return;
            }
            if (_isVertical == 1)
            {
                //Height = 680;// 711;
                //Width = 352;

                Height = 656;// 711;
                Width = 396;


                if (m_lft != null)
                {
                    m_lft.ShowMyBar(true, this, false);
                }
            }
            else
            {
                //Height = 352;
                //Width = 680;//711;

                Height = 396;// 711;
                Width = 656;
                
                if (m_lft != null)
                {
                    m_lft.ShowMyBar(true, this, false);
                }
            }
        }
        #endregion
        private void Grid_MouseLeftButtonDown(object sender, MouseButtonEventArgs e)
        {
            if (m_lft != null)
            {
                m_lft.ShowMyBar(false, this, false);
            }
            DragMove();
            if (m_lft != null)
            {
                m_lft.ShowMyBar(false, this, false);
            }

          //  Debug.WriteLine("Grid_MouseLeftButtonDown AAAAAAAAAA\n");
        }
        protected override void OnTextInput(TextCompositionEventArgs e)
        {
            base.OnTextInput(e);
            Player.OnText(e.Text);
        }

        protected override void OnKeyDown(KeyEventArgs e)
        {
            {
                byte[] data = new byte[8];

                data[0] = 0; // keydown
                data[1] = 0;
                data[2] = 0;
                data[3] = 0;
                byte[] KeyCode = BitConverter.GetBytes((int)e.Key);
                KeyCode.CopyTo(data, 4);

                DataManager.sendKeyBoardData(data);
            }

            base.OnKeyDown(e);
            Player.OnKeyEvent(e);
        }

        protected override void OnKeyUp(KeyEventArgs e)
        {
            base.OnKeyUp(e);
            Player.OnKeyEvent(e);
        }

        private void PhoneView_OnSizeChanged(object sender, SizeChangedEventArgs e)
        {

            return;
            IntPtr hwnd = new System.Windows.Interop.WindowInteropHelper(this).Handle;
            if (_rgn != IntPtr.Zero)
            {
                NativeMethods.DeleteObject(_rgn);
                _rgn = IntPtr.Zero;
            }
            if (_isFull)
            {
                //   _rgn = NativeMethods.CreateRectRgn(0, 0, Convert.ToInt32(ActualWidth), Convert.ToInt32(ActualHeight));
            }
            else
            {
                _rgn = NativeMethods.CreateRoundRectRgn(0, 0, Convert.ToInt32(ActualWidth), Convert.ToInt32(ActualHeight), 50, 50);
            }

            NativeMethods.SetWindowRgn(hwnd, _rgn, true);
        }

        private void PhoneView_OnLoaded(object sender, RoutedEventArgs e)
        {
            var hwnd = new System.Windows.Interop.WindowInteropHelper(this).Handle;
            _rgn = NativeMethods.CreateRoundRectRgn(0, 0, Convert.ToInt32(ActualWidth), Convert.ToInt32(ActualHeight), 50, 50);
            NativeMethods.SetWindowRgn(hwnd, _rgn, true);
             
             m_lft =  FloatingToolBars.ShowToolBars(Window.GetWindow(this));

             if (m_lft != null)
             {
                 Player._lft = m_lft;

                 m_PhoneViewModel._llft = m_lft;
             }


            //Window pw = Window.GetWindow(m_flt);
        }

        private void PhoneView_OnUnloaded(object sender, RoutedEventArgs e)
        {
            if (_rgn != IntPtr.Zero)
            {
                NativeMethods.DeleteObject(_rgn);
                _rgn = IntPtr.Zero;
            }
        }
        private void ResizeGrip_MouseLeftButtonDown_1(object sender, MouseButtonEventArgs e)
        {
            if (e.ClickCount == 1)
            {
                var fe = sender as FrameworkElement;
                _resizer.ResizeDirection = ResizeDirection.SouthEast;
                _resizer.ResizeOrigin = fe.PointToScreen(Mouse.GetPosition(fe));
                _resizer.OriginalHeight = ActualHeight;
                _resizer.OriginalWidth = ActualWidth;
                fe.CaptureMouse();
                e.Handled = true;
            }
        }

        private void ResizeGrip_MouseLeftButtonUp_1(object sender, MouseButtonEventArgs e)
        {

            var fe = sender as FrameworkElement;
            Debug.Assert(resizeGrip != null);

            if (resizeGrip.IsMouseCaptured)
            {
                fe.ReleaseMouseCapture();
                e.Handled = true;
            }
        }

        private void ResizeGrip_MouseMove_1(object sender, MouseEventArgs e)
        {
            if ((sender as FrameworkElement).IsMouseCaptured)
            {
                var fe = sender as FrameworkElement;
                Point point = fe.PointToScreen(Mouse.GetPosition(fe));
                double widthDelta = 0;
                double heightDelta = 0;
                switch (_resizer.ResizeDirection)
                {
                    case ResizeDirection.NorthEast:
                        widthDelta = point.X - _resizer.ResizeOrigin.X;
                        heightDelta = _resizer.ResizeOrigin.Y - point.Y;
                        break;
                    case ResizeDirection.NorthWest:
                        widthDelta = _resizer.ResizeOrigin.X - point.X;
                        heightDelta = _resizer.ResizeOrigin.Y - point.Y;
                        break;
                    case ResizeDirection.SouthEast:
                        widthDelta = point.X - _resizer.ResizeOrigin.X;
                        Debug.WriteLine(" widthDelta = {0} point.X={1} ， _resizer.ResizeOrigin.X={2} ", widthDelta, point.X, _resizer.ResizeOrigin.X);

                        heightDelta = point.Y - _resizer.ResizeOrigin.Y;

                        //         Debug.WriteLine("heightDelta:= {0} = point.Y ={1} - _resizer.ResizeOrigin.Y ={2};  ", heightDelta, point.Y, _resizer.ResizeOrigin.Y);

                        break;
                    case ResizeDirection.SouthWest:
                        widthDelta = _resizer.ResizeOrigin.X - point.X;
                        heightDelta = point.Y - _resizer.ResizeOrigin.Y;
                        break;
                    default:
                        Debug.Fail("Unexpected ResizeDirection: " + _resizer.ResizeDirection);
                        break;
                }
                _resizer.ResizeToWidth = Math.Max(0, _resizer.OriginalWidth + widthDelta);
                _resizer.ResizeToHeight = Math.Max(0, _resizer.OriginalHeight + heightDelta);
                FireOnResize(_resizer);
                e.Handled = true;
            }
        }

        private void FireOnResize(Resizer resizer)
        {
			if (m_lft != null)
			{
				m_lft.ShowMyBar(false, this, false);
			}
            
            double SW = SystemParameters.PrimaryScreenWidth;
            double SH = SystemParameters.PrimaryScreenHeight;

            //int SH = Screen.PrimaryScreen.Bounds.Height;
            //int SW = Screen.PrimaryScreen.Bounds.Width;

            double Height1 = resizer.ResizeToHeight;
            if (Height1 > SH * 9 / 10)
                Height1 = SH * 9 / 10;

            double Width1 = 0;
            if (_isVertical == 1) // ActualWidth > ActualHeight )
            {
                if (Height1 < 580)
                    Height1 = 580;
                Width1 = Height1 * 396 / 656;
                if (Width1 > SW * 9 / 10)
                {
                    Width1 = SW * 9 / 10;
                    Height1 = Width1 * 656 / 396;
                }
                if (Width1 < 100)
                {
                    Width1 = 100;
                    Height1 = Width1 * 656 / 396;
                }
            }
            else
            {
                if (Height1 < 300)
                    Height1 = 300;

                Width1 = Height1 * 656 / 396;
                if (Width1 > SW * 9 / 10)
                {
                    Width1 = SW * 9 / 10;
                    Height1 = Width1 * 396 / 656;
                }
                if (Width1 < 100)
                {
                    Width1 = 100;
                    Height1 = Width1 * 396 / 656; 
                }
            }
            Width = Width1;
            Height = Height1;
            //setrgn


            //m_lft.Left = this.Left + this.Width - 94;
            //m_lft.Top = this.Top ;


            Debug.WriteLine("FireOnResize width:{0}, height:{1} ,this.width={2} ,this.height={3}", ActualWidth, ActualHeight, Width1, Height1);
        }

        private void UIElement_OnMouseEnter(object sender, MouseEventArgs e)
        {
            var fe = sender as FrameworkElement;
            if (fe == null)
                return;
            fe.Opacity = 1.0;
        }
        private void UIElement_OnMouseLeave(object sender, MouseEventArgs e)
        {
            var fe = sender as FrameworkElement;
            if (fe == null)
                return;
            fe.Opacity = 0.0;
        }
    }
}
