/************************************************************************************
* xt_thread_api.h : header file
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
#ifndef __XT_THREAD_API_H__
#define __XT_THREAD_API_H__

#include "xt_thread_define.h"
#include "xt_file_define.h"

#ifdef __cplusplus
extern "C" {
#endif

#if defined(__LINUX__)
	#include <pthread.h>
#endif

#if defined(__LINUX__)
#define XT_SHELL_PATH "/bin/sh"
#elif defined(__WINDOWS__)
#define XT_SHELL_PATH "cmd.exe"
#endif

/**
 * The prototype for any Xtrovert thread worker functions.
 */
#define XT_THREAD_FUNC  __stdcall
/* The prototype for any Xtrovert thread worker functions. */
typedef struct _tag_xt_thread_t xt_thread_t;
typedef void* (XT_THREAD_FUNC *xt_pfn_thread_t)(xt_thread_t*,void*);

#if defined(__LINUX__)
	typedef struct _tag_xt_thread_t
	{
		BOOL			new_mem;
		pthread_t		*td;
		void			*data;
		xt_pfn_thread_t func;
		xt_state_t		exitval;
	}xt_thread_t;

	typedef struct _tag_xt_threadattr_t
	{
		BOOL			new_mem;
		pthread_attr_t	attr;
	}xt_threadattr_t;

	typedef struct _tag_xt_threadkey_t
	{
		pthread_key_t key;
	}xt_threadkey_t;

	typedef struct _tag_xt_thread_once_t
	{
		BOOL			new_mem;
		pthread_once_t	once;
	}xt_thread_once_t;

	typedef struct _tag_xt_procattr_t
	{
		BOOL			new_mem;
		xt_file_t		*parent_in;
		xt_file_t		*child_in;
		xt_file_t		*parent_out;
		xt_file_t		*child_out;
		xt_file_t		*parent_err;
		xt_file_t		*child_err;
		TCHAR			*currdir;
		int				cmdtype;
		int				detached;
		xt_child_errfn_t	*errfn;
		int				errchk;
		xt_uid_t		uid;
		xt_gid_t		gid;
	}xt_procattr_t;

#elif defined(__WINDOWS__)
	typedef struct _tag_xt_thread_t
	{
		BOOL			new_mem;
		HANDLE			td;
		int				cancel;
		int				cancel_how;
		void			*data;
		xt_pfn_thread_t func;
		xt_state_t		exitval;
	}xt_thread_t;

	typedef struct _tag_xt_threadattr_t
	{
		BOOL			new_mem;
		int				detach;
		int				stacksize;
	}xt_threadattr_t;

	typedef struct _tag_xt_threadkey_t
	{
		DWORD key;
	}xt_threadkey_t;

	typedef struct _tag_xt_thread_once_t
	{
		BOOL			new_mem;
		long			value;
	}xt_thread_once_t;

	typedef struct _tag_xt_procattr_t
	{
		BOOL			new_mem;
		xt_file_t		*parent_in;
		xt_file_t		*child_in;
		xt_file_t		*parent_out;
		xt_file_t		*child_out;
		xt_file_t		*parent_err;
		xt_file_t		*child_err;
		TCHAR			*currdir;
		int				cmdtype;
		int				detached;
		xt_child_errfn_t *errfn;
		int				errchk;
	}xt_procattr_t;

#endif
	
/**
* Create and initialize a new threadattr variable
* @param new_attr The newly created threadattr.
* @param cont The pool to use
*/
XT_DECLARE(xt_state_t) xt_threadattr_create( xt_threadattr_t **attr, BOOL new_mem );

XT_DECLARE(xt_state_t) xt_threadattr_destroy( xt_threadattr_t *attr );

/**
 * Set if newly created threads should be created in detached state.
 * @param attr The threadattr to affect 
 * @param on Non-zero if detached threads should be created.
 */
XT_DECLARE(xt_state_t) xt_threadattr_detach_set( xt_threadattr_t *attr, int on);

/**
 * Get the detach state for this threadattr.
 * @param attr The threadattr to reference
 * @return APR_DETACH if threads are to be detached, or APR_NOTDETACH
 * if threads are to be joinable. 
 */
XT_DECLARE(xt_state_t) xt_threadattr_detach_get( xt_threadattr_t *attr );

/**
 * Set the stack size of newly created threads.
 * @param attr The threadattr to affect 
 * @param stacksize The stack size in bytes
 */
XT_DECLARE(xt_state_t) xt_threadattr_stacksize_set( xt_threadattr_t *attr, xt_size_t stacksize );

