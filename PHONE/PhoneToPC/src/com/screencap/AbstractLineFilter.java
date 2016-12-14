package com.screencap;

/**
 * @brief 抽象的行过滤器
 * @details 以行的方式遍历标准输出，都进行一次过滤判断
 */
public abstract class AbstractLineFilter implements IStdoutFilter<String> {

	/** 行内容 */
	protected String line;

	/**
	 * @brief 行过滤操作
	 * @param line
	 *            标准输出的某行内容
	 * @return true：过滤；false：保留
	 */
	protected abstract boolean lineFilter(String line);

	public boolean filter(String stdout) {
		this.line = stdout;
		return lineFilter(stdout);
	}

	public String handle() {
		return line; // 默认返回原行
	}

}