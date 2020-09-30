#pragma once
//
//文件名称：	Socket.h
//功能描述：	封装网络Socket的功能，通过接口实现所有的网络操作
//				
//

#include "BaseType.h"
#include "SocketAPI.h"
//////////////////////////////////////////////////////////////////////////////
// class Socket
//
// TCP Client Socket
//
// 
//
// 
//////////////////////////////////////////////////////////////////////////////

class SocketManager
{
	//////////////////////////////////////////////////
	// constructor/destructor
	//////////////////////////////////////////////////
public:

	// constructor
	SocketManager();
	SocketManager(const CHAR* host, UINT port);

	// destructor
	virtual ~SocketManager();
	//////////////////////////////////////////////////
	// API
	//////////////////////////////////////////////////
public:
	BOOL Init(UINT port, UINT backlog = 5);

	// accept new connection
	BOOL Accept();
	//////////////////////////////////////////////////
	// methods
	//////////////////////////////////////////////////
public:
	BOOL Create();

	// close connection
	VOID Close();

	// try connect to remote host
	BOOL Connect();
	BOOL Connect(const CHAR* host, UINT port);

	// close previous connection and connect to another socket
	BOOL Reconnect(const CHAR* host, UINT port);

	// send data to peer
	UINT Send(const VOID* buf, UINT len, UINT flags = 0);

	// receive data from peer
	UINT Receive(VOID* buf, UINT len, UINT flags = 0);

	UINT Available()const;

	SOCKET Accept(struct sockaddr* addr, UINT* addrlen);

	BOOL Bind();
	BOOL Bind(UINT port);

	BOOL Listen(INT backlog);

	//////////////////////////////////////////////////
	// methods
	//////////////////////////////////////////////////
public:

	// get/set socket's linger status
	UINT GetLinger()const;
	BOOL SetLinger(UINT lingertime);

	BOOL IsReuseAddr()const;
	BOOL SetReuseAddr(BOOL on = TRUE);

	// get/set socket's nonblocking status
	BOOL IsNonBlocking()const;
	BOOL SetNonBlocking(BOOL on = TRUE);

	// get/set receive buffer size
	UINT GetReceiveBufferSize()const;
	BOOL SetReceiveBufferSize(UINT size);

	// get/set send buffer size
	UINT GetSendBufferSize()const;
	BOOL SetSendBufferSize(UINT size);

	UINT GetPort()const;
	IP_t GetHostIP()const;

	// check if socket is valid
	BOOL IsValid()const;

	// get socket descriptor
	SOCKET GetSOCKET()const;

	BOOL IsSockError()const;

public:

	SOCKET m_SocketID;

	// socket address structure
	SOCKADDR_IN m_SockAddr;

	// peer host
	CHAR m_Host[IP_SIZE];

	// peer port
	UINT m_Port;


};
