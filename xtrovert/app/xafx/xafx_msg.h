/************************************************************************************
* xafx_msg.h : header file
*
* Definition header, An Application framework for common use
*			Xtrovert Application Frameworks (xafx, the last 'x' means nothing).
*
* AUTHOR	:	Sean Feng <SeanFeng2006@hotmail.com>
* DATE		:	Aug. 7, 2012
* Copyright (c) 2009-?. All Rights Reserved.
*
* This code may be used in compiled form in any way you desire. This
* file may be redistributed unmodified by any means PROVIDING it is 
* not sold for profit without the authors written consent, and 
* providing that this notice and the authors name and all copyright 
* notices remains int act. 
*
* An email letting me know how you are using it would be nice as well. 
*
* This file is provided "as is" with no expressed or implied warranty.
* The author accepts no liability for any damage/loss of business that
* this product may cause.
*
************************************************************************************/
#ifndef __X_AFX_MSG_H__
#define __X_AFX_MSG_H__

#include "XtObjPool.h"

/*
*	Message structure
*/
typedef struct _tag_xafx_msg_t
{
	HANDLE			hndl;
	UINT			msg;
	WPARAM			wParam;
	LPARAM			lParam;
	DWORD			time;
	POINT			pt;
}xafx_msg_t;

typedef struct _tag_xafx_msg_que_t
{
	xt_size_t			capacity; /* The quantity of messages that could be put in this queue at most. */
	xt_size_t			head_idx;
	xt_size_t			read_counter;
	xt_size_t			tail_idx;
	xt_size_t			write_counter;
	xafx_msg_t			msg[0];
}xafx_msg_que_t;

#define xafx_msg_que_calc_size(c)		(sizeof(xafx_msg_que_t)+(c)*sizeof(xafx_msg_t))

/* Be careful to call this function. */
extern "C" void xafx_msgque_clear( xafx_msg_que_t *q );

extern "C" xafx_msg_que_t* xafx_msgque_create( xt_size_t capacity );
extern "C" void xafx_msgque_destroy( xafx_msg_que_t *q );

extern "C" xafx_msg_t* xafx_msgque_get_write_msg( xafx_msg_que_t *q );
extern "C" void xafx_msgque_writeend( xafx_msg_que_t *q );

extern "C" xafx_msg_t* xafx_msgque_get_read_msg( xafx_msg_que_t *q );
extern "C" void xafx_msgque_readend( xafx_msg_que_t *q );

#endif /*__X_AFX_MSG_H__*/
