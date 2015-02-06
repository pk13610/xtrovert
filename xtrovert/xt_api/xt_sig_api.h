/************************************************************************************
* xt_sig_api.h : header file
*
* Signal Control Handler header
*
* AUTHOR	:	Sean Feng <SeanFeng2006@hotmail.com>
* DATE		:	Nov. 7, 2009
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
#ifndef __XT_SIG_API_H__
#define __XT_SIG_API_H__

#include "xt_define.h"
#include "xt_thread_api.h"
#include <signal.h>

#ifdef __cplusplus
extern "C" {
#endif

/****************************************************
	Cross-platform signal functions.
****************************************************/
#if defined(__WINDOWS__)
typedef void (*sighandler_t)(int);
#endif
/* Cross-platform signal() function. */
sighandler_t	xt_signal( int signum, sighandler_t handler );

/****************************************************
	Xtrovert signal.
****************************************************/
#define xt_ignore_signal(sig)			signal( (sig), SIG_IGN )
void			xt_ignore_sig_def(void);
int				xt_set_sigterm_handler( sighandler_t handler );

int				xt_stop_external_console( pid_t proc_id );

/****************************************************
	MS-WINDOWS Console functions
****************************************************/
#if defined(__WINDOWS__)
/*Example for setting up: SetConsoleCtrlHandler( xt_ctrl_term, TRUE );*/
/*Example for Cleaning up: SetConsoleCtrlHandler( xt_ctrl_term, FALSE );*/
int WINAPI xt_console_term( DWORD dwEvent );
#elif defined(__LINUX__)

#endif

#ifdef __cplusplus
}
#endif

#endif /*__XT_SIG_API_H__*/
