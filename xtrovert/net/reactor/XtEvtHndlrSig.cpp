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

#include "net/reactor/XtEvtHndlrSig.h"

#if defined(__WINDOWS__)
/* warning C4100: 'XXX' : unreferenced formal parameter */
#pragma warning (disable: 4100)
#endif

CXtEvtHndlrSig::CXtEvtHndlrSig(void)
{
}

CXtEvtHndlrSig::~CXtEvtHndlrSig(void)
{
}

int CXtEvtHndlrSig::Init( void *pCfg )
{
	return FUN_RET_OK;
}

int CXtEvtHndlrSig::HandleEvents( xt_event_t nEvents, void *pArg )
{
	_tprintf( _T("Caught an interrupt signal: %d\n"),  GetEventfd() );
	return FUN_RET_OK;
}

void CXtEvtHndlrSig::OnRegisterGetFD( xt_event_fd_t *pWhat )
{
	*pWhat = SIGINT;
	return;
}

void CXtEvtHndlrSig::OnRegisterGetEvents( xt_event_t *pWhat )
{
	*pWhat = XT_EV_SIGNAL|XT_EV_PERSIST;
	return;
}
