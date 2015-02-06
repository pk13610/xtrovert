/************************************************************************************
* XtAppMsgMap.h : header file
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
#ifndef __XTAPPMSGMAP_H__
#define __XTAPPMSGMAP_H__

#include "xt_define.h"

/* pointer to afx_msg member function */
#if !defined(XT_MSG_CALL)
#define XT_MSG_CALL
#endif

class CXtCmdTarget;
typedef void (XT_MSG_CALL CXtCmdTarget::*xt_pfn_msghandler_t)(void);

/**********************************************************************
*		Xtrovert Application Message signature
***********************************************************************/
enum xt_signatrue
{
	xtsig_end = 0,     // [marks end of message map]

	xtsig_b_D_v,				// BOOL (CDC*)
	xtsig_b_b_v,				// BOOL (BOOL)
	xtsig_b_u_v,				// BOOL (UINT)
	xtsig_b_h_v,				// BOOL (HANDLE)
	xtsig_b_W_uu,				// BOOL (CWnd*, UINT, UINT)
	xtsig_b_W_COPYDATASTRUCT,				// BOOL (CWnd*, COPYDATASTRUCT*)
	xtsig_b_v_HELPINFO,		// BOOL (LPHELPINFO);
	xtsig_CTLCOLOR,			// HBRUSH (CDC*, CWnd*, UINT)
	xtsig_CTLCOLOR_REFLECT,	// HBRUSH (CDC*, UINT)
	xtsig_i_u_W_u,				// int (UINT, CWnd*, UINT)  // ?TOITEM
	xtsig_i_uu_v,				// int (UINT, UINT)
	xtsig_i_W_uu,				// int (CWnd*, UINT, UINT)
	xtsig_i_v_s,				// int (LPTSTR)
	xtsig_l_w_l,				// LRESULT (WPARAM, LPARAM)
	xtsig_l_uu_M,				// LRESULT (UINT, UINT, CMenu*)
	xtsig_v_b_h,				// void (BOOL, HANDLE)
	xtsig_v_h_v,				// void (HANDLE)
	xtsig_v_h_h,				// void (HANDLE, HANDLE)
	xtsig_v_v_v,				// void ()
	xtsig_v_u_v,				// void (UINT)
	xtsig_v_u_u,				// void (UINT, UINT)
	xtsig_v_uu_v,				// void (UINT, UINT)
	xtsig_v_v_ii,				// void (int, int)
	xtsig_v_u_uu,				// void (UINT, UINT, UINT)
	xtsig_v_u_ii,				// void (UINT, int, int)
	xtsig_v_u_W,				// void (UINT, CWnd*)
	xtsig_i_u_v,				// int (UINT)
	xtsig_u_u_v,				// UINT (UINT)
	xtsig_b_v_v,				// BOOL ()
	xtsig_v_w_l,				// void (WPARAM, LPARAM)
	xtsig_MDIACTIVATE,			// void (BOOL, CWnd*, CWnd*)
	xtsig_v_D_v,				// void (CDC*)
	xtsig_v_M_v,				// void (CMenu*)
	xtsig_v_M_ub,				// void (CMenu*, UINT, BOOL)
	xtsig_v_W_v,				// void (CWnd*)
	xtsig_v_v_W,				// void (CWnd*)
	xtsig_v_W_uu,				// void (CWnd*, UINT, UINT)
	xtsig_v_W_p,				// void (CWnd*, CPoint)
	xtsig_v_W_h,				// void (CWnd*, HANDLE)
	xtsig_C_v_v,				// HCURSOR ()
	xtsig_ACTIVATE,			// void (UINT, CWnd*, BOOL)
	xtsig_SCROLL,				// void (UINT, UINT, CWnd*)
	xtsig_SCROLL_REFLECT,		// void (UINT, UINT)
	xtsig_v_v_s,				// void (LPTSTR)
	xtsig_v_u_cs,				// void (UINT, LPCTSTR)
	xtsig_OWNERDRAW,			// void (int, LPTSTR) force return TRUE
	xtsig_i_i_s,				// int (int, LPTSTR)
	xtsig_u_v_p,				// UINT (CPoint)
	xtsig_u_v_v,				// UINT ()
	xtsig_v_b_NCCALCSIZEPARAMS,		// void (BOOL, NCCALCSIZE_PARAMS*)
	xtsig_v_v_WINDOWPOS,			// void (WINDOWPOS*)
	xtsig_v_uu_M,				// void (UINT, UINT, HMENU)
	xtsig_v_u_p,				// void (UINT, CPoint)
	xtsig_SIZING,				// void (UINT, LPRECT)
	xtsig_MOUSEWHEEL,			// BOOL (UINT, short, CPoint)
	xtsig_MOUSEHWHEEL,			// void (UINT, short, CPoint)
	xtsigcmd_v,				// void ()
	xtsigcmd_b,				// BOOL ()
	xtsigcmd_RANGE,			// void (UINT)
	xtsigcmd_EX,				// BOOL (UINT)
	xtsignotify_v,				// void (NMHDR*, LRESULT*)
	xtsignotify_b,				// BOOL (NMHDR*, LRESULT*)
	xtsignotify_RANGE,			// void (UINT, NMHDR*, LRESULT*)
	xtsignotify_EX,			// BOOL (UINT, NMHDR*, LRESULT*)
	xtsigcmdui,				// void (CCmdUI*)
	xtsigcmdui_RANGE,			// void (CCmdUI*, UINT)
	xtsigcmd_v_pv,				// void (void*)
	xtsigcmd_b_pv,				// BOOL (void*)
	xtsig_l,				// LRESULT ()
	xtsig_l_p,				// LRESULT (CPOINT)	
	xtsig_u_W_u,				// UINT (CWnd*, UINT)
	xtsig_v_u_M,				// void (UINT, CMenu* )
	xtsig_u_u_M,				// UINT (UINT, CMenu* )
	xtsig_u_v_MENUGETOBJECTINFO,		// UINT (MENUGETOBJECTINFO*)
	xtsig_v_M_u,				// void (CMenu*, UINT)
	xtsig_v_u_LPMDINEXTMENU,		// void (UINT, LPMDINEXTMENU)
	xtsig_APPCOMMAND,			// void (CWnd*, UINT, UINT, UINT)
	xtsig_RAWINPUT,			// void (UINT, HRAWINPUT)
	xtsig_u_u_u,				// UINT (UINT, UINT)
	xtsig_MOUSE_XBUTTON,			// void (UINT, UINT, CPoint)
	xtsig_MOUSE_NCXBUTTON,			// void (short, UINT, CPoint)
	xtsig_INPUTLANGCHANGE,			// void (UINT, UINT)
	xtsig_v_u_hkl,					// void (UINT, HKL)
	xtsig_INPUTDEVICECHANGE,		// void (unsigned short, HANDLE)
	// Old
	xtsig_bD = xtsig_b_D_v,      // BOOL (CDC*)
	xtsig_bb = xtsig_b_b_v,      // BOOL (BOOL)
	xtsig_bWww = xtsig_b_W_uu,    // BOOL (CWnd*, UINT, UINT)
	xtsig_hDWw = xtsig_CTLCOLOR,    // HBRUSH (CDC*, CWnd*, UINT)
	xtsig_hDw = xtsig_CTLCOLOR_REFLECT,     // HBRUSH (CDC*, UINT)
	xtsig_iwWw = xtsig_i_u_W_u,    // int (UINT, CWnd*, UINT)
	xtsig_iww = xtsig_i_uu_v,     // int (UINT, UINT)
	xtsig_iWww = xtsig_i_W_uu,    // int (CWnd*, UINT, UINT)
	xtsig_is = xtsig_i_v_s,      // int (LPTSTR)
	xtsig_lwl = xtsig_l_w_l,     // LRESULT (WPARAM, LPARAM)
	xtsig_lwwM = xtsig_l_uu_M,    // LRESULT (UINT, UINT, CMenu*)
	xtsig_vv = xtsig_v_v_v,      // void (void)

