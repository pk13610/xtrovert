/************************************************************************************
* xt_alg_heap.c: Implementation File
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

#include "xt_alg_heap.h"
#include "xt_mem_api.h"

#if defined(__WINDOWS__)
/* warning C4100: 'XXX' : unreferenced formal parameter */
#pragma warning (disable: 4100)
#endif

#define xt_sizeof_flexible_array(struct_type,arr_type,count)			(sizeof(struct_type) + count*sizeof(arr_type))

xt_heap_t* xt_heap_create( xt_size_t max_count, int (*fn_cmp)(const void*, const void*) )
{
	xt_heap_t *heap = NULL;

	if ( fn_cmp == NULL )
	{
		fn_cmp = xt_sort_cmp_def;
	}

	heap = (xt_heap_t*)xt_malloc( xt_sizeof_flexible_array(xt_heap_t,xt_heap_elem_t,max_count) );
	if ( heap )
	{
		heap->capacity = max_count;
		heap->count = 0;
		heap->fn_cmp = fn_cmp;
		heap->arr = (xt_heap_elem_t*)(&(heap->arr)+sizeof(void*));
		return heap;
	}

	return NULL;
}

void xt_heap_release( xt_heap_t *heap )
{
	xt_free(heap);
}

int xt_heap_push( xt_heap_t *heap, xt_heap_elem_t x )
{
	if ( heap->count == heap->capacity )
	{
		return -1; /* Full */
	}

	heap->arr[heap->count] = x;
	++heap->count;

	xt_heap_build( heap );

	return FUN_RET_OK;
}

int xt_heap_erase( xt_heap_t *heap, xt_heap_elem_t *px )
{
	xt_heap_elem_t *_px = xt_heap_search( heap, 0, px );
	return xt_heap_erase_( heap, _px );
}

int xt_heap_erase_( xt_heap_t *heap, xt_heap_elem_t *px )
{
	xt_size_t idx = 0;

#define _arr				heap->arr
#define len					heap->count

	if ( xt_is_heap_elem(heap,px) && xt_in_heap_range(heap,px) )
	{
		*px = _arr[len-1];
		--len;

		idx = px - _arr;
		xt_heap_adjust( heap, idx );

		return FUN_RET_OK;
	}

#undef _arr
#undef len

	return -1;
}

int xt_heap_pop( xt_heap_t *heap, xt_heap_elem_t *px ) /* Popup element on top of this heap ( the mini one ) */
{
	if ( heap->count == 0 )
	{
		return -1;
	}

#define _arr				heap->arr
#define len					heap->count

	if ( px )
	{
		*px = _arr[0];
	}

	XT_SWAP( _arr[0], _arr[len-1] );
	--heap->count;

	xt_heap_adjust( heap, 0 );

#undef _arr
#undef len

	return FUN_RET_OK;
}

/* Assumption: idx nodes above level are all in order. */
void xt_heap_adjust( xt_heap_t *heap, xt_size_t idx )
{
	/*idx, element index that to be adjusted. */
	xt_size_t lchild = xt_heap_idx_lchild(idx);
	xt_size_t rchild = xt_heap_idx_rchild(idx);
	xt_size_t _idx = 0; /* Largest(smallest) among (idx,lchild and rchild) */
#define _arr				heap->arr
#define len					heap->count

	while( lchild<len || rchild<len )
	{
		/* 
			Make decision on _idx.
			_idx should be whom among (idx,lchild and rchild).
		*/
		if ( lchild<len && heap->fn_cmp(&_arr[lchild], &_arr[idx])>0 )
		{
			_idx = lchild;
		}
		else
		{
			_idx = idx;
		}
		/* _idx was setup. */
		if ( rchild<len && heap->fn_cmp(&_arr[rchild], &_arr[_idx])>0 )
		{
			_idx = rchild;
		}

		if ( _idx != idx ) /* one of lchild and rchild, should be parent node.  */
		{
			XT_SWAP( _arr[idx], _arr[_idx] ); /* arr[idx] is new parent. */
			/* Keep adjusting its child(left or right), whose index is _idx */

			/* Prepare for next loop */
			idx = _idx;
			lchild = xt_heap_idx_lchild(idx);
			rchild = xt_heap_idx_rchild(idx);
		}
		else
		{
			break;
		}
	}

#undef _arr
#undef len

	return;
}

int xt_heap_build( xt_heap_t *heap )
{
	xt_size_t i = 0;

#define _arr				heap->arr
#define len					heap->count

	/* From latest leaf's parent to root node */
	for ( i=(len/2-1); i!=((xt_size_t)(~0)); --i )
	{
		xt_heap_adjust( heap, i );
	}

#undef _arr
#undef len

	return FUN_RET_OK;
}

int xt_heap_sort( xt_heap_t *heap )
{
	xt_size_t i = 0;
	xt_size_t _len = heap->count; /* Save len(heap->count), so that we can restore this data member, after adjust this heap */

#define _arr				heap->arr
#define len					heap->count

	/*xt_heap_build( heap );*/ /* Make array into heap */

	for ( i=len-1; i>=1; --i )
	{
		XT_SWAP( _arr[0], _arr[i] );
		--len; /* Heapfy the (n-1) members */
		xt_heap_adjust( heap, 0 );
	}

	/* restore heap->count */
	heap->count = _len;

#undef _arr
#undef len

	return 0;
}

xt_heap_elem_t*	xt_heap_search( xt_heap_t *heap, xt_size_t idx_start, const xt_heap_elem_t *px )
{
	xt_heap_elem_t *ret = NULL;

	xt_size_t _idx = 0;
	xt_size_t lchild = 0;
	xt_size_t rchild = 0;

	int	comp = 0;

#define _arr				heap->arr
#define len					heap->count

	if ( len <= idx_start )
	{
		return NULL;
	}

	_idx = idx_start;
	while( _idx < len )
	{
		comp = heap->fn_cmp( px, &_arr[_idx] );
		if ( comp == 0 ) /* Found */
		{
			ret = &_arr[_idx];
			break;
		}

		if ( comp > 0 ) /* Go upper. */
		{
			/* Do NOT have to move down, because the lower elements, in this sub-tree, must not be bigger than [*px] */
			goto lbl_go_upper;
		}

		/* ( comp < 0 ) from here. */
		lchild = xt_heap_idx_lchild(_idx);
		if ( lchild < len ) /* Keep moving down. */
		{
			/* search among LEFT-children. */
			_idx = lchild;
			continue;
		}

		/* lchild >= len */
		/* NO more children */
		/* Go upper. */
lbl_go_upper:
		while( xt_heap_is_rchild(_idx) && _idx!=idx_start )
		{
			_idx = xt_heap_idx_parent(_idx); /* Go upper. */
		}

		if ( _idx == idx_start ) /* NOT found. */
		{
			break;
			/*return NULL;*/
		}

		_idx = xt_heap_idx_parent(_idx); /* Go upper. */

		rchild = xt_heap_idx_rchild(_idx);
		
		if ( rchild < len ) /* Keep moving down. */
		{
			/* search among RIGHT-children. */
			_idx = rchild;
			continue;
		}
		goto lbl_go_upper;
	}
	
#undef _arr
#undef len

	return ret;
}

void xt_test_heap_sort( xt_heap_elem_t *arr, xt_size_t len )
{
	xt_heap_t heap;

	heap.capacity = len;
	heap.count = len;
	heap.arr = arr;
	
	heap.fn_cmp = xt_sort_cmp_def;

	xt_heap_build( &heap );

	xt_heap_sort( &heap );
}
