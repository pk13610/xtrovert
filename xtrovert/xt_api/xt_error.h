/************************************************************************************
* ExtError.h : header file
*
* Socket Error header
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
#ifndef __XT_ERROR_H__
#define __XT_ERROR_H__

#include "xt_define.h"
#include <errno.h>

#ifdef __cplusplus
extern "C" {
#endif

#if defined(__LINUX__)
#define XT_STD_ERR_EPERM			EPERM
#define XT_STD_ERR_ENOENT			ENOENT /*No such file or directory*/
#define XT_STD_ERR_ESRCH			ESRCH
#define XT_STD_ERR_EINTR			EINTR
#define XT_STD_ERR_EIO				EIO
#define XT_STD_ERR_ENXIO			ENXIO
#define XT_STD_ERR_E2BIG			E2BIG
#define XT_STD_ERR_ENOEXEC			ENOEXEC
#define XT_STD_ERR_EBADF			EBADF
#define XT_STD_ERR_ECHILD			ECHILD
#define XT_STD_ERR_EAGAIN			EAGAIN
#define XT_STD_ERR_ENOMEM			ENOMEM
#define XT_STD_ERR_EACCES			EACCES
#define XT_STD_ERR_EFAULT			EFAULT
#define XT_STD_ERR_EBUSY			EBUSY
#define XT_STD_ERR_EEXIST			EEXIST
#define XT_STD_ERR_EXDEV			EXDEV
#define XT_STD_ERR_ENODEV			ENODEV
#define XT_STD_ERR_ENOTDIR			ENOTDIR
#define XT_STD_ERR_EISDIR			EISDIR
#define XT_STD_ERR_EINVAL			EINVAL
#define XT_STD_ERR_ENFILE			ENFILE
#define XT_STD_ERR_EMFILE			EMFILE
#define XT_STD_ERR_ENOTTY			ENOTTY
#define XT_STD_ERR_EFBIG			EFBIG
#define XT_STD_ERR_ENOSPC			ENOSPC
#define XT_STD_ERR_ESPIPE			ESPIPE
#define XT_STD_ERR_EROFS			EROFS
#define XT_STD_ERR_EMLINK			EMLINK
#define XT_STD_ERR_EPIPE			EPIPE
#define XT_STD_ERR_EDOM			EDOM
#define XT_STD_ERR_EDEADLK			EDEADLK
#define XT_STD_ERR_ENAMETOOLONG	ENAMETOOLONG
#define XT_STD_ERR_ENOLCK			ENOLCK
#define XT_STD_ERR_ENOSYS			ENOSYS
#define XT_STD_ERR_ENOTEMPTY		ENOTEMPTY
/* POSIX SUPPLEMENT */
#define XT_STD_ERR_EADDRINUSE			EADDRINUSE
#define XT_STD_ERR_EADDRNOTAVAIL		EADDRNOTAVAIL
#define XT_STD_ERR_EAFNOSUPPORT		EAFNOSUPPORT
#define XT_STD_ERR_EALREADY			EALREADY
#define XT_STD_ERR_EBADMSG				EBADMSG
#define XT_STD_ERR_ECANCELED			ECANCELED
#define XT_STD_ERR_ECONNABORTED		ECONNABORTED
#define XT_STD_ERR_ECONNREFUSED		ECONNREFUSED
#define XT_STD_ERR_ECONNRESET			ECONNRESET
#define XT_STD_ERR_EDESTADDRREQ		EDESTADDRREQ
#define XT_STD_ERR_EHOSTUNREACH		EHOSTUNREACH
#define XT_STD_ERR_EIDRM				EIDRM
#define XT_STD_ERR_EINPROGRESS			EINPROGRESS
#define XT_STD_ERR_EISCONN				EISCONN
#define XT_STD_ERR_ELOOP				ELOOP
#define XT_STD_ERR_EMSGSIZE			EMSGSIZE
#define XT_STD_ERR_ENETDOWN			ENETDOWN
#define XT_STD_ERR_ENETRESET			ENETRESET
#define XT_STD_ERR_ENETUNREACH			ENETUNREACH
#define XT_STD_ERR_ENOBUFS				ENOBUFS
#define XT_STD_ERR_ENODATA				ENODATA
#define XT_STD_ERR_ENOLINK				ENOLINK
#define XT_STD_ERR_ENOMSG				ENOMSG
#define XT_STD_ERR_ENOPROTOOPT			ENOPROTOOPT
#define XT_STD_ERR_ENOSR				ENOSR
#define XT_STD_ERR_ENOSTR				ENOSTR
#define XT_STD_ERR_ENOTCONN			ENOTCONN
#define XT_STD_ERR_ENOTRECOVERABLE		ENOTRECOVERABLE
#define XT_STD_ERR_ENOTSOCK			ENOTSOCK
#define XT_STD_ERR_ENOTSUP				ENOTSUP
#define XT_STD_ERR_EOPNOTSUPP			EOPNOTSUPP
#define XT_STD_ERR_EOTHER				EOTHER
#define XT_STD_ERR_EOVERFLOW			EOVERFLOW
#define XT_STD_ERR_EOWNERDEAD			EOWNERDEAD
#define XT_STD_ERR_EPROTO				EPROTO
#define XT_STD_ERR_EPROTONOSUPPORT		EPROTONOSUPPORT
#define XT_STD_ERR_EPROTOTYPE			EPROTOTYPE
#define XT_STD_ERR_ETIME				ETIME
#define XT_STD_ERR_ETIMEDOUT			ETIMEDOUT
#define XT_STD_ERR_ETXTBSY				ETXTBSY
#define XT_STD_ERR_EWOULDBLOCK			EWOULDBLOCK

