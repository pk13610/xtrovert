/************************************************************************************
*	ExtSktAPI.c: Implementation File
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
#include "xt_net_api.h"
#include "xt_str_api.h"

#if defined(__WINDOWS__)
/* warning C4100: 'XXX' : unreferenced formal parameter */
#pragma warning (disable: 4100)
#endif

const int _PKT_SEG_BUF_SIZE_ = 1024;	/* 1K Bytes, packet segment buffer size for socket receive/send data */
const int _PKT_BUF_SIZE_ = 32*1024;	/* 32K Bytes, packet buffer size for receive/send a whole packet data */

int PrepareWinSock()
{
	int nRet = FUN_RET_OK;

#if defined(__WINDOWS__)

	WORD wVersionRequested;
	WSADATA wsaData;
	int err;

	wVersionRequested = MAKEWORD( 2, 2 );

	err = WSAStartup( wVersionRequested, &wsaData );
	if ( err != 0 ) {
		/* Tell the user that we could not find a usable */
		/* WinSock DLL.                                  */
		return -2;
	}

	/* Confirm that the WinSock DLL supports 2.2.*/
	/* Note that if the DLL supports versions greater    */
	/* than 2.2 in addition to 2.2, it will still return */
	/* 2.2 in wVersion since that is the version we      */
	/* requested.                                        */

	if ( LOBYTE( wsaData.wVersion ) != 2 ||
		HIBYTE( wsaData.wVersion ) != 2 ) {
			/* Tell the user that we could not find a usable */
			/* WinSock DLL.                                  */
			WSACleanup( );
			return -3; 
	}
	/* The WinSock DLL is acceptable. Proceed. */

#endif /* __WINDOWS__ */

	return nRet;
}

int CleanWinSock()
{
	int nRet = FUN_RET_OK;
#if defined(__WINDOWS__)
	nRet = WSACleanup();
#endif /* __WINDOWS__ */

	return nRet;
}

socket_fd_t	xt_socket( int family, int type, int proto )
{
	/************************************************************************
	* Parameters:
	* 	nDomain[in]: AF_INET(internet socket), IF_UNIX(internal socket), ...
	* 	nType[in]: SOCK_STREAM(TCP), SOCK_DGRAM(UDP), ...
	* 	nProtocol[in]: 0
	* Return Value:
	* 	Socket descriptor
	* Remarks:
	************************************************************************/
	socket_fd_t sock = INVALID_SOCKET;
	sock = socket( family, type, proto );

	if ( sock == INVALID_SOCKET )
	{
		MARK_CALL_FUN( _T(__FUNCTION__) );
	}

	return sock;
}

int	xt_bind( socket_fd_t nSockfd, const SOCKADDR *pAddr, SOCKLEN_T nAddrLen )
{
	/************************************************************************
	* Parameters:
	*  	nSockfd[in]:
	*  	pAddr[in]:
	*  	nAddLlen[in]:
	* Return Value:
	*  
	* Remarks:
	*	*For TCP/IP, if the port is specified as zero, 
	*	 the service provider assigns a unique port to the application with 
	*	 a value between 1024 and 5000
	************************************************************************/
	int nRet = FUN_RET_OK;

	/*???*/
	/*
	pAddr == NULL?
	nAddrLen <= 0 ?
	*/
	/*What would happen?*/
	if ( nSockfd == INVALID_SOCKET ) /*|| pAddr==NULL || nAddrLen<=0*/
	{
		return FUN_RET_ERR_ILLEGAL_ARGS;
	}

	nRet = bind( nSockfd, pAddr, nAddrLen );
	if ( nRet < 0 )
	{
		MARK_CALL_FUN( _T(__FUNCTION__) );
	}

	return nRet;
}

int	xt_connect( socket_fd_t nSockfd, const SOCKADDR *pAddr, SOCKLEN_T nAddrLen )
{
	/************************************************************************
	* Parameters:
	*	nSockfd[in]:
	*	pAddr[in]:
	*	nAddLlen[in]:
	* Return Value:
	*
	* Remarks:
	************************************************************************/
	int nRet = FUN_RET_OK;

	if ( nSockfd==INVALID_SOCKET || pAddr==NULL || nAddrLen<=0 )
	{
		return FUN_RET_ERR_ILLEGAL_ARGS;
	}

	nRet = connect( nSockfd, pAddr, nAddrLen );
	if ( nRet < 0 )
	{
		MARK_CALL_FUN( _T(__FUNCTION__) );
	}

	return nRet;
}

int	xt_getsockname( socket_fd_t nSockfd, SOCKADDR *pAddr, SOCKLEN_T *pAddrLen )
{
	/************************************************************************
	* Parameters:
	*	nSockfd[in]:
	*	pAddr[in][out]:
	*	pAddLlen[in][out]:
	* Return Value:
	*
	* Remarks:
	************************************************************************/
	int nRet = FUN_RET_OK;

	if ( nSockfd==INVALID_SOCKET || pAddr==NULL || pAddrLen==NULL )
	{
		return FUN_RET_ERR_ILLEGAL_ARGS;
	}

	nRet = getsockname( nSockfd, pAddr, pAddrLen );
	if ( nRet < 0 )
	{
		MARK_CALL_FUN( _T(__FUNCTION__) );
	}

	return nRet;
}

int xt_get_sockname_str( socket_fd_t nSockfd, TCHAR *addr, int len )
{
	SOCKADDR_STORAGE sock_addr;
	SOCKLEN_T sock_len = sizeof(sock_addr);

	xt_getsockname( nSockfd, (SOCKADDR*)&sock_addr, &sock_len );

	extract_sock_addr_struct( (SOCKADDR*)&sock_addr, sock_len, addr, &len );

	return FUN_RET_OK;
}

int	xt_getpeername( socket_fd_t nSockfd, SOCKADDR *pAddr, SOCKLEN_T *pAddrLen )
{
	/************************************************************************
	* Parameters:
	*	nSockfd[in]:
	*	pAddr[in]:
	*	nAddLlen[in]:
	* Return Value:
	*
	* Remarks:
	************************************************************************/
	int nRet = FUN_RET_OK;

	if ( nSockfd==INVALID_SOCKET || pAddr==NULL || pAddrLen==NULL )
	{
		return FUN_RET_ERR_ILLEGAL_ARGS;
	}

	nRet = getpeername( nSockfd, pAddr, pAddrLen );
	if ( nRet < 0 )
	{
		MARK_CALL_FUN( _T(__FUNCTION__) );
	}

	return nRet;
}

