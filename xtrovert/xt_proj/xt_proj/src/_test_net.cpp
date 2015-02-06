/************************************************************************************
* _test_net.cpp: Implementation File
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

#include "_test_net.h"
#include "xt_time_api.h"

#define _MAX_CONNECTIONS_			(100)
void StartNetServer( NET_SKT_TYPE skt_type, const TCHAR *szLocalAddr )
{
  	int nRet = FUN_RET_OK;
  
  	CXtReactor		reactor;
// 	CXtNetAcceptor	net_svr;
// 	SOCKADDR_IN		saddr;
// 	CXtNetConnPool	pool;
 	CXtEvtHndlrSig	sig_int_handler;
// 
// 	xt_net_acceptor_cfg_t cfg;
// 
 	_tprintf( _T("StartNetServer!\n") );
// 
// 	switch ( skt_type )
// 	{
// 	case SOCK_STREAM:
// 		cfg.base_cfg.net_cfg_flag = XT_FLAG_CREATE_TCP;
// 		break;
// 	case SOCK_DGRAM:
// 		cfg.base_cfg.net_cfg_flag = XT_FLAG_CREATE_UDP;
// 		break;
// 	default:
// 		break;
// 	}
// 	cfg.base_cfg.sock_addr.saddr = (SOCKADDR*)&saddr;
// 	cfg.base_cfg.sock_addr.addrlen = sizeof(SOCKADDR_IN);
// 	cfg.conn_pool = &pool;
// 
// 	fill_sock_addr_struct( cfg.base_cfg.sock_addr.saddr, &(cfg.base_cfg.sock_addr.addrlen), szLocalAddr, AF_INET );
// 
// 	pool.SetupConnectorInfo( AF_INET, skt_type );
// 	pool.Init( _MAX_CONNECTIONS_ );
// 
// 	net_svr.Init( &cfg );
// 
 	nRet = reactor.Init();
// 
// 	sig_int_handler.Init( (void*)SIGINT );
// 
// 	reactor.RegisterHandler( &sig_int_handler );
// 	reactor.RegisterHandler( &net_svr );

	sig_int_handler.Register( &reactor );

	reactor.Dispatch();

	sig_int_handler.Release();
// 	net_svr.Release();
// 
// 	pool.Release();

	reactor.Release();

	return;
}

void StartNetClient( NET_SKT_TYPE skt_type, const TCHAR *szPeerAddr )
{
// 	int nRet = FUN_RET_OK;
// 
// 	CXtReactor		reactor;
// 	CXtNetConnector	net_client;
// 	SOCKADDR_IN		saddr;
// 
// 	CXtEvtHndlrSig	sig_int_handler;
// 
// 	xt_net_conn_cfg_t cfg;
// 
// 	_tprintf( _T("StartNetClient!\n") );
// 
// 	switch ( skt_type )
// 	{
// 	case SOCK_STREAM:
// 		cfg.base_cfg.net_cfg_flag = XT_FLAG_CREATE_TCP;
// 		break;
// 	case SOCK_DGRAM:
// 		cfg.base_cfg.net_cfg_flag = XT_FLAG_CREATE_UDP;
// 		break;
// 	default:
// 		break;
// 	}
// 
// 	cfg.acceptor = NULL;
// 	cfg.base_cfg.sock_addr.saddr = (SOCKADDR*)&saddr;
// 	cfg.base_cfg.sock_addr.addrlen = sizeof(SOCKADDR_IN);
// 
// 	fill_sock_addr_struct( cfg.base_cfg.sock_addr.saddr, &(cfg.base_cfg.sock_addr.addrlen), szPeerAddr, AF_INET );
// 
// 	net_client.Init( &cfg );
// 
// 	nRet = reactor.Init( _MAX_EVENTS_ );	
// 
// 	sig_int_handler.Init( (void*)SIGINT );
// 
// 	reactor.RegisterHandler( &sig_int_handler );
// 	reactor.RegisterHandler( &net_client );
// 
// 	reactor.Dispatch();
// 
// 	sig_int_handler.Release();
// 	net_client.Release();
// 
// 	reactor.Release();

	return;
}

int _test_net_main( int argc, _TCHAR* argv[] )
{
	__CTestNetThread test_thread;

	int ret = FUN_RET_OK;
	ret = xt_prepare_win_sock();
	if ( ret < 0 )
	{
		_tprintf( _T("xt_prepare_win_sock failed!\n") );
		return 0;
	}
	/* Starting Network thread. */
	TCHAR _param = argv[1][0];
	test_thread.Start( &_param );

	xt_msleep( 1000 );

	test_thread.BlockWait();

	test_thread.Release();

	getchar();

	xt_clean_win_sock();

	return 0;
}
