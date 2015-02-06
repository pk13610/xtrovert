/************************************************************************************
*	xt_str_api.c: Implementation File
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
#include "xt_str_api.h"


/********************************************************************************/
/* Static functions start */
static int xt_shit_bits( int nBase )
{
	int nShit = 0;
	switch ( nBase )
	{
	case 2:
		nShit = 1;
		break;
	case 8:
		nShit = 3;
		break;
	case 16:
		nShit = 4;
		break;
	default:
		break;
	}

	if ( nShit == 0 )
	{
		return -3;
	}

	return nShit;
}

int xt_skip_space_char( const TCHAR *szSrc )
{
	int nLen = _tcslen( szSrc );
	int nPos = 0;
	while( nPos < nLen )
	{
		if ( szSrc[nPos] != _T(' ') && szSrc[nPos] != _T('\t') )
		{
			break;
		}
		++nPos;
	}

	if ( nPos >= nLen )
	{
		return -3;
	}

	return nPos;
}

/* Static functions end */
/********************************************************************************/

int xt_str_is_num( const TCHAR *val )
{
	int nPos = 0;
	int bNum = 1;
	/* Skip Space character */
	nPos = xt_skip_space_char( val );
	if ( nPos < 0 )
	{
		return 0;
	}
	val += nPos;

	if ( *val==_T('-') ||  *val==_T('+') )
	{
		++val;
	}

	while ( *val != 0 )
	{
		switch ( *val )
		{
		case _T('0'):
		case _T('1'):
		case _T('2'):
		case _T('3'):
		case _T('4'):
		case _T('5'):
		case _T('6'):
		case _T('7'):
		case _T('8'):
		case _T('9'):
		case _T('A'):
		case _T('B'):
		case _T('C'):
		case _T('D'):
		case _T('E'):
		case _T('F'):
			break;
		default:
			bNum = 0;
			break;
		}
		++val;
	}

	return bNum;
}

int xt_str2_num( const TCHAR *szSrc, void *pDst, xt_size_t nDstSize, int nBase )
{
	INT64 nVal = 0;
	int nSign = 1;
	int nPos = 0;
	BOOL bFail = FALSE;

	int nShiftBits = 0;
	if ( nBase != 10 )
	{
		nShiftBits = xt_shit_bits( nBase );
		if ( nShiftBits <= 0 )
		{
			return -3;
		}
	}

	/* Skip Space character */
	nPos = xt_skip_space_char( szSrc );
	if ( nPos < 0 )
	{
		return -4;
	}
	szSrc += nPos;

	if ( *szSrc == _T('-') )
	{
		nSign = -1;
		++szSrc;
	}
	else if ( *szSrc == _T('-') )
	{
		++szSrc;
	}

	while ( *szSrc != 0 )
	{
		switch ( *szSrc )
		{
		case _T(' '):
			bFail = TRUE;
			break;
		case _T('0'):
		case _T('1'):
			if ( nShiftBits == 0 )
			{
				nVal *= 10;
			}
			else
			{
				nVal = nVal << nShiftBits;
			}
			nVal += ((*szSrc)-_T('0'));
			break;
/*		case _T('-'):
			nSign = -1;
*/
			break;
		default:
			bFail = TRUE;
			break;
		}

		if ( bFail )
		{
			break;
		}

		++szSrc;
	} 

	if ( bFail )
	{
		return -5;
	}

	nVal *= nSign;

	memcpy( pDst, &nVal, nDstSize );

	return 0;
}

