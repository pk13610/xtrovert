/************************************************************************************
*	ExtApp.cpp: Implementation File
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
#include "app/inc/XtApp.h"
#include "xt_file_api.h"
#include "xt_thread_api.h"

#if defined(__WINDOWS__)
/* warning C4100: 'XXX' : unreferenced formal parameter */
#pragma warning (disable: 4100)
#endif

//////////////////////////////////////////////////////////////////////////
// CLASS CXtApp
//////////////////////////////////////////////////////////////////////////

/* _T('-') */
const TCHAR XT_STRCMD_DIRECTIVE_BRIEF = _T('-');
/* Complete Directive_T("--") [CPLT means Complete] */
const TCHAR XT_STRCMD_DIRECTIVE_CPLT[2] = {_T('-'), _T('-')}; /* _T("--") */
/* _T('/') */
const TCHAR XT_STRCMD_DIRECTIVE_SLASH = _T('/'); /* Compatible to Windows _T('/') */

int CXtApp::m_nTermFlag = XTAPP_TF_STOP;
CXtApp* CXtApp::m_pThisApp = NULL;

int xt_strcmd_directive_type( const TCHAR *src, const TCHAR **cmd )
{
	int nRet = XT_STRCMD_TYPE_END;
	int len = 0;

	len = _tcslen(src);
	if ( len < 2 )
	{
		return XT_STRCMD_TYPE_END;
	}

	/* len >= 2 */
	switch( src[0] )
	{
	case XT_STRCMD_DIRECTIVE_BRIEF:
		if ( src[0]==XT_STRCMD_DIRECTIVE_CPLT[0] && src[1]==XT_STRCMD_DIRECTIVE_CPLT[1] )
		{
			nRet = XT_STRCMD_TYPE_CPLT;
			if ( len == 2 )
			{
				nRet = XT_STRCMD_TYPE_END;
			}				
			else /*  len > 2  */
			{
				*cmd = src + sizeof(XT_STRCMD_DIRECTIVE_CPLT);
			}
		}
		else
		{
			nRet = XT_STRCMD_TYPE_BRIEF;
			*cmd = src + sizeof(XT_STRCMD_DIRECTIVE_SLASH);
		}
		break;
	case XT_STRCMD_DIRECTIVE_SLASH:
		nRet = XT_STRCMD_TYPE_SLASH;
		*cmd = src + sizeof(XT_STRCMD_DIRECTIVE_SLASH);
		break;
	default:
		nRet = XT_STRCMD_TYPE_END;
		break;
	}

	return nRet;
}

CXtApp::CXtApp(void)
{
	Reset();
	m_pThisApp = this;
}

CXtApp::~CXtApp(void)
{
	Reset();
}

int CXtApp::InitApplication( int argc, TCHAR *argv[] )
{
	int nRet = FUN_RET_OK;

	/* Command Line Arguments Processing. */
	nRet = ProcessShellCommand( argc, argv );	

	SetupDaemonMode();

	nRet = OnInitApp();
	if ( nRet < 0 )
	{
		return -5;
	}

	return FUN_RET_OK;
}

int CXtApp::ExitApplication(void)
{
	OnExitApp();

	return FUN_RET_OK;
}

int CXtApp::InitInstance(void)
{
	int nRet = FUN_RET_OK;

	if ( m_bDaemon ) /*  == TRUE */
	{
		nRet = Daemon();
		if ( nRet < 0 ) /* Failed to transform the process into daemon mode. */
		{
			m_bDaemon = FALSE;
			return -3;
		}
	}

	nRet = OnInitInstance();
	if ( nRet < 0 )
	{
		return -5;
	}

	return FUN_RET_OK;
}

int CXtApp::ExitInstance(void)
{
	OnExitInstance();


	return FUN_RET_OK;
}

int	CXtApp::RestartInstance(void)
{
	return FUN_RET_OK;
}

int CXtApp::Run(void)
{
	m_nTermFlag = XTAPP_TF_RUNNING;

	return CXtThread::Run();
}

#if defined(_DEBUG)
void CXtApp::SetLabel( const TCHAR *szLabel )
{
	if ( szLabel == NULL )
	{
		return;
	}

	int len =  _tcslen(szLabel);
	if ( len >= _MAX_STR_LEN_32_ )
	{
		len = (_MAX_STR_LEN_32_-1);
	}

	_tcsncpy( m_szLabel, szLabel, len*sizeof(TCHAR) );
	m_szLabel[len] = 0;
}
#endif

