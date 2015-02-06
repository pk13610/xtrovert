/************************************************************************************
*	NetReactor.cpp: Implementation File
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

#include "NetReactor.h"

/************************************************************************/
/*              CNetEvtHandler                                          */
/************************************************************************/
CNetEvtHandler::CNetEvtHandler(VOID)
{

}

CNetEvtHandler::~CNetEvtHandler(VOID)
{

}

INT CNetEvtHandler::HandleEvents( INT nEvents )
{
	assert( nEvents );
// 	if ( (nEvents & (EPOLLERR | EPOLLHUP)) != 0 )
// 	{
// 		OnClose();
// 	}
// 
// 	if ( (nEvents & EPOLLIN) != 0 )
// 	{
// 		OnRead();
// 	}
// 
// 	if ( (nEvents & EPOLLOUT) != 0 )
// 	{
// 		OnWrite();
// 	}
	return FUN_RET_OK;
}

/************************************************************************/
/*              CNetReactor                                             */
/************************************************************************/
CNetReactor::CNetReactor(VOID)
{
	Reset();
}

CNetReactor::~CNetReactor(VOID)
{
	Release();
}

// Init event_base object
INT CNetReactor::Init(VOID)
{
#if defined(__WINDOWS__)
		struct event_config *cfg = event_config_new();
		if ( cfg == NULL )
		{
			return -3;
		}
		if ( event_config_set_flag(cfg, EVENT_BASE_FLAG_STARTUP_IOCP) < 0 ) // Start IOCP on Ms-Windows
		{
			event_config_free( cfg );
			return -4;
		}
		m_pBase = event_base_new_with_config( cfg );
		event_config_free( cfg );
#else // Under other OS, it's simple. Libevent would choose a back-end
	m_pBase = event_base_new();
#endif /*(__WINDOWS__)*/

	if ( m_pBase == NULL )
	{
		return -3;
	}

	return FUN_RET_OK;
}

// VOID CNetReactor::Release(VOID)
// {
// }

INT CNetReactor::RegisterHandler( CNetEvtHandler *pHandler )
{
	assert(pHandler);
	event *pEvent = NULL;
	event_add( pEvent, NULL );

	return FUN_RET_OK;
}

INT CNetReactor::RemoveHandler( CNetEvtHandler *pHandler )
{
	assert(pHandler);
	event *pEvent = NULL;
	event_del( pEvent );

	return FUN_RET_OK;
}

INT CNetReactor::PollHandler( timeval *ptv )
{
	assert(ptv);
	event_base_dispatch( m_pBase );
	return FUN_RET_OK;
}

INT CNetReactor::HandleEvents( size_t nCount/*=0*/ )
{
	assert(nCount);
	return FUN_RET_OK;
}

// VOID CNetReactor::Reset(VOID)
// {
// }
