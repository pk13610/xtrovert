/************************************************************************************
* xt_net_msgbuf.h : header file
*
* Send/Receive msessage buffer Definition header
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
#ifndef __XT_NET_MSGBUF_H__
#define __XT_NET_MSGBUF_H__

#include "xt_define.h"

#ifdef __cplusplus
extern "C" {
#endif

#define XT_NET_MSG_BUF_SIZE_TCP		((xt_size_t)(_MEM_PAGE_SIZE_DEFAULT_))
#define XT_NET_MSG_BUF_SIZE_UDP		((xt_size_t)(64*1024)) /* 64k */

typedef unsigned short bytes_counter_t;

typedef struct _tag_xt_net_msgbuf_t
{
	xt_size_t			capacity;
	xt_size_t			head;
	bytes_counter_t		read_bytes;
	xt_size_t			tail;
	bytes_counter_t		write_bytes;
	char				buf[0];
}xt_net_msgbuf_t;

/* Be careful to call this function. */
void				xt_net_msgbuf_clear( xt_net_msgbuf_t *msgbuf );

xt_net_msgbuf_t*	xt_net_msgbuf_create( xt_size_t bytes );
void				xt_net_msgbuf_release( xt_net_msgbuf_t *msgbuf );

char*				xt_net_msgbuf_getwrite( xt_net_msgbuf_t *msgbuf, xt_size_t *buf_len );
void				xt_net_msgbuf_writeend( xt_net_msgbuf_t *msgbuf, xt_size_t bytes );

char*				xt_net_msgbuf_getread( xt_net_msgbuf_t *msgbuf, xt_size_t *buf_len );
void				xt_net_msgbuf_readend( xt_net_msgbuf_t *msgbuf, xt_size_t bytes );

#ifdef __cplusplus
}
#endif

#endif /*__XT_NET_MSGBUF_H__*/
