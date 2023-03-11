#include "lzma457lib.h"

#include "../lzma457/CPP/Common/MyWindows.h"
#include "../lzma457/CPP/Common/MyInitGuid.h"

#include "../lzma457/CPP/7zip/Common/StreamUtils.h"
#include "../lzma457/CPP/7zip/Compress/LZMA/LZMADecoder.h"
#include "../lzma457/CPP/7zip/Compress/LZMA/LZMAEncoder.h"

#include "stringstream.h"

CLzma457::SProperties::SProperties()
{
	InitDefault();
}

void CLzma457::SProperties::InitDefault()
{
	A = -1;
	D = -1;
	FB = -1;
	MC = -1;
	LC = -1;
	LP = -1;
	PB = -1;
	MF = -1;
}

void CLzma457::SProperties::Normalize()
{
	if (A != 0/* && A != 1*/)
	{
		A = -1;	// 1
	}
	if (D < 0 || D > 30)
	{
		D = -1;	// 23
	}
	if (FB < 5 || FB > 273)
	{
		FB = -1;	// 128
	}
	if (MC < 0)
	{
		MC = -1;	// 0
	}
	if (LC < 0 || LC > 8)
	{
		LC = -1;	// 3
	}
	if (LP < 0 || LP > 4)
	{
		LP = -1;	// 0
	}
	if (PB < 0 || PB > 4)
	{
		PB = -1;	// 2
	}
	if (MF <= kMatchFinderNone || MF >= kMatchFinderMax)
	{
		MF = -1;	// kMatchFinderBT4
	}
}

bool CLzma457::Compress(const std::string& a_sUncompressedData, std::string& a_sCompressedData, SProperties a_Properties /* = SProperties() */)
{
	a_Properties.Normalize();

	bool bDictionaryIsDefined = false;
	UInt32 uDictionary = static_cast<UInt32>(-1);
	if (a_Properties.D != -1)
	{
		UInt32 uDicLog = a_Properties.D;
		uDictionary = 1 << uDicLog;
		bDictionaryIsDefined = true;
	}
	std::wstring sMF = L"BT4";
	if (a_Properties.MF != -1)
	{
		switch (a_Properties.MF)
		{
		case kMatchFinderBT2:
			sMF = L"BT2";
			break;
		case kMatchFinderBT3:
			sMF = L"BT3";
			break;
		case kMatchFinderBT4:
			sMF = L"BT4";
			break;
		case kMatchFinderHC4:
			sMF = L"HC4";
			break;
		}
	}

	UInt32 uNumThreads = 1;

	CMyComPtr<ISequentialInStream> pInStream = new CStringReferenceInStream(a_sUncompressedData);

	CMyComPtr<ISequentialOutStream> pOutStream = new CStringReferenceOutStream(a_sCompressedData);

	UInt64 uFileSize = 0;

	NCompress::NLZMA::CEncoder *pEncoderSpec = new NCompress::NLZMA::CEncoder;
	CMyComPtr<ICompressCoder> pEncoder = pEncoderSpec;

	if (!bDictionaryIsDefined)
	{
		uDictionary = 1 << 23;
	}

	UInt32 uPosStateBits = 2;
	UInt32 uLitContextBits = 3; // for normal files
	// UInt32 uLitContextBits = 0; // for 32-bit data
	UInt32 uLitPosBits = 0;
	// UInt32 uLitPosBits = 2; // for 32-bit data
	UInt32 uAlgorithm = 1;
	UInt32 uNumFastBytes = 128;
	UInt32 uMatchFinderCycles = 16 + uNumFastBytes / 2;
	bool bMatchFinderCyclesDefined = false;

	bool bEos = false;

	if (a_Properties.A != -1)
	{
		uAlgorithm = a_Properties.A;
	}

	if (a_Properties.FB != -1)
	{
		uNumFastBytes = a_Properties.FB;
	}
	bMatchFinderCyclesDefined = a_Properties.MC != -1;
	if (bMatchFinderCyclesDefined)
	{
		uMatchFinderCycles = a_Properties.MC;
	}
	if (a_Properties.LC != -1)
	{
		uLitContextBits = a_Properties.LC;
	}
	if (a_Properties.LP != -1)
	{
		uLitPosBits = a_Properties.LP;
	}
	if (a_Properties.PB != -1)
	{
		uPosStateBits = a_Properties.PB;
	}

	PROPID uPropIDs[] =
	{
		NCoderPropID::kDictionarySize,
		NCoderPropID::kPosStateBits,
		NCoderPropID::kLitContextBits,
		NCoderPropID::kLitPosBits,
		NCoderPropID::kAlgorithm,
		NCoderPropID::kNumFastBytes,
		NCoderPropID::kMatchFinder,
		NCoderPropID::kEndMarker,
		NCoderPropID::kNumThreads,
		NCoderPropID::kMatchFinderCycles,
	};
	const int kNumPropsMax = sizeof(uPropIDs) / sizeof(uPropIDs[0]);

	PROPVARIANT properties[kNumPropsMax];
	for (int p = 0; p < 6; p++)
	{
		properties[p].vt = VT_UI4;
	}

	properties[0].ulVal = static_cast<UInt32>(uDictionary);
	properties[1].ulVal = static_cast<UInt32>(uPosStateBits);
	properties[2].ulVal = static_cast<UInt32>(uLitContextBits);
	properties[3].ulVal = static_cast<UInt32>(uLitPosBits);
	properties[4].ulVal = static_cast<UInt32>(uAlgorithm);
	properties[5].ulVal = static_cast<UInt32>(uNumFastBytes);

	properties[6].vt = VT_BSTR;
	properties[6].bstrVal = const_cast<BSTR>(static_cast<const wchar_t*>(sMF.c_str()));

	properties[7].vt = VT_BOOL;
	properties[7].boolVal = bEos ? VARIANT_TRUE : VARIANT_FALSE;

	properties[8].vt = VT_UI4;
	properties[8].ulVal = static_cast<UInt32>(uNumThreads);

	// it must be last in property list
	properties[9].vt = VT_UI4;
	properties[9].ulVal = static_cast<UInt32>(uMatchFinderCycles);

	int nNumProps = kNumPropsMax;
	if (!bMatchFinderCyclesDefined)
	{
		nNumProps--;
	}

	if (pEncoderSpec->SetCoderProperties(uPropIDs, properties, nNumProps) != S_OK)
	{
		return false;
	}
	pEncoderSpec->WriteCoderProperties(pOutStream);

	if (bEos)
	{
		uFileSize = static_cast<UInt64>(static_cast<Int64>(-1));
	}
	else
	{
		uFileSize = a_sUncompressedData.size();
	}

	for (int i = 0; i < 8; i++)
	{
		Byte b = Byte(uFileSize >> (8 * i));
		if (pOutStream->Write(&b, 1, 0) != S_OK)
		{
			return false;
		}
	}
	HRESULT nResult = pEncoder->Code(pInStream, pOutStream, 0, 0, 0);
	if (nResult == E_OUTOFMEMORY)
	{
		return false;
	}
	else if (nResult != S_OK)
	{
		return false;
	}

	return true;
}

