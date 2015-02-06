/************************************************************************************
*	XtEventHandler.cpp: Implementation File
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

#include "xt_net_api.h"
#include "XtEventHandler.h"
/*#include "net/reactor/XtReactor.h"*/

#if defined(__WINDOWS__)
/* warning C4100: 'XXX' : unreferenced formal parameter */
#pragma warning (disable: 4100)
#endif

CXtEventHandler::CXtEventHandler(void)
{
	Reset();
}

CXtEventHandler::~CXtEventHandler(void)
{
	Release();
}

int CXtEventHandler::Init( void *pCfg )
{
	return FUN_RET_OK;
}

void CXtEventHandler::Release(void)
{
	if ( m_bFreeWrapper )
	{
		EventFree();
	}

	Reset();
}

int CXtEventHandler::Register( CXtReactor *pReactor )
{
	int nRet = FUN_RET_OK;

	if ( pReactor == NULL )
	{
		return FUN_RET_ERR_ILLEGAL_ARGS;
	}

	if ( pReactor->IsValid() == FALSE )
	{
		return FUN_RET_ERR_ILLEGAL_CALL;
	}

	xt_event_fd_t fd = 0;
	xt_event_t nEvents = 0;
	OnRegisterGetFD( &fd );
	if ( fd == 0 )
	{
		return FUN_RET_ERR_ILLEGAL_CALL;
	}

	OnRegisterGetEvents( &nEvents );
	
	nRet = BindReactor( pReactor, fd, nEvents );
	/* Events or FD has been changed, so we should bind reactor. */
// 	if ( xt_conv2_reactor_event(nEvents)!=xt_evtwrapper_get_events(m_pEventWrapper) || fd!=xt_evtwrapper_get_fd(m_pEventWrapper) )
// 	{
// 		
// 	}

	nRet = MakeActive( TRUE );

	return nRet;
}

int CXtEventHandler::Resign(void)
{
	int nRet = FUN_RET_OK;

	nRet = MakeActive( FALSE );

	return nRet;
}

BOOL CXtEventHandler::IsActive(void)
{
	if ( m_pEventWrapper == NULL )
	{
		return FALSE;
	}

	return (xt_evtwrapper_get_base(m_pEventWrapper)!=NULL);
}

int CXtEventHandler::HandleEvents( xt_event_t nEvents, void *pArg )
{
	_tprintf( _T("CXtEventHandler::HandleEvents(%d):%d\n"), GetEventfd(), nEvents );

	return FUN_RET_OK;
}

xt_event_fd_t CXtEventHandler::GetEventfd(void)
{
	if ( m_pEventWrapper == NULL )
	{
		return 0;
	}

	return (xt_evtwrapper_get_fd(m_pEventWrapper)); 
}

xt_event_t CXtEventHandler::GetEvents(void)
{
	if ( m_pEventWrapper == NULL )
	{
		return 0;
	}

	return (xt_evtwrapper_get_events(m_pEventWrapper));
}

BOOL CXtEventHandler::IsValid(void)
{
	if ( m_pEventWrapper == NULL )
	{
		return FALSE;
	}

	return (xt_evtwrapper_get_base(m_pEventWrapper)!=NULL);
}

void CXtEventHandler::OnHandleEvents( xt_event_fd_t nEventfd, xt_reactor_evt_t nEvents, void *pArg )
{
#define pThis ((CXtEventHandler*)pArg)
	pThis->HandleEvents( nEvents, NULL );
#undef pThis
}

void CXtEventHandler::Reset(void)
{
	m_pEventWrapper = NULL;

	m_bFreeWrapper = FALSE;

#if defined(_DEBUG)
	memset( m_szLabel, 0, _MAX_STR_LEN_8_*sizeof(TCHAR) );
#endif
}

int CXtEventHandler::BindReactor( CXtReactor *pReactor, xt_event_fd_t fd, xt_event_t nEvents )
{
	int nRet = FUN_RET_OK;

	if ( m_pEventWrapper )
	{
		nRet = EventAssign( pReactor, fd, nEvents );		
	}
	else
	{
		nRet = EventNew( pReactor, fd, nEvents );
	}

	return nRet;
}

int CXtEventHandler::MakeActive( BOOL bActive )
{
	int nRet = FUN_RET_OK;

	if ( IsValid() == FALSE )
	{
		return FUN_RET_ERR_ILLEGAL_CALL;
	}

	if ( bActive )
	{
		struct timeval timeout = { 0, 0 };
		OnRegisterGetTimeout( &timeout );
		if ( timeout.tv_sec==0 && timeout.tv_usec==0 )
		{
			nRet = event_add( m_pEventWrapper, NULL );
		}
		else
		{
			nRet = event_add( m_pEventWrapper, &timeout );
		}
	}
	else
	{
		nRet = event_del( m_pEventWrapper );
	}
	return nRet;
}

int CXtEventHandler::EventAssign( CXtReactor *pReactor, xt_event_fd_t fd, xt_event_t nEvents )
{
	return event_assign( m_pEventWrapper, pReactor->GetContext(), fd, xt_conv2_reactor_event(nEvents), OnHandleEvents, this );
}

int CXtEventHandler::EventNew( CXtReactor *pReactor, xt_event_fd_t fd, xt_event_t nEvents )
{
	m_pEventWrapper = event_new( pReactor->GetContext(), fd, xt_conv2_reactor_event(nEvents), OnHandleEvents, this );
	if ( m_pEventWrapper == NULL )
	{
		return -5;
	}

	m_bFreeWrapper = TRUE;

	return FUN_RET_OK;
}

void CXtEventHandler::EventFree(void)
{
	event_free( m_pEventWrapper );
	m_pEventWrapper = NULL;
	m_bFreeWrapper = FALSE;
}

void CXtEventHandler::OnRegisterGetFD( xt_event_fd_t *pWhat )
{
	return;
}

void CXtEventHandler::OnRegisterGetEvents( xt_event_t *pWhat )
{
	return;
}

void CXtEventHandler::OnRegisterGetTimeout( struct timeval *pWhat )
{
	return;
}

#if defined(_DEBUG)
void CXtEventHandler::SetLabel( const TCHAR *szLabel )
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
