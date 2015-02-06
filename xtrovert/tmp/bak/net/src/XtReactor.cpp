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

#include "net/inc/XtReactor.h"
#include <event2/thread.h>

#if defined(__WINDOWS__)
/* warning C4100: 'XXX' : unreferenced formal parameter */
#pragma warning (disable: 4100)
#endif

//class CXtReactor
CXtReactor::CXtReactor(VOID)
{
	Reset();
}

CXtReactor::~CXtReactor(VOID)
{
	Release();
}

int CXtReactor::Init( xt_size_t nHandlerCount )
{
	/* nHandlerSize: Initial handler count */
	int nRet = FUN_RET_OK;
	if ( m_pEvtBase ) /* != NULL */
	{
		return FUN_RET_ERR_ILLEGAL_CALL;
	}

	/* Initialize event_base struct */
// 	if ( force_select )
// 	{
// 		m_pEvtBase = event_base_new();
// 		if ( m_pEvtBase == NULL )
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
		m_pEvtBase = event_base_new();
		if ( m_pEvtBase == NULL )
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

	/* Allocate memory for m_EventPool */
	if ( nHandlerCount == _INVALID_SIZE_T_VALUE_ )
	{
		nHandlerCount = _XT_REACT_HANDLER_SIZE_DEF_;
	}
	nRet = m_EventPool.Init( nHandlerCount );
	if ( nRet < 0 )
	{
		return -100;
	}
	m_nEvPoolGrow = nHandlerCount;

/*lbl_end:*/

	return nRet;
}

int CXtReactor::Reinit(void)
{
	/* Call for after fork */
	int nRet = FUN_RET_OK;
	
	if ( m_pEvtBase == NULL )
	{
		return FUN_RET_ERR_ILLEGAL_CALL;
	}

	nRet = event_reinit( m_pEvtBase );
	if ( nRet < 0 )
	{
		return XT_REACT_INIT_FAIL;
	}
	return FUN_RET_OK;	
}

void CXtReactor::Release(void)
{
	xt_event_t *xt_event = NULL;
	xt_event = m_EventPool.GetFirstObj();
	while ( xt_event ) /*  != NULL */
	{
		RemoveHandler( xt_event->evt_handler );
		xt_event->evt_handler->Release();

		xt_event = m_EventPool.GetNextObj();
	}

	/* Release memory */
	m_EventPool.Release();

	if ( m_pEvtBase ) /* != NULL */
	{
		event_base_free( m_pEvtBase );
	}

	if ( m_pEvtBaseCfg )
	{
		event_config_free( m_pEvtBaseCfg );
	}

	Reset();
}

int CXtReactor::RegisterHandler( CXtEventHandler *pEvtHandler, short nEvents )
{
	int nRet = FUN_RET_OK;

	if ( pEvtHandler == NULL )
	{
		return FUN_RET_ERR_ILLEGAL_ARGS;
	}

	xt_event_t *xt_event = NULL;
	xt_obj_id_t handler_id = _INVALID_SIZE_T_VALUE_;
	xt_event = EvtPoolAlloc( &handler_id );
	if ( xt_event == NULL )
	{
		return XT_REACT_ERR_MEM_FAIL;
	}

	/* libevent new event */
	xt_event->_event = event_new( m_pEvtBase, fd, events, CXtEventHandler::OnHandleEvents, pEvtHandler );
	if ( xt_event->_event == NULL )
	{
		nRet = XT_REACT_ERR_MEM_FAIL;
		goto lbl_err;
	}

	xt_event->evt_handler = pEvtHandler;
	xt_event->evt_handle = handler_id;

	nRet = event_add( _event,  );

	pEvtHandler->BindReactor( this );

	return FUN_RET_OK;

lbl_err:

	if ( xt_event )
	{
		m_EventPool.FreeObj( xt_event );
	}

	return nRet;
}

int CXtReactor::RemoveHandler( CXtEventHandler *pEvtHandler )
{
	int nRet = FUN_RET_OK;

	pEvtHandler->BindReactor( NULL );

	return nRet;
}

int CXtReactor::Dispatch(void)
{
	int nRet = FUN_RET_OK;

	if ( m_pEvtBase == NULL )
	{
		return XT_REACT_INIT_FAIL;
	}

	event_base_dispatch( m_pEvtBase );

	return nRet;
}

int CXtReactor::Stop(void)
{
	int nRet = FUN_RET_OK;

	if ( m_pEvtBase == NULL )
	{
		return XT_REACT_INIT_FAIL;
	}

	event_base_loopexit( m_pEvtBase, NULL );

	return nRet;
}

int CXtReactor::SetupForceSelect(void)
{
	if ( m_pEvtBaseCfg == NULL )
	{
		new_event_base_cfg;
		if ( m_pEvtBaseCfg == NULL )
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
	if ( m_pEvtBaseCfg == NULL )
	{
		new_event_base_cfg;
		if ( m_pEvtBaseCfg == NULL )
		{
			return XT_REACT_ERR_MEM_FAIL;
		}
	}
	return event_config_set_num_cpus_hint( m_pEvtBaseCfg, nCpus );;
}

void CXtReactor::Reset(void)
{
	m_pEvtBase = NULL;
	m_pEvtBaseCfg = NULL;

	m_nEvPoolGrow = 0;
	m_nEvtHandleStart = 0;
}

#if defined(__WINDOWS__)
int CXtReactor::IocpInit(void)
{
	if ( m_pEvtBaseCfg == NULL )
	{
		new_event_base_cfg;
		if ( m_pEvtBaseCfg == NULL )
		{
			return XT_REACT_ERR_MEM_FAIL;
		}
	}

	evthread_use_windows_threads();
	event_config_set_flag( m_pEvtBaseCfg, EVENT_BASE_FLAG_STARTUP_IOCP );
	m_pEvtBase = event_base_new_with_config(m_pEvtBaseCfg);

	event_config_free(m_pEvtBaseCfg);

	return FUN_RET_OK;
}
#endif

xt_event_t*	CXtReactor::EvtPoolAlloc( xt_obj_id_t *pObjId )
{
	int nRet = FUN_RET_OK;
	xt_event_t *xt_event = NULL;

	xt_event = m_EventPool.AllocObj( pObjId );
	if ( xt_event )
	{
		return xt_event;
	}
	/* Failed to allocate object */

	nRet = m_EventPool.InflatePool( m_nEvPoolGrow );
	if ( nRet < 0 ) /* Failed to inflate */
	{
		return NULL;
	}

	xt_event = m_EventPool.AllocObj( pObjId );
	if ( xt_event == NULL )
	{
		return NULL;
	}

	return xt_event;
}
