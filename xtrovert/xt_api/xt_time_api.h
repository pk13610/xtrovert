/************************************************************************************
* xt_time_api.h : header file
*
* Time Functions Definition header
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
#ifndef __XT_TIME_API_H__
#define __XT_TIME_API_H__

#include "xt_define.h"
#include <time.h>

#ifdef __cplusplus
extern "C" {
#endif

int		xt_localtime( const time_t *timep, struct tm *result );

#define  xt_time time
/*time_t*	xt_time( time_t *timep );*/

time_t	GetDateTime(void);

const TCHAR* get_time_str( time_t t, TCHAR *szStr );

int		xt_sleep( UINT seconds );				/* In Seconds */
int		xt_msleep( UINT milli_seconds );		/* In Milli-Seconds */
/*int		xt_usleep( UINT64 micro_seconds );*/		/* In Micro-Seconds */
/*int		xt_nsleep( UINT64 nano_seconds );*/		/* In Nano-Seconds */

#if defined(__WINDOWS__)
typedef clock_t		xt_ticker_t;
#elif defined(__LINUX__)
typedef long		xt_ticker_t;
#endif

void			xt_reset_ticker( xt_ticker_t *ticker );
xt_ticker_t		xt_test_ticker( xt_ticker_t *ticker );

#ifdef __cplusplus
}
#endif

#endif /*__XT_TIME_API_H__*/
