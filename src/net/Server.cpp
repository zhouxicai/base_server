
#include "Server.h"
//#include "Config.h"
//#include "Log.h"
//#include "PlayerPool.h"
//#include "Config.h"
//#include "TimeManager.h"
//#include "PacketFactoryManager.h"
//#include "OnlineUser.h"
//#include "Team.h"
//#include "ChatCenter.h"
//#include "GuildManager.h"
//#include "CityManager.h"
//#include "MailCenter.h"
//#include "SceneInfo.h"
//#include "WorldTimeInfo.h"


#define ACCEPT_ONESTEP 50


Server* g_pServerManager = NULL ;

Server::Server( )
{
__ENTER_FUNCTION

	FD_ZERO( &m_ReadFDs[SELECT_BAK] ) ;
	FD_ZERO( &m_WriteFDs[SELECT_BAK] ) ;
	FD_ZERO( &m_ExceptFDs[SELECT_BAK] ) ;

	m_Timeout[SELECT_BAK].tv_sec = 0;
	m_Timeout[SELECT_BAK].tv_usec = 0;

	m_MinFD = m_MaxFD = INVALID_SOCKET ;

	m_iFDSize = 0 ;
	
	SetActive(TRUE) ;


__LEAVE_FUNCTION
}

Server::~Server( )
{
__ENTER_FUNCTION

	SAFE_DELETE( m_pServerSocket ) ;

__LEAVE_FUNCTION
}