int xt_get_peername_str( socket_fd_t nSockfd, TCHAR *addr, int len )
{
	SOCKADDR_STORAGE sock_addr;
	SOCKLEN_T sock_len = sizeof(sock_addr);

	xt_getpeername( nSockfd, (SOCKADDR*)&sock_addr, &sock_len );

	extract_sock_addr_struct( (SOCKADDR*)&sock_addr, sock_len, addr, &len );

	return FUN_RET_OK;
}

int	xt_listen( socket_fd_t nSockfd, int nBacklog )
{
	/************************************************************************
	* Parameters:
	* 	nSockfd[in]:
	* 	nBacklog[in]:
	* Return Value:
	* 
	* Remarks:
	*
	************************************************************************/
	int nRet = FUN_RET_OK;
	CHAR *pLsnQ = NULL;

	if ( nSockfd==INVALID_SOCKET || nBacklog<0 )
	{
		return FUN_RET_ERR_ILLEGAL_ARGS;
	}
	
	if ( nBacklog == 0 )
	{
		pLsnQ = getenv("LISTENQ");
		if ( pLsnQ != NULL )
		{
			nBacklog = atoi( pLsnQ );
		}
	}

/*
#ifdef __LINUX__
	TCHAR *pLsnQ = NULL;
	if ( nBacklog == 0 )
	{
		pLsnQ = getenv(_T("LISTENQ"));
		if ( pLsnQ != NULL )
		{
			nBacklog = atoi( pLsnQ );
		}
	}
#endif
*/
	nRet = listen( nSockfd, nBacklog );
	if ( nRet < 0 )
	{
		MARK_CALL_FUN( _T(__FUNCTION__) );
	}

	return nRet;
}

socket_fd_t	xt_accept( socket_fd_t nSockfd, SOCKADDR *pAddr, SOCKLEN_T *pAddrLen )
{
	/************************************************************************
	* Parameters:
	*  	nSockfd[in]: socket descriptor
	*  	pAddr[out]: socket address structure
	*  	pAddrLen[out]: length of socket address structure
	*  Return Value:
	*  	Socket descriptor
	* Remarks:
	*
	************************************************************************/
	socket_fd_t nSock = INVALID_SOCKET;

	if ( nSockfd == INVALID_SOCKET ) /* || pAddr==NULL || pAddrLen==NULL*/
	{
		return INVALID_SOCKET;
	}

	nSock = accept( nSockfd, pAddr, pAddrLen );

	if ( nSock == INVALID_SOCKET )
	{
		MARK_CALL_FUN( _T(__FUNCTION__) );
	}

	return nSock;
}
/*
  nLevel        nOptName
SOL_SOCKET    SO_REUSEADDR
SOL_SOCKET    SO_KKEPALIVE
SOL_SOCKET    SO_LINGER
SOL_SOCKET    SO_BROADCAST
SOL_SOCKET    SO_OOBINLINE
SOL_SOCKET    SO_SNDBUF
SOL_SOCKET    SO_RCVBUF
SOL_SOCKET    SO_TYPE
SOL_SOCKET    SO_ERROR
SOL_TCP       SO_NODELAY
*/
int	xt_getsockopt( socket_fd_t nSockfd, int nLevel, int nOptName, void *pOptVal, SOCKLEN_T *pOptLen )
{
	/************************************************************************
	* Parameters:
	*	nSockfd[in]: socket descriptor
	*	nLevel[in]: socket option level ( SOL_SOCKET , ... )
	*	nOptName[in]: socket option name ( SO_REUSEADDR , SO_LINGER , ... )
	*	pOptVal[out]: pointer to contain option value
	*	pOptLen[out]: length of pOptVal
	* Return Value:
	*	Socket descriptor
	* Remarks:
	*
	************************************************************************/
	int nRet = FUN_RET_OK;
	if ( nSockfd==INVALID_SOCKET || pOptVal==NULL || pOptLen==NULL )
	{
		return FUN_RET_ERR_ILLEGAL_ARGS;
	}

	nRet = getsockopt( nSockfd, nLevel, nOptName, pOptVal, pOptLen );
	if ( nRet != 0 )
	{
		MARK_CALL_FUN( _T(__FUNCTION__) );
	}

	return nRet;
}

int	xt_setsockopt( socket_fd_t nSockfd, int nLevel, int nOptName, const void *pOptVal, SOCKLEN_T nOptLen )
{
	/************************************************************************
	* Parameters:
	*	nSockfd[in]: socket descriptor
	*	nLevel[in]: socket option level ( SOL_SOCKET , ... )
	*	nOptName[in]: socket option name ( SO_REUSEADDR , SO_LINGER , ... )
	*	pOptVal[out]: pointer to contain option value
	*	pOptLen[out]: length of pOptVal
	* Return Value:
	*	Socket descriptor
	* Remarks:
	*
	************************************************************************/
	int nRet = FUN_RET_OK;
	if ( nSockfd==INVALID_SOCKET || pOptVal==NULL || nOptLen<=0 )
	{
		return FUN_RET_ERR_ILLEGAL_ARGS;
	}

	nRet = setsockopt( nSockfd, nLevel, nOptName, pOptVal, nOptLen );
	if ( nRet != 0 )
	{
		MARK_CALL_FUN( _T(__FUNCTION__) );
	}

	return nRet;
}

int xt_send( socket_fd_t nSockfd, const CHAR *pBuf, UINT nSize, int nFlags )
{
	/************************************************************************
	* Parameters:
	*	nSockfd[in]: socket descriptor
	*	pBuf[in]: socket option level ( SOL_SOCKET , ... )
	*	nSize[in]: socket option name ( SO_REUSEADDR , SO_LINGER , ... )
	*	nFlags[out]: pointer to contain option value
	* Return Value:
	*	length of bytes sent
	* Remarks:
	*
	************************************************************************/
	int nRet = FUN_RET_OK;
	if ( nSockfd==INVALID_SOCKET || pBuf==NULL )
	{
		return FUN_RET_ERR_ILLEGAL_ARGS;
	}

	if ( nSize == 0 )
	{
		return 0;
	}
	
	nRet = send( nSockfd, pBuf, nSize, nFlags );
	if ( nRet < 0 )
	{
		MARK_CALL_FUN( _T(__FUNCTION__) );
	}

	return nRet;
}

