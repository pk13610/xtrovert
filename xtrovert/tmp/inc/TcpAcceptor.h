/************************************************************************************
* TcpAcceptor.h : header file
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

#ifndef __TCP_ACCEPTOR_H__
#define __TCP_ACCEPTOR_H__

#include "TcpConnector.h"
// 
// unsigned long HashString(char *lpszFileName, unsigned long dwHashType)
// { 
// 	unsigned char *key = (unsigned char *)lpszFileName;
// 	unsigned long seed1 = 0x7FED7FED, seed2 = 0xEEEEEEEE;
// 	int ch;
// 
// 	while(*key != 0)
// 	{ 
// 		ch = toupper(*key++);
// 
// 		seed1 = cryptTable[(dwHashType << 8) + ch] ^ (seed1 + seed2);
// 		seed2 = ch + seed1 + seed2 + (seed2 << 5) + 3; 
// 	}
// 	return seed1; 
// }
// int GetHashTablePos(char *lpszString, SOMESTRUCTURE *lpTable, int nTableSize)
// { 
// 	int nHash = HashString(lpszString), nHashPos = nHash % nTableSize;
// 
// 	if (lpTable[nHashPos].bExists && !strcmp(lpTable[nHashPos].pString, lpszString)) 
// 		return nHashPos; 
// 	else 
// 		return -1; //Error value 
// } 
// int GetHashTablePos(char *lpszString, MPQHASHTABLE *lpTable, int nTableSize)
// { 
// 	const int HASH_OFFSET = 0, HASH_A = 1, HASH_B = 2;
// 	int nHash = HashString(lpszString, HASH_OFFSET);
// 	int nHashA = HashString(lpszString, HASH_A);
// 	int nHashB = HashString(lpszString, HASH_B);
// 	int nHashStart = nHash % nTableSize, nHashPos = nHashStart;
// 
// 	while (lpTable[nHashPos].bExists)
// 	{ 
// 		if (lpTable[nHashPos].nHashA == nHashA && lpTable[nHashPos].nHashB == nHashB) 
// 			return nHashPos; 
// 		else 
// 			nHashPos = (nHashPos + 1) % nTableSize;
// 
// 		if (nHashPos == nHashStart) 
// 			break; 
// 	}
// 
// 	return -1; //Error value 
// } 

typedef struct _tagTCP_SVR_CFG
{
	USHORT			nFamily; /* AF_INET AF_INET6 AF_UNIX */
	TCHAR			szAddNative[INET_ADDRSTRLEN_MAX];	/*IP String like 127.0.0.1:7788*/
}TCP_SVR_CFG;

typedef struct _tagINET_SVR_PARAM
{
	size_t				nMaxClientCount; // Connection count could be managed
	INT					nBackLog; // Listen Backlog count
}INET_SVR_PARAM;

typedef struct _tagCONNECTOR_POOL
{
	size_t			nConnCntMax;
	size_t			nConnCnt;
	CTcpConnector	**pConnIndicator;
	CTcpConnector	*pConnectors;
}CONNECTOR_POOL;

CONST size_t _TCP_ACCEPTOR_CONN_CUNT_DEF_  = _ELEMENT_COUNT_DEFAULT_;

class CTcpAcceptor : public CEventHandler
{
public:
	CTcpAcceptor(VOID);
	virtual ~CTcpAcceptor(VOID);

	virtual INT				Init( CONST TCP_SVR_CFG *pInfo=NULL, CONST INET_SVR_PARAM *pParam=NULL );
	virtual INT				Start(VOID);

	virtual BOOL			IsValid(VOID)					{ return (m_sktNative.IsValid()!=NULL); }

	virtual INT				Close(VOID);
	virtual INT				Release(VOID); // Release all client's connections and native memory

	virtual INT				KickClient( size_t nIndex );

	virtual VOID			RegisterReactor( CNetReactor *pReactor )					{ m_pReactor = pReactor; }

//	virtual INT				HandleEvents( INT nEvents );

//	virtual NET_SKT_TYPE	GetSocketType()					{ return m_cfgNetHost.nType; };

protected:
	virtual INT				OnRead(VOID);
	virtual INT				OnWrite(VOID);
	virtual INT				OnTimeout(VOID);
	virtual INT				OnClose(VOID);

	VOID					Reset(VOID); // Reset data members

	INT						AllocConnectors(VOID);
	CTcpConnector*			AddConnector( CExtSocketTcp *pSocket );
	INT						RemConnector( CTcpConnector *pConnector );

protected:
	SOCKADDR_STORAGE		m_AddrNative;
	CONNECTOR_POOL			m_ConnPool;
	CExtSocketTcp			m_sktNative;
	CNetReactor				*m_pReactor;
private:
};

#endif /*__TCP_ACCEPTOR_H__*/
