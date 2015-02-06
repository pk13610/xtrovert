/************************************************************************************
* XtReactor.h : header file
*
* Reactor
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


#ifndef __XT_REACTOR_H__
#define __XT_REACTOR_H__

#include "xt_net_api.h"
#include "XtObjPool.h"

#include <event2/event.h>

#include "XtEventHandler.h"


/*****************************************
 *			Class CXtReactor
 *****************************************/
/*
	Flags:
		force_select: 强制使用select机制
*/
#define _XT_REACT_HANDLER_SIZE_DEF_			32
class CXtReactor
{
public:

	/* Constructor & Destructor */
	CXtReactor(void);
	~CXtReactor(void);
	int						Init( xt_size_t nHandlerCount ); /* nHandlerSize: Initial handler count */
	int						Reinit(void); /* Call for after fork */
	void					Release(void);

	int						RegisterHandler( CXtEventHandler *pEvtHandler, const struct timeval *tmOut );
	int						RemoveHandler( CXtEventHandler *pEvtHandler );

	/* Waiting incoming events */
	int						Dispatch(void); /* Block calling thread. */
	int						Stop(void);

	BOOL					IsValid(void)										{ return is_valid; }

	xt_size_t				GetHandlerCount(void)								{ return m_EventPool.GetObjCount(); }

	/* Setup Reactor mechanism before Init()  */
	int						SetupForceSelect(void);

	int						SetCpuHint( int nCpus );

	/* Optimize */
	void					SetupMemFun( void *(*malloc_fn)(size_t sz), void *(*realloc_fn)(void *ptr, size_t sz), void (*free_fn)(void *ptr) );

	const TCHAR*			QueryBackend(void);

protected:

	static void				OnHandleEvents( xt_event_fd_t fd, short events, void *arg );

	void					Reset(void);

#if defined(__WINDOWS__)
	/* IOCP reactor */
	int						IocpInit(void);
#endif

	xt_event_t*				EvtPoolAlloc( xt_obj_id_t *pObjId );

	#define is_valid						(m_pEvtBase!=NULL)

	struct event_base		*m_pEvtBase;
	struct event_config		*m_pEvtBaseCfg;

	CXtEventPool			m_EventPool;
	xt_size_t				m_nEvPoolGrow;
	xt_evt_handle_t			m_nEvtHandleStart;

	#define _INVALID_EVT_HANDLE_START_	m_nEvtHandleStart
	#define new_event_base_cfg			m_pEvtBaseCfg = event_config_new()

private:
};

#define XT_REACT_ERR_OK						FUN_RET_OK
#define XT_REACT_INIT_FAIL					-4
#define XT_REACT_ERR_MEM_FAIL				-5

/*****************************************
 *			Class CXtReactor END
 *****************************************/

#endif /*__XT_REACTOR_H__*/
