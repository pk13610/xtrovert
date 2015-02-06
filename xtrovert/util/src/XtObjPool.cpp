/************************************************************************************
* XtObjPool.cpp: Implementation File
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

#include "xt_mem_api.h"
#include "XtObjPool.h"
#include "xt_alg_heap.h"

#if defined(__WINDOWS__)
/* warning C4100: 'XXX' : unreferenced formal parameter */
#pragma warning (disable: 4127)
#pragma warning (disable: 4100)
#endif

/* Non-Lock queue, for one consumer and one producer. */
typedef struct _tag_xt_ptrque_t
{
	xt_size_t	capacity;
	xt_size_t	head;
	xt_size_t	tail;
	USHORT		readable;
	USHORT		writeable;
	void		*elem[0];
}xt_ptrque_t;

#define xt_ptrque_count(_q_ptr) (((_q_ptr)->tail+(_q_ptr)->capacity-(_q_ptr)->head)%((_q_ptr)->capacity))

void* xt_ptrque_pop( xt_ptrque_t *q_ptr )
{
	void *ret = NULL;
	if ( q_ptr->head == q_ptr->tail )
	{
		if ( q_ptr->readable == 0 )
		{
			q_ptr->writeable = 1;
			return NULL;
		}
		else /* q_ptr->readable == 1 */
		{
			q_ptr->writeable = 0;
		}
	}

	ret = q_ptr->elem[q_ptr->head];

	++(q_ptr->head);
	q_ptr->head %= q_ptr->capacity;

	return ret;
}

int xt_ptrque_push( xt_ptrque_t *q_ptr, void *elem )
{
	if ( q_ptr->head == q_ptr->tail )
	{
		if ( q_ptr->readable == 1 )
		{
			q_ptr->writeable = 0;
			return -5; /* Queue is full. */
		}
		else /* q_ptr->readable == 0 */
		{
			q_ptr->writeable = 1;
		}
	}

	q_ptr->elem[q_ptr->tail] = elem;

	++(q_ptr->tail);
	q_ptr->tail %= q_ptr->capacity;

	return 0;
}

static int	xt_obj_pool_comp( const xt_obj_handle_t objHandle1, const xt_obj_handle_t objHandle2 )
{
	return ((char*)objHandle1 - (char*)objHandle2);
}

/**************************************************************
		Class CExtObjPool
**************************************************************/

#define _xt_poolchk_node_size_avail_		(_MEM_PAGE_SIZE_DEFAULT_- sizeof(xt_poolchk_node_t))

const xt_size_t _XT_OBJPOOL_CAPACITY_INIT_ = ((_MEM_PAGE_SIZE_DEFAULT_-sizeof(xt_obj_handle_buf_t))/sizeof(void*));
const xt_size_t _XT_OBJPOOL_COUNT_IN_MEMPAGE_ =  1024;

CXtObjPool::CXtObjPool(void)
{
	Reset();
}

CXtObjPool( xt_size_t nObjSize ) : nObjSize(m_nObjSize)
{
	memset( &m_ObjHndBuf, 0, sizeof(xt_obj_handle_buf_t) );
	m_ObjHndBuf.fn_cmp = xt_obj_pool_comp;

	/* Object data storage. Chunk Data */
	m_nChunkCount = 0;	
	m_pMemChunkList = NULL;

	m_nRealCapacity = 0; /* if (m_ObjHndBuf.capacity==m_nRealCapacity) { AllocChunk(m_nGrowCapacity); } */
	m_nRealCapacityGrow = grow_capacity_def; /* if (m_ObjHndBuf.capacity==m_nRealCapacity) { AllocChunk(); } */

	/*m_nGrowPool = 0;*/ /* if (m_ObjHndBuf.capacity==m_ObjHndBuf.count) { AllocChunk(m_nGrowCapacity); } */

	m_phndLateObj = NULL;
	m_phndActiveObj = NULL;
}

CXtObjPool::~CXtObjPool(void)
{
	Destroy();
}

