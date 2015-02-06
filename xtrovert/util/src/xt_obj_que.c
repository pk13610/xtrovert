/************************************************************************************
* xt_file_api.c: Implementation File
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

#include "xt_obj_que.h"
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

#define xt_objque_calc_unit_size(obj_size)		((obj_size)%(XT_PACK_BYTES)==0)?(obj_size):(((obj_size)+XT_PACK_BYTES)/XT_PACK_BYTES);
#define xt_objque_calc_size(c,unit_size)		(sizeof(xt_obj_que_t)+(c)*(unit_size))
#define xt_objque_len(q_ptr)		(((q_ptr)->tail+(q_ptr)->capacity-(q_ptr)->head)%(q_ptr)->capacity)

#define xt_objque_elem_buf(q_ptr,idx)		((q_ptr)->buf + ((q_ptr)->idx)*((q_ptr)->unit_size))

#define xt_objque_reset_overflow(q_ptr)								\
				do													\
				{													\
					(q_ptr)->read_counter &= xt_counter_max;		\
					(q_ptr)->write_counter &= xt_counter_max;		\
				} while (0)

void xt_objque_clear( xt_obj_que_t *q )
{
	q->head = 0;
	q->tail = 0;
	q->read_counter = 0;
	q->write_counter = 0;
}

xt_obj_que_t* xt_objque_create( xt_size_t capacity, int obj_size )
{
	xt_obj_que_t *q = NULL;

	int unit_size = 0;

	unit_size = xt_objque_calc_unit_size(obj_size);
	
	q = (xt_obj_que_t*)xt_mem_alloc(xt_objque_calc_size(capacity,unit_size));

	if ( q )
	{
		xt_objque_clear( q );
		q->capacity = capacity;
		q->elem_size = obj_size;
		q->unit_size = unit_size;
	}

	return q;
}

void xt_objque_release( xt_obj_que_t *q )
{
	xt_objque_clear( q );

	xt_free( q );
}

void* xt_objque_get_write_buf( xt_obj_que_t *q, xt_size_t *obj_size )
{
	xt_size_t tmp_head = q->head;

	if ( tmp_head == q->tail ) /* Queue is full or empty */
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

	if ( obj_size )
	{
		*obj_size = q->elem_size;
	}

	return xt_objque_elem_buf(q, q->tail);
}

void xt_objque_writeend( xt_obj_que_t *q )
{
	xt_size_t tmp_tail = q->tail;

	++tmp_tail;
	tmp_tail %= q->capacity; /* in case overflow */

	++(q->write_counter);

	q->tail = tmp_tail;
}

void* xt_objque_get_read_buf( xt_obj_que_t *q, xt_size_t *obj_size )
{
	xt_size_t tmp_tail = q->tail;

	if ( tmp_tail == q->head ) /* Queue is full or empty */
	{
		/* if empty */
		if ( q->read_counter >= q->write_counter )
		{
			if ( xt_counter_overflow(q->read_counter) ) /* q->write_counter must be overflow! */
			{
				xt_objque_reset_overflow(q);
			}
			return NULL;
		}
		/* else if full, OK! */
	}

	if ( obj_size )
	{
		*obj_size = q->elem_size;
	}

	return xt_objque_elem_buf(q, q->head);
}

void xt_objque_readend( xt_obj_que_t *q )
{
	/* Assumption: bytes > 0 && bytes<data_len */
	xt_size_t tmp_head = q->head;

	++tmp_head;
	tmp_head %= q->capacity; /* in case overflow */

	++(q->read_counter);

	q->head = tmp_head;
}

/******************************************************************************************/
