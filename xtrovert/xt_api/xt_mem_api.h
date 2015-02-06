/************************************************************************************
* xt_mem_api.h : header file
*
* xt_mem_api API Definition header
*
* AUTHOR	:	Sean Feng <SeanFeng2006@hotmail.com>
* DATE		:	Aug. 7, 2012
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
#ifndef __XT_MEM_API_H__
#define __XT_MEM_API_H__

#include "xt_define.h"

#ifdef __cplusplus
extern "C" {
#endif

/***********************************************************************
* Platform Independent Memory API Collection (exception based)
***********************************************************************/
int		_safe_memcpy( void *dst, const void *src, xt_size_t count );

void*	xt_malloc( xt_size_t nSize );
void	xt_free( void *p );

extern void* xt_mem_alloc( xt_size_t size );
extern void xt_mem_free( void *p );

#ifdef __cplusplus
}
#endif

#endif /*__XT_MEM_API_H__*/
