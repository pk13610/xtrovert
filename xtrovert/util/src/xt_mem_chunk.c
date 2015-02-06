/************************************************************************************
* xt_mem_chunk.c: Implementation File
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

#include "xt_mem_chunk.h"

#if defined(__WINDOWS__)
/* warning C4100: 'XXX' : unreferenced formal parameter */
#pragma warning (disable: 4100)
#endif

xt_mem_chunk_t* xt_memchunk_create( xt_size_t size )
{
	return (xt_mem_chunk_t*)xt_mem_alloc( size );
}

void xt_memchunk_destroy( xt_mem_chunk_t *mem_chunk )
{
	xt_free( mem_chunk );
}
