/************************************************************************************
* xt_file_api.h : header file
*
* File API Definition header
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
#ifndef __XT_FILE_API_H__
#define __XT_FILE_API_H__

#include "xt_file_define.h"

#if defined(__WINDOWS__)
#include "xt_ftw_win.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

/***********************************************************************
* Platform Independent File Access API
***********************************************************************/

int xt_file_create( const TCHAR *szFile, mode_t nMode );
int xt_file_open( const TCHAR *szFile, int nFlag, mode_t nMode );
int xt_file_read( int nFd, void *pBuff, UINT nByteCount );
int xt_file_write( int nFd, const void *pBuff, UINT nByteCount );
int xt_file_seek( int nFd, OFF_T nOffset, int nWhence );
int xt_file_close( int nFd );

int xt_file_access( const TCHAR *szFile, int nMode ); /*nMode=F_OK*/


int	xt_file_stat( const TCHAR *szPath, struct _stat *pStatBuf );
/* Difference between this function and the above one? Look up the definition of this file */
int	xt_file_lstat( const TCHAR *szPath, struct _stat *pStatBuf );

int xt_file_fstat( int nFd, struct _stat *pStatBuf );

xt_size_t xt_file_size( const TCHAR *szPath );
xt_size_t xt_file_size_fd( int nFd ); /* With File-Descriptor */

BOOL IsValidFd( int nFd );

int xt_copy_file( const TCHAR *szFileSrc, const TCHAR *szFileDest );
int RenameFile( const TCHAR *szFileSrc, const TCHAR *szFileDest );
int RemoveFile( const TCHAR *szFile );
/* Force to remove a file regardless of the target file's attributes */
int ForceRemoveFile( const TCHAR *szFile );
	
int ChMod( const TCHAR *szFile, mode_t mode );

/* DIR Functions */
int	xt_mkdir( const TCHAR *szPath );
int xt_mkdir_p( const TCHAR *szPath ); /* Make dir with parents */
int xt_rm( const TCHAR *szPath );
int xt_rename( const TCHAR *szPathSrc, const TCHAR *szPathDest );
int xt_chdir( const TCHAR *szPath );

int xt_get_exe_dir( TCHAR *szExeDir, int nLen );
int xt_get_cwd( TCHAR *szExeDir, int nLen );

int xt_set_cwd( const TCHAR *szExeDir, int nLen );

/*
int fcntl(int fd, int cmd);
int fcntl(int fd, int cmd, long arg);
int fcntl(int fd,int cmd, struct flock *lock);
*/
int xt_fcntl( int fd, int cmd );
/*int xt_fcntl_( int fd, int cmd, long arg );*/
/*int xt_fcntl( int fd, int cmd, struct flock *lock );*/

int xt_ioctl( int fd, int request, void *pArg );

int set_file_noblocking( int nFd, BOOL on );
BOOL get_file_blocking( int nFd );
int set_file_noblocking2( int nFd, BOOL on ); /* ioctl(fd, FIONBO, pArg) */

UINT get_file_available( int nFd );

/* 	
	int xt_ftw( const TCHAR *path, int(*fn)(const TCHAR*, const _STAT*, INT), intnfds );
 	int xt_ftw_cb_def( const TCHAR *szFile, const _STAT *file_state, intnFlag );
*/

#ifndef _XOPEN_SOURCE
	#define _XOPEN_SOURCE 500
#endif
	
int xt_nftw( const TCHAR *path, int(*fn) (const TCHAR *, const _STAT *, int, struct FTW *), int depth, int flags );
int xt_nftw_cb_def( const TCHAR *fname, const _STAT *sb, int flags, struct FTW *ftwbuf );


#ifdef __cplusplus
}
#endif

#endif /*__XT_FILE_API_H__*/
