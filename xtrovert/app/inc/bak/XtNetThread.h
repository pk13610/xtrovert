/************************************************************************************
* XtNetThread.h : header file
*
* CXtApp Definition header, An Application framework for common use.
*
* AUTHOR	:	Sean Feng <SeanFeng2006@hotmail.com>
* DATE		:	Aug. 7, 2012
* Copyright (c) 2009-?. All Rights Reserved.
*
* This code may be used in compiled form in any way you desire. This
* file may be redistributed unmodified by any means PROVIDING it is 
* not sold for profit without the authors written consent, and 
* providing that this notice and the authors name and all copyright 
* notices remains int act. 
*
* An email letting me know how you are using it would be nice as well. 
*
* This file is provided "as is" with no expressed or implied warranty.
* The author accepts no liability for any damage/loss of business that
* this product may cause.
*
************************************************************************************/
#ifndef __XT_NET_THREAD_H__
#define __XT_NET_THREAD_H__

#include "xtrovert.h"
#include "net/reactor/XtReactor.h"

class CXtReactorRunner : public CXtThread
{
public:
	CXtReactorRunner(void);
	virtual ~CXtReactorRunner(void);

protected:

	/* Setup threads attributes. */
	virtual int					Setup(void);

	virtual int					Execute(void)
	{
#define _NET_SVR_ADDR_STR_					_T("127.0.0.1:9995")
		//#define _NET_SVR_ADDR_STR_					_T("0.0.0.0:9995")
#define _NET_CLIENT_ADDR_STR_				_T("0.0.0.0:9995")

		if ( *((TCHAR*)m_pParam) == _T('s') )
		{
			StartNetServer( NET_SKT_TYPE_TCP, _NET_SVR_ADDR_STR_ );
		}
		else
		{
			StartNetClient( NET_SKT_TYPE_TCP, _NET_SVR_ADDR_STR_ );
		}

		m_pReactor->Dispatch();

		m_bStop = TRUE;

		return FUN_RET_OK;
	}

	CXtReactor					*m_pReactor;

private:
};

#endif /*__XT_NET_THREAD_H__*/