int	xt_sendto( socket_fd_t nSockfd, const CHAR *pBuf, UINT nSize, int nFlags, const SOCKADDR *pDestAddr, SOCKLEN_T nAddrLen )
{	
	/************************************************************************
	* Parameters:
	*	nSockfd[in]: socket descriptor
	*	pBuf[in]: socket option level ( SOL_SOCKET , ... )
	*	nSize[in]: socket option name ( SO_REUSEADDR , SO_LINGER , ... )
	*	nFlags[out]: pointer to contain option value
	*	pDestAddr
	*	nAddrLen
	* Return Value:
	*	length of bytes sent
	* Remarks:
	*
	************************************************************************/
	int nRet = FUN_RET_OK;
	if ( nSockfd==INVALID_SOCKET || pBuf==NULL )
	{
		return FUN_RET_ERR_ILLEGAL_ARGS;
	}

	if ( nSize == 0 )
	{
		return 0;
	}

	nRet = sendto( nSockfd, pBuf, nSize, nFlags, pDestAddr, nAddrLen );
	if ( nRet < 0 )
	{
		MARK_CALL_FUN( _T(__FUNCTION__) );
	}

	return nRet;
}

int	xt_recv( socket_fd_t nSockfd, CHAR *pBuf, UINT nSize, int nFlags )
{
	/************************************************************************
	* Parameters:
	*  	nSockfd[in]: socket descriptor
	*  	pBuf[in]: input buffer
	* 	nSize[in]: input data length
	*  	nFlags[out]: send flag (MSG_OOB,MSG_DONTROUTE)
	* Return Value:
	*  	length of bytes received
	* Remarks:
	************************************************************************/
	int nRet = FUN_RET_OK;
	if ( nSockfd==INVALID_SOCKET || pBuf==NULL )
	{
		return FUN_RET_ERR_ILLEGAL_ARGS;
	}

	if ( nSize == 0 )
	{
		return 0;
	}

	nRet = recv( nSockfd, pBuf, nSize, nFlags );
	if ( nRet == SOCKET_ERROR )
	{
		MARK_CALL_FUN( _T(__FUNCTION__) );
	}

	return nRet;
}

int	xt_recvfrom( socket_fd_t nSockfd, CHAR *pBuf, UINT nSize, int nFlags, SOCKADDR *pSrcAddr, SOCKLEN_T *pAddrLen )
{
	/************************************************************************
	* Parameters:
	*  	nSockfd[in]: socket descriptor
	*  	pBuf[in]: input buffer
	* 	nSize[in]: input data length
	*  	nFlags[out]: send flag (MSG_OOB,MSG_DONTROUTE)
	* Return Value:
	*  	length of bytes received
	* Remarks:
	************************************************************************/
	int nRet = FUN_RET_OK;
	if ( nSockfd==INVALID_SOCKET || pBuf==NULL ) /*|| pSrcAddr==NULL || pAddrLen<=0*/
	{
		return FUN_RET_ERR_ILLEGAL_ARGS;
	}

	if ( nSize == 0 )
	{
		return 0;
	}

	nRet = recvfrom( nSockfd, pBuf, nSize, nFlags, pSrcAddr, pAddrLen );
	if ( nRet < 0 )
	{
		MARK_CALL_FUN( _T(__FUNCTION__) );
	}

	return nRet;
}

int	xt_closesock( socket_fd_t nSockfd )
{
	/************************************************************************
	* Parameters:
	*	nSockfd[in]: socket descriptor
	*	nHow[in]: how to close ( all , send , receive )
	* Return Value:
	*	length of bytes received
	* Remarks:
	*
	************************************************************************/
	int nRet = FUN_RET_OK;
	if ( nSockfd == INVALID_SOCKET )
	{
		return FUN_RET_ERR_ILLEGAL_ARGS;
	}

	nRet = closesocket( nSockfd );
	if ( nRet == SOCKET_ERROR )
	{
		MARK_CALL_FUN( _T(__FUNCTION__) );
	}

	return nRet;
}


/* SHUT_RD, SHUT_WR, SHUT_RDWR have the value 0, 1, 2 */
int	xt_shutdown( socket_fd_t nSockfd, int nHow )
{
	/************************************************************************
	* Parameters:
	*	nSockfd[in]: socket descriptor
	*	nHow[in]: how to close ( all , send , receive )
	* Return Value:
	*	length of bytes received
	* Remarks:
	*
	************************************************************************/
	int nRet = FUN_RET_OK;
	if ( nSockfd==INVALID_SOCKET || nHow<SHUTDOWN_RECV || nHow>=SHUTDOWN_TYPE_END )
	{
		return FUN_RET_ERR_ILLEGAL_ARGS;
	}

	nRet = shutdown( nSockfd, nHow );
	if ( nRet <= SOCKET_ERROR )
	{
		MARK_CALL_FUN( _T(__FUNCTION__) );
	}

	return nRet;
}

SOCKLEN_T xt_get_sockaddr_len( const SOCKADDR *pAddr )
{
	SOCKLEN_T nSktLen = -1;

	if ( pAddr == NULL )
	{
		return FUN_RET_ERR_ILLEGAL_ARGS;
	}

	switch( pAddr->sa_family )
	{
	case AF_INET:
		nSktLen = sizeof(SOCKADDR_IN);
		break;
	case AF_INET6:
		nSktLen = sizeof(SOCKADDR_IN6);
		break;
#if defined (__LINUX__)
	case AF_UNIX:
		nSktLen = sizeof(struct sockaddr_un);
		break;
#endif
	default:
		break;
	}

	return nSktLen;
}

int	SktGetLastError(void)
{
#if defined(__LINUX__)
	return errno;
#elif defined(__WINDOWS__)
	return  WSAGetLastError();
#else
	return -3;
#endif
}

int TestNetworkParam( socket_fd_t nSockfd, NET_PARAM *param )
{
	/************************************************************************
	* Parameters:
	*	nSockfd[in]: socket descriptor
	*	param[out]:
	* Return Value:
	*
	* Remarks:
	*
	************************************************************************/
	int nRet = FUN_RET_OK;
	if ( nSockfd==INVALID_SOCKET || param==NULL )
	{
		return FUN_RET_ERR_ILLEGAL_ARGS;
	}

	return nRet;
}

 UINT xt_inet_addr( const TCHAR *szIP ) /* Unicode & ANSI string version */
{
	UINT nIP = 0;

	if ( xt_inet_aton( szIP, (IN_ADDR*)&nIP ) == 1 )
	{
		return nIP;
	}

	return INADDR_NONE;
}

