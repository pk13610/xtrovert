/************************************************************************************
* XtNetUdpAcceptor.h : header file
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

#ifndef __XT_NET_UDP_ACCEPTOR_H__
#define __XT_NET_UDP_ACCEPTOR_H__

#include "net/inc/XtEventHandler.h"

class CXtNetUdpConnector;
class CXtUdpConnPool;

/*****************************************
 *			Class CXtNetAcceptor
 *****************************************/
typedef struct _tag_xt_acceptor_cfg_t
{
	const TCHAR		*local_addr;
	CXtUdpConnPool	*conn_pool;
}xt_acceptor_cfg_t;

/* UDP Acceptor */
class CXtNetUdpAcceptor : public CXtEventHandler
{
public:
	CXtNetUdpAcceptor(void);
	virtual ~CXtNetUdpAcceptor(void);

	/* Create socket according to local ip-address */
	virtual int				Init( void *arg );
	virtual void			AttachInit( xt_event_fd_t fd );
	virtual void			Release(void);

protected:

#define _connector_count_			128

	virtual void			Reset(void);

	virtual int				HandleEvents( xt_event_t events );

	virtual xt_event_fd_t	InitListener( const TCHAR *szAddr );

	virtual CXtNetUdpConnector*	AllocConnector( xt_obj_id_t *pObjId );

	CXtUdpConnPool			*m_pConnPool;

private:
};

#endif /*__XT_NET_UDP_ACCEPTOR_H__*/