#elif defined(__WINDOWS__) /* WINDOWS errno */
#define XT_STD_ERR_EPERM			
#define XT_STD_ERR_ENOENT			ENOENT	/*No such file or directory*/
#define XT_STD_ERR_ESRCH			
#define XT_STD_ERR_EINTR			WSAEINTR
#define XT_STD_ERR_EIO				
#define XT_STD_ERR_ENXIO			
#define XT_STD_ERR_E2BIG			
#define XT_STD_ERR_ENOEXEC			
#define XT_STD_ERR_EBADF			WSAEBADF
#define XT_STD_ERR_ECHILD			
#define XT_STD_ERR_EAGAIN			WSATRY_AGAIN
#define XT_STD_ERR_ENOMEM			ENOMEM
#define XT_STD_ERR_EACCES			WSAEACCES
#define XT_STD_ERR_EFAULT			WSAEFAULT
#define XT_STD_ERR_EBUSY			
#define XT_STD_ERR_EEXIST			
#define XT_STD_ERR_EXDEV			
#define XT_STD_ERR_ENODEV			
#define XT_STD_ERR_ENOTDIR			
#define XT_STD_ERR_EISDIR	
#define XT_STD_ERR_EINVAL			EINVAL
#define XT_STD_ERR_ENFILE			
#define XT_STD_ERR_EMFILE			WSAEMFILE
#define XT_STD_ERR_ENOTTY			
#define XT_STD_ERR_EFBIG			
#define XT_STD_ERR_ENOSPC			
#define XT_STD_ERR_ESPIPE			
#define XT_STD_ERR_EROFS			
#define XT_STD_ERR_EMLINK			
#define XT_STD_ERR_EPIPE			
#define XT_STD_ERR_EDOM			
#define XT_STD_ERR_EDEADLK			
#define XT_STD_ERR_ENAMETOOLONG	
#define XT_STD_ERR_ENOLCK			
#define XT_STD_ERR_ENOSYS			
#define XT_STD_ERR_ENOTEMPTY		WSAENOTEMPTY
	/* POSIX SUPPLEMENT */
