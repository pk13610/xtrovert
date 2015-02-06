static SOCKET_ID init_async_tcp_sock( const TCHAR *addr_local, USHORT *af )
{
	SOCKET_ID fd = INVALID_SOCKET;
	int ret = FUN_RET_OK;
	USHORT family = AF_UNSPEC;

	SOCKADDR_STORAGE saddr;
	SOCKLEN_T sock_len = sizeof(saddr);

	family = addr_str_af( addr_local );
	if ( fill_sock_addr_struct((SOCKADDR*)&saddr, &sock_len, addr_local, family) < 0 )
	{
		return INVALID_SOCKET;
	}

	fd = xt_socket( family, SOCK_STREAM, 0 );
	if ( set_sock_nonblocking( fd, TRUE ) < 0 )
	{
		xt_closesock( fd );
		return INVALID_SOCKET;
	}

	ret = xt_bind( fd, (SOCKADDR*)&saddr, sock_len );
	if ( ret < 0 )
	{
		goto lbl_err;
	}

	if ( af )
	{
		*af = family;
	}

	return fd;

lbl_err:
	if ( fd != INVALID_SOCKET )
	{
		xt_closesock( fd );
	}

	return INVALID_SOCKET;
}