int xt_inet_aton( const TCHAR *szIP, IN_ADDR *pAddr ) /* Unicode & ANSI string version */
{
/* 
	int inet_aton(const char *cp, struct in_addr *ap) 
	inet_aton将 点分十进制串cp 转换为一个网络字节顺序的32位整数 IP地址
	 例如将串cp "192.168.33.123 "
	 转为 1100 0000   1010 1000    0010 0001     0111 1011 
	 成功返回1，出错返回0
	转换成功，结果保存在结构体指针ap中
*/

/*
	const CHAR *szTmpIP = NULL;
#ifdef _TCHAR_DEFINED
	CHAR szIpBuf[INET_ADDRSTRLEN] = {0};
	xt_size_t nCount = wcstombs( szIpBuf, szIP, INET_ADDRSTRLEN );
	if ( nCount == 0 )
	{
		return INADDR_NONE;
	}
	szTmpIP = szIpBuf;
#else
	szTmpIP = szIP;
#endif
*/
	int dots = 0;
	register UINT acc = 0, addr = 0;

	do
	{
		register TCHAR cc = *szIP;
		switch (cc) 
		{
		case _T('0'):
		case _T('1'):
		case _T('2'):
		case _T('3'):
		case _T('4'):
		case _T('5'):
		case _T('6'):
		case _T('7'):
		case _T('8'):
		case _T('9'):
			acc = acc * 10 + (cc - _T('0'));
			break;

		case _T('.'):
			if (++dots > 3)
			{
				return 0;
			}
			/* Fall through */

		case _T('\0'):
			if (acc > 255) 
			{
				return 0;
			}
			addr = addr << 8 | acc; /* 这句是精华,每次将当前值左移八位加上后面的值 */
			acc = 0;
			break;

		default:
			return 0;
		}

	} while ( (*szIP++) != 0 );

	/*Normalize the address */
	if (dots < 3)
	{
		addr <<= 8 * (3 - dots) ;
	}

	/*Store it if requested */
	if (pAddr) 
	{
		pAddr->s_addr = htonl(addr);
		/*pAddr->s_addr = addr;*/
	}

	return 1;
}

int xt_inet_ntoa( UINT nIP, TCHAR *szIP, int nLen ) /* Unicode & ANSI string version */
{
	UCHAR *pByte = (UCHAR*)&nIP;

	if ( szIP==NULL || nLen<INET_ADDRSTRLEN )
	{
		return FUN_RET_ERR_ILLEGAL_ARGS;
	}

	if ( ENDIANNESS == 'l' ) /* Little endian */
	{
		_sntprintf( szIP, nLen, _T("%u.%u.%u.%u"), pByte[0], pByte[1], pByte[2], pByte[3] );
	}
	else
	{
		/* Big Endian */
		_sntprintf( szIP, nLen, _T("%u.%u.%u.%u"), pByte[3], pByte[2], pByte[1], pByte[0] );
	}

	return FUN_RET_OK;
}

/* ddd.ddd.ddd.ddd -> nnnnn */
int xt_inet_pton( const TCHAR *ip_str, int src_len, void *dst, SOCKLEN_T *dst_size, int af/*=AF_INET*/ )
{
    int nRet = FUN_RET_OK;

    if ( ip_str==NULL || src_len<=0 || dst==NULL || dst_size==NULL )
    {
        return FUN_RET_ERR_ILLEGAL_ARGS;
    }

	switch ( af )
	{
	case AF_INET:
		if ( *dst_size < NS_INADDRSZ )
		{
			nRet = -4;
		}
		nRet = xt_inet_aton( ip_str, (IN_ADDR*)dst );
		if ( nRet == 1 )
		{
			*dst_size = NS_INADDRSZ;
		}
		else
		{
			nRet = -5;
		}
		break;
	case AF_INET6:
		if ( *dst_size < NS_IN6ADDRSZ )
		{
			nRet = -6;
		}
		nRet = xt_inet_pton6( ip_str, (UCHAR*)dst );
		if ( nRet == 1 )
		{
			*dst_size = NS_IN6ADDRSZ;
		}
		else
		{
			nRet = -7;
		}
		break;
/*
#if defined (__LINUX__)
		case AF_UNIX:
		{
			struct sockaddr_un	*unp = (struct sockaddr_un *) sa;

			if ( unp->sun_path[0] == 0 )
			{
				strcpy(str, "(no pathname bound)");
			}
			else
			{
				_sntprintf(str, sizeof(str), _T("%s"), unp->sun_path);
			}
			return(str);
		}
		break;
#endif
*/
	default:
		nRet = -3;
		break;
	}

    return nRet;
}

int xt_inet_pton4( const TCHAR *src, UCHAR *dst )
{
	return xt_inet_aton( src, (IN_ADDR*)dst );
}

int xt_inet_pton6( const TCHAR *src, UCHAR * dst)
{
	static const TCHAR xdigits[] = _T("0123456789abcdef");
	UCHAR tmp[NS_IN6ADDRSZ], *tp, *endp, *colonp;
	const TCHAR *curtok;
	int ch, saw_xdigit;
	UINT val;

	tp = (UCHAR*)memset(tmp, 0, NS_IN6ADDRSZ);
	endp = tp + NS_IN6ADDRSZ;
	colonp = NULL;
	/** Leading :: requires some special handling. */
	if (*src == ':')
		if (*++src != ':')
			return (0);
	curtok = src;
	saw_xdigit = 0;
	val = 0;
	while ((ch = tolower (*src++)) != _T('\0')) {
		const TCHAR *pch = NULL;
		/*pch = strchr(xdigits, (int)ch); ?????????????????????*/
		if (pch != NULL) {
			val <<= 4;
			val |= (pch - xdigits);
			if (val > 0xffff)
				return (0);
			saw_xdigit = 1;
			continue;
		}
		if (ch == ':') {
			curtok = src;
			if (!saw_xdigit) {
				if (colonp)
					return (0);
				colonp = tp;
				continue;
			} else if (*src == _T('\0')) {
				return (0);
			}
			if (tp + NS_INT16SZ > endp)
				return (0);
			*tp++ = (u_char) (val >> 8) & 0xff;
			*tp++ = (u_char) val & 0xff;
			saw_xdigit = 0;
			val = 0;
			continue;
		}
		if (ch == _T('.') && ((tp + NS_INADDRSZ) <= endp) &&
		    xt_inet_pton4(curtok, tp) > 0) {
			tp += NS_INADDRSZ;
			saw_xdigit = 0;
			break;	/** '\0' was seen by inet_pton4(). */
		}
		return (0);
	}
	if (saw_xdigit) {
		if (tp + NS_INT16SZ > endp)
			return (0);
		*tp++ = (u_char) (val >> 8) & 0xff;
		*tp++ = (u_char) val & 0xff;
	}
	if (colonp != NULL) {
		/**
		 * Since some memmove()'s erroneously fail to handle
		 * overlapping regions, we'll do the shift by hand.
		 */
		const int n = tp - colonp;
		int i;

		if (tp == endp)
			return (0);
		for (i = 1; i <= n; ++i) {
			endp[- i] = colonp[n - i];
			colonp[n - i] = 0;
		}
		tp = endp;
	}
	if (tp != endp)
		return (0);
	memcpy(dst, tmp, NS_IN6ADDRSZ);
	return (1);
}

