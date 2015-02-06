/************************************************************************************
* xt_timer.h : header file
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
#ifndef __XT_TIMER_H__
#define __XT_TIMER_H__

#include "xt_time_api.h"

#ifdef __cplusplus
extern "C" {
#endif


typedef UINT64		xt_timer_counter_t;

typedef struct _tag_xt_timer_t
{
	xt_timer_counter_t		interval;
	xt_timer_counter_t		last_shot;
}xt_timer_t;

#define _test_timer(_tmr, _now)						((_now) >= ((_tmr).last_shot+(_tmr).interval))

#define init_timer(_tmr, _now, _interval)			do { (_tmr).interval=(_interval); (_tmr).last_shot=(_now); } while(0)
#define update_timer(_tmr, _now)					((_tmr).last_shot = (_now))

#ifdef __cplusplus
}
#endif

#endif /*__XT_TIMER_H__*/
