/************************************************************************************
*	XtEvtBinderPool.cpp: Implementation File
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
#include "XtNetAcceptor.h"
#include "XtNetConnector.h"

#if defined(__WINDOWS__)
/* warning C4100: 'XXX' : unreferenced formal parameter */
#pragma warning (disable: 4100)
#endif

/*****************************************
 *			Class CXtNetConnector
 *****************************************/

CXtNetConnector::CXtNetConnector(void)
{
	Reset();
}

CXtNetConnector::~CXtNetConnector(void)
{
	Release();
}

int CXtNetConnector::Init( void *pCfg )
{
	int nRet = FUN_RET_OK;

	if ( is_valid_fd )
	{
		return FUN_RET_ERR_ILLEGAL_CALL;
	}

	m_pfnHandleEvents = OnHandleConnectEvents;

	return nRet;

}

void CXtNetConnector::Release(void)
{
	Reset();
}

int CXtNetConnector::HandleEvents( xt_event_t nEvents, void *pArg )
{
	return m_pfnHandleEvents( this, nEvents, pArg );

	if ( nEvents & EV_WRITE )
	{
		if ( m_nConnState == XT_CONN_STATE_CONNECTING )
		{
			m_nConnState = XT_CONN_STATE_CONNECTED;
			OnConnected();
		}
		nRet = OnWriteTcp();
		if ( nRet < 0 )
		{
			goto LBL_ERR;
		}
	}

	if ( nEvents & EV_READ )
	{
		nRet = OnReadTcp();
		if ( nRet < 0 )
		{
			goto LBL_ERR;
		}
	}

	if ( nEvents & EV_TIMEOUT )
	{
		nRet = OnTimeoutTcp();
		if ( nRet < 0 )
		{
			goto LBL_ERR;
		}
		if ( nRet == 1 )
		{
			if ( GetReconnectInterval() == 0 )
			{
				Reconnect();
			}
			else if ( GetHeartbeatInterval() == 0 )
			{
				nRet = XT_CONN_ERR_TIMEOUT;
				goto LBL_ERR;
			}
			++m_nTimeOutCounter;
		}
	}

	if ( nEvents & XT_EVENT_ACCEPTED )
	{
		m_EventDsc.fd = xt_accept( , m_pSockAddr, &m_nAddrLen );
		if ( m_EventDsc.fd == INVALID_SOCKET )
		{
			return -7;
		}

		if ( Filter(m_pSockAddr, m_nAddrLen) < 0 )
		{
			return -8;
		}
	}

	return nRet;

LBL_ERR:
	nRet = OnErrorTcp( nRet );

	return nRet;
}

void CXtNetConnector::OnHandleConnectEvents( xt_event_fd_t nEventfd, xt_reactor_evt_t nEvents, void *pArg )
{

}

void CXtNetConnector::OnHandleMessageEvents( xt_event_fd_t nEventfd, xt_reactor_evt_t nEvents, void *pArg )
{

}

void CXtNetConnector::Reset(void)
{
	m_pfnHandleEvents = NULL;

	m_pAcceptor = NULL;
}

int CXtNetConnector::OnAccepted(void)
{
	return FUN_RET_OK;
}

int CXtNetConnector::OnConnected(void)
{
	return FUN_RET_OK;
}

int CXtNetConnector::OnDisConnected(void)
{
	return FUN_RET_OK;
}

int CXtNetConnector::OnSend(void)
{
	return FUN_RET_OK;
}

int CXtNetConnector::OnRecv(void)
{
	return FUN_RET_OK;
}

int CXtNetConnector::OnTimeout(void)
{
	return FUN_RET_OK;
}

int CXtNetConnector::OnError( int nErrCode )
{
	/* Return 0 to ignore default operation */
	return -1;
}

void CXtNetConnector::SetConnectInterval(void)
{
	m_timeout.tv_sec = 1;
	m_timeout.tv_usec = 0;
}

int CXtNetConnector::HandleAccept( CXtNetConnector *pThis, xt_event_t nEvents, void *pArg )
{
#define handler_ptr_witharg			((CXtNetEventHandler*)(pArg))

	return pThis->ProcAccept( handler_ptr_witharg );

#undef handler_ptr_witharg
	return FUN_RET_OK;
}

int CXtNetConnector::HandleAcceptUdp( CXtNetConnector *pThis, xt_event_t nEvents, void *pArg )
{
#define handler_ptr_witharg			((CXtNetEventHandler*)(pArg))

#undef handler_ptr_witharg
	return FUN_RET_OK;
}

int	CXtNetConnector::HandleConnect(CXtNetConnector *pThis, xt_event_t nEvents, void *pArg )
{
	int nRet = FUN_RET_OK;

	if ( nEvents & XT_EV_WRITE ) /* Connected. */
	{
		nRet = pThis->ProcConnected();
		return FUN_RET_OK;
	}

	if ( nEvents & XT_EV_TIMEOUT )
	{
		nRet = pThis->ProcConnect();
	}

	if ( nRet < 0 ) /* Connect Failed. */
	{
		pThis->ProcError( XT_CONN_ERR_BADFD );
	}

	return FUN_RET_OK;
}

