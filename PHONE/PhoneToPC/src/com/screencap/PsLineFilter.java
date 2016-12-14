package com.screencap;

/**
 * ps命令的行过滤及处理成pid的实现
 */
public final class PsLineFilter extends AbstractLineFilter {

	@Override
	protected boolean lineFilter(String line) {
		// 过滤空行及标题行
		if (null == line || "".endsWith(line) || line.startsWith("USER")) {
			return true;
		}
		return false;
	}

	@Override
	public String handle() {
		try {
			return line.trim().split("\\s+")[1]; // 获取PID列
		} catch (Exception e) { // null和越界异常
			return line;
		}
	}

}