/* Create object pool with capacity of nObjCount elements */
int CXtObjPool::Create( xt_size_t nObjCount )
{
	/*
	*	SetupObjSize();
	*	Allocate handle-buffer memory.
	*	InflateRealCapacity();
	*/
	
	/*
		Return : Object_Count == m_ObjHndBuf.capacity
	*/
	int nRet = FUN_RET_OK;

	if ( is_pool_valid )
	{
		return FUN_RET_ERR_ILLEGAL_CALL;
	}

	if ( nObjCount==0 || nObjCount==_INVALID_SIZE_T_VALUE_ )
	{
		return FUN_RET_ERR_ILLEGAL_ARGS;
	}

	SetupObjSize();
	if ( m_nObjSize==0 || m_nObjSize==_INVALID_SIZE_T_VALUE_ )
	{
		return FUN_RET_ERR_ILLEGAL_CALL;
	}

	/* Grow gradient. */
	SetupRealCapacityGrow();

	nRet = InflatePool( nObjCount );
	if ( nRet < 0 )
	{
		return -5;
	}

	OnInitPool();

	return nRet;
}

int CXtObjPool::InflatePool( xt_size_t nInflateCount )
{
	/*
		Return : inflated count == nObjCount
	*/
	int nRet = FUN_RET_OK;

	if ( nInflateCount==0 || nInflateCount==_INVALID_SIZE_T_VALUE_ )
	{
		return FUN_RET_ERR_ILLEGAL_ARGS;
	}

	if ( !is_pool_elem_valid )
	{
		return FUN_RET_ERR_ILLEGAL_CALL;
	}

	/* Create handle buffer. */
	nRet = InflateCapacity( nInflateCount );
	if ( nRet < 0 )
	{
		return (-1*XT_STD_ERR_ENOMEM);
	}

	if ( to_inflate_realcapacity )
	{
		nRet = InflateRealCapacity();
		if ( nRet == 0 )
		{
			return -7;
		}
	}

	return nObjCount;
}

void CXtObjPool::Destroy(void)
{
	/* Link new chunk to tail of chunk list */
	xt_poolchk_node_t* pChunk = NULL;

	OnReleasePool();

	/* Free all chunk_nodes */
	while( m_pMemChunkList != NULL )
	{
		FreeChunk( m_pMemChunkList );
	}

	/* Free handle buffer array. */
	if ( m_ObjHndBuf.handle_arr )
	{
		xt_mem_free( m_ObjHndBuf.handle_arr );
	}

	Reset();
}

void CXtObjPool::Clear(void)
{
	m_ObjHndBuf.count = 0;
}

void* CXtObjPool::AllocObj( xt_obj_id_t *pObjId )
{
	if ( is_pool_full )
	{
		return NULL;
	}

	if ( m_phndActiveObj == NULL )
	{
		m_phndActiveObj = AllocActiveObj();
		if ( m_phndActiveObj == NULL )
		{
			return NULL;
		}
	}

	if ( pObjId )
	{
		*pObjId = 0;
	}

	return (*m_phndActiveObj);
}

int CXtObjPool::ActivateObj(void)
{
	if ( m_phndActiveObj==NULL || !is_pool_valid )
	{
		return FUN_RET_ERR_ILLEGAL_CALL;
	}

	HandleArrPush();

	m_phndActiveObj = NULL;
	return FUN_RET_OK;
}

int CXtObjPool::FreeObj( void *pObj )
{
	if ( !is_pool_valid )
	{
		return FUN_RET_ERR_ILLEGAL_CALL;
	}

	if ( m_phndLateObj == NULL )
	{
		/* Search */
		xt_obj_handle_t x = (xt_obj_handle_t)pObj;
		m_phndLateObj = HandleArrSearch( 0, x );
		if ( m_phndLateObj == NULL )
		{
			return -5;
		}
	}
	else if ( *m_phndLateObj != pObj )
	{
		/* Search */
		xt_obj_handle_t x = (xt_obj_handle_t)pObj;
		m_phndLateObj = HandleArrSearch( 0, x );
		if ( m_phndLateObj == NULL )
		{
			return -6;
		}
	}

	/* erase m_pLateObjHnd */
	HandleArrErase_( m_phndLateObj );
	m_phndLateObj = NULL;

	ResetObj( pObj );

	return FUN_RET_OK;
}

