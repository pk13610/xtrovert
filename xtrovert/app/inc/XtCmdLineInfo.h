/************************************************************************************
* XtCmdLineInfo.h : header file
*
* header
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
#ifndef __XTCMDLINEINFO_H__
#define __XTCMDLINEINFO_H__

#include "XTRuntimeClass.h"

class CXtCmdTarget;
typedef int (CXtCmdTarget::*xt_pfn_strcmd_parser_t)( const TCHAR *param );
typedef union _tag_xt_fn_strcmd_parser_t
{
	xt_pfn_strcmd_parser_t pfn;   // generic member function pointer

	int (CXtCmdTarget::*_pfn)(const TCHAR *);
}xt_fn_strcmd_parser_t;

typedef struct _tag_xt_strcmd_info_t
{
	const TCHAR				*cmd_str;
	const TCHAR				*cmd_str_brief;
	const TCHAR				*param_list;
	const TCHAR				*description;
}xt_strcmd_info_t;

void xt_strcmd_info_print( const xt_strcmd_info_t *info_ptr );

typedef struct _tag_xt_strcmd_entry_t
{
	xt_strcmd_info_t		cmd_info;
	xt_pfn_strcmd_parser_t	pfn_parser;
}xt_strcmd_entry_t;

#define xt_cmdline_entry_null				{ {0, 0, 0, 0}, (xt_pfn_strcmd_parser_t)0 }

extern BOOL						xt_strcmd_entry_isnull( const xt_strcmd_entry_t *entries );
extern const xt_strcmd_entry_t*	xt_strcmd_entry_search_cmd( const xt_strcmd_entry_t *entriy_list, const TCHAR *cmd, int len );
extern const xt_strcmd_entry_t*	xt_strcmd_entry_search_cmd_brief( const xt_strcmd_entry_t *entriy_list, const TCHAR *cmd, int len );
extern const xt_strcmd_entry_t*	xt_strcmd_entry_search_param( const xt_strcmd_entry_t *entriy_list, const TCHAR *param, int len );
/* each member in entry->cmd_info is 0, but pfn_parser or pClassParser is NOt 0 */
extern const xt_strcmd_entry_t*	xt_strcmd_entry_search_nullcmd( const xt_strcmd_entry_t *entriy_list );


typedef struct _tag_xt_strcmd_map_t
{
	const struct _tag_xt_strcmd_map_t*	(*pfnGetBaseMap)(void);
	int									entry_count;
	const xt_strcmd_entry_t				*entries;
}xt_strcmd_map_t;

#define xt_strcmd_map_is_root(_map_ptr)			((_map_ptr)==NULL || (_map_ptr)->pfnGetBaseMap==NULL)

#define XT_DECLARE_CMDLINE_MAP()() \
protected: \
	static const xt_strcmd_map_t* GetThisCmdLineMap(void); \
	virtual const xt_strcmd_map_t* GetCmdLineMap(void) const; \


#define XT_BEGIN_CMDLINE_MAP(theClass, baseClass) \
	XT_PTM_WARNING_DISABLE		\
	const xt_strcmd_map_t* theClass::GetCmdLineMap(void) const \
			{ return GetThisCmdShellMap(); } \
	const xt_strcmd_map_t* theClass::GetThisCmdLineMap(void)	\
	{ \
		typedef theClass ThisClass;								\
		typedef baseClass TheBaseClass;							\
		static const xt_strcmd_entry_t _cmdLineEntries[] =		\
		{

#define XT_ON_CMDLINE(_str_cmd,_str_cmdbrief,_param_list,_str_desc,_pfn_parser)			\
			{ {(_str_cmd), (_str_cmdbrief), (_param_list), (_str_desc)},					\
						static_cast<xt_pfn_strcmd_parser_t> (_pfn_parser) },

#define XT_ON_CMDLINE_PAR(_param_list,_str_desc,_pfn_parser)	\
			{ {0, 0, (_param_list), (_str_desc)}, static_cast<xt_pfn_strcmd_parser_t> (_pfn_parser) },

#define XT_ON_CMDLINE_VARPAR(_pfn_parser)	\
			{ {0, 0, 0, 0}, static_cast<xt_pfn_strcmd_parser_t> (_pfn_parser) },

#define XT_ON_CMDLINE_EX(_str_cmd,_str_cmdbrief,_param_list,_str_desc,_classname_parser)	\
			{ {(_str_cmd), (_str_cmdbrief), (_param_list), (_str_desc)}, (xt_pfn_strcmd_parser_t)0 },

#define XT_END_CMDLINE_MAP() \
			xt_cmdline_entry_null \
		}; \
		static const xt_strcmd_map_t cmdLineMap = \
			{ &TheBaseClass::GetThisCmdLineMap, sizeof(_cmdLineEntries)/sizeof(xt_strcmd_entry_t)-1, &_cmdLineEntries[0] }; \
		return &cmdLineMap; \
	}				\
	XT_PTM_WARNING_RESTORE


enum
{
	XT_STRCMD_TYPE_BRIEF = 0,
	XT_STRCMD_TYPE_SLASH,
	XT_STRCMD_TYPE_CPLT,

	XT_STRCMD_TYPE_END
};
/**********************************************************************
*		CLASS CXtCmdLineInfo
***********************************************************************/
class CXtCmdLineInfo : public CXtObject
{
public:
	// Sets default values
	CXtCmdLineInfo(void);
	virtual ~CXtCmdLineInfo(void);

	// plain char* version on UNICODE for source-code backwards compatibility
	void					ParseCommand( const TCHAR *cszCmd, const TCHAR *cszParam, BOOL bBrief=TRUE ); /* Parse 1 command at a time */
#ifdef _UNICODE
	void					ParseCommand( const char *cszCmd, const TCHAR *cszParam, BOOL bBrief=TRUE ); /* Parse 1 command at a time */
#endif

	CXtCmdTarget			*m_pContext;

	// Implementation
protected:

	virtual void			Reset(void);

	virtual BOOL			IsValidCmdParam( xt_strcmd_entry_t *pEntry, const TCHAR *cszParam ) const;

	int						GetCmdCount(void) const;
	const xt_strcmd_info_t* QueryCommand( int nCmdIdx ) const;
	const xt_strcmd_info_t* LookupCommand( const TCHAR *szCmd, BOOL bBrief=TRUE ) const;

	XT_DECLARE_CMDLINE_MAP()
};

/**********************************************************************
*		CLASS CXtAppCmdLine
***********************************************************************/
class CXtAppCmdLine : public CXtCmdLineInfo
{
public:
	CXtAppCmdLine(void);
	virtual ~CXtAppCmdLine(void);
protected:
private:

	virtual BOOL			IsValidCmdParam( xt_strcmd_entry_t *pEntry, const TCHAR *cszParam ) const;

	/* Print usage information. */
	int 					OnAppCmdLineHelp( const TCHAR *cszParam ); /* filename.exe -h,(--help),? */
	/* Print version information. */
	int						OnAppCmdLineVersion( const TCHAR *cszParam ); /* filename.exe -ver,(--version) */

	int 					OnAppCmdLineVarParam( const TCHAR *cszParam );

	XT_DECLARE_CMDLINE_MAP()
};

#endif /*__XTCMDLINEINFO_H__*/
