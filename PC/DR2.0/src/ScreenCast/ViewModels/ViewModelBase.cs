using System;
using System.ComponentModel;
using System.Diagnostics;
using System.IO;
using System.Reflection;
using System.Windows;
using System.Windows.Input;
using System.Windows.Media.Imaging;
using ScreenCast.Commands;
using ScreenCast.Views;
using ScreenCast.ViewModels;
using System.Threading;
using ScreenCast.Views;

namespace ScreenCast.ViewModels
{
    class ViewModelBase : INotifyPropertyChanged
    {
        private FrameworkElement _fe;
        private static readonly unsafe DR2API.OnBufferCallback _onBuffer = OnDR2BufferCallback;
        public static event DR2API.OnBufferCallback OnBuffer;
        public static IntPtr ApiHandle { get; set; }
        public static bool IsMobile { get; set; }
        public static bool Started { get; private set; }

        private MainWindow m_MainWindow = null;

        #region INotifyPropertyChanged
        public event PropertyChangedEventHandler PropertyChanged;

        public void NotifyPropertyChanged(string propertyName)
        {
            PropertyChangedEventHandler handler = PropertyChanged;
            if (handler != null)
                handler(this, new PropertyChangedEventArgs(propertyName));
        }
        #endregion
        public ViewModelBase(FrameworkElement fe)
        {
            _fe = fe;

            m_MainWindow = MainWindow.g_MainWindow;
        }
        private static unsafe void OnDR2BufferCallback(byte* buffer, int width, int height, int isVertical, int bkSize)
        {
            if (OnBuffer != null)
                OnBuffer(buffer, width, height, isVertical, bkSize);
        }

        public static void Create()
        {
            if (Program.IsRun)
            {
                ApiHandle = DR2API.DR2_Create();
            }
        }

        public static unsafe int Start(string name, DR2API.OnDeviceStateChanged onState)
        {
            int ret = 0;
            if (Program.IsRun)
            {
                ret = DR2API.DR2_Start(ApiHandle, name, onState, _onBuffer);
            }
            if (ret == 0)
            {
                Started = true;
            }
            else
            {
                Trace.WriteLine("start failed");
                string errtring = App.GetString("WifiError");
                string captionstr = App.GetString("WifiWaring");
                MessageBoxButton button = MessageBoxButton.OK;
                MessageBoxImage icon = MessageBoxImage.Warning;
                MessageBox.Show(errtring, captionstr , button  , icon );
                System.Environment.Exit(0);
            }
            //ximu***
            Thread regThread = new Thread(startReg);
            regThread.IsBackground = true;
            regThread.Start();
            //ximu***
            return ret;
        }

        // open a new Thread to do this ,because this contain a while(true) 
        static void startReg()
        {
            DR2API.DR2_registSocketFun(ApiHandle, ScreenCast.Program.dm.run); // regist the function.
        }


        public static void Stop()
        {
            if (Program.IsRun)
            {
                DR2API.DR2_Click(ApiHandle, 2);
                DR2API.DR2_Stop(ApiHandle);
            }
            Started = false;
        }
        private ICommand _helpCommand;

        public ICommand HelpCommand
        {
            get { return _helpCommand ?? (_helpCommand = new DelegateCommand(OnHelpCommand)); }
        }
        private void OnHelpCommand()
        {
            var app = Application.Current as App;
            var s = string.Format("{1}\\helps\\{0}.chm", app.Lang, Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location));
            if (File.Exists(s))
            {
                try
                {
                    Process.Start(s);
                }
                catch (Exception ex)
                {
                    Trace.WriteLine(ex);
                }
                return;
            }
        }


        /// <summary>
        /// 关于 按钮
        /// </summary>
        private ICommand _aboutCommand;
        public ICommand AboutCommand
        {
            get { return _aboutCommand ?? (_aboutCommand = new DelegateCommand(OnAboutCommand)); }
        }
        private void OnAboutCommand()
        {
            var dlg = new AboutWindow
            {
                DataContext = this,
                WindowStartupLocation = WindowStartupLocation.CenterOwner,
            };
            if (_fe != null)
            {
                dlg.Owner = Window.GetWindow(_fe);
            }
            dlg.ShowDialog();
        }


        /// <summary>
        /// 关于 帮助窗口
        /// </summary>
        private ICommand _WindowHelpCommand;
        public ICommand WindowHelpCommand
        {
            get { return _WindowHelpCommand ?? (_WindowHelpCommand = new DelegateCommand(OnWindowHelpCommand)); }
        }
        private void OnWindowHelpCommand()
        {
            IntPtr hForeground = NativeMethods.GetForegroundWindow();
            System.Drawing.Rectangle rect = new System.Drawing.Rectangle();
            NativeMethods.GetWindowRect(hForeground, ref rect);

            var dlg = new WindowHelp
            {
                DataContext = this,
                WindowStartupLocation = WindowStartupLocation.Manual,

                Left = rect.Left + 50,
                Top = rect.Top + 50,

            };
            if (_fe != null)
            {
                dlg.Owner = Window.GetWindow(_fe);
            }
            dlg.ShowDialog();
        }


        /// <summary>
        /// 菜单栏 按钮
        /// </summary>
        private ICommand _menuCommand;

        public ICommand AboutMenuCommand
        {
            get { return _menuCommand ?? (_menuCommand = new DelegateCommand(OnMenuCommand)); }
        }

       // public AboutMenu g_AboutMenu =null;
        private void OnMenuCommand()
        {

        //    PopupMenu.


//             IntPtr hForeground = NativeMethods.GetForegroundWindow();
//             System.Drawing.Rectangle rect = new System.Drawing.Rectangle();
//             NativeMethods.GetWindowRect(hForeground, ref rect);
//             if (g_AboutMenu == null)
//             {
//                   g_AboutMenu = new AboutMenu
//                     {
//                         //DataContext = this,
//                         WindowStartupLocation = WindowStartupLocation.Manual,
//                         Left = rect.Left + 509,
//                         Top = rect.Top + 38,
//                     };
//             }
//             if (_fe != null)
//             {
//            //     dlg.Owner = Window.GetWindow(_fe);
//             }
//             g_AboutMenu.Show();
//             g_AboutMenu.FocusableChanged += OnFocusableChanged;
//             //dlg.ShowDialog();
        }
//         private void OnFocusableChanged(object sender, DependencyPropertyChangedEventArgs e)
//         {
//             g_AboutMenu.Hide();
//         }


//         private  void OnLostFocus(RoutedEventArgs e)
//         {
//             g_AboutMenu.Hide();
//         }



        public BitmapImage AppIcon { get { return (Application.Current as App).Icon; } }



        /// <summary>
        /// 右上角退出按钮
        /// </summary>
        private ICommand _ClickMinExit;
        public ICommand ClickMinExit
        {
            get { return _ClickMinExit ?? (_ClickMinExit = new DelegateCommand(OnClickMinExit)); }
        }
        private void OnClickMinExit()
        {
            Application.Current.Shutdown();
        }



        private ICommand _howToConnectCommand;

        public ICommand HowToConnectCommand
        {
            get { return _howToConnectCommand ?? (_howToConnectCommand = new DelegateCommand(OnHowToConnectCommand)); }
        }
        private void OnHowToConnectCommand()
        {
            if (Program.IsRun)
            {
                DR2API.DR2_Click(ApiHandle, 1, m_MainWindow._isLangUS);
            }
        }




    }
}
