/************************************************************************************
* _test_net.h : header file
*
* 
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
* 1 producer thread and 1 consumer thread.
* Create and release in consumer thread.
************************************************************************************/
#ifndef ___TEST_NET_H__
#define ___TEST_NET_H__

#include "xt_net_api.h"
//////////////////////////////////////////////////////////////////////////
//#include <event2/event.h>
#include "net/reactor/XtReactor.h"
#include "net/reactor/XtEventHandler.h"
#include "net/reactor/XtEvtHndlrSig.h"
// #include "net/reactor/XtNetAcceptor.h"
// #include "net/reactor/XtNetConnector.h"
//////////////////////////////////////////////////////////////////////////

#include "XtThread.h"

#ifdef __cplusplus
extern "C" {
#endif

//////////////////////////////////////////////////////////////////////////
// // UDP
// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>
// #include <sys/socket.h>
// #include <netinet/in.h>
// #include <arpa/inet.h>
// #include <event.h>
// #include <event2/listener.h>
// 
// #define SVR_IP                          "127.0.0.1"
// #define SVR_PORT                        10000
// #define BUF_SIZE                        1024
// 
// void read_cb(int fd, short event, void *arg) {
// 	char                buf[BUF_SIZE];
// 	int                 len;
// 	int                 size = sizeof(struct sockaddr);
// 	struct sockaddr_in  client_addr;
// 
// 	memset(buf, 0, sizeof(buf));
// 	len = recvfrom(fd, buf, sizeof(buf), 0, (struct sockaddr *)&client_addr, &size);
// 
// 	if (len == -1) {
// 		perror("recvfrom()");
// 	} else if (len == 0) {
// 		printf("Connection Closed\n");
// 	} else {
// 		printf("Read: len [%d] - content [%s]\n", len, buf);
// 
// 		/* Echo */
// 		sendto(fd, buf, len, 0, (struct sockaddr *)&client_addr, size);
// 	}
// }
// 
// int bind_socket(struct event *ev) {
// 	int                 sock_fd;
// 	int                 flag = 1;
// 	struct sockaddr_in  sin;
// 
// 	/* Create endpoint */
// 	if ((sock_fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
// 		perror("socket()");
// 		return -1;
// 	}
// 
// 	/* Set socket option */
// 	if (setsockopt(sock_fd, SOL_SOCKET, SO_REUSEADDR, &flag, sizeof(int)) < 0) {
// 		perror("setsockopt()");
// 		return 1;
// 	}
// 
// 	/* Set IP, port */
// 	memset(&sin, 0, sizeof(sin));
// 	sin.sin_family = AF_INET;
// 	sin.sin_addr.s_addr = inet_addr(SVR_IP);
// 	sin.sin_port = htons(SVR_PORT);
// 
// 	/* Bind */
// 	if (bind(sock_fd, (struct sockaddr *)&sin, sizeof(struct sockaddr)) < 0) {
// 		perror("bind()");
// 		return -1;
// 	} else {
// 		printf("bind() success - [%s] [%u]\n", SVR_IP, SVR_PORT);
// 	}
// 
// 	/* Init one event and add to active events */
// 	event_set(ev, sock_fd, EV_READ | EV_PERSIST, &read_cb, NULL);
// 	if (event_add(ev, NULL) == -1) {
// 		printf("event_add() failed\n");
// 	}
// 
// 	return 0;
// }
// 
// int main(void) {
// 	struct event  ev;
// 
// 	/* Init. event */
// 	if (event_init() == NULL) {
// 		printf("event_init() failed\n");
// 		return -1;
// 	}
// 
// 	/* Bind socket */
// 	if (bind_socket(&ev) != 0) {
// 		printf("bind_socket() failed\n");
// 		return -1;
// 	}
// 
// 	/* Enter event loop */
// 	event_dispatch();
// 
// 	return 0;
// }
//////////////////////////////////////////////////////////////////////////


// class CTestConn : public CXtNetConnector
// {
// public:
// 	CTestConn()
// 	{
// 		m_bWriteDisable = FALSE;
// 
// 		m_bReadyRecv = TRUE;
// 		m_bReadySend = TRUE;
// 
// 		sum = 0;
// 		m_buf[0] = 0;
// 	}
// protected:
// 
// 	int OnRecv(void)
// 	{
// 		int bytes = 0;
// 
// 		bytes = xt_recv( m_EventDsc.fd, m_buf+sum, 1024-sum, 0 );
// 		if ( bytes <= 0 )
// 		{
// 			return 0;
// 		}
// 
// // 		while ( bytes > 0 )
// // 		{
// // 			bytes = xt_recv( m_event_info.fd, m_buf+sum, 1024-sum, 0 );
// // 			sum += bytes;
// // 		}
// 		
// // 		if ( bytes <= 0 )
// // 		{
// // 			_tprintf( _T("Recv Error: %d\n"), SktGetLastError() );
// // 			m_pReactor->RemoveHandler( this );
// // 			return 0;
// // 		}
// 
// 		xt_send( m_event_info.fd, m_buf+sum, bytes, 0 );
// 
// 		if ( sum >= 1024 )
// 		{
// 			sum = 0;
// 		}
// 		sum += bytes;
// 		m_buf[sum] = 0;
// 		_tprintf( _T("%s"), m_buf );
// 		return FUN_RET_OK;
// 	}
// 
// 	int OnSend(void)
// 	{
// 		if ( m_bWriteDisable )
// 		{
// 			return FUN_RET_OK;
// 		}
// 		m_bWriteDisable = TRUE;
// 		xt_send( m_event_info.fd, SEND_MSG, _tcslen(SEND_MSG), 0 );
// 
// 		_tprintf( _T("Send:%s"), SEND_MSG );
// 		return FUN_RET_OK;
// 	}
// 
// 	int OnError( int nErroCode )
// 	{
// 		return FUN_RET_OK;
// 	}
// 
// 	char			m_buf[1024];
// 	int				sum;
// 	BOOL			m_bWriteDisable;
// };
// 
// class CTestPool : public CXtConnPool
// {
// public:
// 
// protected:
// 
// 	DIRECT_BIND_BOJECT(CTestConn)
// };
// 
// enum net_msg_type
// {
// 	NOTIFY_NEW_MSG = 0,
// 	NOTIFY_NEW_CONN,
// 	NOTIFY_LOST_CONN,
// 
// 	NOTIFY_END,
// };
// 
// typedef struct _tag_net_notify_data_t
// {
// 	union
// 	{
// 		char			*buf;
// 		CXtEventHandler	*evt_handler;
// 	};
// 
// 	xt_size_t		count;
// }net_notify_data_t;
// 
// typedef struct _tag_net_notify_msg_t
// {
// 	int					msg_type;
// 	CXtEventHandler		*owner;
// 	net_notify_data_t	data;
// }net_notify_msg_t;
// 
// typedef struct _tag_net_msg_que_t
// {
// 	xt_size_t			capacity;
// 	xt_size_t			head;
// 	xt_size_t			tail; /* (head==tail) <==> full/empty */
// 	int					full; /* full == 1 */
// 	net_notify_msg_t	msg_buf[0];
// }net_msg_que_t;
// 
// #define msg_que_push(msg_que_p)			((++((msg_que_p)->tail))%=capacity)
// #define msg_que_pop(msg_que_p)			((++((msg_que_p)->head))%=capacity)
// #define msg_que_isfull(msg_que_p)		(full==1)
// #define msg_que_isempty(msg_que_p)		((((msg_que_p)->head==(msg_que_p)->tail)&&(full==0))


#define	SEND_MSG			_T("Hello world!\n")

#ifdef __cplusplus
}
#endif

extern void StartNetServer( NET_SKT_TYPE skt_type, const TCHAR *szLocalAddr );
extern void StartNetClient( NET_SKT_TYPE skt_type, const TCHAR *szPeerAddr );

class __CTestNetThread : public CXtThread
{
public:
	__CTestNetThread(void)
	{

	}

	virtual ~__CTestNetThread(void)
	{

	}
protected:

	virtual int					Execute(void)
	{
#define _NET_SVR_ADDR_STR_					_T("127.0.0.1:9995")
//#define _NET_SVR_ADDR_STR_					_T("0.0.0.0:9995")
#define _NET_CLIENT_ADDR_STR_				_T("0.0.0.0:9995")

		if ( *((TCHAR*)m_pParam) == _T('s') )
		{
			StartNetServer( NET_SKT_TYPE_TCP, _NET_SVR_ADDR_STR_ );
		}
		else
		{
			StartNetClient( NET_SKT_TYPE_TCP, _NET_SVR_ADDR_STR_ );
		}

		m_bStop = TRUE;

		return FUN_RET_OK;
	}

private:
};

#endif /*___TEST_NET_H__*/
