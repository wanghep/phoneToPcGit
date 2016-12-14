using System;
using System.Net;
using System.Threading;
using System.Windows;
using ScreenCast.Commands;
using ScreenCast.Views;
using System.Windows.Threading;
using ScreenCast.Client;
using System.Windows.Forms;

namespace ScreenCast.ViewModels
{
    class MainViewModel : ViewModelBase
    {
        private MainWindow _view;
    //    public  MainViewModel g_MainWindow = null;

        private Thread _initThread;

        private DispatcherTimer _mainTimer;
        private KeyboardHook k_hook;
        //private event KeyEventHandler hook_KeyDown;
        System.Drawing.Point lastPoint;
        Boolean inPhone = true;

        //private const string KEY_ErrorStateText_NULL = "设备之间连接已被用户断开..";
		//private const string KEY_ErrorStateText_Normal = "打开手机进行投屏吧";
	    //private const string KEY_ErrorStateText_Outtime = "因为超时断开连接...";
		//private const string KEY_ErrorStateText_MYdisconnect = "主动断开连接...";
			
        public MainViewModel(MainWindow view)
            : base(view)
        {
           // g_MainWindow = this;
            _view = view;
            ComputerName = Dns.GetHostName();
            Start();
			
            _mainTimer = new DispatcherTimer();
			_mainTimer.Interval = TimeSpan.FromSeconds(5);
			_mainTimer.Tick += new EventHandler(dTimer_Tick);
			_mainTimer.IsEnabled = true;
            k_hook = new KeyboardHook();
            k_hook.KeyDownEvent += new System.Windows.Forms.KeyEventHandler(hook_KeyDown);//钩住键按下
            k_hook.Start();//安装键盘钩子
        }

       public void hook_KeyDown(object sender, KeyEventArgs e)
       {
           //判断按下的键（Ctrl + C）,change an idea
           if (e.KeyValue == (int)Keys.C && (int)Control.ModifierKeys == (int)Keys.Control)
           {
               //System.Windows.Forms.MessageBox.Show("按下了粘贴快捷键组合");               
               getClipBoradSource();
               //if (inPhone)
               //{ 
                  //int ret = DR2.Server.DataManager.sendCtrlData(System.BitConverter.GetBytes(e.KeyValue));
                  
               //}
           }
           //判断按下的键（Ctrl + V）
          /* if (e.KeyValue == (int)Keys.V&& (int)Control.ModifierKeys == (int)Keys.Control)
           {
               System.Windows.Forms.MessageBox.Show("按下了拷贝快捷键组合");
               lastPoint=getCursorPos();
               
               if (true)//inPhone
               {
                   int ret = DR2.Server.DataManager.sendCtrlData(System.BitConverter.GetBytes(e.KeyValue));  
               }
           }*/
       }

