/*
Name: ServerSocket.cpp
*/

#include "ServerSocket.h"
#include "Socket.h"

using namespace android;

ServerSocket::ServerSocket(int port){

}

static int ServerSocket::ServerStatus(int port){
	Socket *serverSocket = new Socket();
	if(!serverSocket->create()){
		return SOCKET_CREATE_ERR;
	}
	if(!serverSocket->bind(port, "127.0.0.1")){
		return SOCKET_BIND_ERR;
	}
	if(!serverSocket->listen()){
		return SOCKET_LISTEN_ERR;
	}
	return SOCKET_OK;
}

ServerSocket::~ServerSocket(){
}

