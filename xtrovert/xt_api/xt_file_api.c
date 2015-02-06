/************************************************************************************
* xt_file_api.c: Implementation File
*
* DESCRIPTION  :   
*
* AUTHOR	    :	Sean Feng <SeanFeng2006@hotmail.com>
* DATE		    :	Sep. 24, 2010
*
* Copyright (c) 2010-?. All Rights Reserved.
*
* REMARKS:
*		
*      
************************************************************************************/

#include "xt_file_api.h"
#include "xt_str_api.h"

#if defined(__WINDOWS__)
/* warning C4100: 'XXX' : unreferenced formal parameter */
#pragma warning (disable: 4100)
#endif

#ifdef __WINDOWS__
#include "fcntl.h"
#elif __LINUX__
#endif

int xt_file_create( const TCHAR *szFile, mode_t nMode )
{
	/************************************************************************
	* Parameters:
	*
	* Return Value:
	* 	
	* Remarks:
	************************************************************************/
	int nRet = FUN_RET_OK;

	if ( szFile == NULL )
	{
		return FUN_RET_ERR_ILLEGAL_ARGS;
	}

	nRet = _tcreat( szFile, nMode );
	if ( nRet == -1 )
	{
		MARK_CALL_FUN( _T(__FUNCTION__) );
	}

	return nRet;
}

int xt_file_open( const TCHAR *szFile, int nFlag, mode_t nMode )
{
	/************************************************************************
	* Parameters:
	*
	* Return Value:
	* 	
	* Remarks:
	************************************************************************/
	int nRet = FUN_RET_OK;
	int nBinMode = 0; /* Linux doesn't care about this */

	if ( szFile == NULL )
	{
		return FUN_RET_ERR_ILLEGAL_ARGS;
	}

	/************************************************************************/
	nBinMode = 0; /* Linux doesn't care about this */
#if defined(__WINDOWS__)
	nBinMode = _O_BINARY; /* Opening file with text mode in FUCKING Windows causes ridiculous result. */
#endif
	/************************************************************************/
	nRet = _topen( szFile, nFlag|nBinMode, nMode );
	if ( nRet == -1 )
	{
		MARK_CALL_FUN( _T(__FUNCTION__) );
	}

	return nRet;
}

int xt_file_read( int nFd, void *pBuff, UINT nByteCount )
{
	/************************************************************************
	* Parameters:
	*
	* Return Value:
	* 	
	* Remarks:
	************************************************************************/
	int nRet = FUN_RET_OK;

	if ( !IsValidFd(nFd) || pBuff==NULL )
	{
		return FUN_RET_ERR_ILLEGAL_ARGS;
	}

	if ( nByteCount == 0 )
	{
		return FUN_RET_OK;
	}

	nRet = _read( nFd, pBuff, nByteCount );
	if ( nRet == -1 )
	{
		MARK_CALL_FUN( _T(__FUNCTION__) );
	}

	return nRet;
}

int xt_file_write( int nFd, const void *pBuff, UINT nByteCount )
{
	/************************************************************************
	* Parameters:
	*
	* Return Value:
	* 	
	* Remarks:
	************************************************************************/
	int nRet = FUN_RET_OK;

	if ( !IsValidFd(nFd) || pBuff==NULL )
	{
		return FUN_RET_ERR_ILLEGAL_ARGS;
	}

	if ( nByteCount == 0 )
	{
		return FUN_RET_OK;
	}

	nRet = _write( nFd, pBuff, nByteCount );
	if ( nRet == -1 )
	{
		MARK_CALL_FUN( _T(__FUNCTION__) );
	}

	return nRet;
}

int xt_file_seek( int nFd, OFF_T nOffset, int nWhence )
{
	/************************************************************************
	* Parameters:
	*
	* Return Value:
	* 	
	* Remarks:
	************************************************************************/
	int nRet = FUN_RET_OK;

	if ( !IsValidFd(nFd) || nWhence<SEEK_SET ||  nWhence>SEEK_END )
	{
		return FUN_RET_ERR_ILLEGAL_ARGS;
	}

	nRet = _lseek( nFd, nOffset, nWhence );
	if ( nRet == -1 )
	{
		MARK_CALL_FUN( _T(__FUNCTION__) );
	}

	return nRet;
}

int xt_file_close( int nFd )
{
	/************************************************************************
	* Parameters:
	*
	* Return Value:
	* 	
	* Remarks:
	************************************************************************/
	int nRet = FUN_RET_OK;

	if ( !IsValidFd(nFd) )
	{
		return FUN_RET_ERR_ILLEGAL_ARGS;
	}

	nRet = _close( nFd );
	if ( nRet == -1 )
	{
		MARK_CALL_FUN( _T(__FUNCTION__) );
	}

	return nRet;
}

