using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Text;

namespace ScreenCast.Client
{
    public delegate void ProcessDataFunClass(byte[] data);

    class DataManager:ProcessData
    {
        const int DATA_TYPE_MOUSE = 1;
        const int DATA_TYPE_KEYBOARD = 2;
        const int DATA_TYPE_CTRL = 3;
        const int DATA_TYPE_HEARTBEAT_REQUEST = 4;
        const int DATA_TYPE_HEARTBEAT_RESPONSE = 5;

        static Client client = null;

        static ProcessDataFunClass proFun = null;

        public DataManager(int recvDataLen = 1024, int port = 8888)
        {
            client = new Client(recvDataLen, port);
            client.RegistProcessObj(this);  //regist client
        }

        public void run(string ip)  //new a object and run this method
        {
            client.mainThread(ip);
        }


        ~DataManager()
        {
            client = null;
        }

        public void exit()
        {
            client.exit();
        }

        static public int registProcessObj(ProcessDataFunClass proFun)
        {
            DataManager.proFun = proFun;

            return 0;
        }

        //重载父类虚函数
        public override void processData(byte[] data, int dataLen)
        {
            int dataType = BitConverter.ToInt32(data, 4);

            byte[] realData = new byte[dataLen - 8];

            Array.Copy(data, 8, realData, 0, realData.Length);

            switch (dataType)
            {
                case DATA_TYPE_MOUSE:

                    break;

                case DATA_TYPE_KEYBOARD:

                    break;

                case DATA_TYPE_CTRL: //处理接收到的控制信息
                    proFun(realData);

                    break;

                case DATA_TYPE_HEARTBEAT_REQUEST:    //心跳回应包
                    //Trace.WriteLine("******************************jie shou dao  xin tiao bao **************");
                    // string value = BitConverter.ToString(realData);
                    //Trace.WriteLine("****************************" + value + "*************************");

                    //sendHeartBeatPackage(new byte[0]);
                    //sendHeartBeatPackage(realData);

                    break;

                default:
                    //Error Message
                    break;

            }

        }

        static private byte[] addType(byte[] data, int type)
        {
            byte[] typeData = BitConverter.GetBytes(type);
            byte[] newData = new byte[data.Length + typeData.Length];
            typeData.CopyTo(newData, 0);
            data.CopyTo(newData, typeData.Length);

            return newData;
        }


        //发送心跳包信息
        static private int sendHeartBeatPackage(byte[] data)
        {
            if (null == client)
            {
                return -1;
            }

            byte[] newData = addType(data, DATA_TYPE_HEARTBEAT_RESPONSE);

            return client.mySend(newData);

        }

        //发送鼠标相关数据
        static public int sendMouseData(byte[] data)
        {
            if (null == client)
            {
                Trace.WriteLine("sendMouseData msg **************************");
                return -1;
            }
            byte[] newData = addType(data, DATA_TYPE_MOUSE);

            return client.mySend(newData);


        }

        //发送键盘相关信息
        static public int sendKeyBoardData(byte[] data)
        {
            if (null == client)
            {
                Trace.WriteLine("Server in null: ");
                return -1;
            }
            byte[] newData = addType(data, DATA_TYPE_KEYBOARD);

            return client.mySend(newData);

        }

        //发送控制相关信息
        static public int sendCtrlData(byte[] data)
        {
            if (null == client)
            {
                return -1;
            }
            byte[] newData = DataManager.addType(data, DATA_TYPE_CTRL);

            return client.mySend(newData);
        }

        static public int sendRawData(byte[] data)
        {
            if (null == client)
            {
                return -1;
            }
            return client.mySend(data);
        }
    }
}
