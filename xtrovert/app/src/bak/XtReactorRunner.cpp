/************************************************************************************
*	XtReactorRunner.cpp: Implementation File
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
#include "XtReactorRunner.h"

#if defined(__WINDOWS__)
/* warning C4100: 'XXX' : unreferenced formal parameter */
#pragma warning (disable: 4100)
#endif

//////////////////////////////////////////////////////////////////////////
// CLASS CXtReactorRunner
//////////////////////////////////////////////////////////////////////////
CXtReactorRunner::CXtReactorRunner(void)
{

}

CXtReactorRunner::~CXtReactorRunner(void)
{

}

int CXtReactorRunner::Setup(void)
{
	int nRet = 0;
	nRet = CXtThread::Setup();

#if defined(_DEBUG)
	SetLabel( _T("XtReacRunner") );
#endif

	return nRet;
}