          private System.Drawing.Point getCursorPos()
          {
                               // Point showPoint = new Point();
                  //bool hasGet= GetCursorPos(showPoint);
                  //return showPoint;
              System.Drawing.Point point = System.Windows.Forms.Cursor.Position;
              //System.Windows.Forms.MessageBox.Show("记录此时光标位置");
              Console.WriteLine("光标位置是%d,%d",point.X,point.Y);
              return point;
          }
          private void getClipBoradSource()
          {
             
              System.Drawing.Point currentPoint=getCursorPos();
/*          #if 0
              if (currentPoint.X - lastPoint.X > 200 && currentPoint.Y - lastPoint.Y > 200)
              {
                  System.Windows.Forms.MessageBox.Show("从手机剪贴板取数据发到PC");
                  getClipDataFromPhone();
              }
              else if (lastPoint.X - currentPoint.X > 200 && lastPoint.Y - currentPoint.Y > 200)
              {
                  System.Windows.Forms.MessageBox.Show("从PC剪贴板取数据发到手机");
                  getClipDataFromPC();
              }
              else {
                  System.Windows.Forms.MessageBox.Show("从哪来回哪去");
              }
#else      */
            //System.Windows.Forms.MessageBox.Show("从PC剪贴板取数据发到手机");
            getClipDataFromPC();
//    temp              System.Windows.Forms.MessageBox.Show("从手机剪贴板取数据发到PC");
//      testc android 2 PC        getClipDataFromPhone();
//#endif
          }
         private void getClipDataFromPhone()
         {
             //PC向Phone侧发起握手，开始读
             //byte[] data = { 1 };
             String handShakeFlag = "11111111";
             byte[] handShakedata = System.Text.Encoding.Default.GetBytes(handShakeFlag);
             int ret = DataManager.sendRawData(handShakedata);
             if (ret == -1)
             {
                 System.Windows.Forms.MessageBox.Show("pc与phone通信失败");
             }
             else 
             {
                 //接收Phone侧的剪贴板数据
                 //byte[] dataFromPhone;
                 //ProcessDataFunClass(dataFromPhone);
                 //DataManager.registProcessObj(cb);

             }
         }
         private void getClipDataFromPC()
         {
             byte[] PCClipData = null;
             //PC向Phone侧发起握手，开始写
             String handShakeFlag = "00000000";
             byte[] data = System.Text.Encoding.Default.GetBytes(handShakeFlag);
             /* int ret = DR2.Server.DataManager.sendRawData(data);
              if (ret == -1)
              {
                  System.Windows.Forms.MessageBox.Show("pc与phone通信失败");
              }

              else */
             {
                 //获得pc剪贴板内容，往手机侧送
                 // GetDataObject检索当前剪贴板上的数据
                 System.Windows.IDataObject iData = System.Windows.Clipboard.GetDataObject(); 
                 // 将数据与指定的格式进行匹配，返回bool
                 if (iData.GetDataPresent(System.Windows.DataFormats.Text))
                 {    
                     // GetData检索数据并指定一个格式    
                     String clipText = (string)iData.GetData(System.Windows.DataFormats.Text);
                     PCClipData = System.Text.Encoding.Default.GetBytes(clipText);
                 }
                 else
                 {
                     System.Windows.Forms.MessageBox.Show("剪贴板中数据不可转换为文本","错误");     
                 }
                 
                 int isSuccess = DataManager.sendRawData(PCClipData);
                 //if(isSuccess==1)                    
                 //    System.Windows.Forms.MessageBox.Show("pc向手机拷贝成功");
                 //else 
                 //    System.Windows.Forms.MessageBox.Show("pc向手机拷贝失败");            
             }

         }
		private void dTimer_Tick(object sender, EventArgs e)
		{
            CallStateText = ""; //App.GetString("KEY_ErrorStateText_Outtime");
			_mainTimer.Stop();
		}
   

        private void Start()
        {
            _onState = OnDeviceStateChanged;
            DeviceState = DeviceState.Idle;
            Create();
            if (!Started && ApiHandle != IntPtr.Zero)
            {
                _initThread = new Thread(OnInit);
                _initThread.Start();
            }
        }
        #region Fields

        private DR2API.OnDeviceStateChanged _onState;
        private bool _loaded;
        #endregion

        void OnDeviceStateChanged(IntPtr handle, IntPtr devHandle, string devName, bool mobile, DeviceState state)
        {
            IsMobile = mobile;
            DeviceName = devName;
            DeviceState = state;

			if (state == DeviceState.ConnectOuttime)
			{
                CallStateText = App.GetString("KeyErrorStateTextOuttime");
				//_mainTimer.Start();
			}
			else if (state == DeviceState.MYDisconnected)
			{
                CallStateText = App.GetString("KeyErrorStateTextMYdisconnect");
				//_mainTimer.Start();
			}
			else
			{
                CallStateText = App.GetString("KeyErrordefault");
			}

            _view.OnStateChanged(state);
        }
        #region Commands
        
        private System.Windows.Input.ICommand _howToConnectCommand;

        public System.Windows.Input.ICommand HowToConnectCommand
        {
            get { return _howToConnectCommand ?? (_howToConnectCommand = new DelegateCommand(OnHowToConnectCommand)); }
        }


