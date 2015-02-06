/************************************************************************************
* xt_define.h : header file
*
* Definition header
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

#ifndef __XT_DEFINE_H__
#define __XT_DEFINE_H__

#include "xtypes.h"

#include <assert.h>

#if defined(__WINDOWS__)
/************************************************************************/
/* Ignore the following Warnings                                        */
/************************************************************************/
/* warning C4100: 'XXX' : unreferenced formal parameter */
#pragma warning (disable: 4127)
#pragma warning (disable: 4100)
#pragma warning (disable: 4996) /* Ignore: "wcstombs" warning */
#pragma warning (disable: 4200) /* Ignore:  warning C4200: nonstandard extension used : zero-sized array in struct/union */
#endif

#ifdef __cplusplus
extern "C" {
#endif

#define MAKEWORD(a, b)      ((WORD)(((BYTE)(((DWORD_PTR)(a)) & 0xff)) | ((WORD)((BYTE)(((DWORD_PTR)(b)) & 0xff))) << 8))
#define MAKELONG(a, b)      ((LONG)(((WORD)(((DWORD_PTR)(a)) & 0xffff)) | ((DWORD)((WORD)(((DWORD_PTR)(b)) & 0xffff))) << 16))
#define LOWORD(l)           ((WORD)(((DWORD_PTR)(l)) & 0xffff))
#define HIWORD(l)           ((WORD)((((DWORD_PTR)(l)) >> 16) & 0xffff))
#define LOBYTE(w)           ((BYTE)(((DWORD_PTR)(w)) & 0xff))
#define HIBYTE(w)           ((BYTE)((((DWORD_PTR)(w)) >> 8) & 0xff))

#define XT_SWAP(a,b)			\
	do {						\
		if ( (a) != (b) ) {		\
			(a) ^= (b);			\
			(b) ^= (a);			\
			(a) ^= (b);			\
		}						\
	} while(0)

#define XT_SWAP_PTR(pa,pb)					\
		do {								\
			void *tmp;						\
			void **ppb = (void**)(&(pb));	\
			tmp = (pa);						\
			(pa) = (pb);					\
			*ppb = tmp;						\
	} while(0)

#define XT_SWAP_BLOCK(pa,pb,size)							\
	do {													\
		for ( xt_size_t i=0; i<(size); ++i ) {				\
			XT_SWAP( ((char*)(pa))[i], ((char*)(pb))[i] );	\
		}													\
	} while(__LINE__==-1)


#define _MAX_STR_LEN_8_					8
#define _MAX_STR_LEN_16_				16
#define _MAX_STR_LEN_32_				32
#define _MAX_STR_LEN_64_				64
#define _MAX_STR_LEN_128_				128
#define _MAX_STR_LEN_1024_				1024
#define _MAX_STR_LEN_64K_				(64*_MAX_STR_LEN_1024_) /* 64k TCHAR */

#define _INVALID_SIZE_T_VALUE_				((xt_size_t)(~0))
#define _INVALID_ELEMENT_COUNT_				((xt_size_t)(~0))

#define _ELEMENT_COUNT_DEFAULT_			(_MAX_STR_LEN_1024_)

/* Return values for function calling */
#define FUN_RET_OK					0	/* Function returned normally, with NO error occurs. */
#define FUN_RET_ERR_UNKNOWN			-1	/* Function returned unknown error, need errno to get further information. */
#define FUN_RET_ERR_ILLEGAL_ARGS	-2	/* Illegal parameters passed into function. */
#define FUN_RET_ERR_ILLEGAL_CALL	-3	/* Causes illegal operations while calling this function. Obsoleted */
#define FUN_RET_ERR_NOTIMPL			-4	/* Function or method NOT supported. */

#define _SYMBOL_SLASH_				_T('/')
#define _SYMBOL_BACK_SLASH_			_T('\\')

#define XT_MAKE_STR(x)				#x

extern const TCHAR _SYMBOL_DIR_LINUX_STD_;
extern const TCHAR _SYMBOL_DIR_WIN_STD_;

extern TCHAR szLastSysCall[_MAX_STR_LEN_128_];
#define MARK_CALL_FUN(STR_FUN) \
	memset( szLastSysCall, 0, sizeof(szLastSysCall) );\
	memcpy( szLastSysCall, STR_FUN, _tcslen(STR_FUN)*sizeof(TCHAR) )

void xt_srand(void);

/* This macro is used to reduce size requirements of some classes */
#if !defined(XT_NOVTABLE)
	#if !defined(_DEBUG) && defined(__WINDOWS__)
		#define XT_NOVTABLE __declspec(novtable)
	#else
		#define XT_NOVTABLE
	#endif
#endif

/* XT_CDECL is used for rare functions taking variable arguments */
#ifndef XT_CDECL
#define XT_CDECL __cdecl
#endif

#ifndef XT_STDECL
#define XT_STDECL __stdcall
#endif

#if !defined(__WINDOWS__)
	#define XT_DECLARE(type)            type
	#define XT_DECLARE_NONSTD(type)     type
	#define XT_DECLARE_DATA
#elif defined(XT_DECLARE_EXPORT)
	#define XT_DECLARE(type)            __declspec(dllexport) type __stdcall
	#define XT_DECLARE_NONSTD(type)     __declspec(dllexport) type __cdecl
	#define XT_DECLARE_DATA             __declspec(dllexport)
#elif defined(XT_DECLARE_IMPORT)
	#define XT_DECLARE(type)            __declspec(dllimport) type __stdcall
	#define XT_DECLARE_NONSTD(type)     __declspec(dllimport) type __cdecl
	#define XT_DECLARE_DATA             __declspec(dllimport)
#else
	#define XT_DECLARE(type)            type __stdcall
	#define XT_DECLARE_NONSTD(type)     type __cdecl
	#define XT_DECLARE_DATA
#endif

#if defined(__LINUX__)

#define UNREFERENCED_PARAMETER(P)          (P)
#define DBG_UNREFERENCED_PARAMETER(P)      (P)
#define DBG_UNREFERENCED_LOCAL_VARIABLE(V) (V)

/* Eliminate warning C4100: 'XXX' : unreferenced formal parameter */
#if defined(_DEBUG)
#define UNUSED(x)
#else
#define UNUSED(x) UNREFERENCED_PARAMETER(x)
#endif

#define UNUSED_ALWAYS(x) UNREFERENCED_PARAMETER(x)

#endif /* __LINUX__ */

#if defined(__WINDOWS__)
#define XT_PTM_WARNING_DISABLE \
	__pragma(warning( push )) \
	__pragma(warning( disable : 4867 ))
#define XT_PTM_WARNING_RESTORE \
	__pragma(warning( pop ))
#else
#define XT_PTM_WARNING_DISABLE
#define XT_PTM_WARNING_RESTORE
#endif
#endif /* __WINDOWS__ */

#ifdef __cplusplus
} /* ... extern "C" */
#endif

#endif /*__XT_DEFINE_H__*/
