/************************************************************************************
* XtEvtHndlrSig.h : header file
*
* Event Handler class for signal
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

#ifndef __XT_EVENT_HANDLER_SIG_H__
#define __XT_EVENT_HANDLER_SIG_H__

#include "net/reactor/XtEventHandler.h"
#include "xt_sig_api.h"

/*****************************************
 *			Class CXtEvtHndlrSig
 *****************************************/

class CXtEvtHndlrSig : public CXtEventHandler
{
public:
	// non-block
	CXtEvtHndlrSig(void);
	virtual ~CXtEvtHndlrSig(void);

	/* Create event struct */
	virtual int				Init( void *pCfg );

	virtual int				HandleEvents( xt_event_t nEvents, void *pArg );

protected:

	virtual void			OnRegisterGetFD( xt_event_fd_t *pWhat );
	virtual void			OnRegisterGetEvents( xt_event_t *pWhat );

private:
};

#endif /*__XT_EVENT_HANDLER_SIG_H__*/
