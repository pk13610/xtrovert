/************************************************************************************
* XtThread.h : header file
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
#ifndef __XT_THREAD_H__
#define __XT_THREAD_H__

#if defined(__WINDOWS__)
#pragma warning (disable: 4200) /* Ignore:  warning C4200: nonstandard extension used : zero-sized array in struct/union */
#endif

#include "app/inc/XtCmdTarget.h" /* Base Class */
#include "app/inc/xt_thread_api.h"

typedef void* (XT_CDECL *xt_thread_proc_t)(void*);

class CXtThread : public CXtCmdTarget
{
public:
	CXtThread(void);
	CXtThread( xt_thread_proc_t pfnThreadProc, void *pParam );
	virtual ~CXtThread(void);

	int					Create( DWORD dwCreateFlags=0, UINT nStackSize=0, const xt_threadattr_t *pAttrs=NULL );

	// Application logics initialization/destruction.
	virtual int			InitInstance(void);
	// thread termination
	virtual int			ExitInstance(void); // default will 'delete this'

	// running and idle processing
	virtual BOOL		PreTranslateMessage( xt_appmsg_t *pMsg );
	virtual BOOL		PumpMessage(void);     // low level message pump
	virtual BOOL		OnIdle( LONG lCount ); // return TRUE if more idle processing
	virtual BOOL		IsIdleMessage( xt_appmsg_t *pMsg );  // checks for special messages

	virtual int			Run(void);

	int					GetThreadPriority(void);
	BOOL				SetThreadPriority(int nPriority);

	// Operations
	DWORD				SuspendThread(void);
	DWORD				ResumeThread(void);
	BOOL				PostThreadMessage(UINT message, WPARAM wParam, LPARAM lParam);


#if defined(_DEBUG)
	virtual void AssertValid(void) const;
	/*virtual void Dump(CDumpContext& dc) const;*/
#endif
	
	xt_thread_proc_t	m_pfnThreadProc;
	void				*m_pThreadParam;

protected:

	virtual void		Reset(void);

	xt_thread_t			m_ThreadInfo;

	xafx_msg_que_t		*m_pMsgQue;

private:

};

#endif /*__XT_THREAD_H__*/
