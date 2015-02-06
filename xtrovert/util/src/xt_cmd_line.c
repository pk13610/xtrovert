/************************************************************************************
* xt_cmd_line.c: Implementation File
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

#include "xt_cmd_line.h"

#if defined(__WINDOWS__)
/* warning C4100: 'XXX' : unreferenced formal parameter */
#pragma warning (disable: 4100)
#endif

void cmdline_progress_show( xt_cmdln_prog_t *progress )
{
	/*_tprintf(  );*/
}

void cmdline_progress_update( xt_cmdln_prog_t *progress, xt_size_t count )
{
	if ( count == progress->total )
	{
		_tprintf( _T("\n") );
	}
}
