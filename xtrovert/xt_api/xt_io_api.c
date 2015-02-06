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
	EPOLLIN ����ʾ��Ӧ���ļ����������Զ��������Զ�SOCKET�����رգ�;
	EPOLLOUT����ʾ��Ӧ���ļ�����������д;
	EPOLLPRI����ʾ��Ӧ���ļ��������н��������ݿɶ�������Ӧ�ñ�ʾ�д������ݵ�����;
	EPOLLERR����ʾ��Ӧ���ļ���������������;
	EPOLLHUP����ʾ��Ӧ���ļ����������Ҷ�;
	EPOLLET�� ��EPOLL��Ϊ��Ե����(Edge Triggered)ģʽ�����������ˮƽ����(Level Triggered)��˵��;
	EPOLLONESHOT��ֻ����һ���¼���������������¼�֮���������Ҫ�����������socket�Ļ�����Ҫ�ٴΰ����socket���뵽EPOLL������;
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
