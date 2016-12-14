using System;
using System.Diagnostics;
using System.Reflection;
using System.Threading;
using System.Windows;
using ScreenCast.Views;
using ScreenCast.Client;

namespace ScreenCast
{
    class Program
    {
        public static DataManager dm = null; //add the filed

        public static bool IsRun = false;
        private static Mutex _mutex;
        [STAThread]
        public static void Main(string[] args)
        {


            bool createNew;
            _mutex = new Mutex(true, "KT_" + Assembly.GetEntryAssembly().GetName().Name, out createNew);
            if (!createNew)
            {
                Trace.WriteLine("已经运行一个实例");
                return;
            }
            IsRun = true;
            AppDomain.CurrentDomain.UnhandledException += CurrentDomain_UnhandledException;
            var app = new App();
            app.InitializeComponent();
            string  sLang  = app.LoadLang();

            dm = new DataManager();

            bool isPcWhenOpen = false;
            if (args.Length == 0)
            {
                Debug.WriteLine("not start when pc open\n");
            }
            else
                isPcWhenOpen = true;

            Application.Current.MainWindow = new MainWindow(sLang, isPcWhenOpen);
            app.Run();

            dm.exit();

            _mutex.Close();
        }

        static void CurrentDomain_UnhandledException(object sender, UnhandledExceptionEventArgs e)
        {
            Trace.WriteLine(e.ExceptionObject);
        }
    }
}
