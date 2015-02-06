/************************************************************************************
* TcpConnector.h : header file
*
* Class Definition header
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

#ifndef __TCP_CONNECTOR_H__
#define __TCP_CONNECTOR_H__

#include "NetReactor.h"

class CTcpConnector : public CNetEvtHandler
{
public:
	CTcpConnector(VOID);
	virtual ~CTcpConnector(VOID);

	virtual INT		Init( CNetReactor *pReactor )
	{
		m_pBe = bufferevent_socket_new( pReactor, fd, BEV_OPT_CLOSE_ON_FREE );

		if (!m_pBe) {
			fprintf(stderr, "Error constructing bufferevent!");
			event_base_loopbreak(base);
			return;
		}
		bufferevent_setcb( m_pBe, OnRead, OnWrite, OnClose, NULL );
		bufferevent_enable( m_pBe, EV_WRITE );
		bufferevent_disable( m_pBe, EV_READ );
	}

protected:

	virtual VOID		OnRead() = 0;
	virtual VOID		OnWrite() = 0;
	virtual VOID		OnTimeout() = 0;
	virtual VOID		OnClose() = 0;

protected:
	struct bufferevent				*m_pBe;
private:
};

#endif /*__TCP_CONNECTOR_H__*/
