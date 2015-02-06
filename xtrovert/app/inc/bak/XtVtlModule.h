/************************************************************************************
* XtVirtCom.h : header file
*
* CXtVirtCom Definition header, Virtual communication component for IPC use.
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
#ifndef __XT_VTL_COM_H__
#define __XT_VTL_COM_H__

#include "xtrovert.h"

// class CXtVtAsyncIo
// {
// public:
// 	CXtVtAsyncIo(void);
// 	virtual ~CXtVtAsyncIo(void);
// 
// 	virtual int			Open( const TCHAR *pAddress, int nFlag=0 );
// 
// 	virtual int			Write( const void *pData, int nLen );
// 	virtual int			Read( void *pData, int *pLen );
// 
// 	virtual int			GetLastError(void);
// protected:
// private:
// };

//////////////////////////////////////////////////////////////////////////
// CLASS CXtVtlCom
//////////////////////////////////////////////////////////////////////////
class CXtVtlCom
{
public:

	CXtVtlCom(void);
	virtual ~CXtVtlCom(void);

	virtual int				Open( void *pArg );
	virtual int				Close(void);
	
	virtual int				Read( const void *pRequest, char *pBuf, xt_size_t nBufLen );
	virtual int				Write( const void *pCmd, char *pBuf, xt_size_t nLen );

protected:

	virtual void			Roger( const void *pMsg, int nMsgLen );

private:
};

#endif /*__XT_VTL_COM_H__*/
