/************************************************************************************
* XtAppModule.h : header file
*
* Definition header
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
#ifndef __XT_APP_MODULE_H__
#define __XT_APP_MODULE_H__

#include "xtrovert.h"

// #ifdef __cplusplus
// extern "C" {
// #endif

class CXtApp;
typedef struct _tag_xt_app_module_t
{
	CXtApp		*pApp;
}xt_app_module_t;

extern int xt_appmodule_init(void);

extern xt_app_module_t xt_app_module;

// #ifdef __cplusplus
// }
// #endif

#endif /*__XT_APP_MODULE_H__*/
