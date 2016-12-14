using System.Windows;
using System.Windows.Input;
using ScreenCast.Commands;

using ScreenCast.Views;


namespace ScreenCast.ViewModels
{
    public interface IClientView
    {
        void FullScreen(bool full);
        void OnBuffer(int width, int height, int isVertical, int bkSize);
    }
    class PhoneViewModel : ViewModelBase
    {
        private IClientView _client;
        private bool _fullScreen;
        private bool _fulling;
        public bool IsChangeFullScrenn { get { return _fulling; } }


        public FloatingToolBars _llft;
        public PhoneViewModel(IClientView client)
            : base(client as FrameworkElement)
        {
            _llft = null;
            _client = client;

           // _client.FullScreen(true);

           int isQp =  DR2API.DR2_GetISQP(ViewModelBase.ApiHandle) ;
            if(isQp == 1)
                OnFullScreenCommand();

            //string isQP = System.Configuration.ConfigurationManager.AppSettings["ISQP"];
            //if (string.Compare(isQP, "true", true) == 0)
            //  OnFullScreenCommand();
            //DR2.Views.PhoneView sss = _client;
        }
        #region Fields
        
        #endregion
        #region Commands
        private ICommand _fullScreenCommand;

        public ICommand FullScreenCommand
        {
            get { return _fullScreenCommand ?? (_fullScreenCommand = new DelegateCommand(OnFullScreenCommand)); }
        }
        private void OnFullScreenCommand()
        {

            if (Program.IsRun)
                DR2API.DR2_SetWndChanging(ApiHandle, true);
            _fulling = true;
            _fullScreen = !_fullScreen;
            if (_client != null)
                _client.FullScreen(_fullScreen);
            _fulling = false;
            if (Program.IsRun)
                DR2API.DR2_SetFullscreen(ApiHandle, _fullScreen);
            if (Program.IsRun)
                DR2API.DR2_SetWndChanging(ApiHandle, false);

        }

        private ICommand _minimizeCommand;

        public ICommand MinimizeCommand
        {
            get { return _minimizeCommand ?? (_minimizeCommand = new DelegateCommand(OnMinimizeCommand)); }
        }

        private void OnMinimizeCommand()
        {
            //Window.GetWindow(_client as DependencyObject).WindowState = WindowState.Minimized;

            Window.GetWindow(_client as DependencyObject).Hide();

            if (_llft != null)
                _llft.Hide();

        }
        private ICommand _closeCommand;

        public ICommand CloseCommand
        {
            get { return _closeCommand ?? (_closeCommand = new DelegateCommand(OnCloseCommand)); }
        }

        private void OnCloseCommand()
        {

            Window.GetWindow(_client as DependencyObject).Close();

            if (Program.IsRun)
            {
                //DR2API.DR2_SetFullscreen(ApiHandle, false);
                DR2API.DR2_Click(ApiHandle, 2);
            }

            
        }
        #endregion

        public void OnGetBuffer(bool show, int width, int height, int isVertical, int bkSize)
        {
            if (_client != null && !_fulling)
            {
                _client.OnBuffer(width, height, isVertical, bkSize);
            }
        }
    }
}
