/************************************************************************************
* xt_reactor_define.h : header file
*
* Reactor structures defination
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
#ifndef __XT_REACTOR_DEFINE_H__
#define __XT_REACTOR_DEFINE_H__

#include "xt_define.h"
#include "xt_error.h"

#include <event2/event.h>
#include <event2/thread.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct event_base			xt_reactor_t;
typedef struct event_config			xt_reactor_cfg_t;

typedef evutil_socket_t				xt_event_fd_t;

typedef struct event				xt_evt_wrapper_t;

typedef short						xt_reactor_evt_t;

typedef event_callback_fn			xt_pfn_handle_events_t;

#define xt_evtwrapper_get_base(wrapper_ptr)				event_get_base((wrapper_ptr))
#define xt_evtwrapper_get_fd(wrapper_ptr)				event_get_fd((wrapper_ptr))
#define xt_evtwrapper_get_events(wrapper_ptr)			event_get_events((wrapper_ptr))


#define XT_REACTOR_EVT_MASK			(0x0000FFFF)
typedef UINT						xt_event_t; /* EV_TIMEOUT, EV_READ,  EV_WRITE, EV_SIGNAL, EV_PERSIST, EV_ET */
#define xt_conv2_reactor_event(xt_event)				(xt_reactor_evt_t((xt_event)&XT_REACTOR_EVT_MASK))


xt_reactor_t*		xt_reactor_create(void); /* Allocate memory for reactor */
int					xt_reactor_init( xt_reactor_t *reactor, xt_reactor_cfg_t *cfg ); /* Setup reactor with cfg */
void				xt_reactor_destroy( xt_reactor_t *reactor ); /* Release memory for reactor */
void				xt_reactor_start(void);
void				xt_reactor_stop( struct timeval *delay );

xt_evt_wrapper_t*	xt_evtwrapper_create(void);
int					xt_evtwrapper_init( xt_evt_wrapper_t *evt_wrapper );
void				xt_evtwrapper_destroy( xt_evt_wrapper_t *evt_wrapper );
int					xt_evtwrapper_register( xt_evt_wrapper_t *evt_wrapper, xt_reactor_t *reactor );
int					xt_evtwrapper_resign( xt_evt_wrapper_t *evt_wrapper );

/*typedef unsigned int				xt_evt_handle_t;*/
/*#define _INVALID_EVT_HANDLE_		~xt_evt_handle_t(0)*/

/* Error code */
#define XT_REACT_ERR_OK				FUN_RET_OK
#define XT_REACT_INIT_FAIL			-4 /* Handler & Reactor was NOT init or failed to be init */
#define XT_REACT_ERR_MEM_FAIL		-5
/* Error code END */

#ifdef __cplusplus
}
#endif

#endif /*__XT_REACTOR_DEFINE_H__*/