int	CXtNetConnector::HandleMessage(CXtNetConnector *pThis, xt_event_t nEvents, void *pArg )
{
	int nRet = FUN_RET_OK;

	if ( nEvents & XT_EV_READ )
	{
		nRet = pThis->ProcRecv();
		if ( nRet < 0 )
		{
			goto LBL_ERR;
		}
	}

	if ( nEvents & XT_EV_WRITE )
	{
		nRet = pThis->ProcSend();
		if ( nRet < 0 )
		{
			goto LBL_ERR;
		}
	}

	if ( nEvents & XT_EV_TIMEOUT )
	{
		nRet = pThis->ProcTimeout();
		if ( nRet < 0 )
		{
			goto LBL_ERR;
		}
	}

	if ( nEvents | XT_NETEV_DISCONNECT )
	{
		nRet = pThis->Disconnect();
		if ( nRet < 0 )
		{
			goto LBL_ERR;
		}
	}

LBL_ERR:
	if ( nRet < 0 ) /* Connect Failed. */
	{
		pThis->ProcError( nRet );
	}

	return FUN_RET_OK;
}

int CXtNetConnector::ProcAccept( void *pArg )
{
#define handler_ptr_witharg			((CXtNetAcceptor*)(pArg))
	m_EventDsc.fd = xt_accept( handler_ptr_witharg->GetEventfd(), m_pSockAddr, &m_nAddrLen );
	if ( m_EventDsc.fd == INVALID_SOCKET )
	{
		return -7;
	}

	if ( Filter(m_pSockAddr, m_nAddrLen) < 0 )
	{
		return -8;
	}

	m_pfnHandleEvents = HandleMessage;

	m_nEvents = XT_EV_READ|XT_EV_PERSIST;

	m_pAcceptor = handler_ptr_witharg;

	evutil_make_socket_nonblocking( m_EventDsc.fd );

	OnAccepted();

#undef handler_ptr_witharg
	return FUN_RET_OK;
}

int CXtNetConnector::InitConnect(void)
{
	int nErrCode = FUN_RET_OK;

	if ( CreateEventDesc(SOCK_STREAM) < 0 )
	{
		return -5;
	}

	m_nEvents = XT_EV_WRITE|XT_EV_PERSIST;

	nErrCode = BindAddress();
	if ( nErrCode < 0 )
	{
		return nErrCode;
	}

	SetConnectInterval(); /* XT_EV_TIMEOUT setup here. */

	return FUN_RET_OK;
}

int CXtNetConnector::ProcConnect(void)
{
	int nRet = FUN_RET_OK;
	int nErrCode = 0;

	nRet = xt_connect( m_EventDsc.fd, m_pSockAddr, m_nAddrLen );
	if ( nRet == 0 )
	{
		ProcConnected();
		return FUN_RET_OK;
	}

	nErrCode = SktGetLastError();
	if ( nRet == -1 )
	{
		switch( nErrCode )
		{
		case xt_STD_ERR_EINPROGRESS:
		case xt_STD_ERR_EWOULDBLOCK:
		case xt_STD_ERR_EAGAIN:
			nRet = 0;
			break;
		default: /* Connect Failed. */
			/*nRet = -1;*/
			break;
		}
	}

	return nRet;
}

int CXtNetConnector::ProcConnected(void)
{
	CXtReactor *pReactor = m_pReactor;

	pReactor->RemoveHandler( this );

	m_pfnHandleEvents = HandleMessage;
	m_nEvents = XT_EV_READ|XT_EV_PERSIST;

	OnConnected();

	pReactor->RegisterHandler( this );

	return FUN_RET_OK;
}

int CXtNetConnector::Disconnect(void)
{
	xt_closesock( m_EventDsc.fd	);
	reset_event_desc;
	
	return FUN_RET_OK;
}

int CXtNetConnector::ProcRecv(void)
{
	OnRecv();
	return FUN_RET_OK;
}

int	CXtNetConnector::ProcSend(void)
{
	OnSend();
	return FUN_RET_OK;
}

int CXtNetConnector::ProcTimeout(void)
{
	return OnTimeout();
}

int CXtNetConnector::ProcError( int nErrCode )
{
	if ( OnError(nErrCode) == FUN_RET_OK )
	{
		return FUN_RET_OK;
	}

	switch ( nErrCode )
	{
	case XT_CONN_ERR_PEERCLOSED:
		break;
	case XT_CONN_ERR_BADFD:
		{
			CXtReactor *pReactor = m_pReactor;
			pReactor->RemoveHandler( this );
			xt_closesock( m_EventDsc.fd );
			InitConnect();
			pReactor->RegisterHandler( this );
		}
		break;
	case XT_CONN_ERR_CONNDROP:
		break;
	case XT_CONN_ERR_SERVICEDOWN:
		break;
	case  XT_CONN_ERR_UNREACH:
		break;
	case XT_CONN_ERR_TIMEOUT:
		break;
	case XT_CONN_ERR_TRYAGAIN:
		return FUN_RET_OK;
		break;
	default:
		break;
	}

	if ( m_pAcceptor )
	{
		m_pAcceptor->HandleEvents( XT_NETEV_DISCONNECT, this );
	}
	else
	{
		Disconnect();
	}

	return FUN_RET_OK;
}
/*****************************************
 *			Class CXtNetConnector END
 *****************************************/

