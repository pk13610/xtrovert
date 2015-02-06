/************************************************************************************
* XtCmdLineInfo.cpp: Implementation File
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

#include "XtCmdLineInfo.h"

#if defined(__WINDOWS__)
/* warning C4100: 'XXX' : unreferenced formal parameter */
#pragma warning (disable: 4127)
#pragma warning (disable: 4100)
#endif

void xt_strcmd_info_print( const xt_strcmd_info_t *info_ptr )
{
	_tprintf( _T("-%s(--%s)\t%s\t%s\n"), info_ptr->cmd_str_brief, info_ptr->cmd_str, info_ptr->param_list, info_ptr->description );
}

BOOL xt_strcmd_entry_isnull( const xt_strcmd_entry_t *entry )
{
	if ( entry->pfn_parser || entry->pClassParser || 
		entry->cmd_info.cmd_str || entry->cmd_info.cmd_str_brief || 
		entry->cmd_info.description || entry->cmd_info.param_list )
	{
		return FALSE;
	}

	return TRUE;
}

const xt_strcmd_entry_t*	xt_strcmd_entry_search_cmd( const xt_strcmd_entry_t *entriy_list, const TCHAR *cmd, int len )
{
	const xt_strcmd_entry_t *ret = NULL;
	int i = 0;

	while (1)
	{
		if ( entriy_list[i].cmd_info.cmd_str )
		{
			if ( _tcsncmp( entriy_list[i].cmd_info.cmd_str, cmd, len ) == 0 )
			{
				ret = &entriy_list[i];
				break;
			}
		}
		else
		{
			if ( xt_strcmd_entry_isnull(&entriy_list[i]) )
			{
				break;
			}			
		}
		++i;
	}

	return ret;
}

const xt_strcmd_entry_t*	xt_strcmd_entry_search_cmd_brief( const xt_strcmd_entry_t *entriy_list, const TCHAR *cmd, int len )
{
	const xt_strcmd_entry_t *ret = NULL;
	int i = 0;

	while (1)
	{
		if ( entriy_list[i].cmd_info.cmd_str )
		{
			if ( _tcsncmp( entriy_list[i].cmd_info.cmd_str_brief, cmd, len ) == 0 )
			{
				ret = &entriy_list[i];
				break;
			}
		}
		else
		{
			if ( xt_strcmd_entry_isnull(&entriy_list[i]) )
			{
				break;
			}			
		}
		++i;
	}

	return ret;
}

const xt_strcmd_entry_t*	xt_strcmd_entry_search_param( const xt_strcmd_entry_t *entriy_list, const TCHAR *param, int len )
{
	const xt_strcmd_entry_t *ret = NULL;
	int i = 0;

	while (1)
	{
		if ( entriy_list[i].cmd_info.param_list )
		{
			if ( _tcsncmp( entriy_list[i].cmd_info.param_list, param, len ) == 0 )
			{
				ret = &entriy_list[i];
				break;
			}
		}
		else
		{
			if ( xt_strcmd_entry_isnull(&entriy_list[i]) )
			{
				break;
			}			
		}

		if ( ret )
		{
			break;
		}
		++i;
	}

	return ret;
}

const xt_strcmd_entry_t*	xt_strcmd_entry_search_nullcmd( const xt_strcmd_entry_t *entriy_list )
{
	const xt_strcmd_entry_t *ret = NULL;
	int i = 0;

	while (1)
	{
		if ( entriy_list[i].cmd_info.cmd_str==NULL &&
			 entriy_list[i].cmd_info.cmd_str_brief == NULL &&
			 entriy_list[i].cmd_info.param_list == NULL )
		{
			if ( entriy_list[i].pfn_parser || entriy_list[i].pClassParser )
			{
				ret = &entriy_list[i];
			}

			break;
		}

		++i;
	}

	return ret;
}

/**********************************************************************
*		CLASS CXtCmdLineInfo
**********************************************************************/
CXtCmdLineInfo::CXtCmdLineInfo(void)
{
	Reset();
}

CXtCmdLineInfo::~CXtCmdLineInfo(void)
{
	Reset();
}

