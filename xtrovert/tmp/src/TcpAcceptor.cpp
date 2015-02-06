/************************************************************************************
*	TcpConnector.cpp: Implementation File
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

#include "TcpAcceptor.h"
#include "Heap.h"

CTcpAcceptor::CTcpAcceptor(VOID)
{

}

CTcpAcceptor::~CTcpAcceptor(VOID)
{
	Release();
}


INT CTcpAcceptor::Init( CONST TCP_SVR_CFG *pInfo, CONST INET_SVR_PARAM *pParam/*=NULL*/ )
{
	INT nRet = FUN_RET_OK;
	if ( pInfo == NULL )
	{
		return FUN_RET_ERR_ILLEGAL_ARGS;
	}

	SOCKLEN_T nLen = sizeof(m_AddrNative);
	nRet = ExtSktAPI::fill_sock_addr_struct( (SOCKADDR*)&m_AddrNative, &nLen, pInfo->szAddNative, pInfo->nFamily );
	if ( nRet < 0 )
	{
		return nRet;
	}

	if ( pParam != NULL )
	{
		if ( pParam->nMaxClientCount == 0 )
		{
			m_ConnPool.nConnCntMax = _TCP_ACCEPTOR_CONN_CUNT_DEF_;
		}
		else
		{
			m_ConnPool.nConnCntMax = pParam->nMaxClientCount;
		}
	}
	
	nRet = AllocConnectors();
	if ( nRet < 0 )
	{
		return nRet;
	}

	return FUN_RET_OK;
}

INT CTcpAcceptor::Start(VOID)
{
	INT nRet = FUN_RET_OK;
	SOCKET_ID nSocket;
	nSocket = m_sktNative.Create( (SOCKADDR*)&m_AddrNative, sizeof(m_AddrNative), 0 );
	if ( nSocket == INVALID_SOCKET )
	{
		return ERR_SOCK_FD_INIT_FAILED;
	}

	nRet = m_sktNative.Listen();
	if ( nRet < 0 )
	{
		m_sktNative.Close();
		return ERR_SOCK_FD_LISTEN_FAILED;
	}

	return FUN_RET_OK;
}

INT CTcpAcceptor::Close(VOID)
{
	KickClient( _INVALID_ELEMENT_COUNT_ );
	m_sktNative.Close();

	return FUN_RET_OK;
}

// INT CTcpAcceptor::HandleEvents( INT nEvents )
// {
// 
// }

INT CTcpAcceptor::Release(VOID)
{
	Close();
	if ( *(m_ConnPool.pConnIndicator) != NULL )
	{
		delete [] *(m_ConnPool.pConnIndicator);
	}

	Reset();
	return FUN_RET_OK;
}

INT CTcpAcceptor::KickClient( size_t nIndex )
{
	if ( nIndex < m_ConnPool.nConnCnt )
	{
		m_ConnPool.pConnIndicator[nIndex]->Close();
		RemConnector( m_ConnPool.pConnIndicator[nIndex] );
	}
	else if ( nIndex == _INVALID_ELEMENT_COUNT_ )
	{
		for ( size_t i=0; i<m_ConnPool.nConnCnt; i++ )
		{
			m_ConnPool.pConnIndicator[i]->Close();
		}
		m_ConnPool.nConnCnt = 0;
	}
	else
	{
		return FUN_RET_ERR_ILLEGAL_ARGS;
	}

	return FUN_RET_OK;
}

INT CTcpAcceptor::OnRead(VOID)
{
	if ( m_sktNative.IsValid() == FALSE )
	{
		return FUN_RET_ERR_ILLEGAL_CALL;
	}

	INT nRet = FUN_RET_OK;
	CExtSocketTcp socket;
	nRet = m_sktNative.Accept( socket, NULL, NULL );
	if ( nRet < 0 )
	{
		return nRet;
	}
	
	CTcpConnector *pConnector = NULL;
	pConnector = AddConnector( &socket );
	if ( pConnector == NULL )
	{
		socket.Close();
		return ERR_SOCK_CLIENT_FULL;
	}

	m_pReactor->RegisterHandler( pConnector );

	return FUN_RET_OK;
}