/**
 * Set the stack guard area size of newly created threads.
 * @param attr The threadattr to affect 
 * @param guardsize The stack guard area size in bytes
 * @note Thread library implementations commonly use a "guard area"
 * after each thread's stack which is not readable or writable such that
 * stack overflows cause a segfault; this consumes e.g. 4K of memory
 * and increases memory management overhead.  Setting the guard area
 * size to zero hence trades off reliable behaviour on stack overflow
 * for performance. */
XT_DECLARE(xt_state_t) xt_threadattr_guardsize_set( xt_threadattr_t *attr, xt_size_t guardsize );

/**
 * Create a new thread of execution
 * @param new_thread The newly created thread handle.
 * @param attr The threadattr to use to determine how to create the thread
 * @param func The function to start the new thread in
 * @param data Any data to be passed to the starting function
 * @param cont The pool to use
 */
XT_DECLARE(xt_state_t) xt_thread_create( xt_thread_t **new_thread, xt_threadattr_t *attr, xt_pfn_thread_t fn, void *data, BOOL new_mem );

/**
 * stop the current thread
 * @param thd The thread to stop
 * @param retval The return value to pass back to any thread that cares
 */
XT_DECLARE(xt_state_t) xt_thread_exit( xt_thread_t *thd, xt_state_t retval );

/**
 * block until the desired thread stops executing.
 * @param retval The return value from the dead thread.
 * @param thd The thread to join
 */
XT_DECLARE(xt_state_t) xt_thread_join( xt_state_t *retval, xt_thread_t *thd ); 

/**
 * detach a thread
 * @param thd The thread to detach 
 */
XT_DECLARE(xt_state_t) xt_thread_detach( xt_thread_t *thd );

/**
 * force the current thread to yield the processor
 */
XT_DECLARE(void) xt_thread_yield(void);

/**
 * Initialize the control variable for apr_thread_once.  If this isn't
 * called, apr_initialize won't work.
 * @param control The control variable to initialize
 * @param p The pool to allocate data from.
 */
XT_DECLARE(xt_state_t) xt_thread_once_init( xt_thread_once_t **control, BOOL new_mem );

/**
 * Run the specified function one time, regardless of how many threads
 * call it.
 * @param control The control variable.  The same variable should
 *                be passed in each time the function is tried to be
 *                called.  This is how the underlying functions determine
 *                if the function has ever been called before.
 * @param func The function to call.
 */
XT_DECLARE(xt_state_t) xt_thread_once( xt_thread_once_t *control, void (*func)(void) );

/**
 * Compare two thread id's
 * @param tid1 1st Thread ID to compare
 * @param tid2 2nd Thread ID to compare
 * @return non-zero if the two threads are equal, zero otherwise
 */ 
XT_DECLARE(int) xt_os_thread_equal( xt_os_thread_t tid1, xt_os_thread_t tid2 );

/**
 * Setup the process for a single thread to be used for all signal handling.
 * @warning This must be called before any threads are created
 */
/*XT_DECLARE(xt_state_t) xt_setup_signal_thread(void);*/

/**
 * Make the current thread listen for signals.  This thread will loop
 * forever, calling a provided function whenever it receives a signal.  That
 * functions should return 1 if the signal has been handled, 0 otherwise.
 * @param signal_handler The function to call when a signal is received
 * apr_status_t apr_signal_thread((int)(*signal_handler)(int signum))
 */
/*XT_DECLARE(xt_state_t) xt_signal_thread( int(*signal_handler)(int signum) );*/

pthread_t xt_thread_self(void);

XT_DECLARE(pid_t) xt_fork_daemon(void);

XT_DECLARE(xt_state_t) xt_atexit( void (*pfn_exit)(void) );

int xt_used_mt(void);

/**
 * Create and initialize a new procattr variable
 * @param new_attr The newly created procattr. 
 * @param cont The pool to use
 */
XT_DECLARE(xt_state_t) xt_procattr_create( xt_procattr_t **attr, BOOL new_mem );

/**
 * Determine if any of stdin, stdout, or stderr should be linked to pipes 
 * when starting a child process.
 * @param attr The procattr we care about. 
 * @param in Should stdin be a pipe back to the parent?
 * @param out Should stdout be a pipe back to the parent?
 * @param err Should stderr be a pipe back to the parent?
 * @note If APR_NO_PIPE, there will be no special channel, the child
 * inherits the parent's corresponding stdio stream.  If APR_NO_FILE is 
 * specified, that corresponding stream is closed in the child (and will
 * be INVALID_HANDLE_VALUE when inspected on Win32). This can have ugly 
 * side effects, as the next file opened in the child on Unix will fall
 * into the stdio stream fd slot!
 */
