package android.app;

public abstract class VKeyboardManager {
	public abstract boolean inputKeyevent(long downtime,long eventtime,int action,int code,int repeat,int metastate,int flag);
	public abstract boolean clickTouch(int x, int y,boolean isvios);   //isvios default value is false
}