int xt_str2_unum( const TCHAR *szSrc, void *pDst, xt_size_t nDstSize, int nBase )
{
	UINT64 nVal = 0;
	int nPos = 0;
	int nShiftBits = 0;
	BOOL bFail = FALSE;
	if ( nBase != 10 )
	{
		nShiftBits = xt_shit_bits( nBase );
		if ( nShiftBits <= 0 )
		{
			return -3;
		}
	}

	/* Skip Space character */
	nPos = xt_skip_space_char( szSrc );
	if ( nPos < 0 )
	{
		return -4;
	}
	szSrc += nPos;

	bFail = FALSE;
	while ( *szSrc != 0 )
	{
		switch ( *szSrc )
		{
		case _T(' '):
			bFail = TRUE;
			break;
		case _T('0'):
		case _T('1'):
		case _T('2'):
		case _T('3'):
		case _T('4'):
		case _T('5'):
		case _T('6'):
		case _T('7'):
		case _T('8'):
		case _T('9'):
			if ( nShiftBits == 0 )
			{
				nVal *= 10;
			}
			else
			{
				nVal = nVal << nShiftBits;
			}
			nVal += ((*szSrc)-_T('0'));
			break;
		case _T('A'):
		case _T('B'):
		case _T('C'):
		case _T('D'):
		case _T('E'):
		case _T('F'):
			nVal = nVal << nShiftBits;
			nVal += (((*szSrc)-_T('A'))+10);
			break;
		default:
			bFail = TRUE;
			break;
		}

		if ( bFail )
		{
			break;
		}

		++szSrc;
	} 

	if ( bFail )
	{
		return -5;
	}

	memcpy( pDst, &nVal, nDstSize );

	return 0;
}

int xt_str_trim_left( TCHAR *szBuff, int nLen )
{
	// Return counter of characters in szBuffer
	int i = 0;
	int j = 0;

	if ( szBuff==NULL || nLen<=0 )
	{
		return FUN_RET_ERR_ILLEGAL_ARGS;
	}

	for ( i=0; i<nLen; ++i )
	{
		if ( szBuff[i]!=_T(' ') && szBuff[i]!=_T('\t') && szBuff[i]!=_T('\n') && szBuff[i]!=_T('\r') && szBuff[i]!=0 )
		{
			break;
		}
	}

    if ( i == 0 ) // Need not to trim
    {
        return FUN_RET_OK;
    }

	j = i;
	if ( i >= nLen ) /* Buffer contains "SPACE" or "TAB" characters only. */
	{
		szBuff[0] = 0;
	}
	else /* Found a non-"SPACE" or "TAB" character */
	{
        for ( j=i; j<nLen; ++j )
        {
            if ( szBuff[j] == 0 ) /* 0 repeats from here, and then leave them alone */
            {
                break;
            }
            szBuff[j-i] = szBuff[j];
        }
        szBuff[j-i] = 0;
	}

	return FUN_RET_OK;
}

int xt_str_trim_right( TCHAR *szBuff, int nLen )
{
	int i = 0;

	// Return counter of characters in szBuffer
	if ( szBuff==NULL || nLen<=0 )
	{
		return FUN_RET_ERR_ILLEGAL_ARGS;
	}

    i = GetStrLen( szBuff, nLen ) - 1; // The last character's position
    if ( i < 0 )
    {
        return -2;
    }

	while ( szBuff[i]==_T(' ') || szBuff[i]==_T('\t') || szBuff[i]==_T('\n') || szBuff[i]==_T('\r') )
	{
		szBuff[i] = 0;
        --i;
        if ( i < 0 )
        {
            break;
        }
	}

	return FUN_RET_OK;
}

int xt_str_trim( TCHAR *szBuff, int nLen )
{
    if ( szBuff==NULL || nLen<0 )
    {
        return FUN_RET_ERR_ILLEGAL_ARGS;
    }

    if ( nLen == 0 )
    {
        return FUN_RET_OK;
    }

    if ( xt_str_trim_right(szBuff, nLen) < 0 )
    {
        return -4;
    }

	return xt_str_trim_left(szBuff, nLen);
}

int GetStrLen( const TCHAR *szBuff, int nMaxLen )
{
	int nPos = 0;

	if ( szBuff==NULL || nMaxLen<0 )
	{
		return FUN_RET_ERR_ILLEGAL_ARGS;
	}
	
	if ( nMaxLen == 0 )
	{
		return FUN_RET_OK;
	}

	nPos = 0;
	for ( nPos=0; nPos<nMaxLen; ++nPos )
	{
		if ( szBuff[nPos]==_SYMBOL_BACK_SLASH_ )
		{
			continue;
		}
		if ( szBuff[nPos]==0 || szBuff[nPos]==_T('\r') || szBuff[nPos]==_T('\n') )
		{
			break;
		}
	}
	if ( nPos >= nMaxLen )
	{
		nPos = nMaxLen;
	}

	return nPos;
}

