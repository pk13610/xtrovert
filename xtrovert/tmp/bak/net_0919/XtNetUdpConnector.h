/************************************************************************************
* XtNetUdpConnector.h : header file
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

#ifndef __XT_NET_UDP_CONNECTOR_H__
#define __XT_NET_UDP_CONNECTOR_H__

#include "net/inc/XtEventHandler.h"

/* UDP Connector */
class CXtNetUdpConnector : public CXtEventHandler
{
public:
	// non-block
	CXtNetUdpConnector(void);
	virtual ~CXtNetUdpConnector(void);

	/* Create event struct */
	virtual int				Init( void *arg );
	virtual void			AttachInit( xt_event_fd_t fd );
	virtual void			Release(void);

	/*virtual void			Ping(void);*/

protected:

	virtual void			Reset(void);

	virtual int				HandleEvents( xt_event_t events ); /* , void *arg  */

	virtual int				InitConnect( const TCHAR *szAddrPeer );

	virtual int				OnRecv(void);
	virtual int				OnSend(void);
	virtual int				OnError( int nErroCode ); /* Return action for each nErroCode */

	char					*m_pSendBuf;
	char					*m_pRecvBuf;

	BOOL					m_bReadyRecv;
	BOOL					m_bReadySend;

private:
};

/**********************************************
 *			class CXtUdpConnPool
 **********************************************/
class CXtUdpConnPool : public CXtObjPool
{
public:
	CXtUdpConnPool(void);
	virtual ~CXtUdpConnPool(void);

protected:

	DIRECT_BIND_BOJECT(CXtNetUdpConnector)
	virtual void				ReleaseObj( void *pObjAddr );

private:
};

#endif /*__XT_NET_UDP_CONNECTOR_H__*/
