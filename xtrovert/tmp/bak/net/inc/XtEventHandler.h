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

#include "xt_define.h"

#include <event2/event.h>

typedef evutil_socket_t				xt_event_fd_t;
typedef short						xt_event_t; /* EV_TIMEOUT EV_READ  EV_WRITE EV_SIGNAL EV_PERSIST EV_ET */

typedef unsigned int				xt_evt_handle_t;
#define _INVALID_EVT_HANDLE_		~xt_evt_handle_t(0)

/*****************************************
 *			Class CXtEventtHandler
 *****************************************/
class CXtReactor;
class CXtEventHandler;

typedef struct _tag_xt_event_t
{
	struct event		*_event;
	CXtEventHandler		*evt_handler;
	xt_evt_handle_t		evt_handle;
}xt_event_t;

/*
event types:
	socket: af_unix, af_inet, af_inet6, tcp/udp
	
	timer:
	signal:
*/

/* Event Type */
enum
{
	XT_EVT_T_IO = 0,
	XT_EVT_T_SIG,
	XT_EVT_T_TIMER,

	XT_EVT_T_END
};

class CXtEvent
{
public:
	// non-block
	CXtEvent(void);
	virtual ~CXtEvent(void);

	/* Create event struct */
	virtual int				Init( void *arg );
	virtual void			Release(void);

	virtual int				Register( CXtReactor *reactor );

	static void				OnHandleEvents( xt_event_fd_t fd, short events, void *arg );

	xt_evt_handle_t			GetHandle(void)							{ return (m_pXtEvt)?m_pXtEvt->evt_handle:_INVALID_EVT_HANDLE_; }

	BOOL					IsValid(void)							{ return is_valid_handler; }

	xt_event_t*				GetEvent(void)							{ return m_pXtEvt; }

protected:

	#define is_valid_handler						(m_pXtEvt!=NULL)

	void					Reset(void);

	virtual void			HandleEvents( short events );

	CXtReactor				*m_pReactor;
/*	xt_event_t				*m_pXtEvt;*/

	xt_event_fd_t			m_fd;
	xt_event_t				m_events;

private:
};


/**********************************************
 *			class CXtEventPool
 *	For storage of "struct event" of libevent
 **********************************************/

class CXtEventPool : public CXtObjPool
{
public:
	CXtEventPool(void);
	virtual ~CXtEventPool(void);

	/*xt_event_t*/

	virtual int					Init( xt_size_t nObjCount );
	virtual void				Release(void);
protected:
private:
};

#endif /*__XT_EVENT_HANDLER_H__*/