/* nnnnn -> ddd.ddd.ddd.ddd */
int xt_inet_ntop( const void *ip_num, int src_size, TCHAR *ip_str, int *dst_len, int af/*=AF_INET*/ )
{
    int nRet = FUN_RET_OK;

    if ( ip_num==NULL || src_size<=0 || ip_str==NULL || dst_len==NULL )
    {
        return FUN_RET_ERR_ILLEGAL_ARGS;
    }

	switch ( af )
	{
	case AF_INET:
		if ( *dst_len < INET_ADDRSTRLEN )
		{
			nRet = -4;
		}
		nRet = xt_inet_ntoa( *((UINT*)ip_num), ip_str, *dst_len );
		if ( nRet == FUN_RET_OK )
		{
			*dst_len = INET_ADDRSTRLEN;
		}
		else
		{
			nRet = -5;
		}
		break;
	case AF_INET6:
		break;
#if defined (__LINUX__)
	case AF_UNIX:
		break;
#endif
	default:
		nRet = -3;
		break;
	}

    return nRet;
}
int xt_inet_ntop4( const UCHAR *src, TCHAR *dst, int dst_len )
{
	return xt_inet_ntoa( *((UINT*)src), dst, dst_len );
}

/* const char *
 * inet_ntop6(src, dst, size)
 *  convert IPv6 binary address into presentation (printable) format
 * author:
 *  Paul Vixie, 1996.
 */
int xt_inet_ntop6( const UCHAR *src, TCHAR *dst, int dst_len )
{
	if ( src==NULL || dst==NULL || dst_len<=0 )
	{
		return FUN_RET_ERR_ILLEGAL_ARGS;
	}
    /*
     * Note that int32_t and int16_t need only be "at least" large enough
     * to contain a value of the specified size.  On some systems, like
     * Crays, there is no such thing as an integer variable with 16 bits.
     * Keep this in mind if you think this function should have been coded
     * to use pointer overlays.  All the world's not a VAX.
     */

//     char tmp[sizeof "ffff:ffff:ffff:ffff:ffff:ffff:255.255.255.255"], *tp;
//     struct { int base, len; } best, cur;
//     u_int words[NS_IN6ADDRSZ / NS_int16SZ];
//     int i; 
//     /*
//      * Preprocess:
//      *  Copy the input (bytewise) array into a wordwise array.
//      *  Find the longest run of 0x00's in src[] for :: shorthanding.
//      */
//     memset(words, '', sizeof words);
//     for (i = 0; i < NS_IN6ADDRSZ; i += 2)
//         words[i / 2] = (src[i] << 8) | src[i + 1];
//     best.base = -1;
//     cur.base = -1;
//     best.len = 0;
//     cur.len = 0;
//     for (i = 0; i < (NS_IN6ADDRSZ / NS_int16SZ); ++i) {
//         if (words[i] == 0) {
//             if (cur.base == -1)
//                 cur.base = i, cur.len = 1;
//             else
//                 ++(cur.len);
//         } else {
//             if (cur.base != -1) {
//                 if (best.base == -1 || cur.len > best.len)
//                     best = cur;
//                 cur.base = -1;
//             }
//         }
//     }
//     if (cur.base != -1) {
//         if (best.base == -1 || cur.len > best.len)
//             best = cur;
//     }
//     if (best.base != -1 && best.len < 2)
//         best.base = -1;
// 
//     /*
//      * Format the result.
//      */
// 
//     tp = tmp;
//     for (i = 0; i < (NS_IN6ADDRSZ / NS_int16SZ); ++i) {
//         /* Are we inside the best run of 0x00's? */
//         if (best.base != -1 && i >= best.base &&
//             i < (best.base + best.len)) {
//             if (i == best.base)
//                 *tp++ = ':';
//             continue;
//         }
//         /* Are we following an initial run of 0x00s or any real hex? */
//         if (i != 0)
//             *tp++ = ':';
//         /* Is this address an encapsulated IPv4? */
//         if (i == 6 && best.base == 0 &&
//             (best.len == 6 || (best.len == 5 && words[5] == 0xffff))) {
//             if (!inet_ntop4(src+12, tp, sizeof tmp - (tp - tmp)))
//                 return (NULL);
//             tp += strlen(tp);
//             break;
//         }
//         tp += SPRintF((tp, "%x", words[i]));
//     }
//     /* Was it a trailing run of 0x00's? */
//     if (best.base != -1 && (best.base + best.len) ==
//         (NS_IN6ADDRSZ / NS_int16SZ))
//         *tp++ = ':';
//     *tp++ = '';
//  
//     /*
//      * Check for overflow, copy, and we're done.
//      */
//     if ((int)(tp - tmp) > size) {
//         __set_errno (ENOSPC);
//         return (NULL);
//     }
//     return strcpy(dst, tmp);

	return FUN_RET_OK;
}

int fill_inet_addr_struct( SOCKADDR *pAddr, SOCKLEN_T *pAddrLen, const TCHAR *szIP, USHORT nPort, USHORT nFamily/*=AF_INET*/ )
{
	int nIpStrLen = 0;
	if ( pAddr==NULL || pAddrLen==NULL || szIP==NULL )
	{
		return FUN_RET_ERR_ILLEGAL_ARGS;
	}

	memset( pAddr, 0, *pAddrLen );
	pAddr->sa_family = nFamily;
	nIpStrLen = _tcslen( szIP );
	switch( nFamily )
	{
	case AF_INET:
		((SOCKADDR_IN*)pAddr)->sin_port = htons( nPort );
		((SOCKADDR_IN*)pAddr)->sin_addr.s_addr = xt_inet_addr( szIP );
		*pAddrLen = sizeof(SOCKADDR_IN);
		break;
	case AF_INET6:
		((SOCKADDR_IN6*)pAddr)->sin6_port = htons( nPort );
		if ( xt_inet_pton(szIP, nIpStrLen, &(((SOCKADDR_IN6*)pAddr)->sin6_addr), pAddrLen, AF_INET6) < 0 )
		{
			return -4;
		}
		*pAddrLen = sizeof(SOCKADDR_IN6);
		break;
/*	case AF_UNIX:
			return fill_unix_addr_struct(pAddr, pAddrLen, szIP, nIpStrLen);
		break;
*/
	default:
		return ERR_SOCK_AF_NOT_SURPPORT;
		break;
	}
	return FUN_RET_OK;
}