int xt_file_access( const TCHAR *szFile, int nMode )
{
	/************************************************************************
	* Parameters:
	*
	* Return Value:
	* 	
	* Remarks:
	************************************************************************/
	int nRet = FUN_RET_OK;

	if ( szFile == NULL )
	{
		return FUN_RET_ERR_ILLEGAL_ARGS;
	}

	nRet = _taccess( szFile, nMode );
	if ( nRet == -1 )
	{
		MARK_CALL_FUN( _T(__FUNCTION__) );
	}

	return nRet;
}

int xt_file_stat( const TCHAR *szPath, struct _stat *pStatBuf )
{
	/************************************************************************
	* Parameters:
	*
	* Return Value:
	* 	
	* Remarks:
	************************************************************************/
	int nRet = FUN_RET_OK;

	if ( szPath==NULL || pStatBuf==NULL )
	{
		return FUN_RET_ERR_ILLEGAL_ARGS;
	}

	nRet = _tstat( szPath, pStatBuf );
	if ( nRet == -1 )
	{
		MARK_CALL_FUN( _T(__FUNCTION__) );
	}

	return nRet;
}

int xt_file_lstat( const TCHAR *szPath, struct _stat *pStatBuf )
{
	/************************************************************************/
	/* Return Value:				                                        */
	/*	0 for success			                                            */
	/*	errno for error occurs                                              */
	/* Remarks:																*/
	/*	This function is  identical  to  file_stat(),  except that if		*/
	/*	"szPath" is a symbolic link, then the link itself is stat-ed, not   */
	/*	the file that it refers to.											*/
	/************************************************************************/	
	int nRet = FUN_RET_OK;

	if ( szPath==NULL || pStatBuf==NULL )
	{
		return FUN_RET_ERR_ILLEGAL_ARGS;
	}

	/*nRet = _lstat( szPath, pStatBuf );*/
	if ( nRet == -1 )
	{
		MARK_CALL_FUN( _T(__FUNCTION__) );
	}

	return nRet;
}

int xt_file_fstat( int nFd, struct _stat *pStatBuf )
{
	/************************************************************************
	* Parameters:
	*
	* Return Value:
	* 	
	* Remarks:
	************************************************************************/
	int nRet = FUN_RET_OK;

	if ( !IsValidFd(nFd) || pStatBuf==NULL )
	{
		return FUN_RET_ERR_ILLEGAL_ARGS;
	}

	//nRet = _fstat( nFd, pStatBuf );
	if ( nRet == -1 )
	{
		MARK_CALL_FUN( _T(__FUNCTION__) );
	}

	return nRet;
}

xt_size_t xt_file_size( const TCHAR *szPath )
{
	struct _stat StatBuf;

	if ( xt_file_stat(szPath, &StatBuf) < 0 )
	{
		return INVALID_FILE_SIZE;
	}

	return (xt_size_t)StatBuf.st_size;
}

xt_size_t xt_file_size_fd( int nFd )
{
	struct _stat StatBuf;

	if ( xt_file_fstat(nFd, &StatBuf) < 0 )
	{
		return INVALID_FILE_SIZE;
	}

	return (xt_size_t)StatBuf.st_size;
}

BOOL IsValidFd( int nFd )
{
	return (nFd!=INVALID_FILE_FD); 
}

int xt_fcntl( int fd, int cmd )
{
#if defined(__LINUX__)
	int nRet = FUN_RET_OK;
	nRet = fcntl( fd, cmd );
	if ( nRet < 0 )
	{
		switch ( errno )
		{
		case xt_STD_ERR_EINTR:
		case xt_STD_ERR_EBADF:
		case xt_STD_ERR_EACCES:
		case xt_STD_ERR_EAGAIN:
		case xt_STD_ERR_EDEADLK:
		case xt_STD_ERR_EMFILE:
		case xt_STD_ERR_ENOLCK:
		default:
			break;
		}
	}
	return nRet;
#elif defined(__WINDOWS__)
	return FUN_RET_OK;
#endif
}
/*
int xt_fcntl( int fd, int cmd, LONG arg )
{
#if defined(__LINUX__)
	int nRet = FUN_RET_OK;
	nRet = fcntl( fd, cmd, arg );
	if ( nRet < 0 )
	{
		switch ( errno )
		{
		case xt_STD_ERR_EINTR:
		case xt_STD_ERR_EBADF:
		case xt_STD_ERR_EACCES:
		case xt_STD_ERR_EAGAIN:
		case xt_STD_ERR_EDEADLK:
		case xt_STD_ERR_EMFILE:
		case xt_STD_ERR_ENOLCK:
		default:
			break;
		}
	}
	return nRet;
#elif defined(__WINDOWS__)
	return FUN_RET_OK;
#endif
}

int xt_fcntl( int fd, int cmd, struct flock *lock )
{
	return FUN_RET_OK;
}
*/
int xt_ioctl( int fd, int request, void *pArg )
{
#if defined(__LINUX__)
	int nRet = FUN_RET_OK;
	nRet = ioctl( fd, cmd, pArg );
	if ( nRet < 0 )
	{
		switch ( errno )
		{
		case xt_STD_ERR_EBADF:
		case xt_STD_ERR_ENOTTY:
		case xt_STD_ERR_EINVAL:
		default:
			break;
		}
	}
	return nRet;
#elif defined(__WINDOWS__)
	return FUN_RET_OK;
#endif
}

