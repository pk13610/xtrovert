/************************************************************************************
* XtNetConnector.h : header file
*
* Event Handler class
*
* AUTHOR	:	Sean Feng <SeanFeng2006@hotmail.com>
* DATE		:	Nov. 7, 2009
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

#ifndef __XT_NETCONNECTOR_H__
#define __XT_NETCONNECTOR_H__

#include "XtNetEvtHandler.h"
#include "XtObjPool.h"
#include "xt_mem_chunk.h"

#define XT_CONN_ERR_PEERCLOSED				(-100)
#define XT_CONN_ERR_BADFD					(XT_CONN_ERR_PEERCLOSED-1)
/*xt_STD_ERR_ETIMEDOUT,The connection has been dropped, because of a network failure or because the system on the other end went down without notice.*/
#define XT_CONN_ERR_CONNDROP				(XT_CONN_ERR_PEERCLOSED-2)
#define XT_CONN_ERR_SERVICEDOWN				(XT_CONN_ERR_PEERCLOSED-3)
/*xt_STD_ERR_EHOSTUNREACH,The remote host cannot be reached from this host at this time.*/
#define XT_CONN_ERR_UNREACH					(XT_CONN_ERR_PEERCLOSED-4)
/*xt_STD_ERR_ECONNABORTED,The virtual circuit was terminated due to a time-out or other failure. The application should close the socket as it is no longer usable.*/
#define XT_CONN_ERR_TIMEOUT					(XT_CONN_ERR_PEERCLOSED-5)
#define XT_CONN_ERR_TRYAGAIN				(XT_CONN_ERR_PEERCLOSED-6)

#define XT_CONN_ERR_BUFFULL					(XT_CONN_ERR_PEERCLOSED-7)

class CXtNetEvtHandler;
typedef int (CXtNetEvtHandler::*pfn_net_handle_events_t)( CXtNetEvtHandler *pThis, xt_event_t nEvents, void *pArg );

class CXtNetConnector : public CXtNetEvtHandler
{
public:

	CXtNetConnector(void);
	virtual ~CXtNetConnector(void);

	virtual int				Init( void *pCfg );
	virtual void			Release(void);

	virtual int				HandleEvents( xt_event_t nEvents, void *pArg );

protected:

	virtual void			Reset(void);

	/***********************************************************************/
	virtual int				OnAccepted(void);

	virtual int				OnConnected(void);
	virtual int				OnDisConnected(void);
	virtual int				OnSend(void);				/* Return 0 to ignore default operation */
	virtual int				OnRecv(void);				/* Return 0 to ignore default operation */
	virtual int				OnTimeout(void);			/* Return 0 to ignore default operation */
	virtual int				OnError( int nErrCode );	/* Return 0 to ignore default operation */

	virtual void			SetConnectInterval(void); /* m_Timeout */
	/***********************************************************************/

	/* Data Member */
	pfn_net_handle_events_t		m_pfnHandleEvents;

	CXtNetAcceptor				*m_pAcceptor;	

private:


	/***********************************************************************/
	static int				OnHandleConnectEvents( CXtNetConnector *pThis, xt_event_t nEvents, void *pArg );
	static int				OnHandleMessageEvents( CXtNetConnector *pThis, xt_event_t nEvents, void *pArg );

	static int				HandleAccept( CXtNetConnector *pThis, xt_event_t nEvents, void *pArg );
	static int				HandleAcceptUdp( CXtNetConnector *pThis, xt_event_t nEvents, void *pArg );
	/* Process Connect operation. */
	static int				HandleConnect(CXtNetConnector *pThis, xt_event_t nEvents, void *pArg );
	/* Handle Receive & Send */
	static int				HandleMessage( CXtNetConnector *pThis, xt_event_t nEvents, void *pArg );

	/***********************************************************************/
	int						ProcAccept( void *pArg );

	int						InitConnect(void);
	int						ProcConnect(void);
	int						ProcConnected(void);
	int						Disconnect(void);

	int						ProcRecv(void);
	int						ProcSend(void);

	int						ProcTimeout(void);
	int						ProcError( int nErrCode );
	/***********************************************************************/
};
/*****************************************
 *			Class CXtNetConnector END
 *****************************************/

/*****************************************
 *			Class CXtNetConnPool
 *****************************************/
class CXtNetConnPool : public CXtObjPool
{
public:
	CXtNetConnPool(void);
	virtual ~CXtNetConnPool(void);

	virtual int					SetupConnectorInfo( USHORT nFaminly, int nSockType=NET_SKT_TYPE_TCP );

	virtual void				Release(void);

protected:

	virtual void				Reset(void);

	virtual void				SetupObjSize(void);

	virtual int					InitObj( void *pObjAddr );
	virtual void				ResetObj( void *pObjAddr );
	virtual void				ReleaseObj( void *pObjAddr );

	int						m_nSockType;

	int						m_nRecvBufCapacity;

	SOCKLEN_T				m_nSockLenMax;
	
	USHORT					m_nFamily;

#define reset_conn_sock_len(pconn)		((pconn)->m_nAddrLen = m_nSockLenMax)

private:
};
/*****************************************
 *			Class CXtNetConnPool END
 *****************************************/

#endif /*__XT_NETCONNECTOR_H__*/
