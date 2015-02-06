/************************************************************************************
* XtNetEvtHandler.h : header file
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

#ifndef __XT_NETEVTHANDLER_H__
#define __XT_NETEVTHANDLER_H__

#include "XtEventHandler.h"
#include "xt_net_api.h"
#include "net/inc/xt_net_msgbuf.h"

/*
	XT_EV_START = 0x40
	XT_NETEV_DISCONNECT = 0x200
*/

/******************************************************************
	Customize Events
		Connected, Disconnected
******************************************************************/
#define XT_NETEV_CONNECTED			(XT_EV_START)
#define XT_NETEV_DISCONNECTED		((XT_NETEV_CONNECTED)<<1)
/******************************************************************
	Customize Operations:
		Connect, Disconnect
******************************************************************/
#define XT_NETEV_CONNECT			((XT_NETEV_DISCONNECTED)<<1)
#define XT_NETEV_DISCONNECT			((XT_NETEV_CONNECT)<<1)
#define XT_NETEV_SHUTDOWN			((XT_NETEV_DISCONNECT)<<1)

typedef struct _tag_xt_sockaddr_t
{
	SOCKLEN_T				addrlen;
	SOCKADDR				*saddr;
}xt_sockaddr_t;

typedef struct _tag_xt_net_evthandler_cfg_t
{
	xt_sockaddr_t				sock_addr;
	xt_event_fd_t				*evt_fd_ptr;
}xt_net_evthandler_cfg_t;

/*****************************************
 *			Class CXtNetEvtHandler
 *****************************************/
class CXtNetEvtHandler : public CXtEventHandler
{
public:
	CXtNetEvtHandler(void);
	virtual ~CXtNetEvtHandler(void);

	virtual int				Init( void *pCfg );
	virtual void			Release(void);

	virtual int				HandleEvents( xt_event_t nEvents, void *pArg );

protected:

	virtual void			Reset(void);

	virtual void			OnRegisterGetFD( xt_event_fd_t *pWhat );
	virtual void			OnRegisterGetEvents( xt_event_t *pWhat );
	virtual void			OnRegisterGetTimeout( struct timeval *pWhat );

	int						AllocSockAddr( SOCKADDR *pSockAddr );
	void					ReleaseSockAddr(void);

	xt_event_fd_t			CreateSocket( int nSockType );
	virtual int				BindAddress(void);

	xt_sockaddr_t			*m_pSockAddr;

private:
};

/*****************************************
 *			Class CXtNetEvtHandler END
 *****************************************/

#endif /*__XT_NETEVTHANDLER_H__*/