INT CTcpAcceptor::OnWrite(VOID)
{
	return FUN_RET_OK;
}

INT CTcpAcceptor::OnTimeout(VOID)
{
	return FUN_RET_OK;
}

INT CTcpAcceptor::OnClose(VOID)
{
	Close();
	return FUN_RET_OK;
}

INT CTcpAcceptor::AllocConnectors(VOID)
{
	//m_nElemCntMax
	// m_pPoolIndicator buffer size = m_nElemCntMax * sizeof(m_pPoolIndicator)
	// m_pBuff buffer size = m_nElemCntMax * m_nElemCnt
	*(m_ConnPool.pConnIndicator) = (CTcpConnector*)ExtMemAPI::ext_mem_alloc( m_ConnPool.nConnCntMax*sizeof(*m_ConnPool.pConnIndicator) + m_ConnPool.nConnCntMax*sizeof(*m_ConnPool.pConnectors) );
	if ( *(m_ConnPool.pConnIndicator) == NULL )
	{
		return ERR_HEAPBLOCK_BUF_INIT_FAIL;
	}

	//memset( *(m_ConnPool.pConnIndicator), 0, m_ConnPool.nConnCntMax*sizeof(*m_ConnPool.pConnIndicator) + m_ConnPool.nConnCntMax*sizeof(*m_ConnPool.pConnectors) );

	m_ConnPool.pConnectors = *(m_ConnPool.pConnIndicator) + m_ConnPool.nConnCntMax*sizeof(*m_ConnPool.pConnIndicator);

	for ( size_t i=0; i<m_ConnPool.nConnCntMax; i++ )
	{
		m_ConnPool.pConnIndicator[i] = m_ConnPool.pConnectors + i;
	}

	return FUN_RET_OK;
}

CTcpConnector* CTcpAcceptor::AddConnector( CExtSocketTcp *pSocket )
{
	if ( m_ConnPool.nConnCnt == m_ConnPool.nConnCntMax )
	{
		return NULL;
	}

	(m_ConnPool.pConnIndicator)[0]->Attach( pSocket );

	CTcpConnector *pConnector = (m_ConnPool.pConnIndicator)[0];

	m_ConnPool.nConnCnt ++;

	SortMiniHeep( (LONG*)(*m_ConnPool.pConnIndicator), m_ConnPool.nConnCnt );
	SortMiniHeep( (LONG*)((*m_ConnPool.pConnIndicator)+m_ConnPool.nConnCnt), m_ConnPool.nConnCntMax-m_ConnPool.nConnCnt );

	return pConnector;
}

INT CTcpAcceptor::RemConnector( CTcpConnector *pConnector )
{
	if ( m_ConnPool.nConnCnt == 0 )
	{
		return ERR_SOCK_CLIENT_EMPTY;
	}

	size_t i = 0;
	for ( i=0; i<m_ConnPool.nConnCnt; i++ )
	{
		if ( pConnector == m_ConnPool.pConnIndicator[i] )
		{
			break;
		}
	}

	if ( i >= m_ConnPool.nConnCnt )
	{
		return -100;
	}

	SwapLong( (LONG*)(*m_ConnPool.pConnIndicator+i), (LONG*)(*m_ConnPool.pConnIndicator+m_ConnPool.nConnCnt-1) );
	m_ConnPool.nConnCnt--;

	SortMiniHeep( (LONG*)(*m_ConnPool.pConnIndicator), m_ConnPool.nConnCnt );
	SortMiniHeep( (LONG*)((*m_ConnPool.pConnIndicator)+m_ConnPool.nConnCnt), m_ConnPool.nConnCntMax-m_ConnPool.nConnCnt );

	return FUN_RET_OK;
}

VOID CTcpAcceptor::Reset(VOID) // Reset data members
{
	memset( &m_AddrNative, 0, sizeof(m_AddrNative) );
	memset( &m_ConnPool, 0, sizeof(m_ConnPool) );
}