void* CXtObjPool::GetAt( xt_size_t nIndex )
{
	if ( nIndex >= m_ObjHndBuf.count )
	{
		return NULL;
	}

	return (m_ObjHndBuf.handle_arr[nIndex]);
}

void* CXtObjPool::SearchObj( const void *pKeyObj )
{
	if ( !is_pool_valid )
	{
		return NULL;
	}

	xt_obj_handle_t x = (xt_obj_handle_t)pKeyObj;
	m_phndLateObj = HandleArrSearch( 0, x );
	if ( m_phndLateObj == NULL )
	{
		return NULL;
	}

	return (*m_phndLateObj);
}

void CXtObjPool::Reset(void)
{
	m_nObjSize = 0;

	memset( &m_ObjHndBuf, 0, sizeof(xt_obj_handle_buf_t) );
	m_ObjHndBuf.fn_cmp = xt_obj_pool_comp;

	/* Object data storage. Chunk Data */
	m_nChunkCount = 0;	
	m_pMemChunkList = NULL;

	m_nRealCapacity = 0; /* if (m_ObjHndBuf.capacity==m_nRealCapacity) { AllocChunk(m_nGrowCapacity); } */
	m_nRealCapacityGrow = grow_capacity_def; /* if (m_ObjHndBuf.capacity==m_nRealCapacity) { AllocChunk(); } */

	/*m_nGrowPool = 0;*/ /* if (m_ObjHndBuf.capacity==m_ObjHndBuf.count) { AllocChunk(m_nGrowCapacity); } */

	m_phndLateObj = NULL;
	m_phndActiveObj = NULL;
}

int CXtObjPool::OnInitPool(void)
{
	return FUN_RET_OK;
}

void CXtObjPool::OnReleasePool(void)
{
	return;
}

void CXtObjPool::SetupRealCapacityGrow(void)
{
	m_nRealCapacityGrow = _xt_poolchk_node_size_avail_ / m_nObjSize;

	m_nRealCapacityGrow = (m_nRealCapacityGrow==0)?1:m_nRealCapacityGrow;
}

xt_obj_handle_t* CXtObjPool::AllocActiveObj(void)
{
	if ( to_inflate_realcapacity )
	{
		xt_size_t nRet = FUN_RET_OK;
		nRet = InflateRealCapacity();
		if ( nRet == 0 )
		{
			return NULL;
		}
	}

	return &(m_ObjHndBuf.handle_arr[m_ObjHndBuf.count]);
}

int CXtObjPool::InflateCapacity( xt_size_t nInflateCount )
{
	/*
		Expected Capacity: m_ObjHndBuf.capacity + nInflateCount
	*/

	/*
		Return : 0 for ok.
				-? for errors.
	*/

	xt_obj_handle_t *pNewBuf = NULL;
	xt_size_t upper_capacity = m_ObjHndBuf.capacity;
	xt_size_t nExpectCapacity = 0;
	
	if ( upper_capacity > _XT_OBJPOOL_CAPACITY_INIT_ )
	{
		upper_capacity -= _XT_OBJPOOL_CAPACITY_INIT_;
		upper_capacity %= _XT_OBJPOOL_COUNT_IN_MEMPAGE_;
		upper_capacity = _XT_OBJPOOL_COUNT_IN_MEMPAGE_ - upper_capacity;
		upper_capacity += m_ObjHndBuf.capacity;
	}
	else /* upper_capacity <= _XT_OBJPOOL_CAPACITY_DEF_ */
	{
		upper_capacity = _XT_OBJPOOL_CAPACITY_INIT_;
	}

	nExpectCapacity = m_ObjHndBuf.capacity + nInflateCount;
	if ( nExpectCapacity<m_ObjHndBuf.capacity || nExpectCapacity<nInflateCount ) /* Overflow */
	{
		return -5;
	}
	
	if ( nExpectCapacity >  upper_capacity ) /* Need re-allocate memory. */
	{
		pNewBuf = AllocHandleArr( nExpectCapacity );
		if ( pNewBuf == NULL )
		{
			return -5;
		}

		if ( m_ObjHndBuf.handle_arr ) /* Old buffer */
		{
			xt_size_t i = 0;
			/* Copy old buffer data to new buffer  */
			for ( i=0; i<m_ObjHndBuf.capacity; ++i )
			{
				pNewBuf[i] = m_ObjHndBuf.handle_arr[i];
			}
			/* Free old buffer. */
			xt_mem_free( m_ObjHndBuf.handle_arr );
		}

		m_ObjHndBuf.handle_arr = pNewBuf;
	}
	/* Make the new nInflateCount NULL. */
	for ( i; i<nExpectCapacity; ++i )
	{
		m_ObjHndBuf.handle_arr[i] = NULL;
	}
	
	m_ObjHndBuf.capacity = nExpectCapacity;

	return FUN_RET_OK;
}


