/************************************************************************************
*	xt_thread_api.c: Implementation File
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

#include "xt_thread_api.h"

#if defined(__WINDOWS__)
/* warning C4100: 'XXX' : unreferenced formal parameter */
#pragma warning (disable: 4100)
#endif

#if defined( __WINDOWS__)
#include <process.h>    /* _beginthreadex, _endthreadex */
#elif defined(__LINUX__)
 #include<pthread.h>
#endif

pthread_t xt_thread_self(void)
{
#if defined(__WINDOWS__)
	return GetCurrentThreadId();
#elif defined(__LINUX__)
	return pthread_self();
#else
	return 0;
#endif /* __LINUX__ */
}

XT_DECLARE(pid_t) xt_fork_daemon(void)
{
	/*
		Return Value:
			To be comply to Linux fork sys-call,
			0:	child process
			>0:	parent process, process id
			<0: error
	*/
	pid_t nPid = FUN_RET_OK;
#if defined(__LINUX__)
	/* fork twice */
	/* fork first time */
	xt_set_sighandler( SIGCHLD, SIG_IGN );
	nPid = fork();
	if ( nPid < 0 ) // error
	{
		return -3;
	}
	else
	{
		if ( nPid > 0 ) // Parent Process
		{
			return nPid;
		}
	}

	/* Here child process goes */
	setsid();
	/* fork second time */
	xt_set_sighandler( SIGCHLD, SIG_IGN );
	nPid = fork();
	if ( nPid < 0 ) // error
	{
		return -4;
	}
	else
	{
		if ( nPid > 0 ) // Parent Process
		{
			return nPid;
		}
	}

	umask(0); /* setup file creation model */

	nPid = 0; /* To identify child process */
#elif defined(__WINDOWS__)
#else
	return FUN_RET_OK;
#endif
	return nPid;
}

XT_DECLARE(xt_state_t) xt_atexit( void (*pfn_exit)(void) )
{
	return atexit(pfn_exit);
}

int xt_used_mt(void) /* Whether Multi-thread lib linked or not */
{
#if defined(__LINUX__)
	return ((pthread_create==NULL)?0:1);
#elif defined(__WINDOWS__)
	return -4;//((_beginthreadex!=NULL)?0:1);
#else
	return -3;
#endif
};

