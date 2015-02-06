/************************************************************************************
* XtStateMachine.cpp: Implementation File
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

#include "app/inc/XtStateMachine.h"

#if defined(__WINDOWS__)
/* warning C4100: 'XXX' : unreferenced formal parameter */
#pragma warning (disable: 4127)
#pragma warning (disable: 4100)
#endif

/* Input processing. Return state. */
int CXtStateMachine::HandleEvent( int nEvent, void *pMsg, void **pTarget )
{
	m_pActiveState->HandleEvent( nEvent, pMsg );
}

int CXtStateMachine::SwitchState( CXtStateModule *pState )
{
	if ( pState->TestAdmission() ) /* == TRUE */
	{
	}
	
}

/**********************************************************************
*		CLASS CXtCmdShell END
**********************************************************************/
