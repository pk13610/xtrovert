/************************************************************************************
*	XtNetAcceptor.cpp: Implementation File
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
#include "net/inc/XtNetAcceptor.h"
#include "net/inc/XtNetConnector.h"
#include "net/inc/XtReactor.h"

#if defined(__WINDOWS__)
/* warning C4100: 'XXX' : unreferenced formal parameter */
#pragma warning (disable: 4100)
#endif

// void init_xt_acceptor_cfg( xt_acceptor_cfg_t *cfg )
// {
// 	cfg->conn_count_init = XT_CONN_COUNT_INIT;
// 	cfg->conn_count_max = XT_CONN_COUNT_MAX;
// }

CXtNetAcceptor::CXtNetAcceptor(void)
{
}

CXtNetAcceptor::~CXtNetAcceptor(void)
{
}

/* Create socket according to local ip-address */
int CXtNetAcceptor::Init( void *pArg )
{
	#define cfg		((xt_acceptor_cfg_t*)pArg)
	#define local_address		cfg->local_addr
	#define conn_pool			cfg->conn_pool

	int ret = FUN_RET_OK;

	m_EventDsc.fd = InitListener( local_address );
	if ( m_EventDsc.fd == _INVALID_EVENT_FD_ )
	{
		return -4;
	}

	ret = xt_listen( m_EventDsc.fd, 5 );

	m_nEvents = (EV_READ|EV_PERSIST); /* <==> AddEvents( EV_READ|EV_PERSIST ); */

	m_pConnPool = conn_pool;
	
	return FUN_RET_OK;
}

void CXtNetAcceptor::Release(void)
{
	CXtNetEventHandler::Release();
}

void CXtNetAcceptor::Reset(void)
{
	m_pConnPool = NULL;
	CXtNetEventHandler::Reset();
}

int CXtNetAcceptor::HandleEvents( xt_event_t nEvents )
{
	xt_event_fd_t client_sock = _INVALID_EVENT_FD_;

	CXtNetConnector	*conn = AllocConnector(NULL);
	if ( conn == NULL )
	{
		return -4;
	}

	conn->AttachInit( this );

	/*_tprintf( _T("AllocConnector:[%p]\n"), conn );*/

	SOCKADDR_STORAGE saddr;
	SOCKLEN_T sock_len;
	sock_len = sizeof( saddr );

	client_sock = xt_accept( m_EventDsc.fd, (SOCKADDR*)&saddr, &sock_len );
	if ( client_sock == INVALID_SOCKET )
	{
		return -5;
	}

	_tprintf( _T("xt_accept:[%d]\n"), client_sock );

	if ( evutil_make_socket_nonblocking(client_sock) < 0 )
	{
		xt_closesock( client_sock );
		_tprintf("evutil_make_socket_nonblocking\n");
		return -100;
	}	
	conn->AttachInit( client_sock );
	/*connector->AddEvents( EV_READ|EV_WRITE|EV_PERSIST );*/

	m_pReactor->RegisterHandler( conn );

	return FUN_RET_OK;
}

xt_event_fd_t CXtNetAcceptor::InitListener( const TCHAR *szAddr )
{
	xt_event_fd_t fd = _INVALID_EVENT_FD_;
	int ret = FUN_RET_OK;
	USHORT family = AF_UNSPEC;

	SOCKADDR_STORAGE saddr;
	SOCKLEN_T sock_len = sizeof(saddr);

	family = addr_str_af( szAddr );
	if ( fill_sock_addr_struct((SOCKADDR*)&saddr, &sock_len, szAddr, family) < 0 )
	{
		return _INVALID_EVENT_FD_;
	}

	fd = xt_socket( family, SOCK_STREAM, 0 );
	if ( evutil_make_socket_nonblocking( fd ) < 0 )
	{
		xt_closesock( fd );
		/*_tprintf("evutil_make_socket_nonblocking\n");*/
		return _INVALID_EVENT_FD_;
	}

	ret = xt_bind( fd, (SOCKADDR*)&saddr, sock_len );
	if ( ret < 0 )
	{
		goto lbl_err;
	}

	return fd;

lbl_err:
	if ( fd != _INVALID_EVENT_FD_ )
	{
		xt_closesock( fd );
	}

	return _INVALID_EVENT_FD_;
}

CXtNetConnector* CXtNetAcceptor::AllocConnector( xt_obj_id_t *pObjId )
{
	CXtNetConnector *conn = NULL;

	conn = (CXtNetConnector*)m_pConnPool->AllocObj( pObjId );
	if ( conn == NULL )
	{
		m_pConnPool->InflatePool( _connector_count_ );
		conn = (CXtNetConnector*)m_pConnPool->AllocObj( pObjId );
		if ( conn == NULL )
		{
			return NULL;
		}
	}

	return conn;
}

int CXtNetAcceptor::Accept( CXtNetConnector *pconnector )
{

}
