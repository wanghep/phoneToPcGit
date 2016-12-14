using System;
using System.Diagnostics;
using System.Drawing;
using System.IO;
using System.Net.Cache;
using System.Reflection;
using System.Windows;
using System.Windows.Forms;
using System.Windows.Input;
using System.Windows.Media.Animation;
using ScreenCast.Models;
using ScreenCast.ViewModels;
using Application = System.Windows.Application;
using MouseEventArgs = System.Windows.Forms.MouseEventArgs;
using ScreenCast.Commands;
using System.Windows.Controls;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using Microsoft.Win32;
using ScreenCast.Controls;
using Brushes = System.Windows.Media.Brushes;
using Environment = System.Environment;
using System.Runtime.InteropServices;
using ZXing;
using System.Net;
using ZXing.Common;
using ZXing.QrCode;


namespace ScreenCast.Views
{
	/// <summary>
	/// MainWindow.xaml 的交互逻辑
	/// </summary>
	public partial class MainWindow
	{

        public static  MainWindow g_MainWindow = null;
		private DeviceState _lastState = DeviceState.Idle;
		private Window _win;
		private NotifyIcon _notifyIcon;
		//private Window _wnd;
		private bool _exiting;
	    private MainViewModel _vm;

       // private ConnectPageModel _cm;
		private PhoneView pvw;

		public System.Windows.Forms.MenuItem  _autoRun;

        public System.Windows.Forms.MenuItem _ShowFirstPage;

        //private Connecting m_WndConnect;


        public string  _isLangUS;

        public bool _ischaged;

        public MainWindow(string sLang, bool isPcWhenOpen)
		{
			InitializeComponent();

            //if (isLangUS == true)
            if (string.Compare(sLang, "en-US", true) == 0)
            {
                ImgPCtoPc.Source = PCtoPc.Source;
                ImgPCtoPc.Width = PCtoPc.Width;
                ImgPCtoPc.Height = PCtoPc.Height;

                ImgPhoneToPc.Source = PhonetoPc.Source;
                ImgPhoneToPc.Width = PhonetoPc.Width;
                ImgPhoneToPc.Height = PhonetoPc.Height;
            }

            g_MainWindow = this;
            _ischaged = false;

            _isLangUS = sLang;

			_vm = new MainViewModel(this);
            DataContext = _vm;
// 			var sb = FindResource("Storyboard") as Storyboard;
// 			if (sb != null)
// 				sb.Begin();

			PCConnet.Visibility = Visibility.Visible;
			PhoneConnet.Visibility = Visibility.Collapsed;

            connecterr.Visibility = Visibility.Collapsed;
            connecting.Visibility = Visibility.Collapsed; 

			CreateNotifyIcon();

            if (isPcWhenOpen == true)
            {
                if (Settings.Current.ShowFirstPage)
                {
                    Show();
                }
                else
                    Hide();
            }
            else
                Show();

            //ximu***
            //create erweima ...
            //create2weima();
            //ximu***

            CheckBoxAutoRun.IsChecked = AutoLaunch;
            CheckBoxShowFirstPage.IsChecked = Settings.Current.ShowFirstPage;





// 
// 			if (Settings.Current.ShowFirstPage)
// 			{
// 				Hide();
// 				_wnd = new SplashWindow();
// 				_wnd.Show();
// 				_wnd.Closed += (sender, args) =>
// 				{
// 					try
// 					{
// 						if (!_exiting)
// 							this.Show();
// 					}
// 					catch (Exception ex)
// 					{
// 						Trace.WriteLine(ex);
// 					}
// 					_wnd = null;
// 				};
// 			}
// 			else
// 			{
// 				Show();
// 			}
		}

        #region 2weima

        //// 回收对象
        [DllImport("gdi32")]
        static extern int DeleteObject(IntPtr o);

