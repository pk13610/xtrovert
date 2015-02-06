/************************************************************************************
* xt_alg_sort_selsort.c: Implementation File
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

#include "xt_alg_sort_selsort.h"

#if defined(__WINDOWS__)
/* warning C4100: 'XXX' : unreferenced formal parameter */
#pragma warning (disable: 4100)
#endif

void xt_select_sort( xt_sort_elem_t *arr, xt_size_t len, xt_fn_sort_cmp_t fn_cmp )
{
	xt_size_t idx_start = 0;
	xt_size_t idx_end = 0;
	if ( len == 0 )
	{
		return;
	}

	idx_end = len - 1;
	xt_select_sort2( arr, idx_start, idx_end, fn_cmp );
}

void xt_select_sort2( xt_sort_elem_t *arr, xt_size_t idx_start, xt_size_t idx_end, xt_fn_sort_cmp_t fn_cmp )
{
	xt_size_t i = 0; /* [idx_start..idx_end], could be instead by idx_start. */
	xt_size_t j = 0; /* [(i+1)..idx_end] */
	xt_size_t k = 0; /* Position for max/min value among [j..idx_end] */

	for ( i=idx_start; i<=idx_end; ++i )
	{
		k = i;
		for ( j=i+1; j<=idx_end; ++j )
		{
			if ( fn_cmp(&arr[j],&arr[k]) > 0 )
			{
				k = j;
			}
		}

		XT_SWAP( arr[i], arr[k] );
	}
}