xt_size_t CXtObjPool::InflateRealCapacity(void)
{
	/*
		Assumption:
			to_inflate_realcapacity is true.
		Return : 0  for error.
				inflated object count.
		
		Remarks:
	*/
	xt_size_t i = 0;
	xt_poolchk_node_t *pChunk = NULL;

	/* Calculate grow object count. */
	i = GetGrowRealCapacity();
	if ( i == 0 )
	{
		return 0;
	}

	pChunk = AllocChunk( i );
	if ( pChunk == NULL )
	{
		return 0;
	}
	/*MountChunk( pChunk );*/ /* Append new chunk to end of m_pMemChunkList */
	
	/* handle_arr setting up [m_nRealCapacity..(m_nRealCapacity+pChunk->pool_chunk.count)] */
	for ( i=0; i<pChunk->pool_chunk.count; ++i )
	{
		m_ObjHndBuf.handle_arr[i+m_nRealCapacity] = (pChunk->pool_chunk.buf + m_nObjSize*i);
		ConstructObj( m_ObjHndBuf.handle_arr[i+m_nRealCapacity] );
		InitObj( m_ObjHndBuf.handle_arr[i+m_nRealCapacity] );
	}
	m_nRealCapacity += pChunk->pool_chunk.count; /* m_nRealCapacity increased. */

	return i; /* inflated object count */
}

/* For garbage collection use. */
xt_size_t CXtObjPool::ShrinkRealCapacity(void)
{
	xt_size_t i = 0;
	xt_poolchk_node_t *pChunk = NULL;

	for ( i=0; i<pChunk->pool_chunk.count; ++i )
	{
		m_ObjHndBuf.handle_arr[i+m_nRealCapacity] = (pChunk->pool_chunk.buf + m_nObjSize*i);
		ReleaseObj( m_ObjHndBuf.handle_arr[i+m_nRealCapacity] );
	}

	FreeChunk( pChunk );
}

xt_size_t CXtObjPool::GetGrowRealCapacity(void)
{
	xt_size_t nGrow = m_nRealCapacityGrow;
	xt_size_t nTotalCapacity = m_nRealCapacity + m_nRealCapacityGrow;
	if ( nTotalCapacity<m_nRealCapacity || nTotalCapacity<m_nRealCapacityGrow ) /* Overflow */
	{
		return 0;
	}

	if ( nTotalCapacity > m_ObjHndBuf.capacity ) /* m_ObjHndBuf.capacity should be extended first. */
	{
		nGrow = m_ObjHndBuf.capacity - m_nRealCapacity;
	}

	return nGrow;
}

xt_obj_handle_t* CXtObjPool::AllocHandleArr( xt_size_t nObjCount )
{
	return (xt_obj_handle_t*)xt_mem_alloc( nObjCount*sizeof(xt_obj_handle_t) );
}

void CXtObjPool::FreeHandleArr( xt_obj_handle_t *pArr )
{
	if ( pArr )
	{
		xt_mem_free( pArr );
	}
}