int extract_inet_addr_struct( const SOCKADDR *pAddr, SOCKLEN_T nAddrLen, TCHAR *szIP, int *pIpBufLen, USHORT *nPort )
{
	/*
	*	return AF_INET,AF_INET6 or AF_UNIX
	*			- integer & AF_UNSPEC for failure
	*/
	USHORT nFamily = AF_UNSPEC;
	if ( pAddr==NULL || nAddrLen<=0 || szIP==NULL || pIpBufLen==NULL )
	{
		return FUN_RET_ERR_ILLEGAL_ARGS;
	}
	switch( pAddr->sa_family )
	{
	case AF_INET:
		*nPort = ntohs( ((SOCKADDR_IN*)pAddr)->sin_port );
		if  ( xt_inet_ntoa(((SOCKADDR_IN*)pAddr)->sin_addr.s_addr, szIP, *pIpBufLen) < 0 )
		{
			return -4;
		}		
		nFamily = AF_INET;
		break;
	case AF_INET6:
		*nPort = ntohs( ((SOCKADDR_IN6*)pAddr)->sin6_port );
		if ( xt_inet_ntop(&(((SOCKADDR_IN6*)pAddr)->sin6_addr), sizeof(((SOCKADDR_IN6*)pAddr)->sin6_addr), szIP, pIpBufLen, AF_INET6) < 0 )
		{
			return -5;
		}
		nFamily = AF_INET6;
		break;
		/*
	case AF_UNIX:
		return extract_unix_addr_struct(pAddr, nAddrLen, szIP, *pIpBufLen);
		break;
		*/
	default:
		return ERR_SOCK_AF_NOT_SURPPORT;
		break;
	}
	*pIpBufLen = _tcslen( szIP );

	return nFamily;
}

int fill_sock_addr_struct( SOCKADDR *pAddr, SOCKLEN_T *pAddrLen, const TCHAR *szAddr, USHORT nFamily/*=NET_PRO_FAMILY_DEF*/ )
{
	int nStrLen = 0;
	TCHAR szIP[INET_ADDRSTRLEN_MAX] = {0};
	USHORT nPort = 0;

	if ( pAddr==NULL || pAddrLen==NULL || szAddr==NULL )
	{
		return FUN_RET_ERR_ILLEGAL_ARGS;
	}

	memset( pAddr, 0, *pAddrLen );
	pAddr->sa_family = nFamily;
	nStrLen = _tcslen( szAddr );

	switch ( nFamily )
	{
	case AF_INET:
		{
			if ( split_ip_port(szAddr, szIP, &nPort) < 0 )
			{
				return -4;
			}
			if ( nStrLen > INET_ADDRSTRLEN_MAX+INET_PORT_STRLEN_MAX )
			{
				return -5;
			}
			((SOCKADDR_IN*)pAddr)->sin_port = htons( nPort );
			((SOCKADDR_IN*)pAddr)->sin_addr.s_addr = xt_inet_addr( szIP );
			*pAddrLen = sizeof(SOCKADDR_IN);
		}
		break;
	case AF_INET6:
		{
			if ( split_ip_port(szAddr, szIP, &nPort) < 0 )
			{
				return -6;
			}
			if ( nStrLen > INET_ADDRSTRLEN_MAX+INET_PORT_STRLEN_MAX )
			{
				return -7;
			}
			((SOCKADDR_IN6*)pAddr)->sin6_port = htons( nPort );
			if ( xt_inet_pton(szIP, nStrLen, &(((SOCKADDR_IN6*)pAddr)->sin6_addr), pAddrLen, AF_INET6) < 0 )
			{
				return -8;
			}
			*pAddrLen = sizeof(SOCKADDR_IN6);
		}		
		break;
	case AF_UNIX:
		return fill_unix_addr_struct( pAddr, pAddrLen, szAddr, nStrLen );
		break;
	default:
		return ERR_SOCK_AF_NOT_SURPPORT;
		break;
	}

	return FUN_RET_OK;
}

int fill_sock_addr_struct_any( SOCKADDR *pAddr, SOCKLEN_T *pAddrLen, USHORT nFamily/*=NET_PRO_FAMILY_DEF*/ )
{
	/*ADDR_ANY*/
	#define port_any			(0)

	if ( pAddr==NULL || pAddrLen==NULL )
	{
		return FUN_RET_ERR_ILLEGAL_ARGS;
	}

	memset( pAddr, 0, *pAddrLen );
	pAddr->sa_family = nFamily;

	switch ( nFamily )
	{
	case AF_INET:
		{
			((SOCKADDR_IN*)pAddr)->sin_port = port_any;
			((SOCKADDR_IN*)pAddr)->sin_addr.s_addr = ADDR_ANY;
			*pAddrLen = sizeof(SOCKADDR_IN);
		}
		break;
	case AF_INET6:
		{
			((SOCKADDR_IN6*)pAddr)->sin6_port = port_any;
			/*((SOCKADDR_IN6*)pAddr)->sin6_addr.s6_addr = ADDR_ANY;*/ /* ???? */
			*pAddrLen = sizeof(SOCKADDR_IN6);
		}		
		break;
	case AF_UNIX:
		/*return fill_unix_addr_struct( pAddr, pAddrLen, szAddr, nStrLen );*/
		break;
	default:
		return ERR_SOCK_AF_NOT_SURPPORT;
		break;
	}

	return FUN_RET_OK;
}

