using System.IO;
using System.Reflection;

namespace ScreenCast.Models
{
    class Environment
    {
        private static string _iniFilePath;
        private static string _myAppPath;

        public static string IniFilePath
        {
            get
            {
                return _iniFilePath ?? (_iniFilePath = Path.Combine(MyAppDataPath, "ScreenCast.ini"));
            }
        }

        public static string MyAppDataPath
        {
            get
            {
                if (_myAppPath == null)
                {
                    _myAppPath = Path.Combine(AppDataPath, "ScreenCast");
                    if (!Directory.Exists(_myAppPath))
                        Directory.CreateDirectory(_myAppPath);
                }
                return _myAppPath;
            }
        }
        public static string AppDataPath
        {
            get
            {
                var str = System.Environment.GetEnvironmentVariable("appdata");
                if (string.IsNullOrEmpty(str))
                    return Path.Combine(Path.GetDirectoryName(Assembly.GetEntryAssembly().Location), "AppData");
                if (!Directory.Exists(str))
                    Directory.CreateDirectory(str);
                return str;
            }
        }
    }
}
