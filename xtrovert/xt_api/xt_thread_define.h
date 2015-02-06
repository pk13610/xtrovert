/************************************************************************************
* xt_thread_define.h : header file
*
* Thread API Definition header
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
************************************************************************************/
#ifndef __XT_THREAD_DEFINE_H__
#define __XT_THREAD_DEFINE_H__

#include "xt_define.h"
#include "xt_error.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Process & Thread data types definition for Platform-cross. */
#if defined(__WINDOWS__)
	/*Thread Id type*/
	typedef UINT pthread_t;
	/*Process Id Type*/
	typedef int pid_t;

	typedef HANDLE                xt_os_thread_t;
	typedef HANDLE                xt_os_proc_t;
	typedef DWORD                 xt_os_threadkey_t; 
	typedef FILETIME              xt_os_imp_time_t;
	typedef SYSTEMTIME			xt_os_exp_time_t;

	/* Structure for determining user ownership. */
	typedef PSID				xt_uid_t;
	/* Structure for determining group ownership. */
	typedef PSID				xt_gid_t;

#elif defined(__LINUX__)

	typedef pthread_t             xt_os_thread_t;      /**< native thread */
	typedef pthread_key_t         xt_os_threadkey_t;   /**< native thread address */

	typedef pid_t                 xt_os_proc_t;        /**< native pid */
	typedef struct timeval        xt_os_imp_time_t;    /**< native timeval */
	typedef struct tm             xt_os_exp_time_t;    /**< native tm */
	/* Structure for determining user ownership. */
	typedef uid_t						xt_uid_t;
	/* Structure for determining group ownership. */
	typedef gid_t						xt_gid_t;

#endif

/**
 * @defgroup xt_thread_proc Threads and Process Functions
 * @ingroup Xtrovert 
 * @{
 */
typedef enum _tag_xt_cmdtype_t{
    XT_SHELLCMD,			/**< use the shell to invoke the program */
    XT_PROGRAM,				/**< invoke the program directly, no copied env */
    XT_PROGRAM_ENV,			/**< invoke the program, replicating our environment */
    XT_PROGRAM_PATH,		/**< find program on PATH, use our environment */
    XT_SHELLCMD_ENV			/**< use the shell to invoke the program,
                             *   replicating our environment
                             */
}xt_cmdtype_t;

typedef enum _tag_xt_wait_how_t{
    XT_WAIT,           /**< wait for the specified process to finish */
    XT_NOWAIT          /**< do not wait -- just see if it has finished */
}xt_wait_how_t;

/* I am specifically calling out the values so that the macros below make
 * more sense.  Yes, I know I don't need to, but I am hoping this makes what
 * I am doing more clear.  If you want to add more reasons to exit, continue
 * to use bitmasks.
 */
typedef enum _tag_xt_exit_why_t{
    XT_PROC_EXIT = 1,          /**< process exited normally */
    XT_PROC_SIGNAL = 2,        /**< process exited due to a signal */
    XT_PROC_SIGNAL_CORE = 4    /**< process exited and dumped a core file */
} xt_exit_why_t;

/** did we exit the process */
#define XT_PROC_CHECK_EXIT(x)        ((x) & XT_PROC_EXIT)
/** did we get a signal */
#define XT_PROC_CHECK_SIGNALED(x)    ((x) & XT_PROC_SIGNAL)
/** did we get core */
#define XT_PROC_CHECK_CORE_DUMP(x)   ((x) & XT_PROC_SIGNAL_CORE)

/** @see xt_procattr_io_set */
#define XT_NO_PIPE          0
/** @see xt_procattr_io_set and apr_file_pipe_create_ex */
#define XT_FULL_BLOCK       1
/** @see xt_procattr_io_set and apr_file_pipe_create_ex */
#define XT_FULL_NONBLOCK    2
/** @see xt_procattr_io_set */
#define XT_PARENT_BLOCK     3
/** @see xt_procattr_io_set */
#define XT_CHILD_BLOCK      4
/** @see xt_procattr_io_set */
#define XT_NO_FILE          8

