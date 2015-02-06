/************************************************************************************
*	xtrovert.c: Implementation File
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

#include "xtrovert.h"

/* Get CPU core/count */
unsigned int cpu_count(void)
{
	static unsigned int count = 0;
	if ( count == 0 )
	{
#if defined(__LINUX__)
		count = sysconf(_SC_NPROCESSORS_CONF);
#elif defined(__WINDOWS__)
		SYSTEM_INFO si;
		GetSystemInfo(&si);
		count = si.dwNumberOfProcessors;
#endif
	}

	return count;
}

int get_compiler_info( CC_INFO *pInfo )
{
	if ( pInfo == NULL )
	{
		return FUN_RET_ERR_ILLEGAL_ARGS;
	}

	memset( pInfo, 0, sizeof(CC_INFO) );

	return FUN_RET_OK;
}

int ext_get_built_info( xt_libinfo_t *pInfo )
{
	if ( pInfo == NULL )
	{
		return FUN_RET_ERR_ILLEGAL_ARGS;
	}

// 	memset( pInfo, 0, sizeof(EXT_LIB_INFO) );
// 	_sntprintf( pInfo->szVersion, EXT_LIB_VER_LEN, _T("%s"), EXTROVERT_VER );
// 	_sntprintf( pInfo->szBuildStamp, EXT_LIB_BUILD_STAMP_LEN, _T("%s %s"), _T(__DATE__), _T(__TIME__) );

	return FUN_RET_OK;
}
