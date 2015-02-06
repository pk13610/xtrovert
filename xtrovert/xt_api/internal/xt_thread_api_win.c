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

#if defined(__WINDOWS__)
/* warning C4100: 'XXX' : unreferenced formal parameter */
#pragma warning (disable: 4100)

#include <process.h>    /* _beginthreadex, _endthreadex */
#include <TlHelp32.h> /* PROCESSENTRY32W */

DWORD xt_tls_thread = 0;
/* Heavy on no'ops, here's what we want to pass if there is APR_NO_FILE
 * requested for a specific child handle;
 */
static xt_file_t no_file = { 0 };

XT_DECLARE(xt_state_t) xt_threadattr_create( xt_threadattr_t **attr, BOOL new_mem )
{
	if ( new_mem )
	{
		*attr = xt_mem_alloc( sizeof(xt_threadattr_t) );
	}

	if ((*attr) == NULL)
	{
		return XT_STD_ERR_ENOMEM;
	}

	(*attr)->new_mem = new_mem;
	(*attr)->detach = 0;
	(*attr)->stacksize = 0;

	return FUN_RET_OK;
}

XT_DECLARE(xt_state_t) xt_threadattr_destroy( xt_threadattr_t *attr )
{
	if ( attr->new_mem )
	{
		xt_mem_free( attr );
	}

	return FUN_RET_OK;	
}

XT_DECLARE(xt_state_t) xt_threadattr_detach_set( xt_threadattr_t *attr, int on)
{
	attr->detach = on;
	return FUN_RET_OK;
}

XT_DECLARE(xt_state_t) xt_threadattr_detach_get( xt_threadattr_t *attr )
{
	if ( attr->detach == 1 )
		return XT_DETACH;
	return XT_NOTDETACH;
}

XT_DECLARE(xt_state_t) xt_threadattr_stacksize_set( xt_threadattr_t *attr, xt_size_t stacksize )
{
	attr->stacksize = stacksize;
	return FUN_RET_OK;
}

XT_DECLARE(xt_state_t) xt_threadattr_guardsize_set( xt_threadattr_t *attr, xt_size_t guardsize )
{
	return XT_ENOTIMPL;
}

static void* xt_thread_worker( void *opaque )
{
	xt_thread_t *thd = (xt_thread_t*)opaque;
	TlsSetValue( xt_tls_thread, thd->td );
	return thd->func( thd, thd->data );
}

XT_DECLARE(xt_state_t) xt_thread_create( xt_thread_t **new_thread, xt_threadattr_t *attr, xt_pfn_thread_t fn, void *data, BOOL new_mem )
{
	unsigned temp;
    HANDLE handle;

	if ( new_mem )
	{
		*new_thread = xt_mem_alloc( sizeof(xt_thread_t) );
	}

	if ( (*new_thread) == NULL )
	{
		return XT_STD_ERR_ENOMEM;
	}

	(*new_thread)->new_mem = new_mem;
    (*new_thread)->data = data;
    (*new_thread)->func = fn;
    (*new_thread)->td   = NULL;

    /* Use 0 for default Thread Stack Size, because that will
     * default the stack to the same size as the calling thread.
     */
	handle = (HANDLE)_beginthreadex( NULL, (DWORD)(attr?attr->stacksize:0), 
						(unsigned int (XT_THREAD_FUNC *)(void *))xt_thread_worker, 
						(*new_thread), 0, &temp );
    if ( handle == 0 )
	{
        return XT_FROM_OS_ERROR(_doserrno);
    }

    if (attr && attr->detach)
	{
        CloseHandle(handle);
    }
    else
	{
		(*new_thread)->td = handle;
	}

    return FUN_RET_OK;
}

XT_DECLARE(xt_state_t) xt_thread_exit( xt_thread_t *thd, xt_state_t retval )
{
	thd->exitval = retval;
	if ( thd->new_mem )
	{
		xt_mem_free( thd );
	}

	_endthreadex(0);

	return FUN_RET_OK;
}

