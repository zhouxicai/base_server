//#include "stdafx.h"
//////////////////////////////////////////////////////////////////////
// 
// Socket.cpp
// 
// 
//////////////////////////////////////////////////////////////////////

#include "SocketManager.h"

SocketManager::SocketManager()
{
	__ENTER_FUNCTION_FOXNET

		m_SocketID = INVALID_SOCKET;
	memset(&m_SockAddr, 0, sizeof(SOCKADDR_IN));
	memset(m_Host, 0, IP_SIZE);
	m_Port = 0;

	__LEAVE_FUNCTION_FOXNET
}

SocketManager::SocketManager(const CHAR* host, UINT port)
{
	__ENTER_FUNCTION_FOXNET

		strncpy(m_Host, host, IP_SIZE - 1);
	m_Port = port;

	create();

	__LEAVE_FUNCTION_FOXNET
}

SocketManager::~SocketManager()
{
	__ENTER_FUNCTION_FOXNET

		close();

	__LEAVE_FUNCTION_FOXNET
}

BOOL SocketManager::Init(UINT port, UINT backlog)
{
	__ENTER_FUNCTION_FOXNET

	BOOL ret = FALSE;

	// create socket
	ret = this->create();
	if (ret == FALSE)
		throw 1;
	//	Assert( ret ) ;

		// reuse address before Bind()
		// 바인드 하기 전에 주소 리유즈를 시스템에 알려야 한다.
	ret = this->setReuseAddr();
	if (ret == FALSE)
		throw 1;
	//	Assert( ret ) ;

		// bind address to socket
		// 이미 port가 m_Impl에 저장되어 있으므로, 파라미터없는 Bind()를 호출해도 된다.
	ret = this->bind(port);
	if (ret == FALSE)
		throw 1;
	//	Assert( ret ) ;

	//	m_Impl->setSendBufferSize( 9000 );
	//	m_Impl->setReceiveBufferSize( 9000 );

		// set listening queue size
	ret = this->listen(backlog);
	if (ret == FALSE)
		throw 1;
	//	Assert( ret ) ;
	return ret;
	__LEAVE_FUNCTION_FOXNET
}

BOOL SocketManager::create()
{
	__ENTER_FUNCTION_FOXNET

		m_SocketID = SocketAPI::socket_ex(AF_INET, SOCK_STREAM, 0);

	memset(&m_SockAddr, 0, sizeof(m_SockAddr));

	m_SockAddr.sin_family = AF_INET;

	if (isValid())
		return TRUE;
	else
		return FALSE;

	__LEAVE_FUNCTION_FOXNET

		return FALSE;
}

// close previous connection and connect to another server socket
BOOL SocketManager::reconnect(const CHAR* host, UINT port)
{
	__ENTER_FUNCTION_FOXNET

		// delete old socket impl object
		close();

	// create new socket impl object
	strncpy(m_Host, host, IP_SIZE - 1);
	m_Port = port;

	create();

	// try to connect
	return connect();

	__LEAVE_FUNCTION_FOXNET

		return FALSE;
}

VOID SocketManager::close()
{
	__ENTER_FUNCTION_FOXNET

		//	if( !isSockError() ) 
		if (isValid() && !isSockError())
		{
			_MY_TRY
			{
				SocketAPI::closesocket_ex(m_SocketID);
			}
				_MY_CATCH
			{
			}
		}

	m_SocketID = INVALID_SOCKET;
	memset(&m_SockAddr, 0, sizeof(SOCKADDR_IN));
	memset(m_Host, 0, IP_SIZE);
	m_Port = 0;

	__LEAVE_FUNCTION_FOXNET
}

BOOL SocketManager::connect()
{
	__ENTER_FUNCTION_FOXNET

		m_SockAddr.sin_addr.s_addr = inet_addr(m_Host);

	// set sockaddr's port
	m_SockAddr.sin_port = htons(m_Port);

	// try to connect to peer host
	BOOL result = SocketAPI::connect_ex(m_SocketID, (const struct sockaddr *)&m_SockAddr, sizeof(m_SockAddr));
	if (result)
		return TRUE;
	else
		return FALSE;

	__LEAVE_FUNCTION_FOXNET

		return 0;
}

