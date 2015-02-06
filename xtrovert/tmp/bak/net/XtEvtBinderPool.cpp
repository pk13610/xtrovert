/************************************************************************************
*	XtEvtBinderPool.cpp: Implementation File
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

#include "XtEventHandler.h"
#include "xt_mem_api.h"
#include "XtEvtBinderPool.h"

#if defined(__WINDOWS__)
/* warning C4100: 'XXX' : unreferenced formal parameter */
#pragma warning (disable: 4100)
#endif

int xt_evt_pool_elem_cmp( const void *arg1, const void *arg2 )
{
#define pbinder1			((xt_evt_binder_t*)arg1)
#define pbinder2			((xt_evt_binder_t*)arg2)
	return ((pbinder1->evt_handler)) - ((pbinder2->evt_handler));
#undef pbinder1
#undef pbinder2
}

/*****************************************
 *			Class CXtEvtBinderPool
 *****************************************/

CXtEvtBinderPool::CXtEvtBinderPool(VOID)
{
	Reset();
}

CXtEvtBinderPool::~CXtEvtBinderPool(VOID)
{
	Release();
}


int CXtEvtBinderPool::Init( xt_size_t nMaxCount )/* xt_size_t nHandlerCount */
{
	/* nHandlerSize: Initial handler count */
	int nRet = FUN_RET_OK;

	if ( is_pool_valid )
	{
		return -4;
	}

	nRet = AllocPool( nMaxCount );
	if ( nRet < 0 )
	{
		return -5;
	}

	return nRet;
}

void CXtEvtBinderPool::Release(void)
{
	FreePool();
	Reset();
}

xt_evt_binder_t* CXtEvtBinderPool::AllocBinder( const CXtEventHandler *pEventHandler )
{
	xt_evt_binder_t *pBinder = NULL;
	if ( pEventHandler == NULL )
	{
		return NULL;
	}

	if ( m_BinderHeap.count >= m_BinderHeap.capacity )
	{
		return NULL;
	}

	hnd_buf[m_BinderHeap.count]->evt_handler = pEventHandler;
	pBinder = hnd_buf[m_BinderHeap.count];

	++m_BinderHeap.count;

	xt_heap_build( &m_BinderHeap );

	return pBinder;
}

// void CXtEvtBinderPool::FreeBinder( const CXtEventHandler *pEventHandler )
// {
// 	if ( pEventHandler == NULL )
// 	{
// 		return;
// 	}
// 
// 	if ( m_BinderHeap.count == 0 )
// 	{
// 		return;
// 	}
// 
// 	xt_evt_binder_t key;
// 	key.evt_handler = pEventHandler;
// 
// 	xt_heap_erase( &m_BinderHeap, (xt_heap_elem_t*)&key );
// }

xt_evt_binder_hnd_t CXtEvtBinderPool::GetBinderHandle( const CXtEventHandler *pEventHandler )
{
	xt_evt_binder_hnd_t hndEvtBinder = NULL;
	xt_evt_binder_t key;
	key.evt_handler = pEventHandler;

	hndEvtBinder = (xt_evt_binder_hnd_t)xt_heap_search( &m_BinderHeap, 0, (xt_heap_elem_t*)&key );

	return hndEvtBinder;
}

void CXtEvtBinderPool::FreeBinder( xt_evt_binder_hnd_t hndEvtBinder )
{
	xt_heap_erase_( &m_BinderHeap, (xt_heap_elem_t*)hndEvtBinder );
}

xt_evt_binder_t* CXtEvtBinderPool::GetAt( xt_size_t nIdx )
{
	if ( nIdx >= binder_count )
	{
		return NULL;
	}

	return binder_buf[nIdx];
}

int CXtEvtBinderPool::AllocPool( xt_size_t nMaxCount )
{
#define total_mem_size		((sizeof(xt_evt_binder_t*)+evt_binder_size)*nMaxCount)

	m_BinderHeap.arr = (xt_heap_elem_t*)xt_mem_alloc( total_mem_size );
	if ( hnd_buf == NULL )
	{
		return -5;
	}

	m_BinderHeap.capacity = nMaxCount;

	for( xt_size_t i=0; i<nMaxCount; ++i )
	{
		hnd_buf[i] = binder_buf[i];
	}

	return FUN_RET_OK;
}

void CXtEvtBinderPool::FreePool(void)
{
	if ( m_BinderHeap.arr )
	{
		xt_free( m_BinderHeap.arr );
		m_BinderHeap.arr = NULL;
	}
}

void CXtEvtBinderPool::Reset(void)
{
	m_BinderHeap.capacity = 0;
	m_BinderHeap.count = 0;
	m_BinderHeap.fn_cmp = xt_evt_pool_elem_cmp;
	m_BinderHeap.arr = NULL;
}

/*****************************************
 *			Class CXtEvtBinderPool END
 *****************************************/
