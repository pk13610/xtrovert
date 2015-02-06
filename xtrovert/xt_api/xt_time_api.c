/************************************************************************************
*	xt_time_api.c: Implementation File
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
#include "xt_time_api.h"

#if defined(__WINDOWS__)
/* warning C4100: 'XXX' : unreferenced formal parameter */
#pragma warning (disable: 4100)
#endif


/*errno_t localtime_s( struct tm* _tm, const time_t *time );*/
/*struct tm *localtime_r(const time_t *timep, struct tm *result);*/

int xt_localtime( const time_t *timep, struct tm *result )
{
#if defined(__WINDOWS__)
	errno_t err = localtime_s( result, timep );
	if ( err != 0 )
	{
#elif defined(__LINUX__)	
	struct tm *err = localtime_r( timep, result );
	if ( err == NULL )
	{
#else
	{
		return -2;
#endif
		return -1;
	}

	return 0;
}

int xt_sleep( UINT seconds )				/* In Seconds */
{
#if defined (__LINUX__)
	return sleep( seconds );
#elif defined (__WINDOWS__)
	assert(seconds);
	//sleep( seconds );
	return FUN_RET_OK;
#else
	return -3;
#endif
}

int xt_msleep( UINT milli_seconds )		/* In Milli-Seconds */
{
#if defined (__LINUX__)
	return usleep( milli_seconds*1000 );
#elif defined (__WINDOWS__)
	Sleep( milli_seconds );
	return FUN_RET_OK;
#else
	return -3;
#endif

}

// return string: YYYY-MM-DD HH:MM:SS
const TCHAR* get_time_str( time_t t, TCHAR *szStr )
{
	struct tm _tm;
	static TCHAR stszStr[MAX_PATH] = {0};
	if ( !szStr )
	{
		szStr = stszStr;
	}

	if ( xt_localtime(&t, &_tm) < 0 )
	{
		szStr[0] = 0;
		return szStr;
	}

	_sntprintf( szStr, MAX_PATH, _T("%04d-%02d-%02d %02d:%02d:%02d")
		, 1900+_tm.tm_year, 1+_tm.tm_mon, _tm.tm_mday
		, _tm.tm_hour, _tm.tm_min, _tm.tm_sec);

	return szStr;
}

void xt_reset_ticker( xt_ticker_t *ticker )
{
#if defined(__WINDOWS__)
	*ticker = clock();
#elif defined(__LINUX__)
	return;
#endif
	return;
}

xt_ticker_t xt_test_ticker( xt_ticker_t *ticker )
{
#if defined(__WINDOWS__)
	return (clock()-(*ticker));
#elif defined(__LINUX__)
	return 0;
#else
	return 0;
#endif
}

#if defined (__UNIT_TEST__)
/*
<UnitTest Compile Command:>
<Linux="g++ -g -D__LINUX__ -D__UNIT_TEST__ -I./ time_ex.cpp -o time_ex">
<Windows="cl /Zi /D__WINDOWS__ /D__UNIT_TEST__ /I./ time_ex.cpp time_ex">
*/
#include <stdio.h>
int main()
{
	time_t _now;
	tm _newtime;
	int nRet = 0;

	time( &_now );

	nRet = xt_localtime( &_now, &_newtime );
	if ( nRet < 0 )
	{
		printf( "Error:%d, Ret:%d\n", errno, nRet );
	}
	else
	{
		printf( "Time:%04d-%02d-%02d\n", _newtime.tm_year+1900, _newtime.tm_mon+1, _newtime.tm_mday );
	}

	return 0;
}

#endif