        /** * 创建二维码图片 */
        private ImageSource createQRCode(String content, int width, int height)
        {
            EncodingOptions options;
            //包含一些编码、大小等的设置 
            BarcodeWriter write = null;
            //用来生成二维码，对应的BarcodeReader用来解码 
            options = new QrCodeEncodingOptions
            {
                DisableECI = true,
                CharacterSet = "UTF-8",
                Width = width,
                Height = height,
                Margin = 0
            };

            write = new BarcodeWriter();
            write.Format = BarcodeFormat.QR_CODE;
            write.Options = options;
            Bitmap bitmap = write.Write(content);

            IntPtr ip = bitmap.GetHbitmap();
            BitmapSource bitmapSource = System.Windows.Interop.Imaging.CreateBitmapSourceFromHBitmap
            (
                ip,
                IntPtr.Zero,
                Int32Rect.Empty,
                System.Windows.Media.Imaging.BitmapSizeOptions.FromEmptyOptions()
             );

            DeleteObject(ip);

            return bitmapSource;
        }

        private void create2weima()
        {
            IPHostEntry ihe = Dns.GetHostByName(Dns.GetHostName());
            IPAddress ipa = ihe.AddressList[0];
            string content = ipa.ToString() + "~8888";
            //string content = ipa.ToString();
            ImageSource imgs = createQRCode(content, 150, 150);
            //给控件source属性赋值
            //erweimaImg.Source = imgs;


        }


        #endregion

        private void CreateNotifyIcon()
		{
			_notifyIcon = new NotifyIcon { Text = Title };
            var stream = Assembly.GetEntryAssembly().GetManifestResourceStream("ScreenCast.app.ico");
			if (stream != null)
			{
				_notifyIcon.Icon = new Icon(stream);
			}
			_notifyIcon.Visible = true;
			_notifyIcon.MouseClick += NotifyIconOnMouseClick;
            _notifyIcon.MouseMove+= NotifyIconOnMouseMove;
            
			_autoRun = new System.Windows.Forms.MenuItem(App.GetString("StartWhenOpen"), OnAutoRunClick)
            {
                Checked = AutoLaunch,
            };
            _ShowFirstPage = new System.Windows.Forms.MenuItem(App.GetString("ShowFirstPage"), OnCheckBoxShowFirstPage)
            {
                Checked = Settings.Current.ShowFirstPage,
            };

			var menus = new[]
            {
               _autoRun,
               _ShowFirstPage,
//                 new System.Windows.Forms.MenuItem(App.GetString("ShowFirstPage"), (sender, args) =>
//                 {
//                     var mi = (sender as System.Windows.Forms.MenuItem);
//                     mi.Checked = !mi.Checked;
//                     Settings.Current.ShowFirstPage = mi.Checked;
// 
//                 }) {Checked = Settings.Current.ShowFirstPage},

                new System.Windows.Forms.MenuItem(App.GetString("Disconnect"), OnDisConnectClick),
                //new System.Windows.Forms.MenuItem(App.GetString("ShowNormal"), OnShowNormalClick),
                new System.Windows.Forms.MenuItem(App.GetString("Exit"), OnExitClick),
            };

			_notifyIcon.ContextMenu = new System.Windows.Forms.ContextMenu(menus);
		}
        private void NotifyIconOnMouseMove(object sender, MouseEventArgs e)
        {
            _autoRun.Checked = AutoLaunch;
        }





		private void OnAutoRunClick(object sender, EventArgs eventArgs)
		{
			AutoLaunch = !AutoLaunch;
			_autoRun.Checked = AutoLaunch;
		    CheckBoxAutoRun.IsChecked = AutoLaunch;
		}

        private void OnCheckBoxShowFirstPage(object sender, EventArgs e)
        {
            bool SFP = Settings.Current.ShowFirstPage;
            Settings.Current.ShowFirstPage = !SFP;
            _ShowFirstPage.Checked = !SFP;
            CheckBoxShowFirstPage.IsChecked = !SFP;
        }



		private void OnExitClick(object sender, EventArgs e)
		{
			_exiting = true;
			_notifyIcon.Visible = false;
			Application.Current.Shutdown();
		}


		private void OnDisConnectClick(object sender, EventArgs e)
		{
			if (Program.IsRun)
				DR2API.DR2_Click(ViewModelBase.ApiHandle, 2);
		}
		//开机自动 开启 fun
	
        

	