#define XT_STD_ERR_EADDRINUSE			WSAEADDRINUSE
#define XT_STD_ERR_EADDRNOTAVAIL		WSAEADDRNOTAVAIL
#define XT_STD_ERR_EAFNOSUPPORT		WSAEAFNOSUPPORT
#define XT_STD_ERR_EALREADY			WSAEALREADY
#define XT_STD_ERR_EBADMSG				
#define XT_STD_ERR_ECANCELED			WSAECANCELLED
#define XT_STD_ERR_ECONNABORTED		WSAECONNABORTED
#define XT_STD_ERR_ECONNREFUSED		WSAECONNREFUSED
#define XT_STD_ERR_ECONNRESET			WSAECONNRESET
#define XT_STD_ERR_EDESTADDRREQ		WSAEDESTADDRREQ	
#define XT_STD_ERR_EHOSTUNREACH		WSAEHOSTUNREACH
#define XT_STD_ERR_EIDRM				
#define XT_STD_ERR_EINPROGRESS			WSAEINPROGRESS
#define XT_STD_ERR_EISCONN				WSAEISCONN
#define XT_STD_ERR_ELOOP				WSAELOOP
#define XT_STD_ERR_EMSGSIZE			WSAEMSGSIZE
#define XT_STD_ERR_ENETDOWN			WSAENETDOWN
#define XT_STD_ERR_ENETRESET			WSAENETRESET
#define XT_STD_ERR_ENETUNREACH			WSAENETUNREACH
#define XT_STD_ERR_ENOBUFS				WSAENOBUFS
#define XT_STD_ERR_ENODATA				
#define XT_STD_ERR_ENOLINK				
#define XT_STD_ERR_ENOMSG				
#define XT_STD_ERR_ENOPROTOOPT			WSAENOPROTOOPT
#define XT_STD_ERR_ENOSR				
#define XT_STD_ERR_ENOSTR				
#define XT_STD_ERR_ENOTCONN			WSAENOTCONN
#define XT_STD_ERR_ENOTRECOVERABLE		
#define XT_STD_ERR_ENOTSOCK			WSAENOTSOCK
#define XT_STD_ERR_ENOTSUP				
#define XT_STD_ERR_EOPNOTSUPP			WSAEOPNOTSUPP
#define XT_STD_ERR_EOTHER				
#define XT_STD_ERR_EOVERFLOW			
#define XT_STD_ERR_EOWNERDEAD			
#define XT_STD_ERR_EPROTO				
#define XT_STD_ERR_EPROTONOSUPPORT		WSAEPROTONOSUPPORT
#define XT_STD_ERR_EPROTOTYPE			WSAEPROTOTYPE
#define XT_STD_ERR_ETIME				
#define XT_STD_ERR_ETIMEDOUT			WSAETIMEDOUT
#define XT_STD_ERR_ETXTBSY				
#define XT_STD_ERR_EWOULDBLOCK			WSAEWOULDBLOCK
#endif

/**
 * XT_OS_START_ERROR is where the APR specific error values start.
 */
#define XT_OS_START_ERROR				(20000)

/**
 * XT_OS_ERRSPACE_SIZE is the maximum number of errors you can fit
 *    into one of the error/status ranges below -- except for
 *    XT_OS_START_USERERR, which see.
 */
#define XT_OS_ERRSPACE_SIZE 50000
/**
 * XT_UTIL_ERRSPACE_SIZE is the size of the space that is reserved for
 * use within apr-util. This space is reserved above that used by APR
 * internally.
 * @note This number MUST be smaller than XT_OS_ERRSPACE_SIZE by a
 *       large enough amount that APR has sufficient room for its
 *       codes.
 */
#define XT_UTIL_ERRSPACE_SIZE 20000
/**
 * XT_OS_START_STATUS is where the APR specific status codes start.
 */
#define XT_OS_START_STATUS    (XT_OS_START_ERROR + XT_OS_ERRSPACE_SIZE)
/**
 * XT_UTIL_START_STATUS is where APR-Util starts defining its
 * status codes.
 */
#define XT_UTIL_START_STATUS   (XT_OS_START_STATUS + \
                           (XT_OS_ERRSPACE_SIZE - XT_UTIL_ERRSPACE_SIZE))
/**
 * XT_OS_START_USERERR are reserved for applications that use APR that
 *     layer their own error codes along with APR's.  Note that the
 *     error immediately following this one is set ten times farther
 *     away than usual, so that users of apr have a lot of room in
 *     which to declare custom error codes.
 *
 * In general applications should try and create unique error codes. To try
 * and assist in finding suitable ranges of numbers to use, the following
 * ranges are known to be used by the listed applications. If your
 * application defines error codes please advise the range of numbers it
 * uses to dev@apr.apache.org for inclusion in this list.
 *
 * Ranges shown are in relation to XT_OS_START_USERERR
 *
 * Subversion - Defined ranges, of less than 100, at intervals of 5000
 *              starting at an offset of 5000, e.g.
 *               +5000 to 5100,  +10000 to 10100
 *
 * Apache HTTPD - +2000 to 2999
 */