void CXtCmdLineInfo::ParseCommand( const TCHAR *cszCmd, const TCHAR *cszParam, BOOL bBrief/*=TRUE*/ )
{
	const xt_strcmd_map_t *pCmdMap = NULL;
	const xt_strcmd_entry_t *pEntry = NULL;

	int nRet = FUN_RET_OK;

	BOOL bFound = FALSE;

	int len = 0;

	pCmdMap = GetCmdLineMap();
	while ( !xt_strcmd_map_is_root(pCmdMap) )
	{
		if ( pCmdMap->entries == NULL )
		{
			pCmdMap = pCmdMap->pfnGetBaseMap();
			continue;
		}

		if ( cszCmd ) /* Search command string */
		{
			len = _tcslen( cszCmd );
			if ( bBrief )
			{
				pEntry = xt_strcmd_entry_search_cmd_brief( pCmdMap->entries, cszCmd, len );
			}
			else
			{
				pEntry = xt_strcmd_entry_search_cmd( pCmdMap->entries, cszCmd, len );
			}
			nRet = -5;

			if ( pEntry )
			{
				if ( IsValidCmdParam(pEntry, cszParam) == FALSE )
				{
					pEntry = NULL;
					nRet = -6;
				}
			}

		}
		else if ( cszParam ) /* Search by parameter string */
		{
			len = _tcslen( cszParam );
			pEntry = xt_strcmd_entry_search_param( pCmdMap->entries, cszParam, len );
			nRet = -6;
		}
		else
		{
			pEntry = xt_strcmd_entry_search_nullcmd( pCmdMap->entries );
			nRet = -6;
		}

		if ( pEntry && pEntry->pfn_parser )
		{
			bFound = TRUE;
			xt_fn_strcmd_parser_t strcmd_parser_fn;
			strcmd_parser_fn.pfn = pEntry->pfn_parser;
			nRet = (this->*strcmd_parser_fn._pfn)(cszParam);
			break;
		}
		else
		{
			nRet = -7;
		}

		pCmdMap = pCmdMap->pfnGetBaseMap();
	}

	return nRet;
}

#if defined(_UNICODE)
void CCommandLineInfo::ParseCommand( const char *cszCmd, const TCHAR *cszParam, BOOL bBrief/*=TRUE*/ )
{
	return;
}
#endif

void CXtCmdLineInfo::Reset(void)
{
	m_pContext = NULL;
}

BOOL CXtCmdLineInfo::IsValidCmdParam( xt_strcmd_entry_t *pEntry, const TCHAR *cszParam ) const
{
	return TRUE;
}

int CXtCmdLineInfo::GetCmdCount(void) const
{
	int nRet = 0;

	const xt_strcmd_map_t *pCmdMap = NULL;

	pCmdMap = GetCmdLineMap();
	while ( !xt_strcmd_map_is_root(pCmdMap) )
	{
		if ( pCmdMap->entries == NULL )
		{
			pCmdMap = pCmdMap->pfnGetBaseMap();
			continue;
		}

		nRet += pCmdMap->entry_count;

		pCmdMap = pCmdMap->pfnGetBaseMap();
	}

	return nRet;
}

const xt_strcmd_info_t* CXtCmdLineInfo::QueryCommand( int nCmdIdx ) const
{
	const xt_strcmd_map_t *pCmdMap = NULL;
	const xt_strcmd_entry_t *pEntry = NULL;
	int nCurCmdIdx = 0;

	pCmdMap = GetCmdLineMap();
	while ( !xt_strcmd_map_is_root(pCmdMap) )
	{
		if ( pCmdMap->entries == NULL )
		{
			pCmdMap = pCmdMap->pfnGetBaseMap();
			continue;
		}

		nCurCmdIdx += pCmdMap->entry_count;

		if ( nCurCmdIdx > nCmdIdx )
		{
			nCurCmdIdx -= pCmdMap->entry_count;
			nCmdIdx -= nCurCmdIdx;

			pEntry = &pCmdMap->entries[nCmdIdx];

			break;
		}

		pCmdMap = pCmdMap->pfnGetBaseMap();
	}

	if ( pEntry == NULL )
	{
		return NULL;
	}


	return &(pEntry->cmd_info);
}

