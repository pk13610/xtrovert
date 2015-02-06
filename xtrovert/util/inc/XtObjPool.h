/************************************************************************************
* XtObjPool.h : header file
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
#ifndef __XT_OBJ_POOL_H__
#define __XT_OBJ_POOL_H__

#if defined(__WINDOWS__)
#pragma warning (disable: 4200) /* Ignore:  warning C4200: nonstandard extension used : zero-sized array in struct/union */
#endif

#include "xt_define.h"
#include <new> /* Placement new */

/* Object_Pool Memory. [Pool-Chunk] */
typedef struct _tag_xt_pool_chunk_t
{
	xt_size_t				size;
	xt_size_t				count;
	char					buf[0];
}xt_pool_chunk_t;

typedef struct _tag_xt_pool_chunk_node_t
{
	struct _tag_xt_pool_chunk_node_t	*next;
	xt_pool_chunk_t						pool_chunk;
}xt_poolchk_node_t;

/* Object_Pool Memory. [Pool-Chunk] END */

/* Object Handle Array buffer. */
typedef void*			xt_obj_handle_t;

typedef int (*xt_fn_objpool_cmp_t)( const xt_obj_handle_t objHandle1, const xt_obj_handle_t objHandle2 );

/*static int	xt_obj_pool_comp( const xt_obj_handle_t objHandle1, const xt_obj_handle_t objHandle2 );*/

typedef struct _tag_xt_obj_handle_buf_t
{
	xt_size_t				capacity;
	xt_size_t				count;
	xt_fn_objpool_cmp_t		fn_cmp;
	xt_obj_handle_t			*handle_arr;
}xt_obj_handle_buf_t;
/* Object Handle Array buffer END */

/* Pool commit an ID for a specified obj in pool */
typedef xt_size_t xt_obj_id_t;
#define _INVALID_OBJ_ID_				((xt_obj_id_t)(~0))

/* return object pointer by object handle */
/*#define xt_from_obj_handle(obj_handle)					(*(obj_handle))*/

#define DIRECT_BIND_BOJECT(obj_class)						\
	virtual void SetupObjSize(void)							\
	{														\
		m_nObjSize = sizeof(obj_class);						\
	}														\
	virtual void ConstructObj( void *pObjAddr )				\
	{														\
		new(pObjAddr) obj_class;							\
	}


// const xt_size_t _XT_OBJPOOL_CAPACITY_INIT_; /* = ((_MEM_PAGE_SIZE_DEFAULT_-sizeof(xt_obj_handle_buf_t))/sizeof(void*)); */
// const xt_size_t _XT_OBJPOOL_COUNT_IN_MEMPAGE_;

class CXtObjPool
{
public:
	CXtObjPool(void);
	CXtObjPool( xt_size_t nObjSize );
	virtual ~CXtObjPool(void);

	/* Create object pool with capacity of nObjCount elements */
	int							Create( xt_size_t nObjCount );
	int							InflatePool( xt_size_t nInflateCount );
	void						Destroy(void);

	/*virtual xt_size_t			GarbageCollect(void);*/ /* Return value: memory-size returned to system */

	void						Clear(void);

	void*						AllocObj( xt_obj_id_t *pObjId );
	int							ActivateObj(void);

	int							FreeObj( void *pObj );

	void*						GetAt( xt_size_t nIndex );

	void*						SearchObj( const void *pKeyObj );

	xt_size_t					Capacity(void)											{ return m_ObjHndBuf.capacity; }
	xt_size_t					Count(void)												{ return m_ObjHndBuf.count; }

	BOOL						IsFull(void)											{ return (m_ObjHndBuf.count==m_ObjHndBuf.capacity); }
	BOOL						IsEmpty(void)											{ return (m_ObjHndBuf.count==0); }

/*****************************************************************************************/
protected:

#define is_pool_valid			((m_ObjHndBuf.handle_arr) && (m_ObjHndBuf.fn_cmp))
#define is_pool_elem_valid		(m_nObjSize!=0 && m_nObjSize!=_INVALID_SIZE_T_VALUE_)

	void						Reset(void);

	virtual int					OnInitPool(void);
	virtual void				OnReleasePool(void);

	virtual void				SetupObjSize(void)					{ return; }

	virtual void				SetupRealCapacityGrow(void);

	xt_obj_handle_t*			AllocActiveObj(void);

	int							InflateCapacity( xt_size_t nInflateCount );

	xt_size_t					InflateRealCapacity(void); /* inflate by m_nRealCapacityGrow */
	/*xt_size_t					ShrinkRealCapacity(void);*/

	xt_size_t					GetGrowRealCapacity(void);
	
	virtual void				ConstructObj( void *pObjAddr );
	virtual int					InitObj( void *pObjAddr );
	virtual void				ResetObj( void *pObjAddr )					{ return; } /* Called in FreeObj(). */
	/*virtual void				ReleaseObj( void *pObjAddr );*/

	xt_obj_handle_buf_t			m_ObjHndBuf;
	xt_obj_handle_t				*m_phndLateObj; /* Handle pointer to Latest accessed object. */
	xt_obj_handle_t				*m_phndActiveObj;
	
	xt_size_t					m_nObjSize;
	xt_size_t					m_nRealCapacityGrow; /* m_nRealCapacity + m_nRealCapacityGrow at one time. */

#define grow_capacity_def		_ELEMENT_COUNT_DEFAULT_		/*m_nObjGrow*/

#define is_pool_full			(m_ObjHndBuf.count==m_ObjHndBuf.capacity)
#define is_pool_empty			(m_ObjHndBuf.count==0)

/*****************************************************************************************/
private:
	/* Handle Array Operation. */	
	xt_obj_handle_t*			AllocHandleArr( xt_size_t nObjCount );
	void						FreeHandleArr( xt_obj_handle_t *pArr );
	/* Assumption: idx nodes above level are all in order. */
	void						HandleArrAdjust( xt_size_t idx ); /*xt_heap_adjust*/
	int							HandleArrBuild(void);
	xt_obj_handle_t*			HandleArrSearch( xt_size_t idx_start, const xt_obj_handle_t x );
	
	int							HandleArrPush(void);
	int							HandleArrErase( xt_obj_handle_t x );
	int							HandleArrErase_( xt_obj_handle_t *px );
	int							HandleArrPop( xt_obj_handle_t *px ); /* Popup element on top of this heap ( the mini one ) */

	xt_poolchk_node_t*			AllocChunk( xt_size_t nObjCount );
	void						FreeChunk( xt_poolchk_node_t *pChunk );

	/* Object data storage. Chunk Data */
	xt_size_t					m_nChunkCount;
	xt_poolchk_node_t			*m_pMemChunkList;

	/* m_nRealCapacity <= m_ObjHndBuf.capacity */
	xt_size_t					m_nRealCapacity; /* if (m_ObjHndBuf.count==m_nRealCapacity) { AllocChunk(m_nGrowCapacity); } */

	#define to_inflate_realcapacity		(m_ObjHndBuf.count==m_nRealCapacity)
/*****************************************************************************************/
};

#endif /*__xt_OBJ_POOL_H__*/
