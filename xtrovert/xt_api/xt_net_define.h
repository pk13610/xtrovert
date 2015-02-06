/************************************************************************************
* NetDefine.h : header file
*
* NetDefine header
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

#ifndef __XT_NET_DEFINE_H__
#define __XT_NET_DEFINE_H__

#include "xt_define.h"

#ifdef __cplusplus
extern "C" {
#endif

/************************************************************************/
/*      Include Files                                                   */
/************************************************************************/
#if defined(__WINDOWS__)

//#include <WinSock2.h>

#include <ws2ipdef.h> /* INET_ADDRSTRLEN,INET6_ADDRSTRLEN */
/* #include <Ws2tcpip.h> */
#pragma comment(lib,"Ws2_32.lib")

#elif defined (__LINUX__)
#include <stddef.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/un.h> 

/*#include <sys/stat.h>*/
/*#include <fcntl.h> */

#ifndef	HAVE_ADDRINFO_STRUCT
/*# include	"../lib/addrinfo.h" */ /* From UNP ??? */
#endif

#endif /*__LINUX__*/

/************************************************************************/
/*      Type Define		                                                */
/************************************************************************/
#if defined (__WINDOWS__)

typedef SOCKET	socket_fd_t;
typedef INT SOCKLEN_T;

/*typedef USHORT		sa_family_t;*/
/*typedef struct sockaddr SOCKADDR;*/
/*typedef struct sockaddr_in SOCKADDR_IN;*/

/*INT WSAAPI InetPton(_In_   INT  Family,	_In_   PCTSTR pszAddrString,	_Out_  PVOID pAddrBuf);*/
/*PCTSTR WSAAPI InetNtop(	_In_   INT  Family,	_In_   PVOID pAddr,	_Out_  PTSTR pStringBuf, _In_   xt_size_t StringBufSize );*/
#define _inet_pton		InetPton
#define _inet_ntop		InetNtop

#define		INET_ADDRSTRLEN_MAX		INET6_ADDRSTRLEN

#define		UNIX_PATH_MAX_			0

#elif defined(__LINUX__)

typedef INT	socket_fd_t;
typedef socklen_t SOCKLEN_T;

typedef struct in_addr			IN_ADDR;
typedef struct in6_addr			IN6_ADDR;

typedef struct sockaddr			SOCKADDR;
typedef struct sockaddr_in		SOCKADDR_IN;
typedef struct sockaddr_in6		SOCKADDR_IN6;
typedef struct sockaddr_un		SOCKADDR_UN;

#define     INVALID_SOCKET	((socket_fd_t)(~0))
#define		SOCKET_ERROR	(-1)

//#define		UNIX_PATH_MAX_		((INT)(sizeof(SOCKADDR_UN)-sizeof(USHORT)))
#define		INET_ADDRSTRLEN_MAX		UNIX_PATH_MAX

#define		closesocket		close

/*INT WSAAPI InetPton(_In_   INT  Family,	_In_   PCTSTR pszAddrString,	_Out_  PVOID pAddrBuf);*/
/*PCTSTR WSAAPI InetNtop(	_In_   INT  Family,	_In_   PVOID pAddr,	_Out_  PTSTR pStringBuf, _In_   xt_size_t StringBufSize );*/
#define _inet_pton		inet_pton
#define _inet_ntop		inet_ntop

typedef struct sockaddr_storage SOCKADDR_STORAGE;

typedef union _SOCKADDR_INET
{
	SOCKADDR_IN Ipv4;
	SOCKADDR_IN6 Ipv6;
	USHORT si_family;
} SOCKADDR_INET, *PSOCKADDR_INET;

#endif /*__LINUX__*/

#define 	NS_INADDRSZ		4
#define 	NS_IN6ADDRSZ	16
#define 	NS_INT16SZ		2

#define		INET_PORT_STRLEN_MAX	6

