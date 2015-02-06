/************************************************************************************
*	xt_io_api.c: Implementation File
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

#include "xt_io_api.h"

#if defined(__LINUX__)
int xt_epoll_create( INT size )
{
	return epoll_create( size );
}

int xt_epoll_ctl( int epoll_fd, int op, int fd, struct epoll_event *ev )
{
	/*
	EPOLLIN ：表示对应的文件描述符可以读（包括对端SOCKET正常关闭）;
	EPOLLOUT：表示对应的文件描述符可以写;
	EPOLLPRI：表示对应的文件描述符有紧急的数据可读（这里应该表示有带外数据到来）;
	EPOLLERR：表示对应的文件描述符发生错误;
	EPOLLHUP：表示对应的文件描述符被挂断;
	EPOLLET： 将EPOLL设为边缘触发(Edge Triggered)模式，这是相对于水平触发(Level Triggered)来说的;
	EPOLLONESHOT：只监听一次事件，当监听完这次事件之后，如果还需要继续监听这个socket的话，需要再次把这个socket加入到EPOLL队列里;
	*/
	/* EPOLL_CTL_ADD EPOLL_CTL_DEL EPOLL_CTL_MOD */
	return epoll_ctl(epoll_fd, op, fd, ev);
}

int xt_epoll_wait( int epoll_fd, struct epoll_event *ev, int max_events, int tmout )
{
	return epoll_wait( epoll_fd, ev, max_events, tmout );
}
#elif defined(__WINDOWS__)
#endif /*defined(__WINDOWS__)*/

int xt_select( UINT fd_max, FD_SET *fd_read, FD_SET *fd_write, FD_SET *fd_exception, const struct timeval *tmout )
{
	return select( fd_max, fd_read, fd_write, fd_exception, tmout );
}