int set_file_noblocking( int nFd, BOOL on )
{
#if defined(__LINUX__)
	int nFlags = fcntl( nFd, F_GETFL, 0 );
	if ( on )
	{
		nFlags |= O_NONBLOCK;
	}
	else
	{
		nFlags |= ~O_NONBLOCK;
	}
	fcntl( nFd, F_SETFL, nFlags );
#elif defined(__WINDOWS__)
#endif
	return FUN_RET_OK;
}

BOOL get_file_blocking( int nFd )
{
	/*
	TRUE: blocked
	FALSE: non-blocked
	*/
#if defined(__LINUX__)
	int nFlags = fcntl( nFd, F_GETFL, 0 );
	return nFlags | O_NONBLOCK;
#elif defined(__WINDOWS__)
	return FALSE;
#endif
}

int set_file_noblocking2( int nFd, BOOL on ) /* ioctl(fd, FIONBO, pArg) */
{
	OFF_T arg = ((on)?1:0);
	return xt_ioctl( nFd, FIONBIO, &arg );
}

UINT get_file_available( int nFd )
{
#if defined(__LINUX__)
	UINT arg = 0;
	ioctl( nFd, FIONREAD, &arg );
	return arg;
#elif defined(__WINDOWS__)
	return 0;
#endif
}

/************************************************************************/
/************************************************************************/
int xt_copy_file( const TCHAR *szFileSrc, const TCHAR *szFileDest )
{
	/************************************************************************
	* Parameters:
	*
	* Return Value:
	* 	
	* Remarks:
	************************************************************************/
	int nRet = FUN_RET_OK;

	if ( szFileSrc==NULL || szFileDest==NULL )
	{
		return FUN_RET_ERR_ILLEGAL_ARGS;
	}

	return nRet;
}
int RenameFile( const TCHAR *szFileSrc, const TCHAR *szFileDest )
{
	/************************************************************************
	* Parameters:
	*
	* Return Value:
	* 	
	* Remarks:
	************************************************************************/
	int nRet = FUN_RET_OK;

	if ( szFileSrc==NULL || szFileDest==NULL )
	{
		return FUN_RET_ERR_ILLEGAL_ARGS;
	}

	nRet = _trename( szFileSrc, szFileDest );
	if ( nRet != 0 )
	{
		MARK_CALL_FUN( _T(__FUNCTION__) );
	}

	return nRet;
}

int RemoveFile( const TCHAR *szFile )
{
	/************************************************************************
	* Parameters:
	*
	* Return Value:
	* 	
	* Remarks:
	************************************************************************/	
	int nRet = FUN_RET_OK;

	if ( szFile == NULL )
	{
		return FUN_RET_ERR_ILLEGAL_ARGS;
	}
	
	nRet = _tremove( szFile );
	if ( nRet == -1 )
	{
		MARK_CALL_FUN( _T(__FUNCTION__) );
	}

	return nRet;
}

int ForceRemoveFile( const TCHAR *szFile )
{
	/************************************************************************
	* Parameters:
	*
	* Return Value:
	* 	
	* Remarks:
	*	*Force to remove a file regardless of the target file's attributes.
	************************************************************************/	
	int nRet = FUN_RET_OK;

	if ( szFile == NULL )
	{
		return FUN_RET_ERR_ILLEGAL_ARGS;
	}

#ifdef __WINDOWS__
	/* Remove attributes of this file */
	//nRet = _tchmod( szFile, _S_IWRITE );
	if ( nRet < 0 )
	{
		MARK_CALL_FUN( _T(__FUNCTION__) );
	}
#endif
	/* Remove this file */
	nRet = RemoveFile( szFile );

	return nRet;
}

int ChMod( const TCHAR *szFile, mode_t mode )
{
	/************************************************************************
	* Parameters:
	*
	* Return Value:
	* 	
	* Remarks:
	************************************************************************/
	int nRet = FUN_RET_OK;

	if ( szFile == NULL )
	{
		return FUN_RET_ERR_ILLEGAL_ARGS;
	}
	
	nRet = _tchmod( szFile, mode );
	if ( nRet < 0 )
	{
		MARK_CALL_FUN( _T(__FUNCTION__) );
	}

	return nRet;
}

