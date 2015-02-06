/************************************************************************************
* XtLogger.h : header file
*
* XtLogger Definition header
*
* AUTHOR	:	Sean Feng <SeanFeng2006@hotmail.com>
* DATE		:	Aug. 7, 2012
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
#ifndef __XTLOGGER_H__
#define __XTLOGGER_H__

#include "xtrovert.h"

//////////////////////////////////////////////////////////////////////////
// CLASS CXtLogger
//////////////////////////////////////////////////////////////////////////
class CXtLogger
{
public:

	CXtLogger(void);
	virtual ~CXtLogger(void);


protected:

private:
};

#define XT_LOGGER_INIT()
#define XT_LOG					LOG

#endif /*__XTLOGGER_H__*/
