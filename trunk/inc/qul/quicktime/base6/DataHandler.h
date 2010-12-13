/**
 * === QuickTime Utility ===
 *  Unit Name:  DataHandler
 *  Author:     Lincoln Yu
 *  Purpose:	QuickTime Data-handler encapsulation
 * == History ==        
 *  2006-02-12:	Created.
 *  2006-02-24:	[:DataEasyEater:] and [:DataEasySpitter:] added.
 *
 */

#ifndef _DataHandler_H
#define _DataHandler_H

#include <qul/quicktime/base6/QTC_Def.h>
#include <qul/quicktime/base6/QTC_Util.h>
#include <qul/quicktime/base6/DataTypes.h>


_QTC_BEGIN

class DataHandler
	{
public:
	enum Mode
		{
		dhRead,
		dhWrite
		};

	DataHandler () : m_ciDataHandler(NULL)
		{
		}

	virtual ~DataHandler ()
		{
		Close();
		}

	Result OpenForRead (const ::DataHandler &dh)
		{
		Result err = noErr;
		CHECK_ERR(DataHOpenForRead(dh));
		m_ciDataHandler = dh;
	bail:
		return err;
		}

	Result OpenForRead (Handle hDataRef, OSType type)
		{
		Result err = noErr;

		CHECK_ERR(OpenAComponent(GetDataHandler(hDataRef, type, kDataHCanRead), &m_ciDataHandler));
		DataHSetDataRef(m_ciDataHandler, hDataRef);
		CHECK_ERR(DataHOpenForRead(m_ciDataHandler));

	bail:
		return err;
		}

	Result OpenForWrite (Handle hDataRef, OSType dataRefType, OSType creator, OSType fileType, 
		Boolean bDeleteExisting = false)
		{
		Result err = noErr;

		CHECK_ERR(OpenAComponent(GetDataHandler(hDataRef, dataRefType, kDataHCanWrite), &m_ciDataHandler));
		DataHSetDataRef(m_ciDataHandler, hDataRef);
		CHECK_ERR(DataHCreateFile(m_ciDataHandler, creator, bDeleteExisting));
		DataHSetMacOSFileType(m_ciDataHandler, fileType);
		CHECK_ERR(DataHOpenForWrite(m_ciDataHandler));

	bail:
		return err;
		}

	Result ScheduleData64 (Ptr pBuf, QT_Wide *pOffset, long size, long refCon = 0, 
		DataHSchedulePtr pScheduleRec = NULL, DataHCompletionUPP uppCompletion = NULL)
		{
		return DataHScheduleData64(m_ciDataHandler, pBuf, pOffset, size, refCon, pScheduleRec, uppCompletion);
		}

	Result ScheduleData (Ptr pBuf, long offset, long size, long refCon = 0, 
		DataHSchedulePtr pScheduleRec = NULL, DataHCompletionUPP uppCompletion = NULL)
		{
		return DataHScheduleData(m_ciDataHandler, pBuf, offset, size, refCon, pScheduleRec, uppCompletion);
		}

	Result Write64 (Ptr pBuf, QT_Wide *pOffset, long size, DataHCompletionUPP uppCompletion = NULL,
		long refCon = 0)
		{
		return DataHWrite64(m_ciDataHandler, pBuf, pOffset, size, uppCompletion, refCon);
		}

	Result Write (Ptr pBuf, long offset, long size, DataHCompletionUPP uppCompletion = NULL, long refCon = 0)
		{
		return DataHWrite(m_ciDataHandler, pBuf, offset, size, uppCompletion, refCon);
		}

	Result GetFileSize (long *pSize)
		{
		return DataHGetFileSize(m_ciDataHandler, pSize);
		}

	Result GetFileSize64 (QT_Wide *pSize)
		{
		return DataHGetFileSize64 (m_ciDataHandler, pSize);
		}

	void Close ()
		{
		if (m_ciDataHandler)
			CloseComponent(m_ciDataHandler);
		}

private:
	ComponentInstance	m_ciDataHandler;	

	};	// class DataHandler


