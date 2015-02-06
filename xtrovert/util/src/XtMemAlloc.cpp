/************************************************************************************
* CXtMemAlloc.cpp: Implementation File
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

#include "xtMemAlloc.h"
#include "xt_mem_api.h"

void* CXtMemAlloc::Alloc( xt_size_t size )
{
	return xt_malloc( size );
}

void CXtMemAlloc::Free( void *buf )
{
	xt_free( buf );
}
