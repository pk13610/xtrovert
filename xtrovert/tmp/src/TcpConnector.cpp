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

#include "TcpConnector.h"

CTcpConnector::CTcpConnector(VOID)
{
	Reset();
}

CTcpConnector::~CTcpConnector(VOID)
{
	Reset();
}

INT CTcpConnector::Init( CONST TCP_CONNECTOR_CFG *pInfo, CONST INET_CONNECTOR_PARAM *pParam/*=NULL*/ )
{
	assert(pParam);
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

	nLen = sizeof(m_AddrPeer);	
	nRet = ExtSktAPI::fill_sock_addr_struct( (SOCKADDR*)&m_AddrPeer, &nLen, pInfo->szAddrPeerr, pInfo->nFamily );
	if ( nRet < 0 )
	{
		return nRet;
	}

	nRet = AllocQue();

	return nRet;
}

INT CTcpConnector::Start(VOID)
{
	INT nRet = FUN_RET_OK;
	SOCKET_ID nSocket;
	nSocket = m_sktNative.Create( (SOCKADDR*)&m_AddrNative, sizeof(m_AddrNative), 0 );
	if ( nSocket == INVALID_SOCKET )
	{
		return ERR_SOCK_FD_INIT_FAILED;
	}

	nRet = m_sktNative.Connect( (SOCKADDR*)&m_AddrPeer, sizeof(m_AddrPeer) );
	if ( nRet < 0 )
	{
		m_sktNative.Close();
		return ERR_SOCK_FD_CONNECT_FAILED;
	}

	return FUN_RET_OK;
}

INT CTcpConnector::Close(VOID)
{
	return m_sktNative.Close();
}

INT CTcpConnector::Release(VOID)
{
	m_sktNative.Close();

	if ( m_pQueMsgIn != NULL )
	{
		m_pQueMsgIn->Release();
		delete m_pQueMsgIn;
	}

	if ( m_pQueMsgOut != NULL )
	{
		m_pQueMsgOut->Release();
		delete m_pQueMsgOut;
	}
	Reset();
	return FUN_RET_OK;
}

INT CTcpConnector::Attach( CExtSocketTcp *pSocket ) /* Passive Connected */
{
	if ( pSocket == NULL )
	{
		return FUN_RET_ERR_ILLEGAL_ARGS;
	}

	if ( pSocket->IsValid() == FALSE )
	{
		return FUN_RET_ERR_ILLEGAL_CALL;
	}

	if ( m_sktNative.IsValid() == TRUE )
	{
		return -3;
	}

	INT nRet = FUN_RET_OK;
	nRet = m_sktNative.Attach ( pSocket->GetSocketID() );
	if ( m_pQueMsgIn==NULL || m_pQueMsgOut==NULL )
	{
		nRet = AllocQue();
	}

	return nRet;
}

// INT CTcpConnector::HandleEvents( INT nEvents )
// {
// 	if ( nEvents & (EPOLLERR | EPOLLHUP) )
// }

INT CTcpConnector::OnRead()
{
	m_pQueMsgIn->RecvMessage( &m_sktNative );
	return FUN_RET_OK;
}

INT CTcpConnector::OnWrite()
{
	m_pQueMsgOut->SendMessage( &m_sktNative );
	return FUN_RET_OK;
}

INT CTcpConnector::OnTimeout()
{
	return FUN_RET_OK;
}

INT CTcpConnector::OnClose()
{
	m_sktNative.Close();
	return FUN_RET_OK;
}

VOID CTcpConnector::Reset(VOID) // Reset data members
{
	memset( &m_AddrNative, 0, sizeof(m_AddrNative) );
	memset( &m_AddrPeer, 0, sizeof(m_AddrPeer) );

	m_pQueMsgIn = NULL;
	m_pQueMsgOut = NULL;
}

INT CTcpConnector::AllocQue(VOID)
{
	INT nRet = FUN_RET_OK;
	try
	{
		if ( m_pQueMsgIn == NULL )
		{
			m_pQueMsgIn = new CSktMsgQueInput;
		}
	}
	catch (...)
	{
		m_pQueMsgIn = NULL;
		return ERR_HEAPBLOCK_BUF_INIT_FAIL;
	}

	try
	{
		if ( m_pQueMsgOut == NULL )
		{
			m_pQueMsgOut = new CSktMsgQueOutPut;
		}
	}
	catch (...)
	{
		delete m_pQueMsgIn;
		m_pQueMsgIn = NULL;
		m_pQueMsgOut = NULL;
		return ERR_HEAPBLOCK_BUF_INIT_FAIL;
	}

	nRet = m_pQueMsgIn->Create();
	if ( nRet < 0 )
	{
		return ERR_HEAPBLOCK_BUF_INIT_FAIL;
	}
	nRet = m_pQueMsgOut->Create();
	if ( nRet < 0 )
	{
		Release();
		return ERR_HEAPBLOCK_BUF_INIT_FAIL;
	}

	return nRet;
}