int xt_mkdir( const TCHAR *szPath )
{
	/************************************************************************
	* Parameters:
	*
	* Return Value:
	* 	
	* Remarks:
	************************************************************************/
#if defined(__WINDOWS__)
	return _tmkdir(szPath);
#elif defined(__LINUX__)
	return mkdir(szPath,0755);
#else
	return -3;
#endif
}

int xt_mkdir_p( const TCHAR *szPath )
{
	/************************************************************************
	* Parameters:
	*
	* Return Value:
	* 	
	* Remarks:
	************************************************************************/
	int nRet = FUN_RET_OK;
	int i = 0;
	int len = 0;
	TCHAR *dir_tmp = NULL;

	if( NULL == szPath )
	{
		return 0;
	}

	dir_tmp = _tcsdup(szPath);
	len = _tcslen(dir_tmp);

	for ( i=0; i<len; ++i )
	{
		if ( dir_tmp[i]==_T('\\') || dir_tmp[i]==_T('/') )
		{ 
			dir_tmp[i] = _T('\0');

			nRet = _taccess( dir_tmp,F_OK );
			if ( nRet != 0 )
			{
				nRet = _tmkdir(dir_tmp);
				if ( nRet != 0 )
				{
					return -1;
				} 
			}
			// To be comply to Linux, change dir symbol '\\' to '/'
			dir_tmp[i] = _T('/');
		} 
	}

	nRet = _tmkdir(dir_tmp);
	free(dir_tmp);

	return nRet;
}

int xt_rm( const TCHAR *szPath )
{
	/************************************************************************
	* Parameters:
	*
	* Return Value:
	* 	
	* Remarks:
	************************************************************************/
	//int nRet = FUN_RET_OK;
	
	return _tunlink(szPath);
}

int xt_rename( const TCHAR *szPath, const TCHAR *szPathNew )
{
	/************************************************************************
	* Parameters:
	*
	* Return Value:
	* 	
	* Remarks:
	************************************************************************/
	int nRet = FUN_RET_OK;
	return nRet;
}

int xt_chdir( const TCHAR *szPath )
{
	/************************************************************************
	* Parameters:
	*
	* Return Value:
	* 	
	* Remarks:
	************************************************************************/
	//int nRet = FUN_RET_OK;	
	return _tchdir(szPath);
}

int xt_get_exe_dir( TCHAR *szExeDir, int nLen )
{
	///////////////////////////////////////////////////////////////////////
	// Parameters:
	//	szExeDir [out]: a path string, like ""C:\Dir1\dir2\", 
	//					the last character is a '\\'
	// Return Value:
	//	*If the function succeeds, the return value is length of the 
	//		string that is copied to the buffer
	//	*If any errors occurred, a negative integer will be returned 
	///////////////////////////////////////////////////////////////////////
	int nRet = FUN_RET_OK;
#if defined(__WINDOWS__)
	TCHAR *pStr = NULL;
#elif defined(__LINUX__)
	CHAR *pStr = NULL;
	CHAR *pTmp = NULL;
#endif

	if ( szExeDir == NULL || nLen <= 0 )
	{
		return FUN_RET_ERR_ILLEGAL_ARGS;
	}

	memset( (void*)szExeDir,0, nLen*sizeof(TCHAR) );
#if defined(__WINDOWS__)
	pStr = szExeDir;
	nRet = GetModuleFileName(NULL, szExeDir, nLen);
	if ( nRet <= 0 )
	{
		nRet = -1*GetLastError();
		return nRet;
	}
	pStr += nRet;
	while( pStr!=szExeDir && (*pStr!=_SYMBOL_BACK_SLASH_ && *pStr!=_SYMBOL_SLASH_) )
	{
		*pStr = 0;
		pStr--;
	}
	nRet = pStr - szExeDir;

#elif defined(__LINUX__)
	pStr = (CHAR*)szExeDir;
	pTmp = (CHAR*)szExeDir;
#if defined(_TCHAR_DEFINED)
	CHAR szPathTmp[MAX_PATH] = {0};
	pTmp = szPathTmp;
	pStr = szPathTmp;
#endif
	nRet = readlink( "/proc/self/exe", pTmp );
	if ( nRet < 0 )
	{
		nRet = -1*errno;
	}
	pTmp += nRet;
	while( pStr != pTmp && *pTmp!=_SYMBOL_SLASH_ )
	{
		*pTmp = 0;
		pTmp--;
	}
	nRet = pTmp - pStr;
#if defined(_TCHAR_DEFINED)
	else
	{
		if ( mbstowcs(szExeDir, szPathTmp, nRet) != nRet )
		{
			return -5;
		}
	}
#endif

#else
	return -3;
#endif

	return nRet;
}

