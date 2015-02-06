/************************************************************************************
* XtDocument.h : header file
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
#ifndef __XTDOCUMENT_H__
#define __XTDOCUMENT_H__

#include "xtrovert.h"
#include "XtCmdTarget.h"

#if defined(__WINDOWS__)
#pragma warning (disable: 4200) /* Ignore:  warning C4200: nonstandard extension used : zero-sized array in struct/union */
#endif

/*****************************************
 *			Class CXtDocTemplate
 *****************************************/
class CXtDocument;
class CXtService;
class XT_NOVTABLE CXtDocTemplate : public CCmdTarget
{
public:

	XT_DECLARE_DYNAMIC(CXtDocTemplate)

	CXtDocTemplate(void);
	virtual ~CXtDocTemplate(void);

	virtual void				LoadTemplate(void);

	virtual int					AddDocument( CXtDocument *pDoc );
	virtual int					RemoveDocument( CXtDocument *pDoc );
	virtual xt_size_t			GetDocumentCount(void);

	virtual xt_position_ptr_t	GetFirstDocPosition(void) const;
	virtual CXtDocument*		GetNextDoc( xt_position_ptr_t &rPos ) const;

	virtual BOOL				OnCmdMsg( UINT nID, int nCode, void *pExtra, xt_cmdhandlerinfo_t *pCmdHandlerInfo );

protected:

	virtual void				Reset(void);

	virtual CXtDocument*		CreateNewDocument(void);

	CXtService					*m_pSvcInterface;

private:
};

/*****************************************
 *			Class CXtDocument
 *****************************************/
class XT_NOVTABLE CXtDocument : public CCmdTarget
{
public:

	XT_DECLARE_DYNAMIC(CXtDocument)

	CXtDocument(void);
	virtual ~CXtDocument(void);

	virtual BOOL				IsModified(void);
	virtual void				SetModifiedFlag( BOOL bModified=TRUE );

	virtual BOOL				OnCmdMsg( UINT nID, int nCode, void *pExtra, xt_cmdhandlerinfo_t *pCmdHandlerInfo );

	// File helpers
	virtual BOOL				OnNewDocument(void);
	virtual BOOL				OnOpenDocument( const TCHAR *szDocName );
	virtual BOOL				OnSaveDocument( const TCHAR *szDocName );
	virtual void				OnCloseDocument(void);

protected:

	virtual void				Reset(void);

	virtual int					DoSave(void);

	CXtDocTemplate				*m_pDocTemplate;

	CXtService					*m_pSvcList;

	CXtThread					*m_pThread;

private:
};

/*****************************************
 *			Class CXtDocument
 *****************************************/
class CXtDocManager : public CXtObject
{
public:
	CXtDocManager(void);
	virtual ~CXtDocManager(void);

protected:
	CXtDocTemplate			*m_pTemplates;
private:
};

#endif /*__XTDOCUMENT_H__*/
