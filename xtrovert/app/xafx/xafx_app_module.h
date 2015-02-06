/************************************************************************************
* xafx_app_module.h : header file
*
* Definition header, An Application framework for common use
*			Xtrovert Application Frameworks (xafx, the last 'x' means nothing).
*
* AUTHOR	:	Sean Feng <SeanFeng2006@hotmail.com>
* DATE		:	Aug. 7, 2012
* Copyright (c) 2009-?. All Rights Reserved.
*
* This code may be used in compiled form in any way you desire. This
* file may be redistributed unmodified by any means PROVIDING it is 
* not sold for profit without the authors written consent, and 
* providing that this notice and the authors name and all copyright 
* notices remains int act. 
*
* An email letting me know how you are using it would be nice as well. 
*
* This file is provided "as is" with no expressed or implied warranty.
* The author accepts no liability for any damage/loss of business that
* this product may cause.
*
************************************************************************************/
#ifndef __X_AFX_APP_MODULE_H__
#define __X_AFX_APP_MODULE_H__

#include "app/xafx/xafx_msg.h"
#include "app/inc/XtThread.h" /* class CXtThread */
#include "xt_mutex.h"

typedef struct _tag_xafx_thread_module_t
{
	pthread_t		thread_id;
	xafx_msg_que_t	*msg_q_in;
	xafx_msg_que_t	*msg_q_out;
}xafx_thread_module_t;

#define xafx_thdmdl_msgqin_off								(sizeof(xafx_thread_module_t))
#define xafx_thdmdl_msgqout_off(_q_capacity)				(xafx_thdmdl_msgqin_off + xafx_msg_que_calc_size(_q_capacity))

#define _XT_THREAD_MODULE_POOL_INIT_COUNT_				4
#define _XT_THREAD_MODULE_POOL_INFLATE_COUNT_			2 /* == m_nRealCapacityGrow */

class CXtThradModulePool : public CXtObjPool
{
public:

	CXtThradModulePool(void);
	virtual ~CXtThradModulePool(void);

protected:
	void				Reset(void);

	virtual void		SetupObjSize(void);
	virtual void		ConstructObj( void *pObjAddr );
	virtual int			InitObj( void *pObjAddr );
	virtual void		ResetObj( void *pObjAddr ); /* Called in FreeObj(). */

	virtual void		SetupRealCapacityGrow(void);

	void				SetMsgQue( void *pObjAddr );

private:
};

extern CXtThradModulePool* xafx_get_threadmodulepool(void);

extern CXtThradModulePool* xafx_threadmodulepool_create(void);
extern void				xafx_threadmodulepool_destroy(void);

extern xafx_thread_module_t*	xafx_thread_msgque_get( pthread_t thread_id );
extern xafx_thread_module_t*	xafx_thread_msgque_setup( pthread_t thread_id );

extern CXtThradModulePool *_pThreadModulePool;
extern xt_mutex_t _mutex_thread_module_pool;

#endif /*__X_AFX_APP_MODULE_H__*/