#define XT_OS_START_USERERR    (XT_OS_START_STATUS + XT_OS_ERRSPACE_SIZE)
/**
 * XT_OS_START_USEERR is obsolete, defined for compatibility only.
 * Use XT_OS_START_USERERR instead.
 */
#define XT_OS_START_USEERR     XT_OS_START_USERERR
/**
 * XT_OS_START_CANONERR is where APR versions of errno values are defined
 *     on systems which don't have the corresponding errno.
 */
#define XT_OS_START_CANONERR  (XT_OS_START_USERERR \
                                 + (XT_OS_ERRSPACE_SIZE * 10))
/**
 * XT_OS_START_EAIERR folds EAI_ error codes from getaddrinfo() into
 *     XT_status_t values.
 */
#define XT_OS_START_EAIERR    (XT_OS_START_CANONERR + XT_OS_ERRSPACE_SIZE)
/**
 * XT_OS_START_SYSERR folds platform-specific system error values into
 *     XT_status_t values.
 */
#define XT_OS_START_SYSERR    (XT_OS_START_EAIERR + XT_OS_ERRSPACE_SIZE)

/** @see XT_STATUS_IS_ENOSTAT */
#define XT_ENOSTAT        (XT_OS_START_ERROR + 1)
/** @see XT_STATUS_IS_ENOPOOL */
#define XT_ENOPOOL        (XT_OS_START_ERROR + 2)
/* empty slot: +3 */
/** @see XT_STATUS_IS_EBADDATE */
#define XT_EBADDATE       (XT_OS_START_ERROR + 4)
/** @see XT_STATUS_IS_EINVALSOCK */
#define XT_EINVALSOCK     (XT_OS_START_ERROR + 5)
/** @see XT_STATUS_IS_ENOPROC */
#define XT_ENOPROC        (XT_OS_START_ERROR + 6)
/** @see XT_STATUS_IS_ENOTIME */
#define XT_ENOTIME        (XT_OS_START_ERROR + 7)
/** @see XT_STATUS_IS_ENODIR */
#define XT_ENODIR         (XT_OS_START_ERROR + 8)
/** @see XT_STATUS_IS_ENOLOCK */
#define XT_ENOLOCK        (XT_OS_START_ERROR + 9)
/** @see XT_STATUS_IS_ENOPOLL */
#define XT_ENOPOLL        (XT_OS_START_ERROR + 10)
/** @see XT_STATUS_IS_ENOSOCKET */
#define XT_ENOSOCKET      (XT_OS_START_ERROR + 11)
/** @see XT_STATUS_IS_ENOTHREAD */
#define XT_ENOTHREAD      (XT_OS_START_ERROR + 12)
/** @see XT_STATUS_IS_ENOTHDKEY */
#define XT_ENOTHDKEY      (XT_OS_START_ERROR + 13)
/** @see XT_STATUS_IS_EGENERAL */
#define XT_EGENERAL       (XT_OS_START_ERROR + 14)
/** @see XT_STATUS_IS_ENOSHMAVAIL */
#define XT_ENOSHMAVAIL    (XT_OS_START_ERROR + 15)
/** @see XT_STATUS_IS_EBADIP */
#define XT_EBADIP         (XT_OS_START_ERROR + 16)
/** @see XT_STATUS_IS_EBADMASK */
#define XT_EBADMASK       (XT_OS_START_ERROR + 17)
/* empty slot: +18 */
/** @see XT_STATUS_IS_EDSOPEN */
#define XT_EDSOOPEN       (XT_OS_START_ERROR + 19)
/** @see XT_STATUS_IS_EABSOLUTE */
#define XT_EABSOLUTE      (XT_OS_START_ERROR + 20)
/** @see XT_STATUS_IS_ERELATIVE */
#define XT_ERELATIVE      (XT_OS_START_ERROR + 21)
/** @see XT_STATUS_IS_EINCOMPLETE */
#define XT_EINCOMPLETE    (XT_OS_START_ERROR + 22)
/** @see XT_STATUS_IS_EABOVEROOT */
#define XT_EABOVEROOT     (XT_OS_START_ERROR + 23)
/** @see XT_STATUS_IS_EBADPATH */
#define XT_EBADPATH       (XT_OS_START_ERROR + 24)
/** @see XT_STATUS_IS_EPATHWILD */
#define XT_EPATHWILD      (XT_OS_START_ERROR + 25)
/** @see XT_STATUS_IS_ESYMNOTFOUND */
#define XT_ESYMNOTFOUND   (XT_OS_START_ERROR + 26)
/** @see XT_STATUS_IS_EPROC_UNKNOWN */
#define XT_EPROC_UNKNOWN  (XT_OS_START_ERROR + 27)
/** @see XT_STATUS_IS_ENOTENOUGHENTROPY */
#define XT_ENOTENOUGHENTROPY (XT_OS_START_ERROR + 28)
/** @} */

