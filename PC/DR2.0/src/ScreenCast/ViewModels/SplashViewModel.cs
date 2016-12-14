using System;
using System.Windows.Input;
using ScreenCast.Commands;
using ScreenCast.Models;
using ScreenCast.Views;
using System.Windows.Threading;


namespace ScreenCast.ViewModels
{
    class SplashViewModel : ViewModelBase
    {
        private SplashWindow _view;

        public DispatcherTimer _mainTimer; 
        public SplashViewModel(SplashWindow view)
            : base(view)
        {
            _view = view;

            _mainTimer = new DispatcherTimer();
            _mainTimer.Interval = TimeSpan.FromSeconds(5);
            _mainTimer.Tick += new EventHandler(dTimer_Tick);
            _mainTimer.IsEnabled = true;
            _mainTimer.Start();

        }


        private void dTimer_Tick(object sender, EventArgs e)
        {
            _view.Close();
            _mainTimer.Stop();
        }


        public bool ShowFirstPage
        {
            get { return !Settings.Current.ShowFirstPage; }
            set
            {
                Settings.Current.ShowFirstPage = !value;
            }
        }
        private ICommand _experienceCommand;

        public ICommand ExperienceCommand
        {
            get { return _experienceCommand ?? (_experienceCommand = new DelegateCommand(OnExperienceCommand)); }
        }
        private void OnExperienceCommand()
        {
            _view.Close();
        }
    }
}
