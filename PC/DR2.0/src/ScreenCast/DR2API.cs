using System;
using System.Runtime.InteropServices;
using System.Windows;

namespace ScreenCast
{
    public delegate void cbFun(string ip);  //声明一个委托

    public class DR2API
    {
        [UnmanagedFunctionPointer(CallingConvention.StdCall, CharSet = CharSet.Ansi)]
        public unsafe delegate void OnBufferCallback(byte* buffer, int width, int height, int isVertical, int bkSize);

        [UnmanagedFunctionPointer(CallingConvention.StdCall, CharSet = CharSet.Ansi)]
        public delegate void OnDeviceStateChanged(IntPtr handle, IntPtr devHandle, string devName, bool mobile, DeviceState state);

        [DllImport("DR2API.dll", CallingConvention = CallingConvention.StdCall, CharSet = CharSet.Ansi)]
        public static extern IntPtr DR2_Create();

        [DllImport("DR2API.dll", CallingConvention = CallingConvention.StdCall, CharSet = CharSet.Ansi)]
        public static extern void DR2_Destroy(IntPtr handle);
        [DllImport("DR2API.dll", CallingConvention = CallingConvention.StdCall, CharSet = CharSet.Ansi)]
        public static extern int DR2_Start(IntPtr handle, string name, OnDeviceStateChanged stateChanged, OnBufferCallback callback);
        [DllImport("DR2API.dll", CallingConvention = CallingConvention.StdCall, CharSet = CharSet.Ansi)]
        public static extern void DR2_Stop(IntPtr handle);

        [DllImport("DR2API.dll", CallingConvention = CallingConvention.StdCall, CharSet = CharSet.Ansi)]
        public static extern void DR2_Draw(IntPtr handle, IntPtr wnd, IntPtr hdc, RECT rc, int fullScreen);

        /*wangheping add start */
        [DllImport("DR2API.dll", CallingConvention = CallingConvention.StdCall, CharSet = CharSet.Ansi)]
        public static extern void DR2_GetDrawRect(IntPtr handle, ref RECT rectin, ref RECT rectout);

        [DllImport("DR2API.dll", CallingConvention = CallingConvention.StdCall, CharSet = CharSet.Ansi)]
        public static extern void DR2_test();

        [DllImport("DR2API.dll", CallingConvention = CallingConvention.StdCall, CharSet = CharSet.Ansi)]
        public static extern void DR2_registSocketFun(IntPtr handle, cbFun pFun);
        /*wangheping add end */

        [DllImport("DR2API.dll", CallingConvention = CallingConvention.StdCall, CharSet = CharSet.Ansi)]
        public static extern void DR2_Click(IntPtr handle, int clickCmd, string sLang=null);

        [DllImport("DR2API.dll", CallingConvention = CallingConvention.StdCall, CharSet = CharSet.Ansi)]
        public static extern void DR2_SetDrawWnd(IntPtr handle, IntPtr wnd);
        [DllImport("DR2API.dll", CallingConvention = CallingConvention.StdCall, CharSet = CharSet.Ansi)]
        public static extern void DR2_SetWndChanging(IntPtr handle, bool changing);
        [DllImport("DR2API.dll", CallingConvention = CallingConvention.StdCall, CharSet = CharSet.Ansi)]
        public static extern void DR2_SetFullscreen(IntPtr handle, bool changing);

        [DllImport("DR2API.dll", CallingConvention = CallingConvention.StdCall, CharSet = CharSet.Ansi)]
        public static extern int DR2_GetISQP(IntPtr handle);
        
        #region UIBC
        
        [DllImport("DR2API.dll", CallingConvention = CallingConvention.StdCall, CharSet = CharSet.Unicode)]
        public static extern void DR2_OnText(IntPtr handle, string text);

