#include "stringstream.h"
#include <string.h>

CStringReferenceInStream::CStringReferenceInStream(const std::string& a_sString)
	: m_sString(a_sString)
	, m_uSize(a_sString.size())
	, m_uPos(0)
{
}

CStringReferenceInStream::~CStringReferenceInStream()
{
}

STDMETHODIMP CStringReferenceInStream::Read(void* a_pData, UInt32 a_uSize, UInt32* a_pProcessedSize)
{
	if (m_uPos + a_uSize > m_uSize)
	{
		a_uSize = m_uSize - m_uPos;
	}
	memcpy(a_pData, m_sString.c_str() + m_uPos, a_uSize);
	m_uPos += a_uSize;
	if (a_pProcessedSize != nullptr)
	{
		*a_pProcessedSize = a_uSize;
	}
	return S_OK;
}

CStringReferenceOutStream::CStringReferenceOutStream(std::string& a_sString)
	: m_sString(a_sString)
	, m_uSize(a_sString.size())
{
}

CStringReferenceOutStream::~CStringReferenceOutStream()
{
}

STDMETHODIMP CStringReferenceOutStream::Write(const void* a_pData, UInt32 a_uSize, UInt32* a_pProcessedSize)
{
	m_sString.append(reinterpret_cast<const char*>(a_pData), a_uSize);
	std::string::size_type newSize = m_sString.size();
	a_uSize = static_cast<unsigned int>(newSize - m_uSize);
	m_uSize = newSize;
	if (a_pProcessedSize != nullptr)
	{
		*a_pProcessedSize = a_uSize;
	}
	return S_OK;
}