XT_DECLARE(xt_state_t) xt_procattr_io_set( xt_procattr_t *attr, int in, int out, int err );

/**
 * Set the child_in and/or parent_in values to existing apr_file_t values.
 * @param attr The procattr we care about. 
 * @param child_in apr_file_t value to use as child_in. Must be a valid file.
 * @param parent_in apr_file_t value to use as parent_in. Must be a valid file.
 * @remark  This is NOT a required initializer function. This is
 *          useful if you have already opened a pipe (or multiple files)
 *          that you wish to use, perhaps persistently across multiple
 *          process invocations - such as a log file. You can save some 
 *          extra function calls by not creating your own pipe since this
 *          creates one in the process space for you.
 * @bug Note that calling this function with two NULL files on some platforms
 * creates an APR_FULL_BLOCK pipe, but this behavior is neither portable nor
 * is it supported.  @see apr_procattr_io_set instead for simple pipes.
 */
XT_DECLARE(xt_state_t) xt_procattr_child_in_set( xt_procattr_t *attr, xt_file_t *child_in, xt_file_t *parent_in );

/**
 * Set the child_out and parent_out values to existing apr_file_t values.
 * @param attr The procattr we care about. 
 * @param child_out apr_file_t value to use as child_out. Must be a valid file.
 * @param parent_out apr_file_t value to use as parent_out. Must be a valid file.
 * @remark This is NOT a required initializer function. This is
 *         useful if you have already opened a pipe (or multiple files)
 *         that you wish to use, perhaps persistently across multiple
 *         process invocations - such as a log file. 
 * @bug Note that calling this function with two NULL files on some platforms
 * creates an APR_FULL_BLOCK pipe, but this behavior is neither portable nor
 * is it supported.  @see apr_procattr_io_set instead for simple pipes.
 */
XT_DECLARE(xt_state_t) xt_procattr_child_out_set( xt_procattr_t *attr, xt_file_t *child_out, xt_file_t *parent_out );

/**
 * Set the child_err and parent_err values to existing apr_file_t values.
 * @param attr The procattr we care about. 
 * @param child_err apr_file_t value to use as child_err. Must be a valid file.
 * @param parent_err apr_file_t value to use as parent_err. Must be a valid file.
 * @remark This is NOT a required initializer function. This is
 *         useful if you have already opened a pipe (or multiple files)
 *         that you wish to use, perhaps persistently across multiple
 *         process invocations - such as a log file. 
 * @bug Note that calling this function with two NULL files on some platforms
 * creates an APR_FULL_BLOCK pipe, but this behavior is neither portable nor
 * is it supported.  @see apr_procattr_io_set instead for simple pipes.
 */
XT_DECLARE(xt_state_t) xt_procattr_child_err_set( xt_procattr_t *attr, xt_file_t *child_err, xt_file_t *parent_err );

/**
 * Set which directory the child process should start executing in.
 * @param attr The procattr we care about. 
 * @param dir Which dir to start in.  By default, this is the same dir as
 *            the parent currently resides in, when the createprocess call
 *            is made. 
 */
XT_DECLARE(xt_state_t) xt_procattr_dir_set( xt_procattr_t *attr, const TCHAR *dir );

/**
 * Set what type of command the child process will call.
 * @param attr The procattr we care about. 
 * @param cmd The type of command.  One of:
 * <PRE>
 *            APR_SHELLCMD     --  Anything that the shell can handle
 *            APR_PROGRAM      --  Executable program   (default) 
 *            APR_PROGRAM_ENV  --  Executable program, copy environment
 *            APR_PROGRAM_PATH --  Executable program on PATH, copy env
 * </PRE>
 */
XT_DECLARE(xt_state_t) xt_procattr_cmdtype_set( xt_procattr_t *attr, xt_cmdtype_t cmd );

/**
 * Determine if the child should start in detached state.
 * @param attr The procattr we care about. 
 * @param detach Should the child start in detached state?  Default is no. 
 */
XT_DECLARE(xt_state_t) xt_procattr_detach_set( xt_procattr_t *attr, int detach );

/**
 * Specify an error function to be called in the child process if APR
 * encounters an error in the child prior to running the specified program.
 * @param attr The procattr describing the child process to be created.
 * @param errfn The function to call in the child process.
 * @remark At the present time, it will only be called from apr_proc_create()
 *         on platforms where fork() is used.  It will never be called on other
 *         platforms, on those platforms apr_proc_create() will return the error
 *         in the parent process rather than invoke the callback in the now-forked
 *         child process.
 */