BOOL SocketManager::connect(const CHAR* host, UINT port)
{
	__ENTER_FUNCTION_FOXNET

		strncpy(m_Host, host, IP_SIZE - 1);
	m_Port = port;

	return connect();

	__LEAVE_FUNCTION_FOXNET

		return 0;
}

UINT SocketManager::send(const VOID* buf, UINT len, UINT flags)
{
	__ENTER_FUNCTION_FOXNET

		return SocketAPI::send_ex(m_SocketID, buf, len, flags);

	__LEAVE_FUNCTION_FOXNET

		return 0;
}

UINT SocketManager::receive(VOID* buf, UINT len, UINT flags)
{
	__ENTER_FUNCTION_FOXNET

		return SocketAPI::recv_ex(m_SocketID, buf, len, flags);

	__LEAVE_FUNCTION_FOXNET

		return 0;
}

UINT SocketManager::available()const
{
	__ENTER_FUNCTION_FOXNET

		return SocketAPI::availablesocket_ex(m_SocketID);

	__LEAVE_FUNCTION_FOXNET

		return 0;
}

UINT SocketManager::getLinger()const
{
	__ENTER_FUNCTION_FOXNET

		struct linger ling;
	UINT len = sizeof(ling);

	SocketAPI::getsockopt_ex(m_SocketID, SOL_SOCKET, SO_LINGER, &ling, &len);

	return ling.l_linger;

	__LEAVE_FUNCTION_FOXNET

		return 0;
}

BOOL SocketManager::Accept()
{
	__ENTER_FUNCTION_FOXNET

		UINT addrlen = sizeof(SOCKADDR_IN);
	this->close();

	this->m_SocketID = this->accept((struct sockaddr *)(&(this->m_SockAddr)), &addrlen);
	if (this->m_SocketID == INVALID_SOCKET)
		return FALSE;

	this->m_Port = ntohs(this->m_SockAddr.sin_port);
	strncpy(this->m_Host, inet_ntoa(this->m_SockAddr.sin_addr), IP_SIZE - 1);

	return TRUE;

	__LEAVE_FUNCTION_FOXNET

		return FALSE;
}

SOCKET SocketManager::accept(struct sockaddr* addr, UINT* addrlen)
{
	__ENTER_FUNCTION_FOXNET

		return SocketAPI::accept_ex(m_SocketID, addr, addrlen);

	__LEAVE_FUNCTION_FOXNET

		return INVALID_SOCKET;
}

BOOL SocketManager::setLinger(UINT lingertime)
{
	__ENTER_FUNCTION_FOXNET

		struct linger ling;

	ling.l_onoff = lingertime > 0 ? 1 : 0;
	ling.l_linger = lingertime;

	return SocketAPI::setsockopt_ex(m_SocketID, SOL_SOCKET, SO_LINGER, &ling, sizeof(ling));

	__LEAVE_FUNCTION_FOXNET

		return FALSE;
}

UINT SocketManager::getSockError()const
{
	__ENTER_FUNCTION_FOXNET

		return isSockError();

	__LEAVE_FUNCTION_FOXNET

		return 0;
}

BOOL SocketManager::isNonBlocking()const
{
	__ENTER_FUNCTION_FOXNET

		return SocketAPI::getsocketnonblocking_ex(m_SocketID);

	__LEAVE_FUNCTION_FOXNET

		return FALSE;
}

BOOL SocketManager::setNonBlocking(BOOL on)
{
	__ENTER_FUNCTION_FOXNET

		return SocketAPI::setsocketnonblocking_ex(m_SocketID, on);

	__LEAVE_FUNCTION_FOXNET

		return FALSE;
}

UINT SocketManager::getReceiveBufferSize()const
{
	__ENTER_FUNCTION_FOXNET

		UINT ReceiveBufferSize;
	UINT size = sizeof(ReceiveBufferSize);

	SocketAPI::getsockopt_ex(m_SocketID, SOL_SOCKET, SO_RCVBUF, &ReceiveBufferSize, &size);

	return ReceiveBufferSize;

	__LEAVE_FUNCTION_FOXNET

		return 0;
}

