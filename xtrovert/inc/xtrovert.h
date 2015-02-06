/************************************************************************************
* xtrovert.h : header file
*
* Common sys-call & API encapsulations
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

#ifndef __XTROVERT_H__
#define __XTROVERT_H__

#include "xt_define.h"

#ifdef __cplusplus
extern "C" {
#endif

/*#include <new>*/ /* Placement new */

/*#define _ALIGN_BYTES_	sizeof(long)*/

/**********************************************************************
* Hardware Information
**********************************************************************/
/* Get CPU core/count */
extern unsigned int cpu_count(void);
/**********************************************************************
* Hardware Information END
**********************************************************************/

/**********************************************************************
* Compiler Information
**********************************************************************/
/*__GNUC__¡¢__GNUC_MINOR__¡¢__GNUC_MINOR__¡¢__GNUC_PATCHLEVEL__*/
/*__VERSION__*/
#define CC_NAME_LEN					16
#define CC_VER_LEN					16

typedef struct _tag_CC_INFO
{
	TCHAR szCcName[CC_NAME_LEN];
	TCHAR szCcVer[CC_VER_LEN];
}CC_INFO;

int get_compiler_info( CC_INFO *pInfo );
/**********************************************************************
* Compiler Information END
**********************************************************************/

/**********************************************************************
*		XTROVERT INFO
**********************************************************************/
/* Xtrovert Application Info example:
	Copyright: 
	Application: extorvert_app [Application full Name, like xt_app.exe, "xtrovert application"]
	App-Version: 0.00.0001 Beta/RTM
	Compiler: MS-VC, 2010 [Compiler: name, Version]
	Built info: Date-Time (Debug/Release) 64bit/32bit [2014-07-15 (Release) 64bit]
*/

#define XT_LIB_COPYRIGHT _T("Copyright (c) 2014 Sean Feng " \
							"Foundation or its licensors, as applicable.")

#define __XTROVERT_VER_MAJOR__			0
#define __XTROVERT_VER_MINOR__			00
#define __XTROVERT_VER_PATCHLEVEL__		0001 /*0001*/

/*__EXTROVERT_VER_MAJOR__.__EXTROVERT_VER_MINOR__.__EXTROVERT_VER_PATCHLEVEL__*/
#define XT_LIB_VER_STR_LEN			16
#define XT_LIB_BUILT_STAMP_LEN		32

#define __xt_ver_mum_to_str(x)		_T(#x)
#define _xt_ver_mum_to_str(x)		__xt_ver_mum_to_str(x)
#define __XTROVERT_VER_STR__		_xt_ver_mum_to_str(__XTROVERT_VER_MAJOR__##.##__XTROVERT_VER_MINOR__##.##__XTROVERT_VER_PATCHLEVEL__)
typedef struct _tag_xt_libinfo_t
{
	TCHAR szVersion[XT_LIB_VER_STR_LEN];
	TCHAR szBuildStamp[XT_LIB_BUILT_STAMP_LEN];
}xt_libinfo_t;

#if defined(_DEBUG)
#define __XT_RELEASE_TYPE_STR__			_T("Debug")
#else
#define __XT_RELEASE_TYPE_STR__			_T("Release")
#endif

int xt_get_built_info( xt_libinfo_t *pInfo );
/**********************************************************************
*		XTROVERT INFO END
**********************************************************************/

#ifdef __cplusplus
} /* ... extern "C" */
#endif

#endif /* __XTROVERT_H__ */
