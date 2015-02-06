/************************************************************************************
*	XtNetUdpConnector.cpp: Implementation File
*
* DESCRIPTION  :   
*
* AUTHOR	    :	Sean Feng <SeanFeng2006@hotmail.com>
* DATE		    :	Sep. 24, 2010
*
* Copyright (c) 2010-?. All Rights Reserved.
*
* REMARKS:
*		
*      
************************************************************************************/

#include "xt_net_api.h"
#include "net/inc/XtNetUdpConnector.h"
#include "net/inc/XtReactor.h"

#if defined(__WINDOWS__)
/* warning C4100: 'XXX' : unreferenced formal parameter */
#pragma warning (disable: 4100)
#endif

CXtNetUdpConnector::CXtNetUdpConnector(void)
{
}

CXtNetUdpConnector::~CXtNetUdpConnector(void)
{
}

#define _CONNECTOR_EVENTS_					(EV_READ|EV_WRITE|EV_PERSIST)

int CXtNetUdpConnector::Init( void *arg )
{
	#define peer_address		((const TCHAR*)arg)

	m_event_info.fd = InitConnect( peer_address );
	if ( m_event_info.fd == INVALID_SOCKET )
	{
		return -4;
	}

	m_event_info.events = _CONNECTOR_EVENTS_;

	return FUN_RET_OK;
}

void CXtNetUdpConnector::AttachInit( xt_event_fd_t fd )
{
	m_event_info.fd = fd;
	m_event_info.events = _CONNECTOR_EVENTS_;
}

 void CXtNetUdpConnector::Release(void)
{
	if ( is_valid_fd )
	{
		xt_closesock( m_event_info.fd );
		m_event_info.fd = INVALID_SOCKET;
	}
}

void CXtNetUdpConnector::Reset(void)
{
	m_bReadyRecv = FALSE;
	m_bReadySend = FALSE;
}

int CXtNetUdpConnector::HandleEvents( xt_event_t events )
{
	int ret = FUN_RET_OK;
	if ( ((events&EV_READ)==EV_READ) && m_bReadyRecv )
	{
		ret = OnRecv();
		if ( ret < 0 )
		{
			ret = OnError( ret );
		}
	}

	if ( ret == ERR_CONNECTOR_FATAL )
	{
		m_pReactor->RemoveHandler( this );
		return ERR_CONNECTOR_FATAL;
	}

	if ( (events&EV_WRITE) == EV_WRITE && m_bReadySend )
	{
		ret = OnSend();
		if ( ret < 0 )
		{
			ret = OnError( ret );
		}
	}

	if ( ret == ERR_CONNECTOR_FATAL )
	{
		m_pReactor->RemoveHandler( this );
		return ERR_CONNECTOR_FATAL;
	}

	return FUN_RET_OK;
}

xt_event_fd_t CXtNetUdpConnector::InitConnect( const TCHAR *szAddrPeer )
{
	socket_fd_t fd = INVALID_SOCKET;
	int ret = FUN_RET_OK;
	USHORT family = AF_UNSPEC;

	SOCKADDR_STORAGE saddr_peer;
	SOCKLEN_T sock_len = sizeof(saddr_peer);

	family = addr_str_af( szAddrPeer );
	if ( fill_sock_addr_struct((SOCKADDR*)&saddr_peer, &sock_len, szAddrPeer, family) < 0 )
	{
		return INVALID_SOCKET;
	}

	fd = xt_socket( family, SOCK_DGRAM, 0 );
	if ( evutil_make_socket_nonblocking( fd ) < 0 )
	{
		xt_closesock( fd );
		/*_tprintf("evutil_make_socket_nonblocking\n");*/
		return INVALID_SOCKET;
	}

	return fd;
}

int CXtNetUdpConnector::OnRecv(void)
{
	return FUN_RET_OK;
}
int CXtNetUdpConnector::OnSend(void)
{
	return FUN_RET_OK;
}
int CXtNetUdpConnector::OnError( int nErroCode )
{
	return FUN_RET_OK;
}

/**********************************************
 *			class CXtUdpConnPool
 **********************************************/

CXtUdpConnPool::CXtUdpConnPool(void)
{

}

CXtUdpConnPool::~CXtUdpConnPool(void)
{

}

void CXtUdpConnPool::ReleaseObj( void *pObjAddr )
{
	return;
}
