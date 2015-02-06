/************************************************************************************
* xafx_app.h : header file
*
* Definition header, An Application framework for common use
*			Xtrovert Application Frameworks (xafx, the last 'x' means nothing).
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
#ifndef __X_AFX_APP_H__
#define __X_AFX_APP_H__

#include "app/xafx/xafx.h"
#include "app/inc/XtApp.h" /* class CXtApp */

extern CXtApp* XT_STDECL xafx_get_app(void);

extern int XT_STDECL xafx_app_init( int argc, TCHAR *argv[] );
extern int XT_STDECL xafx_app_exit(void);

extern int XT_STDECL xafx_init_thread(void);
extern int XT_STDECL xafx_term_thread(void);

#endif /*__X_AFX_APP_H__*/
