/************************************************************************************
* xt_io_api.h : header file
*
* IO-Model API Definition header
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
#ifndef __XT_IO_API_H__
#define __XT_IO_API_H__

#include "xt_define.h"

#ifdef __cplusplus
extern "C" {
#endif

/***********************************************************************
* Platform Independent Socket API Collection (exception based)
***********************************************************************/

/* epoll */
#if defined(__LINUX__)
	int				xt_epoll_create( int size );
	int				xt_epoll_ctl( int epoll_fd, int op, int fd, struct epoll_event *ev );
	int				xt_epoll_wait( int epoll_fd, struct epoll_event *ev, int max_events, int tmout );
#elif defined(__WINDOWS__)
/* IOCP */
/*
typedef struct _OVERLAPPED {
ULONG_PTR internal;
ULONG_PTR internalHigh;
union {
struct {
DWORD Offset;
DWORD OffsetHigh;
};

PVOID Pointer;
};

HANDLE hEvent;
} OVERLAPPED, *LPOVERLAPPED;


Port port = CreateIoCompletionPort(INVALID_HANDLE_VALUE, 0, 0, fixedThreadCount());
HANDLE WINAPI CreateIoCompletionPort(
__in          HANDLE FileHandle,
__in          HANDLE ExistingCompletionPort,
__in          ULONG_PTR CompletionKey,
__in          DWORD NumberOfConcurrentThreads
);

BOOL WINAPI GetQueuedCompletionStatus(
__in          HANDLE CompletionPort,
__out         LPDWORD lpNumberOfBytes,
__out         PULONG_PTR lpCompletionKey,
__out         LPOVERLAPPED* lpOverlapped,
__in          DWORD dwMilliseconds
);

PostQueuedCompletionStatus
*/

#endif /*__WINDOWS__*/

int				xt_select( UINT fd_max, FD_SET *fd_read, FD_SET *fd_write, FD_SET *fd_exception, const struct timeval *tmout );

#ifdef __cplusplus
}
#endif

#endif /*__XT_IO_API_H__*/
