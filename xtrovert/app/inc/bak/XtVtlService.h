/************************************************************************************
* XtVtlService.h : header file
*
* CXtVtlService Definition header
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
#ifndef __XT_VTL_SERVICE_H__
#define __XT_VTL_SERVICE_H__

#include "xtrovert.h"

typedef struct _tag_xt_vtl_svc_t
{

}xt_vtl_svc_t;

//////////////////////////////////////////////////////////////////////////
// CLASS CXtVtlService
//////////////////////////////////////////////////////////////////////////
class CXtVtlService
{
public:

	CXtVtlService(void);
	virtual ~CXtVtlService(void);

	virtual int					InitialInstance();

	virtual int					Run(void);

	virtual int					ExitInstance(void);

protected:

	virtual int					OnCommand( xt_size_t nCmdID, void *pParam );

private:
};

#endif /*__XT_VTL_SERVICE_H__*/
