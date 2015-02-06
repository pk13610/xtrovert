/************************************************************************************
* XtVtlStorage.h : header file
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
#ifndef __XT_VTL_STORAGE_H__
#define __XT_VTL_STORAGE_H__

#include "XtVtlCom.h"

//////////////////////////////////////////////////////////////////////////
// CLASS CXtVtlStorage
//////////////////////////////////////////////////////////////////////////
class CXtVtlStorage : public CXtVtlCom
{
public:

	CXtVtlStorage(void);
	virtual ~CXtVtlStorage(void);

	virtual int				Init( void *pArg );
	virtual int				Release(void);

	/* For use of process inner calling */
	virtual int				Get( const void *pDescriptor, void **pData, xt_size_t *pSize );
	
protected:
	virtual int				Load();
	virtual int				Dump();
	virtual int				DumpAll(void);

private:
};

#endif /*__XT_VTL_STORAGE_H__*/
