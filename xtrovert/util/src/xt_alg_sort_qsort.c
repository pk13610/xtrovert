/************************************************************************************
* xt_alg_sort_qsort.c: Implementation File
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

#include "xt_alg_sort_qsort.h"
#include "xt_mem_api.h"

#if defined(__WINDOWS__)
/* warning C4100: 'XXX' : unreferenced formal parameter */
#pragma warning (disable: 4100)
#endif

#ifdef __cplusplus
extern "C" {
#endif
static xt_size_t xt_qsort_partition_x( xt_sort_elem_t *arr, xt_size_t idx_start, xt_size_t idx_end, xt_fn_sort_cmp_t fn_cmp ); /* arr[idx_start..idx_end] */
#ifdef __cplusplus
}
#endif

void qsort_getk_x( xt_sort_elem_t *arr, xt_size_t idx_start, xt_size_t idx_end, xt_size_t first_k, xt_fn_sort_cmp_t fn_cmp )
{
	xt_size_t mid  = 0;
	
	if ( idx_start<idx_end && idx_end<_INVALID_SIZE_T_VALUE_ && first_k>0 )
	{
		mid = xt_qsort_partition_x( arr, idx_start, idx_end, fn_cmp );
		if ( (mid-idx_start) == first_k )
		{
			return;
		}

		if ( (mid-idx_start) > first_k )
		{
			qsort_getk_x( arr, idx_start, mid-1, first_k, fn_cmp );
		}
		else
		{
			first_k -= mid;
			qsort_getk_x( arr, mid+1, idx_end, first_k, fn_cmp );
		}
	}

	return;
}

void qsort_getk( xt_sort_elem_t *arr, xt_size_t len, xt_size_t first_k, xt_fn_sort_cmp_t fn_cmp )
{
	xt_size_t idx_start = 0;
	xt_size_t idx_end = 0;
	if ( len == 0 )
	{
		return;
	}

	idx_end = len - 1;
	qsort_getk_x( arr, idx_start, idx_end, first_k, fn_cmp );
}

void xt_quick_sort_loop_1( xt_sort_elem_t *arr, xt_size_t len )
{
	/* One loop statement version */
	xt_sort_elem_t min = 0;
	xt_size_t i = 0;
	xt_size_t j = 0;
	xt_size_t k = 0;

	for ( i=0; i<(len-1)*len; ++i )
	{
		j = i/len;
		k = i%len;

		if ( k == j )
			min = j;
		else if( k > j )
		{
			if ( arr[k] < arr[min] )
				min = k;
		}
		if ( k == len-1 )
		{
			XT_SWAP( arr[min], arr[j] );
		}
	}
}

/* Quick Sort Partition */
static xt_size_t xt_qsort_partition_x( xt_sort_elem_t *arr, xt_size_t idx_start, xt_size_t idx_end, xt_fn_sort_cmp_t fn_cmp ) /* arr[idx_start..idx_end] */
{
	/* Return index of pivot */
	xt_sort_elem_t pivot = 0;
	xt_size_t i = 0;
	xt_size_t j = 0;

	pivot = arr[idx_start]; /* pivot */
	i = idx_start;

	for ( j=idx_start+1; j<=idx_end; ++j )
	{
		/* idx_start <= "i" <= "j" <= idx_end */
		/* Elements on the left of "i", is less than "x" */
		/* Elements between "i" and "j", is bigger than "x" */
		/* Elements on the right of "j", is about to be processed in next loop */
		if ( fn_cmp(&arr[j], &pivot) < 0 ) /*if ( arr[j] <= pivot )*/
		{
			++i;
			XT_SWAP( arr[i], arr[j] );
		}
	}

	XT_SWAP( arr[idx_start], arr[i] );

	return i;
}

void xt_quick_sort_x1( xt_sort_elem_t *arr, xt_size_t len, xt_fn_sort_cmp_t fn_cmp )
{
	xt_size_t idx_start = 0;
	xt_size_t idx_end = 0;
	if ( len == 0 )
	{
		return;
	}

	idx_end = len - 1;
	xt_quick_sort_x2( arr, idx_start, idx_end, fn_cmp );
}
#if defined(__WINDOWS__)
#define MAX_THRESH 8
#else /*__LINUX__*/
#define MAX_THRESH 4
#endif
void xt_quick_sort_x2( xt_sort_elem_t *arr, xt_size_t idx_start, xt_size_t idx_end, xt_fn_sort_cmp_t fn_cmp )
{
	xt_size_t mid = 0;

	if ( idx_start<idx_end && idx_end<_INVALID_SIZE_T_VALUE_ )
	{
		/* Median-of-Three Decision */
		mid = idx_start + ((idx_end-idx_start)>>1);
		if ( fn_cmp(&arr[idx_start], &arr[mid]) > 0 )
		{
			XT_SWAP( arr[idx_start], arr[mid] );
		}
		if ( fn_cmp(&arr[mid], &arr[idx_end]) > 0 )
		{
			XT_SWAP( arr[mid], arr[idx_end] );
		}
		else
		{
			goto jump_over;
		}

		if ( fn_cmp(&arr[idx_start], &arr[mid]) > 0 )
		{
			XT_SWAP( arr[idx_start], arr[mid] );
		}

jump_over:;
		/*  */
		mid = xt_qsort_partition_x( arr, idx_start, idx_end, fn_cmp );
		xt_quick_sort_x2( arr, idx_start, mid-1, fn_cmp );
		xt_quick_sort_x2( arr, mid+1, idx_end, fn_cmp );
	}

	return;
}
#if defined(MAX_THRESH)
#undef MAX_THRESH
#endif

void xt_quick_sort_x( xt_sort_elem_t *arr, xt_size_t len )
{
	xt_quick_sort_x1( arr, len, xt_sort_cmp_def );
}

/*
	return value:
		assume order: a b
		>0: order: b a
		<0: order: a b
*/
static int	std_qsort_cmp( const void *a, const void *b )
{
	return ((*(xt_sort_elem_t*)a) - (*(xt_sort_elem_t*)b));
}

void xt_libc_qsort( xt_sort_elem_t *arr, xt_size_t len )
{
	xt_qsort( arr, len, sizeof(xt_sort_elem_t), std_qsort_cmp );
}

