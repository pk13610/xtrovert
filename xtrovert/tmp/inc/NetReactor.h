/************************************************************************************
* NetReactor.h : header file
*
* Class Definition header
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

#ifndef __NETREACTOR_H__
#define __NETREACTOR_H__

#include "ExtReactor.h"

class CNetReactor;
/*****************************************
 *			CEventHandler Class
 *****************************************/
class CNetEvtHandler
{
public:
	// Constructor & Destructor
	CNetEvtHandler(VOID);
	virtual ~CNetEvtHandler(VOID);
	// events maybe read/write/timeout/close .etc
	virtual INT			HandleEvents( INT nEvents ) = 0;

//	virtual HANDLE		GetHandle() = 0;
	virtual BOOL		IsValid(VOID) = 0;

	virtual UINT		GetEvents(VOID)						{ return m_nEvtSet; }
	virtual VOID		ClearEvents(VOID)					{ m_nEvtSet = 0; }
	virtual VOID		MaskEvents( UINT nEvents )			{ m_nEvtSet |= nEvents; }

protected:
	virtual INT			OnRead() = 0;
	virtual INT			OnWrite() = 0;
	virtual INT			OnTimeout() = 0;
	virtual INT			OnClose() = 0;

	UINT				m_nEvtSet;

	//struct event		m_Event;

private:	

};

/*****************************************
 *			CNetReactor Class
 *****************************************/

class CNetReactor : public CExtReactor
{
public:
	// Constructor & Destructor
	CNetReactor();
	virtual ~CNetReactor();

	virtual INT				Init(VOID); // Init event_base object
	//virtual VOID			Release(VOID);

	virtual INT				RegisterHandler( CNetEvtHandler *pHandler );
	virtual INT				RemoveHandler( CNetEvtHandler *pHandler );
	virtual INT				PollHandler( timeval *ptv );
	virtual INT				HandleEvents( size_t nCount=0 );

protected:

	//virtual VOID			Reset(VOID);

	struct event			**m_pEventPool;

private:
};

#endif /*__NETREACTOR_H__*/