int xt_get_cwd( TCHAR *szExeDir, int nLen )
{
	///////////////////////////////////////////////////////////////////////
	// Parameters:
	//	szExeDir [out]: a path string, like ""C:\Dir1\dir2\", 
	//					the last character is a '\\'
	// Return Value:
	//	*If the function succeeds, the return value is length of the 
	//		string that is copied to the buffer
	//	*If any errors occurred, a negative integer will be returned 
	///////////////////////////////////////////////////////////////////////
	int nRet = FUN_RET_OK;
	if ( szExeDir == NULL || nLen <= 0 )
	{
		return FUN_RET_ERR_ILLEGAL_ARGS;
	}

	memset( (void*)szExeDir,0, nLen*sizeof(TCHAR) );

#if defined(__WINDOWS__)
	if ( _tgetcwd(szExeDir,nLen) == NULL )
	{
		return -1*GetLastError();
	}

	nRet = _tcslen( szExeDir );
#elif defined(__LINUX__)
	CHAR *pTmp = (CHAR*)szExeDir;
#if defined(_TCHAR_DEFINED)
	CHAR szPathTmp[MAX_PATH] = {0};
	pTmp = szPathTmp;
#endif
	nRet = getcwd( pTmp, nLen );
	if ( nRet < 0 )
	{
		nRet = -1*errno;
	}
#if defined(_TCHAR_DEFINED)
	else
	{
		if ( mbstowcs(szExeDir, szPathTmp, nRet) != nRet )
		{
			return -5;
		}
	}
#endif

#else
	return -3;
#endif

	return nRet;
}

int xt_set_cwd( const TCHAR *szExeDir, int nLen )
{
	return FUN_RET_OK;
}

// int xt_ftw_cb_def( const TCHAR *szFile, const _STAT *file_state, int nFlag )
// {
// 	return FUN_RET_OK;
// }
// 
// int xt_ftw( const TCHAR *path, int (*fn)(const TCHAR*, const _STAT*, INT), int nfds )
// {
// #if defined(__LINUX__)
// 	return ftw( path, fn, nfds );
// #else
// #endif
// // 	TCHAR * const paths[2] = { (TCHAR *)path, NULL };
// // 	FTSENT *cur;
// // 	FTS *ftsp;
// // 	int error = 0, fnflag, sverrno;
// // 
// // #if 0
// // 	/** GLibc allows nfds < 1 and treats it as nfds == 1.  Since nfds is
// // 	   not used in this OpenBSD version anyway, just ignore it for
// // 	   Linux compatibility. */
// // 	/** XXX - nfds is currently unused */
// // 	if (nfds < 1 || nfds > OPEN_MAX) {
// // 		errno = EINVAL;
// // 		return (-1);
// // 	}
// // #endif
// // 
// // 	ftsp = fts_open(paths, FTS_LOGICAL | FTS_COMFOLLOW | FTS_NOCHDIR, NULL);
// // 	if (ftsp == NULL)
// // 		return (-1);
// // 	while ((cur = fts_read(ftsp)) != NULL) {
// // 		switch (cur->fts_info) {
// // 		case FTS_D:
// // 			fnflag = FTW_D;
// // 			break;
// // 		case FTS_DNR:
// // 			fnflag = FTW_DNR;
// // 			break;
// // 		case FTS_DP:
// // 			/** we only visit in preorder */
// // 			continue;
// // 		case FTS_F:
// // 		case FTS_DEFAULT:
// // 			fnflag = FTW_F;
// // 			break;
// // 		case FTS_NS:
// // 		case FTS_NSOK:
// // 		case FTS_SLNONE:
// // 			fnflag = FTW_NS;
// // 			break;
// // 		case FTS_SL:
// // 			fnflag = FTW_SL;
// // 			break;
// // 		case FTS_DC:
// // 			errno = ELOOP;
// // 			/** FALLTHROUGH */
// // 		default:
// // 			error = -1;
// // 			goto done;
// // 		}
// // 		error = fn(cur->fts_path, cur->fts_statp, fnflag);
// // 		if (error != 0)
// // 			break;
// // 	}
// // done:
// // 	sverrno = errno;
// // 	if (fts_close(ftsp) != 0 && error == 0)
// // 		error = -1;
// // 	else
// // 		errno = sverrno;
// // 	return (error);
// 
// 	return FUN_RET_OK;
// }

#if defined(__WINDOWS__)
typedef struct _ftsent {
	USHORT		 fts_info;     /* flags for FTSENT structure */
	TCHAR          *fts_accpath;  /* access path */
	TCHAR          *fts_path;     /* root path */
	SHORT          fts_pathlen;  /* strlen(fts_path) */
	TCHAR          *fts_name;     /* filename */
	SHORT          fts_namelen;  /* strlen(fts_name) */
	SHORT          fts_level;    /* depth (-1 to N) */
	int            fts_errno;    /* file errno */
	LONG           fts_number;   /* local numeric value */
	void          *fts_pointer;  /* local address value */
	struct ftsent *fts_parent;   /* parent directory */
	struct ftsent *fts_link;     /* next file structure */
	struct ftsent *fts_cycle;    /* cycle structure */
	_STAT		   *fts_statp;    /* stat(2) information */
} FTSENT;