BOOL Server::Init( )
{
__ENTER_FUNCTION

	//hxj
	m_pServerSocket = new SocketManager(  ) ;
//	Assert( m_pServerSocket ) ;
	m_pServerSocket->Init(999);
	m_pServerSocket->SetNonBlocking() ;

	m_SocketID = m_pServerSocket->GetSOCKET() ;
//	Assert( m_SocketID != INVALID_SOCKET ) ;

	FD_SET(m_SocketID , &m_ReadFDs[SELECT_BAK]);
	FD_SET(m_SocketID , &m_ExceptFDs[SELECT_BAK]);

	m_MinFD = m_MaxFD = m_SocketID;

	m_Timeout[SELECT_BAK].tv_sec = 0;
	m_Timeout[SELECT_BAK].tv_usec = 0;

//	m_ThreadID = MyGetCurrentThreadID( ) ;

	//for( int i=0; i<OVER_MAX_SERVER; i++ )
	//{
	//	m_aServerHash[i] = INVALID_ID ;
	//}

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

BOOL Server::Select( )
{
__ENTER_FUNCTION

	m_Timeout[SELECT_USE].tv_sec  = m_Timeout[SELECT_BAK].tv_sec;
	m_Timeout[SELECT_USE].tv_usec = m_Timeout[SELECT_BAK].tv_usec;

	m_ReadFDs[SELECT_USE]   = m_ReadFDs[SELECT_BAK];
	m_WriteFDs[SELECT_USE]  = m_WriteFDs[SELECT_BAK];
	m_ExceptFDs[SELECT_USE] = m_ExceptFDs[SELECT_BAK];

//	MySleep(100) ;

	_MY_TRY 
	{
		int iRet = SocketAPI::select_ex(	(int)m_MaxFD+1 , 
											&m_ReadFDs[SELECT_USE] , 
											&m_WriteFDs[SELECT_USE] , 
											&m_ExceptFDs[SELECT_USE] , 
											&m_Timeout[SELECT_USE] ) ;
//		Assert( iRet!=SOCKET_ERROR ) ;
	} 
	_MY_CATCH
	{
//		Log::SaveLog( WORLD_LOGFILE, "ERROR: Server::Select( )..." ) ;
	}

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

BOOL Server::RemovePlayer(/* Player* pPlayer */)
{
__ENTER_FUNCTION

	BOOL ret = FALSE ;

	return ret ;

__LEAVE_FUNCTION

	return FALSE ;
}

BOOL Server::ProcessInputs( )
{
__ENTER_FUNCTION

	BOOL ret = FALSE ;

	if (m_MinFD == INVALID_SOCKET && m_MaxFD == INVALID_SOCKET) // no player exist
	{ 
		return TRUE ;
	}

	//新连接接入：
	if( FD_ISSET(m_SocketID,&m_ReadFDs[SELECT_USE]) )
	{
		for( int i=0; i<ACCEPT_ONESTEP; i++ )
		{
			if( !AcceptNewConnection() )
				break;
		}
	}

	//数据读取
	//uint nPlayerCount = GetPlayerNumber() ;
	//for( uint i=0; i<nPlayerCount; i++ )
	//{
	//	if( m_pPlayers[i]==INVALID_ID )
	//		continue ;

	//	ServerPlayer* pPlayer = g_pPlayerPool->GetPlayer(m_pPlayers[i]) ;
	//	Assert( pPlayer ) ;

	//	SOCKET s = pPlayer->GetSocket()->GetSOCKET() ;
	//	if( s == m_SocketID )
	//		continue ;

	//	if( FD_ISSET( s, &m_ReadFDs[SELECT_USE] ) )
	//	{
	//		if( pPlayer->GetSocket()->IsSockError() )
	//		{//连接出现错误
	//			RemovePlayer( pPlayer ) ;
	//		}
	//		else
	//		{//连接正常
	//			_MY_TRY
	//			{
	//				ret = pPlayer->ProcessInput( ) ;
	//				if( !ret )
	//				{
	//					RemovePlayer( pPlayer ) ;
	//				}
	//			}
	//			_MY_CATCH
	//			{
	//				RemovePlayer( pPlayer ) ;
	//			}
	//		}
	//	}
	//}


	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

BOOL Server::ProcessOutputs( )
{
__ENTER_FUNCTION

	BOOL ret = FALSE ;

	if (m_MinFD == INVALID_SOCKET && m_MaxFD == INVALID_SOCKET) // no player exist
	{ 
		return TRUE ;
	}

	//数据发送
	//uint nPlayerCount = GetPlayerNumber() ;
	//for( uint i=0; i<nPlayerCount; i++ )
	//{
	//	if( m_pPlayers[i]==INVALID_ID )
	//		continue ;

	//	ServerPlayer* pPlayer = g_pPlayerPool->GetPlayer(m_pPlayers[i]) ;
	//	Assert( pPlayer ) ;

	//	SOCKET s = pPlayer->GetSocket()->GetSOCKET() ;
	//	if( s == m_SocketID )
	//		continue ;

	//	if( FD_ISSET( s, &m_WriteFDs[SELECT_USE] ) )
	//	{
	//		if( pPlayer->GetSocket()->IsSockError() )
	//		{//连接出现错误
	//			RemovePlayer( pPlayer ) ;
	//		}
	//		else
	//		{//连接正常
	//			_MY_TRY
	//			{
	//				ret = pPlayer->ProcessOutput( ) ;
	//				if( !ret )
	//				{
	//					RemovePlayer( pPlayer ) ;
	//				}
	//			}
	//			_MY_CATCH
	//			{
	//				RemovePlayer( pPlayer ) ;
	//			}
	//		}
	//	}
	//}

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

BOOL Server::ProcessExceptions( )
{
__ENTER_FUNCTION

	if (m_MinFD == INVALID_SOCKET && m_MaxFD == INVALID_SOCKET) // no player exist
	{ 
		return TRUE ;
	}

	//uint nPlayerCount = GetPlayerNumber() ;
	//for( uint i=0; i<nPlayerCount; i++ )
	//{
	//	if( m_pPlayers[i]==INVALID_ID )
	//		continue ;

	//	//某个玩家断开网络连接

	//	ServerPlayer* pPlayer = g_pPlayerPool->GetPlayer(m_pPlayers[i]) ;
	//	Assert( pPlayer ) ;

	//	SOCKET s = pPlayer->GetSocket()->GetSOCKET() ;
	//	if( s == m_SocketID )
	//	{//侦听句柄出现问题，难。。。
	//		Assert( FALSE ) ;
	//		continue ;
	//	}

	//	if( FD_ISSET( s, &m_ExceptFDs[SELECT_USE] ) )
	//	{
	//		RemovePlayer( pPlayer ) ;
	//	}
	//}

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

BOOL Server::ProcessCommands( )
{
__ENTER_FUNCTION

	if (m_MinFD == INVALID_SOCKET && m_MaxFD == INVALID_SOCKET) // no player exist
	{ 
		return TRUE ;
	}


	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

BOOL Server::AcceptNewConnection( )
{
__ENTER_FUNCTION

	BOOL ret = FALSE ;


//	Log::SaveLog( WORLD_LOGFILE, "Server::AcceptNewConnection(SOCKET:%d)...OK", 
//		client->GetSocket()->GetSOCKET() ) ;

	return TRUE ;



__LEAVE_FUNCTION

	return FALSE ;
}

BOOL Server::AddPlayer(/* Player* pPlayer */)
{
__ENTER_FUNCTION

	//if( m_iFDSize>=FD_SETSIZE )
	//{//已经超出能够检测的网络句柄最大数；
	//	return FALSE ;
	//}

	//BOOL ret = PlayerManager::AddPlayer( pPlayer ) ;
	//if( !ret )
	//{
	//	return FALSE ;
	//}

	//SOCKET fd = pPlayer->GetSocket()->GetSOCKET() ;
	//Assert( fd != INVALID_SOCKET ) ;

	//m_MinFD = min(fd , m_MinFD);
	//m_MaxFD = max(fd , m_MaxFD);

	//FD_SET(fd , &m_ReadFDs[SELECT_BAK]);
	//FD_SET(fd , &m_WriteFDs[SELECT_BAK]);
	//FD_SET(fd , &m_ExceptFDs[SELECT_BAK]);

	//m_iFDSize++ ;

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

BOOL Server::DelPlayer(/* Player* pPlayer */)
{
__ENTER_FUNCTION


	//FD_CLR(fd , &m_ReadFDs[SELECT_BAK]);
	//FD_CLR(fd , &m_ReadFDs[SELECT_USE]);
	//FD_CLR(fd , &m_WriteFDs[SELECT_BAK]);
	//FD_CLR(fd , &m_WriteFDs[SELECT_USE]);
	//FD_CLR(fd , &m_ExceptFDs[SELECT_BAK]);
	//FD_CLR(fd , &m_ExceptFDs[SELECT_USE]);

	//m_iFDSize-- ;
	//Assert( m_iFDSize>=0 ) ;

	//PlayerManager::RemovePlayer( pPlayer->PlayerID() ) ;

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

BOOL Server::HeartBeat( )
{
__ENTER_FUNCTION

	//DWORD dwTime = g_pTimeManager->CurrentTime() ;

	//uint nPlayerCount = GetPlayerNumber() ;
	//for( uint i=0; i<nPlayerCount; i++ )
	//{
	//	if( m_pPlayers[i] == INVALID_ID )
	//		continue ;

	//	Player* pPlayer = g_pPlayerPool->GetPlayer(m_pPlayers[i]) ;
	//	if( pPlayer==NULL )
	//	{
	//		Assert(FALSE) ;
	//		return FALSE ;
	//	}

	//	ret = pPlayer->HeartBeat( dwTime ) ;
	//	if( !ret )
	//	{//如果逻辑操作返回失败，则需要断开当前连接
	//		ret = RemovePlayer( pPlayer ) ;
	//		Assert( ret ) ;
	//	}
	//}


	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

void Server::RemoveAllPlayer( )
{
__ENTER_FUNCTION

	//uint nPlayerCount = GetPlayerNumber() ;
	//for( uint i=0; i<nPlayerCount; i++ )
	//{
	//	if( m_pPlayers[0] == INVALID_ID )
	//		break ;

	//	Player* pPlayer = g_pPlayerPool->GetPlayer(m_pPlayers[0]) ;
	//	if( pPlayer==NULL )
	//	{
	//		Assert(FALSE) ;
	//		break ;
	//	}
	//
	//	RemovePlayer( pPlayer ) ;
	//}

__LEAVE_FUNCTION
}

void Server::Loop( )
{
__ENTER_FUNCTION

	while( IsActive() )
	{
		BOOL ret = FALSE ;
//		UINT uTime = g_pTimeManager->CurrentTime() ;

		_MY_TRY
		{
			ret = Select( ) ;
			Assert( ret ) ;

			ret = ProcessExceptions( ) ;
			Assert( ret ) ;

			ret = ProcessInputs( ) ;
			Assert( ret ) ;

			ret = ProcessOutputs( ) ;
			Assert( ret ) ;
		}
		_MY_CATCH
		{
		}

		_MY_TRY
		{
			ret = ProcessCommands( ) ;
			Assert( ret ) ;
		}
		_MY_CATCH
		{
		}

		_MY_TRY
		{
			ret = HeartBeat( ) ;
			Assert( ret ) ;
		}
		_MY_CATCH
		{
		}
	};

__LEAVE_FUNCTION
}

VOID Server::BroadCast(/* Packet* pPacket */)
{
__ENTER_FUNCTION

	//uint nPlayerCount = GetPlayerNumber() ;
	//for( uint i=0; i<nPlayerCount; i++ )
	//{
	//	if( m_pPlayers[i] == INVALID_ID )
	//		continue ;

	//	Player* pPlayer = g_pPlayerPool->GetPlayer(m_pPlayers[i]) ;
	//	if( pPlayer==NULL )
	//	{
	//		Assert(FALSE) ;
	//		continue ;
	//	}

	//	pPlayer->SendPacket( pPacket ) ;
	//}

__LEAVE_FUNCTION
}

VOID Server::BroadCastServer(/*Packet* pPacket*/)
{
	
	__ENTER_FUNCTION

	//uint nPlayerCount = GetPlayerNumber() ;
	//for( uint i=0; i<nPlayerCount; i++ )
	//{
	//	if( m_pPlayers[i] == INVALID_ID )
	//		continue ;

	//	ServerPlayer* pPlayer = (ServerPlayer*)g_pPlayerPool->GetPlayer(m_pPlayers[i]) ;
	//	if( pPlayer==NULL )
	//	{
	//		Assert(FALSE) ;
	//		continue ;
	//	}
	//	if(pPlayer->GetServerData()->m_Type == SERVER_GAME)
	//		pPlayer->SendPacket( pPacket ) ;
	//}

	return;
	
	__LEAVE_FUNCTION
}


