/************************************************************************************
*	xafx.cpp: Implementation File
*
* DESCRIPTION  :   
*
* AUTHOR	    :	Sean Feng <SeanFeng2006@hotmail.com>
* DATE		    :	Sep. 24, 2010
*
* Copyright (c) 2010-?. All Rights Reserved.
*
* REMARKS:
*		
*      
************************************************************************************/
#include "app/xafx/xafx.h"

#if defined(__WINDOWS__)
/* warning C4100: 'XXX' : unreferenced formal parameter */
#pragma warning (disable: 4100)
#endif

//////////////////////////////////////////////////////////////////////////
// _xafx_main()
//////////////////////////////////////////////////////////////////////////
int XT_STDECL _xafx_main( int argc, TCHAR *argv[] )
{
	int nRet = FUN_RET_OK;
	CXtApp *pXtApp = NULL;

	pXtApp = xafx_get_app();
	if ( pXtApp == NULL )
	{
		_tprintf( _T("Aborted! xafx_get_app()==NULL\n") );
		return FUN_RET_OK;
	}

	nRet = xafx_app_init( argc, argv );
	if ( nRet < 0 )
	{
		_tprintf( _T("Aborted! xafx_app_init()==%d\n"), nRet );
		goto LBL_INIT_FAIL;
	}

	nRet = pXtApp->InitApplication( argc, argv );
	if ( nRet < 0 )
	{
		_tprintf( _T("Aborted! InitApplication()==%d\n"), nRet );
		goto LBL_INIT_FAIL;
	}

LBL_START_INSTANCE:
	nRet = pXtApp->InitInstance();
	if ( nRet < 0 )
	{
		_tprintf( _T("Aborted! InitInstance()==%d\n"), nRet );
		goto LBL_INIT_FAIL;
	}

	nRet = pXtApp->Run();
	if ( nRet < 0 )
	{
		_tprintf( _T("Exiting! Run()==%d\n"), nRet );
		goto LBL_INIT_FAIL;
	}

	nRet = pXtApp->ExitInstance();
	if ( nRet < 0 )
	{
		_tprintf( _T("Exiting! ExitInstance()==%d\n"), nRet );
		goto LBL_INIT_FAIL;
	}

	if ( CXtApp::m_nTermFlag == XTAPP_TF_RESTART )
	{
		nRet = pXtApp->RestartInstance();
		goto LBL_START_INSTANCE;
	}

	nRet = pXtApp->ExitApplication();
	if ( nRet < 0 )
	{
		_tprintf( _T("Exiting! ExitApplication()==%d\n"), nRet );
		goto LBL_INIT_FAIL;
	}

LBL_INIT_FAIL:

	xafx_app_exit();

	pXtApp = NULL;

	return FUN_RET_OK;
}
