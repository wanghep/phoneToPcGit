package com.phoneToPc;

import java.net.InetAddress;
import java.net.NetworkInterface;
import java.net.SocketException;
import java.util.Enumeration;

import org.apache.http.conn.util.InetAddressUtils;

import android.util.Log;
import android.view.KeyEvent;

public class util {

	public static String getLocalIP_Address() {
		String ipAddress = "";
		try {

			for (Enumeration<NetworkInterface> en = NetworkInterface
					.getNetworkInterfaces(); en.hasMoreElements();) {
				NetworkInterface intf = en.nextElement();
				for (Enumeration<InetAddress> enumIpAddr = intf
						.getInetAddresses(); enumIpAddr.hasMoreElements();) {
					InetAddress inetAddress = enumIpAddr.nextElement();
					if (!inetAddress.isLoopbackAddress()
							&& InetAddressUtils.isIPv4Address(inetAddress
									.getHostAddress())) {
						ipAddress += inetAddress.getHostAddress().toString();
					}
				}
			}
		} catch (SocketException e) {
			Log.d("WifiPreference IpAddress", e.toString());

		}

		return ipAddress;
	}

	public static int pcKeyCodeToPhoneKeyCode(int pcKeyCode) {
		int ret = -1;

		/* 主键盘 */

		/*
		 * ret = KeyEvent.KEYCODE_AT; // @
		 * 
		 * ret = KeyEvent.KEYCODE_POUND ; // # ret = KeyEvent.
		 * KEYCODE_NUMPAD_LEFT_PAREN; // ( ret = KeyEvent.
		 * KEYCODE_NUMPAD_RIGHT_PAREN; // )
		 */

		// 字母a到z
		if ((pcKeyCode >= 65) && (pcKeyCode <= 90)) {
			ret = pcKeyCode - (65 - KeyEvent.KEYCODE_A);
		}

		// 主键盘 0到9
		else if ((pcKeyCode >= 48) && (pcKeyCode <= 57)) {
			ret = pcKeyCode - (48 - KeyEvent.KEYCODE_0);
		}

		// F1到F12
		else if ((pcKeyCode >= 112) && (pcKeyCode <= 123)) {
			ret = pcKeyCode - (112 - KeyEvent.KEYCODE_F1);
		}
		// ESC
		else if ((pcKeyCode == 27)) {
			ret = KeyEvent.KEYCODE_ESCAPE;
		}

		// `~ 数字1左边的键
		else if ((pcKeyCode == 192)) {
			ret = KeyEvent.KEYCODE_GRAVE;
		}

		// tab
		else if ((pcKeyCode == 9)) {
			ret = KeyEvent.KEYCODE_TAB;
		}

		// backspace
		else if ((pcKeyCode == 8)) {
			ret = KeyEvent.KEYCODE_DEL;
		}

		// Enter
		else if ((pcKeyCode == 108)) {
			ret = KeyEvent.KEYCODE_ENTER;
		}

		// Caps Lock
		else if ((pcKeyCode == 20)) {
			// ret = KeyEvent.KEYCODE_CAPS_LOCK;
			ret = KeyEvent.KEYCODE_SHIFT_LEFT;
		}

		// NumLock
		else if ((pcKeyCode == 144)) {
			ret = KeyEvent.KEYCODE_NUM_LOCK;
		}

		// Insert
		else if ((pcKeyCode == 45)) {
			ret = KeyEvent.KEYCODE_INSERT;
		}

		// Delete
		else if ((pcKeyCode == 46)) {
			ret = KeyEvent.KEYCODE_FORWARD_DEL;
		}

		// Home
		else if ((pcKeyCode == 36)) {
			ret = KeyEvent.KEYCODE_MOVE_HOME;
		}

		// End
		else if ((pcKeyCode == 35)) {
			ret = KeyEvent.KEYCODE_MOVE_END;
		}

		// PgUp
		else if ((pcKeyCode == 33)) {
			ret = KeyEvent.KEYCODE_PAGE_UP;
		}

		// PgDn
		else if ((pcKeyCode == 34)) {
			ret = KeyEvent.KEYCODE_PAGE_DOWN;
		}

		// Scroll Lock
		else if ((pcKeyCode == 145)) {
			ret = KeyEvent.KEYCODE_SCROLL_LOCK;
		}

		// Pause Break??
		else if ((pcKeyCode == 19)) {
			ret = KeyEvent.KEYCODE_BREAK;
		}

		// Shift
		else if ((pcKeyCode == 16)) {
			ret = KeyEvent.KEYCODE_SHIFT_RIGHT;
		}

		// Ctrl
		else if ((pcKeyCode == 17)) {
			ret = KeyEvent.KEYCODE_CTRL_LEFT;
		}

		// Alt
		else if ((pcKeyCode == 18)) {
			ret = KeyEvent.KEYCODE_ALT_LEFT;
		}

		// Space
		else if ((pcKeyCode == 32)) {
			ret = KeyEvent.KEYCODE_SPACE;
		}

		// , <
		else if ((pcKeyCode == 188)) {
			ret = KeyEvent.KEYCODE_COMMA;
		}

		// . >
		else if ((pcKeyCode == 190)) {
			ret = KeyEvent.KEYCODE_PERIOD;
		}

		// /?
		else if ((pcKeyCode == 191)) {
			ret = KeyEvent.KEYCODE_SLASH;
		}

		// [{
		else if ((pcKeyCode == 219)) {
			ret = KeyEvent.KEYCODE_LEFT_BRACKET;
		}

		// ]}
		else if ((pcKeyCode == 221)) {
			ret = KeyEvent.KEYCODE_RIGHT_BRACKET;
		}

		// \|
		else if ((pcKeyCode == 220)) {
			ret = KeyEvent.KEYCODE_BACKSLASH;
		}

		// ' "
		else if ((pcKeyCode == 222)) {
			ret = KeyEvent.KEYCODE_APOSTROPHE;
		}

		/*
		 * 适配火狐浏览器 // ;: else if ((pcKeyCode == 59)) { ret =
		 * KeyEvent.KEYCODE_SEMICOLON; }
		 * 
		 * // -_ else if ((pcKeyCode == 173)) { ret = KeyEvent.KEYCODE_MINUS; }
		 * 
		 * // = + else if ((pcKeyCode == 61)) { ret = KeyEvent.KEYCODE_EQUALS;
		 * //ret = KeyEvent.KEYCODE_PLUS; }
		 */

		// ;:
		else if ((pcKeyCode == 186)) {
			ret = KeyEvent.KEYCODE_SEMICOLON;
		}

		// -_
		else if ((pcKeyCode == 189)) {
			ret = KeyEvent.KEYCODE_MINUS;
		}

		// = +
		else if ((pcKeyCode == 187)) {
			ret = KeyEvent.KEYCODE_EQUALS;
			// ret = KeyEvent.KEYCODE_PLUS;
		}

		// Backspace
		else if ((pcKeyCode == 8)) {
			ret = KeyEvent.KEYCODE_BACK;
		}

		// Left
		else if ((pcKeyCode == 37)) {
			ret = KeyEvent.KEYCODE_DPAD_LEFT;
		}

		// Up
		else if ((pcKeyCode == 38)) {
			ret = KeyEvent.KEYCODE_DPAD_UP;
		}

		// Right
		else if ((pcKeyCode == 39)) {
			ret = KeyEvent.KEYCODE_DPAD_RIGHT;
		}

		// Down
		else if ((pcKeyCode == 40)) {
			ret = KeyEvent.KEYCODE_DPAD_DOWN;
		}

		// 小键盘

		// 小键盘按键'0~9'

		else if ((pcKeyCode >= 96) && (pcKeyCode <= 105)) {
			// ret = pcKeyCode - (96 - KeyEvent.KEYCODE_NUMPAD_0);
			ret = pcKeyCode - (96 - KeyEvent.KEYCODE_0);
		}

		// 小键盘按键'+'
		else if ((pcKeyCode == 107)) {
			ret = KeyEvent.KEYCODE_NUMPAD_ADD;
		}

		// 小键盘按键'-'
		else if ((pcKeyCode == 109)) {
			ret = KeyEvent.KEYCODE_NUMPAD_SUBTRACT;
		}

		// 小键盘按键'*'
		else if ((pcKeyCode == 106)) {
			ret = KeyEvent.KEYCODE_NUMPAD_MULTIPLY;
		}

		// 小键盘按键'/'55
		else if ((pcKeyCode == 111)) {
			ret = KeyEvent.KEYCODE_NUMPAD_DIVIDE;
		}

		// 小键盘按键'.'
		else if ((pcKeyCode == 110)) {
			// ret = KeyEvent. KEYCODE_NUMPAD_DOT;
			ret = KeyEvent.KEYCODE_PERIOD;
		}

		// 小键盘按键'Enter'
		else if ((pcKeyCode == 13)) {
			ret = KeyEvent.KEYCODE_NUMPAD_ENTER;
		}
		return ret;
	}

