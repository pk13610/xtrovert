/************************************************************************************
* _test_app.cpp: Implementation File
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

#include "_test_app.h"

CXtTestApp::CXtTestApp(void)
{
}

CXtTestApp::~CXtTestApp(void)
{

}

int CXtTestApp::OnInitApp(void)
{
	return FUN_RET_OK;
}

int CXtTestApp::OnExitApp(void)
{
	return FUN_RET_OK;
}

int CXtTestApp::OnInitInstance(void)
{
	SetLabel( _T("CXtTestApp") );

	TCHAR szDir[MAX_PATH] = {0};
	GetExeDir(szDir);
	_tprintf( _T("GetExeDir:%s\n"), szDir );

	return FUN_RET_OK;
}

int CXtTestApp::OnExitInstance(void)
{
	m_szLabel[0] = 0;
	return FUN_RET_OK;
}

int CXtTestApp::AppProc(void)
{
	while ( m_nTermFlag == XTAPP_TF_RUNNING )
	{
		// 		while( bIdle && !PeekMessage() )
		// 		{
		// 
		// 		}
		// 
		// 		do
		// 		{
		// 			if ( !PumpMessage() )
		// 			{
		// 				//
		// 			}
		// 			
		// 		} while ( PeekMessage() );
		_tprintf( _T("%s AppProc!\n"), m_szLabel );
		Sleep(1000);
	}

	return FUN_RET_OK;
}

int CXtTestApp::OnAppCmdLineStart( const TCHAR *cszParam )
{
	_tprintf( _T("Start.:.. %s\n"), cszParam );
	return FUN_RET_OK;
}

int CXtTestApp::OnAppCmdLineServer( const TCHAR *cszParam )
{
	_tprintf( _T("OnAppCmdLineServer: %s\n"), cszParam );
	return FUN_RET_OK;
}

int CXtTestApp::OnAppCmdLineClient( const TCHAR *cszParam )
{
	_tprintf( _T("OnAppCmdLineClient: %s\n"), cszParam );
	return FUN_RET_OK;
}

CXtTestApp theXtApp;

int _test_app_main( int argc, TCHAR *argv[] )
{
	_xt_app_main( argc, argv );
	return 0;
}
