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
#include "xt_mem_api.h"

#if defined(__LINUX__)

DWORD xt_tls_thread = 0;
/* Heavy on no'ops, here's what we want to pass if there is APR_NO_FILE
 * requested for a specific child handle;
 */
static xt_file_t no_file = { 0 };

/* Destroy the threadattr object */
static xt_state_t threadattr_cleanup( void *data )
{
	xt_threadattr_t *attr = data;
	xt_state_t rv;

	rv = pthread_attr_destroy( &attr->attr );

	return rv;
}

XT_DECLARE(xt_state_t) xt_threadattr_create( xt_threadattr_t **attr, BOOL new_mem )
{
	xt_state_t stat;
	if ( new_mem )
	{
		*attr = xt_mem_alloc( sizeof(xt_threadattr_t) );
		
	}

	if ( *attr == NULL )
	{
		return XT_STD_ERR_ENOMEM;
	}
	
	stat = pthread_attr_init( &(*attr)->attr );

	if ( stat == 0 )
	{
		/*apr_pool_cleanup_register(pool, *new, threadattr_cleanup,apr_pool_cleanup_null);*/
		(*attr)->new_mem = new_mem;

		return FUN_RET_OK;
	}

	return stat;
}

XT_DECLARE(xt_state_t) xt_threadattr_destroy( xt_threadattr_t *attr )
{
	if ( attr->new_mem )
	{
		xt_mem_free( attr );
	}

	return FUN_RET_OK;	
}

#if defined(__LINUX__)
#define XT_DETACH_ARG(v) (((v) ? PTHREAD_CREATE_DETACHED : PTHREAD_CREATE_JOINABLE))
#else
#define XT_DETACH_ARG(v) (((v) ? XT_TREAD_DETATCH_ON : XT_TREAD_DETATCH_OFF))
#endif

#if defined(PTHREAD_CREATE_DETACHED)
#define DETACH_ARG(v) ((v) ? PTHREAD_CREATE_DETACHED : PTHREAD_CREATE_JOINABLE)
#else
#define DETACH_ARG(v) ((v) ? 1 : 0)
#endif

