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

#ifndef __XT_NET_ACCEPTOR_H__
#define __XT_NET_ACCEPTOR_H__

#include "net/inc/XtEvtHndlrNet.h"

class CXtNetConnector;
class CXtConnPool;

typedef struct _tag_xt_acceptor_cfg_t
{
	const TCHAR		*local_addr;
	CXtConnPool		*conn_pool;
}xt_acceptor_cfg_t;

/*****************************************
 *			Class CXtNetAcceptor
 *****************************************/
/* TCP Acceptor */
/* Start listener service */
/* Client connection management */
class CXtNetAcceptor : public CXtNetEventHandler
{
public:
	// non-block
	CXtNetAcceptor(void);
	virtual ~CXtNetAcceptor(void);

	/* Create socket according to local ip-address */
	virtual int				Init( void *pArg );
	virtual void			Release(void);

protected:

#define _connector_count_			128

	virtual void			Reset(void);

	virtual int				HandleEvents( xt_event_t nEvents );

	virtual xt_event_fd_t	InitListener( const TCHAR *szAddr );
	/* Allocate memory for new connector. */
	virtual CXtNetConnector*	AllocConnector( xt_obj_id_t *pObjId );

	virtual int				Accept( CXtNetConnector *pconnector );

	CXtConnPool				*m_pConnPool;

private:
};

#endif /*__XT_NET_ACCEPTOR_H__*/
