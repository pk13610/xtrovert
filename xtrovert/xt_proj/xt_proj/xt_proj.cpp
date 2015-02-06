// xt_proj.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "_test_app.h"


#if defined(__WINDOWS__)
/* warning C4100: 'XXX' : unreferenced formal parameter */
#pragma warning (disable: 4100)
//#pragma warning (disable: 4389)
#endif


extern int _test_net_main( int argc, _TCHAR* argv[] );
extern int _test_app_main( int argc, _TCHAR* argv[] );

int _tmain( int argc, _TCHAR* argv[] )
{
	_test_app_main( argc, argv );

	getchar();

	return FUN_RET_OK;
}
