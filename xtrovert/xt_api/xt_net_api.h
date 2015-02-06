/************************************************************************************
* xt_net_api.h : header file
*
* Socket API Definition header
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
#ifndef __XT_NET_API_H__
#define __XT_NET_API_H__

#include "xt_net_define.h"

#ifdef __cplusplus
extern "C" {
#endif

/***********************************************************************
* Platform Independent Socket API Collection (exception based)
***********************************************************************/

int			PrepareWinSock(void);
int			CleanWinSock(void);

#if defined(__WINDOWS__)
#define xt_prepare_win_sock()					PrepareWinSock()
#define xt_clean_win_sock()						CleanWinSock()
#else
#define xt_prepare_win_sock()					0
#define xt_clean_win_sock()						0
#endif

/************************************************************************/
socket_fd_t	xt_socket( int family, int type, int proto );

/* For TCP/IP, if the port is specified as zero, 
*  the service provider assigns a unique port to the application with a value between 1024 and 5000 */
int			xt_bind( socket_fd_t nSockfd, const SOCKADDR *pAddr, SOCKLEN_T nAddrLen );

int			xt_connect( socket_fd_t nSockfd, const SOCKADDR *pAddr, SOCKLEN_T nAddrLen );

int			xt_getsockname( socket_fd_t nSockfd, SOCKADDR *pAddr, SOCKLEN_T *pAddrLen );
int			xt_getpeername( socket_fd_t nSockfd, SOCKADDR *pAddr, SOCKLEN_T *pAddrLen );

int			xt_get_sockname_str( socket_fd_t nSockfd, TCHAR *addr, int len );
int			xt_get_peername_str( socket_fd_t nSockfd, TCHAR *addr, int len );

int			xt_listen( socket_fd_t nSockfd, int nBacklog );

socket_fd_t	xt_accept( socket_fd_t nSockfd, SOCKADDR *pAddr, SOCKLEN_T *pAddrLen );

int			xt_getsockopt( socket_fd_t nSockfd, int nLevel, int nOptName, void *pOptVal, SOCKLEN_T *pOptLen );

int			xt_setsockopt( socket_fd_t nSockfd, int nLevel, int nOptName, const void *pOptVal, SOCKLEN_T nOptLen );

int			xt_send( socket_fd_t nSockfd, const CHAR *pBuf, UINT nSize, int nFlags );

int			xt_sendto( socket_fd_t nSockfd, const CHAR *pBuf, UINT nSize, int nFlags, const SOCKADDR *pDestAddr, SOCKLEN_T nAddrLen );

int			xt_recv( socket_fd_t nSockfd, CHAR *pBuf, UINT nSize, int nFlags );

int			xt_recvfrom( socket_fd_t nSockfd, CHAR *pBuf, UINT nSize, int nFlags, SOCKADDR *pSrcAddr, SOCKLEN_T *pAddrLen );

int			xt_closesock( socket_fd_t nSockfd );

/* SHUT_RD, SHUT_WR, SHUT_RDWR have the value 0, 1, 2 */
int			xt_shutdown( socket_fd_t nSockfd, int nHow );

SOCKLEN_T	xt_get_sockaddr_len( const SOCKADDR *pAddr );

/************************************************************************/

/************************************************************************/
int	SktGetLastError(void);
/************************************************************************/	
/* NetWorks parameter data struct */
typedef struct tagNET_PARAM
{
	/* MMU & MTU through a route path */
	USHORT		route_mmu;
	USHORT		route_mtu;
	USHORT		rtt; /* ping time cost */
	USHORT		ttl;
}NET_PARAM;

int			TestNetworkParam( socket_fd_t nSockfd, NET_PARAM *param );
/************************************************************************/

/************************************************************************/
/* IP Address conversion functions */
/* For IP V4 */
UINT		xt_inet_addr( const TCHAR *szIP ); /* Unicode & ANSI string version */
int			xt_inet_aton( const TCHAR *szIP, IN_ADDR *pAddr ); /* Unicode & ANSI string version */
int			xt_inet_ntoa( UINT nIP, TCHAR *szIP, int nLen ); /* Unicode & ANSI string version */
/* END For IP V4 */