/** @see xt_file_pipe_create_ex */
#define XT_READ_BLOCK       3
/** @see xt_file_pipe_create_ex */
#define XT_WRITE_BLOCK      4

/** @see xt_procattr_io_set 
 * @note Win32 only effective with version 1.2.12, portably introduced in 1.3.0
 */
#define XT_NO_FILE          8

/**
 * @defgroup XT_OC Other Child Flags
 * @{
 */
#define XT_OC_REASON_DEATH         0     /**< child has died, caller must call
                                           * unregister still */
#define XT_OC_REASON_UNWRITABLE    1     /**< write_fd is unwritable */
#define XT_OC_REASON_RESTART       2     /**< a restart is occurring, perform
                                           * any necessary cleanup (including
                                           * sending a special signal to child)
                                           */
#define XT_OC_REASON_UNREGISTER    3     /**< unregister has been called, do
                                           * whatever is necessary (including
                                           * kill the child) */
#define XT_OC_REASON_LOST          4     /**< somehow the child exited without
                                           * us knowing ... buggy os? */
#define XT_OC_REASON_RUNNING       5     /**< a health check is occurring, 
                                           * for most maintainence functions
                                           * this is a no-op.
                                           */


/** The XT process type */
typedef struct _tag_xt_proc_t {
    /** The process ID */
    pid_t pid;
    /** Parent's side of pipe to child's stdin */
    void *in;
    /** Parent's side of pipe to child's stdout */
    void *out;
    /** Parent's side of pipe to child's stdouterr */
    void *err;
#if defined(WIN32)
    /** (Win32 only) Creator's handle granting access to the process
     * @remark This handle is closed and reset to NULL in every case
     * corresponding to a waitpid() on Unix which returns the exit status.
     * Therefore Win32 correspond's to Unix's zombie reaping characteristics
     * and avoids potential handle leaks.
     */
    HANDLE hproc;
#endif
}xt_proc_t;

/** @} */

/**
 * The prototype for APR child errfn functions.  (See the description
 * of apr_procattr_child_errfn_set() for more information.)
 * It is passed the following parameters:
 * @param pool Pool associated with the apr_proc_t.  If your child
 *             error function needs user data, associate it with this
 *             pool.
 * @param err Xtrovert error code describing the error
 * @param description Text description of type of processing which failed
 */
typedef void (xt_child_errfn_t)( xt_state_t err, const TCHAR *description );


typedef enum _tag_xt_kill_conditions_t{
    XT_KILL_NEVER,             /**< process is never killed (i.e., never sent
                                 * any signals), but it will be reaped if it exits
                                 * before the pool is cleaned up */
    XT_KILL_ALWAYS,            /**< process is sent SIGKILL on apr_pool_t cleanup */
    XT_KILL_AFTER_TIMEOUT,     /**< SIGTERM, wait 3 seconds, SIGKILL */
    XT_JUST_WAIT,              /**< wait forever for the process to complete */
    XT_KILL_ONLY_ONCE          /**< send SIGTERM and then wait */
} xt_kill_conditions_t;

/* For int xt_used_mt(void) */
#if defined(__LINUX__)
int pthread_create( pthread_t*, const pthread_attr_t*, void* (*)(void*), void* ) __attribute__((weak));
#elif defined(__WINDOWS__)
//__declspec(selectany) HANDLE _beginthreadex( VOID*, UINT, Uint (__stdcall * initialcode) (void *), VOID*, UINT, UINT* );
//__declspec(selectany) HANDLE _beginthreadex( VOID*, UINT, Uint (__stdcall * initialcode) (void *), VOID*, UINT, UINT* );
#else
#endif
/* For inline BOOL	xt_used_mt(VOID) -- END */

/***********************************************************************
* Platform Independent Thread API Collection
***********************************************************************/

	
#ifdef __cplusplus
}
#endif

#endif /*__XT_THREAD_DEFINE_H__*/