		private void NotifyIconOnMouseClick(object sender, MouseEventArgs e)
		{
            //Debug.Write("left click  icon\n");
            if (_win == null)
            {
                Show();
                string strexe = Environment.CurrentDirectory;

                string pcpath = strexe + "\\pc.gif";
                if (File.Exists(pcpath))
                {
                    //存在
                    MediaElementPc.Source = new Uri(pcpath);
                }
                else
                    MediaElementPc.Source = null;


                string phonepath = strexe + "\\phone.gif";
                if (File.Exists(phonepath))
                {
                    MediaElementPhone.Source = new Uri(phonepath);
                }
                else
                    MediaElementPhone.Source = null;


                string connectpath = strexe + "\\connecting.gif";
                if (File.Exists(connectpath))
                {
                    MediaElementConnect.Source = new Uri(connectpath);
                }
                else
                    MediaElementConnect.Source = null;

                WindowState = WindowState.Normal;
            }
            else
            {
                pvw.nowstate();
            }
		}

		public void OnStateChanged(DeviceState state)
		{
			if (_lastState == state)
			{
				return;
			}
			_lastState = state;
			if (Dispatcher.CheckAccess())
			{
				SetNewState(state);
			}
			else
			{
				Dispatcher.BeginInvoke(new Action(() => SetNewState(state)));
			}
		}

		private void SetNewState(DeviceState state)
		{
			if (state == DeviceState.Connecting)
			{
                

                PCConnet.Visibility = Visibility.Collapsed;
                PhoneConnet.Visibility = Visibility.Collapsed;

                connecterr.Visibility = Visibility.Collapsed;
                connecting.Visibility = Visibility.Visible; 


			}
			else if (state == DeviceState.Connected   )
			{
				Hide();
               

				if (ViewModelBase.IsMobile)
				{
					pvw = new PhoneView();
					_win = pvw;
				}
				else
				{
					_win = new PcView();
				}
				_win.Closed += (sender, args) => ViewModelBase.Stop();
				_win.Show();
			}
			else
			{
				if (_win != null)
				{
					_win.Close();
					_win = null;
				}
                Show();

                PCConnet.Visibility = Visibility.Collapsed;
                PhoneConnet.Visibility = Visibility.Collapsed;

                connecterr.Visibility = Visibility.Visible;// Collapsed;
                connecting.Visibility = Visibility.Collapsed; ; 

			}
		}

        private void OnClickMainPageChange1(object sender, RoutedEventArgs e)
        {
                PCConnet.Visibility = Visibility.Visible;
                PhoneConnet.Visibility = Visibility.Collapsed;

                connecterr.Visibility = Visibility.Collapsed;
                connecting.Visibility = Visibility.Collapsed; 

        }
        private void OnClickMainPageChange2(object sender, RoutedEventArgs e)
        {
                PCConnet.Visibility = Visibility.Collapsed;
                PhoneConnet.Visibility = Visibility.Visible;

                connecterr.Visibility = Visibility.Collapsed;
                connecting.Visibility = Visibility.Collapsed; 
        }

		private void Grid_MouseLeftButtonDown(object sender, MouseButtonEventArgs e)
		{
			DragMove();
		}

		private void MainWindow_OnClosed(object sender, EventArgs e)
		{
			OnExitClick(this, e);
		}

		private void ImageButton_Back(object sender, RoutedEventArgs e)
		{
			MainWindow_OnClosed(sender, e);
		}

		private void OnMinClickMin(object sender, RoutedEventArgs e)  //最小化
		{

			if (_win == null)
			{
				Hide();
			    MediaElementPc.Source = null;
			    MediaElementConnect.Source = null;
			    MediaElementPhone.Source = null;
			}
			else
			{                  
				//_win.Show();
				_win.Hide();
			}
		}

		private void OnClickMinExit(object sender, RoutedEventArgs e) // 退出
		{
			MainWindow_OnClosed(sender, e);
		}


//         private void OnClickMenuCommand(object sender, RoutedEventArgs e)
//         {
// 
//             var dlg = new AboutMenu
//             {
//                 DataContext = this,
//                 WindowStartupLocation = WindowStartupLocation.Manual,
//                 Left =  this.Left + 509,
//                 Top = this.Top +38,
//             };
//             
//             dlg.Owner = Window.GetWindow(this);
//             dlg.ShowDialog();
// 
//         }


		private void OnShowNormalClick(object sender, EventArgs e)
		{

			if (_win == null)
			{
				Show();
				WindowState = WindowState.Normal;
			}
			else
			{
				pvw.nowstate();
			}
		}


