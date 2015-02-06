/************************************************************************************
* XtNetAcceptor.h : header file
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

#ifndef __XT_NETACCEPTOR_H__
#define __XT_NETACCEPTOR_H__

#include "net/reactor/XtNetConnector.h"

typedef struct _tag_xt_acceptor_cfg_t
{
	xt_sockaddr_t			*evt_sock_addr;
	CXtNetConnPool			*conn_pool;
}xt_acceptor_cfg_t;

class CXtNetAcceptor : public CXtNetEvtHandler
{
public:
	CXtNetAcceptor(void);
	virtual ~CXtNetAcceptor(void);

	virtual int				Init( void *pCfg );
	virtual void			Release(void);

	virtual int				HandleEvents( xt_event_t nEvents, void *pArg );

protected:

	virtual void			Reset(void);

	virtual int				InitAcceptor( int nSockType );

	virtual int				Kick( CXtNetConnector *pConnector );

private:
};

#endif /*__XT_NETACCEPTOR_H__*/
