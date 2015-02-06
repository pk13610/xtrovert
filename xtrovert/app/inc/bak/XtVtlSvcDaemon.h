/************************************************************************************
* XtVtlSvcDaemon.h : header file
*
* CXtVtlSvcDaemon Definition header
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
#ifndef __XT_VTL_SVC_DAEMON_H__
#define __XT_VTL_SVC_DAEMON_H__

#include "xtrovert.h"

//////////////////////////////////////////////////////////////////////////
// CLASS CXtVtlSvcDaemon
//////////////////////////////////////////////////////////////////////////
class CXtVtlSvcDaemon : public CXtThread
{
public:

	CXtVtlSvcDaemon(void);
	virtual ~CXtVtlSvcDaemon(void);

protected:

	virtual int				LoopService(void)
	{
		for ( xt_size_t i=0; i<m_nSvcCount; ++i )
		{
			m_pSvcList[i].Run();
		}
		
		return FUN_RET_OK;
	}

	xt_size_t				m_nSvcCount;
	CXtVtlService			*m_pSvcList;

private:
};

#endif /*__XT_VTL_SVC_DAEMON_H__*/