int ConvertDirWin( TCHAR *szDir, int nBufLen )
{
	//////////////////////////////////////////////////////////////////////////
	// Return:
	//	* szDir string length
	// Remarks:
	//	* Turn the incoming string to this pattern : dir1\\dir2\\dir3\\
	//////////////////////////////////////////////////////////////////////////

	TCHAR szBuf[_MAX_PATH] = {0};
	int nStrLen = 0;
	int i=0;
	int nDstBufPos = 0;
	int nMultipleSlash = 0;

	if ( szDir==NULL || nBufLen<0 )
	{
		return FUN_RET_ERR_ILLEGAL_ARGS;
	}
	
	if ( nBufLen == 0 )
	{
		return FUN_RET_OK;
	}

	nStrLen = GetStrLen( szDir, nBufLen );
	for ( i=0; i<nStrLen; ++i )
	{
		if ( szDir[i]==0 || szDir[i]==_T('\r') || szDir[i]==_T('\n') )  // Reach the end of the string
		{
			break;
		}

		if ( szDir[i] == _SYMBOL_DIR_WIN_STD_ ) // Found '\\'
		{
			++nMultipleSlash;
			if ( nMultipleSlash >= 2 )
			{
				continue;
			}

			memset( szBuf+nDstBufPos, _SYMBOL_DIR_WIN_STD_, 2*sizeof(_SYMBOL_DIR_WIN_STD_) );
			nDstBufPos += 2;
			continue;
		}

		szBuf[nDstBufPos] = szDir[i];
		++nDstBufPos;
		nMultipleSlash = 0;
	}

	if ( nDstBufPos == 0 )
	{
		return -3;
	}

	if ( szBuf[nDstBufPos-1] != _SYMBOL_DIR_WIN_STD_ )
	{
		memset( szBuf+nDstBufPos, _SYMBOL_DIR_WIN_STD_, 2*sizeof(_SYMBOL_DIR_WIN_STD_) );
		nDstBufPos += 2;
	}

	if ( nDstBufPos >= nBufLen ) // Overflow
	{
		return -4;
	}

	memset( szDir, 0, nBufLen*sizeof(TCHAR) );
	memcpy( szDir, szBuf, nDstBufPos*sizeof(TCHAR) );

	return nDstBufPos;
}

int ConvertDirLnx( TCHAR *szDir, int nBufLen )
{
	//////////////////////////////////////////////////////////////////////////
	// Return:
	//	* szDir string length
	// Remarks:
	//	* Turn the incoming string to this pattern : dir1/dir2/dir3/
	//////////////////////////////////////////////////////////////////////////

	TCHAR szBuf[_MAX_PATH] = {0};

	int nStrLen = GetStrLen( szDir, nBufLen );
	int i=0;
	int nDstBufPos = 0;
	int nMultipleSlash = 0;

	if ( szDir==NULL || nBufLen<0 )
	{
		return FUN_RET_ERR_ILLEGAL_ARGS;
	}
	
	if ( nBufLen == 0 )
	{
		return FUN_RET_OK;
	}

	nStrLen = GetStrLen( szDir, nBufLen );
	for ( i=0; i<nStrLen; ++i )
	{
		if ( szDir[i]==0 || szDir[i]==_T('\r') || szDir[i]==_T('\n') )  // Reach the end of the string
		{
			break;
		}

		if ( szDir[i] == _SYMBOL_DIR_LINUX_STD_ ) // Found '/'
		{
			nMultipleSlash += 1;
			if ( nMultipleSlash >= 2 )
			{
				continue;
			}
		}

		szBuf[nDstBufPos] = szDir[i];
		++nDstBufPos;
		nMultipleSlash = 0;
	}

	if ( nDstBufPos == 0 )
	{
		return -2;
	}

	if ( szBuf[nDstBufPos-1] != _SYMBOL_DIR_LINUX_STD_ )
	{
		szBuf[nDstBufPos] = _SYMBOL_DIR_LINUX_STD_;
		++nDstBufPos;
	}

	if ( nDstBufPos >= nBufLen ) // Overflow
	{
		return -3;
	}

	memset( szDir, 0, nBufLen*sizeof(TCHAR) );
	memcpy( szDir, szBuf, nDstBufPos*sizeof(TCHAR) );

	return nDstBufPos;
}

