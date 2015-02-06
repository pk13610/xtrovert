/************************************************************************************
* XtApp.h : header file
*
* CXtApp Definition header, An Application framework for common use
*			(Xtrovert Application Frameworks).
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
#ifndef __XT_APPLICATION_H__
#define __XT_APPLICATION_H__

#include "XtThread.h"

/*
	Application Parameter:
		-buildinfo(bi)		Compiler, Platform(Win/Linux, 32/64bits), Built date.
		-start/stop/restart
		-reload
*/

//////////////////////////////////////////////////////////////////////////
// CLASS CXtApp
//////////////////////////////////////////////////////////////////////////

/* Terminate flag */
enum {
	XTAPP_TF_STOP=0,
	XTAPP_TF_RUNNING,
	XTAPP_TF_RESTART,

	XTAPP_TF_END
};

/* _T('-') */
const TCHAR XT_STRCMD_DIRECTIVE_BRIEF;
/* Complete Directive_T("--") [CPLT means Complete] */
const TCHAR XT_STRCMD_DIRECTIVE_CPLT[2]; /* _T("--") */
/* _T('/') */
const TCHAR XT_STRCMD_DIRECTIVE_SLASH; /* Compatible to Windows _T('/') */

class CXtApp : public CXtThread
{
public:

	CXtApp(void);
	virtual ~CXtApp(void);
	
	// System Environment initialization/destruction.
	int					InitApplication( int argc, TCHAR *argv[] );
	int					ExitApplication(void);

	// Application logics initialization/destruction.
	int					InitInstance(void);
	int					ExitInstance(void); // return app exit code
	int					RestartInstance(void); // handle restart by Restart Manager

	int					Run(void);

	/* Administrator Mode */
	/*int					OptCmdAdmin(void);*/
	/*int					CmdNetState(void);*/
	/*int					CmdNetConnect( TCHAR *szAddress );*/
	/*int					CmdNetRestart(void);*/
	/*int					CmdNetPing( TCHAR *szAddress );*/

	static int			m_nTermFlag;

	static CXtApp		*m_pThisApp;

#if defined(_DEBUG)
	void				SetLabel( const TCHAR *szLabel );
	TCHAR				m_szLabel[_MAX_STR_LEN_32_];
#endif

protected:

	virtual void		Reset(void);

//	virtual BOOL		OnIdle( LONG lCount ); // return TRUE if more idle processing

	/***************************************************************** 
	*	InitApplication() is implemented with the following methods.
	*	ExitApplication() is implemented with the following methods.
	******************************************************************/
	/* React to a shell-issued command line directive. */
	virtual int			ProcessShellCommand( int argc, TCHAR *argv[] );
	virtual BOOL		GetShellCommand( int argc, TCHAR* argv[], const TCHAR **cszOption, const TCHAR **cszParam );
	/* Decide whether process runs under Daemon Mode. */
	virtual void		SetupDaemonMode(void); /* Setup : m_bDaemon = TRUE/FALSE */
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
	int 				Daemon(void);
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

	/* Methods */
	int 				GetCwd( TCHAR *szDir ); /* Get current working directory. */
	int 				SetCwd( const TCHAR *szDir ); /* Set current working directory. */
	int 				GetExeDir( TCHAR *szDir ); /* Get directory where exe-file lies in. */
	const TCHAR*		GetCmdLineString(void); /* Get command line string, that how to start this program. */

	static void			GotTerminate( int sig );

	/*virtual int			WaitThreads(void);*/

	/* This process is running under daemon mode or not? */
	BOOL				m_bDaemon; /* SetupDaemonMode() setup this member. */

	BOOL				m_bRestart;

private:

};

#endif /*__XT_APPLICATION_H__*/