	xtsig_vw = xtsig_v_u_v,      // void (UINT)
	xtsig_vww = xtsig_v_u_u,     // void (UINT, UINT)
	xtsig_vww2 = xtsig_v_uu_v,    // void (UINT, UINT) // both come from wParam
	xtsig_vvii = xtsig_v_v_ii,    // void (int, int) // wParam is ignored
	xtsig_vwww = xtsig_v_u_uu,    // void (UINT, UINT, UINT)
	xtsig_vwii = xtsig_v_u_ii,    // void (UINT, int, int)
	xtsig_vwl = xtsig_v_w_l,     // void (UINT, LPARAM)
	xtsig_vbWW = xtsig_MDIACTIVATE,    // void (BOOL, CWnd*, CWnd*)
	xtsig_vD = xtsig_v_D_v,      // void (CDC*)
	xtsig_vM = xtsig_v_M_v,      // void (CMenu*)
	xtsig_vMwb = xtsig_v_M_ub,    // void (CMenu*, UINT, BOOL)

	xtsig_vW = xtsig_v_W_v,      // void (CWnd*)
	xtsig_vWww = xtsig_v_W_uu,    // void (CWnd*, UINT, UINT)
	xtsig_vWp = xtsig_v_W_p,     // void (CWnd*, CPoint)
	xtsig_vWh = xtsig_v_W_h,     // void (CWnd*, HANDLE)
	xtsig_vwW = xtsig_v_u_W,     // void (UINT, CWnd*)
	xtsig_vwWb = xtsig_ACTIVATE,    // void (UINT, CWnd*, BOOL)
	xtsig_vwwW = xtsig_SCROLL,    // void (UINT, UINT, CWnd*)
	xtsig_vwwx = xtsig_SCROLL_REFLECT,    // void (UINT, UINT)
	xtsig_vs = xtsig_v_v_s,      // void (LPTSTR)
	xtsig_vOWNER = xtsig_OWNERDRAW,  // void (int, LPTSTR), force return TRUE
	xtsig_iis = xtsig_i_i_s,     // int (int, LPTSTR)
	xtsig_wp = xtsig_u_v_p,      // UINT (CPoint)
	xtsig_wv = xtsig_u_v_v,      // UINT (void)
	xtsig_vPOS = xtsig_v_v_WINDOWPOS,    // void (WINDOWPOS*)
	xtsig_vCALC = xtsig_v_b_NCCALCSIZEPARAMS,   // void (BOOL, NCCALCSIZE_PARAMS*)
	xtsig_vNMHDRpl = xtsignotify_v,    // void (NMHDR*, LRESULT*)
	xtsig_bNMHDRpl = xtsignotify_b,    // BOOL (NMHDR*, LRESULT*)
	xtsig_vwNMHDRpl = xtsignotify_RANGE,   // void (UINT, NMHDR*, LRESULT*)
	xtsig_bwNMHDRpl = xtsignotify_EX,   // BOOL (UINT, NMHDR*, LRESULT*)
	xtsig_bHELPINFO = xtsig_b_v_HELPINFO,   // BOOL (HELPINFO*)
	xtsig_vwSIZING = xtsig_SIZING,    // void (UINT, LPRECT) -- return TRUE

