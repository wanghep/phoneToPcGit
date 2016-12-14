/*
Name: ServerSocket.cpp
*/

#ifndef SERVER_SOCKET_H
#define	SERVER_SOCKET_H

#define SOCKET_CREATE_ERR     0x01
#define SOCKET_BIND_ERR       0x02
#define SOCKET_LISTEN_ERR     0x03
#define SOCKET_OK             0


class ServerSocket{
public:
	ServerSocket(){};
	ServerSocket(int port); 
	static int ServerStatus(int port);
	virtual ~ServerSocket();
};


#endif