/* Different ways a socket may be shutdown. */
enum SHUTDOWN_TYPE
{
#ifdef __LINUX__
	SHUTDOWN_RECV = SHUT_RD,
	SHUTDOWN_SEND = SHUT_WR,
	SHUTDOWN_BOTH = SHUT_RDWR,
#elif defined __WINDOWS__
	SHUTDOWN_RECV = 0x00, /*SD_RECEIVE,*/
	SHUTDOWN_SEND = 0x01, /*SD_SEND,*/
	SHUTDOWN_BOTH = 0x02, /*SD_BOTH	,*/
#endif

	SHUTDOWN_TYPE_END
};


enum OS_PLATFORM_TYPE
{
	PLATFORM_MS_WIN = 0,
	PLATFORM_LINUX,

	PLATFORM_TYPE_CNT
};

enum NET_SKT_TYPE /* Net Socket Types */
{
	NET_SKT_TYPE_INVALID = 0,
	NET_SKT_TYPE_TCP = SOCK_STREAM,
	NET_SKT_TYPE_UDP = SOCK_DGRAM,
	NET_SKT_TYPE_RAW = SOCK_RAW,

	NET_SKT_TYPE_CNT
};

/* DEFAULT VALUE */
#define NET_SKT_TYPE_DEF		NET_SKT_TYPE_TCP
#define NET_PRO_FAMILY_DEF		AF_INET /* AF_INET AF_INET6 AF_UNIX/AF_LOCAL */

/************************************************************************/

/************************************************************************/
#define ERR_SOCK_AF_NOT_SURPPORT		-3			/* AF_XXXX NOT support */
#define ERR_SOCK_FD_INIT_FAILED			-4			/* Failed to create socket */
#define ERR_SOCK_FD_INVALID				-5			/* Socket descriptor equals to INVALID_SOCKET */
#define ERR_SOCK_FD_LISTEN_FAILED		-6			/* Failed to listen on socket id */
#define ERR_SOCK_FD_ACCEPT_FAILED		-7			/* Failed to Accept to peer host */
#define ERR_SOCK_CLIENT_FULL			-8			/* No more room to accept new connection */
#define ERR_SOCK_CLIENT_EMPTY			-9			/* No available connection */
#define ERR_SOCK_FD_CONNECT_FAILED		-10			/* Failed to connect to peer host */
#define ERR_SOCK_ADDR_NULL				-11			/* SOCKADDR is NULL */
#define ERR_SOCK_PEER_CLOSED			-12			/* PEER SOCKET CLOSED, recv == 0 */

typedef struct _tagNET_HOST_ADDR_STR
{
	USHORT			sa_family; /* AF_INET AF_INET6 AF_UNIX */
	TCHAR			szAddNative[INET_ADDRSTRLEN_MAX];	/*IP String like 127.0.0.1:7788*/
}NET_HOST_ADDR_STR;

/*listener, client, remote*/

typedef struct _tagNET_HOST_ADDR_PAIR
{
	enum NET_SKT_TYPE	nSockType;
	NET_HOST_ADDR_STR	addrLocal;
	NET_HOST_ADDR_STR	addrPeer;
}NET_HOST_ADDR_PAIR;

#define _XT_SOCKADDR_IN_SIZE_				sizeof(SOCKADDR_IN)
#define _XT_SOCKADDR_IN6_SIZE_				sizeof(SOCKADDR_IN6)
#define _XT_SOCKADDR_UNIX_SIZE_				UNIX_PATH_MAX_

#define _xt_SOCKADDR_SIZE_(af_domain, out_size)				\
	do {														\
		switch(af_domain)										\
		{														\
		case AF_INET:	(out_size)=sizeof(SOCKADDR_IN); break;	\
		case AF_INET6:	(out_size)=sizeof(SOCKADDR_IN6); break;	\
		case AF_UNIX:	(out_size)=UNIX_PATH_MAX_; break;		\
		default: (out_size) = 0; break;							\
		}														\
	} while(__LINE__==-1)

#define		_IS_VALID_SOCKET_(sock_fd)						(sock_fd != INVALID_SOCKET)

#ifdef __cplusplus
}
#endif

#endif /*__XT_NET_DEFINE_H__*/