/*****************************************
 *			Class CXtNetConnPool
 *****************************************/
static int xt_connpool_tcp_cmp( const xt_obj_handle_t objHandle1, const xt_obj_handle_t objHandle2 )
{
#define conn_ptr(_arg)	((CXtNetConnector*)(_arg))
	return (conn_ptr(objHandle1)->GetEventfd() - conn_ptr(objHandle2)->GetEventfd());
#undef conn_ptr
}

static int xt_connpool_udp_cmp( const xt_obj_handle_t objHandle1, const xt_obj_handle_t objHandle2 )
{
#define conn_ptr(_arg)	((CXtNetConnector*)(_arg))
	return xt_net_saddr_cmp(conn_ptr(objHandle1)->m_pSockAddr, conn_ptr(objHandle2)->m_pSockAddr );
#undef conn_ptr
}

CXtNetConnPool::CXtNetConnPool(void)
{
	Reset();
}

CXtNetConnPool::~CXtNetConnPool(void)
{
	Release();
}

int CXtNetConnPool::SetupConnectorInfo( USHORT nFaminly, int nSockType/*=NET_SKT_TYPE_TCP*/ )
{
	switch( nSockType )
	{
	case NET_SKT_TYPE_TCP:
		m_ObjHndBuf.fn_cmp = xt_connpool_tcp_cmp;
		break;
	case NET_SKT_TYPE_UDP:
		m_ObjHndBuf.fn_cmp = xt_connpool_udp_cmp;
		break;
	default:
		return -5;
		break;
	}

	_xt_SOCKADDR_SIZE_(nFaminly,m_nSockLenMax);

	m_nFamily = nFaminly;
	m_nSockType = nSockType;

	return FUN_RET_OK;
}

void CXtNetConnPool::Release(void)
{
	xt_size_t i = 0;
	CXtNetConnector *pConnector = NULL;

	for ( i=0; i<m_ObjHndBuf.count; ++i )
	{
		pConnector = (CXtNetConnector*)GetAt( i );
		pConnector->Release();
	}

	Reset();
}

void CXtNetConnPool::Reset(void)
{
	m_nSockType = 0;

	m_nRecvBufCapacity = 0;

	m_nSockLenMax = 0;

	m_nFamily = 0;
}

void CXtNetConnPool::SetupObjSize(void)
{
	switch( m_nSockType )
	{
	case NET_SKT_TYPE_TCP:
		m_nObjSize = sizeof(CXtNetConnector) + m_nSockLenMax + xt_net_msgbuf_calc_size(XT_NET_MSG_BUF_SIZE_TCP);
		m_nRecvBufCapacity = XT_NET_MSG_BUF_SIZE_TCP;
		break;
	case NET_SKT_TYPE_UDP:
		m_nObjSize = sizeof(CXtNetConnector) + m_nSockLenMax;
		break;
	default:
		break;
	}

	return;
}

int CXtNetConnPool::InitObj( void *pObjAddr )
{
#define connector_ptr	((CXtNetConnector*)pObjAddr)

	new(pObjAddr) CXtNetConnector;

	connector_ptr->m_pSockAddr = (SOCKADDR*)(((char*)pObjAddr) + sizeof(CXtNetConnector));
	reset_conn_sock_len(connector_ptr);
	connector_ptr->m_pSockAddr->sa_family = m_nFamily;

	if ( m_nRecvBufCapacity )
	{
		connector_ptr->m_pRecvMsg = (xt_net_msgbuf_t*)((char*)(connector_ptr->m_pSockAddr) + m_nSockLenMax);
		connector_ptr->m_pRecvMsg->capacity = m_nRecvBufCapacity;
		xt_net_msgbuf_clear( connector_ptr->m_pRecvMsg );
	}

#undef connector_ptr

	return FUN_RET_OK;
}

void CXtNetConnPool::ResetObj( void *pObjAddr )
{
#define connector_ptr	((CXtNetConnector*)pObjAddr)
	reset_conn_sock_len(connector_ptr);

	if ( m_nRecvBufCapacity )
	{
		xt_net_msgbuf_clear( connector_ptr->m_pRecvMsg );
	}
	
#undef connector_ptr
	return;
}

void CXtNetConnPool::ReleaseObj( void *pObjAddr )
{
#define connector_ptr	((CXtNetConnector*)pObjAddr)
	reset_conn_sock_len(connector_ptr);

	if ( m_nRecvBufCapacity )
	{
		xt_net_msgbuf_clear( connector_ptr->m_pRecvMsg );
	}
#undef connector_ptr
	return;
}

/*****************************************
 *			Class CXtNetConnPool END
 *****************************************/
