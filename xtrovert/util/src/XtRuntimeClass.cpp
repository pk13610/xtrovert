/************************************************************************************
*	XtRuntimeClass.cpp: Implementation File
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

#include "XtRuntimeClass.h"

static TCHAR szCObject[] = "classCXtObject";
struct CXtRuntimeClass CXtObject::classCXtObject =
{ szCObject, sizeof(CXtObject), 0xffff, NULL, NULL };
static XT_CLASSINIT _init_CXtObject(&CXtObject::classCXtObject);

CXtRuntimeClass *CXtRuntimeClass::pFirstClass = NULL;

VOID XtClassInit(CXtRuntimeClass *pNewClass)
{
	pNewClass->m_pNextClass = CXtRuntimeClass::pFirstClass;
	CXtRuntimeClass::pFirstClass = pNewClass;
// 	AFX_MODULE_STATE* pModuleState = AfxGetModuleState();
// 	AfxLockGlobals(CRIT_RUNTIMECLASSLIST);
// 	pModuleState->m_classList.AddHead(pNewClass);
// 	AfxUnlockGlobals(CRIT_RUNTIMECLASSLIST);
}

/*****************************************
 *			CXtRuntimeClass Class
 *****************************************/
CXtObject* CXtRuntimeClass::CreateObject(VOID)
{
	if (m_pfnCreateObject == NULL)
	{
// 		TRACE1("Error: Trying to create object which is not "
// 			"DECLARE_DYNCREATE \nor DECLARE_SERIAL: %hs.\n",
// 			m_lpszClassName);
		return NULL;
	}

	CXtObject* pObject = NULL;
	pObject = (*m_pfnCreateObject)();

	return pObject;
}

CXtObject* CXtRuntimeClass::CreateObjectInplace( VOID *pInplace )
{
	if (m_pfnCreateObjectInplace == NULL)
	{
		return NULL;
	}

	CXtObject* pObject = NULL;
	pObject = (*m_pfnCreateObjectInplace)(pInplace);

	return pObject;
}

CXtRuntimeClass* CXtRuntimeClass::Load()
{
	TCHAR szClassName[64] = {0};
	CXtRuntimeClass* pClass = NULL;

	// instead of Load from file, we Load from cin.
	for (pClass = pFirstClass; pClass != NULL; pClass = pClass->m_pNextClass)
	{
		if (_tcscmp(szClassName, pClass->m_lpszClassName) == 0)
			return pClass;
	}

	//TRACE1("Error: Class not found: %s \n", szClassName);
	return NULL; // not found
}

/*****************************************
 *			CXtObject Class
 *****************************************/
CXtRuntimeClass* CXtObject::GetRuntimeClass(void) const
{
//	return &CExtObject::classCExtObject;
	return _XT_RUNTIME_CLASS(CXtObject);
}

// #ifdef _AFXDLL
// CExtRuntimeClass* CExtObject::_GetBaseClass()
// {
// 	return NULL;
// }
// CExtRuntimeClass* CExtObject::GetThisClass()
// {
// 	return _Ext_RUNTIME_CLASS(CExtObject);
// }
// #endif

BOOL CXtObject::IsKindOf( const CXtRuntimeClass* pClass) const
{
	CXtRuntimeClass* pClassThis = GetRuntimeClass();
	while (pClassThis != NULL)
	{
		if (pClassThis == pClass)
			return TRUE;
		pClassThis = pClassThis->m_pBaseClass;
	}
	return FALSE;       // walked to the top, no match
}
