/************************************************************************************
* XtCmdTarget.cpp: Implementation File
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

#include "app/inc/XtCmdTarget.h"

#if defined(__WINDOWS__)
/* warning C4100: 'XXX' : unreferenced formal parameter */
#pragma warning (disable: 4127)
#pragma warning (disable: 4100)
#endif

BOOL CXtCmdTarget::OnCmdMsg( UINT nID, int nCode, void *pExtra, xt_cmdhandlerinfo_t *pCmdHandlerInfo )
{
	return FALSE;
}

/**********************************************************************
*		CLASS CXtCmdShell END
**********************************************************************/
