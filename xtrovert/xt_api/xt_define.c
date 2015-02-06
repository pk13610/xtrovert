/************************************************************************************
*	xt_io_api.c: Implementation File
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

#include "xt_define.h"
#include "xt_time_api.h"

/****************************************************************************************/

const TCHAR _SYMBOL_DIR_LINUX_STD_ = _SYMBOL_SLASH_;
const TCHAR _SYMBOL_DIR_WIN_STD_ = _SYMBOL_BACK_SLASH_;

TCHAR szLastSysCall[_MAX_STR_LEN_128_];

/****************************************************************************************/

void xt_srand(void)
{
	static unsigned int rand_seed = 0;
	if ( rand_seed == 0 )
	{
		rand_seed = (int)time(0);
	}

	srand( rand_seed );
	rand_seed += rand();
}
