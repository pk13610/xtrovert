/************************************************************************************
* xt_mem_chunk.h : header file
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
#ifndef __XT_MEM_CHUNK_H__
#define __XT_MEM_CHUNK_H__

#include "xt_mem_api.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _tag_xt_mem_chunk_t
{
	xt_size_t			size;
	char				*buf;
}xt_mem_chunk_t;

xt_mem_chunk_t* xt_memchunk_create( xt_size_t size );
void			xt_memchunk_destroy( xt_mem_chunk_t *mem_chunk );

#ifdef __cplusplus
}
#endif

#endif /*__XT_MEM_CHUNK_H__*/
