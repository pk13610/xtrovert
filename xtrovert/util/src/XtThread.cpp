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

#include "XtThread.h"
#include "xt_time_api.h"

#if defined(__WINDOWS__)
/* warning C4100: 'XXX' : unreferenced formal parameter */
#pragma warning (disable: 4127)
#pragma warning (disable: 4100)
#endif

CXtThread::CXtThread()
{
	Reset();
}

CXtThread::~CXtThread()
{
	Release();
}

int CXtThread::Start( void *pParam/*=NULL*/ )
{
	//////////////////////////////////////////////////////////////////////////
	//
	//////////////////////////////////////////////////////////////////////////
	if ( m_nThreadId != 0 ) // We already have an instance of this thread
	{
		return FUN_RET_ERR_ILLEGAL_CALL;
	}

	if ( pParam != NULL )
	{
		if ( SetThreadParam(pParam) < 0 )
		{
			return -5;
		}
	}
	int nRet = 0;
	nRet = xt_thread_create( &m_nThreadId, NULL, (xt_thread_fun_t)CXtThread::fnThreadEntry, this );	
	if ( nRet < 0 )
	{
		m_nThreadId = 0;
		return -6;
	}

#if defined(__WINDOWS__)
	m_hThread = (HANDLE)nRet;
#endif

	return FUN_RET_OK;
}

void CXtThread::Release(void)
{
	/* Stop thread function. */
	Stop();

	BlockWait();

#if defined(__WINDOWS__)
	BOOL bRetTest = FALSE;
	if ( m_hThread != INVALID_HANDLE_VALUE )
	{
		bRetTest = ::CloseHandle( m_hThread );
	}
#elif defined(__LINUX__)
#endif

	Reset();
}

int CXtThread::Suspend()
{
	int nRet = FUN_RET_OK;

	if ( m_nState == TS_SUSPEND )
	{
		return FUN_RET_OK;
	}
	if ( m_nState != TS_RUNNING )
	{
		return -2;
	}

#if defined(__WINDOWS__)
	//do windows specific things here... 
#elif defined (__LINUX__)
	pthread_mutex_lock( &m_Mutex );
	flag--;
	pthread_mutex_unlock( &m_Mutex );
#endif

	return nRet;
}

int CXtThread::Resume()
{
	int nRet = FUN_RET_OK;

	if ( m_nState == TS_RUNNING )
	{
		return FUN_RET_OK;
	}
	if ( m_nState != TS_SUSPEND )
	{
		return -2;
	}


#if defined (__WINDOWS__)
	//do windows specific things here... 
#elif defined (__LINUX__)
	pthread_mutex_lock( &m_Mutex );
	flag++;
	pthread_cond_signal( &m_Condition );
	pthread_mutex_unlock( &m_Mutex );
#endif

	return nRet;
}

int CXtThread::BlockWait(void)
{
	while( m_bStop == FALSE )
	{
		xt_msleep( 50 );
	}

	return FUN_RET_OK;
}

#if defined(_DEBUG)
void CXtThread::SetLabel( const TCHAR *szLabel )
{
	if ( szLabel == NULL )
	{
		return;
	}
	int len =  _tcslen(szLabel);
	if ( len >= _MAX_STR_LEN_8_ )
	{
		len = (_MAX_STR_LEN_8_-1);
	}

	_tcsncpy( m_szLabel, szLabel, len*sizeof(TCHAR) );
	m_szLabel[len] = 0;
}
#endif

/* Setup threads attributes. */
int CXtThread::Setup(void)
{
#if defined (__WINDOWS__)
#elif defined __LINUX__
	pthread_detach( pthread_self() );
	pthread_setcancelstate( PTHREAD_CANCEL_ENABLE, NULL );
	pthread_setcanceltype( PTHREAD_CANCEL_ASYNCHRONOUS, NULL );
#endif
	/* Initialize here */

	return FUN_RET_OK;
}

void* CXtThread::fnThreadEntry( void *pDerivedThread )
{
	if ( pDerivedThread == NULL )
	{
		return NULL;
	}

	//((CXtThread*)pDerivedThread)->Run( ((CXtThread*)pDerivedThread)->GetThreadParam() );
	((CXtThread*)pDerivedThread)->Run();

	return pDerivedThread;
}

void CXtThread::Run(void)
{
	m_bStop = FALSE;
	if ( Setup() < 0 )
	{
		m_bStop = TRUE;
	}

	m_nState = TS_RUNNING;
	while( !m_bStop ) // Should keep running
	{
#if defined(__WINDOWS__)
		//do windows specific things here... 
		//no member variables accessed here so its ok...
#elif defined(__LINUX__)
		pthread_mutex_lock( &m_Mutex );
		while( flag <= 0 )
		{
			m_nState = TS_SUSPEND;
			pthread_cond_wait( &m_Condition, &m_Mutex );
		}
		m_nState = TS_RUNNING;
		pthread_mutex_unlock( &m_Mutex );
#endif
		//actual thread work here
		if ( Execute() < 0 )
		{
			m_bStop = TRUE;
		}
	}

	OnExit();

	m_nState = TS_STOPPED;

}

int CXtThread::Exit( UINT *pRetVal )
{
	UINT nRetValTemp = 0;

	if ( pRetVal == NULL )
	{
		pRetVal = &nRetValTemp;
	}

// #if defined(__WINDOWS__)
// 	xt_thread_exit( pRetVal );
// #elif defined(__LINUX__)
// 	pthread_exit( pRetVal );
// #endif
	xt_thread_exit( pRetVal );

	OnExit();

	return FUN_RET_OK;
}

void CXtThread::OnExit(void)
{
	_tprintf( _T("Thread[%d] Exits!\n"), m_nThreadId );

#if defined(__WINDOWS__)
	BOOL bRetTest = FALSE;
	bRetTest = ::CloseHandle( m_hThread );
#elif defined(__LINUX__)
#endif

	Reset();
}

//void CXtThread::Execute( void *pParam )
int CXtThread::Execute(void)
{
	_tprintf( _T("Thread[%d] Starts!\n"), m_nThreadId );
	/* Execute code */
	if ( m_pParam != NULL )
	{

	}
	return FUN_RET_OK;
}

void CXtThread::Reset(void)
{
#if defined (__WINDOWS__)
	m_hThread = INVALID_HANDLE_VALUE;
#endif
	m_nThreadId = 0;
	m_nState = TS_STOPPED;
	m_bStop = TRUE;
	m_pParam = NULL;
}