const xt_strcmd_info_t* CXtCmdLineInfo::LookupCommand( const TCHAR *szCmd, BOOL bBrief/*=TRUE*/ ) const
{
	const xt_strcmd_map_t *pCmdMap = NULL;
	const xt_strcmd_entry_t *pEntry = NULL;

	const xt_strcmd_info_t *ret = NULL;
	int len = 0;

	len = _tcslen( szCmd );
	pCmdMap = GetCmdLineMap();
	while ( !xt_strcmd_map_is_root(pCmdMap) )
	{
		if ( pCmdMap->entries == NULL )
		{
			pCmdMap = pCmdMap->pfnGetBaseMap();
			continue;
		}

		if ( bBrief )
		{
			pEntry = xt_strcmd_entry_search_cmd_brief( pCmdMap->entries, szCmd, len );
		}
		else
		{
			pEntry = xt_strcmd_entry_search_cmd( pCmdMap->entries, szCmd, len );
		}

		if ( pEntry )
		{
			ret = &(pEntry->cmd_info);
			break;
		}

		pCmdMap = pCmdMap->pfnGetBaseMap();
	}

	return ret;
}


const xt_strcmd_map_t* CXtCmdLineInfo::GetThisCmdLineMap(void)
{
	static const xt_strcmd_map_t cmdShellMap = { NULL, 0,NULL };
	return &cmdShellMap;
}

const xt_strcmd_map_t* CXtCmdLineInfo::GetCmdLineMap(void) const
{
	return GetThisCmdLineMap();
}

/**********************************************************************
*		CLASS CXtCmdLineInfo END
**********************************************************************/

/**********************************************************************
*		CLASS CXtAppCmdLine
***********************************************************************/
XT_BEGIN_CMDLINE_MAP(CXtAppCmdLine,CXtCmdLineInfo)
	XT_ON_CMDLINE(_T("help"),_T("h"),NULL,_T("Show application usage. Equals to 'filename ?[ENTER]'"),OnAppCmdLineHelp)
	XT_ON_CMDLINE(_T("version"),_T("ver"),NULL,_T("Show application version information."),OnAppCmdLineVersion)
	/* Process Parameter */
	XT_ON_CMDLINE_VARPAR(OnAppCmdLineVarParam)
XT_END_CMDLINE_MAP()

CXtAppCmdLine::CXtAppCmdLine(void)
{
	Reset();
}

CXtAppCmdLine::~CXtAppCmdLine(void)
{
	Reset();
}

BOOL CXtAppCmdLine::IsValidCmdParam( xt_strcmd_entry_t *pEntry, const TCHAR *cszParam ) const
{
	int len = 0;
	int len2 = 0;
	int i = 0;

	if ( pEntry->cmd_info.param_list == NULL )
	{
		return TRUE;
	}
	

	len = _tcslen( cszParam );
	len2 = _tcslen( pEntry->cmd_info.param_list );

	while ( len <= len2 )
	{
		if ( _tcsncmp(pEntry->cmd_info.param_list+i, cszParam, len) == 0 )
		{
			return TRUE;
		}
		++i;
	}
	

	return FALSE;
}

int CXtAppCmdLine::OnAppCmdLineHelp( const TCHAR *cszParam )
{
	int i = 0;
	int nCount = 0;
	const xt_strcmd_info_t *pInfo = NULL;

	nCount = GetCmdCount();
	for ( i=0; i<nCount; ++i )
	{
		pInfo = QueryCommand( i );
		if ( pInfo )
		{
			xt_strcmd_info_print( pInfo );
		}
	}

	return FUN_RET_OK;
}

int CXtAppCmdLine::OnAppCmdLineVersion( const TCHAR *cszParam )
{
	const TCHAR *szVer = NULL;
	_tprintf( _T("version:%s\n"), szVer );

	return FUN_RET_OK;
}

int CXtAppCmdLine::OnAppCmdLineVarParam( const TCHAR *cszParam )
{
	return FUN_RET_OK;
}
