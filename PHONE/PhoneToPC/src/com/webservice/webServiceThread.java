package com.webservice;
import org.eclipse.jetty.server.Handler;
import org.eclipse.jetty.server.Server;
import org.eclipse.jetty.server.handler.HandlerCollection;
import org.eclipse.jetty.server.handler.ResourceHandler;
import org.eclipse.jetty.servlet.ServletContextHandler;
import org.eclipse.jetty.servlet.ServletHolder;

import android.os.Environment;
import android.util.Log;




/**
 * 功能：用于socket的交互
 * 用于新socket连接的读写线程类
 * @author wufenglong
 * 
 */
	
public class webServiceThread implements Runnable {
	
	Server server = null;
	@Override
	public void run() {

		server = new Server(8080);
		//Servlet
		ServletContextHandler servletContextHandler = new ServletContextHandler(
				ServletContextHandler.SESSIONS);
		servletContextHandler.setContextPath("/");
		ServletHolder sh = new ServletHolder(HciServlet.class);
		servletContextHandler.addServlet(sh, "/hci");
		//server.setHandler(servletContextHandler);
		
		//文件服务器
		ResourceHandler resourceHandler = new ResourceHandler();
		resourceHandler.setDirectoriesListed(true);
		resourceHandler.setResourceBase(Environment.getExternalStorageDirectory().toString()+"/ScreenshotsImages/");
		resourceHandler.setStylesheet("");//如果没有这一行代码的话，在请求资源的时候后台总是会不停的抛出NullPointerException

		HandlerCollection handlers = new HandlerCollection();
		handlers.setHandlers(new Handler[] {resourceHandler,servletContextHandler});
		server.setHandler(handlers); 
		
		try {
			server.start();
			server.join();
		} catch (Exception e) {
			Log.e(this.getClass().getName(), "start server error:" + e.getMessage() );
			e.printStackTrace();
		}
		
		
	}
	
	public void stop()
	{

		if( server!= null )
		{
			try {
				server.stop();
			} catch (Exception e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
		}
	}
	
}


