/************************************************************************************
*	xt_mm_api.c: Implementation File
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
#include "xt_mem_api.h"

int _safe_memcpy( void *dst, const void *src, xt_size_t count )
{
	/************************************************************************
	* Parameters:
	*
	* Return Value:
	* 
	* Remarks:
	************************************************************************/
	xt_size_t i = 0;

	if ( dst==NULL || src==NULL )
	{
		return FUN_RET_ERR_ILLEGAL_ARGS;
	}
	
	if ( count == 0 )
	{
		return FUN_RET_OK;
	}

	if ( (char*)src>(char*)dst && (char*)src<=((char*)dst+count) )
	{
		return -2;
	}

	if ( (char*)dst>(char*)src && (char*)dst<=((char*)src+count) )
	{
		return -3;
	}

	for ( i=0; i<count; ++i )
	{
		((char*)dst)[i] = ((char*)src)[i];
	}

	return count;
}

void* xt_malloc( xt_size_t nSize )
{
	return malloc( nSize );
}

void xt_free( void *p )
{
	free(p);
	p = NULL;
}

void* xt_mem_alloc( xt_size_t size )
{
	return xt_malloc( size );
}

void xt_mem_free( void *buf )
{
	xt_free( buf );
}
