/************************************************************************************
*	xafx_msg.cpp: Implementation File
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

#include "xafx/xafx_msg.h"
#include "xt_mem_api.h"

#if defined(__WINDOWS__)
/* warning C4100: 'XXX' : unreferenced formal parameter */
#pragma warning (disable: 4100)
#pragma warning (disable: 4200) /* Ignore:  warning C4200: nonstandard extension used : zero-sized array in struct/union */
#pragma warning (disable: 4189) /* local variable is initialized but not referenced */
#endif

#define xt_counter_max					(((xt_size_t)(~0))>>1) /*01111111,11111111*/
#define xt_ovflow_mask					(~(xt_counter_max)) /* 10000000,00000000 */
#define xt_counter_overflow(c)			(xt_ovflow_mask&(c))

#define xafx_msg_que_len(q_ptr)			(((q_ptr)->tail_idx+(q_ptr)->capacity-(q_ptr)->head_idx)%(q_ptr)->capacity)

#define xafx_msg_que_reset_overflow(q_ptr)							\
				do													\
				{													\
					(q_ptr)->read_counter &= xt_counter_max;		\
					(q_ptr)->write_counter &= xt_counter_max;		\
				} while (0)

/* Be careful to call this function. */
void xafx_msgque_clear( xafx_msg_que_t *q )
{
	q->head_idx = 0;
	q->tail_idx = 0;
	q->read_counter = 0;
	q->write_counter = 0;
}

xafx_msg_que_t* xafx_msgque_create( xt_size_t capacity )
{
	xafx_msg_que_t *q = NULL;

	q = (xafx_msg_que_t*)xt_mem_alloc(xafx_msg_que_calc_size(capacity));

	if ( q == NULL )
	{
		return NULL;
	}

	xafx_msgque_clear( q );
	q->capacity = capacity;

	return (q);
}

void xafx_msgque_destroy( xafx_msg_que_t *q )
{
	xafx_msgque_clear( q );
	xt_free( q );
}

xafx_msg_t* xafx_msgque_get_write_msg( xafx_msg_que_t *q )
{
	xt_size_t tmp_head = q->head_idx;

	if ( tmp_head == q->tail_idx ) /* Queue is full or empty */
	{
		if ( xt_counter_overflow(q->write_counter) )
		{
			return NULL; /* Waiting until read_counter gets overflow. */
		}
		/* if full */
		if ( q->read_counter < q->write_counter )
		{
			return NULL;
		}
		/* else empty queue, OK! */
	}

	return &(q->msg[q->tail_idx]);
}

void xafx_msgque_writeend( xafx_msg_que_t *q )
{
	xt_size_t tmp_tail = q->tail_idx;

	++tmp_tail;
	tmp_tail %= q->capacity; /* in case overflow */

	++(q->write_counter);

	q->tail_idx = tmp_tail;
}

xafx_msg_t* xafx_msgque_get_read_msg( xafx_msg_que_t *q )
{
	xt_size_t tmp_tail = q->tail_idx;

	if ( tmp_tail == q->head_idx ) /* Queue is full or empty */
	{
		/* if empty */
		if ( q->read_counter >= q->write_counter )
		{
			if ( xt_counter_overflow(q->read_counter) ) /* q->write_counter must be overflow! */
			{
				xafx_msg_que_reset_overflow(q);
			}
			return NULL;
		}
		/* else if full, OK! */
	}

	return &(q->msg[q->head_idx]);
}

void xafx_msgque_readend( xafx_msg_que_t *q )
{
	/* Assumption: bytes > 0 && bytes<data_len */
	xt_size_t tmp_head = q->head_idx;

	++tmp_head;
	tmp_head %= q->capacity; /* in case overflow */

	++(q->read_counter);

	q->head_idx = tmp_head;
}
