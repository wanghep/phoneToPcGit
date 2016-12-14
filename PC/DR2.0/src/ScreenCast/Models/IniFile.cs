using System.Runtime.InteropServices;
using System.Text;

namespace ScreenCast.Models
{
    class IniFile
    {
        [DllImport("kernel32")]
        private static extern int WritePrivateProfileString(string section, string key, string value, string filePath);

        [DllImport("kernel32")]
        private static extern int GetPrivateProfileString(string section, string key, string defVal, StringBuilder retVal, int size, string filePath);

        public static string GetString(string section, string key, string def)
        {
            var temp = new StringBuilder(1024);
            GetPrivateProfileString(section, key, def, temp, 1024, Environment.IniFilePath);
            return temp.ToString();
        }

        public static void WriteString(string section, string key, string value)
        {
            WritePrivateProfileString(section, key, value, Environment.IniFilePath);
        }
    }
}
