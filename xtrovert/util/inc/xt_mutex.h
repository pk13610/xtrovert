/************************************************************************************
* xt_mutex.h : header file
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
#ifndef __XT_MUTEX_H__
#define __XT_MUTEX_H__

#include "xt_define.h"

#ifdef __cplusplus
extern "C" {
#endif


#if defined(__LINUX__)
typedef pthread_mutex_t						xt_mutex_t;
#define xt_mutex_init(MUTEX)				pthread_mutex_init(MUTEX, NULL)
#define xt_mutex_destroy(MUTEX)				pthread_mutex_destroy(MUTEX)
#define xt_mutex_lock(MUTEX)				pthread_mutex_lock(MUTEX)
#define xt_mutex_unlock(MUTEX)				pthread_mutex_unlock(MUTEX)
#elif defined(__WINDOWS__)
typedef CRITICAL_SECTION					xt_mutex_t;
#define xt_mutex_init(MUTEX)				InitializeCriticalSection(MUTEX)
#define xt_mutex_destroy(MUTEX)				DeleteCriticalSection(MUTEX)
#define xt_mutex_lock(MUTEX)				EnterCriticalSection(MUTEX)
#define xt_mutex_unlock(MUTEX)				LeaveCriticalSection(MUTEX)
#endif

#ifdef __cplusplus
}
#endif

#endif /*__XT_TIMER_H__*/