int extract_sock_addr_struct( const SOCKADDR *pAddr, SOCKLEN_T nAddrLen, TCHAR *szAddr, int *pIpBufLen )
{
	/*
	*	return AF_INET,AF_INET6 or AF_UNIX
	*			- integer & AF_UNSPEC for failure
	*/
	USHORT nFamily = AF_UNSPEC;
	if ( pAddr==NULL || nAddrLen<=0 || szAddr==NULL || pIpBufLen==NULL )
	{
		return FUN_RET_ERR_ILLEGAL_ARGS;
	}
	switch( pAddr->sa_family )
	{
	case AF_INET:
		{
			TCHAR szIP[INET_ADDRSTRLEN_MAX] = {0};
			USHORT nPort = 0;
			nPort = ntohs( ((SOCKADDR_IN*)pAddr)->sin_port );
			if  ( xt_inet_ntoa(((SOCKADDR_IN*)pAddr)->sin_addr.s_addr, szIP, *pIpBufLen) < 0 )
			{
				return -4;
			}
			if ( compose_ip_port(szAddr, szIP, &nPort) < 0 )
			{
				return -5;
			}
			nFamily = AF_INET;
		}
		break;
	case AF_INET6:
		{
			TCHAR szIP[INET_ADDRSTRLEN_MAX] = {0};
			USHORT nPort = 0;
			nPort = ntohs( ((SOCKADDR_IN6*)pAddr)->sin6_port );
			if ( xt_inet_ntop(&(((SOCKADDR_IN6*)pAddr)->sin6_addr), sizeof(((SOCKADDR_IN6*)pAddr)->sin6_addr), szIP, pIpBufLen, AF_INET6) < 0 )
			{
				return -5;
			}
			if ( compose_ip_port(szAddr, szIP, &nPort) < 0 )
			{
				return -5;
			}
			nFamily = AF_INET6;
		}		
		break;
	case AF_UNIX:
		return extract_unix_addr_struct(pAddr, nAddrLen, szAddr, pIpBufLen);
		break;
	default:
		return ERR_SOCK_AF_NOT_SURPPORT;
		break;
	}
	*pIpBufLen = _tcslen( szAddr );

	return nFamily;
}

USHORT ip_str_ver( const TCHAR *ip ) /*AF_UNSPEC*/
{
	int nStrLen = 0;
	USHORT nFamily = AF_UNSPEC;

	if ( ip == NULL )
	{
		return AF_UNSPEC;
	}
	
	nStrLen = _tcslen(ip);
	if ( nStrLen > INET_ADDRSTRLEN_MAX )
	{
		return AF_UNSPEC;
	}

	do 
	{
		switch ( *ip )
		{
		case _T('.'):
			nFamily = AF_INET;
			break;
		case _T(':'):
			nFamily = AF_INET6;
			break;
		default:
			/* nFamily==AF_UNSPEC */
			break;
		}

		++ip;
	} while ( (*ip)!=0 && nFamily==AF_UNSPEC );

	return nFamily;
}

USHORT addr_str_af( const TCHAR *addr )
{
	int len = 0;
	USHORT family = AF_UNSPEC;

	if ( addr == NULL )
	{
		return AF_UNSPEC;
	}

	len = _tcslen(addr);
	if ( len > INET_ADDRSTRLEN_MAX )
	{
		return AF_UNSPEC;
	}

	do 
	{
		switch ( *addr )
		{
		case _T('.'):
			family = AF_INET;
			break;
		case _T(':'):
			family = AF_INET6;
			break;
#if defined(__LINUX__)
		case _T('/'):
			family = AF_UNIX;
			break;
#endif
		default:
			/* family==AF_UNSPEC */
			break;
		}

		++addr;
	} while ( (*addr)!=0 && family==AF_UNSPEC );

	return family;
}

int split_ip_port( const TCHAR *szSrc, TCHAR *szIP, USHORT *nPort )
{
	/************************************************************************
	* szSrc : "127.0.0.1:7788\0" "[FFFF::0::EF]:7788"
	************************************************************************/
	USHORT nFamily = AF_UNSPEC;
	int nPos = 0;
	int nLen = 0;
	int nPos2 = 0;
	TCHAR szPort[INET_PORT_STRLEN_MAX] = {0};
	int nRet = 0;

	if ( szSrc==NULL || szIP==NULL )
	{
		return FUN_RET_ERR_ILLEGAL_ARGS;
	}

	nFamily = ip_str_ver(szSrc);
	if ( nFamily == AF_UNSPEC )
	{
		return FUN_RET_ERR_ILLEGAL_ARGS;
	}

	nPos = 0;
	nLen = _tcslen( szSrc );

	if ( nLen > INET_ADDRSTRLEN_MAX )
	{
		return -3;
	}

	nPos = nLen;
	while( nPos >= 0 )
	{
		if ( szSrc[nPos] == _T(':') )
		{
			break;
		}
		--nPos;
	}

	if ( nPos < 0 )
	{
		return -4;
	}
	/* else found a space character _T(' ') */

	if ( nFamily == AF_INET )
	{
		memcpy( szIP, szSrc, nPos*sizeof(TCHAR) ); /* Got IP String */
	}
	else if ( nFamily == AF_INET6 )
	{
		memcpy( szIP, szSrc+1, (nPos-1)*sizeof(TCHAR) ); /* Got IP String */
	}	
	
	++nPos; /* Move forward, skip _T(':') */
	nPos2 = nPos;

	while( nPos2<nLen )
	{
		if ( szSrc[nPos2] == _T('\0') ) /* Reached Tail */
		{
			break;
		}
		++nPos2;
	}
/*
	if ( nPos2 >= nLen )
	{
		return -5;
	}
*/
	/* else found a NULL character _T('\0') */

	nLen = nPos2 - nPos;
	++nLen;
	if ( nLen > INET_PORT_STRLEN_MAX )
	{
		return -6;
	}
	memcpy( szPort, &szSrc[nPos], nLen*sizeof(TCHAR) );	

	nRet = 0;
	nRet = xt_str2_unum( szPort, (void*)nPort, sizeof(*nPort), 10 );
	if ( nRet < 0 )
	{
		return -7;
	}

	return FUN_RET_OK;
}

int compose_ip_port( TCHAR *szDst, const TCHAR *szIP, USHORT *nPort )
{
	if ( szDst==NULL || szIP==NULL )
	{
		return FUN_RET_ERR_ILLEGAL_ARGS;
	}

	switch( ip_str_ver(szIP) )
	{
	case AF_INET:
		_sntprintf( szDst, INET_ADDRSTRLEN_MAX, _T("%s:%d"), szIP, *nPort );
		break;
	case AF_INET6:
		_sntprintf( szDst, INET_ADDRSTRLEN_MAX, _T("[%s]:%d"), szIP, *nPort );
		break;
	default:
		szDst[0] = 0;
		break;
	}
	
	return _tcslen(szDst);
}

static int fill_unix_addr_struct( SOCKADDR *pAddr, SOCKLEN_T *pAddrLen, const TCHAR *szPath, int nPathLen )
{
#if defined(__LINUX__)
	if ( pAddr==NULL || pAddrLen==NULL || szPath==NULL || nPathLen<=0 )
	{
		return FUN_RET_ERR_ILLEGAL_ARGS;
	}

	if ( *pAddrLen < sizeof(SOCKADDR_UN) )
	{
		return -3;
	}

	if ( nPathLen > UNIX_PATH_MAX_ )
	{
		return -4;
	}

	CHAR *pTmp = (CHAR*)szPath;
#if defined(_TCHAR_DEFINED)
	CHAR szPathTmp[UNIX_PATH_MAX_] = {0};
	if ( wcstombs(szPathTmp, szPath, nPathLen) != nPathLen )
	{
		return -5;
	}
	pTmp = szPathTmp;
#endif
	memset( pAddr, 0, *pAddrLen );
	pAddr->sun_family = AF_UNIX;
	strncpy( pAddr->sun_path, pTmp, nPathLen );

	*pAddrLen = sizeof(pAddr->sun_family) + strlen(pAddr->sun_path);
	return FUN_RET_OK;
#else
	return ERR_SOCK_AF_NOT_SURPPORT;
#endif
}

