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

#include "xt_net_api.h"
#include "net/inc/XtEventHandler.h"

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

int CXtEventHandler::Init( void *arg )
{
	return FUN_RET_OK;
}

void CXtEventHandler::Release(void)
{
	Reset();
}

void CXtEventHandler::OnHandleEvents( xt_event_fd_t fd, short events, void *arg )
{
	CXtEventHandler *handler = CXtEventHandler*(arg);
	handler->HandleEvents( events );
}

void CXtEventHandler::Reset(void)
{
	m_pReactor = NULL;
	m_pXtEvt = NULL;

	m_fd = INVALID_SOCKET;
	m_events = 0;
}

void CXtEventHandler::HandleEvents( short events )
{
// 	xt_event_fd_t fd = INVALID_SOCKET;
// 	fd = event_get_fd( m_pXtEvt->_event );
	return;
}
