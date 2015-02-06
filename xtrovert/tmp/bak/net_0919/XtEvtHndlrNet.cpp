/************************************************************************************
*	XtEvtHndlrSig.cpp: Implementation File
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

#include "net/inc/XtEvtHndlrNet.h"

#if defined(__WINDOWS__)
/* warning C4100: 'XXX' : unreferenced formal parameter */
#pragma warning (disable: 4100)
#endif

CXtNetEventHandler::CXtNetEventHandler(void)
{
	Reset();
}

CXtNetEventHandler::~CXtNetEventHandler(void)
{
	Release();
}

int CXtNetEventHandler::AttachInit( void *pArg )
{
	#define cfg		((xt_acceptor_cfg_t*)pArg)
	#define _event_fd_			*((xt_event_fd_t*)pArg)
	#define conn_pool			cfg->conn_pool


	m_EventDsc.fd = _event_fd_;

	return FUN_RET_OK;
}

virtual void CXtNetEventHandler::Release(void)
{
	if ( is_valid_fd )
	{
		xt_closesock( GetEventfd() );
		m_EventDsc.fd_handle = _INVALID_EVENT_FD_;
	}

	CXtEventHandler::Release(void);
}

void CXtNetEventHandler::Reset(void)
{
	m_pSAddr = NULL;
	CXtEventHandler::Reset();
}