xt_poolchk_node_t* CXtObjPool::AllocChunk( xt_size_t nObjCount )
{
	xt_poolchk_node_t* pChunk = NULL;

#define chunk_obj_mem_size			(m_nObjSize*nObjCount)
	pChunk = (xt_poolchk_node_t*)xt_mem_alloc( chunk_obj_mem_size + sizeof(xt_poolchk_node_t) );
	if ( pChunk == NULL )
	{
		return NULL;
	}

	pChunk->next = NULL;
	pChunk->pool_chunk.count = nObjCount;
	pChunk->pool_chunk.size = chunk_obj_mem_size;
#undef chunk_obj_mem_size

	if ( m_pMemChunkList )
	{
		/* Link new chunk to tail of chunk list */
		xt_poolchk_node_t* pCurChunk = m_pMemChunkList;
		while( pCurChunk->next != NULL )
		{
			pCurChunk = pCurChunk->next;
		}
		pCurChunk->next = pChunk;
		++m_nChunkCount;
	}
	else /*( m_pMemChunkList == NULL )*/
	{
		m_pMemChunkList = pChunk;
		m_nChunkCount = 1;
	}

	return pChunk;
}

void CXtObjPool::FreeChunk( xt_poolchk_node_t *pChunk )
{
	/*
	*	Assumption:
	*		pChunk points to a valid address, 
	*			& pChunk is in m_pMemChunkList currently.
	*		That is m_pMemChunkList!=NULL
	*/

	xt_size_t i = 0;
	void *pObj = NULL;

	if ( m_pMemChunkList == pChunk ) /* Head node should be removed. */
	{
		m_pMemChunkList = pChunk->next;
	}
	else /* Search pCurChunk in m_pMemChunkList. */
	{
		/* Link new chunk to tail of chunk list */
		xt_poolchk_node_t* pCurChunk = m_pMemChunkList;
		while( pCurChunk->next != pChunk )
		{
			pCurChunk = pCurChunk->next;
		}
		/* So far, pChunk == pCurChunk->next */
		pCurChunk->next = pChunk->next;
	}

	pChunk->next = NULL;
	xt_mem_free( pChunk );

	--m_nChunkCount;
}

void CXtObjPool::ConstructObj( void *pObjAddr )
{
	/* new(pObjAddr) obj_class; */
	return FUN_RET_OK;
}

int CXtObjPool::InitObj( void *pObjAddr )
{
	return FUN_RET_OK;
}

// void CXtObjPool::ReleaseObj( void *pObjAddr )
// {
// 	return;
// }

/* handle_arr sort/operation. */
/* Assumption: idx nodes above level are all in order. */
void CXtObjPool::HandleArrAdjust( xt_size_t idx )
{
	/*idx, element index that to be adjusted. */
	xt_size_t lchild = xt_heap_idx_lchild(idx);
	xt_size_t rchild = xt_heap_idx_rchild(idx);
	xt_size_t _idx = 0; /* Largest(smallest) among (idx,lchild and rchild) */
#define _arr				m_ObjHndBuf.handle_arr
#define len					m_ObjHndBuf.count

	while( lchild<len || rchild<len )
	{
		/* 
			Make decision on _idx.
			_idx should be whom among (idx,lchild and rchild).
		*/
		if ( lchild<len && m_ObjHndBuf.fn_cmp(_arr[lchild], _arr[idx])>0 )
		{
			_idx = lchild;
		}
		else
		{
			_idx = idx;
		}
		/* _idx was setup. */
		if ( rchild<len && m_ObjHndBuf.fn_cmp(_arr[rchild], _arr[_idx])>0 )
		{
			_idx = rchild;
		}

		if ( _idx != idx ) /* one of lchild and rchild, should be parent node.  */
		{
			XT_SWAP_PTR( _arr[idx], _arr[_idx] ); /* arr[idx] is new parent. */
			/* Keep adjusting its child(left or right), whose index is _idx */

			/* Prepare for next loop */
			idx = _idx;
			lchild = xt_heap_idx_lchild(idx);
			rchild = xt_heap_idx_rchild(idx);
		}
		else
		{
			break;
		}
	}

#undef _arr
#undef len

	return;
}

