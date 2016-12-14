package com.screencap;

import java.io.BufferedReader;
import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.File;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.util.ArrayList;

/** Shell工具类 */
public final class ShellUtil {
	private static ShellUtil instance = null;

	private ShellUtil() {

		// Exists only to defeat instantiation.
	}

	/** 返回ShellUtil的单例 */
	public static ShellUtil getInstance() {
		if (instance == null) {
			instance = new ShellUtil();
			instance.root();
		}
		return instance;
	}

	/** \link #root()\endlink后的进程 */
	private Process process;

	/** \link #root()\endlink后的父进程的标准输入 */
	private DataOutputStream dos;
	private DataInputStream in;
	
	/** 标准输出的过滤 */
	private IStdoutFilter<String> mIStdoutFilter;

	/** 设置标准输出的过滤器 */
	public void setFilter(IStdoutFilter<String> filter) {
		this.mIStdoutFilter = filter;
	}

	/** 重置过滤器为空 */
	public void resetFilter() {
		this.mIStdoutFilter = null;
	}

	/**
	 * @brief 切换至ROOT用户
	 * @details 执行su命令，变更为root用户
	 * @pre 设备已经破解，否则su不可用
	 * 
	 * @return 是否成功
	 */
	public boolean root() {
		try {
			// 执行su变更用户身份为root
			process = Runtime.getRuntime().exec("su");
			
			// 转成DataOutputStream方便写入字符串
			dos = new DataOutputStream(process.getOutputStream());
			in = new DataInputStream(process.getInputStream()); 
			/*new Thread(new Runnable() {
				public void run() {
					while (true) {
						readInputStream();
					}
				}
			}).start();*/
			
		} catch (Exception e) {
			e.printStackTrace();
			return false;
		}
		return true;
	}

	/**
	 * @brief ROOT权限下执行命令
	 * @pre 执行\link #root()\endlink
	 * 
	 * @param cmd
	 *            命令
	 */
	public boolean rootCommand(String cmd) {
		if (null != dos) {
			try {
				dos.writeBytes(cmd);
				dos.flush();
			} catch (IOException e) {
				e.printStackTrace();
				return false;
			}
			return true;
		}
		return false;
	}
	
	public void readInputStream() {
		
		 BufferedReader reader = new BufferedReader(new InputStreamReader(in));  
         int read;  
         char[] buffer = new char[1024*1024];  
         StringBuffer output = new StringBuffer();  
         try {
			while ((read = reader.read(buffer)) > 0) {  
			     output.append(buffer, 0, read);  
			 }
			 reader.close();  
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}  
	}
	
	// /**
	// * @brief \link #rootCommand()\endlink后的结果
	// * @pre 执行\link #rootCommand()\endlink
	// *
	// * @warning 不能在stdin流输入命令后再从stdout获输出结果
	// * （之前测试版也放在不同位置试过，都不成，死锁？没找到更多资料）
	// *
	// * @return 输出结果的集合
	// */
	// public ArrayList<String> getStdout() {
	// ArrayList<String> lineArray = new ArrayList<String>();
	// try {
	// handleStdout(lineArray, process);
	// } catch (IOException e) {
	// e.printStackTrace();
	// }
	// return lineArray;
	// }

	/** 释放占用资源 */
	public boolean rootRelease() {
		try {
			dos.writeBytes("exit\n");
			dos.flush();
			process.waitFor(); // 等待执行完成
		} catch (Exception e) {
			e.printStackTrace();
			return false;
		} finally {
			try {
				if (null != process) {
					process.destroy();
				}
				if (null != dos) {
					dos.close();
				}
			} catch (Exception e) {
				e.printStackTrace();
			}
		}
		return true;
	}

	/**
	 * @brief 执行一个shell命令
	 * 
	 * @param cmd
	 *            命令&参数组成的数组
	 * @param workDir
	 *            命令工作目录
	 * @param isStdout
	 *            是否输出结果
	 * @return 输出结果的集合
	 */
	public ArrayList<String> execCommand(String[] cmd, String workDir,
			boolean isStdout) {
		ArrayList<String> lineArray = null;
		try {
			// 创建操作系统进程（也可以由Runtime.exec()启动）
			ProcessBuilder builder = new ProcessBuilder(cmd);
			// 设置命令工作目录
			if (workDir != null) {
				builder.directory(new File(workDir));
			}
			// 合并标准错误和标准输出
			builder.redirectErrorStream(true);
			// 启动一个新进程
			Process process = builder.start();

			// 如果输出结果的话
			if (isStdout) {
				lineArray = new ArrayList<String>(); // 创建对象
				handleStdout(lineArray, process);
			}
		} catch (Exception e) {
			e.printStackTrace();
		}
		return lineArray;
	}

	/**
	 * 处理标准输出内容
	 * 
	 * @throws IOException
	 */
	private void handleStdout(ArrayList<String> lineArray, Process process)
			throws IOException {
		InputStream is = process.getInputStream(); // 获得标准输出流
		if (null != mIStdoutFilter) { // 如果设置了过滤
			// 判断是否是行过滤器
			if (mIStdoutFilter instanceof AbstractLineFilter) {
				// 转成BufferedReader
				BufferedReader br = new BufferedReader(
						new InputStreamReader(is));
				String line;
				while (null != (line = br.readLine())) {
					/* 如果未被过滤，则将处理后内容加入List */
					if (!mIStdoutFilter.filter(line)) {
						lineArray.add(mIStdoutFilter.handle());
					}
				}
				if (br != null) {
					br.close();
				}
			} else {
				// 默认把流直接转成字符串返回
				lineArray.add(inputStream2Str(is));
			}
		} else {
			// 默认把流直接转成字符串返回
			lineArray.add(inputStream2Str(is));
		}
		if (is != null) {
			is.close();
		}
	}

	/**
	 * 输入流转成字符串
	 * 
	 * @throws IOException
	 */
	public String inputStream2Str(InputStream is) throws IOException {
		StringBuffer out = new StringBuffer();
		byte[] b = new byte[4096];
		for (int n; (n = is.read(b)) != -1;) {
			out.append(new String(b, 0, n));
		}
		return out.toString();
	}

}