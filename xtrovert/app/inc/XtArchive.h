/************************************************************************************
* XtArchive.h : header file
*
* header
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
#ifndef __XTARCHIVE_H__
#define __XTARCHIVE_H__

#include "xtrovert.h"
#include "XtCmdTarget.h"

#if defined(__WINDOWS__)
#pragma warning (disable: 4200) /* Ignore:  warning C4200: nonstandard extension used : zero-sized array in struct/union */
#endif

typedef struct _tag_xt_archive_state_t
{
	struct time_t	_ctime;          // creation date/time of file
	struct time_t	_mtime;          // last modification date/time of file
	struct time_t	_atime;          // last access date/time of file
	xt_size_t		size;            // logical size of file in bytes
	BYTE			_attribute;       // logical OR of CFile::Attribute enum values
	BYTE			_padding;        // pad the structure to a WORD
	TCHAR			_address[_MAX_PATH]; // absolute path name
}xt_archive_state_t;

/* Backend: Std-IO, File, Database, Network, Memory/Share-Memory) */
class CXtArchiveObj : public CXtObject
{
public:

	// Flag values
	enum OpenFlags {
		modeRead =         (int) 0x00000,
		modeWrite =        (int) 0x00001,
		modeReadWrite =    (int) 0x00002,
		shareCompat =      (int) 0x00000,
		shareExclusive =   (int) 0x00010,
		shareDenyWrite =   (int) 0x00020,
		shareDenyRead =    (int) 0x00030,
		shareDenyNone =    (int) 0x00040,
		modeNoInherit =    (int) 0x00080,
		modeCreate =       (int) 0x01000,
		modeNoTruncate =   (int) 0x02000,
		typeText =         (int) 0x04000, // typeText and typeBinary are
		typeBinary =       (int) 0x08000, // used in derived classes only
		osNoBuffer =       (int) 0x10000,
		osWriteThrough =   (int) 0x20000,
		osRandomAccess =   (int) 0x40000,
		osSequentialScan = (int) 0x80000,
	};

	enum Attribute {
		normal =    0x00,
		readOnly =  0x01,
		hidden =    0x02,
		system =    0x04,
		volume =    0x08,
		directory = 0x10,
		archive =   0x20
	};

	enum BufferCommand { bufferRead=0, bufferWrite, bufferCommit, bufferCheck };
	enum BufferFlags 
	{ 
		bufferDirect = 0x01,
		bufferBlocking = 0x02
	};

	enum SeekPosition { begin = 0x0, current = 0x1, end = 0x2 };

	CXtArchiveObj(void);
	virtual ~CXtArchiveObj(void);

	virtual xt_size_t			GetPosition(void) const;
	BOOL						GetStatus( xt_archive_state_t &rStatus ) const;

	// Operations
	virtual int					Open( const TCHAR *szAddress, UINT nOpenFlags );
	/// <summary>
	/// This static function deletes the file specified by the path.</summary>
	/// <param name="lpszFileName">A string that is the path to the desired file. The path can be relative or absolute.</param>
	/// <param name="pTM">Pointer to CAtlTransactionManager object</param>

	static void XT_STDECL		Remove( const TCHAR *szAddress ); 
	/// <summary>
	/// This method retrieves status information related to a given CFile object instance or a given file path.</summary>
	/// <returns> 
	/// TRUE if succeeds; otherwise FALSE.</returns>
	/// <param name="lpszFileName">A string that is the path to the desired file. The path can be relative or absolute.</param>
	/// <param name="rStatus">A reference to a user-supplied CFileStatus structure that will receive the status information.</param>
	/// <param name="pTM">Pointer to CAtlTransactionManager object</param>

	static BOOL XT_STDECL		GetStatus( const TCHAR *szAddress, xt_archive_state_t &rStatus );

	xt_size_t					SeekToEnd(void);
	void						SeekToBegin(void);

	virtual int					Read( void *pBuf, xt_size_t nBufSize );
	virtual int					Write( const void *pBuf, xt_size_t nSize );

