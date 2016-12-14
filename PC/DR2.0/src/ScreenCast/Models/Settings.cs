using System;
using System.Collections.Generic;
using System.Linq;
using System.Net.NetworkInformation;
using System.Text;

namespace ScreenCast.Models
{
    class Settings
    {
        private static Settings _instance;
        private bool _showFirstPage;
        private const string SECTION_SHOW = "Show";
        private const string KEY_SHOWFIRSTPAGE = "ShowFirstPage";
        public static Settings Current
        {
            get { return _instance ?? (_instance = new Settings()); }
        }

        private Settings()
        {
            _showFirstPage = Convert.ToInt32(IniFile.GetString(SECTION_SHOW, KEY_SHOWFIRSTPAGE, "1")) == 1;
        }
        public bool ShowFirstPage
        {
            get { return _showFirstPage; }
            set
            {
                _showFirstPage = value;
                IniFile.WriteString(SECTION_SHOW, KEY_SHOWFIRSTPAGE, _showFirstPage ? "1" : "0");
            }
        }
    }
}