XT_DECLARE(xt_state_t) xt_procattr_child_errfn_set( xt_procattr_t *attr, xt_child_errfn_t *errfn );

/**
 * Specify that apr_proc_create() should do whatever it can to report
 * failures to the caller of apr_proc_create(), rather than find out in
 * the child.
 * @param attr The procattr describing the child process to be created.
 * @param chk Flag to indicate whether or not extra work should be done
 *            to try to report failures to the caller.
 * @remark This flag only affects apr_proc_create() on platforms where
 *         fork() is used.  This leads to extra overhead in the calling
 *         process, but that may help the application handle such
 *         errors more gracefully.
 */
XT_DECLARE(xt_state_t) xt_procattr_error_check_set( xt_procattr_t *attr, int chk );

/**
 * Determine if the child should start in its own address space or using the 
 * current one from its parent
 * @param attr The procattr we care about. 
 * @param addrspace Should the child start in its own address space?  Default
 *                  is no on NetWare and yes on other platforms.
 */
XT_DECLARE(xt_state_t) xt_procattr_addrspace_set( xt_procattr_t *attr, int addrspace );

/**
 * Set the username used for running process
 * @param attr The procattr we care about. 
 * @param username The username used
 * @param password User password if needed. Password is needed on WIN32
 *                 or any other platform having
 *                 APR_PROCATTR_USER_SET_REQUIRES_PASSWORD set.
 */
XT_DECLARE(xt_state_t) xt_procattr_user_set( xt_procattr_t *attr, const TCHAR *username, const TCHAR *password );

/**
 * Set the group used for running process
 * @param attr The procattr we care about. 
 * @param groupname The group name  used
 */
XT_DECLARE(xt_state_t) xt_procattr_group_set( xt_procattr_t *attr, const TCHAR *groupname );

/**
 * This is currently the only non-portable call in APR.  This executes 
 * a standard unix fork.
 * @param proc The resulting process handle. 
 * @param cont The pool to use. 
 * @remark returns APR_INCHILD for the child, and APR_INPARENT for the parent
 * or an error.
 */
XT_DECLARE(xt_state_t) xt_proc_fork( xt_proc_t *proc );

/**
 * Create a new process and execute a new program within that process.
 * @param new_proc The resulting process handle.
 * @param progname The program to run 
 * @param args the arguments to pass to the new program.  The first 
 *             one should be the program name.
 * @param env The new environment table for the new process.  This 
 *            should be a list of NULL-terminated strings. This argument
 *            is ignored for APR_PROGRAM_ENV, APR_PROGRAM_PATH, and
 *            APR_SHELLCMD_ENV types of commands.
 * @param attr the procattr we should use to determine how to create the new
 *         process
 * @param pool The pool to use.
 * @note This function returns without waiting for the new process to terminate;
 * use apr_proc_wait for that.
 */
XT_DECLARE(xt_state_t) xt_proc_create( xt_proc_t *new_proc, const TCHAR *progname, const TCHAR * const *args,
                                          const TCHAR * const *env, 
                                          xt_procattr_t *attr );

/**
 * Wait for a child process to die
 * @param proc The process handle that corresponds to the desired child process 
 * @param exitcode The returned exit status of the child, if a child process 
 *                 dies, or the signal that caused the child to die.
 *                 On platforms that don't support obtaining this information, 
 *                 the status parameter will be returned as APR_ENOTIMPL.
 * @param exitwhy Why the child died, the bitwise or of:
 * <PRE>
 *            APR_PROC_EXIT         -- process terminated normally
 *            APR_PROC_SIGNAL       -- process was killed by a signal
 *            APR_PROC_SIGNAL_CORE  -- process was killed by a signal, and
 *                                     generated a core dump.
 * </PRE>
 * @param waithow How should we wait.  One of:
 * <PRE>
 *            APR_WAIT   -- block until the child process dies.
 *            APR_NOWAIT -- return immediately regardless of if the 
 *                          child is dead or not.
 * </PRE>
 * @remark The child's status is in the return code to this process.  It is one of:
 * <PRE>
 *            APR_CHILD_DONE     -- child is no longer running.
 *            APR_CHILD_NOTDONE  -- child is still running.
 * </PRE>
 */
XT_DECLARE(xt_state_t) xt_proc_wait( xt_proc_t *proc, int *exitcode, xt_exit_why_t *exitwhy, xt_wait_how_t waithow );