	virtual void				Abort(void);
	virtual int					Flush(void);
	virtual void				Close(void);

	virtual UINT				GetBufferPtr( UINT nCommand, UINT nCount=0, void **ppBufStart=NULL, void **ppBufMax=NULL );

protected:

	virtual void				Reset(void);

	BOOL						m_bDirectBuffer;
	BOOL						m_bBlocking;

private:
};

class CXtArchive
{
public:

	enum XT_ARCHIVE_MODE { store=0, load=1, bNoFlushOnDelete=2, bNoByteSwap=4 };

	CXtArchive(void);
	CXtArchive( CXtArchiveObj *pArchiveObj, UINT nMode, int nBufSize, void *pBuf );
	~CXtArchive(void);
	/* Attributes */
	BOOL						IsLoading(void) const;
	BOOL						IsStoring(void) const;
	BOOL						IsBufferEmpty(void) const;

	UINT						GetObjectSchema(void);
	UINT						SetObjectSchema( UINT nSchema );

	/* Operations */
	xt_size_t					Read( void *pBuf, xt_size_t nBufSize );
	xt_size_t					Write( const void *pBuf, xt_size_t nSize );
	int							Flush(void);
	void						Close(void);
	void						Abort(void);

	// Object I/O is pointer based to avoid added construction overhead.
	// Use the Serialize member function directly for embedded objects.
	CXtArchive& XT_STDECL operator<<( CXtArchive &ar, const CObject *pOb );

	CXtArchive& XT_STDECL operator>>( CXtArchive &ar, CObject *&pOb );
	CXtArchive& XT_STDECL operator>>( CXtArchive &ar, const CObject *&pOb );

	// insertion operations
	CXtArchive& operator<<(BYTE by);
	CXtArchive& operator<<(WORD w);
	CXtArchive& operator<<(LONG l);
	CXtArchive& operator<<(DWORD dw);
	CXtArchive& operator<<(float f);
	CXtArchive& operator<<(double d);
	CXtArchive& operator<<(LONGLONG dwdw);
	CXtArchive& operator<<(ULONGLONG dwdw);

	CXtArchive& operator<<(int i);
	CXtArchive& operator<<(short w);
	CXtArchive& operator<<(char ch);
#ifdef _NATIVE_WCHAR_T_DEFINED
	CXtArchive& operator<<(wchar_t ch);
#endif
	CXtArchive& operator<<(unsigned u);

	CXtArchive& operator<<(bool b);

	// extraction operations
	CXtArchive& operator>>(BYTE &by);
	CXtArchive& operator>>(WORD &w);
	CXtArchive& operator>>(DWORD &dw);
	CXtArchive& operator>>(LONG &l);
	CXtArchive& operator>>(float &f);
	CXtArchive& operator>>(double &d);
	CXtArchive& operator>>(LONGLONG &dwdw);
	CXtArchive& operator>>(ULONGLONG &dwdw);

	CXtArchive& operator>>(int &i);
	CXtArchive& operator>>(short &w);
	CXtArchive& operator>>(char &ch);
#ifdef _NATIVE_WCHAR_T_DEFINED
	CXtArchive& operator>>(wchar_t &ch);
#endif
	CXtArchive& operator>>(unsigned &u);
	CXtArchive& operator>>(bool &b);

protected:

	virtual void				Reset(void);

	CXtObject*					ReadObject( const CXtRuntimeClass *pClass );
	void						WriteObject( const CXtObject *pObj );

	CXtRuntimeClass*			ReadClass( const CXtRuntimeClass *pClassRequested=NULL, UINT *pSchema=NULL, DWORD *pObTag=NULL );
	void						SerializeClass( const CXtRuntimeClass *pClassRef );

	CXtArchiveObj				*m_pBackend;
	/* Version control to objects for serializing operation. */
	UINT						m_nObjectSchema;

private:
};

#endif /*__XTARCHIVE_H__*/
