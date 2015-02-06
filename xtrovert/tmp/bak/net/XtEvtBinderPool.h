/************************************************************************************
* XtEvtBinderPool.h : header file
*
* Reactor
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
#ifndef __XT_EVT_BINDER_POOL_H__
#define __XT_EVT_BINDER_POOL_H__

#include "xt_define.h"
#include "xt_alg_heap.h"
#include <event2/event_struct.h>
#include "xt_reactor_define.h"

/* Declaration */
class CXtEventHandler;

/* Binder struct */
typedef struct _tag_xt_evt_binder_t
{
	const CXtEventHandler	*evt_handler;
	struct event			_event;
}xt_evt_binder_t;
#define evt_binder_size		((sizeof(CXtEventHandler*)+event_get_struct_event_size()))

/* Binder Handle */
typedef xt_evt_binder_t**		xt_evt_binder_hnd_t;

#define make_evt_binder_ptr(heap_elem_ptr)			((xt_evt_binder_t*)(heap_elem_ptr))
#define binder_from_handle(hndBinder)				((xt_evt_binder_t*)(*hndBinder))

/*****************************************
 *			Class CXtEvtBinderPool
 *****************************************/
class CXtEvtBinderPool
{
public:

	/* Constructor & Destructor */
	CXtEvtBinderPool(void);
	~CXtEvtBinderPool(void);

	int						Init( xt_size_t nMaxCount );
	void					Release(void);

	xt_evt_binder_t*		AllocBinder( const CXtEventHandler *pEventHandler );
	/*void					FreeBinder( const CXtEventHandler *pEventHandler );*/
	xt_evt_binder_hnd_t		GetBinderHandle( const CXtEventHandler *pEventHandler );
	void					FreeBinder( xt_evt_binder_hnd_t hndEvtBinder );

	xt_evt_binder_t*		GetAt( xt_size_t nIdx );

	xt_size_t				Count(void)								{ return m_BinderHeap.count; }

	BOOL					IsFull(void)							{ return (m_BinderHeap.count!=m_BinderHeap.capacity); }
	BOOL					IsEmpty(void)							{ return (m_BinderHeap.count==0); }

protected:

	void					Reset(void);

	int						AllocPool( xt_size_t nMaxCount );
	void					FreePool(void);

	xt_heap_t				m_BinderHeap;

#define is_pool_valid		(m_BinderHeap.arr)

#define hnd_buf				((xt_evt_binder_hnd_t)(m_BinderHeap.arr))
#define binder_capacity		(m_BinderHeap.capacity)
#define binder_count		(m_BinderHeap.count)
#define binder_buf			(hnd_buf+binder_capacity)
private:
};

/*****************************************
 *			Class CXtEvtBinderPool END
 *****************************************/

#endif /*__XT_EVT_BINDER_POOL_H__*/
