using System;
using System.Windows;
using System.Windows.Input;
using System.Windows.Media;
using ScreenCast.Controls;
using ScreenCast.Models;
using ScreenCast.ViewModels;
using ScreenCast.Views;


namespace ScreenCast.Views
{
    /// <summary>
    /// FloatingToolBars.xaml 的交互逻辑
    /// </summary>
    public partial class FloatingToolBars
    {

        private MainWindow m_MainWindow = null;

        //private static  IntPtr _wnd;

        private static FloatingToolBars m_plf;

        public FloatingToolBars()
        {
            InitializeComponent();

            m_MainWindow = MainWindow.g_MainWindow;
        }

        public static FloatingToolBars ShowToolBars(Window parent)
        {
            var ret = new FloatingToolBars
            {
                Owner = parent,
                
                DataContext  = parent.DataContext, 

            };
            ret.Left = parent.Left + parent.Width - 140;
            ret.Top = parent.Top;
            //ret.Show();
            m_plf = ret;
            return ret;
        }

        public FloatingToolBars ShowMyBar(bool isshow, Window parent, bool isfullscreen)
        {

            if (isfullscreen)
            {
                m_plf.Left = parent.Width - 140;
                m_plf.Top = 0;
            }
            else
            {
                m_plf.Left = parent.Left + parent.Width - 140;
                m_plf.Top = parent.Top;
                // m_plf.ftbname.Opacity = 1.0;
            }
            if (isshow)
            {
                m_plf.ftbname.Opacity = 1.0; 
                m_plf.Show();
            }
            else { 
                m_plf.Hide();
            }
         //   Debug.WriteLine("left:{0} ,top:{1}", m_plf.Left, m_plf.Top);
            return m_plf;
        }



         private void UIElementFloat_OnMouseEnter(object sender, MouseEventArgs e)
         {
             //if (WindowState == WindowState.Normal)
             //{

             //}
             var fe = sender as FrameworkElement;
             if (fe == null)
                 return;
             fe.Opacity = 1.0;
         }
         private void UIElementFloat_OnMouseLeave(object sender, MouseEventArgs e)
         {
             var fe = sender as FrameworkElement;
             if (fe == null)
                 return;
             fe.Opacity = 0.0;
         }

         private void OnClickMenuPopup(object sender, RoutedEventArgs e)
         {
             CheckBoxAutoRun.IsChecked = m_MainWindow.AutoLaunch;
             CheckBoxShowFirstPage.IsChecked = Settings.Current.ShowFirstPage;

             PopupMenu.HorizontalOffset = -143;
             PopupMenu.VerticalOffset = 6;
             PopupMenu.IsOpen = true;

         }

         private void OnAutoRunClick(object sender, EventArgs e)
         {
             m_MainWindow.AutoLaunch = !m_MainWindow.AutoLaunch;
             m_MainWindow._autoRun.Checked = m_MainWindow.AutoLaunch;
             CheckBoxAutoRun.IsChecked = m_MainWindow.AutoLaunch;
         }

         private void OnCheckBoxShowFirstPage(object sender, EventArgs e)
         {
             bool SFP = Settings.Current.ShowFirstPage;
             Settings.Current.ShowFirstPage = !SFP;
             m_MainWindow._ShowFirstPage.Checked = !SFP;
             CheckBoxShowFirstPage.IsChecked = !SFP;
         }

         private void OnMenuClickHelp(object sender, RoutedEventArgs e)
         {
             if (Program.IsRun)
                 DR2API.DR2_Click(ViewModelBase.ApiHandle, 4, m_MainWindow._isLangUS);
         }

         private void OnMenuClickEnter(object sender, MouseEventArgs e)
         {
             ((ImageButton)sender).Foreground = Brushes.White; 
         }

         private void OnMenuClickLeave(object sender, MouseEventArgs e)
         {
             ((ImageButton)sender).Foreground = Brushes.Gray; 
         }



            //public static FloatingToolBars ShowToolBarsHande(Window parent)
       // {
           // var wndHelper = new WindowInteropHelper(Window.GetWindow(this));
            //_wnd = wndHelper.Handle;
            //return _wnd;
        //}







    }
}