	// signatures specific to CCmdTarget
	xtsig_cmdui = xtsigcmdui,   // void (CCmdUI*)
	xtsig_cmduiw = xtsigcmdui_RANGE,  // void (CCmdUI*, UINT)
	xtsig_vpv = xtsigcmd_v_pv,     // void (void*)
	xtsig_bpv = xtsigcmd_b_pv,     // BOOL (void*)

	// Other aliases (based on implementation)
	xtsig_vwwh = xtsig_v_uu_M,                // void (UINT, UINT, HMENU)
	xtsig_vwp = xtsig_v_u_p,                 // void (UINT, CPoint)
	xtsig_bw = xtsig_b_u_v,      // BOOL (UINT)
	xtsig_bh = xtsig_b_h_v,      // BOOL (HANDLE)
	xtsig_iw = xtsig_i_u_v,      // int (UINT)
	xtsig_ww = xtsig_u_u_v,      // UINT (UINT)
	xtsig_bv = xtsig_b_v_v,      // BOOL (void)
	xtsig_hv = xtsig_C_v_v,      // HANDLE (void)
	xtsig_vb = xtsig_vw,      // void (BOOL)
	xtsig_vbh = xtsig_v_b_h,    // void (BOOL, HANDLE)
	xtsig_vbw = xtsig_vww,    // void (BOOL, UINT)
	xtsig_vhh = xtsig_v_h_h,    // void (HANDLE, HANDLE)
	xtsig_vh = xtsig_v_h_v,      // void (HANDLE)
	xtsig_viSS = xtsig_vwl,   // void (int, STYLESTRUCT*)
	xtsig_bwl = xtsig_lwl,
	xtsig_vwMOVING = xtsig_vwSIZING,  // void (UINT, LPRECT) -- return TRUE

