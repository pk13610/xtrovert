/************************************************************************************
* platform.h : header file
*
* Type definitions for the basic sized types.
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

#ifndef __PLATFORM_H__
#define __PLATFORM_H__

#ifdef __cplusplus
extern "C" {
#endif

/************************************************************************************
* Compiler Built-in Macros for recognizing OS & COMPILER
*
* 	Operating System Macros:
*		Windows:	_WIN32
* 		Linux:		linux
* 		Solaris:	__sun
*
*	Compiler Macros:
* 		VC:			_MSC_VER
* 		gcc/g++:	__GNUC__
* 		SunCC:		__SUNPRO_C & __SUNPRO_CC
* __MINGW32__
* __CLR_VER (common language runtime)�İ汾
*_CPPLIB_VER 
*_INTEGRAL_MAX_BITS

_M_IX86		Defined for x86 processors. See Values for _M_IX86 for more details.

_M_IA64		Defined for Itanium Processor Family 64-bit processors.

_M_IX86_FP	Expands to a value indicating which /arch compiler option was used:

0 if /arch was not used.

1 if /arch:SSE was used.

2 if /arch:SSE2 was used.

See /arch (Minimum CPU Architecture) for more information.

_M_MPPC		Defined for Power Macintosh platforms (no longer supported).

_M_MRX000	Defined for MIPS platforms (no longer supported).

_M_PPC		Defined for PowerPC platforms (no longer supported).

_M_X64		Defined for x64 processors.

_MFC_VER	Defines the MFC version. For example, 0x0700 represents MFC version 7.

_MSC_VER	Reports the major and minor versions of the compiler. For example, 1310 for Microsoft Visual C++ .NET 2003. 1310 represents version 13 and a 1.0 point release. The Visual C++ 2005 compiler version is 1400.
			Type cl /? at the command line to see the major and minor versions of your compiler along with the build number.

_WIN32		Defined for applications for Win32 and Win64. Always defined.

_WIN64		Defined for applications for Win64.

_Wp64		Defined when specifying /Wp64.
************************************************************************************/

/************************************************************************************
* OS & Compilers Types
************************************************************************************/
/* If "linux" defined, "__LINUX__" would be defined automatically */
/* COMPILER_VER_STR_A -- "_A", String is in ASCII mode, indicated by macro */
#define __ver_mum_to_str(x)		#x
#define _ver_mum_to_str(x)		__ver_mum_to_str(x)

#if defined(linux) && defined(__GNUC__) /* linux & gcc/g++*/
/*
	#define GCC_VERSION (__GNUC__ * 10000 \
	+ __GNUC_MINOR__ * 100 \
	+ __GNUC_PATCHLEVEL__)
*/
#define __LINUX__
#define COMPILER_NAME_STR_A		__GNUC__
#define COMPILER_VER			GCC_VERSION
#define COMPILER_VER_STR_A		__VERSION__
/* If "WIN32" defined, "__WINDOWS__" would be defined automatically */
#elif defined(WIN32) && defined(_MSC_VER) /* Windows && VC */
#define __WINDOWS__
#define COMPILER_NAME_STR_A		"VC++"
#define COMPILER_VER			_MSC_FULL_VER
/*_MSC_FULL_VER*/
#define COMPILER_VER_STR_A		_ver_mum_to_str(COMPILER_VER)

#elif (defined(__sun) && (defined(__SUNPRO_C) || defined(__SUNPRO_CC))) /* SUN-OS && CC */
#define __SUN_OS__
#define COMPILER_NAME_STR_A		"__SUNPRO_CC"
#define COMPILER_VER			000
#define COMPILER_VER_STR_A		"???"
#endif
/************************************************************************************
* OS & Compilers Types END
************************************************************************************/

/************************************************************************************
*  Application compiled with Bits - 32?64bits (NOT OS or CPU bitS)
************************************************************************************/
#define __APP_BITS__					(sizeof(void*)*8)