XT_DECLARE(xt_state_t) xt_thread_join( xt_state_t *retval, xt_thread_t *thd )
{
	xt_state_t rv = FUN_RET_OK;

	if (!thd->td)
	{
		/* Can not join on detached threads */
		return XT_DETACH;
	}

	rv = WaitForSingleObject( thd->td, INFINITE );
	if ( rv == WAIT_OBJECT_0 || rv == WAIT_ABANDONED )
	{
		/* If the thread_exit has been called */
		if (!thd->new_mem)
			*retval = thd->exitval;
		else
			rv = XT_INCOMPLETE;
	}
	else
	{
		rv = xt_get_os_error();
	}

	CloseHandle(thd->td);
	thd->td = NULL;

	return rv;
}

XT_DECLARE(xt_state_t) xt_thread_detach( xt_thread_t *thd )
{
	if ( thd->td && CloseHandle(thd->td) )
	{
		thd->td = NULL;
		return FUN_RET_OK;
	}
	else
	{
		return xt_get_os_error();
	}
}

XT_DECLARE(void) xt_thread_yield(void)
{
    /* SwitchToThread is not supported on Win9x, but since it's
     * primarily a noop (entering time consuming code, therefore
     * providing more critical threads a bit larger timeslice)
     * we won't worry too much if it's not available.
     */
}

XT_DECLARE(xt_state_t) xt_thread_once_init( xt_thread_once_t **control, BOOL new_mem )
{
	if ( new_mem )
	{
		*control = xt_mem_alloc( sizeof(**control) );
	}

	if ( *control )
	{
		return FUN_RET_OK;
	}

	(*control)->new_mem = new_mem;

	return XT_STD_ERR_ENOMEM;
}

XT_DECLARE(xt_state_t) xt_thread_once( xt_thread_once_t *control, void (*func)(void) )
{
	if ( !InterlockedExchange(&control->value, 1) )
	{
		func();
	}
	return FUN_RET_OK;
}

XT_DECLARE(int) xt_os_thread_equal( xt_os_thread_t tid1, xt_os_thread_t tid2 )
{
    /* Since the only tid's we support our are own, and
     * apr_os_thread_current returns the identical handle
     * to the one we created initially, the test is simple.
     */
    return (tid1 == tid2);
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

    return FUN_RET_OK;
}

XT_DECLARE(xt_state_t) xt_procattr_io_set( xt_procattr_t *attr, int in, int out, int err )
{
    xt_state_t stat = FUN_RET_OK;

    if ( in )
	{
        /* APR_CHILD_BLOCK maps to APR_WRITE_BLOCK, while
         * APR_PARENT_BLOCK maps to APR_READ_BLOCK, so transpose 
         * the CHILD/PARENT blocking flags for the stdin pipe.
         * stdout/stderr map to the correct mode by default.
         */
        if ( in == XT_CHILD_BLOCK )
            in = XT_READ_BLOCK;
        else if ( in == XT_PARENT_BLOCK )
            in = XT_WRITE_BLOCK;

        if (in == XT_NO_FILE)
            attr->child_in = &no_file;
        else
		{
			/*stat = apr_file_pipe_create_ex( &attr->child_in, &attr->parent_in, in, attr->pool );*/
        }
//         if (stat == FUN_RET_OK)
//             stat = apr_file_inherit_unset( attr->parent_in );
    }
    if ( out && stat==FUN_RET_OK )
	{
        if ( out == XT_NO_FILE )
            attr->child_out = &no_file;
        else
		{ 
            /*stat = apr_file_pipe_create_ex(&attr->parent_out, &attr->child_out, out, attr->pool);*/
        }
//         if ( stat == FUN_RET_OK )
//             stat = apr_file_inherit_unset(attr->parent_out);
    }
    if ( err && stat==FUN_RET_OK )
	{
        if ( err == XT_NO_FILE )
            attr->child_err = &no_file;
        else
		{ 
			/*stat = apr_file_pipe_create_ex(&attr->parent_err, &attr->child_err, err, attr->pool);*/
        }
// 		if ( stat == FUN_RET_OK )
//             stat = apr_file_inherit_unset(attr->parent_err);
    }
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
	return FUN_RET_OK;
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
	DWORD stat;
	DWORD time;

	if ( waithow == XT_WAIT )
		time = INFINITE;
	else
		time = 0;

	if ( (stat = WaitForSingleObject(proc->hproc, time)) == WAIT_OBJECT_0 )
	{
		if ( GetExitCodeProcess(proc->hproc, &stat) )
		{
			if (exitcode)
				*exitcode = stat;
			if (exitwhy)
				*exitwhy = why_from_exit_code(stat);
			CloseHandle(proc->hproc);
			proc->hproc = NULL;
			return XT_CHILD_DONE;
		}
	}
	else if ( stat == WAIT_TIMEOUT )
	{
		return XT_CHILD_NOTDONE;
	}
	return xt_get_os_error();
}