typedef struct {
	struct _ftsent *fts_cur; /* current node */
	struct _ftsent *fts_child; /* linked list of children */
	struct _ftsent **fts_array; /* sort array */
	_dev_t fts_dev; /* starting device # */
	TCHAR *fts_path; /* path for this descent */
	INT	 fts_rfd; /* fd for root */
	_off_t fts_pathlen; /* sizeof(path) */
	_off_t fts_nitems; /* elements in the sort array */
	int (*fts_compar) /* compare function */
		(const struct _ftsent * const *, const struct _ftsent * const *);

#define FTS_COMFOLLOW 0x001 /* follow command line symlinks */
#define FTS_LOGICAL 0x002 /* logical walk */
#define FTS_NOCHDIR 0x004 /* don't change directories */
#define FTS_NOSTAT 0x008 /* don't get stat info */
#define FTS_PHYSICAL 0x010 /* physical walk */
#define FTS_SEEDOT 0x020 /* return dot and dot-dot */
#define FTS_XDEV 0x040 /* don't cross devices */
#define FTS_WHITEOUT 0x080 /* return whiteout information */
#define FTS_OPTIONMASK 0x0ff /* valid user option mask */

#define FTS_NAMEONLY 0x100 /* (private) child names only */
#define FTS_STOP 0x200 /* (private) unrecoverable error */
	int fts_options; /* fts_open options, global flags */
	void *fts_clientptr; /* thunk for sort function */
} FTS;
#endif

int xt_nftw_cb_def( const TCHAR *fname, const _STAT *sb, int flags, struct FTW *ftwbuf )
{
	/*
	int flags:
	FTW_F
	The object is a file.
	FTW_D
	The object is a directory.
	FTW_DP
	The object is a directory and subdirectories have been visited. (This condition will only occur if the FTW_DEPTH flag is included in flags.)
	FTW_SL
	The object is a symbolic link. (This condition will only occur if the FTW_PHYS flag is included in flags.)
	FTW_SLN
	The object is a symbolic link that does not name an existing file. (This condition will only occur if the FTW_PHYS flag is not included in flags.)
	FTW_DNR
	The object is a directory that cannot be read. The fn() function will not be called for any of its descendants.
	FTW_NS
	The stat() function failed on the object because of lack of appropriate permission. The stat buffer passed to fn() is undefined. Failure of stat() for any other reason is considered an error and nftw() returns -1.
	*/
	/************************************************************************
	 Return Value:
		* xt_nftw() return what my return value.
		* 0 : To tell xt_nftw() to continue
		* non-zero : To tell xt_nftw() to finish
	************************************************************************/
// 	printf("%-3s %2d %7jd   %-40s %d %s\n",
// 		(flags == FTW_D) ?   "d"   : (flags == FTW_DNR) ? "dnr" :
// 		(flags == FTW_DP) ?  "dp"  : (flags == FTW_F) ?   "f" :
// 		(flags == FTW_NS) ?  "ns"  : (flags == FTW_SL) ?  "sl" :
// 		(flags == FTW_SLN) ? "sln" : "???",
// 		ftwbuf->level, (intmax_t) sb->st_size,
// 		fpath, ftwbuf->base, fpath + ftwbuf->base);
	return 0; /* To tell xt_nftw() to continue */
}

