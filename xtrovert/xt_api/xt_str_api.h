/************************************************************************************
* xt_str_api.h : header file
*
* String Functions Definition header
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
#ifndef __XT_STR_API_H__
#define __XT_STR_API_H__

#include "xt_define.h"

#ifdef __cplusplus
extern "C" {
#endif

/***********************************************************************
* Platform Independent String Function Collection (exception based)
***********************************************************************/

int xt_skip_space_char( const TCHAR *szSrc );
int xt_str_is_num( const TCHAR *val );

/* nBase might be 2,8,10,16 */
int xt_str2_num( const TCHAR *szSrc, void *pDst, xt_size_t nDstSize, int nBase );
int xt_str2_unum( const TCHAR *szSrc, void *pDst, xt_size_t nDstSize, int nBase );

int xt_str_trim_left( TCHAR *szBuff, int nLen );
int xt_str_trim_right( TCHAR *szBuff, int nLen );
int xt_str_trim( TCHAR *szBuff, int nLen );

int GetStrLen( const TCHAR *szBuff, int nMaxLen );

int ConvertDirWin( TCHAR *szDir, int nBufLen );
int ConvertDirLnx( TCHAR *szDir, int nBufLen );

int MakeDirStrLook( TCHAR *szDir, int nBufLen );

int SplitPathFile( const TCHAR *szFullPath, TCHAR *szPath, TCHAR *szFile );

#ifdef __cplusplus
}
#endif

#endif /*__XT_STR_API_H__*/
