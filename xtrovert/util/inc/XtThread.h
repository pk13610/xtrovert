/************************************************************************************
* XtThread.h : header file
*
* header
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
#ifndef __XT_THREAD_H__
#define __XT_THREAD_H__

#if defined(__WINDOWS__)
#pragma warning (disable: 4200) /* Ignore:  warning C4200: nonstandard extension used : zero-sized array in struct/union */
#endif

#include "xt_thread_api.h"

enum EXT_THREAD_STATE
{
	TS_STOPPED = 0,
	TS_RUNNING,
	TS_SUSPEND,

	THREAD_STATE_END
};

class CXtThread
{
public:
	CXtThread(void);
	virtual ~CXtThread(void);

	virtual int					Start( void *pParam=NULL );
	virtual void				Release(void);

	virtual int					Suspend(void);
	virtual int					Resume(void);

	virtual void				Stop(void)						{ m_bStop = TRUE; }

	virtual int					BlockWait(void); /* After stop() called. Called in other threads to detect until this Thread ends. */

	virtual xt_thread_t			GetThreadId(void)				{ return m_nThreadId; }
	virtual int					GetState(void)					{ return m_nState; }

#if defined(_DEBUG)
	void						SetLabel( const TCHAR *szLabel );
	TCHAR						m_szLabel[_MAX_STR_LEN_8_];
#endif

protected:

	/* Setup threads attributes. */
	virtual int					Setup(void);

	virtual int					SetThreadParam( void *pParam )	{ m_pParam = pParam; return FUN_RET_OK; }
	virtual void*				GetThreadParam(void)	const	{ return m_pParam; }

	virtual void				Run(void);
	virtual int					Execute(void);

	virtual int					Exit( UINT *pRetVal );
	virtual void				OnExit(void);

	virtual void				Reset(void);

#if defined (__WINDOWS__)
	HANDLE						m_hThread;
#endif

	xt_thread_t					m_nThreadId;
	void						*m_pParam;
	int							m_nState;
	BOOL						m_bStop;

#if defined (__LINUX__)
	pthread_mutex_t m_Mutex = PTHREAD_MUTEX_INITIALIZER;
	pthread_cond_t m_Condition = PTHREAD_COND_INITIALIZER;
#endif

private:

	static void*				fnThreadEntry( void *pDerivedThread );
};

#endif /*__XT_THREAD_H__*/