	xtsig_vW2 = xtsig_v_v_W,                 // void (CWnd*) (CWnd* comes from lParam)
	xtsig_bWCDS = xtsig_b_W_COPYDATASTRUCT,               // BOOL (CWnd*, COPYDATASTRUCT*)
	xtsig_bwsp = xtsig_MOUSEWHEEL,                // BOOL (UINT, short, CPoint)
	xtsig_vws = xtsig_v_u_cs,
};

/**********************************************************************
*		Message map and message dispatch
***********************************************************************/
union xt_appmsg_map_functions_t
{
	xt_pfn_msghandler_t pfn;   /* generic member function pointer */

	BOOL (XT_MSG_CALL CCmdTarget::*pfn_b_D)(CDC*);
	BOOL (XT_MSG_CALL CCmdTarget::*pfn_b_b)(BOOL);
	BOOL (XT_MSG_CALL CCmdTarget::*pfn_b_u)(UINT);
	BOOL (XT_MSG_CALL CCmdTarget::*pfn_b_h)(HANDLE);
	BOOL (XT_MSG_CALL CCmdTarget::*pfn_b_W_u_u)(CWnd*, UINT, UINT);
	BOOL (XT_MSG_CALL CCmdTarget::*pfn_b_W_COPYDATASTRUCT)(CWnd*, COPYDATASTRUCT*);
	BOOL (XT_MSG_CALL CCmdTarget::*pfn_b_HELPINFO)(LPHELPINFO);
	HBRUSH (XT_MSG_CALL CCmdTarget::*pfn_B_D_W_u)(CDC*, CWnd*, UINT);
	HBRUSH (XT_MSG_CALL CCmdTarget::*pfn_B_D_u)(CDC*, UINT);
	int (XT_MSG_CALL CCmdTarget::*pfn_i_u_W_u)(UINT, CWnd*, UINT);
	int (XT_MSG_CALL CCmdTarget::*pfn_i_u_u)(UINT, UINT);
	int (XT_MSG_CALL CCmdTarget::*pfn_i_W_u_u)(CWnd*, UINT, UINT);
	int (XT_MSG_CALL CWnd::*pfn_i_s)(LPTSTR);
	LRESULT (XT_MSG_CALL CWnd::*pfn_l_w_l)(WPARAM, LPARAM);
	LRESULT (XT_MSG_CALL CWnd::*pfn_l_u_u_M)(UINT, UINT, CMenu*);
	void (XT_MSG_CALL CWnd::*pfn_v_b_h)(BOOL, HANDLE);
	void (XT_MSG_CALL CWnd::*pfn_v_h)(HANDLE);
	void (XT_MSG_CALL CWnd::*pfn_v_h_h)(HANDLE,HANDLE);
	void (XT_MSG_CALL CWnd::*pfn_v_v)();
	int (XT_MSG_CALL CWnd::*pfn_i_u)(UINT);
	HCURSOR (XT_MSG_CALL CWnd::*pfn_C_v)();
	UINT (XT_MSG_CALL CWnd::*pfn_u_u)(UINT);
	BOOL (XT_MSG_CALL CWnd::*pfn_b_v)();
	void (XT_MSG_CALL CWnd::*pfn_v_u)(UINT);
	void (XT_MSG_CALL CWnd::*pfn_v_u_u)(UINT, UINT);
	void (XT_MSG_CALL CWnd::*pfn_v_i_i)(int, int);
	void (XT_MSG_CALL CWnd::*pfn_v_u_u_u)(UINT, UINT, UINT);
	void (XT_MSG_CALL CWnd::*pfn_v_u_i_i)(UINT, int, int);
	void (XT_MSG_CALL CWnd::*pfn_v_w_l)(WPARAM, LPARAM);
	void (XT_MSG_CALL CWnd::*pfn_v_b_W_W)(BOOL, CWnd*, CWnd*);
	void (XT_MSG_CALL CWnd::*pfn_v_D)(CDC*);
	void (XT_MSG_CALL CWnd::*pfn_v_M)(CMenu*);
	void (XT_MSG_CALL CWnd::*pfn_v_M_u_b)(CMenu*, UINT, BOOL);
	void (XT_MSG_CALL CWnd::*pfn_v_W)(CWnd*);
	void (XT_MSG_CALL CWnd::*pfn_v_W_u_u)(CWnd*, UINT, UINT);
	void (XT_MSG_CALL CWnd::*pfn_v_W_p)(CWnd*, CPoint);
	void (XT_MSG_CALL CWnd::*pfn_v_W_h)(CWnd*, HANDLE);
	void (XT_MSG_CALL CWnd::*pfn_v_u_W)(UINT, CWnd*);
	void (XT_MSG_CALL CWnd::*pfn_v_u_W_b)(UINT, CWnd*, BOOL);
	void (XT_MSG_CALL CWnd::*pfn_v_u_u_W)(UINT, UINT, CWnd*);
	void (XT_MSG_CALL CWnd::*pfn_v_s)(LPTSTR);
	void (XT_MSG_CALL CWnd::*pfn_v_u_cs)(UINT, LPCTSTR);
	void (XT_MSG_CALL CWnd::*pfn_v_i_s)(int, LPTSTR);
	int (XT_MSG_CALL CWnd::*pfn_i_i_s)(int, LPTSTR);
	UINT (XT_MSG_CALL CWnd::*pfn_u_p)(CPoint);
	LRESULT (XT_MSG_CALL CWnd::*pfn_l_p)(CPoint);
	UINT (XT_MSG_CALL CWnd::*pfn_u_v)();
	void (XT_MSG_CALL CWnd::*pfn_v_b_NCCALCSIZEPARAMS)(BOOL, NCCALCSIZE_PARAMS*);
	void (XT_MSG_CALL CWnd::*pfn_v_v_WINDOWPOS)(WINDOWPOS*);
	void (XT_MSG_CALL CWnd::*pfn_v_u_u_M)(UINT, UINT, HMENU);
	void (XT_MSG_CALL CWnd::*pfn_v_u_p)(UINT, CPoint);
	void (XT_MSG_CALL CWnd::*pfn_v_u_pr)(UINT, LPRECT);
	BOOL (XT_MSG_CALL CWnd::*pfn_b_u_s_p)(UINT, short, CPoint);
	void (XT_MSG_CALL CWnd::*pfn_MOUSEHWHEEL)(UINT, short, CPoint);
	LRESULT (XT_MSG_CALL CWnd::*pfn_l_v)();
	UINT (XT_MSG_CALL CWnd::*pfn_u_W_u)(CWnd*, UINT);
	void (XT_MSG_CALL CWnd::*pfn_v_u_M)(UINT, CMenu*);
	UINT (XT_MSG_CALL CWnd::*pfn_u_u_M)(UINT, CMenu*);
	UINT (XT_MSG_CALL CWnd::*pfn_u_v_MENUGETOBJECTINFO)(MENUGETOBJECTINFO*);
	void (XT_MSG_CALL CWnd::*pfn_v_M_u)(CMenu*, UINT);
	void (XT_MSG_CALL CWnd::*pfn_v_u_LPMDINEXTMENU)(UINT, LPMDINEXTMENU);
	void (XT_MSG_CALL CWnd::*pfn_APPCOMMAND)(CWnd*, UINT, UINT, UINT);
	BOOL (XT_MSG_CALL CWnd::*pfn_RAWINPUT)(UINT, HRAWINPUT);
	UINT (XT_MSG_CALL CWnd::*pfn_u_u_u)(UINT, UINT);
	void (XT_MSG_CALL CWnd::*pfn_MOUSE_XBUTTON)(UINT, UINT, CPoint);
	void (XT_MSG_CALL CWnd::*pfn_MOUSE_NCXBUTTON)(short, UINT, CPoint);
	void (XT_MSG_CALL CWnd::*pfn_INPUTLANGCHANGE)(UINT, UINT);
	BOOL (XT_MSG_CALL CWnd::*pfn_v_u_h)(UINT, HANDLE);
	void (XT_MSG_CALL CWnd::*pfn_INPUTDEVICECHANGE)(unsigned short, HANDLE);

