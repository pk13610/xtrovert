/************************************************************************************
* XtRuntimeClass.h : header file
*
* Dynamic Reation of Runtime Class Definition header
*
* AUTHOR	:	Sean Feng <SeanFeng2006@hotmail.com>
* DATE		:	Aug. 7, 2012
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

#ifndef __XT_RUNTIMECLASS_H__
#define __XT_RUNTIMECLASS_H__

#include "xt_define.h"

class CXtObject;

struct CXtRuntimeClass
{
	// Attributes
	CONST TCHAR *m_lpszClassName;
	INT m_nObjectSize;
	UINT m_wSchema; // schema number of the loaded class
	CXtObject* (*m_pfnCreateObject)(); // NULL => abstract class
	CXtObject* (*m_pfnCreateObjectInplace)(VOID*); // NULL => abstract class
	CXtRuntimeClass* m_pBaseClass;
 
	CXtObject* CreateObject(VOID);
	CXtObject* CreateObjectInplace( VOID *pInpalce );
	static CXtRuntimeClass* Load();

	//  CExtRuntimeClass objects linked together in simple list
	static CXtRuntimeClass *pFirstClass; // start of class list
	CXtRuntimeClass *m_pNextClass;    // linked list of registered classes
};

// generate static object constructor for class registration
VOID XtClassInit(CXtRuntimeClass *pNewClass);
struct XT_CLASSINIT
{ XT_CLASSINIT(CXtRuntimeClass *pNewClass) { XtClassInit(pNewClass); } };

#define _XT_RUNTIME_CLASS(class_name) ((CXtRuntimeClass*)(&class_name::class##class_name))
// #ifdef _AFXDLL
// #define EXT_RUNTIME_CLASS(class_name) (class_name::GetThisClass())
// #else
// #define EXT_RUNTIME_CLASS(class_name) _EXT_RUNTIME_CLASS(class_name)
// #endif

#define XT_RUNTIME_CLASS(class_name) _XT_RUNTIME_CLASS(class_name)

#define XT_DECLARE_DYNAMIC(class_name) \
public: \
		static CXtRuntimeClass class##class_name; \
		virtual CXtRuntimeClass* GetRuntimeClass() CONST;

#define XT_DECLARE_DYNCREATE(class_name) \
		XT_DECLARE_DYNAMIC(class_name) \
		static CXtObject* CreateObject();		\
		static CXtObject* CreateObjectInplace( VOID *pInpalce );

#define _XT_IMPLEMENT_RUNTIMECLASS(class_name, base_class_name, wSchema, pfnNew, pfnNewInplace) \
		static TCHAR _lpsz##class_name[] = #class_name; \
		CXtRuntimeClass class_name::class##class_name = { \
			_lpsz##class_name, sizeof(class_name), wSchema, pfnNew, pfnNewInplace, \
				XT_RUNTIME_CLASS(base_class_name), NULL }; \
		static XT_CLASSINIT _init_##class_name(&class_name::class##class_name); \
		CXtRuntimeClass* class_name::GetRuntimeClass() CONST \
			{ return &class_name::class##class_name; } \

#define XT_IMPLEMENT_DYNAMIC(class_name, base_class_name) \
		_XT_IMPLEMENT_RUNTIMECLASS(class_name, base_class_name, 0xFFFF, NULL, NULL)

#define XT_IMPLEMENT_DYNCREATE(class_name, base_class_name) \
		CXtObject* class_name::CreateObject() \
				{ return new class_name; } \
		CXtObject* class_name::CreateObjectInplace( VOID *pInpalce ) \
				{ return new (pInpalce)class_name; } \
		_XT_IMPLEMENT_RUNTIMECLASS(class_name, base_class_name, 0xFFFF, \
				class_name::CreateObject, class_name::CreateObjectInplace)

class CXtObject
{
public:
	CXtObject::CXtObject()
	{
	}
	virtual CXtObject::~CXtObject()
	{
	}

// 	// Diagnostic allocations
// 	VOID* operator new(size_t nSize);
// 	VOID* operator new(size_t, VOID* p);
// 	VOID operator delete(VOID* p);
// 	VOID operator delete(VOID* p, VOID* pPlace);

	virtual CXtRuntimeClass* GetRuntimeClass(void) const;
	BOOL IsKindOf( const CXtRuntimeClass* pClass ) const;

// #if defined(_DEBUG) && !defined(_AFX_NO_DEBUG_CRT)
// 	// for file name/line number tracking using DEBUG_NEW
// 	void* PASCAL operator new(size_t nSize, LPCSTR lpszFileName, int nLine);
// 	void PASCAL operator delete(void *p, LPCSTR lpszFileName, int nLine);
// #endif

public:
	static CXtRuntimeClass classCXtObject;
// #ifdef _AFXDLL
// 	static CExtRuntimeClass* _GetBaseClass();
// 	static CExtRuntimeClass* GetThisClass();
// #endif
};

#endif /*__XT_RUNTIMECLASS_H__*/