XT_DECLARE(xt_state_t) xt_threadattr_detach_set( xt_threadattr_t *attr, int on)
{
	xt_state_t stat;
#ifdef HAVE_ZOS_PTHREADS
	int arg = XT_DETACH_ARG(on);
	stat = pthread_attr_setdetachstate( &attr->attr, &arg );
	if ( stat == 0 )
	{
#else
	stat = pthread_attr_setdetachstate( &attr->attr, XT_DETACH_ARG(on) );
	if ( stat == 0 )
	{
#endif
			return FUN_RET_OK;
	}
	else {
#ifdef HAVE_ZOS_PTHREADS
		stat = errno;
#endif

		return stat;
	}
}

XT_DECLARE(xt_state_t) xt_threadattr_detach_get( xt_threadattr_t *attr )
{
	int state;

#ifdef PTHREAD_ATTR_GETDETACHSTATE_TAKES_ONE_ARG
	state = pthread_attr_getdetachstate( &attr->attr );
#else
	pthread_attr_getdetachstate( &attr->attr, &state );
#endif
	if ( state == XT_DETACH_ARG(1) )
		return XT_DETACH;

	return XT_NOTDETACH;
}

XT_DECLARE(xt_state_t) xt_threadattr_stacksize_set( xt_threadattr_t *attr, xt_size_t stacksize )
{
	int stat;

	stat = pthread_attr_setstacksize( &attr->attr, stacksize );
	if ( stat == 0 )
	{
		return FUN_RET_OK;
	}
#ifdef HAVE_ZOS_PTHREADS
	stat = errno;
#endif

	return stat;
}

XT_DECLARE(xt_state_t) xt_threadattr_guardsize_set( xt_threadattr_t *attr, xt_size_t guardsize )
{
#ifdef HAVE_PTHREAD_ATTR_SETGUARDSIZE
	xt_state_t rv;

	rv = pthread_attr_setguardsize( &attr->attr, size );
	if (rv == 0) {
		return FUN_RET_OK;
	}
#ifdef HAVE_ZOS_PTHREADS
	rv = errno;
#endif
	return rv;
#else
	return XT_ENOTIMPL;
#endif
}

static void* xt_thread_worker( void *opaque )
{
	xt_thread_t *thd = (xt_thread_t*)opaque;
	return thd->func( thd, thd->data );
}

XT_DECLARE(xt_state_t) xt_thread_create( xt_thread_t **new_thread, xt_threadattr_t *attr, xt_pfn_thread_t fn, void *data, BOOL new_mem )
{
	xt_state_t stat;
	pthread_attr_t *temp;

	if ( new_mem )
	{
		*new_thread = xt_mem_alloc( sizeof(xt_thread_t) );
	}

	if ( (*new_thread) == NULL )
	{
		return XT_STD_ERR_ENOMEM;
	}

	(*new_thread)->data = data;
	(*new_thread)->func = fn;

	if (attr)
		temp = &attr->attr;
	else
		temp = NULL;

	stat = pthread_create( (*new_thread)->td, temp, xt_thread_worker, (*new_thread) );
	if ( stat == 0 )
	{
		return FUN_RET_OK;
	}
	else {
#ifdef HAVE_ZOS_PTHREADS
		stat = errno;
#endif

		return stat;
	}
}

XT_DECLARE(xt_state_t) xt_thread_exit( xt_thread_t *thd, xt_state_t retval )
{
	thd->exitval = retval;
	pthread_exit(NULL);
	return FUN_RET_OK;
}

XT_DECLARE(xt_state_t) xt_thread_join( xt_state_t *retval, xt_thread_t *thd )
{
	xt_state_t stat;
	xt_state_t *thread_stat;

	stat = pthread_join( *thd->td, (void *)&thread_stat );
	if ( stat == 0 )
	{
		*retval = thd->exitval;
		return FUN_RET_OK;
	}
	else {
#ifdef HAVE_ZOS_PTHREADS
		stat = errno;
#endif

		return stat;
	}
}

XT_DECLARE(xt_state_t) xt_thread_detach( xt_thread_t *thd )
{
	xt_state_t stat;

#ifdef HAVE_ZOS_PTHREADS
	stat = pthread_detach( thd->td );
	if ( stat == 0 )
	{
#else
	stat = pthread_detach( *thd->td );
	if ( stat == 0 )
	{
#endif
		return FUN_RET_OK;
	}
	else {
#ifdef HAVE_ZOS_PTHREADS
		stat = errno;
#endif

		return stat;
	}
}

XT_DECLARE(void) xt_thread_yield(void)
{
#ifdef HAVE_PTHREAD_YIELD
	#ifdef HAVE_ZOS_PTHREADS
		pthread_yield(NULL);
	#else
		pthread_yield();
	#endif /* HAVE_ZOS_PTHREADS */
#else
	#ifdef HAVE_SCHED_YIELD
		sched_yield();
	#endif
#endif
}

XT_DECLARE(xt_state_t) xt_thread_once_init( xt_thread_once_t **control, BOOL new_mem )
{
	static const pthread_once_t once_init = PTHREAD_ONCE_INIT;

	if ( new_mem )
	{
		*control = xt_mem_alloc( sizeof(**control) );
	}

	if ( *control )
	{
		return FUN_RET_OK;
	}

	(*control)->new_mem = new_mem;
	(*control)->once = once_init;

	return FUN_RET_OK;
}

XT_DECLARE(xt_state_t) xt_thread_once( xt_thread_once_t *control, void (*func)(void) )
{
	return pthread_once(&control->once, func);
}

XT_DECLARE(int) xt_os_thread_equal( xt_os_thread_t tid1, xt_os_thread_t tid2 )
{
    /* Since the only tid's we support our are own, and
     * apr_os_thread_current returns the identical handle
     * to the one we created initially, the test is simple.
     */
    return pthread_equal(tid1, tid2);
}

/* 
 * some of the ideas expressed herein are based off of Microsoft
 * Knowledge Base article: Q190351
 *
 */
XT_DECLARE(xt_state_t) xt_procattr_create( xt_procattr_t **att, BOOL new_mem )
{
	if ( new_mem )
	{
		*att = xt_mem_alloc( sizeof(xt_procattr_t) );
	}

	if ( *att == NULL )
	{
		return XT_STD_ERR_ENOMEM;
	}

	(*att)->new_mem = new_mem;
	(*att)->cmdtype = XT_PROGRAM;
	(*att)->uid = (*att)->gid = -1;
	return FUN_RET_OK;
}

XT_DECLARE(xt_state_t) xt_procattr_io_set( xt_procattr_t *attr, int in, int out, int err )
{
	xt_state_t stat = FUN_RET_OK;
	return stat;
}

XT_DECLARE(xt_state_t) xt_procattr_child_in_set( xt_procattr_t *attr, xt_file_t *child_in, xt_file_t *parent_in )
{
	xt_state_t rv = FUN_RET_OK;

	return rv;
}

XT_DECLARE(xt_state_t) xt_procattr_child_out_set( xt_procattr_t *attr, xt_file_t *child_out, xt_file_t *parent_out )
{
	xt_state_t rv = FUN_RET_OK;

	return rv;
}

XT_DECLARE(xt_state_t) xt_procattr_child_err_set( xt_procattr_t *attr, xt_file_t *child_err, xt_file_t *parent_err )
{
	xt_state_t rv = FUN_RET_OK;

	return rv;
}

XT_DECLARE(xt_state_t) xt_procattr_dir_set( xt_procattr_t *attr, const TCHAR *dir )
{
	return FUN_RET_OK;
}

XT_DECLARE(xt_state_t) xt_procattr_cmdtype_set( xt_procattr_t *attr, xt_cmdtype_t cmd )
{
	attr->cmdtype = cmd;
	return FUN_RET_OK;
}

XT_DECLARE(xt_state_t) xt_procattr_detach_set( xt_procattr_t *attr, int detach )
{
	attr->detached = detach;
	return FUN_RET_OK;
}

XT_DECLARE(xt_state_t) xt_procattr_child_errfn_set( xt_procattr_t *attr, xt_child_errfn_t *errfn )
{
	attr->errfn = errfn;
	return FUN_RET_OK;
}

XT_DECLARE(xt_state_t) xt_procattr_error_check_set( xt_procattr_t *attr, int chk )
{
	attr->errchk = chk;
	return FUN_RET_OK;
}

XT_DECLARE(xt_state_t) xt_procattr_addrspace_set( xt_procattr_t *attr, int addrspace )
{
	/* won't ever be used on this platform, so don't save the flag */
	return FUN_RET_OK;
}

XT_DECLARE(xt_state_t) xt_procattr_user_set( xt_procattr_t *attr, const TCHAR *username, const TCHAR *password )
{
	return FUN_RET_OK;
}

XT_DECLARE(xt_state_t) xt_procattr_group_set( xt_procattr_t *attr, const TCHAR *groupname )
{
	return FUN_RET_OK;
}

XT_DECLARE(xt_state_t) xt_proc_fork( xt_proc_t *proc )
{
	int pid;

	memset(proc, 0, sizeof(xt_proc_t));

	if ( (pid = fork()) < 0 )
	{
		return errno;
	}
	else if ( pid == 0 )
	{
		proc->pid = getpid();

		/*apr_random_after_fork(proc);*/

		return XT_INCHILD;
	}

	proc->pid = pid;

	return XT_INPARENT;
}

XT_DECLARE(xt_state_t) xt_proc_create( xt_proc_t *new_proc, const TCHAR *progname, const TCHAR * const *args,
											const TCHAR * const *env, 
											xt_procattr_t *attr )
{
	return FUN_RET_OK;
}

static xt_exit_why_t why_from_exit_code( DWORD exit )
{
    /* See WinNT.h STATUS_ACCESS_VIOLATION and family for how
     * this class of failures was determined
     */
    if ( ((exit & 0xC0000000)==0xC0000000) && !(exit & 0x3FFF0000) )
        return XT_PROC_SIGNAL;
    else
        return XT_PROC_EXIT;

    /* ### No way to tell if Dr Watson grabbed a core, AFAICT. */
}

XT_DECLARE(xt_state_t) xt_proc_wait( xt_proc_t *proc, int *exitcode, xt_exit_why_t *exitwhy, xt_wait_how_t waithow )
{
	pid_t pstatus;
	int waitpid_options = WUNTRACED;
	int exit_int;
	int ignore;
	xt_exit_why_t ignorewhy;

	if ( exitcode == NULL )
	{
		exitcode = &ignore;
	}

	if ( exitwhy == NULL )
	{
		exitwhy = &ignorewhy;
	}

	if ( waithow != XT_WAIT )
	{
		waitpid_options |= WNOHANG;
	}

	do
	{
		pstatus = waitpid( proc->pid, &exit_int, waitpid_options );
	} while (pstatus < 0 && errno == EINTR);

	if ( pstatus > 0 )
	{
		proc->pid = pstatus;

		if ( WIFEXITED(exit_int) )
		{
			*exitwhy = XT_PROC_EXIT;
			*exitcode = WEXITSTATUS(exit_int);
		}
		else if ( WIFSIGNALED(exit_int) )
		{
			*exitwhy = XT_PROC_SIGNAL;

#ifdef WCOREDUMP
			if (WCOREDUMP(exit_int)) {
				*exitwhy |= XT_PROC_SIGNAL_CORE;
			}
#endif

			*exitcode = WTERMSIG( exit_int );
		}
		else
		{
			/* unexpected condition */
			return XT_EGENERAL;
		}

		return XT_CHILD_DONE;
	}
	else if ( pstatus == 0 )
	{
		return XT_CHILD_NOTDONE;
	}

	return errno;
}

XT_DECLARE(xt_state_t) xt_proc_wait_all_procs( xt_proc_t *proc, int *exitcode, xt_exit_why_t *exitwhy, xt_wait_how_t waithow )
{
	proc->pid = -1;
	return xt_proc_wait(proc, exitcode, exitwhy, waithow);
}

XT_DECLARE(xt_state_t) xt_proc_detach( int daemonize )
{
	return XT_ENOTIMPL;
}

XT_DECLARE(void) xt_proc_other_child_register( xt_proc_t *proc, 
												void (*maintenance) (int reason, 
																	void *, 
																	int status),
												void *data, xt_file_t *write_fd )
{
	return;
}

XT_DECLARE(void) xt_proc_other_child_unregister( void *data )
{
	return;
}

XT_DECLARE(xt_state_t) xt_proc_other_child_alert( xt_proc_t *proc, int reason, int status )
{
	return XT_ENOTIMPL;
}

XT_DECLARE(void) xt_proc_other_child_refresh_all( int reason )
{

}

XT_DECLARE(xt_state_t) xt_proc_kill( xt_proc_t *proc, int sig )
{
	if ( proc->hproc != NULL )
	{
        if (TerminateProcess(proc->hproc, sig) == 0) {
            return xt_get_os_error();
        }
        /* On unix, SIGKILL leaves a apr_proc_wait()able pid lying around, 
         * so we will leave hproc alone until the app calls apr_proc_wait().
         */
        return FUN_RET_OK;
    }
    return XT_EPROC_UNKNOWN;
}

#endif /* __LINUX__ */
