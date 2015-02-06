/************************************************************************************
*	ExtReactor.cpp: Implementation File
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

#include "XtReactor.h"

#if defined(__WINDOWS__)
/* warning C4100: 'XXX' : unreferenced formal parameter */
#pragma warning (disable: 4100)
#endif

/*****************************************
 *			Class CXtReactor
 *****************************************/
CXtReactor::CXtReactor(VOID)
{
	Reset();
}

CXtReactor::~CXtReactor(VOID)
{
	Release();
}

int CXtReactor::Init(void)/* xt_size_t nHandlerCount */
{
	/* nHandlerSize: Initial handler count */
	int nRet = FUN_RET_OK;
	if ( m_pReactor ) /* != NULL */
	{
		return FUN_RET_ERR_ILLEGAL_CALL;
	}

	/* Initialize event_base struct */
// 	if ( force_select )
// 	{
// 		m_pReactor = event_base_new();
// 		if ( m_pReactor == NULL )
// 		{
// 			nRet = XT_REACT_ERR_MEM_FAIL;
// 		}
// 	}
// 	else
	{
#if defined(__WINDOWS__) /* IOCP with default */
		nRet = IocpInit();
		if ( nRet < 0 )
		{
			nRet = XT_REACT_ERR_MEM_FAIL;
		}
#elif defined(__LINUX__)
		m_pReactor = event_base_new();
		if ( m_pReactor == NULL )
		{
			nRet = XT_REACT_ERR_MEM_FAIL;
		}
#else /* Under other OS, it's simple. libevent would choose a back-end. */
		nRet = FUN_RET_ERR_NOTIMPL;
#endif /*(__WINDOWS__)*/
	}

	if ( nRet < 0 )
	{
		return nRet;
	}

	return nRet;
}

int CXtReactor::Reinit(void)
{
	/* Call for after fork */
	int nRet = FUN_RET_OK;
	
	if ( m_pReactor == NULL )
	{
		return FUN_RET_ERR_ILLEGAL_CALL;
	}

	nRet = event_reinit( m_pReactor );
	if ( nRet < 0 )
	{
		return XT_REACT_INIT_FAIL;
	}
	return FUN_RET_OK;	
}

void CXtReactor::Release(void)
{
	/* Release all struct event first
		event_del();
	*/
	Stop( NULL );

	Reset();
}

int CXtReactor::Dispatch(void)
{
	int nRet = FUN_RET_OK;

	if ( m_pReactor == NULL )
	{
		return XT_REACT_INIT_FAIL;
	}

	event_base_dispatch( m_pReactor );

	return nRet;
}

int CXtReactor::Stop( struct timeval *pDelay )
{
	int nRet = FUN_RET_OK;

	nRet = event_base_loopexit( m_pReactor, pDelay );

	return nRet;
}

int CXtReactor::SetupForceSelect(void)
{
	if ( m_pReactorCfg == NULL )
	{
		new_event_base_cfg;
		if ( m_pReactorCfg == NULL )
		{
			return XT_REACT_ERR_MEM_FAIL;
		}
	}

	return FUN_RET_OK;
}

void CXtReactor::SetupMemFun( void *(*malloc_fn)(size_t sz), void *(*realloc_fn)(void *ptr, size_t sz), void (*free_fn)(void *ptr) )
{
	event_set_mem_functions( malloc_fn, realloc_fn, free_fn );
}

const TCHAR* CXtReactor::QueryBackend(void)
{
	return NULL;
}

int CXtReactor::SetCpuHint( int nCpus )
{
	if ( m_pReactorCfg == NULL )
	{
		new_event_base_cfg;
		if ( m_pReactorCfg == NULL )
		{
			return XT_REACT_ERR_MEM_FAIL;
		}
	}
	return event_config_set_num_cpus_hint( m_pReactorCfg, nCpus );;
}

void CXtReactor::Reset(void)
{
	m_pReactor = NULL;
	m_pReactorCfg = NULL;
}

#if defined(__WINDOWS__)
int CXtReactor::IocpInit(void)
{
	if ( m_pReactorCfg == NULL )
	{
		new_event_base_cfg;
		if ( m_pReactorCfg == NULL )
		{
			return XT_REACT_ERR_MEM_FAIL;
		}
	}

	evthread_use_windows_threads();
	event_config_set_num_cpus_hint( m_pReactorCfg, 1 );
	event_config_set_flag( m_pReactorCfg, EVENT_BASE_FLAG_STARTUP_IOCP );
	m_pReactor = event_base_new_with_config(m_pReactorCfg);

	event_config_free(m_pReactorCfg);
	m_pReactorCfg = NULL;

	return FUN_RET_OK;
}
#endif
