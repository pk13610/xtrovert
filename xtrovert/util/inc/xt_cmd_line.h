/************************************************************************************
* xt_cmd_line.h : header file
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
#ifndef __XT_CMD_LINE_H__
#define __XT_CMD_LINE_H__

#include "xt_define.h"

#ifdef __cplusplus
extern "C" {
#endif

/*******************************************************************
*		Command Line Progress Prompt
*******************************************************************/
#define XT_CMDLINE_PROGRESS_OK						_T("OK")
#define XT_CMDLINE_PROGRESS_PERCENT(percent)		_T(percent##"%")
#define XT_CMDLINE_PROGRESS_FAILED					_T("Failed")
#define XT_CMDLINE_PROGRESS_ERROR					_T("Error")
#define XT_CMDLINE_PROGRESS_DONE					_T("Done")
#define XT_CMDLINE_PROGRESS_COMP					_T("Complete")

#define XT_CMDLINE_CLEAR							_T("\r\033[K")

/*
http://blog.csdn.net/querw/article/details/13095985
Single line:
progress message................................................[30%]

Multiple lines:
progress message xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
	xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx..................[30%]
*/
typedef struct _tag_xt_cmdline_progress_t
{
	const TCHAR			*progress_msg;
	xt_size_t			total;
	int					percent_mode;
}xt_cmdln_prog_t;

#define _cmdline_set_msg(prog,msg)						((prog).progress_msg=(msg))
#define _cmdline_set_total(prog,_total)					((prog).total=(_total))
#define _cmdline_set_percent(prog,mode)					((prog).percent_mode=(mode))

void		cmdline_progress_show( xt_cmdln_prog_t *progress );
void		cmdline_progress_update( xt_cmdln_prog_t *progress, xt_size_t count );

#ifdef __cplusplus
}
#endif

#endif /*__XT_CMD_LINE_H__*/
