/************************************************************************************
* xt_alg_sort_selsort.h : header file
*
* header: Selection Sort
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
#ifndef __XT_ALG_SORT_SELSORT_H__
#define __XT_ALG_SORT_SELSORT_H__

#include "xt_alg_define.h"

#ifdef __cplusplus
extern "C" {
#endif

void xt_select_sort( xt_sort_elem_t *arr, xt_size_t len, xt_fn_sort_cmp_t fn_cmp );
void xt_select_sort2( xt_sort_elem_t *arr, xt_size_t idx_start, xt_size_t idx_end, xt_fn_sort_cmp_t fn_cmp );

#ifdef __cplusplus
}
#endif

#endif /*__XT_ALG_SORT_SELSORT_H__*/
