/************************************************************************************
* XtThread.cpp: Implementation File
*
* DESCRIPTION  :   
*
* AUTHOR	    :	Sean Feng <SeanFeng2006@hotmail.com>
* DATE		    :	Sep. 24, 2010
*
* Copyright (c) 2010-?. All Rights Reserved.
*
* REMARKS:
*		
*      
************************************************************************************/

#include "app/inc/XtThread.h"
#include "app/xafx/xafx.h"

#if defined(__WINDOWS__)
/* warning C4100: 'XXX' : unreferenced formal parameter */
#pragma warning (disable: 4127)
#pragma warning (disable: 4100)
#endif

void* XT_THREAD_FUNC *xt_thread_entry( xt_thread_t *thd, void *data );
{
	int ret = FUN_RET_OK;

	XT_DECLARE(xt_state_t) retval;

	CXtThread *pThread = NULL;

	ret = xafx_init_thread();

	/* Check for simple worker thread. */
	if ( pThread->m_pfnThreadProc != NULL )
	{
		retval = (*pThread->m_pfnThreadProc)( pThread->m_pThreadParam );
	}
	else if ( !pThread->InitInstance() ) /* Check for thread with message loop. */
	{
		retval = pThread->ExitInstance();
	}
	else
	{
		/* Will stop after PostQuitMessage called. */
		retval = pThread->Run();
	}

	xt_thread_exit( thd, retval );

	return thd;
}

CXtThread::CXtThread(void)
{
	Reset();
}

CXtThread::CXtThread( xt_thread_proc_t pfnThreadProc, void *pData )
{
	m_pfnThreadProc = pfnThreadProc;
	m_pThreadParam = pData;
}

CXtThread::~CXtThread(void)
{
	Reset();
}

int CXtThread::Create( DWORD dwCreateFlags/*=0*/, UINT nStackSize/*=0*/, const xt_threadattr_t *pAttrs/*=NULL*/ )
{
	int ret = FUN_RET_OK;

#if !defined(_MT)
	dwCreateFlags;
	nStackSize;
	pAttrs;
	return XT_ENOTIMPL;
#else
	
	xt_threadattr_t attr;

	ret = xt_threadattr_create( &attr, FALSE );

	ret = xt_threadattr_stacksize_set( &attr, nStackSize );
	
	ret = xt_thread_create( &m_ThreadInfo, &attr, xt_thread_entry, &data, FALSE );

	return FUN_RET_OK;
#endif
}

BOOL xt_peek_message( xt_appmsg_t *pAppMsg, HANDLE hnd, UINT wMsgFilterMin, UINT wMsgFilterMax, UINT wRemoveMsg )
{
	return FALSE;
}

int CXtThread::Run(void)
{
	BOOL bIdle = TRUE;
	LONG lIdleCount = 0;

	xt_appmsg_t msg;

	for(;;)
	{
		while ( bIdle && !::xt_peek_message(&msg, NULL, NULL, NULL, PM_NOREMOVE) )
		{
			if ( !OnIdle(lIdleCount++) )
			{
				bIdle = FALSE;
			}
		}

		do 
		{
			if ( !PumpMessage() )
			{
				return ExitInstance();
			}

			if ( IsIdleMessage(&msg) )
			{
				bIdle = TRUE;
				lIdleCount = 0;
			}
		} while ( ::xt_peek_message(&msg, NULL, NULL, NULL, PM_NOREMOVE) );
		
	}

	return FUN_RET_OK;
}

 void CXtThread::Reset(void)
 {
	 m_pfnThreadProc = NULL;
	 m_pThreadParam = NULL;
 }


/**********************************************************************
*		CLASS CXtCmdShell END
**********************************************************************/
