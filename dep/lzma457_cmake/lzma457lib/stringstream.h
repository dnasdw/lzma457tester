#ifndef LZMA457LIB_STRINGSTREAM_H_
#define LZMA457LIB_STRINGSTREAM_H_

#include "../lzma457/CPP/Common/MyWindows.h"
#include "../lzma457/CPP/Common/MyInitGuid.h"

#include "../lzma457/CPP/7zip/IStream.h"
#include "../lzma457/CPP/Common/MyCom.h"

#include <string>

class CStringReferenceInStream
	: public ISequentialInStream
	, public CMyUnknownImp
{
public:
	MY_UNKNOWN_IMP
public:
	CStringReferenceInStream(const std::string& a_sString);
	virtual ~CStringReferenceInStream();
	STDMETHOD(Read)(void* a_pData, UInt32 a_uSize, UInt32* a_pProcessedSize);
private:
	const std::string& m_sString;
	const std::string::size_type m_uSize;
	std::string::size_type m_uPos;
};

class CStringReferenceOutStream
	: public ISequentialOutStream
	, public CMyUnknownImp
{
public:
	MY_UNKNOWN_IMP
public:
	CStringReferenceOutStream(std::string& a_sString);
	virtual ~CStringReferenceOutStream();
	STDMETHOD(Write)(const void* a_pData, UInt32 a_uSize, UInt32* a_pProcessedSize);
private:
	std::string& m_sString;
	std::string::size_type m_uSize;
};

#endif	// LZMA457LIB_STRINGSTREAM_H_
