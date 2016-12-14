using System;
using System.Diagnostics;
using System.IO;
using System.Reflection;
using System.Threading;
using System.Windows;
using System.Windows.Media.Imaging;

namespace ScreenCast
{
    /// <summary>
    /// App.xaml 的交互逻辑
    /// </summary>
    public partial class App
    {
        public string Lang { get; set; }


        public string LoadLang()
        {

            //string GetLang = Thread.CurrentThread.CurrentCulture.Name  ;

            //Lang = System.Configuration.ConfigurationManager.AppSettings["Lang"];
            //bool islangUS = false;

            try
            {
                if (string.IsNullOrEmpty(Lang))
                    Lang = Thread.CurrentThread.CurrentCulture.Name;

                //string.Compare(x, y);
                //if (string.Compare(Lang, "en-US") == 0)
                //if (string.Compare(Lang, "en-US", true) == 0)
               //     islangUS = true;

                var s = string.Format("{1}\\lang\\{0}.xaml", Lang, Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location));
                if (!File.Exists(s))
                {
                    s = string.Format("{1}\\lang\\{0}.xaml", "Zh-cn", Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location));
                    if (!File.Exists(s))
                    {
                        return Lang;
                    }
                }
                Application.Current.Resources.MergedDictionaries.Add(new ResourceDictionary()
                {
                    Source = new Uri(s, UriKind.RelativeOrAbsolute)
                });
            }
            catch (Exception e)
            {
                Trace.WriteLine(e);
            }


            return Lang;

        }

        private BitmapImage _icon;
        public BitmapImage Icon
        {
            get
            {
                if (_icon == null)
                {
                    var stream = Assembly.GetEntryAssembly().GetManifestResourceStream("ScreenCast.icon16.ico");
                    if (stream != null)
                    {
                        _icon = new BitmapImage();
                        _icon.BeginInit();
                        _icon.StreamSource = stream;
                        _icon.EndInit();
                    }
                }
                return _icon;
            }
        }
        public static string GetString(string s)
        {
            return Current.Resources[s] as string;
        }
    }
}
