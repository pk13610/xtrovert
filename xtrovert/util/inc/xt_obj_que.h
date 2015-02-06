/************************************************************************************
* xt_obj_que.h : header file
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
#ifndef __XT_OBJ_QUE_H__
#define __XT_OBJ_QUE_H__

#include "xt_define.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _tag_xt_obj_que_t
{
	xt_size_t			capacity; /* The quantity of objects that could be put in this queue at most. */
	int					elem_size;
	int					unit_size; /* = (elem_size%(XT_PACK_BYTES)==0)?elem_size:((elem_size+XT_PACK_BYTES)/XT_PACK_BYTES) */
	xt_size_t			head; /* index for each unit */
	xt_size_t			read_counter;
	xt_size_t			tail; /* index for each unit */
	xt_size_t			write_counter;
	char				buf[0];
}xt_obj_que_t;

/* Be careful to call this function. */
void xt_objque_clear( xt_obj_que_t *q );

xt_obj_que_t* xt_objque_create( xt_size_t capacity, int obj_size );
void xt_objque_release( xt_obj_que_t *q );

void* xt_objque_get_write_buf( xt_obj_que_t *q, xt_size_t *obj_size );
void xt_objque_writeend( xt_obj_que_t *q );

void* xt_objque_get_read_buf( xt_obj_que_t *q, xt_size_t *obj_size );
void xt_objque_readend( xt_obj_que_t *q );

#ifdef __cplusplus
}
#endif

#endif /*__XT_OBJ_QUE_H__*/
