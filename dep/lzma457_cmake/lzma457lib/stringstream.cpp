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
	unsigned int uSize = static_cast<unsigned int>(m_uSize - m_uPos);
	if (a_uSize > uSize)
	{
		a_uSize = uSize;
	}
	memcpy(a_pData, m_sString.c_str() + m_uPos, a_uSize);
	m_uPos += a_uSize;
	if (a_pProcessedSize != NULL)
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
	std::string::size_type uNewSize = m_sString.size();
	a_uSize = static_cast<unsigned int>(uNewSize - m_uSize);
	m_uSize = uNewSize;
	if (a_pProcessedSize != NULL)
	{
		*a_pProcessedSize = a_uSize;
	}
	return S_OK;
}
