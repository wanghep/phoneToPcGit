using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Linq;
using System.Net.Sockets;
using System.Text;
using System.Threading;
using System.Windows;

namespace ScreenCast.Client
{
    class Client
    {


        Socket clientSocket = null;
        Thread readThread = null;
        Thread writeThread = null;
        int port;
        int count = 0;
        bool isConn = false;
        int recvDataLen;

        byte[] sendData;
        bool sendFlag;
        ProcessData processDataObj;

        static string mylockstr = "mylock";

        bool exitFlag;


        public Client(int recvDataLen = 1024, int port = 8888)
        {
            this.recvDataLen = recvDataLen;
            this.port = port;

            sendData = null;
            sendFlag = false;
            exitFlag = false;
        }

        public void exit()
        {
            exitFlag = true;

            if (clientSocket != null)
            {
                clientSocket.Close();
            }

            //try
            //{
            //    readThread.Abort();
            //    writeThread.Abort();
            //}
            //catch (Exception exe)
            //{
            //    Trace.WriteLine(exe.ToString());
            //}

            clientSocket = null;
        }


        ~Client()
        {
            exit();
        }

        private void writeLog(string content)
        {
            using (StreamWriter sw = new StreamWriter(@"C:\Users\Alex Zhou\Desktop\log.txt", true))
            {
                sw.WriteLine(content);
            }
        }


        public void mainThread(string remoteIp)
        {
            while (!exitFlag)
            {
                if (!isConn)
                {
                    try
                    {
                        readThread.Abort();
                        writeThread.Abort();
                    }
                    catch (Exception exe)
                    {
                        Trace.WriteLine(exe.ToString());
                    }

                    clientSocket = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);

                    try
                    {
                        clientSocket.Connect(remoteIp, port);
                    }
                    catch (Exception exe)
                    {
                         writeLog("**************** connect failed **********************");
                        Trace.WriteLine(exe.ToString());
                        clientSocket.Close();
                        Thread.Sleep(1000);
                        continue;
                    }

                    isConn = true;
                    count++;

                    Trace.WriteLine("connect successful" + "*****" + count + "******");

                    writeLog("connect successful"+"*****"+count+"******");

                    readThread = new Thread(new ParameterizedThreadStart(readThreadFun));
                    readThread.IsBackground = true;
                    readThread.Start(clientSocket);

                    writeThread = new Thread(new ParameterizedThreadStart(writeThreadFun));
                    writeThread.IsBackground = true;
                    writeThread.Start(clientSocket);

                }
                else
                {
                    Thread.Sleep(2000);
                }
            }
        }


        //注册函数(这个函数用与处理socket接收到的数据)
        public void RegistProcessObj(ProcessData processDataObj)
        {
            this.processDataObj = processDataObj;
        }

        public void handleDataFromClient(byte[] data, int num)
        {
            //Trace.WriteLine("!!!!!!!!!!!!go here");
            // byte[] buffer = new byte[128]; 
            //int n = clientSocket.Receive(buffer);
            //MessageBox.Show("来自phone的数据到这里啦");
            String phoneData = Encoding.UTF8.GetString(data, 0, num);
            Trace.WriteLine("!!!!!!!!!!!!!!!!!!!!!!!!,phoneData = " + phoneData);
            putDataToPCClipBoard(phoneData);

        }
        public void putDataToPCClipBoard(String phoneData)
        {
            Trace.WriteLine(phoneData);
            Clipboard.SetText(phoneData);
            // MessageBox.Show("已成功将文本框内容复制到剪贴板!");
        }


        private void readThreadFun(object obj)
        {
            Socket nowSocket = (Socket)obj;
            int len;
            byte[] data = new byte[recvDataLen];

            while (!exitFlag)
            {
                try
                {
                    len = nowSocket.Receive(data);

                    if (len != 0)
                    {
                        //processDataObj.processData(data, len);
                        Trace.WriteLine("*****hanle clip data from phone,clientSocket = " + clientSocket.ToString());
                        handleDataFromClient(data, len);
                    }
                    else
                    {
                        Trace.WriteLine("read client disconnected***");
                        isConn = false;
                        break;
                    }
                }
                catch (Exception e)
                {
                    Trace.WriteLine(e);
                    isConn = false;
                    break;
                }
            }
            isConn = false;
            nowSocket.Close();
        }

        public int mySend(byte[] data)
        {

            if (writeThread == null)
            {
                Trace.WriteLine("writeThread == null");
                return -2;
            }

            lock (mylockstr)
            {
                if (!sendFlag)
                {
                    Trace.WriteLine("mySend****");

                    //writeLog("mySend****");

                    sendData = new byte[data.Length + 4];
                    byte[] len = BitConverter.GetBytes(data.Length);

                    len.CopyTo(sendData, 0);
                    data.CopyTo(sendData, 4);
                    sendFlag = true;
                }
            }
            return 0;
        }


        private void writeThreadFun(object obj)
        {
            Socket nowSocket = (Socket)obj;
            while (!exitFlag)
            {
                lock (mylockstr)
                {
                    if (sendFlag)
                    {
                        try
                        {
                            Trace.WriteLine("clientSocket.Send");

                            nowSocket.Send(sendData, sendData.Length, SocketFlags.None);

                            //writeLog("**********clientSocket.Send*************************");

                        }
                        catch (Exception e)
                        {
                            Trace.WriteLine("*********************FA SONG SHI BAI *******************");
                            Trace.WriteLine(e.ToString());
                            sendFlag = false;
                            sendData = null;
                            break;
                        }

                        sendFlag = false;
                        sendData = null;
                    }
                }
            }

            sendFlag = false;
            sendData = null;
            isConn = false;
            nowSocket.Close();
        }
    }
}
