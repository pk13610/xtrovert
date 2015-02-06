/************************************************************************************
* ExtSignal.cpp: Implementation File
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

#include "xt_sig_api.h"

/****************************************************
	Cross-platform signal functions.
****************************************************/
sighandler_t xt_signal( int signum, sighandler_t handler )
{
	return signal( signum, handler );
}

/****************************************************
	Xtrovert signal.
****************************************************/
void xt_ignore_sig_def(void)
{
#if defined(__LINUX__)
	xt_ignore_signal( SIGPIPE );
	xt_ignore_signal( SIGHUP );
	xt_ignore_signal( SIGCHLD );
#endif
}

int xt_set_sigterm_handler( sighandler_t handler )
{
	signal( SIGINT, handler );
	signal( SIGTERM, handler );
	signal( SIGSEGV, handler );

#if defined(__LINUX__)
	signal( SIGQUIT, handler );
#elif defined(__WINDOWS__)
	if ( !SetConsoleCtrlHandler(xt_console_term, TRUE) )
	{
		return -5;
	}
#else
	return FUN_RET_ERR_NOTIMPL;
#endif

	return FUN_RET_OK;
}

/****************************************************
	MS-WINDOWS Console functions
****************************************************/
#if defined(__WINDOWS__)
int WINAPI xt_console_term( DWORD dwEvent )
{
	switch( dwEvent )
	{
	case CTRL_BREAK_EVENT:
		/* Restart Application? */
	case CTRL_C_EVENT:
	case CTRL_LOGOFF_EVENT:
	case CTRL_SHUTDOWN_EVENT:
	case CTRL_CLOSE_EVENT:
		/* Trigger SIGINT */
		raise(SIGINT);
		break;
	default:
		/* unknown type--better pass it on. */
		return 0;
	}

	return 1;
}
#endif /* __WINDOWS__ */

int xt_stop_external_console( pid_t proc_id )
{
#if defined(__WINDOWS__)
	//proc_id retrieved by GetWindowThreadProcessId(  )
	FreeConsole();
	if ( AttachConsole(proc_id) )
	{
		if ( !GenerateConsoleCtrlEvent(CTRL_C_EVENT, 0) )
		{
			FreeConsole();
			return -4;
		}

		FreeConsole();
	}
	else
	{
		return -5;
	}
#elif defined(__LINUX__)
#endif
	return FUN_RET_OK;
}