	/* type safe variant for thread messages */
	void (XT_MSG_CALL CWinThread::*pfn_THREAD)(WPARAM, LPARAM);

	/* specific type safe variants for WM_COMMAND and WM_NOTIFY messages */
	void (XT_MSG_CALL CCmdTarget::*pfnCmd_v_v)();
	BOOL (XT_MSG_CALL CCmdTarget::*pfnCmd_b_v)();
	void (XT_MSG_CALL CCmdTarget::*pfnCmd_v_u)(UINT);
	BOOL (XT_MSG_CALL CCmdTarget::*pfnCmd_b_u)(UINT);

	void (XT_MSG_CALL CCmdTarget::*pfnNotify_v_NMHDR_pl)(NMHDR*, LRESULT*);
	BOOL (XT_MSG_CALL CCmdTarget::*pfnNotify_b_NMHDR_pl)(NMHDR*, LRESULT*);
	void (XT_MSG_CALL CCmdTarget::*pfnNotify_v_u_NMHDR_pl)(UINT, NMHDR*, LRESULT*);
	BOOL (XT_MSG_CALL CCmdTarget::*pfnNotify_b_u_NMHDR_pl)(UINT, NMHDR*, LRESULT*);
	void (XT_MSG_CALL CCmdTarget::*pfnCmdUI_v_C)(CCmdUI*);
	void (XT_MSG_CALL CCmdTarget::*pfnCmdUI_v_C_u)(CCmdUI*, UINT);

