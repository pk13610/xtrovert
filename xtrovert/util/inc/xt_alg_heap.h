/************************************************************************************
* xt_alg_heap.h : header file
*
* header
*
* AUTHOR	:	Sean Feng <SeanFeng2006@hotmail.com>
* DATE		:	Nov. 7, 2009
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
************************************************************************************/
#ifndef __XT_ALG_HEAP_H__
#define __XT_ALG_HEAP_H__

#include "xt_alg_define.h"

#ifdef __cplusplus
extern "C" {
#endif

/* MAX-HEAP by default */

typedef xt_sort_elem_t	xt_heap_elem_t;

typedef struct _tag_xt_heap_t
{
	xt_size_t			capacity;
	xt_size_t			count;
	xt_fn_sort_cmp_t	fn_cmp;
	xt_heap_elem_t		*arr;
}xt_heap_t;

#define xt_heap_mem_size_pre(_capacity)					(sizeof(xt_heap_t) + (_capacity)*sizeof(xt_heap_elem_t))
#define xt_heap_buf_addr(heap_p)						((char*)(heap_p)->arr)
#define xt_heap_bytes_max(heap_p)						(sizeof(xt_heap_elem_t)*(heap_p)->capacity)
#define xt_heap_buf_size(heap_p)						(sizeof(xt_heap_elem_t)*(heap_p)->count)

#define xt_is_heap_elem(heap_p,x_p)						(((char*)(x_p)-xt_heap_buf_addr(heap_p))%sizeof(xt_heap_elem_t) == 0)
#define xt_in_heap_range(heap_p,x_p)					((x_p)>=(heap_p)->arr && (char*)(x_p)<(xt_heap_buf_addr(heap_p)+xt_heap_buf_size(heap_p)))

/* Root-Element index = 0 */
#define xt_heap_idx_parent(idx)		(((idx)-1)>>1)	/* (i-1)/2 */
#define xt_heap_idx_lchild(idx)		(((idx)<<1)+1)	/* 2*i + 1 */
#define xt_heap_idx_rchild(idx)		(((idx)<<1)+2)	/* 2*i + 2 */

#define xt_heap_is_lchild(idx)		((idx)%2)		/* != 0 */
#define xt_heap_is_rchild(idx)		((idx)%2==0)

/* Depth starts from "0" */
/* Nodes max count in "_depth" */
#define xt_heap_nodes_on(_depth)	(2<<(_depth))

#define xt_heap_height(heap)	

xt_heap_t*		xt_heap_create( xt_size_t max_count, int (*fn_cmp)(const void*, const void*) );
void			xt_heap_release( xt_heap_t *heap );
int				xt_heap_push( xt_heap_t *heap, xt_heap_elem_t x );
int				xt_heap_erase( xt_heap_t *heap, xt_heap_elem_t *px );
int				xt_heap_erase_( xt_heap_t *heap, xt_heap_elem_t *px );
int				xt_heap_pop( xt_heap_t *heap, xt_heap_elem_t *px ); /* Popup element on top of this heap ( the mini/max one ) */
void			xt_heap_adjust( xt_heap_t *heap, xt_size_t idx );
int				xt_heap_build( xt_heap_t *heap );
int				xt_heap_sort( xt_heap_t *heap );

xt_heap_elem_t*	xt_heap_search( xt_heap_t *heap, xt_size_t idx_start, const xt_heap_elem_t *px );

void			xt_test_heap_sort( xt_heap_elem_t *arr, xt_size_t len );

#define xt_make_heap_elem_ptr(_p)			((xt_heap_elem_t*)(_p))

#ifdef __cplusplus
}
#endif

#endif /*__XT_ALG_HEAP_H__*/