/* For both IP V4 and V6 */
/* ddd.ddd.ddd.ddd -> nnnnn */
int			xt_inet_pton( const TCHAR *ip_str, int src_len, void *dst, SOCKLEN_T *dst_size, int af ); /*=NET_PRO_FAMILY_DEF*/
int			xt_inet_pton4( const TCHAR *src, UCHAR *dst );
int			xt_inet_pton6( const TCHAR *src, UCHAR * dst);
/* nnnnn -> ddd.ddd.ddd.ddd */
int			xt_inet_ntop( const void *ip_num, int src_size, TCHAR *ip_str, int *dst_len, int af ); /*=NET_PRO_FAMILY_DEF*/
int			xt_inet_ntop4( const UCHAR *src, TCHAR *dst, int dst_len );
int			xt_inet_ntop6( const UCHAR *src, TCHAR *dst, int dst_len );

int			fill_inet_addr_struct( SOCKADDR *pAddr, SOCKLEN_T *pAddrLen, const TCHAR *szIP, 
									USHORT nPort, USHORT nFamily ); /*=NET_PRO_FAMILY_DEF*/

int			extract_inet_addr_struct( const SOCKADDR *pAddr, SOCKLEN_T nAddrLen, TCHAR *szIP, 
									int *pIpBufLen, USHORT *nPort );
/* AF_UNIX Compatible */
int			fill_sock_addr_struct( SOCKADDR *pAddr, SOCKLEN_T *pAddrLen, const TCHAR *szAddr, USHORT nFamily ); /*=NET_PRO_FAMILY_DEF*/
int			fill_sock_addr_struct_any( SOCKADDR *pAddr, SOCKLEN_T *pAddrLen, USHORT nFamily ); /*=NET_PRO_FAMILY_DEF*/

int			extract_sock_addr_struct( const SOCKADDR *pAddr, SOCKLEN_T nAddrLen, TCHAR *szAddr, int *pIpBufLen );
/* AF_UNIX Compatible END*/

USHORT		ip_str_ver( const TCHAR *ip ); /*AF_UNSPEC*/
USHORT		addr_str_af( const TCHAR *addr ); /*AF_UNSPEC*/

int			split_ip_port( const TCHAR *szSrc, TCHAR *szIP, USHORT *nPort );
int			compose_ip_port( TCHAR *szDst, const TCHAR *szIP, USHORT *nPort );

static	int		fill_unix_addr_struct( SOCKADDR *pAddr, SOCKLEN_T *pAddrLen, const TCHAR *szPath, int nPathLen );
static	int		extract_unix_addr_struct( const SOCKADDR *pAddr, SOCKLEN_T nAddrLen, TCHAR *szPath, int *nPathLen );

int			xt_net_saddr_cmp( const SOCKADDR *saddr1, const SOCKADDR *saddr2 );
int			xt_net_saddr_in6_cmp( const SOCKADDR *saddr1, const SOCKADDR *saddr2 );
#if defined(__LINUX__)
int			xt_net_saddr_un_cmp( const SOCKADDR *saddr1, const SOCKADDR *saddr2 );
#endif /*__LINUX__*/

/* END For both IP V4 and V6 */
/************************************************************************/

/*hostent*	xt_gethostbyname( const TCHAR *name );*/

#if defined(__WINDOWS__)
	int			xt_ioctlsockt( socket_fd_t nSocket, LONG cmd, ULONG *pArg );
#endif

UINT		get_sock_available( socket_fd_t nSocket );

BOOL		get_sock_blocking( socket_fd_t nSocket );
int			set_sock_nonblocking( socket_fd_t nSocket, BOOL on );


#ifdef __cplusplus
}
#endif

#endif /*__XT_NET_API_H__*/