static int extract_unix_addr_struct( const SOCKADDR *pAddr, SOCKLEN_T nAddrLen, TCHAR *szPath, int *nPathLen )
{
#if defined(__LINUX__)
	if ( pAddr==NULL || pAddrLen==NULL || szPath==NULL || *nPathLen<=0 )
	{
		return FUN_RET_ERR_ILLEGAL_ARGS;
	}

	if ( pAddr->sun_family != AF_UNIX )
	{
		return -3;
	}

	int nStrLen = strlen( pAddr->sun_path );
	if ( nStrLen+1 > UNIX_PATH_MAX_ )
	{
		return -4;
	}

	TCHAR *pTmp = (TCHAR*)pAddr->sun_path;
#if defined(_TCHAR_DEFINED)
	TCHAR szPathTmp[UNIX_PATH_MAX_] = {0};
	if ( mbstowcs( szPathTmp, pAddr->sun_path, nStrLen) != nStrLen )
	{
		return -5;
	}
	pTmp = szPathTmp;
#endif
	memset( szPath, 0, *nPathLen );
	strncpy( pTmp, pAddr->sun_path, nStrLen );
	*nPathLen = nStrLen;

	return FUN_RET_OK;
#else
	return ERR_SOCK_AF_NOT_SURPPORT;
#endif
}

int	xt_net_saddr_cmp( const SOCKADDR *saddr1, const SOCKADDR *saddr2 )
{
	/* sa_family == AF_INET */
	int nRet = 0;
#define saddr_in4(_arg)					((SOCKADDR_IN*)(_arg))

	nRet = saddr_in4(saddr1)->sin_addr.s_addr - saddr_in4(saddr2)->sin_addr.s_addr; /* Compare IP Address */
	if ( nRet == 0 )
	{
		/* Compare port number */
		nRet = saddr_in4(saddr1)->sin_port - saddr_in4(saddr2)->sin_port;
	}

#undef saddr_in4

	return nRet;
}

int	xt_net_saddr_in6_cmp( const SOCKADDR *saddr1, const SOCKADDR *saddr2 )
{
	/* sa_family == AF_INET6 */
#define saddr_in6(_arg)					((SOCKADDR_IN6*)(_arg))
#undef saddr_in6

	return FUN_RET_OK;
}

#if defined(__LINUX__)
int	xt_net_saddr_un_cmp( const SOCKADDR *saddr1, const SOCKADDR *saddr2 )
{
	/* sa_family == AF_UNIX */
#define saddr_un(_arg)					((SOCKADDR_UN*)(_arg))
#undef saddr_un

	return FUN_RET_OK;
}
#endif /*__LINUX__*/

/************************************************************************/
/************************************************************************/
#if defined(__WINDOWS__)
int xt_ioctlsockt( socket_fd_t nSocket, LONG cmd, ULONG *pArg )
{
	if ( ioctlsocket(nSocket,cmd,pArg) == SOCKET_ERROR )
	{
		int nErr = WSAGetLastError();
		switch( nErr )
		{
		case WSANOTINITIALISED:
		case WSAENETDOWN:
		case WSAEINPROGRESS:
		case WSAENOTSOCK:
		case WSAEFAULT:
		default:
			break;
		}

		return -3;
	}

	return FUN_RET_OK;
}
#endif

UINT get_sock_available( socket_fd_t nSocket )
{
#if defined(__LINUX__)
	return get_file_available(nSocket);
#elif defined(__WINDOWS__)
	ULONG arg = 0;
	ioctlsocket( nSocket, FIONREAD, &arg );
	return arg;
#endif
}

BOOL get_sock_blocking( socket_fd_t nSocket )
{
#if defined(__LINUX__)
	return get_file_blocking(nSocket);
#elif defined(__WINDOWS__)
	return FALSE;
#endif
}

int set_sock_nonblocking( socket_fd_t nSocket, BOOL on )
{
#if defined(__LINUX__)
	return set_file_noblocking(nSocket, on);
#elif defined(__WINDOWS__)
	ULONG arg = ((on)?1:0);
	return xt_ioctlsockt( nSocket, FIONREAD, &arg );
#endif
}


// //非阻塞connect实现
// int conn_nonb(int sockfd, const struct sockaddr_in *saptr, socklen_t salen, int nsec)
// {
//     int flags, n, error, code;
//     socklen_t len;
//     fd_set wset;
//     struct timeval tval;
// 
//     flags = fcntl(sockfd, F_GETFL, 0);
//     fcntl(sockfd, F_SETFL, flags | O_NONBLOCK);
// 
//     error = 0;
//     if ((n == connect(sockfd, saptr, salen)) == 0) {
//         goto done;
//     } else if (n < 0 && errno != EINPROGRESS){
//         return (-1);
//     }
// 
//     /* Do whatever we want while the connect is taking place */
// 
//     FD_ZERO(&wset);
//     FD_SET(sockfd, &wset);
//     tval.tv_sec = nsec;
//     tval.tv_usec = 0;
// 
//     if ((n = select(sockfd+1, NULL, &wset, 
//                     NULL, nsec ? &tval : NULL)) == 0) {
//         close(sockfd);  /* timeout */
//         errno = ETIMEDOUT;
//         return (-1);
//     }
// 
//     if (FD_ISSET(sockfd, &wset)) {
//         len = sizeof(error);
//         code = getsockopt(sockfd, SOL_SOCKET, SO_ERROR, &error, &len);
//         /* 如果发生错误，Solaris实现的getsockopt返回-1，
//          * 把pending error设置给errno. Berkeley实现的
//          * getsockopt返回0, pending error返回给error. 
//          * 我们需要处理这两种情况 */
//         if (code < 0 || error) {
//             close(sockfd);
//             if (error) 
//                 errno = error;
//             return (-1);
//         }
//     } else {
//         fprintf(stderr, "select error: sockfd not set");
//         exit(0);
//     }
// 
// done:
//     fcntl(sockfd, F_SETFL, flags);  /* restore file status flags */
//     return (0);
// }