int xt_nftw( const TCHAR *path, int (*fn) (const TCHAR *, const _STAT *, int, struct FTW *), int depth, int flags )
{
	/*
	int flags:
	FTW_CHDIR
	If set, nftw() will change the current working directory to each directory as it reports files in that directory. If clear, nftw() will not change the current working directory.
	FTW_DEPTH
	If set, nftw() will report all files in a directory before reporting the directory itself. If clear, nftw() will report any directory before reporting the files in that directory.
	FTW_MOUNT
	If set, nftw() will only report files in the same file system as path. If clear, nftw() will report all files encountered during the walk.
	FTW_PHYS
	If set, nftw() performs a physical walk and does not follow symbolic links. If clear, nftw() will follow links instead of reporting them, and will not report the same file twice.
	*/
#if defined(__LINUX__)
	return nftw( path, fn, depth, flags );
#elif defined(__WINDOWS__)
	//_tchdir(path);

	WIN32_FIND_DATA file_info;
	HANDLE hHandle = INVALID_HANDLE_VALUE;
	_STAT sb;
	int nFlags = 0;
	struct FTW ftw;
	int nErr = 0;
	

	TCHAR szFullPath[MAX_PATH] = {0};
	TCHAR szTarget[MAX_PATH] = {0};
	int nFullPathLen = 0;
	TCHAR szFilePath[MAX_PATH] = {0};

	/************************************************************************/
	depth = 0;
	flags = 0;

	nFullPathLen = _tcslen(path);
	memset( &file_info, 0, sizeof(file_info) );

	if ( nFullPathLen >= MAX_PATH )
	{
		return -4;
	}

	_tcscpy( szFullPath, path );
	nFullPathLen = MakeDirStrLook(szFullPath, MAX_PATH);
	if ( nFullPathLen<0 || nFullPathLen+3>=MAX_PATH )
	{
		return -3;
	}

	_tcscpy( szTarget, szFullPath );
	szTarget[nFullPathLen] = _T('*');
	++nFullPathLen;
	szTarget[nFullPathLen] = _T('.');
	++nFullPathLen;
	szTarget[nFullPathLen] = _T('*');
	++nFullPathLen;
	szTarget[nFullPathLen] = 0;

	/* Here we go ... */
	/*HANDLE hHandle = INVALID_HANDLE_VALUE;*/
	hHandle = FindFirstFileEx( szTarget, FindExInfoStandard, &file_info, FindExSearchNameMatch, NULL, 0 );
	//hHandle = FindFirstFile( path, &file_info );
	if ( hHandle == INVALID_HANDLE_VALUE ) 
	{
		nErr = GetLastError();
		return nErr;
	}

	do
	{
        /*
        #define FILE_ATTRIBUTE_READONLY             0x00000001  
        #define FILE_ATTRIBUTE_HIDDEN               0x00000002  
        #define FILE_ATTRIBUTE_SYSTEM               0x00000004  
        #define FILE_ATTRIBUTE_DIRECTORY            0x00000010  
        #define FILE_ATTRIBUTE_ARCHIVE              0x00000020  
        #define FILE_ATTRIBUTE_DEVICE               0x00000040  
        #define FILE_ATTRIBUTE_NORMAL               0x00000080  
        #define FILE_ATTRIBUTE_TEMPORARY            0x00000100  
        #define FILE_ATTRIBUTE_SPARSE_FILE          0x00000200  
        #define FILE_ATTRIBUTE_REPARSE_POint        0x00000400  
        #define FILE_ATTRIBUTE_COMPRESSED           0x00000800  
        #define FILE_ATTRIBUTE_OFFLINE              0x00001000  
        #define FILE_ATTRIBUTE_NOT_CONTENT_INDEXED  0x00002000  
        #define FILE_ATTRIBUTE_ENCRYPTED            0x00004000  
        #define FILE_ATTRIBUTE_VIRTUAL 
    */
        //_tprintf( _T("File[%s] is Attb:%x\n"), file_info.cFileName, file_info.dwFileAttributes );

        sb.st_ctime = file_info.ftCreationTime.dwLowDateTime;
        sb.st_size = file_info.nFileSizeLow;

		nFlags = 0;
		if ( file_info.dwFileAttributes & FILE_ATTRIBUTE_NORMAL ) /* Normal file - No read/write restrictions */
		{
			//sb.st_mode = 
			nFlags = FTW_F;
		}

		if ( file_info.dwFileAttributes & FILE_ATTRIBUTE_READONLY ) /* Read only file */
		{
			nFlags = FTW_F;
		}

		if ( file_info.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN ) /* Hidden file */
		{
			nFlags = FTW_F;
		}

		if ( file_info.dwFileAttributes & FILE_ATTRIBUTE_SYSTEM ) /* System file */
		{
			nFlags = FTW_F;
		}

		if ( file_info.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY ) /* Subdirectory */
		{
			nFlags = FTW_D;
		}

		if ( file_info.dwFileAttributes & FILE_ATTRIBUTE_ARCHIVE )  /* Archive file */
		{
			nFlags = FTW_F;
		}

// 		ftw.base = cur->fts_pathlen - cur->fts_namelen;
// 		ftw.level = cur->fts_level
		_sntprintf( szFilePath, MAX_PATH, _T("%s%s"), szFullPath, file_info.cFileName );
		nErr = (*fn)( szFilePath, &sb, nFlags, &ftw );
		if ( nErr != 0 )
		{
			break;
		}

		// Step into sub-directory or not
		if ( file_info.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY ) /* Subdirectory */
		{
			if ( file_info.cFileName[0]==_T('.') && file_info.cFileName[1]==0 ) // Dir: "."
			{
			}
			else if ( file_info.cFileName[0]==_T('.') && file_info.cFileName[1]==_T('.') && file_info.cFileName[2]==0 ) // Dir: ".."
			{
			}
			else // Step into sub-directory
			{
				int nErrorCode = xt_nftw( szFilePath, fn, depth, flags );
				if ( nErrorCode < 0 )
				{
					return nErrorCode;
				}
				//_tchdir(path);
				memset( &file_info, 0, sizeof(file_info) );
				memset( &sb, 0, sizeof(sb) );
				continue;
			}
			nFlags = FTW_D;
		}
		// Step into sub-directory or not -- END

		memset( &file_info, 0, sizeof(file_info) );
		memset( &sb, 0, sizeof(sb) );
	}while( FindNextFile(hHandle, &file_info) );

	nErr = GetLastError();
	if (FindClose(hHandle) != 0 && nErr == 0)
		nErr = -1;
	else
		nErr = GetLastError();
	return nErr;
#else
#endif
}

