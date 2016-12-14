using System;
using System.Windows;
using System.Diagnostics;
using System.Drawing;
using System.Windows.Forms;
using ScreenCast.ViewModels;
using System.Windows.Threading;
using ScreenCast.Client;

namespace ScreenCast.Views
{
    class VideoPlayerForm : Label
    {
        private Bitmap _bitmap;
        private VideoPlayerHost _host;

        private int m_x;
        private int m_y;
       // private int oriWidth = 0;
       // private int oriheight = 0;
        static private float PHONE_WIDTH_FIRST_SCREEN = 720; //1280 ; 1440;
        static private float PHONE_HEIGHT_FIRST_SCREEN = 1280; //720; 2560;

        static private float PHONE_WIDTH_SECOND_SCREEN = 864; //1280 ; 1440;
        static private float PHONE_HEIGHT_SECOND_SCREEN = 480; //2560;

        static private float PHONE_WIDTH = 864; //1280 ; 1440;
        static private float PHONE_HEIGHT = 480; //720; 2560;
        
        static private float BLACK_MAGIN_TOP = 45;
        static private float BLACK_MAGIN_BOTTOM = 582;
        
        public DispatcherTimer _mainTimer; 
        
        public VideoPlayerForm(VideoPlayerHost host)
        {
            if (Program.IsRun)
                InitializeComponent();
            _host = host;
            if (Program.IsRun)
            {
                DR2API.DR2_SetDrawWnd(ViewModelBase.ApiHandle, Handle );
            }

            //设置定时器  3 秒一次
            _mainTimer = new DispatcherTimer();
            _mainTimer.Interval = TimeSpan.FromSeconds(3);
            _mainTimer.Tick += new EventHandler(dTimer_Tick);
            _mainTimer.IsEnabled = true;
            _mainTimer.Start();
        }

        private void dTimer_Tick(object sender, EventArgs e)
        {

            if(_host._lft != null)
            {
                //Debug.WriteLine("timer in ");
                //_lft.ShowMyBar(false, this, false);
                _host._lft.Hide();
                _mainTimer.Stop();
            }
            
        }


        protected override void OnMouseDoubleClick(MouseEventArgs e)
        {
            if (_host._vm != null)
            {
                _host._vm.FullScreenCommand.Execute(this);
            }
        }

        private void InitializeComponent()
        {
            SuspendLayout();
            // 
            // VideoPlayerForm
            // 
            //oriheight = 630;
            //oriWidth = 302;
            MouseDown += VideoPlayerForm_MouseDown;
            MouseMove += VideoPlayerForm_MouseMove;
            MouseUp += VideoPlayerForm_MouseUp;
            ResumeLayout(false);
        }

        private void VideoPlayerForm_MouseDown(object sender, MouseEventArgs e)
        {
            Trace.WriteLine("MouseDown: X = " + e.X +"; Y = " + e.Y);
           // Trace.WriteLine("oriWidth: = " + oriWidth + " ; oriheight = " + oriheight);
            Trace.WriteLine("VideoPlayerFormWidth: = " + this.Width + " ; VideoPlayerFormHeight = " + this.Height);
            RECT in_rect = new RECT(1, 1, 1, 1);
            RECT out_rect = new RECT(1, 1, 1, 1);

            DR2API.DR2_GetDrawRect(ViewModelBase.ApiHandle,  ref in_rect,  ref out_rect );


            Trace.WriteLine("in_rect:  " + in_rect.left + " " + in_rect.top + " " +in_rect.right + " " +in_rect.bottom + " ");
            Trace.WriteLine("out_rect: " + out_rect.left + " " + out_rect.top + " " + out_rect.right + " " + out_rect.bottom + " ");

            
            int in_X = (int)e.X;
            int in_Y = (int)e.Y;

            int isVertical = _host.getIsVertical();
           

            if (isVertical == 1)
            {
                VideoPlayerForm.PHONE_HEIGHT = VideoPlayerForm.PHONE_HEIGHT_FIRST_SCREEN;
                VideoPlayerForm.PHONE_WIDTH = VideoPlayerForm.PHONE_WIDTH_FIRST_SCREEN;
            }
            else
            {
                VideoPlayerForm.PHONE_HEIGHT = VideoPlayerForm.PHONE_HEIGHT_SECOND_SCREEN;
                VideoPlayerForm.PHONE_WIDTH = VideoPlayerForm.PHONE_WIDTH_SECOND_SCREEN;

            }
            
            
            var pt = e.Location;
            if (e.Button == MouseButtons.Left)
            {
                { 
                    byte[] data = new byte[12];

                    data[0] = 0; // mouseDown
                    data[1] = 0;
                    data[2] = 0;
                    data[3] = 0;

                    int sendX = 0;
                    sendX = (int)((float)((in_X) - out_rect.left) * (VideoPlayerForm.PHONE_WIDTH / (float)(out_rect.Width)));
                    
                    byte[] X = BitConverter.GetBytes(sendX);

                    X.CopyTo(data, 4);

                    int sendY = 0;
                    sendY = (int)((float)((in_Y) - out_rect.top) * (VideoPlayerForm.PHONE_HEIGHT / (float)(out_rect.Height))); ;
                    
                    byte[] Y = BitConverter.GetBytes(sendY);
                    Y.CopyTo(data, 8);

                    if ((sendX >= 0 && (sendX < VideoPlayerForm.PHONE_WIDTH )) && (sendY >= 0 && (sendY < VideoPlayerForm.PHONE_HEIGHT)))
                    {
                        Trace.WriteLine("send mouse down : X = " + sendX + "; Y = " + sendY);
                        DataManager.sendMouseData(data);
                    }
                    else
                    {
                        Trace.WriteLine(" invalid mouse down : X = " + sendX + "; Y = " + sendY);

                    }
                    
                }

                //Debug.WriteLine("OnMouseLeftButtonDown: width:{0}, height:{1}, x:{2}, y{3}", Width, Height, pt.X, pt.Y);
                if (Program.IsRun)
                    DR2API.DR2_OnMouseLeftButtonDown(ViewModelBase.ApiHandle, e.Clicks, Width, Height, pt.X, pt.Y);
            }
            else if (e.Button == MouseButtons.Right)
            {
                //Debug.WriteLine("OnMouseRightButtonDown: width:{0}, height:{1}, x:{2}, y{3}", Width, Height, pt.X, pt.Y);
                if (Program.IsRun)
                    DR2API.DR2_OnMouseRightButtonDown(ViewModelBase.ApiHandle, Width, Height, pt.X, pt.Y);
            }
        }