bool CLzma457::Uncompress(const std::string& a_sCompressedData, std::string& a_sUncompressedData)
{
	CMyComPtr<ISequentialInStream> pInStream = new CStringReferenceInStream(a_sCompressedData);

	CMyComPtr<ISequentialOutStream> pOutStream = new CStringReferenceOutStream(a_sUncompressedData);

	NCompress::NLZMA::CDecoder* pDecoderSpec = new NCompress::NLZMA::CDecoder;
	CMyComPtr<ICompressCoder> pDecoder = pDecoderSpec;
	const UInt32 kPropertiesSize = 5;
	Byte uProperties[kPropertiesSize];
	UInt32 uProcessedSize = 0;
	if (ReadStream(pInStream, uProperties, kPropertiesSize, &uProcessedSize) != S_OK)
	{
		return false;
	}
	if (uProcessedSize != kPropertiesSize)
	{
		return false;
	}
	if (pDecoderSpec->SetDecoderProperties2(uProperties, kPropertiesSize) != S_OK)
	{
		return false;
	}
	UInt64 uFileSize = 0;
	for (int i = 0; i < 8; i++)
	{
		Byte b = 0;
		if (pInStream->Read(&b, 1, &uProcessedSize) != S_OK)
		{
			return false;
		}
		if (uProcessedSize != 1)
		{
			return false;
		}
		uFileSize |= static_cast<UInt64>(b) << (8 * i);
	}
	if (pDecoder->Code(pInStream, pOutStream, 0, &uFileSize, 0) != S_OK)
	{
		return false;
	}

	return true;
}

bool CLzma457::GetProperties(const std::string& a_sCompressedData, SProperties& a_Properties)
{
	if (a_sCompressedData.size() < 5)
	{
		return false;
	}
	a_Properties.InitDefault();
	a_Properties.LC = a_sCompressedData[0] % 9;
	int nRemainder = static_cast<unsigned char>(a_sCompressedData[0]) / 9;
	a_Properties.LP = nRemainder % 5;
	a_Properties.PB = nRemainder / 5;
	if (a_Properties.PB > 4)
	{
		return false;
	}
	unsigned int uDictionarySize = 0;
	for (int i = 0; i < 4; i++)
	{
		uDictionarySize += static_cast<unsigned int>(a_sCompressedData[i + 1]) << (i * 8);
	}
	int nD = -1;
	while (uDictionarySize != 0)
	{
		nD++;
		uDictionarySize >>= 1;
	}
	if (nD < 0 || nD > 30)
	{
		return false;
	}
	a_Properties.D = nD;
	return true;
}

CLzma457::CLzma457()
{
}
