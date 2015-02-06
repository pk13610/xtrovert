/************************************************************************************
*	xt_reactor_define.c: Implementation File
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

#include "xt_reactor_define.h"

// xt_event_fd_t xt_rat_socket( int family, int sock_type, int proto )
// {
// #if defined(__WINDOWS__)
// 	return WSASocket( family, sock_type, proto, NULL, 0, WSA_FLAG_OVERLAPPED); 
// /*#elif defined(__LINUX__)*/
// #else
// 	return xt_socket( family, sock_type, proto );
// #endif
// 
// 	return INVALID_SOCKET;
// }
// 
// xt_event_fd_t xt_rat_accept( xt_event_fd_t lsnr_fd, SOCKADDR *saddr, SOCKLEN_T *len )
// {
// 	xt_event_fd_t fd = INVALID_SOCKET;
// #if defined(__WINDOWS__)
// 	WSAAccept( lsnr_fd, saddr, len,  );
// /*#elif defined(__LINUX__)*/
// #else
// #endif
// 
// 	return fd;
// }
// 
// int xt_rat_recv( xt_event_fd_t fd, char *buf, int len )
// {
// 
// }
// 
// int xt_rat_send( xt_event_fd_t fd, const char *buf, int len )
// {
// 
// }
// 
// int xt_rat_recvfrom( xt_event_fd_t fd, char *buf, int len, const SOCKADDR *saddr, SOCKLEN_T len )
// {
// 
// }
// 
// int xt_rat_sendto( xt_event_fd_t fd, char *buf, int len, const SOCKADDR *saddr, SOCKLEN_T len )
// {
// 
// }