        private void VideoPlayerForm_MouseMove(object sender, MouseEventArgs e)
        {
            var pt = e.Location;
            //Debug.WriteLine("OnMouseMove: width:{0}, height:{1}, x:{2}, y{3}", Width, Height, pt.X, pt.Y);
            if (Program.IsRun)
            {
                DR2API.DR2_OnMouseMove(ViewModelBase.ApiHandle, Width, Height, pt.X, pt.Y);

                if (_host._lft != null && m_x != pt.X  && m_y != pt.Y)
                {
                    m_x = pt.X  ;
                    m_y = pt.Y ;

                    _mainTimer.Start();
                    _host._lft.ftbname.Opacity = 1.0;
                    _host._lft.Show();
                }
            }

        }

        private void VideoPlayerForm_MouseUp(object sender, MouseEventArgs e)
        {
            Trace.WriteLine("MouseUp: X = " + e.X + "; Y = " + e.Y);

            RECT in_rect = new RECT(1, 1, 1, 1);
            RECT out_rect = new RECT(1, 1, 1, 1);

            DR2API.DR2_GetDrawRect(ViewModelBase.ApiHandle, ref in_rect, ref out_rect);


            Trace.WriteLine("in_rect:  " + in_rect.left + " " + in_rect.top + " " + in_rect.right + " " + in_rect.bottom + " ");
            Trace.WriteLine("out_rect: " + out_rect.left + " " + out_rect.top + " " + out_rect.right + " " + out_rect.bottom + " ");



            int in_X = (int)e.X;
            int in_Y = (int)e.Y;

            int isVertical = _host.getIsVertical();

            if (isVertical == 1)
            {
                VideoPlayerForm.PHONE_HEIGHT = VideoPlayerForm.PHONE_HEIGHT_FIRST_SCREEN;
                VideoPlayerForm.PHONE_WIDTH = VideoPlayerForm.PHONE_WIDTH_FIRST_SCREEN;
            }
            else
            {
                VideoPlayerForm.PHONE_HEIGHT = VideoPlayerForm.PHONE_HEIGHT_SECOND_SCREEN;
                VideoPlayerForm.PHONE_WIDTH = VideoPlayerForm.PHONE_WIDTH_SECOND_SCREEN;

            }

            var pt = e.Location;
            if (e.Button == MouseButtons.Left)
            {
                
                {
                    byte[] data = new byte[12];

                    data[0] = 0; // // mouseMove
                    data[1] = 0;
                    data[2] = 0;
                    data[3] = 1;

                    int sendX = 0;
                    sendX = (int)((float)((in_X) - out_rect.left) * (VideoPlayerForm.PHONE_WIDTH / (float)(out_rect.Width)));

                    byte[] X = BitConverter.GetBytes(sendX);

                    X.CopyTo(data, 4);

                    int sendY = 0;
                    sendY = (int)((float)((in_Y) - out_rect.top) * (VideoPlayerForm.PHONE_HEIGHT / (float)(out_rect.Height))); ;

                    byte[] Y = BitConverter.GetBytes(sendY);
                    Y.CopyTo(data, 8);

                    if ((sendX >= 0 && (sendX < VideoPlayerForm.PHONE_WIDTH)) && (sendY >= 0 && (sendY < VideoPlayerForm.PHONE_HEIGHT)))
                    {
                        Trace.WriteLine("send mouse up : X = " + sendX + "; Y = " + sendY);
                        DataManager.sendMouseData(data);
                    }
                    else
                    {
                        Trace.WriteLine(" invalid mouse down : X = " + sendX + "; Y = " + sendY);

                    }
                }
                 

                //Debug.WriteLine("OnMouseLeftButtonUp: width:{0}, height:{1}, x:{2}, y{3}", Width, Height, pt.X, pt.Y);
                if (Program.IsRun)
                    DR2API.DR2_OnMouseLeftButtonUp(ViewModelBase.ApiHandle, Width, Height, pt.X, pt.Y);
            }
            else if (e.Button == MouseButtons.Right)
            {
                //Debug.WriteLine("OnMouseRightButtonUp: width:{0}, height:{1}, x:{2}, y{3}", Width, Height, pt.X, pt.Y);
                if (Program.IsRun)
                    DR2API.DR2_OnMouseRightButtonUp(ViewModelBase.ApiHandle, Width, Height, pt.X, pt.Y);
            }
        }

#if false
        protected override void OnPaint(PaintEventArgs e)
        {
            IntPtr hdc = e.Graphics.GetHdc();
            if(Program.IsRun)
                DR2API.DR2_Draw(ViewModelBase.ApiHandle, Handle, hdc, new RECT(0, 0, (int)Width, (int)Height), 0);
            e.Graphics.ReleaseHdc(hdc);
        }
#endif
    }
}
