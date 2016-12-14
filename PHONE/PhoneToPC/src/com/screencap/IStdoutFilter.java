package com.screencap;

/** 标准输出过滤接口 */
public interface IStdoutFilter<T> {

	/**
	 * @brief 过滤操作
	 * @param stdout
	 *            标准输出的内容
	 * @return true：过滤；false：保留
	 */
	boolean filter(T stdout);

	/**
	 * @brief 处理操作
	 * @return 处理后的内容
	 */
	T handle();

}