	public static int PCKeyCodeToPhoneKeyCode(int pcKeyCode) {
		int ret = -1;

		/* 主键盘 */

		/*
		 * ret = KeyEvent.KEYCODE_AT; // @ ret = KeyEvent.KEYCODE_POUND ; // #
		 * ret = KeyEvent. KEYCODE_NUMPAD_LEFT_PAREN; // ( ret = KeyEvent.
		 * KEYCODE_NUMPAD_RIGHT_PAREN; // )
		 */

		// 字母a到z
		if ((pcKeyCode >= 44) && (pcKeyCode <= 69)) {
			ret = pcKeyCode - (44 - KeyEvent.KEYCODE_A);
		}

		// 主键盘 0到9
		else if ((pcKeyCode >= 34) && (pcKeyCode <= 43)) {
			ret = pcKeyCode - (34 - KeyEvent.KEYCODE_0);
		}

		// F1到F12
		else if ((pcKeyCode >= 90) && (pcKeyCode <= 101)) {
			ret = pcKeyCode - (90 - KeyEvent.KEYCODE_F1);
		}
		// ESC
		else if ((pcKeyCode == 13)) {
			ret = KeyEvent.KEYCODE_ESCAPE;
		}

		// `~ 数字1左边的键
		else if ((pcKeyCode == 192)) {
			ret = KeyEvent.KEYCODE_GRAVE;
		}

		// tab
		else if ((pcKeyCode == 3)) {
			ret = KeyEvent.KEYCODE_TAB;
		}

		// backspace
		else if ((pcKeyCode == 2)) {
			ret = KeyEvent.KEYCODE_DEL;
		}

		// Enter
		else if ((pcKeyCode == 6)) {
			ret = KeyEvent.KEYCODE_ENTER;
		}

		// Caps Lock
		else if ((pcKeyCode == 8)) {
			ret = KeyEvent.KEYCODE_CAPS_LOCK;
			// ret = KeyEvent.KEYCODE_SHIFT_LEFT;
		}

		// NumLock
		else if ((pcKeyCode == 114)) {
			ret = KeyEvent.KEYCODE_NUM_LOCK;
		}

		// Insert
		else if ((pcKeyCode == 31)) {
			ret = KeyEvent.KEYCODE_INSERT;
		}

		// Delete
		else if ((pcKeyCode == 32)) {
			ret = KeyEvent.KEYCODE_FORWARD_DEL;
		}

		// Home
		else if ((pcKeyCode == 22)) {
			ret = KeyEvent.KEYCODE_MOVE_HOME;
		}

		// End
		else if ((pcKeyCode == 21)) {
			ret = KeyEvent.KEYCODE_MOVE_END;
		}

		// PgUp
		else if ((pcKeyCode == 19)) {
			ret = KeyEvent.KEYCODE_PAGE_UP;
		}

		// PgDn
		else if ((pcKeyCode == 20)) {
			ret = KeyEvent.KEYCODE_PAGE_DOWN;
		}

		// Scroll Lock
		else if ((pcKeyCode == 115)) {
			ret = KeyEvent.KEYCODE_SCROLL_LOCK;
		}

		// Pause Break  
		else if ((pcKeyCode == 19)) {
			ret = KeyEvent.KEYCODE_BREAK;
		}

		// LeftShift
		else if ((pcKeyCode == 116)) {
			ret = KeyEvent.KEYCODE_SHIFT_LEFT;
		}

		// RightShift
		else if ((pcKeyCode == 117)) {
			ret = KeyEvent.KEYCODE_SHIFT_RIGHT;
		}

		// LeftCtrl
		else if ((pcKeyCode == 118)) {
			ret = KeyEvent.KEYCODE_CTRL_LEFT;
		}

		// RightCtrl
		else if ((pcKeyCode == 119)) {
			ret = KeyEvent.KEYCODE_CTRL_RIGHT;
		}

		// LeftAlt
		else if ((pcKeyCode == 120)) {
			ret = KeyEvent.KEYCODE_ALT_LEFT;
		}

		// RightAlt
		else if ((pcKeyCode == 121)) {
			ret = KeyEvent.KEYCODE_ALT_RIGHT;
		}

		// Space
		else if ((pcKeyCode == 18)) {
			ret = KeyEvent.KEYCODE_SPACE;
		}

		// , <
		else if ((pcKeyCode == 142)) {
			ret = KeyEvent.KEYCODE_COMMA;
		}

		// . >
		else if ((pcKeyCode == 144)) {
			ret = KeyEvent.KEYCODE_PERIOD;
		}

		// /?
		else if ((pcKeyCode == 145)) {
			ret = KeyEvent.KEYCODE_SLASH;
		}

		// [{
		else if ((pcKeyCode == 149)) {
			ret = KeyEvent.KEYCODE_LEFT_BRACKET;
		}

		// ]}
		else if ((pcKeyCode == 151)) {
			ret = KeyEvent.KEYCODE_RIGHT_BRACKET;
		}

		// \|
		else if ((pcKeyCode == 89)) {
			ret = KeyEvent.KEYCODE_BACKSLASH;
		}

		// ' "
		else if ((pcKeyCode == 222)) {
			ret = KeyEvent.KEYCODE_APOSTROPHE;
		}

		/*
		 * 适配火狐浏览器 // ;: else if ((pcKeyCode == 59)) { ret =
		 * KeyEvent.KEYCODE_SEMICOLON; }
		 * 
		 * // -_ else if ((pcKeyCode == 173)) { ret = KeyEvent.KEYCODE_MINUS; }
		 * 
		 * // = + else if ((pcKeyCode == 61)) { ret = KeyEvent.KEYCODE_EQUALS;
		 * //ret = KeyEvent.KEYCODE_PLUS; }
		 */

		// ;:
		else if ((pcKeyCode == 140)) {
			ret = KeyEvent.KEYCODE_SEMICOLON;
		}

		// -_
		else if ((pcKeyCode == 143)) {
			ret = KeyEvent.KEYCODE_MINUS;
		}

		// = +
		else if ((pcKeyCode == 141)) {
			ret = KeyEvent.KEYCODE_EQUALS;
			// ret = KeyEvent.KEYCODE_PLUS;
		}

		// Backspace
		else if ((pcKeyCode == 2)) {
			ret = KeyEvent.KEYCODE_BACK;
		}

		// Left
		else if ((pcKeyCode == 23)) {
			ret = KeyEvent.KEYCODE_DPAD_LEFT;
		}

		// Up
		else if ((pcKeyCode == 24)) {
			ret = KeyEvent.KEYCODE_DPAD_UP;
		}

		// Right
		else if ((pcKeyCode == 25)) {
			ret = KeyEvent.KEYCODE_DPAD_RIGHT;
		}

		// Down
		else if ((pcKeyCode == 26)) {
			ret = KeyEvent.KEYCODE_DPAD_DOWN;
		}

		/* 小键盘 */

		// 小键盘按键'0~9'

		else if ((pcKeyCode >= 74) && (pcKeyCode <= 83)) {
			ret = pcKeyCode - (74 - KeyEvent.KEYCODE_0);
		}

		// 小键盘按键'+'
		else if ((pcKeyCode == 85)) {
			ret = KeyEvent.KEYCODE_NUMPAD_ADD;
		}

		// 小键盘按键'-'
		else if ((pcKeyCode == 87)) {
			ret = KeyEvent.KEYCODE_NUMPAD_SUBTRACT;
		}

		// 小键盘按键'*'
		else if ((pcKeyCode == 84)) {
			ret = KeyEvent.KEYCODE_NUMPAD_MULTIPLY;
		}

		// 小键盘按键'/'
		else if ((pcKeyCode == 86)) {
			ret = KeyEvent.KEYCODE_NUMPAD_DIVIDE;
		}

		// 小键盘按键'.'
		else if ((pcKeyCode == 88)) {
			// ret = KeyEvent. KEYCODE_NUMPAD_DOT;
			ret = KeyEvent.KEYCODE_PERIOD;
		}

		// 小键盘按键'Enter'
		else if ((pcKeyCode == 13)) {
			ret = KeyEvent.KEYCODE_NUMPAD_ENTER;
		}
		return ret;
	}
}
