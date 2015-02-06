/************************************************************************************
* TcpConnector.h : header file
*
* Class Definition header
*
* AUTHOR	:	Sean Feng <SeanFeng2006@hotmail.com>
* DATE		:	Aug. 7, 2012
* Copyright (c) 2009-?. All Rights Reserved.
*
* This code may be used in compiled form in any way you desire. This
* file may be redistributed unmodified by any means PROVIDING it is 
* not sold for profit without the authors written consent, and 
* providing that this notice and the authors name and all copyright 
* notices remains intact. 
*
* An email letting me know how you are using it would be nice as well. 
*
* This file is provided "as is" with no expressed or implied warranty.
* The author accepts no liability for any damage/loss of business that
* this product may cause.
*
************************************************************************************/

#ifndef __TCP_CONNECTOR_H__
#define __TCP_CONNECTOR_H__

#include "NetHostInfo.h"
#include "ExtSocketTcp.h"
#include "NetReactor.h"
#include "SktMsgQueue.h"

enum NET_EVENT_TYPES
{
	NET_EVENT_TYPES_OK = 0,
	NET_EVENT_NEW_CONNECTION,
	NET_EVENT_LOST_CONNECTION,
	NET_EVENT_NEW_PACKET,
	NET_EVENT_TYPES_END
};

typedef struct _tagTCP_CONNECTOR_CFG
{
	USHORT			nFamily; /* AF_INET AF_INET6 AF_UNIX */
	TCHAR			szAddNative[INET_ADDRSTRLEN_MAX];	/*IP String like 127.0.0.1 7788*/
	TCHAR			szAddrPeerr[INET_ADDRSTRLEN_MAX];	/*IP String like 127.0.0.1 7788*/
}TCP_CONNECTOR_CFG;

typedef struct _tagINET_CONNECTOR_PARAM
{
	INT					nBackLog;
	INT					nProtocol;
}INET_CONNECTOR_PARAM;


class CTcpConnector : public CNetEvtHandler
{
public:
	CTcpConnector(VOID);
	virtual ~CTcpConnector(VOID);

	virtual INT				Init( CONST TCP_CONNECTOR_CFG *pInfo, CONST INET_CONNECTOR_PARAM *pParam=NULL );
	virtual INT				Start(VOID);

	virtual BOOL			IsValid(VOID);

	virtual INT				Close(VOID); /* Close all connections */
	virtual INT				Release(VOID); /* Release native memory */

	virtual INT				SendPacket( CONST CPacketBase *pkt );
	virtual INT				RecvPacket( CPacketBase *pkt );

	virtual INT				Attach( CExtSocketTcp *pSocket ); /* Passive Connected */

	//virtual INT				HandleEvents( INT nEvents );

	virtual NET_SKT_TYPE	GetSocketType()					{ return m_sktNative.GetSocketType(); }

protected:
	virtual INT				OnRead();
	virtual INT				OnWrite();
	virtual INT				OnTimeout();
	virtual INT				OnClose();

	virtual VOID			Reset(VOID); // Reset data members

protected:
	SOCKADDR_STORAGE		m_AddrNative;
	SOCKADDR_STORAGE		m_AddrPeer;

	CExtSocketTcp			m_sktNative;
	
	CSktMsgQueInput			*m_pQueMsgIn;
	CSktMsgQueOutPut		*m_pQueMsgOut;
private:
};

#endif /*__TCP_CONNECTOR_H__*/
