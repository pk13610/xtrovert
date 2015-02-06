/************************************************************************************
* xt_alg_sort_insert.c: Implementation File
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

#include "xt_alg_sort_insert.h"

#if defined(__WINDOWS__)
/* warning C4100: 'XXX' : unreferenced formal parameter */
#pragma warning (disable: 4100)
#endif

extern void xt_insert_sort3( xt_sort_elem_t *arr, xt_size_t idx_start, xt_size_t idx_end, xt_fn_sort_cmp_t fn_cmp );
void xt_insert_sort( xt_sort_elem_t *arr, xt_size_t len, xt_fn_sort_cmp_t fn_cmp )
{
	xt_size_t idx_start = 0;
	xt_size_t idx_end = 0;
	if ( len == 0 )
	{
		return;
	}

	idx_end = len - 1;

	xt_insert_sort3( arr, idx_start, idx_end, fn_cmp );

	return;
}

void xt_insert_sort2( xt_sort_elem_t *arr, xt_size_t idx_start, xt_size_t idx_end, xt_fn_sort_cmp_t fn_cmp )
{
	xt_size_t i = 0; /* [idx_start..idx_end], could be instead by idx_start. */

	xt_sort_elem_t x = 0;
	xt_sort_elem_t *s = NULL;

	for ( i=idx_start; i<idx_end; ++i )
	{
		s = &arr[i];
		x = s[1];
		while ( s>=arr && fn_cmp(&x,s)<0  )
		{
			s[1] = *s;
			--s;
		}
		s[1] = x;
	}

	return;
}

void xt_insert_sort3( xt_sort_elem_t *arr, xt_size_t idx_start, xt_size_t idx_end, xt_fn_sort_cmp_t fn_cmp )
{
	xt_size_t i = 0; /* [idx_start..idx_end], could be instead by idx_start. */
	xt_size_t j = 0;

	xt_sort_elem_t x = 0;

	for ( i=idx_start; i<idx_end; ++i )
	{
		j = i;
		x = arr[j+1];
		while ( j>=0 && fn_cmp(&x,&(arr[j]))<0  )
		{
			arr[j] = arr[j-1];
			--j;
		}
		arr[j+1] = x;
	}

	return;
}
