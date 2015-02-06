/************************************************************************************
* xt_ticker.h : header file
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
#ifndef __XT_TICKER_H__
#define __XT_TICKER_H__

#include "xt_define.h"

#ifdef __cplusplus
extern "C" {
#endif

// typedef u_int64 ext_uint_64;
// typedef u_int32 ext_uint_32;
// 
// typedef struct _tag_ext_mtimer_t /* Milliseconds timer */
// {
// 	ext_uint_64		interval;
// 	ext_uint_64		last_shot;
// }ext_timer_t;
// 
// #define _EXT_SET_TIMER_(tmr, cur_time, interval)	do { tmr.interval=interval; tmr.last_shot=cur_time; } while(0)
// #define _EXT_RESET_TIMER_(tmr, cur_time)			tmr.last_shot=cur_time
// #define _EXT_TEST_TIMER_(tmr, cur_time)				(tmr.last_shot+tmr.interval)>=cur_time
// 
// typedef ext_uint_64 tick_count_t; /* Support enough years whether by seconds, milliseconds or micro-seconds */
// 
// typedef ext_uint_32 tick_count_t_32; /* up to 67 years by seconds, 27 days by milliseconds */
// /* MSB(Most Significant Bit) indicate whether overflow or not. */
// #define _EXT_TICK_CNT_LIMIT_						(((tick_count_t_32)(~0)))>>1
// #define _EXT_INC_TICK_CNT_(tick)					tick=((tick&_EXT_TICK_CNT_LIMIT_)==_EXT_TICK_CNT_LIMIT_)?(_EXT_TICK_CNT_LIMIT_+1):++tick
// #define _EXT_GET_TICK_CNT_(tick)					tick&_EXT_TICK_CNT_LIMIT_
// /* Get MSB(Most Significant Bit). */
// #define _EXT_OVERFLOW_TICK_CNT_(tick)				tick&(_EXT_TICK_CNT_LIMIT_+1)
// /* Set MSB(Most Significant Bit) to zero. */
// #define _EXT_ZERO_OVERFLOW_(tick)					do { if (_EXT_OVERFLOW_TICK_CNT_(tick)) {tick|=(_EXT_TICK_CNT_LIMIT_+1);} } while (0)

/* typedef uint32 ext_ticker_t; up to 67 years by seconds, 27 days by milliseconds */
/* Support enough years whether by seconds, milliseconds or micro-seconds */
typedef UINT64		xt_ticker_t;

/* MSB(Most Significant Bit) indicate whether overflow or not. */
#define _XT_TICKER_LIMIT_							((((xt_ticker_t)(~0)))>>1) /* MSB(Most Significant Bit) is 0, other bits are 1 */

/* Get MSB(Most Significant Bit). */
#define _xt_ticker_overflow(ticker)					((ticker)&(_XT_TICKER_LIMIT_+1)) /* MSB is 1 ? */
#define _xt_diff_ticker(early_ticker,later_ticker)	((later_ticker) - (early_ticker))

#define xt_reset_ticker(ticker)						((ticker) = 0)
#define xt_inc_ticker(ticker)						(++(ticker))
/* Set MSB(Most Significant Bit) to zero. */
#define xt_ticker_reset_overflow(ticker)			((ticker)&=_XT_TICKER_LIMIT_)

const xt_ticker_t _FRAME_MSECONDS_		= 40; /* x milliseconds for one frame ( x m-seconds/frame ) */

const xt_ticker_t _FRAMES_PPER_SECOND_	= 1000/_FRAME_MSECONDS_; /* FPS: frames-per-second (x frames/second) */

/* s is in seconds */
#define _TICKS_ON_SECOND_(s)						_##s##_SECOND_TICKS_
#define _DECLARE_TICKS_ON_SECOND_(s)				const xt_ticker_t _TICKS_ON_SECOND_(s) = _FRAMES_PPER_SECOND_*(s)

#ifdef __cplusplus
}
#endif

#endif /*__EXT_TICKER_H__*/