int MakeDirStrLook( TCHAR *szDir, int nBufLen )
{
	/***************************************************************
	* Parameters:
	*	szDir[in][out]: A pointer to a character string.
	*	nBufLen[in]: The whole buffer length that pointed by szDir.
	* Return Value:
	*	*Total number of characters in the szDir would be returned 
	*	 on success, exclude NULL character.
	*	*A negative integer would be returned to indicate some 
	*	 certain error occurred.
	* Remarks:
	*	*Remove any '\t' and space characters on the left and right
	*	 of szDir string.
	*	*Append a directory symbol to the end, if there does NOT
	*	 exist at all. And we will get a new one which looks more 
	*	 like a directory string.
	****************************************************************/
	int nRet = FUN_RET_OK;
	if ( szDir==NULL || nBufLen<=0 )
	{
		return -1;
	}
	
	nRet = xt_str_trim( szDir, nBufLen );
	if ( nRet < 0 ) /* The real string length after cleaned by trim function. */
	{
		return -2;
	}

	nRet = _tcslen( szDir )-1;
	
	if ( nRet >= 0 )
	{
#if defined(__WINDOWS__)
		/* Slash and back-slash are both valid dir-symbol under MS-Windows platform. */
		if ( szDir[nRet]==_SYMBOL_DIR_WIN_STD_ || szDir[nRet]==_SYMBOL_DIR_LINUX_STD_ )
		{
			/* Directory symbol already exists at the end of the string. */
			return nRet;
		}
		/* Directory symbol does NOT exist at all. */
		/* Make sure we DO have enough room to append a character. */
		/* Must have extra room to hold at least 2 characters. One is DIR-CHARACTER, while the other is NULL */
		if ( nRet >= (nBufLen-3) ) /* '\\'+'\\'+0  */
		{
			/* Appending a Non-NULL character to the end of a memory buffer 
			   may lead to an overflow while later working with strlen 
			   family functions. */
			/* It's extremely dangerous to fill up a memory buffer without 
				at least one NULL character at the buffer end. */
			return -3;
		}
		++nRet;
		szDir[nRet] = _SYMBOL_DIR_WIN_STD_;
		++nRet;
		szDir[nRet] = _SYMBOL_DIR_WIN_STD_;
#elif defined(__LINUX__)
		if ( szDir[nRet] == _SYMBOL_DIR_LINUX_STD_ ) /* Only Slash is valid under Linux platform. */
		{
			/* Directory symbol already exists at the end of the string. */
			return nRet;
		}
		/* Directory symbol does NOT exist at all. */
		/* Make sure we DO have enough room to append a character. */
		/* Must have extra room to hold at least 2 characters. One is DIR-CHARACTER, while the other is NULL */
		if ( nRet >= (nBufLen-2) ) /* '/'+0  */
		{
			/* Appending a Non-NULL character to the end of a memory buffer 
			   may lead to an overflow while later working with strlen 
			   family functions. */
			/* It's extremely dangerous to fill up a memory buffer without 
				at least one NULL character at the buffer end. */
			return -3;
		}
		++nRet;
		szDir[nRet] = _SYMBOL_DIR_LINUX_STD_;
#endif
		++nRet; /* Count the DIR-CHARACTER which has been just appended. */
		szDir[nRet] = 0;
	}
	
	return nRet;
}

