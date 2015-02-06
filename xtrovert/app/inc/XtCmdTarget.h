/************************************************************************************
* XtCmdTarget.h : header file
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
#ifndef __XTCMDTARGET_H__
#define __XTCMDTARGET_H__

#include "xtrovert.h"
#include "XtCmdLineInfo.h"

#if defined(__WINDOWS__)
#pragma warning (disable: 4200) /* Ignore:  warning C4200: nonstandard extension used : zero-sized array in struct/union */
#endif

class XT_NOVTABLE CXtCmdTarget;
typedef void (XT_MSG_CALL CXtCmdTarget::*xt_pfn_msghandler_t)(void);
#if defined(__WINDOWS__)
#pragma pack(push, XT_PACK_BYTES) /* To eliminate: (warning C4121: alignment of a member was sensitive to packing) */
#endif
typedef struct _tag_xt_cmdhandlerinfo_t
{
	CXtCmdTarget		*xt_cmdtarget;
	void (XT_MSG_CALL CXtCmdTarget::*pfn_member)(void);
}xt_cmdhandlerinfo_t;
#if defined(__WINDOWS__)
#pragma pack(pop)
#endif
/**********************************************************************
*		CLASS CXtCmdTarget
*			With a given command, call command parser, and process it.
*			Behaves like a real shell.
**********************************************************************/
class XT_NOVTABLE CXtCmdTarget : public CXtObject
{
public:

	XT_DECLARE_DYNAMIC(CXtCmdTarget)

	CXtCmdTarget(void);
	virtual ~CXtCmdTarget(void);

	// Overridables
	// route and dispatch standard command message types
	//   (more sophisticated than OnCommand)
	virtual BOOL				OnCmdMsg( UINT nID, int nCode, void *pExtra, xt_cmdhandlerinfo_t *pCmdHandlerInfo );

#if defined(_DEBUG)
	virtual void				AssertValid(void) const;
	/*virtual void Dump( CDumpContext& dc ) const;*/
#endif

protected:

	CXtCmdLineInfo				*m_pCmdLineParser;

private:

	XT_DECLARE_MSGMAP()
};

#endif /*__XTCMDTARGET_H__*/
