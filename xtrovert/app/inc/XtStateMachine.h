/************************************************************************************
* XtStateMachine.h : header file
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
#if defined(__XTSTATEMACHINE_H__)
#define __XTSTATEMACHINE_H__

#include "xtrovert.h"

/*
	Service: Client一次性发起，一组自动完成的操作过程。（操作过程中，不需要Client再次干预.）
*/

#if !defined(_XT_STMAC_STATE_CNT_MAX_)
#define _XT_STMAC_STATE_CNT_MAX_				((USHORT)(~0))
#endif

typedef struct _tag_xt_app_msg_t
{
	HWND hwnd;
	UINT message;
	WPARAM wParam;
	LPARAM lParam;
	DWORD time;
	POINT pt;
}xt_app_msg_t;


typedef struct _tag_xt_statemodule_t
{

}xt_statemodule_t;
/*****************************************
 *			Class CXtStateModule
 *****************************************/
class CXtStateModule
{
public:

	/* Test constrain conditions. */
	BOOL						TestAdmission(void);
	
	int							HandleEvent( int nEvent, void *pMsg, void **pTarget );
	virtual int					OnTimer(void);
	virtual int					OnError( int nErrCode );

protected:

	virtual BOOL				Constraint(void);
private:
};

/*****************************************
 *			Class CXtStateMachine
 *****************************************/
class CXtStateMachine
{
	/*****************************************************
	* Remarks:
	*	Events	: Input to CXtStateMachine.
	*	Target	: Output from CXtStateMachine.
	*	States
	*		start	: Initial state, start waiting events.
	*		halt	: End state, stop receiving events.
	*		aborted	: 
	*		complete: Target has been accomplished.
	* Sponsor: CXtDocTemplate. Modify CXtDocument state.
	* Start state: CXtDocument is NULL
	* Aborted state: CXtDocument is NULL
	* Complete state: CXtDocument is created or opened.
	 *****************************************************/

public:

	CXtStateMachine(void);
	virtual ~CXtStateMachine(void);

	enum _state_id_t { start=0, halt=_XT_STMAC_STATE_CNT_MAX_ };
	enum _end_with_t { aborted=(_XT_STMAC_STATE_CNT_MAX_-1), complete=_XT_STMAC_STATE_CNT_MAX_ };
	int							Init(void)
	{
		static xt_state_map_t _stateTable[] = 
		{
			{ _T("CXtStateModule"), CXtStateMachine::start },

			{ _T("CXtStateModule"), CXtStateMachine::aborted },
			{ _T("CXtStateModule"), CXtStateMachine::complete }
		}
	}

	virtual int					Start( CXtDocument *pSponser, *pMsg );
	virtual int					Abort(void);

	/* Input processing. Return state. */
	int							HandleEvent( int nEvent, void *pMsg, void **pTarget );

	virtual BOOL				IsHalt(void) const;

protected:

	virtual void				Reset(void);

	int							SwitchState( CXtStateModule *pState );

	void						*m_pSponsor;
	void						*m_pTarget;

	CXtStateModule				*m_pActiveState;

	UINT						m_nRef; /* Reference counter for each service owner. */

private:

};

#endif /*__XTSTATEMACHINE_H__*/
