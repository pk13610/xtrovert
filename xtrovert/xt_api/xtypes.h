/************************************************************************************
* xtypes.h : header file
*
* Built-in data type Definitions header
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

#ifndef __X_TYPES_H__
#define __X_TYPES_H__

#include "xtchar.h"

#if defined(__WINDOWS__)
#include <wtypes.h> /* For "INT" alike data types */

#elif defined(__LINUX__)
#include <sys/types.h>

#ifdef __cplusplus
extern "C" {
#endif

#define CONST			const
#define VOID			void

typedef int                 INT;
typedef unsigned int        UINT;

/*typedef long long			INT64;*/
/*typedef unsigned long long	UINT64;*/
typedef int_64_t			INT64;
typedef uint64_t			UINT64


typedef short int           SHORT;
typedef unsigned short int  USHORT;
typedef USHORT				*PUSHORT;

typedef long int            LONG;
typedef unsigned long int   ULONG;
typedef ULONG				*PULONG;

typedef unsigned short      WORD;
typedef unsigned long       DWORD;

typedef float               FLOAT;
typedef FLOAT               *PFLOAT;

typedef int                 BOOL;

typedef float               FLOAT;
typedef double              DOUBLE;

typedef unsigned char       BYTE;

typedef char				*PSZ;

typedef char                CHAR;
typedef unsigned char       UCHAR;
typedef UCHAR				*PUCHAR;
typedef wchar_t				WCHAR;

#if defined(_WCHAR_DEFINED)
typedef WCHAR				TCHAR;
#else
typedef CHAR                TCHAR;
#endif

typedef VOID				*HANDLE;

/* Types use for passing & returning polymorphic values */
typedef UINT_PTR            WPARAM;
typedef LONG_PTR            LPARAM;
typedef LONG_PTR            LRESULT;

/*************************************************
*	NETWORK Data Types
*************************************************/

/*
typedef BOOL near           *PBOOL;
typedef BOOL far            *LPBOOL;
typedef BYTE near           *PBYTE;
typedef BYTE far            *LPBYTE;
typedef int near            *PINT;
typedef int far             *LPINT;
typedef WORD near           *PWORD;
typedef WORD far            *LPWORD;
typedef long far            *LPLONG;
typedef DWORD near          *PDWORD;
typedef DWORD far           *LPDWORD;
typedef void far            *LPVOID;
typedef CONST void far      *LPCVOID;
*/
#ifndef NOMINMAX

#ifndef max
#define max(a,b)            (((a) > (b)) ? (a) : (b))
#endif

#ifndef min
#define min(a,b)            (((a) < (b)) ? (a) : (b))
#endif

#endif  /* NOMINMAX */

#ifndef NULL
#ifdef __cplusplus
#define NULL    0
#else
#define NULL    ((void *)0)
#endif
#endif

#ifndef FALSE
#define FALSE               0
#endif

#ifndef TRUE
#define TRUE                1
#endif

#ifndef IN
#define IN
#endif

#ifndef OUT
#define OUT
#endif

#ifdef __cplusplus
}
#endif
#endif /* __LINUX__ */

#if defined(__WINDOWS__)
#ifdef __cplusplus
extern "C" {
#endif

#ifdef __cplusplus
}
#endif
#endif /* __WINDOWS__ */

typedef size_t			xt_size_t;

typedef void* xt_position_ptr_t;


#endif /*__X_TYPES_H__*/
