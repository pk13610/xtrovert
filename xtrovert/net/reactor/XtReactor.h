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

#include "xt_define.h"
#include <event2/event_struct.h>
#include "xt_reactor_define.h"

/* Declaration */

/*****************************************
 *			Class CXtReactor
 *****************************************/
/*
	Flags:
		force_select: 强制使用select机制
*/
class CXtReactor
{
public:

	/* Constructor & Destructor */
	CXtReactor(void);
	~CXtReactor(void);
	int						Init(void);
	int						Reinit(void); /* Call for after fork */
	void					Release(void);

	xt_reactor_t*			GetContext(void)						{ return m_pReactor; }

	/* Waiting incoming events */
	int						Dispatch(void); /* Block the calling thread. */
	int						Stop( struct timeval *pDelay );

	BOOL					IsValid(void)										{ return (m_pReactor!=NULL); }

	/* Setup Reactor mechanism before Init()  */
	int						SetupForceSelect(void);

	int						SetCpuHint( int nCpus );

	/* Optimize */
	void					SetupMemFun( void *(*malloc_fn)(size_t sz), void *(*realloc_fn)(void *ptr, size_t sz), void (*free_fn)(void *ptr) );

	const TCHAR*			QueryBackend(void);

protected:

	void					Reset(void);

#if defined(__WINDOWS__)
	/* IOCP reactor */
	int						IocpInit(void);
#endif

	xt_reactor_t			*m_pReactor;
	struct event_config		*m_pReactorCfg;

	#define is_valid					(m_pReactor!=NULL)
	#define new_event_base_cfg			m_pReactorCfg = event_config_new()

private:
};

/*****************************************
 *			Class CXtReactor END
 *****************************************/

#endif /*__XT_REACTOR_H__*/