class EasyDataEater : protected DataHandler
	{
	typedef DataHandler		_Mybase;

public:
	enum
		{
		kNegOffset = -2,
		kExceedEOF = -1,
		};
	
	EasyDataEater () : _Mybase()
		{
		}

	EasyDataEater (const ::DataHandler &dh) : _Mybase()
		{
		Open(dh);
		}

	EasyDataEater (Handle hDataRef, OSType type) : _Mybase()
		{
		Open(hDataRef, type);
		}

	Result Open (const ::DataHandler &dh)
		{
		Result err = noErr;
		CHECK_ERR(_Mybase::OpenForRead(dh));
		_Mybase::GetFileSize64(&m_FileSize);
		m_FileOffset = 0;
	bail:
		return err;
		}

	Result Open (Handle hDataRef, OSType type)
		{
		Result err = noErr;
		CHECK_ERR(_Mybase::OpenForRead(hDataRef, type));
		_Mybase::GetFileSize64(&m_FileSize);
		m_FileOffset = 0;
	bail:
		return err;
		}

	void Close ()
		{
		_Mybase::Close();
		}

	bool IsEof ()
		{
		return (m_FileOffset >= m_FileSize);
		}

	void Tell (QT_Wide &offset)
		{
		offset = m_FileOffset;
		}

	Result Seek(const QT_Wide &offset)
		{
		m_FileOffset = offset;
		return ValidateOffset();
		}

	Result SeekCur(long offset)
		{
		m_FileOffset += offset;
		return ValidateOffset();
		}

	Result SeekCur(const QT_Wide &offset)
		{
		m_FileOffset += offset;
		return ValidateOffset();
		}

	long ReadData (void *pBuf, long size)
		{
		Result err = noErr;
		if (m_FileOffset + size > m_FileSize)
			size = (long)(m_FileSize - m_FileOffset);
		CHECK_ERR(ScheduleData64((Ptr)pBuf, &m_FileOffset, size));
		m_FileOffset += size;
	bail:
		if (err != noErr)
			return -1;
		return size;
		}

private:
	Result ValidateOffset()
		{
		if (m_FileOffset < 0)
			{
			m_FileOffset = 0;
			return kNegOffset;
			}
		if (m_FileOffset > m_FileSize)
			{
			m_FileOffset = m_FileSize;
			return kExceedEOF;
			}
		return noErr;
		}

protected:
	QT_Wide	m_FileOffset, m_FileSize;

	};	// class EasyDataEater


class EasyDataSpitter : public DataHandler
	{
	typedef DataHandler		_Mybase;

public:
	EasyDataSpitter () : _Mybase()
		{
		}

	EasyDataSpitter (Handle hDataRef, OSType dataRefType, OSType creator, OSType fileType, 
		Boolean bDeleteExisting = false) : _Mybase()
		{
		Open(hDataRef, dataRefType, creator, fileType, bDeleteExisting);
		}

	Result Open (Handle hDataRef, OSType dataRefType, OSType creator, OSType fileType, 
		Boolean bDeleteExisting = false)
		{
		return _Mybase::OpenForWrite(hDataRef, dataRefType, creator, fileType, bDeleteExisting);
		}

	void Close ()
		{
		_Mybase::Close();
		}

	void Tell (QT_Wide &offset)
		{
		offset = m_FileOffset;
		}

	void Seek(const QT_Wide &offset)
		{
		m_FileOffset = offset;
		}

	void SeekCur(long offset)
		{
		m_FileOffset += offset;
		}

	void SeekCur(const QT_Wide &offset)
		{
		m_FileOffset += offset;
		}

	Result WriteData (Ptr pBuf, long size)
		{
		Result err = noErr;
		CHECK_ERR(Write64(pBuf, &m_FileOffset, size));
		m_FileOffset += size;
	bail:
		return err;
		}
	
protected:
	QT_Wide	m_FileOffset;

	};


_QTC_END

#endif	// ifndef _DataHandler_H