        private System.Windows.Input.ICommand _howToConnectCommand2;
        public System.Windows.Input.ICommand HowToConnectCommand2
        {
            get { return _howToConnectCommand2 ?? (_howToConnectCommand2 = new DelegateCommand(OnHowToConnectCommand2)); }
        }



        private System.Windows.Input.ICommand _CloseCommand;
        public System.Windows.Input.ICommand CloseCommand
        {
            get { return _CloseCommand ?? (_CloseCommand = new DelegateCommand(OnCloseCommand)); }
        }
        private void OnCloseCommand()
        {
            if (Program.IsRun)
            {
               // DR2API.DR2_SetFullscreen(ApiHandle, false);
                DR2API.DR2_Click(ApiHandle, 2);
            }
        }

        private void OnHowToConnectCommand()
        {
            if (Program.IsRun)
                DR2API.DR2_Click(ApiHandle, 1, _view._isLangUS);
        }
        private void OnHowToConnectCommand2()
        {
            if (Program.IsRun)
                DR2API.DR2_Click(ApiHandle, 4, _view._isLangUS);
        }



        /// <summary>
        /// 返回引导页面
        /// </summary>
        private System.Windows.Input.ICommand _BackToSplash;

        public System.Windows.Input.ICommand BackToSplash
        {
            get { return _BackToSplash ?? (_BackToSplash = new DelegateCommand(OnBackToSplash)); }
        }
        private void OnBackToSplash()
        {
            //_view.PCConnet.Visibility = Visibility.Collapsed;

            _view.PCConnet.Visibility = Visibility.Visible;
            _view.PhoneConnet.Visibility = Visibility.Collapsed;
            _view.connecterr.Visibility = Visibility.Collapsed;
            _view.connecting.Visibility = Visibility.Collapsed; 
        }





        private System.Windows.Input.ICommand _disconnectCommand;

        public System.Windows.Input.ICommand DisconnectCommand
        {
            get { return _disconnectCommand ?? (_disconnectCommand = new DelegateCommand(OnDisconnectCommand)); }
        }
        private void OnDisconnectCommand()
        {
            if (Program.IsRun)
                DR2API.DR2_Click(ApiHandle, 2);
        }
        
        private System.Windows.Input.ICommand _loadCommand;

        public System.Windows.Input.ICommand LoadCommand
        {
            get { return _loadCommand ?? (_loadCommand = new DelegateCommand(OnLoadCommand)); }
        }
        private void OnLoadCommand()
        {
            if (!_loaded)
            {
                _loaded = true;
            }
        }

        void OnInit()
        {
            var ret = Start("", _onState);
            if (ret != 0)
            {

            }
        }
        private System.Windows.Input.ICommand _unloadCommand;

        public System.Windows.Input.ICommand UnloadCommand
        {
            get { return _unloadCommand ?? (_unloadCommand = new DelegateCommand(OnUnloadCommand)); }
        }
        private void OnUnloadCommand()
        {
        }

        #endregion
        #region 属性
        private DeviceState _state;

        public DeviceState DeviceState
        {
            get { return _state; }
            set
            {
                if (_state != value)
                {
                    _state = value;
                    NotifyPropertyChanged("DeviceState");
                }
            }
        }
        private string _computerName;

        public string ComputerName
        {
            get { return _computerName; }
            set
            {
                if (_computerName != value)
                {
                    _computerName = value;
                    NotifyPropertyChanged("ComputerName");
                }
            }
        }
        private string _devName;

        public string DeviceName
        {
            get { return _devName; }
            set
            {
                if (_devName != value)
                {
                    _devName = value;
                    NotifyPropertyChanged("DeviceName");
                }
            }
        }

		private string _StateString;
		public string CallStateText
		{
			get { return _StateString; }
			set
			{
				if (_StateString != value)
				{
					_StateString = value;
					NotifyPropertyChanged("CallStateText");
				}
			}
		}




        
        #endregion
    }
}
