using System;
using System.Diagnostics;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Forms.Integration;
using System.Windows.Input;
using ScreenCast.ViewModels;
using System.Windows.Threading;

namespace ScreenCast.Views
{
    class VideoPlayerHost : ContentControl
    {
        internal PhoneViewModel _vm;
        private readonly DR2API.OnBufferCallback _callback;
        private VideoPlayerForm _form;

        public FloatingToolBars _lft;

        //new add
        public int isVertical = 1;

       // public DispatcherTimer _mTimer; 
        public unsafe VideoPlayerHost()
        {
            Loaded += VideoPlayer_Loaded;
            Unloaded += VideoPlayer_Unloaded;
            _callback = OnGetBuffer;
            _form = new VideoPlayerForm(this);
            var host = new WindowsFormsHost { Child = _form };
            Content = host;

            _lft = null;
        }

        public void OnText(string text)
        {
            Trace.WriteLine("OnText:" + text);
            if(Program.IsRun)
                DR2API.DR2_OnText(ViewModelBase.ApiHandle, text);
        }

        public void OnKeyEvent(KeyEventArgs e)
        {
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
            // new add
            this.isVertical = isVertical;

            if (_vm != null)
            {
                _vm.OnGetBuffer(show, width, height, isVertical, bkSize);
            }
            if (_form != null && show)
            {
                _form.Invalidate();
            }
        }

        //new add
        public int getIsVertical()
        {
            return isVertical;
        }
    }
}
