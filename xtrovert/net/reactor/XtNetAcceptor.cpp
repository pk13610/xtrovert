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

#include "XtNetAcceptor.h"
#include "XtNetConnector.h"

#if defined(__WINDOWS__)
/* warning C4100: 'XXX' : unreferenced formal parameter */
#pragma warning (disable: 4100)
#endif

/*****************************************
 *			Class CXtNetAcceptor
 *****************************************/
CXtNetAcceptor::CXtNetAcceptor(void)
{
	Reset();
}

CXtNetAcceptor::~CXtNetAcceptor(void)
{
	Release();
}

int CXtNetAcceptor::Init( void *pCfg )
{
	int nRet = FUN_RET_OK;

#define acceptor_cfg_ptr			((xt_acceptor_cfg_t*)pCfg)

	InitAcceptor();
#undef acceptor_cfg_ptr
	return nRet;
}

void CXtNetAcceptor::Release(void)
{
	xt_size_t i = 0;
	CXtNetConnector *pConnector = NULL;

	xt_closesock( m_EventDsc.fd );

	/*m_pReactor->RemoveHandler( this );*/

	if ( m_pConnPool && m_pReactor )
	{
		for ( i=0; i<m_pConnPool->Count(); ++i )
		{
			pConnector = (CXtNetConnector*)m_pConnPool->GetAt( i );
			/*m_pReactor->RemoveHandler( pConnector );*/
			pConnector->Release();
		}
	}

	Reset();
}
int CXtNetAcceptor::HandleEvents( xt_event_t nEvents, void *pArg )
{
	int nRet = FUN_RET_OK;

	CXtNetConnector *pConnector = NULL;
	xt_event_fd_t client_fd = INVALID_SOCKET;

	if ( nEvents | XT_EV_READ ) /* Accept */
	{
	}
	else if ( nEvents | XT_NETEV_DISCONNECT )
	{
		if ( pArg == this )
		{
			Release();
		}
		else
		{
			pConnector = (CXtNetConnector*)pArg;
			Kick( pConnector );
		}
	}

	pConnector = (CXtNetConnector*)m_pConnPool->AllocObj(NULL);
	if ( pConnector == NULL )
	{
		return -5;
	}

	nRet = pConnector->HandleEvents( XT_NETEV_CONNECTED, this );
	if ( nRet < 0 )
	{
		return -6;
	}

	nRet = m_pConnPool->ActivateObj();

	client_fd = xt_accept( m_EventDsc.fd, pConnector->m_pSockAddr, &(pConnector->m_nAddrLen) );
	if ( client_fd == INVALID_SOCKET )
	{
		return -5;
	}

	if ( Filter(pConnector->m_pSockAddr, pConnector->m_nAddrLen) < 0 )
	{
		return -6;
	}

	xt_net_conn_cfg_t cfg;
	cfg.base_cfg.net_cfg_flag = XT_FLAG_CONNECTED_TCP;
	cfg.base_cfg.evt_fd_ptr = &client_fd;
	cfg.acceptor = this;

	cfg.base_cfg.sock_addr.saddr = pConnector->m_pSockAddr;
	cfg.base_cfg.sock_addr.addrlen = pConnector->m_nAddrLen;
	/* Setup connector's socket-fd */
	nRet = pConnector->Init( &cfg );

	nRet = m_pConnPool->ActivateObj();

	OnAccept( pConnector );

	nRet = m_pReactor->RegisterHandler( pConnector );

	return nRet;
}

int CXtNetAcceptor::Kick( CXtNetConnector *pConnector )
{
	int nRet = FUN_RET_OK;

	m_pReactor->RemoveHandler( pConnector );

	if ( m_pConnPool )
	{
		nRet = m_pConnPool->FreeObj( pConnector );
		if ( nRet < 0 )
		{
			return -6;
		}
	}

	pConnector->HandleEvents( XT_NETEV_DISCONNECT, NULL );

	return nRet;
}

void CXtNetAcceptor::Reset(void)
{
	m_pConnPool = NULL;
}

/*****************************************
 *			Class CXtNetAcceptor END
 *****************************************/