        [DllImport("DR2API.dll", CallingConvention = CallingConvention.StdCall, CharSet = CharSet.Unicode)]
        public static extern void DR2_OnMouseLeftButtonDown(IntPtr handle, int clickCount, int width, int height, int x, int y);
        [DllImport("DR2API.dll", CallingConvention = CallingConvention.StdCall, CharSet = CharSet.Unicode)]
        public static extern void DR2_OnMouseLeftButtonUp(IntPtr handle, int width, int height, int x, int y);
        [DllImport("DR2API.dll", CallingConvention = CallingConvention.StdCall, CharSet = CharSet.Unicode)]
        public static extern void DR2_OnMouseMove(IntPtr handle, int width, int height, int x, int y);
        [DllImport("DR2API.dll", CallingConvention = CallingConvention.StdCall, CharSet = CharSet.Unicode)]
        public static extern void DR2_OnMouseRightButtonDown(IntPtr handle, int width, int height, int x, int y);
        [DllImport("DR2API.dll", CallingConvention = CallingConvention.StdCall, CharSet = CharSet.Unicode)]
        public static extern void DR2_OnMouseRightButtonUp(IntPtr handle, int width, int height, int x, int y);
    
        #endregion
    }

    public enum DeviceState
    {
        Idle,
        Connecting,
        Connected,
        Disconnecting,
        Disconnected,
        ConnectOuttime, //连接超时
        MYDisconnected, // 主动断开连接
        ServiceNotActive,
        ReInvalidData,
        ReInvalidParameter,
        GenFailure,
    }
    [StructLayout(LayoutKind.Sequential, Pack = 0)]
    public struct RECT
    {
        /// <summary> Win32 </summary>
        public int left;
        /// <summary> Win32 </summary>
        public int top;
        /// <summary> Win32 </summary>
        public int right;
        /// <summary> Win32 </summary>
        public int bottom;

        /// <summary> Win32 </summary>
        public static readonly RECT Empty = new RECT();

        /// <summary> Win32 </summary>
        public int Width
        {
            get { return Math.Abs(right - left); }  // Abs needed for BIDI OS
        }
        /// <summary> Win32 </summary>
        public int Height
        {
            get { return bottom - top; }
        }

        /// <summary> Win32 </summary>
        public RECT(int left, int top, int right, int bottom)
        {
            this.left = left;
            this.top = top;
            this.right = right;
            this.bottom = bottom;
        }


        /// <summary> Win32 </summary>
        public RECT(RECT rcSrc)
        {
            this.left = rcSrc.left;
            this.top = rcSrc.top;
            this.right = rcSrc.right;
            this.bottom = rcSrc.bottom;
        }

        /// <summary> Win32 </summary>
        public bool IsEmpty
        {
            get
            {
                // BUGBUG : On Bidi OS (hebrew arabic) left > right
                return left >= right || top >= bottom;
            }
        }
        /// <summary> Return a user friendly representation of this struct </summary>
        public override string ToString()
        {
            if (this == RECT.Empty) { return "RECT {Empty}"; }
            return "RECT { left : " + left + " / top : " + top + " / right : " + right + " / bottom : " + bottom + " }";
        }

        /// <summary> Determine if 2 RECT are equal (deep compare) </summary>
        public override bool Equals(object obj)
        {
            if (!(obj is Rect)) { return false; }
            return (this == (RECT)obj);
        }

        /// <summary>Return the HashCode for this struct (not garanteed to be unique)</summary>
        public override int GetHashCode()
        {
            return left.GetHashCode() + top.GetHashCode() + right.GetHashCode() + bottom.GetHashCode();
        }


        /// <summary> Determine if 2 RECT are equal (deep compare)</summary>
        public static bool operator ==(RECT rect1, RECT rect2)
        {
            return (rect1.left == rect2.left && rect1.top == rect2.top && rect1.right == rect2.right && rect1.bottom == rect2.bottom);
        }

        /// <summary> Determine if 2 RECT are different(deep compare)</summary>
        public static bool operator !=(RECT rect1, RECT rect2)
        {
            return !(rect1 == rect2);
        }
    }
}
