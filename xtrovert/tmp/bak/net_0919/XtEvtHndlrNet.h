/************************************************************************************
* XtEvtHndlrNet.h : header file
*
* Event Handler class for signal
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

#ifndef __XT_EVT_HNDLR_NET_H__
#define __XT_EVT_HNDLR_NET_H__

#include "net/inc/XtEventHandler.h"

typedef struct _tag_xt_sockaddr_t
{
	SOCKLEN_T		len;
	SOCKADDR		saddr[0];
}xt_sockaddr_t;

typedef struct _tag_net_evt_hdlr_cfg_t
{
	xt_event_fd_t			evt_fd;
	xt_sockaddr_t			*saddr_buf;
}net_evt_hdlr_cfg_t;

typedef struct _tag_attach_cfg_t
{
	xt_event_fd_t	evt_fd;
	xt_sockaddr_t	*saddr_buf;
};

/* APPLICATION ERRORS */

/* SYSTEM ERRORS */
/*
IGNROE ERROR:
	EWOULDBLOCK,EAGAIN,E_IN_PROGRESS
*/
/*
*Initialization Errors, exceptions while initialize socket.
	Local-Address is invalid: [Local-ip:Port], socket can NOT be created over this address.
	Reinitialization: [socket-fd, Local-ip:Port], attempt to initialize a valid socket.
	Address is in use: [Local-ip:Port], Address(Port) is currently used by another process.
	unknown error: [Local-ip:Port, errno], some unknown errors.

*Runtime Errors, exceptions while carry on some operation.
	Peer closed: [socket-fd, Local-ip:Port,Peer-ip:Port], Peer socket closed.
	Local closed: [socket-fd, Local-ip:Port,Peer-ip:Port]
	Accept error: [socket-fd, Local-ip:Port,Peer-ip:Port]
	Receive error: [socket-fd, Local-ip:Port,Peer-ip:Port,errno] receiving message failed.
	Send error: [socket-fd, Local-ip:Port,Peer-ip:Port,errno] sending message failed.
	Peer Address unreachable(WSA:10051,): [socket-fd, Local-ip:Port,Peer-ip:Port], No route to peer-host.
	Connection Timeout(WSA:10060): [socket-fd, Local-ip:Port,Peer-ip:Port]Connection timed out, connection attempt failed. Packets lost?
	Connection Refused(WSA:10061): [socket-fd, Local-ip:Port,Peer-ip:Port]Host is alive, but Peer-Server Application is NOT running, probably. Firewall?
	Host-down(WSA:10064):[socket-fd, Local-ip:Port,Peer-ip:Port], Destination host is NOT alive at all
	Dead Network(WSA:10050):

	unknown error: [socket-fd, Local-ip:Port, Peer-ip:Port, errno], some unknown errors.
*/

/**********************************************
*			class CXtNetEventHandler
 **********************************************/
class CXtNetEventHandler : public CXtEventHandler
{
public:
	CXtNetEventHandler(void);
	virtual ~CXtNetEventHandler(void);

	virtual int				Init( void *pArg );
	virtual int				AttachInit( void *pArg );
	virtual void			Release(void);

protected:
	
	virtual void			Reset(void);

	virtual int				SetNonblock( BOOL bBlock );

	/* local-sock-addr for server */
	/* peer-sock-addr for client */
	xt_sockaddr_t			*m_pSAddr;

private:
};

#endif /*__XT_EVT_HNDLR_NET_H__*/