BOOL SocketManager::setReceiveBufferSize(UINT size)
{
	__ENTER_FUNCTION_FOXNET

		return (BOOL)(SocketAPI::setsockopt_ex(m_SocketID, SOL_SOCKET, SO_RCVBUF, &size, sizeof(UINT)));

	__LEAVE_FUNCTION_FOXNET

		return FALSE;
}

UINT SocketManager::getSendBufferSize()const
{
	__ENTER_FUNCTION_FOXNET

		UINT SendBufferSize;
	UINT size = sizeof(SendBufferSize);

	SocketAPI::getsockopt_ex(m_SocketID, SOL_SOCKET, SO_SNDBUF, &SendBufferSize, &size);

	return SendBufferSize;

	__LEAVE_FUNCTION_FOXNET

		return 0;
}

BOOL SocketManager::setSendBufferSize(UINT size)
{
	__ENTER_FUNCTION_FOXNET

		return (BOOL)(SocketAPI::setsockopt_ex(m_SocketID, SOL_SOCKET, SO_SNDBUF, &size, sizeof(UINT)));

	__LEAVE_FUNCTION_FOXNET

		return FALSE;
}

UINT SocketManager::getPort()const
{
	__ENTER_FUNCTION_FOXNET

		return m_Port;

	__LEAVE_FUNCTION_FOXNET
}

IP_t SocketManager::getHostIP()const
{
	__ENTER_FUNCTION_FOXNET

		return (IP_t)(m_SockAddr.sin_addr.s_addr);;

	__LEAVE_FUNCTION_FOXNET
}

BOOL SocketManager::isValid()const
{
	__ENTER_FUNCTION_FOXNET

		return m_SocketID != INVALID_SOCKET;

	__LEAVE_FUNCTION_FOXNET
}

SOCKET SocketManager::getSOCKET()const
{
	__ENTER_FUNCTION_FOXNET

		return m_SocketID;

	__LEAVE_FUNCTION_FOXNET
}

BOOL SocketManager::isSockError()const
{
	__ENTER_FUNCTION_FOXNET

		INT error;
	UINT len = sizeof(error);

	INT Result = SocketAPI::getsockopt_ex2(m_SocketID, SOL_SOCKET, SO_ERROR, &error, &len);

	if (Result == 0)
		return FALSE;
	else
		return TRUE;

	__LEAVE_FUNCTION_FOXNET

		return FALSE;
}

BOOL SocketManager::bind()
{
	__ENTER_FUNCTION_FOXNET

		m_SockAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	m_SockAddr.sin_port = htons(m_Port);

	BOOL result = SocketAPI::bind_ex(m_SocketID, (const struct sockaddr *)&m_SockAddr, sizeof(m_SockAddr));
	if (result)
		return TRUE;
	else
		return FALSE;

	__LEAVE_FUNCTION_FOXNET

		return 0;
}

BOOL SocketManager::bind(UINT port)
{
	__ENTER_FUNCTION_FOXNET

		m_Port = port;

	m_SockAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	m_SockAddr.sin_port = htons(m_Port);

	BOOL result = SocketAPI::bind_ex(m_SocketID, (const struct sockaddr *)&m_SockAddr, sizeof(m_SockAddr));
	if (result)
		return TRUE;
	else
		return FALSE;

	__LEAVE_FUNCTION_FOXNET

		return 0;
}

BOOL SocketManager::listen(INT backlog)
{
	__ENTER_FUNCTION_FOXNET

		return SocketAPI::listen_ex(m_SocketID, backlog);

	__LEAVE_FUNCTION_FOXNET

		return FALSE;
}

BOOL SocketManager::isReuseAddr()const
{
	__ENTER_FUNCTION_FOXNET

		INT reuse;
	UINT len = sizeof(reuse);

	SocketAPI::getsockopt_ex(m_SocketID, SOL_SOCKET, SO_REUSEADDR, &reuse, &len);

	return reuse == 1;

	__LEAVE_FUNCTION_FOXNET

		return 0;
}

BOOL SocketManager::setReuseAddr(BOOL on)
{
	__ENTER_FUNCTION_FOXNET

		INT opt = on == TRUE ? 1 : 0;

	return SocketAPI::setsockopt_ex(m_SocketID, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

	__LEAVE_FUNCTION_FOXNET

		return TRUE;
}

