/************************************************************************************
*	XtNetUdpAcceptor.cpp: Implementation File
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
#include "net/inc/XtNetUdpAcceptor.h"
#include "net/inc/XtNetUdpConnector.h"
#include "net/inc/XtReactor.h"

#if defined(__WINDOWS__)
/* warning C4100: 'XXX' : unreferenced formal parameter */
#pragma warning (disable: 4100)
#endif

CXtNetUdpAcceptor::CXtNetUdpAcceptor(void)
{
}

CXtNetUdpAcceptor::~CXtNetUdpAcceptor(void)
{
}

/* Create socket according to local ip-address */
int CXtNetUdpAcceptor::Init( void *arg )
{
	#define cfg		((xt_acceptor_cfg_t*)arg)
	#define local_address		cfg->local_addr
	#define conn_pool			cfg->conn_pool

	int ret = FUN_RET_OK;

	m_event_info.fd = InitListener( local_address );
	if ( m_event_info.fd == INVALID_SOCKET )
	{
		return -4;
	}

	m_event_info.events = (EV_READ|EV_WRITE|EV_PERSIST); /* <==> AddEvents( EV_READ|EV_WRITE|EV_PERSIST ); */

	m_pConnPool = conn_pool;

	return FUN_RET_OK;
}

void CXtNetUdpAcceptor::AttachInit( xt_event_fd_t fd )
{
// 	 m_event_info.fd = fd;
// 	 m_event_info.events = (EV_READ|EV_PERSIST); /* <==> AddEvents( EV_READ|EV_PERSIST ); */
}

void CXtNetUdpAcceptor::Release(void)
{
	if ( is_valid_fd )
	{
		xt_closesock( m_event_info.fd );
		m_event_info.fd = INVALID_SOCKET;
	}

	Reset();
}

void CXtNetUdpAcceptor::Reset(void)
{
	m_pConnPool = NULL;
}

int CXtNetUdpAcceptor::HandleEvents( xt_event_t events )
{
	conn->AttachInit( m_event_info.fd );

	conn->OnHandleEvents( events );

	return FUN_RET_OK;
}

xt_event_fd_t CXtNetAcceptor::InitListener( const TCHAR *szAddr )
{
	xt_event_fd_t fd = INVALID_SOCKET;
	int ret = FUN_RET_OK;
	USHORT family = AF_UNSPEC;

	SOCKADDR_STORAGE saddr;
	SOCKLEN_T sock_len = sizeof(saddr);

	family = addr_str_af( szAddr );
	if ( fill_sock_addr_struct((SOCKADDR*)&saddr, &sock_len, szAddr, family) < 0 )
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

	ret = xt_bind( fd, (SOCKADDR*)&saddr, sock_len );
	if ( ret < 0 )
	{
		goto lbl_err;
	}

	return fd;

lbl_err:
	if ( fd != INVALID_SOCKET )
	{
		xt_closesock( fd );
	}

	return INVALID_SOCKET;
}

CXtNetUdpConnector* CXtNetUdpAcceptor::AllocConnector( xt_obj_id_t *pObjId )
{
	CXtNetUdpConnector *conn = NULL;

	conn = (CXtNetUdpConnector*)m_pConnPool->AllocObj( pObjId );
	if ( conn == NULL )
	{
		m_pConnPool->InflatePool( _connector_count_ );
		conn = (CXtNetUdpConnector*)m_pConnPool->AllocObj( pObjId );
		if ( conn == NULL )
		{
			return NULL;
		}
	}

	return conn;
}