/************************************************************************************
*  CPU Architecture
************************************************************************************/
#if !defined(_68K_) && !defined(_MPPC_) && !defined(_X86_) && !defined(_IA64_) && !defined(_AMD64_) && defined(_M_IX86)
#define _X86_
#endif

#if !defined(_68K_) && !defined(_MPPC_) && !defined(_X86_) && !defined(_IA64_) && !defined(_AMD64_) && defined(_M_AMD64)
#define _AMD64_
#endif

#if !defined(_68K_) && !defined(_MPPC_) && !defined(_X86_) && !defined(_IA64_) && !defined(_AMD64_) && defined(_M_M68K)
#define _68K_
#endif

#if !defined(_68K_) && !defined(_MPPC_) && !defined(_X86_) && !defined(_IA64_) && !defined(_AMD64_) && defined(_M_MPPC)
#define _MPPC_
#endif

#if !defined(_68K_) && !defined(_MPPC_) && !defined(_X86_) && !defined(_M_IX86) && !defined(_AMD64_) && defined(_M_IA64)
	#if !defined(_IA64_)
		#define _IA64_
	#endif /* !_IA64_ */
#endif

/*"$(CPU)" == "IA64"*/
#if defined(__i386__) || defined(__i386) || defined(i386) /* IA-32 */
#elif defined(__x86_64) && defined(__x86_64__) /* IA-64 */
#endif

/* 32 bits cpu */
#if defined(_X86_)
	#define XT_PACK_BYTES					4 /* default packs structs at 4 bytes */
	#define XT_APP_BITS						32
	#define _MEM_PAGE_SIZE_DEFAULT_			(4*1024) /* 2^2 * 2^10 = 4K */
#elif defined(_AMD64_) /* 64 bits cpu */
	#define XT_PACK_BYTES					8 /* default packs structs at 8 bytes */
	#define XT_APP_BITS						64
	#define _MEM_PAGE_SIZE_DEFAULT_			(8*1024) /* 2^3 * 2^10 = 8K */
#endif

#if defined(PAE)
#define _MEM_PAGE_SIZE_DEFAULT_				(4*1024*1024) /* 2^2 * 2^10 * 2^10 = 4M */
#endif

/* CPU ENDIAN */
/* ENDIANNESS='l'{little endian} 'b'{big endian} */
#if defined(__GNUC__) /* GNUC endian */
	/* GNU C: __BYTE_ORDER__ (__ORDER_LITTLE_ENDIAN__, __ORDER_BIG_ENDIAN__, or __ORDER_PDP_ENDIAN__) */
	#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
		#define ENDIANNESS			'l'
	#elif __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
		#define ENDIANNESS			'b'
	#else /* ??? __ORDER_PDP_ENDIAN__ ??? */
		static union
		{
			char c[4];
			unsigned int my_int;
		} endian_test = { { 'l', '?', '?', 'b' } };
		#define ENDIANNESS ((char)endian_test.my_int)
	#endif

#else /* Other Compiler endian */
	static union
	{
		char c[4];
		unsigned int my_int;
	} endian_test = { { 'l', '?', '?', 'b' } };
	#define ENDIANNESS ((char)endian_test.my_int)
#endif /* CPU ENDIAN */

/************************************************************************************
*  CPU Architecture END
************************************************************************************/

#if defined(__WINDOWS__)
#include <WinSock2.h> /* Included before <Windows.h> is required */
#include <Windows.h>
#endif

/*
Windows Server 2003                                WINVER>=0x0502
Windows XP                                            WINVER>=0x0501
Windows 2000                                          WINVER>=0x0500
Windows NT 4.0                                       WINVER>=0x0400
Windows Me                                            WINVER>=0x0500
Windows 98                                             WINVER>=0x0410
Windows 95                                             WINVER>=0x0400
*/

/* CPU_VENDOR_OS */

#ifdef  __cplusplus
}   /* ... extern "C" */
#endif

#endif /*__PLATFORM_H__*/
