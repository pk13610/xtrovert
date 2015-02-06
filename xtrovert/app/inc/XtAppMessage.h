/************************************************************************************
* XtAppMessage.h : header file
*
* header
*
* AUTHOR	:	Sean Feng <SeanFeng2006@hotmail.com>
* DATE		:	Nov. 7, 2009
* Copyright (c) 2009-?. All Rights Reserved.
*
* This code may be used in compiled form in any way you desire. This
* file may be redistributed unmodified by any means PROVIDING it is 
* not sold for profit without the authors written consent, and 
* providing that this notice and the authors name and all copyright 
* notices remains intact. 
*
* An email letting me know how you are using it would be nice as well. 
*
* This file is provided "as is" with no expressed or implied warranty.
* The author accepts no liability for any damage/loss of business that
* this product may cause.
*
************************************************************************************/
#ifndef __XTAPPMESSAGE_H__
#define __XTAPPMESSAGE_H__

#include "xt_define.h"

#ifdef __cplusplus
extern "C" {
#endif


/**********************************************************************
*		Command notifications for CXtCmdTarget notifications
***********************************************************************/
#define XT_CN_COMMAND              0               // void ()
#define XT_CN_UPDATE_COMMAND_UI    ((UINT)(-1))    // void (CCmdUI*)
#define XT_CN_EVENT                ((UINT)(-2))    // OLE event
#define XT_CN_OLECOMMAND           ((UINT)(-3))    // OLE document command
#define XT_CN_OLE_UNREGISTER       ((UINT)(-4))    // OLE unregister
// > 0 are control notifications
// < 0 are for Xtrovert's use

/*
*	Message structure
*/
typedef struct _tag_xt_appmsg_t
{
	HANDLE			hndl;
	UINT			msg;
	WPARAM			wParam;
	LPARAM			lParam;
	DWORD			time;
	POINT			pt;
}xt_appmsg_t;

#ifdef __cplusplus
} /* ... extern "C" */
#endif

#endif /*__XTAPPMESSAGE_H__*/