// int cb_traverse_dir_handler_def( FILE_INFO *pInfo, void *arg )
// {
// 	int nRet = FUN_RET_OK;
// 	if ( arg == NULL )
// 	{
// 		return NULL;
// 	}
// 	
// 	FILE_INFO file_info;
// 	memcpy( &file_info, pInfo, sizeof(file_info) );
// 	if ( _tcslen(file_info.file_data.name) >= _MAX_PATH )
// 	{
// 		/* Path name contains too much characters, parameter 
// 			"arg" might NOT be exactly the type of 
// 			"typedef struct tagFILE_INFO" */
// 		return FUN_RET_OK;
// 	}
// 	
// 	switch( file_info.file_data.attrib )
// 	{
// 	case _A_SUBDIR: /* A directory */
// 		if ( strcmp(file_info->file_data.name, _T(".."), _tcslen(file_info->file_data.name)) == 0 ) /* A ".." directory */
// 		{
// 			/* Leave it alone */
// 		}
// 		else if ( strcmp(file_info->file_data.name, _T("."), _tcslen(file_info->file_data.name)) == 0 ) /* A "." directory */
// 		{
// 			/* Leave it alone */
// 		}
// 		else
// 		{
// 			int nCount = TraverseDir( file_info->szCurPath, cb_traverse_dir_handler_def, &file_info ); /* We step in it, and traverse it's children. */
// 			if ( nCount < 0 ) /* Something NOT good happened while traversing */
// 			{
// 				nRet = 0; /* ??? What to do on earth? Have not made up my mind yet. Further consideration should be given upon this later! */
// 				break;
// 			}
// 			nRet += nCount;
// 			++nRet; /* Count 1 along with the current directory */
// 		}
// 		break;
// 	case _A_NORMAL: /* A file */
// 		++nRet; /* Count 1 along with the current file */
// 		break;
// 	default:
// 		/* Any thing should be done that I might have missed */
// 	}
// 	
// 	return nRet;
// }
// 
// int TraverseDir( const TCHAR *szDir, (__stdcall)(INT)(*cb_handler)(VOID*), void *opt_arg )
// {
// 	/* return counter of files & folders, exclude "." & ".." */
// 	int nRet = FUN_RET_OK;
// 
// 	if ( szDir == NULL )
// 	{
// 		return -1;
// 	}
// 
// 	int nDirLen = _tcslen( szDir );
// 	if ( nDirLen<=0 || nDirLen>=_MAX_PATH ) /* Path name contains too much characters */
// 	{
// 		return -1;
// 	}
// 	/* Test szDir to make sure that this very directory DOES exist on disk */
// 	xt_file_access( szDir, F_OK );
// 
// 	/* The specific directory does NOT exist at all! Traverse YOUR-SISTER! */
// 
// 	FILE_INFO file_info;
// 	memset( &file_info, 0, sizeof(file_info) );
// 
// 	/*TCHAR szCurDir[_MAX_PATH] = {0}; */
// 	memcpy( file_info.szCurPath, szDir, nDirLen*sizeof(TCHAR) );
// 	nDirLen = MakeDirStrLook( file_info.szCurPath, _MAX_PATH );
// 	if ( nDirLen <= 0 ) /* Oops! An invalid directory string */
// 	{
// 		return -2;
// 	}
// 
// 	if ( cb_handler == NULL ) /* Use default handler */
// 	{
// 		cb_handler = cb_traverse_dir_handler_def;
// 		arg = (VOID*)&file_info;
// 	}
// 
// #ifdef __WINDOWS__
// 	HANDLE hHandle = INVALID_HANDLE_VALUE;
// 
// 	/* Here we go ... */
// 	if( (hHandle=_findfirst(_T("*"),&(file_info.file_data))) == INVALID_HANDLE_VALUE ) /* Nothing in directory "szDir" */
// 	{
// 		return 0;
// 	}
// 
// 	while( _findnext(hHandle, &(file_info.file_data)) == 0 )
// 	{
// 		nRet += (*cb_handler)( &file_info, arg );
// 	}
// 	_findclose( hHandle ); /* Done */
// #elif __LINUX__
// #endif
// 
// 	return nRet;
// }

