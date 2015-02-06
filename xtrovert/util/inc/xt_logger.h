/************************************************************************************
* xt_logger.h : header file
*
* Definition header
*
* AUTHOR	:	Sean Feng <SeanFeng2006@hotmail.com>
* DATE		:	May 30, 2014
* Copyright (c) 2014-?. All Rights Reserved.
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

#if !defined(__XT_LOGGER_H__)
#define __XT_LOGGER_H__

#include "xtrovert.h"

#ifdef __cplusplus
extern "C" {
#endif

enum
{
	LOG_LEVEL_INFO = 0,
	LOG_LEVEL_WARN,
	LOG_LEVEL_ERROR,
	LOG_LEVEL_FATAL,

	LOG_LEVEL_INVALID
};

enum
{
	LOG_TYPE_DEBUG = 0,
	LOG_TYPE_ITEM,
	LOG_TYPE_MONEY,

	LOG_TYPE_INVALID
};

#define _LOG(log_type,fmt, ...)  _tprintf(_T("%d,")fmt _T("\n"), log_type, ##__VA_ARGS__)
#define _LOG_DEBUG(log_type,fmt, ...) _tprintf(_T("%d,") _T(__FILE__)_T("(%d):")fmt _T("\n"), log_type, __LINE__, ##__VA_ARGS__)

#if defined(_DEBUG)
#define LOGGER_DEBUG
#else
#define LOGGER_DEBUG			_LOG_DEBUG
#endif

#define LOGGER_ITEM(fmt, ...)				_LOG(LOG_TYPE_ITEM,fmt, ...)
#define LOGGER_MONEY(fmt, ...)				_LOG(LOG_TYPE_MONEY,fmt, ...)

#ifdef __cplusplus
}
#endif

#endif /*__XT_LOGGER_H__*/


#if !defined(__XT_LOGGER_H__)
#define __XT_LOGGER_H__

#define _RAW_LOG

enum
{
	LOG_LEVEL_INFO = 0,
	LOG_LEVEL_WARN,
	LOG_LEVEL_ERROR,
	LOG_LEVEL_FATAL,

	LOG_LEVEL_INVALID
};

#define _TXT_LOG(log_lvl,fmt, ...)				_tprintf(_T("%d,")##fmt _T("\n"), (log_lvl), ##__VA_ARGS__)
#define _TXT_LOG_DEBUG(log_lvl,fmt, ...)		_tprintf(_T("%d,") _T(__FILE__)_T("(%d):")##fmt _T("\n"), (log_lvl), __LINE__, ##__VA_ARGS__)
#define _TXT_LOG_CALLER(log_lvl,fmt, ...)		_tprintf(_T("%d,") _T(__FILE__)_T("(%s):")##fmt _T("\n"), (log_lvl), __FUNCDNAME__, ##__VA_ARGS__)

#if defined(_DEBUG)
#define LOG_DEBUG						_TXT_LOG_DEBUG
#else
#define LOG_DEBUG
#endif

/* For test purpose */
#define LOG(fmt, ...)					_tprintf(fmt _T("\n"), __VA_ARGS__)
#undef	LOG_DEBUG
#define LOG_DEBUG(fmt, ...)				_tprintf(_T(__FILE__)_T("(%d):")##fmt _T("??\n"), __LINE__, ##__VA_ARGS__)

#endif /*__XT_LOGGER_H__*/
