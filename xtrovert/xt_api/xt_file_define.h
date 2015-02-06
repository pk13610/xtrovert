/************************************************************************************
* xt_file_define.h : header file
*
* FileDefine header
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

#ifndef __XT_FILE_DEFINE_H__
#define __XT_FILE_DEFINE_H__

#include "xt_define.h"

#include <sys/stat.h>
#include <fcntl.h> // for fcntl(), for open/close/read/write


#if defined(__LINUX__)
#include <sys/ioctl.h> // for ioctl()
#elif defined(__WINDOWS__)
#include <io.h> // for open()
#endif

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************************/
#if defined(__LINUX__)
/******************************************************************************/
/* FUNCTIONS */
#define _tcreat			creat
#define _topen			open

#define _close			close

#define _tfopen			fopen

#define _read			read
#define _write			write
#define _lseek			lseek

#define _fgetts			fgets

#define _trename		rename
#define _tremove		remove
#define _taccess		access
#define _tchmod			chmod
#define _tstat			stat
#define _tfstat			fstat

#define _tmkdir			mkdir
#define _trmdir			rmdir

/* MACROs */
#define _O_BINARY		0x0
#define _O_APPEND		O_APPEND
#define _O_RDONLY		O_RDONLY
#define _O_WRONLY		O_WRONLY
#define _O_RDWR			O_RDWR
#define _O_CREAT		O_CREAT
#define _O_TRUNC		O_TRUNC

#endif /*defined(__LINUX__)*/

#if defined(__WINDOWS__)
#include <direct.h>

typedef INT				mode_t;

/******************************************************************************/
/* FILE		MACROs */
#define S_IRWXU			(_S_IWRITE|_S_IREAD)
#define S_IRUSR			_S_IREAD
#define S_IWUSR			_S_IWRITE
#define S_IXUSR			(_S_IWRITE|_S_IREAD)

#define S_IRWXG			(_S_IWRITE|_S_IREAD)
#define S_IRGRP			_S_IREAD
#define S_IWGRP			_S_IWRITE
#define S_IXGRP			(_S_IWRITE|_S_IREAD)

#define S_IRWXO			(_S_IWRITE|_S_IREAD)
#define S_IROTH			_S_IREAD
#define S_IWOTH			_S_IWRITE
#define S_IXOTH			(_S_IWRITE|_S_IREAD)
/******************************************************************************/
/******************************************************************************/
/* For function "FileAccess" */
#define F_OK		0 /* File exits */
#define R_OK		2 /* Read permission */
#define W_OK		4 /* Write Permission */
#define X_OK		0 /* Execute test under LINUX, MS Windows did NOT test this,
						so leave it here for compliance */
/******************************************************************************/

#endif /*defined(__WINDOWS__)*/

typedef off_t	OFF_T;
#if defined(_M_X64) || defined(_M_X64)
	typedef struct _stat64	_STAT;
#else
	typedef struct stat	_STAT;
#endif

/******************************************************************************/
/******************************************************************************/
#define INVALID_FILE_FD							-1

/******************************************************************************/
/* File ERROR Definition */
/******************************************************************************/
#define ERR_FILE_OPENNED						-4
#define ERR_FILE_INVALID_FD						-5


#ifndef INVALID_FILE_SIZE
#define INVALID_FILE_SIZE ((xt_size_t)(~0))
#endif

	/* End of Line */
#if defined(__LINUX__)
	#define _xt_TXT_FILE_EOL_					_T("\n")
	#define _xt_IS_EOL_(_tchar)				(_tchar == _T('\n'))
#elif defined(__WINDOWS__)
	#define _xt_TXT_FILE_EOL_					EOL /*_T("\r\n")*/
	#define _xt_IS_EOL_(_tchar)				(_tchar==_T('\r') || _tchar==_T('\n'))
#endif

typedef struct _tag_xt_file_t
{
	int fd;
}xt_file_t;

#ifdef __cplusplus
}
#endif

#endif /*__XT_FILE_DEFINE_H__*/