int SplitPathFile( const TCHAR *szFullPath, TCHAR *szPath, TCHAR *szFile )
{
	int nFullPathLen = 0;
	int nCurPos = 0;
	if ( szFullPath==NULL || szPath==NULL || szFile==NULL )
	{
		return FUN_RET_ERR_ILLEGAL_ARGS;
	}

	nFullPathLen = _tcslen( szFullPath );
	if ( nFullPathLen <= 0 )
	{
		return FUN_RET_ERR_ILLEGAL_ARGS;
	}
	// else nFullPathLen > 0

	nCurPos = nFullPathLen;
	while( nCurPos >= 0 )
	{
#if defined(__WINDOWS__)
		if ( szFullPath[nCurPos]==_SYMBOL_DIR_WIN_STD_ || szFullPath[nCurPos]==_SYMBOL_DIR_LINUX_STD_ )
		{
#elif defined(__LINUX__)
		if ( szFullPath[nCurPos] == _SYMBOL_DIR_LINUX_STD_ )
		{
#else
#endif
			if ( nCurPos < nFullPathLen ) // "/home/user1/abc/a.txt"
			{
				++nCurPos;
			}
			// else "/home/user1/abc/"
			memcpy( szPath, &szFullPath[0], nCurPos*sizeof(TCHAR) ); // szPath
			memcpy( szFile, &szFullPath[nCurPos], (nFullPathLen-nCurPos)*sizeof(TCHAR) ); // szFile
			break;
		}
		--nCurPos;
	}

	return FUN_RET_OK;
}

// int MakeDirStrLook( TCHAR *szDir, int nBuffLen )
// {
// 	/***************************************************************
// 	* Parameters:
// 	*	szDir[in][out]: A pointer to a character string.
// 	*	nBufLen[in]: The whole buffer length that pointed by szDir.
// 	* Return Value:
// 	*	*Total number of characters in the szDir would be returned 
// 	*	 on success, exclude NULL character.
// 	*	*A negative integer would be returned to indicate some 
// 	*	 certain error occurred.
// 	* Remarks:
// 	*	*Remove any '\t' and space characters on the left and right
// 	*	 of szDir string.
// 	*	*Append a directory symbol to the end, if there does NOT
// 	*	 exist at all. And we will get a new one which looks more 
// 	*	 like a directory string.
// 	****************************************************************/
// 	int nRet = FUN_RET_OK;
// 	if ( szDir==NULL || nBuffLen<0 )
// 	{
// 		return -1;
// 	}
// 	if ( nBuffLen == 0 )
// 	{
// 		return FUN_RET_OK;
// 	}
// 	
// 	nRet = xt_str_trim( szDir, nBuffLen );
// 	if ( nRet < 0 ) /* The real string length after cleaned by trim function. */
// 	{
// 		return -2;
// 	}
// 	
// 	if ( nRet > 0 )
// 	{
// #if defined(__WINDOWS__)
// 		/* Slash and back-slash are both valid dir-symbol under MS-Windows platform. */
// 		if ( szDir[nRet-1]==_SYMBOL_DIR_WIN_STD_ || szDir[nRet-1]==_SYMBOL_DIR_LINUX_STD_ )
// 		{
// 			/* Directory symbol already exists at the end of the string. */
// 			return nRet;
// 		}
// 		/* Directory symbol does NOT exist at all. */
// 		/* Make sure we DO have enough room to append a character. */
// 		/* Must have extra room to hold at least 2 characters. One is DIR-CHARACTER, while the other is NULL */
// 		if ( nRet >= (nBuffLen-1) )
// 		{
// 			/* Appending a Non-NULL character to the end of a memory buffer 
// 			   may lead to an overflow while later working with strlen 
// 			   family functions. */
// 			/* It's extremely dangerous to fill up a memory buffer without 
// 				at least one NULL character at the buffer end. */
// 			return -3;
// 		}
// 		szDir[nRet] = _SYMBOL_DIR_WIN_STD_;
// #elif defined(__LINUX__)
// 		if ( szDir[nRet-1] == _SYMBOL_DIR_LINUX_STD_ ) /* Only Slash is valid under Linux platform. */
// 		{
// 			/* Directory symbol already exists at the end of the string. */
// 			return nRet;
// 		}
// 		/* Directory symbol does NOT exist at all. */
// 		/* Make sure we DO have enough room to append a character. */
// 		/* Must have extra room to hold at least 2 characters. One is DIR-CHARACTER, while the other is NULL */
// 		if ( nRet >= (nBuffLen-1) )
// 		{
// 			/* Appending a Non-NULL character to the end of a memory buffer 
// 			   may lead to an overflow while later working with strlen 
// 			   family functions. */
// 			/* It's extremely dangerous to fill up a memory buffer without 
// 				at least one NULL character at the buffer end. */
// 			return -3;
// 		}
// 		szDir[nRet] = _SYMBOL_DIR_LINUX_STD_;
// #endif
// 		++nRet; /* Count the DIR-CHARACTER which has been just appended. */
// 		szDir[nRet] = 0;
// 	}
// 	
// 	return nRet;
//
/************************************************************************/
/************************************************************************/
