/************************************************************************************
* xt_alg_define.h : header file
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
#ifndef __XT_ALG_DEFINE_H__
#define __XT_ALG_DEFINE_H__

#include "xt_define.h"

#ifdef __cplusplus
extern "C" {
#endif

#if defined(__WINDOWS__)
	#define xt_qsort			qsort
#else
	#define xt_qsort			qsort
#endif

typedef unsigned long	xt_sort_elem_t;

#define _XT_SORT_ELEM_VAL_INVALID_	 ((xt_sort_elem_t)(~0))

typedef int (*xt_fn_sort_cmp_t)(const void*, const void*);
int xt_sort_cmp_def( const void *arg1, const void *arg2 );

#ifdef __cplusplus
}
#endif

#endif /*__XT_ALG_DEFINE_H__*/