XT_DECLARE(xt_state_t) xt_proc_wait_all_procs( xt_proc_t *proc, int *exitcode, xt_exit_why_t *exitwhy, xt_wait_how_t waithow )
{
	DWORD  dwId    = GetCurrentProcessId();
    DWORD  i;
    DWORD  nChilds = 0;
    DWORD  nActive = 0;
    HANDLE ps32;
    PROCESSENTRY32W pe32;
    BOOL   bHasMore = FALSE;
    DWORD  dwFlags  = PROCESS_QUERY_INFORMATION;
    xt_state_t rv = XT_EGENERAL;

    if ( waithow == XT_WAIT )
        dwFlags |= SYNCHRONIZE;

	ps32 = CreateToolhelp32Snapshot( TH32CS_SNAPPROCESS, 0 );
    if ( !(ps32) )
	{
        return xt_get_os_error();
    }
    pe32.dwSize = sizeof(PROCESSENTRY32W);
    if ( !Process32FirstW(ps32, &pe32) )
	{
        if (GetLastError() == ERROR_NO_MORE_FILES)
            return XT_EOF;
        else
            return xt_get_os_error();
    }
    do {
        DWORD  dwRetval = 0;
        DWORD  nHandles = 0;
        HANDLE hProcess = NULL;
        HANDLE pHandles[MAXIMUM_WAIT_OBJECTS];
        do {
            if (pe32.th32ParentProcessID == dwId)
			{
                nChilds++;
                if ( (hProcess = OpenProcess(dwFlags, FALSE, pe32.th32ProcessID)) != NULL )
				{
                    if ( GetExitCodeProcess(hProcess, &dwRetval) )
					{
                        if (dwRetval == STILL_ACTIVE)
						{
                            nActive++;
                            if (waithow == XT_WAIT)
                                pHandles[nHandles++] = hProcess;
                            else
                                CloseHandle(hProcess);
                        }
                        else
						{                                
                            /* Process has exited.
                                * No need to wait for its termination.
                                */
                            CloseHandle(hProcess);
                            if (exitcode)
                                *exitcode = dwRetval;
                            if (exitwhy)
                                *exitwhy  = why_from_exit_code(dwRetval);
                            proc->pid = pe32.th32ProcessID;
                        }
                    }
                    else
					{
                        /* Unexpected error code.
                            * Cleanup and return;
                            */
                        rv = xt_get_os_error();
                        CloseHandle(hProcess);
                        for ( i=0; i<nHandles; i++ )
                            CloseHandle(pHandles[i]);
                        return rv;
                    }
                }
                else {
                    /* This is our child, so it shouldn't happen
                        * that we cannot open our child's process handle.
                        * However if the child process increased the
                        * security token it might fail.
                        */
                }
            }
			bHasMore = Process32NextW( ps32, &pe32 );
        } while ( (bHasMore) && nHandles<MAXIMUM_WAIT_OBJECTS );

        if ( nHandles )
		{
            /* Wait for all collected processes to finish */
            DWORD waitStatus = WaitForMultipleObjects(nHandles, pHandles, TRUE, INFINITE);
            for ( i=0; i<nHandles; i++ )
                CloseHandle(pHandles[i]);
            if ( waitStatus == WAIT_OBJECT_0 )
			{
                /* Decrease active count by the number of awaited
                    * processes.
                    */
                nActive -= nHandles;
            }
            else
			{
                /* Broken from the infinite loop */
                break;
            }
        }
    } while (bHasMore);
    CloseHandle(ps32);

    if ( waithow != XT_WAIT )
	{
        if ( nChilds && nChilds == nActive )
		{
            /* All child processes are running */
            rv = XT_CHILD_NOTDONE;
            proc->pid = -1;
        }
        else {
            /* proc->pid contains the pid of the
                * exited processes
                */
            rv = XT_CHILD_DONE;
        }
    }

    if (nActive == 0) {
        rv = XT_CHILD_DONE;
        proc->pid = -1;
    }

    return rv;
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

#endif /* __WINDOWS__ */