int CXtObjPool::HandleArrBuild(void)
{
	xt_size_t i = 0;

#define _arr				m_ObjHndBuf.handle_arr
#define len					m_ObjHndBuf.count

	/* From latest leaf's parent to root node */
	for ( i=(len/2-1); i!=_INVALID_SIZE_T_VALUE_; --i )
	{
		HandleArrAdjust( i );
	}

#undef _arr
#undef len

	return FUN_RET_OK;
}

xt_obj_handle_t* CXtObjPool::HandleArrSearch( xt_size_t idx_start, const xt_obj_handle_t x )
{
	xt_obj_handle_t *ret = NULL;

	xt_size_t _idx = 0;
	xt_size_t lchild = 0;
	xt_size_t rchild = 0;

	int	comp = 0;

#define _arr				m_ObjHndBuf.handle_arr
#define len					m_ObjHndBuf.count

	if ( len <= idx_start )
	{
		return NULL;
	}

	_idx = idx_start;
	while( _idx < len )
	{
		comp = m_ObjHndBuf.fn_cmp( x, _arr[_idx] );
		if ( comp == 0 ) /* Found */
		{
			ret = &_arr[_idx];
			break;
		}

		if ( comp > 0 ) /* Go upper. */
		{
			/* Do NOT have to move down, because the lower elements, in this sub-tree, must not be bigger than [*px] */
			goto lbl_go_upper;
		}

		/* ( comp < 0 ) from here. */
		lchild = xt_heap_idx_lchild(_idx);
		if ( lchild < len ) /* Keep moving down. */
		{
			/* search among LEFT-children. */
			_idx = lchild;
			continue;
		}

		/* lchild >= len */
		/* NO more children */
		/* Go upper. */
lbl_go_upper:
		while( xt_heap_is_rchild(_idx) && _idx!=idx_start )
		{
			_idx = xt_heap_idx_parent(_idx); /* Go upper. */
		}

		if ( _idx == idx_start ) /* NOT found. */
		{
			break;
			/*return NULL;*/
		}

		_idx = xt_heap_idx_parent(_idx); /* Go upper. */

		rchild = xt_heap_idx_rchild(_idx);

		if ( rchild < len ) /* Keep moving down. */
		{
			/* search among RIGHT-children. */
			_idx = rchild;
			continue;
		}
		goto lbl_go_upper;
	}

#undef _arr
#undef len

	return ret;
}

int CXtObjPool::HandleArrPop( xt_obj_handle_t *px ) /* Popup element on top of this heap ( the mini one ) */
{
#define _arr				m_ObjHndBuf.handle_arr
#define len					m_ObjHndBuf.count
	if ( len == 0 )
	{
		return -1;
	}

	XT_SWAP_PTR( _arr[0], _arr[len-1] );
	--len;

	if ( px )
	{
		*px = _arr[len];
	}

	HandleArrAdjust( 0 );

#undef _arr
#undef len

	return FUN_RET_OK;
}

int CXtObjPool::HandleArrPush(void)
{
	if ( m_ObjHndBuf.count == m_ObjHndBuf.capacity )
	{
		return -1; /* Full */
	}

	/*m_ObjHndBuf.handle_arr[m_ObjHndBuf.count] = x;*/
	++m_ObjHndBuf.count;

	HandleArrBuild();

	return FUN_RET_OK;
}

int CXtObjPool::HandleArrErase( xt_obj_handle_t x )
{
	xt_obj_handle_t *_px = HandleArrSearch( 0, x );
	return HandleArrErase_( _px );
}

int CXtObjPool::HandleArrErase_( xt_obj_handle_t *px )
{
	xt_size_t idx = 0;

#define _arr				m_ObjHndBuf.handle_arr
#define len					m_ObjHndBuf.count

	XT_SWAP_PTR( *px, _arr[len-1] );
	--len;

	idx = px - _arr;
	HandleArrAdjust( idx );

	return FUN_RET_OK;

#undef _arr
#undef len
}
