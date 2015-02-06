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

#ifndef __XT_NET_CONNECTOR_H__
#define __XT_NET_CONNECTOR_H__

#include "net/inc/XtEvtHndlrNet.h"

typedef struct _tag_xt_pipe_buffer_t
{
	xt_size_t			capacity;
	xt_size_t			data_size;
	xt_size_t			head;
	xt_size_t			tail;
	char				buffer[0];
}xt_pipe_buf_t;

#define xt_pipe_buf_clear(buf_p)							\
						do									\
						{									\
							(buf_p)->head = 0;				\
							(buf_p)->tail = 0;				\
							(buf_p)->data_size = 0;			\
						} while (__LINE__==-1)


/*****************************************
 *			Class CXtNetConnector
 *****************************************/

#define ERR_CONNECTOR_OK			FUN_RET_OK
#define ERR_CONNECTOR_FATAL			-100

/* TCP Connector */
class CXtNetConnector : public CXtNetEventHandler
{
public:
	// non-block
	CXtNetConnector(void);
	virtual ~CXtNetConnector(void);

	/* Reinit is allowed. Connector could connect to server 'A' for a while, then switch to server 'B'. */
	virtual int				Init( void *arg );
	virtual int 			AttachInit( void *pArg );
	virtual int				Release(void);

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
 *			class CXtEventPool
 **********************************************/
class CXtConnPool : public CXtObjPool
{
public:
	CXtConnPool(void);
	virtual ~CXtConnPool(void);

	/*xt_event_t*/

protected:

	DIRECT_BIND_BOJECT(CXtNetConnector)
	virtual void				ReleaseObj( void *pObjAddr );

private:
};

#endif /*__XT_NET_CONNECTOR_H__*/