void CXtApp::Reset(void)
{
	m_nTermFlag = XTAPP_TF_STOP;

	m_bRestart = FALSE;

	m_bDaemon = FALSE;

	m_pThisApp = NULL;

	m_pCmdLineParser = NULL;
}

// React to a shell-issued command line directive.
int CXtApp::ProcessShellCommand( int argc, TCHAR *argv[] )
{
	if ( m_pCmdLineParser == NULL )
	{
		return FUN_RET_OK;
	}

	int nRet = FUN_RET_OK;

	const TCHAR *cszOpt = NULL;
	const TCHAR *cszParam = NULL;
	BOOL bBriefOpt = FALSE;

	int nCurArg = 1;

	while( nCurArg < argc )
	{
		cszOpt = NULL;
		cszParam = NULL;
		bBriefOpt = GetShellCommand( argc-nCurArg, &argv[nCurArg], &cszOpt, &cszParam );
		if ( cszOpt && cszParam )
		{
			nCurArg += 2;
		}
		else
		{
			++nCurArg;
		}

		nRet = m_pCmdLineParser->ParseCommand( cszOpt, cszParam, bBriefOpt );
	}

	return FUN_RET_OK;
}

BOOL CXtApp::GetShellCommand( int argc, TCHAR* argv[], const TCHAR **cszOption, const TCHAR **cszParam )
{
	int nCurArg = 0;
	BOOL bBriefOpt = FALSE;
	int nDirectiveType = XT_STRCMD_TYPE_END;

	nDirectiveType = xt_strcmd_directive_type( argv[nCurArg], cszOption );
	switch( nDirectiveType )
	{
	case XT_STRCMD_TYPE_BRIEF:
	case XT_STRCMD_TYPE_SLASH:
		bBriefOpt = TRUE;
		if ( nCurArg+1<argc &&  (argv[nCurArg+1][0]!=XT_STRCMD_TYPE_BRIEF && argv[nCurArg+1][0]!=XT_STRCMD_TYPE_SLASH) )
		{
			*cszParam = argv[nCurArg+1];
		}
		break;
	case XT_STRCMD_TYPE_CPLT:
		bBriefOpt = FALSE;
		if ( nCurArg+1<argc &&  (argv[nCurArg+1][0]!=XT_STRCMD_TYPE_BRIEF && argv[nCurArg+1][0]!=XT_STRCMD_TYPE_SLASH) )
		{
			*cszParam = argv[nCurArg+1];
		}
		break;
	default: /* NOT an option-command. Assume it as parameter. */
		if ( argv[nCurArg][0] == _T('?') )
		{
			bBriefOpt = TRUE;
			*cszOption = _T("h");
		}
		else
		{
			*cszParam = argv[nCurArg];
		}
		break;
	}

	return bBriefOpt;
}

void CXtApp::SetupDaemonMode(void)
{
	m_bDaemon = FALSE;
}

int CXtApp::OnInitApp(void)
{
	return FUN_RET_OK;
}

int CXtApp::OnExitApp(void)
{
	return FUN_RET_OK;
}

int CXtApp::Daemon(void)
{
	pid_t nPid = 0;
	nPid = xt_fork_daemon();
	if ( nPid < 0 ) /* Error, Failed to fork */
	{
		return -3;
	}
	if ( nPid > 0 ) /* Parent Process */
	{
		/*Release();*/
		//exit(0);
		return FUN_RET_OK;
	}
	/* Child Process */

	_tprintf( _T("Transformed to daemon process.\n") );

	return nPid;
}

int CXtApp::OnInitInstance(void)
{

	return FUN_RET_OK;
}

int CXtApp::OnExitInstance(void)
{
	return FUN_RET_OK;
}

int CXtApp::AppProc(void)
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
	}

	return FUN_RET_OK;
}

int CXtApp::GetCwd( TCHAR *szDir )
{
	return xt_get_cwd( szDir, MAX_PATH );
}

int CXtApp::SetCwd( const TCHAR *szDir )
{
	return xt_set_cwd( szDir, MAX_PATH );
}

int CXtApp::GetExeDir( TCHAR *szDir )
{
	return xt_get_exe_dir( szDir, MAX_PATH );
}

const TCHAR* CXtApp::GetCmdLineString(void)
{
	return NULL;
}

void CXtApp::GotTerminate( int sig )
{
	CXtApp::m_nTermFlag = XTAPP_TF_STOP;
}

//////////////////////////////////////////////////////////////////////////
// CLASS CXtApp END
//////////////////////////////////////////////////////////////////////////