/**
 * Wait for any current child process to die and return information 
 * about that child.
 * @param proc Pointer to NULL on entry, will be filled out with child's 
 *             information 
 * @param exitcode The returned exit status of the child, if a child process 
 *                 dies, or the signal that caused the child to die.
 *                 On platforms that don't support obtaining this information, 
 *                 the status parameter will be returned as APR_ENOTIMPL.
 * @param exitwhy Why the child died, the bitwise or of:
 * <PRE>
 *            APR_PROC_EXIT         -- process terminated normally
 *            APR_PROC_SIGNAL       -- process was killed by a signal
 *            APR_PROC_SIGNAL_CORE  -- process was killed by a signal, and
 *                                     generated a core dump.
 * </PRE>
 * @param waithow How should we wait.  One of:
 * <PRE>
 *            APR_WAIT   -- block until the child process dies.
 *            APR_NOWAIT -- return immediately regardless of if the 
 *                          child is dead or not.
 * </PRE>
 * @param p Pool to allocate child information out of.
 * @bug Passing proc as a *proc rather than **proc was an odd choice
 * for some platforms... this should be revisited in 1.0
 */
XT_DECLARE(xt_state_t) xt_proc_wait_all_procs( xt_proc_t *proc,
                                                  int *exitcode,
                                                  xt_exit_why_t *exitwhy,
                                                  xt_wait_how_t waithow );

#define APR_PROC_DETACH_FOREGROUND 0    /**< Do not detach */
#define APR_PROC_DETACH_DAEMONIZE 1     /**< Detach */

/**
 * Detach the process from the controlling terminal.
 * @param daemonize set to non-zero if the process should daemonize
 *                  and become a background process, else it will
 *                  stay in the foreground.
 */
XT_DECLARE(xt_state_t) xt_proc_detach( int daemonize );

/**
 * Register an other_child -- a child associated to its registered 
 * maintence callback.  This callback is invoked when the process
 * dies, is disconnected or disappears.
 * @param proc The child process to register.
 * @param maintenance maintenance is a function that is invoked with a 
 *                    reason and the data pointer passed here.
 * @param data Opaque context data passed to the maintenance function.
 * @param write_fd An fd that is probed for writing.  If it is ever unwritable
 *                 then the maintenance is invoked with reason 
 *                 OC_REASON_UNWRITABLE.
 * @param p The pool to use for allocating memory.
 * @bug write_fd duplicates the proc->out stream, it's really redundant
 * and should be replaced in the APR 1.0 API with a bitflag of which
 * proc->in/out/err handles should be health checked.
 * @bug no platform currently tests the pipes health.
 */
XT_DECLARE(void) xt_proc_other_child_register( xt_proc_t *proc, 
                                           void (*maintenance) (int reason, 
                                                                void *, 
                                                                int status),
                                           void *data, xt_file_t *write_fd );

/**
 * Stop watching the specified other child.  
 * @param data The data to pass to the maintenance function.  This is
 *             used to find the process to unregister.
 * @warning Since this can be called by a maintenance function while we're
 *          scanning the other_children list, all scanners should protect 
 *          themself by loading ocr->next before calling any maintenance 
 *          function.
 */
XT_DECLARE(void) xt_proc_other_child_unregister( void *data );

/**
 * Notify the maintenance callback of a registered other child process
 * that application has detected an event, such as death.
 * @param proc The process to check
 * @param reason The reason code to pass to the maintenance function
 * @param status The status to pass to the maintenance function
 * @remark An example of code using this behavior;
 * <pre>
 * rv = apr_proc_wait_all_procs(&proc, &exitcode, &status, APR_WAIT, p);
 * if (APR_STATUS_IS_CHILD_DONE(rv)) {
 * \#if APR_HAS_OTHER_CHILD
 *     if (apr_proc_other_child_alert(&proc, APR_OC_REASON_DEATH, status)
 *             == APR_SUCCESS) {
 *         ;  (already handled)
 *     }
 *     else
 * \#endif
 *         [... handling non-otherchild processes death ...]
 * </pre>
 */
XT_DECLARE(xt_state_t) xt_proc_other_child_alert( xt_proc_t *proc, int reason, int status );

/**
 * Test all registered other child processes and invoke the maintenance callback 
 * with the appropriate reason code, if still running, or the appropriate reason 
 * code if the process is no longer healthy.
 * @param reason The reason code (e.g. APR_OC_REASON_RESTART) to running processes
 */
XT_DECLARE(void) xt_proc_other_child_refresh_all( int reason );

/** 
 * Terminate a process.
 * @param proc The process to terminate.
 * @param sig How to kill the process.
 */
XT_DECLARE(xt_state_t) xt_proc_kill( xt_proc_t *proc, int sig );
	
#ifdef __cplusplus
}
#endif

#endif /*__XT_THREAD_API_H__*/
