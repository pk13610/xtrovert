/************************************************************************************
*	xafx_app_module.cpp: Implementation File
*
* DESCRIPTION  :   
*
* AUTHOR	    :	Sean Feng <SeanFeng2006@hotmail.com>
* DATE		    :	Sep. 24, 2010
*
* Copyright (c) 2010-?. All Rights Reserved.
*
* REMARKS:
*		
*      
************************************************************************************/

#include "app/xafx/xafx_app_module.h"

#define XAFX_MSG_QUE_CAPACITY			(32)

CXtThradModulePool *_pThreadModulePool = NULL;
xt_mutex_t _mutex_thread_module_pool;

static int	xt_thdmdlpool_comp( const xt_obj_handle_t objHandle1, const xt_obj_handle_t objHandle2 )
{
#define thdmdlpool_key(_mdl_ptr)	(((xafx_thread_module_t*)(_mdl_ptr))->thread_id)
	return (thdmdlpool_key(objHandle1) - thdmdlpool_key(objHandle2));
#undef thdmdlpool_key
}

CXtThradModulePool::CXtThradModulePool(void)
{
	Reset();
}

CXtThradModulePool::~CXtThradModulePool(void)
{
	Reset();
}

void CXtThradModulePool::Reset(void)
{
	m_ObjHndBuf.fn_cmp = xt_thdmdlpool_comp;
}

void CXtThradModulePool::SetupObjSize(void)
{
	m_nObjSize = sizeof(xafx_thread_module_t) + xafx_msg_que_calc_size(XAFX_MSG_QUE_CAPACITY)*2; /* 2 message queue, in & out. */
}

void CXtThradModulePool::ConstructObj( void *pObjAddr )
{
	new(pObjAddr) xafx_thread_module_t;
	return;
}

int CXtThradModulePool::InitObj( void *pObjAddr )
{
	SetMsgQue(pObjAddr);
}

void CXtThradModulePool::ResetObj( void *pObjAddr )
{
	((xafx_thread_module_t*)pObjAddr)->thread_id = 0;
	xafx_msgque_clear( ((xafx_thread_module_t*)pObjAddr)->msg_q_in );
	xafx_msgque_clear( ((xafx_thread_module_t*)pObjAddr)->msg_q_out );
	return;
} /* Called in FreeObj(). */

void CXtThradModulePool::SetupRealCapacityGrow(void)
{
	m_nRealCapacityGrow = _XT_THREAD_MODULE_POOL_INFLATE_COUNT_;
}

void CXtThradModulePool::SetMsgQue( void *pObjAddr )
{
	((xafx_thread_module_t*)pObjAddr)->msg_q_in = (xafx_msg_que_t*)((char*)pObjAddr + xafx_thdmdl_msgqin_off);
	xafx_msgque_clear( ((xafx_thread_module_t*)pObjAddr)->msg_q_in );
	((xafx_thread_module_t*)pObjAddr)->msg_q_in->capacity = XAFX_MSG_QUE_CAPACITY;

	((xafx_thread_module_t*)pObjAddr)->msg_q_out = (xafx_msg_que_t*)((char*)(xafx_thdmdl_msgqout_off(XAFX_MSG_QUE_CAPACITY)));
	xafx_msgque_clear( ((xafx_thread_module_t*)pObjAddr)->msg_q_out );
	((xafx_thread_module_t*)pObjAddr)->msg_q_out->capacity = XAFX_MSG_QUE_CAPACITY;

	return;
}

CXtThradModulePool* xafx_get_threadmodulepool(void)
{
	return _pThreadModulePool;
}

CXtThradModulePool* xafx_threadmodulepool_create(void)
{
	static static CXtThradModulePool thread_module_pool;

	if ( _pThreadModulePool )
	{
		return _pThreadModulePool;
	}

	xt_mutex_init(&_mutex_thread_module_pool);

	thread_module_pool.Create( _XT_THREAD_MODULE_POOL_INIT_COUNT_ );

	_pThreadModulePool = &thread_module_pool;

	return _pThreadModulePool;
}
void xafx_threadmodulepool_destroy(void)
{
	CXtThradModulePool *pThreadModulePool = NULL;

	xt_mutex_lock(&_mutex_thread_module_pool);

	pThreadModulePool = xafx_get_threadmodulepool();
	if ( pThreadModulePool )
	{
		pThreadModulePool->Destroy();
	}
	xt_mutex_unlock(&_mutex_thread_module_pool);

	_pThreadModulePool = NULL;
	xt_mutex_destroy(&_mutex_thread_module_pool);
}

xafx_thread_module_t* xafx_thread_msgque_get( pthread_t thread_id )
{
	CXtThradModulePool *pThreadModulePool = xafx_get_threadmodulepool();
	xafx_thread_module_t *thread_module = NULL;
	xafx_thread_module_t key_thd_module;
	key_thd_module.thread_id = thread_id;

	if ( pThreadModulePool == NULL )
	{
		return NULL;
	}

	thread_module = pThreadModulePool->SearchObj( &key_thd_module );
	if ( thread_module )
	{
		
		return thread_module;
	}

	return NULL;
}


xafx_thread_module_t* xafx_thread_msgque_setup( pthread_t thread_id )
{
	CXtThradModulePool *pThreadModulePool = xafx_get_threadmodulepool();
	xafx_thread_module_t *thread_module = NULL;
	xafx_thread_module_t key_thd_module;
	key_thd_module.thread_id = thread_id;

	if ( pThreadModulePool == NULL )
	{
		return NULL;
	}

	xt_mutex_lock(&_mutex_thread_module_pool);	
	thread_module = pThreadModulePool->SearchObj( &key_thd_module );
	if ( thread_module )
	{
		xt_mutex_unlock(&_mutex_thread_module_pool);
		return thread_module;
	}

	thread_module = (xafx_thread_module_t*)pThreadModulePool->AllocObj();
	if ( thread_module == NULL )
	{
		xt_mutex_unlock(&_mutex_thread_module_pool);
		return NULL;
	}
	thread_module->thread_id = thread_id;
	pThreadModulePool->ActivateObj();

	xt_mutex_unlock(&_mutex_thread_module_pool);
	return thread_module;
}

