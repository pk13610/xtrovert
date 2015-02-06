/************************************************************************************
*	XtNetConnector.cpp: Implementation File
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
#include "net/inc/XtNetConnector.h"
#include "net/inc/XtReactor.h"

#if defined(__WINDOWS__)
/* warning C4100: 'XXX' : unreferenced formal parameter */
#pragma warning (disable: 4100)
#endif

CXtNetConnector::CXtNetConnector(void)
{
}

CXtNetConnector::~CXtNetConnector(void)
{
}

#define _CONNECTOR_EVENTS_					(EV_READ|EV_WRITE|EV_PERSIST)

int CXtNetConnector::Init( void *arg )
{
	#define peer_address		((const TCHAR*)arg)

	m_EventDsc.fd = InitConnect( peer_address );
	if (m_EventDsc.fd == INVALID_SOCKET )
	{
		return -4;
	}

	m_nEvents = _CONNECTOR_EVENTS_;

	return FUN_RET_OK;
}

int CXtNetConnector::AttachInit( void *pArg )
{
#define _acceptor_p_			(CXtNetAcceptor*)arg

#if defined(__WINDOWS__)
	m_EventDsc.fd =  WSAAccept( _acceptor_p_->GetEventfd(), m_pSAddr->saddr, &(m_pSAddr->len), NULL, NULL );
#elif defined(__LINUX__)
	m_EventDsc.fd =  xt_accept( _acceptor_p_->GetEventfd(), m_pSAddr->saddr, &(m_pSAddr->len) );
	if ( m_EventDsc.fd == INVALID_SOCKET )
	{
		return -5;
	}
#else
	return FUN_RET_ERR_NOTIMPL;
#endif

	m_nEvents = _CONNECTOR_EVENTS_;
}

void CXtNetConnector::Release(void)
{
	if ( is_valid_fd )
	{
		xt_closesock( GetEventfd() );
		m_EventDsc.fd_handle = _INVALID_EVENT_FD_;
	}

	/*Reset saddr->len*/

	CXtEventHandler::Release(void);
}

void CXtNetConnector::Reset(void)
{
	m_bReadyRecv = FALSE;
	m_bReadySend = FALSE;
}

int CXtNetConnector::HandleEvents( xt_event_t events )
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

xt_event_fd_t CXtNetConnector::InitConnect( const TCHAR *szAddrPeer )
{
	xt_event_fd_t fd = INVALID_SOCKET;
	int ret = FUN_RET_OK;
	USHORT family = AF_UNSPEC;

	SOCKADDR_STORAGE saddr_peer;
	SOCKLEN_T sock_len = sizeof(saddr_peer);

	family = addr_str_af( szAddrPeer );
	if ( fill_sock_addr_struct((SOCKADDR*)&saddr_peer, &sock_len, szAddrPeer, family) < 0 )
	{
		return INVALID_SOCKET;
	}

	fd = xt_socket( family, SOCK_STREAM, 0 );
	if ( evutil_make_socket_nonblocking( fd ) < 0 )
	{
		xt_closesock( fd );
		/*_tprintf("evutil_make_socket_nonblocking\n");*/
		return INVALID_SOCKET;
	}

	ret = xt_connect( fd, (SOCKADDR*)&saddr_peer, sock_len );
// 	if ( ret < 0 )
// 	{
// 		goto lbl_err;
// 	}

	return fd;

// lbl_err:
// 	if ( fd != INVALID_SOCKET )
// 	{
// 		xt_closesock( fd );
// 	}

	/*return INVALID_SOCKET;*/
}

int CXtNetConnector::OnRecv(void)
{
	return FUN_RET_OK;
}
int CXtNetConnector::OnSend(void)
{
	return FUN_RET_OK;
}
int CXtNetConnector::OnError( int nErroCode )
{
	return FUN_RET_OK;
}

/**********************************************
 *			class CXtEventPool
 **********************************************/

CXtConnPool::CXtConnPool(void)
{

}

CXtConnPool::~CXtConnPool(void)
{

}

void CXtConnPool::ReleaseObj( void *pObjAddr )
{
	return;
}
