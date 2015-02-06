/************************************************************************************
*	xafx_app.cpp: Implementation File
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

#include "app/xafx/xafx_app.h"

CXtApp* XT_STDECL xafx_get_app(void)
{
	/*return xt_app_module.pApp;*/
	return CXtApp::m_pThisApp;
}

int XT_STDECL xafx_app_init( int argc, TCHAR *argv[] )
{
	int ret = FUN_RET_OK;

	CXtApp *pTheApp = xafx_get_app();
	if ( pTheApp == NULL )
	{
		return -5;
	}

	ret = xafx_init_thread( thread_id );

	return ret;
}

int XT_STDECL xafx_app_exit(void)
{

	xafx_term_thread();
	return FUN_RET_OK;
}

int XT_STDECL xafx_init_thread(void)
{
	pthread_t thread_id = xt_thread_self();
	xafx_thread_module_t *thread_module =  xafx_thread_msgque_setup(thread_id);

	return FUN_RET_OK;
}

int XT_STDECL xafx_term_thread(void)
{
	return FUN_RET_OK;
}
