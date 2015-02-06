/************************************************************************************
* NetHostInfo.h : header file
*
* Socket for UDP protocol Class Definition header
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

#ifndef __NET_HOST_INFO_H__
#define __NET_HOST_INFO_H__

#include "NetDefine.h"

typedef struct _tagINET_HOST_INFO
{
	NET_SKT_TYPE	nType; /* NET_SKT_TYPE_TCP NET_SKT_TYPE_UDP NET_SKT_TYPE_RAW */
	USHORT			nFamily; /* AF_INET AF_INET6 AF_UNIX */
	TCHAR			szAddr[INET_ADDRSTRLEN_MAX];	/*IP String like 127.0.0.1 7788*/
}INET_HOST_INFO;

#endif /*__NET_HOST_INFO_H__*/
