using System;
using System.Diagnostics;
using System.Drawing;
using System.Windows;
using System.Windows.Input;
using System.Windows.Interop;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using ScreenCast.ViewModels;
using PixelFormat = System.Drawing.Imaging.PixelFormat;

namespace ScreenCast.Views
{
    public class VideoPlayer : FrameworkElement
    {
        private Bitmap _bitmap;
        private PhoneViewModel _vm;
        unsafe public VideoPlayer()
        {
            Focusable = true;
            Loaded += VideoPlayer_Loaded;
            Unloaded += VideoPlayer_Unloaded;
            _callback = OnGetBuffer;
            MouseLeftButtonDown += VideoPlayer_MouseLeftButtonDown;
            MouseMove += OnMouseMove;
            MouseLeftButtonUp += OnMouseLeftButtonUp;
            MouseRightButtonDown += OnMouseRightButtonDown;
            MouseRightButtonUp += OnMouseRightButtonUp;
        }

        private void OnMouseRightButtonUp(object sender, MouseButtonEventArgs e)
        {
            var pt = e.GetPosition(this);
            //Debug.WriteLine("OnMouseRightButtonUp: width:{0}, height:{1}, x:{2}, y{3}", ActualWidth, ActualHeight, pt.X, pt.Y);
            if (Program.IsRun)
                DR2API.DR2_OnMouseRightButtonUp(ViewModelBase.ApiHandle, (int)ActualWidth, (int)ActualHeight, (int)pt.X, (int)pt.Y);
        }

        private void OnMouseRightButtonDown(object sender, MouseButtonEventArgs e)
        {
            var pt = e.GetPosition(this);
            //Debug.WriteLine("OnMouseRightButtonDown: width:{0}, height:{1}, x:{2}, y{3}", ActualWidth, ActualHeight, pt.X, pt.Y);
            if (Program.IsRun)
                DR2API.DR2_OnMouseRightButtonDown(ViewModelBase.ApiHandle, (int)ActualWidth, (int)ActualHeight, (int)pt.X, (int)pt.Y);
        }

        private void OnMouseLeftButtonUp(object sender, MouseButtonEventArgs e)
        {
            var pt = e.GetPosition(this);
            //Debug.WriteLine("OnMouseLeftButtonUp: width:{0}, height:{1}, x:{2}, y{3}", ActualWidth, ActualHeight, pt.X, pt.Y);
            if (Program.IsRun)
                DR2API.DR2_OnMouseLeftButtonUp(ViewModelBase.ApiHandle, (int)ActualWidth, (int)ActualHeight, (int)pt.X, (int)pt.Y);
        }

        private void OnMouseMove(object sender, MouseEventArgs e)
        {
            var pt = e.GetPosition(this);
            //Debug.WriteLine("OnMouseMove: width:{0}, height:{1}, x:{2}, y{3}", ActualWidth, ActualHeight, pt.X, pt.Y);
            if (Program.IsRun)
                DR2API.DR2_OnMouseMove(ViewModelBase.ApiHandle, (int)ActualWidth, (int)ActualHeight, (int)pt.X, (int)pt.Y);
        }

        public void OnText(string text)
        {
            Trace.WriteLine("OnText:" + text);
            if (Program.IsRun)
                DR2API.DR2_OnText(ViewModelBase.ApiHandle, text);
        }

        void VideoPlayer_MouseLeftButtonDown(object sender, MouseButtonEventArgs e)
        {
            if (e.ClickCount == 2)
            {
                if (_vm != null)
                {
                    _vm.FullScreenCommand.Execute(null);
                    return;
                }
            }
            var pt = e.GetPosition(this);
            //Debug.WriteLine("OnMouseLeftButtonDown: width:{0}, height:{1}, x:{2}, y{3}", ActualWidth, ActualHeight, pt.X, pt.Y);
            if (Program.IsRun)
                DR2API.DR2_OnMouseLeftButtonDown(ViewModelBase.ApiHandle, e.ClickCount, (int) ActualWidth, (int) ActualHeight, (int) pt.X, (int) pt.Y);
            e.Handled = true;
        }

        unsafe void VideoPlayer_Unloaded(object sender, RoutedEventArgs e)
        {
            ViewModelBase.OnBuffer -= _callback;
        }

        unsafe void VideoPlayer_Loaded(object sender, RoutedEventArgs e)
        {
            ViewModelBase.OnBuffer += _callback;
            _vm = DataContext as PhoneViewModel;
        }
        private readonly DR2API.OnBufferCallback _callback;
        private IntPtr _wnd;
        public IntPtr WindowHandle
        {
            get
            {
                if (_wnd != IntPtr.Zero)
                    return _wnd;
                var wndHelper = new WindowInteropHelper(Window.GetWindow(this));
                _wnd = wndHelper.Handle;
                return _wnd;
            }
        }
        protected override void OnRender(DrawingContext dc)
        {
            if (ActualHeight > 0 && ActualWidth > 0 && ViewModelBase.ApiHandle != IntPtr.Zero)
            {
                if (_bitmap == null)
                {
                    _bitmap = new Bitmap((int)ActualWidth, (int)ActualHeight, PixelFormat.Format24bppRgb);
                }
                if (_bitmap != null && (_bitmap.Height != ActualHeight || _bitmap.Width != ActualWidth))
                {
                    _bitmap.Dispose();
                    _bitmap = new Bitmap((int)ActualWidth, (int)ActualHeight, PixelFormat.Format24bppRgb);
                }
                using (var g = Graphics.FromImage(_bitmap))
                {
                    if (Program.IsRun)
                    {
                        DR2API.DR2_Draw(ViewModelBase.ApiHandle, WindowHandle, g.GetHdc(), new RECT(0, 0, (int) ActualWidth, (int) ActualHeight),
                            ActualWidth == Window.GetWindow(this).ActualWidth ? 1 : 0);
                    }
                }
                dc.DrawImage(ToBitmapSource(_bitmap), new Rect(0, 0, ActualWidth, ActualHeight));
                GC.Collect();
            }
        }

        unsafe void OnGetBuffer(byte* buffer, int width, int height, int isVertical, int bkSize)
        {
            if (_vm == null || _vm.IsChangeFullScrenn)
                return;
            bool show = buffer != null;
            if (Dispatcher.CheckAccess())
            {
                OnGetBuffer(show, width, height, isVertical, bkSize);
            }
            else
            {
                Dispatcher.BeginInvoke(new Action(() => OnGetBuffer(show, width, height, isVertical, bkSize)));
            }
        }

        void OnGetBuffer(bool show, int width, int height, int isVertical, int bkSize)
        {
            if (_vm != null)
            {
                _vm.OnGetBuffer(show, width, height, isVertical, bkSize);
            }
            InvalidateVisual();
        }
        public BitmapSource ToBitmapSource(Bitmap bitmap)
        {
            var ip = bitmap.GetHbitmap();
            var bitmapSource = Imaging.CreateBitmapSourceFromHBitmap(ip, IntPtr.Zero, Int32Rect.Empty, BitmapSizeOptions.FromEmptyOptions());
            NativeMethods.DeleteObject(ip);
            return bitmapSource;
        }
    }
}
