/************************************************************************************
* xt_alg_define.c: Implementation File
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

#include "xt_alg_define.h"

#if defined(__WINDOWS__)
/* warning C4100: 'XXX' : unreferenced formal parameter */
#pragma warning (disable: 4100)
#endif

int xt_sort_cmp_def( const void *arg1, const void *arg2 )
{
	/* Ascending order */
	return ((*(xt_sort_elem_t*)arg1) - (*(xt_sort_elem_t*)arg2));
	/* Descending order */
	/*return ((*(xt_sort_elem_t*)arg2) - (*(xt_sort_elem_t*)arg1));*/
}