/**
 * @addtogroup XT_STATE
 * @{
 */
/** @see XT_STATUS_IS_INCHILD */
#define XT_INCHILD        (XT_OS_START_ERROR + 1)
/** @see XT_STATUS_IS_INPARENT */
#define XT_INPARENT       (XT_OS_START_ERROR + 2)
/** @see XT_STATUS_IS_DETACH */
#define XT_DETACH         (XT_OS_START_ERROR + 3)
/** @see XT_STATUS_IS_NOTDETACH */
#define XT_NOTDETACH      (XT_OS_START_ERROR + 4)
/** @see XT_STATUS_IS_CHILD_DONE */
#define XT_CHILD_DONE     (XT_OS_START_ERROR + 5)
/** @see XT_STATUS_IS_CHILD_NOTDONE */
#define XT_CHILD_NOTDONE  (XT_OS_START_ERROR + 6)
/** @see XT_STATUS_IS_TIMEUP */
#define XT_TIMEUP         (XT_OS_START_ERROR + 7)
/** @see XT_STATUS_IS_INCOMPLETE */
#define XT_INCOMPLETE     (XT_OS_START_ERROR + 8)
/* empty slot: +9 */
/* empty slot: +10 */
/* empty slot: +11 */
/** @see XT_STATUS_IS_BADCH */
#define XT_BADCH          (XT_OS_START_ERROR + 12)
/** @see XT_STATUS_IS_BADARG */
#define XT_BADARG         (XT_OS_START_ERROR + 13)
/** @see XT_STATUS_IS_EOF */
#define XT_EOF            (XT_OS_START_ERROR + 14)
/** @see XT_STATUS_IS_NOTFOUND */
#define XT_NOTFOUND       (XT_OS_START_ERROR + 15)
/* empty slot: +16 */
/* empty slot: +17 */
/* empty slot: +18 */
/** @see XT_STATUS_IS_ANONYMOUS */
#define XT_ANONYMOUS      (XT_OS_START_ERROR + 19)
/** @see XT_STATUS_IS_FILEBASED */
#define XT_FILEBASED      (XT_OS_START_ERROR + 20)
/** @see XT_STATUS_IS_KEYBASED */
#define XT_KEYBASED       (XT_OS_START_ERROR + 21)
/** @see XT_STATUS_IS_EINIT */
#define XT_EINIT          (XT_OS_START_ERROR + 22)
/** @see XT_STATUS_IS_ENOTIMPL */
#define XT_ENOTIMPL       (XT_OS_START_ERROR + 23)
/** @see XT_STATUS_IS_EMISMATCH */
#define XT_EMISMATCH      (XT_OS_START_ERROR + 24)
/** @see XT_STATUS_IS_EBUSY */
#define XT_EBUSY          (XT_OS_START_ERROR + 25)
/** @} */

#define XT_FROM_OS_ERROR(e) ((e)==0 ? FUN_RET_OK : (e) + XT_OS_START_SYSERR)

/**
 * @defgroup xt_errno Error Codes
 * @ingroup Xtrovert
 * @{
 */

/**
 * Type for specifying an error or status code.
 */
typedef int			xt_state_t;

#if defined(__LINUX__)
	#define xt_get_os_error()    (errno)
#elif defined(__WINDOWS__)
	#define xt_get_os_error()   (XT_FROM_OS_ERROR(GetLastError()))
#endif

#ifdef __cplusplus
}
#endif

#endif /*__XT_ERROR_H__*/
