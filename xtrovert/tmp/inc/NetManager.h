/************************************************************************************
* NetManager.h : header file
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

#ifndef __NETMANAGER_H__
#define __NETMANAGER_H__

#include "NetHostInfo.h"
#include "NetReactor.h"
#include "IOModelAPI.h"

typedef struct _tagNET_SLOT_DESC /* Net Slot Descriptor for each client on server side */
{
	size_t	nSlotID;	/* Client slot ID in network layer */
	UINT	nBirthTime;	/* Time of connected or first message received */
	UINT	nLastShot;	/* Message exchanging time lately */
}NET_SLOT_DESC;

typedef struct _tagNET_SLOT_EVENT_LIST
{
	NET_SLOT_DESC	*pSlotDesc;
	size_t			nCount;
}NET_SLOT_NEW, NET_SLOT_LOST, NET_SLOT_NEW_MESSAGE;

typedef struct _tagNET_SLOT_NOTIFY
{
	NET_SLOT_DESC	*pSlotDesc;
	INT			nEvent;
}NET_SLOT_NOTIFY;

class CNetManager
{
public:
	CNetManager(VOID)
	{

	}
	virtual ~CNetManager(VOID)
	{

	}

// 	virtual INT			Init();
// 	virtual INT			Setup( VOID *pArg );
// 	virtual INT			Start( VOID *pArg ); /* Run Thread-Function */
// 	virtual INT			Stop(VOID); /* Close all connections and release memory */
// 
// 	virtual INT			Tick( NET_SLOT_NEW *pLstSlotNew, NET_SLOT_LOST *pLstSlotLost, NET_SLOT_NEW_MESSAGE *pLstNewMsg, NET_SLOT_NOTIFY *pOtherNotify );
// 
// 	virtual INT			Start( CONST NET_SLOT_DESC *pSlotDesc );
// 	virtual INT			Shutdown( CONST NET_SLOT_DESC *pSlotDesc, INT nMode );
// 
// 	virtual INT			QuerySlotSockAddr( CONST NET_SLOT_DESC *pSlotDesc, SOCKADDR_STORAGE *pAddr, SOCKLEN_T *pAddrLen );
// 	virtual INT			QuerySlotSockAddr( CONST NET_SLOT_DESC *pSlotDesc, TCHAR *szAddr, INT *pAddrLen );
// 	virtual INT			FilterSlot( CONST NET_SLOT_DESC *pSlotDesc, INT nMode ); /* Disable,Enable,Ignore£¬Resume Peer */
// 
// 	virtual INT			GetNetSvrCount(VOID);
// 	virtual INT			GetPeerCount( NET_SLOT_DESC &slot_id );
// 	virtual INT			AddNetServer( CONST INET_HOST_INFO *pInfo=NULL, CONST INET_SVR_OPT *pOpt=NULL );
// 	virtual INT			RemNetServer( NET_SLOT_DESC &slot_id );
// 
// 	virtual INT			GetClientCount(VOID);
// 	virtual INT			AddNetClient( CONST INET_HOST_INFO *pMyInfo=NULL, CONST INET_HOST_INFO *pPeerInfo=NULL );
// 	virtual INT			RemNetClient( NET_SLOT_DESC &slot_id );
// 
// protected:
// 	virtual INT			Release(VOID);

	CNetReactor			*m_pReactor;
	CEventHandler		*m_pEventHndPool;
	SOCKADDR			*m_pAddrUdpPool;
	BYTE				*m_pMessageBuf;
private:
};

#endif /*__NETMANAGER_H__*/
