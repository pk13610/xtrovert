/************************************************************************************
*	XtNetEvtHandler.cpp: Implementation File
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
#include "xt_mem_api.h"
#include "XtNetEvtHandler.h"

#if defined(__WINDOWS__)
/* warning C4100: 'XXX' : unreferenced formal parameter */
#pragma warning (disable: 4100)
#endif

/*****************************************
 *			Class CXtNetEvtHandler
 *****************************************/
CXtNetEvtHandler::CXtNetEvtHandler(void)
{
	Reset();
}

CXtNetEvtHandler::~CXtNetEvtHandler(void)
{
	Release();
}

int CXtNetEvtHandler::Init( void *pCfg )
{
	return FUN_RET_OK;
}

void CXtNetEvtHandler::Release(void)
{
	Reset();
}

int CXtNetEvtHandler::HandleEvents( xt_event_t nEvents, void *pArg )
{
	_tprintf( _T("CXtNetEvtHandler::HandleEvents(%d):%d\n"), GetEventfd(), nEvents );

	return FUN_RET_OK;
}

void CXtNetEvtHandler::Reset(void)
{
	m_pSockAddr = NULL;
}

void CXtNetEvtHandler::OnRegisterGetFD( xt_event_fd_t *pWhat )
{
	return;
}

void CXtNetEvtHandler::OnRegisterGetEvents( xt_event_t *pWhat )
{
	*pWhat = XT_EV_WRITE|XT_EV_READ|XT_EV_PERSIST;
	return;
}

void CXtNetEvtHandler::OnRegisterGetTimeout( struct timeval *pWhat )
{
	return;
}

xt_event_fd_t CXtNetEvtHandler::CreateSocket( int nSockType )
{
	return xt_socket( m_pSockAddr->saddr->sa_family, nSockType, 0 );
}

int CXtNetEvtHandler::BindAddress(void)
{
	return FUN_RET_OK;
}

/*****************************************
 *			Class CXtNetEvtHandler END
 *****************************************/