		private void MediaElement_MediaEnded_PC(object sender, System.Windows.RoutedEventArgs e)
		{
			//((MediaElement)sender).Position = ((MediaElement)sender).Position.Add(TimeSpan.FromMilliseconds(1));
            MediaElementPc.Position = ((MediaElement)sender).Position.Add(TimeSpan.FromMilliseconds(1));

	
            
		}
        private void MediaElement_MediaEnded_Phone(object sender, RoutedEventArgs e)
        {
            MediaElementPhone.Position = ((MediaElement)sender).Position.Add(TimeSpan.FromMilliseconds(1));
           // MediaElementPhone.Close();
        }

        private void MediaElement_MediaEnded_Connect(object sender, RoutedEventArgs e)
        {
            MediaElementConnect.Position = ((MediaElement)sender).Position.Add(TimeSpan.FromMilliseconds(1));
          //  MediaElementConnect.Close();
        }

		#region 属性

		public bool AutoLaunch
		{
			get
			{
				bool ret = false;
				try
				{
					var reg = Registry.CurrentUser.OpenSubKey(@"Software\Microsoft\Windows\CurrentVersion\Run");
					if (reg != null)
					{
						var obj = reg.GetValue(Assembly.GetEntryAssembly().GetName().Name);

                        string objstring = "\"" + Assembly.GetEntryAssembly().Location + "\"" + " /1";

                        if (obj != null && string.Compare(obj.ToString(), objstring) == 0)
							ret = true;
						reg.Close();
					}
				}
				catch (Exception e)
				{
					Trace.WriteLine(e);

				}
				return ret;
			}
			set
			{
				try
				{
					var reg = Registry.CurrentUser.CreateSubKey(@"Software\Microsoft\Windows\CurrentVersion\Run");
					if (reg != null)
					{
						if (value)
						{
                            string objstring = "\"" + Assembly.GetEntryAssembly().Location + "\"" + " /1";

							reg.SetValue(Assembly.GetEntryAssembly().GetName().Name, objstring );
						}
						else
						{
							reg.DeleteValue(Assembly.GetEntryAssembly().GetName().Name);
						}
					}
				}
				catch (Exception e)
				{
					Trace.WriteLine(e);
				}
			}
		}
		#endregion

	    private void MainWindow_OnLoaded(object sender, RoutedEventArgs e)
	    {
	        _vm.LoadCommand.Execute(null);
	    }

	    private void MainWindow_OnUnloaded(object sender, RoutedEventArgs e)
	    {
	        _vm.UnloadCommand.Execute(null);
	    }



	    private void OnClickMenuPopup(object sender, RoutedEventArgs e)
	    {
            //调整位置
            PopupMenu.HorizontalOffset = -143;
            PopupMenu.VerticalOffset = 6;
            //显示 menu
            PopupMenu.IsOpen = true;
        }

        private void OnShowButtonNextLeft(object sender, System.Windows.Input.MouseEventArgs e)
        {
            ButtonNextLeft.NormalImage = ButtonNextLeft.HoverImage;
        }



	    private void OnShowButtonNextLeftLeave(object sender, System.Windows.Input.MouseEventArgs e)
        {
            ButtonNextLeft.NormalImage = ButtonNextLeft.PushImage;
        }

        private void OnShowButtonNextRight(object sender, System.Windows.Input.MouseEventArgs e)
        {
            ButtonNextRight.NormalImage = ButtonNextRight.HoverImage;
        }

        private void OnShowButtonNextRightLeave(object sender, System.Windows.Input.MouseEventArgs e)
        {
            ButtonNextRight.NormalImage = ButtonNextRight.PushImage;
        }

        private void OnMenuClickHelp(object sender, RoutedEventArgs e)
        {
            if (Program.IsRun)
                DR2API.DR2_Click(ViewModelBase.ApiHandle, 4, _isLangUS);
        }

        private void OnMenuClickEnter(object sender, System.Windows.Input.MouseEventArgs e)
        {
            ((ImageButton)sender).Foreground = Brushes.White; 
            //ButtonMenuClickHelp.Foreground = Brushes.White; 
        }

        private void OnMenuClickLeave(object sender, System.Windows.Input.MouseEventArgs e)
        {
            ((ImageButton)sender).Foreground = Brushes.Gray; 
            //ButtonMenuClickHelp.Foreground = Brushes.Gray; 
        }

    

	}
}

