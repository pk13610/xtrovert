/************************************************************************************
* XtEventHandler.h : header file
*
* Event Handler class
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

#ifndef __XT_EVENT_HANDLER_H__
#define __XT_EVENT_HANDLER_H__

#include "XtReactor.h"

/******************************************************************
	System Predefined:
		EV_TIMEOUT, EV_READ, EV_WRITE, EV_SIGNAL, EV_PERSIST, EV_ET 
******************************************************************/
#define XT_EV_TIMEOUT				EV_TIMEOUT
#define XT_EV_READ					EV_READ
#define XT_EV_WRITE					EV_WRITE
#define XT_EV_SIGNAL				EV_SIGNAL
#define XT_EV_PERSIST				EV_PERSIST
#define XT_EV_ET					EV_ET
/******************************************************************
	Customize Events starts from ...
******************************************************************/
#define XT_EV_START					((EV_ET)<<1)

typedef union _tag_xt_event_dsc_t
{
	xt_event_fd_t		fd;		/* For TCP Client/Server, UDP Server, and Signals */
	xt_event_fd_t		*fd_handle;	/* For UDP client only, points UDP-Server's "fd" */
}xt_event_dsc_t;

/*****************************************
 *			Class CXtEventtHandler
 *****************************************/
class CXtEventHandler
{
public:
	CXtEventHandler(void);
	virtual ~CXtEventHandler(void);

	/* Initialize RUN-TIME configuration. */
	virtual int				Init( void *pCfg );
	/*  */
	virtual void			Release(void);

	/* MakeActive automatically. */
	int						Register( CXtReactor *pReactor );
	int						Resign(void);

	BOOL					IsActive(void);

	virtual int				HandleEvents( xt_event_t nEvents, void *pArg );

	virtual xt_event_fd_t	GetEventfd(void);

	xt_event_t				GetEvents(void);

	BOOL					IsValid(void);

#if defined(_DEBUG)
	void					SetLabel( const TCHAR *szLabel );
	TCHAR					m_szLabel[_MAX_STR_LEN_8_];
#endif

protected:

	static void				OnHandleEvents( xt_event_fd_t nEventfd, xt_reactor_evt_t nEvents, void *pArg );

	virtual void			Reset(void);

	virtual int				BindReactor( CXtReactor *pReactor, xt_event_fd_t fd, xt_event_t nEvents );
	virtual int				MakeActive( BOOL bActive );
	/* libevent functions */
	int						EventAssign( CXtReactor *pReactor, xt_event_fd_t fd, xt_event_t nEvents );
	int						EventNew( CXtReactor *pReactor, xt_event_fd_t fd, xt_event_t nEvents );
	void					EventFree(void);
	/* libevent functions END */

	virtual void			OnRegisterGetFD( xt_event_fd_t *pWhat );
	virtual void			OnRegisterGetEvents( xt_event_t *pWhat );
	virtual void			OnRegisterGetTimeout( struct timeval *pWhat );

	/* Data Member */

	xt_evt_wrapper_t			*m_pEventWrapper;
	BOOL						m_bFreeWrapper; /* m_pEventWrapper==TRUE has been allocated in dynamic memory. We should free m_pEventWrapper in destruction-function. */
#define xt_evtwrapper_size		event_get_struct_event_size()

private:
};

#endif /*__XT_EVENT_HANDLER_H__*/