	void (XT_MSG_CALL CCmdTarget::*pfnCmd_v_pv)(void*);
	BOOL (XT_MSG_CALL CCmdTarget::*pfnCmd_b_pv)(void*);

	void    (XT_MSG_CALL CWnd::*pfn_vPOS)(WINDOWPOS*);
	void    (XT_MSG_CALL CWnd::*pfn_vCALC)(BOOL, NCCALCSIZE_PARAMS*);
	void    (XT_MSG_CALL CWnd::*pfn_vwp)(UINT, CPoint);
	void    (XT_MSG_CALL CWnd::*pfn_vwwh)(UINT, UINT, HANDLE);
	BOOL    (XT_MSG_CALL CWnd::*pfn_bwsp)(UINT, short, CPoint);
};

const xt_appmsg_entry_t* XT_STDECL xt_find_msg_entry( const xt_appmsg_entry_t *entry, UINT nMsg, UINT nCode, UINT nID );

/**********************************************************************
*		Xtrovert Application Message map
***********************************************************************/
typedef struct _tag_xt_appmsg_entry_t
{
	UINT				msg;			/* XtApp Message */
	UINT				code;			/* Control Code or NOTIFY code */
	UINT				svc_id;			/* Control ID or 0 for windows */
	UINT				last_svc_id;	/* Used for entries specifying a range of control of id's  */
	UINT_PTR			sig;			/* Signature type(action) or pointer to message */
	xt_pfn_msghandler_t	pfn;			/* routine to call (or special value) */
}xt_appmsg_entry_t;

#define xt_appmsg_entry_null				{ 0, 0, 0, 0, xtsig_end, (xt_pfn_msghandler_t)0 }

typedef struct _tag_xt_appmsg_map_t
{
	const struct _tag_xt_appmsg_map_t* (XT_STDECL* pfnGetBaseMap)();
	const struct xt_appmsg_entry_t		 *entries;
}xt_appmsg_map_t;

#define XT_DECLARE_MSGMAP() \
protected: \
	static const xt_appmsg_map_t* XT_STDECL GetThisMessageMap(); \
	virtual const xt_appmsg_map_t* GetMessageMap() const; \

#define XT_BEGIN_MSGMAP(theClass, baseClass) \
	XT_PTM_WARNING_DISABLE \
	const xt_appmsg_map_t* theClass::GetMessageMap() const \
		{ return GetThisMessageMap(); } \
	const xt_appmsg_map_t* XT_STDECL theClass::GetThisMessageMap() \
	{ \
		typedef theClass ThisClass;						   \
		typedef baseClass TheBaseClass;					   \
		static const xt_appmsg_entry_t _messageEntries[] =  \
		{

#define XT_ON_COMMAND(id, memberFxn)	\
			{ WM_COMMAND, XT_CN_COMMAND, (WORD)id, (WORD)id, xtsigcmd_v, static_cast<xt_pfn_msghandler_t>(memberFxn) },

#define XT_ON_NOTIFY(wNotifyCode, id, memberFxn) \
			{ WM_NOTIFY, (WORD)(int)wNotifyCode, (WORD)id, (WORD)id, AfxSigNotify_v, (xt_pfn_msghandler_t) \

#define XT_END_MSGMAP() \
			xt_appmsg_entry_null \
		}; \
		static const xt_appmsg_map_t messageMap = { &TheBaseClass::GetThisMessageMap, &_messageEntries[0] }; \
		return &messageMap; \
	}								  \
	XT_PTM_WARNING_RESTORE


#endif /*__XTAPPMSGMAP_H__*/
