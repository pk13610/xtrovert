/************************************************************************************
* _test_app.h : header file
*
* 
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
* 1 producer thread and 1 consumer thread.
* Create and release in consumer thread.
************************************************************************************/
#ifndef ___TEST_APP_H__
#define ___TEST_APP_H__

#include "app/inc/XtApp.h"

class CXtTestApp : public CXtApp
{
public:
	CXtTestApp(void);
	virtual ~CXtTestApp(void);

protected:

	/***************************************************************** 
	*	InitApplication() is implemented with the following methods.
	*	ExitApplication() is implemented with the following methods.
	******************************************************************/
	/* Do something extra in derived-class. */
  	virtual int			OnInitApp(void);
	virtual int			OnExitApp(void);
	/***************************************************************** 
	*	InitApplication()/ExitApplication() implementation END
	******************************************************************/

	/***************************************************************** 
	*	InitInstance() is implemented with the following methods.
	*	ExitInstance() is implemented with the following methods.
	******************************************************************/
  	virtual int			OnInitInstance(void);
  	virtual int			OnExitInstance(void);
	/***************************************************************** 
	*	InitInstance()/ExitInstance() implementation END
	******************************************************************/

	/***************************************************************** 
	*	Run() is implemented with the following methods.
	******************************************************************/
	virtual int 		AppProc(void);
	/***************************************************************** 
	*	Run() implementation END
	******************************************************************/

	int 				OnAppCmdLineStart( const TCHAR *cszParam );

	int 				OnAppCmdLineServer( const TCHAR *cszParam );
	int 				OnAppCmdLineClient( const TCHAR *cszParam );
	

private:

};

#endif /*___TEST_APP_H__*/
