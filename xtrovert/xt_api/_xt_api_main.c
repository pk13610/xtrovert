/************************************************************************************
* _xt_api_main.c: Implementation File
*
* DESCRIPTION  :   Unit-Test code
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

#include <stdio.h>
#include <assert.h>
#include "xt_thread_api.h"
#include "xt_net_api.h"
#include "xt_file_api.h"

#if defined(__WINDOWS__)
/* warning C4100: 'XXX' : unreferenced formal parameter */
#pragma warning (disable: 4100)
#endif

int g = 0;

void *XT_THREAD_FUNC fn_thread( void *arg )
{
	socket_fd_t sock;

	_tprintf( _T("%d\n"), g++ );
	sock = xt_socket( AF_INET, SOCK_STREAM, 0 );
	if ( sock == -1 )
	{
		_tprintf( _T("sock:%d\n"), SktGetLastError() );
	}

	return 0;
}

int xt_api_main( int argc, TCHAR *argv[] )
{
// 	xt_threadattr_t attr;
// 	xt_thread_t tid = INVALID_HANDLE_VALUE;
// 	int ret = 0;
// 	g = 0;
// 	
// 	PrepareWinSock();
// 
// 	ret = xt_threadattr_init( &attr );
// 	_tprintf( _T("xt_threadattr_init ERROR: %d\n"), ret );
// 
// 	attr.detachstate = 1;
// 	for ( ret=0; ret<100; ++ret )
// 		xt_thread_create( &tid, &attr, fn_thread, NULL);
// 
// 	_tprintf( _T("ERROR: %d\n"), ret );
// 
// 	CleanWinSock();
	return 0;
}
