#include <CPP/Common/MyInitGuid.h>
#include <CPP/7zip/Common/StreamUtils.h>
#include <CPP/7zip/Compress/LZMA/LZMADecoder.h>
#include <CPP/7zip/Compress/LZMA/LZMAEncoder.h>
#include <sdw.h>

class CStringReferenceInStream
	: public ISequentialInStream
	, public CMyUnknownImp
{
public:
	MY_UNKNOWN_IMP
public:
	CStringReferenceInStream(const string& src)
		: mSrc(src)
		, mSize(src.size())
		, mPos(0)
	{
	}
	virtual ~CStringReferenceInStream()
	{
	}
	STDMETHOD(Read)(void *data, UInt32 size, UInt32 *processedSize)
	{
		if (mPos + size > mSize)
		{
			size = mSize - mPos;
		}
		memcpy(data, mSrc.c_str() + mPos, size);
		mPos += size;
		if (processedSize != nullptr)
		{
			*processedSize = size;
		}
		return S_OK;
	}
private:
	const string& mSrc;
	const string::size_type mSize;
	string::size_type mPos;
};

class CStringReferenceOutStream
	: public ISequentialOutStream
	, public CMyUnknownImp
{
public:
	MY_UNKNOWN_IMP
public:
	CStringReferenceOutStream(string& dest)
		: mDest(dest)
		, mSize(dest.size())
	{
	}
	virtual ~CStringReferenceOutStream()
	{
	}
	STDMETHOD(Write)(const void *data, UInt32 size, UInt32 *processedSize)
	{
		mDest.append(reinterpret_cast<const char*>(data), size);
		string::size_type newSize = mDest.size();
		size = static_cast<u32>(newSize - mSize);
		mSize = newSize;
		if (processedSize != nullptr)
		{
			*processedSize = size;
		}
		return S_OK;
	}
private:
	string& mDest;
	string::size_type mSize;
};

int uncompressFile(const UString& a_sInputFileName, const UString& a_sOutputFileName)
{
	FILE* fp = UFopen(a_sInputFileName, USTR("rb"), false);
	if (fp == nullptr)
	{
		return 1;
	}
	fseek(fp, 0, SEEK_END);
	u32 uFileSize = static_cast<u32>(ftell(fp));
	fseek(fp, 0, SEEK_SET);
	string sCompressedData;
	sCompressedData.resize(uFileSize);
	if (uFileSize != 0)
	{
		fread(&*sCompressedData.begin(), 1, uFileSize, fp);
	}
	fclose(fp);

	string sUncompressedData;

	CMyComPtr<ISequentialInStream> inStream = new CStringReferenceInStream(sCompressedData);

	CMyComPtr<ISequentialOutStream> outStream = new CStringReferenceOutStream(sUncompressedData);

	NCompress::NLZMA::CDecoder *decoderSpec = new NCompress::NLZMA::CDecoder;
	CMyComPtr<ICompressCoder> decoder = decoderSpec;
	const UInt32 kPropertiesSize = 5;
	Byte properties[kPropertiesSize];
	UInt32 processedSize;
	if (ReadStream(inStream, properties, kPropertiesSize, &processedSize) != S_OK)
	{
		return 1;
	}
	if (processedSize != kPropertiesSize)
	{
		return 1;
	}
	if (decoderSpec->SetDecoderProperties2(properties, kPropertiesSize) != S_OK)
	{
		return 1;
	}
	UInt64 fileSize = 0;
	for (int i = 0; i < 8; i++)
	{
		Byte b;
		if (inStream->Read(&b, 1, &processedSize) != S_OK)
		{
			return 1;
		}
		if (processedSize != 1)
		{
			return 1;
		}
		fileSize |= ((UInt64)b) << (8 * i);
	}
	if (decoder->Code(inStream, outStream, 0, &fileSize, 0) != S_OK)
	{
		return 1;
	}

	fp = UFopen(a_sOutputFileName, USTR("wb"), false);
	if (fp == nullptr)
	{
		return 1;
	}
	if (!sUncompressedData.empty())
	{
		fwrite(sUncompressedData.c_str(), 1, sUncompressedData.size(), fp);
	}
	fclose(fp);
	return 0;
}

int compressFile(const UString& a_sInputFileName, const UString& a_sOutputFileName)
{
	FILE* fp = UFopen(a_sInputFileName, USTR("rb"), false);
	if (fp == nullptr)
	{
		return 1;
	}
	fseek(fp, 0, SEEK_END);
	u32 uFileSize = static_cast<u32>(ftell(fp));
	fseek(fp, 0, SEEK_SET);
	string sUncompressedData;
	sUncompressedData.resize(uFileSize);
	if (uFileSize != 0)
	{
		fread(&*sUncompressedData.begin(), 1, uFileSize, fp);
	}
	fclose(fp);

	string sCompressedData;

	CMyComPtr<ISequentialInStream> inStream = new CStringReferenceInStream(sUncompressedData);

	CMyComPtr<ISequentialOutStream> outStream = new CStringReferenceOutStream(sCompressedData);

	UInt32 numThreads = 1;

	NCompress::NLZMA::CEncoder *encoderSpec = new NCompress::NLZMA::CEncoder;
	CMyComPtr<ICompressCoder> encoder = encoderSpec;

	UInt32 dictionary = 1 << 19;

	UInt32 posStateBits = 2;
	UInt32 litContextBits = 3; // for normal files
	// UInt32 litContextBits = 0; // for 32-bit data
	UInt32 litPosBits = 0;
	// UInt32 litPosBits = 2; // for 32-bit data
	UInt32 algorithm = 1;
	UInt32 numFastBytes = 128;
	UInt32 matchFinderCycles = 16 + numFastBytes / 2;
	bool matchFinderCyclesDefined = false;

	bool eos = false;

	numFastBytes = 273;

	PROPID propIDs[] =
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
	const int kNumPropsMax = sizeof(propIDs) / sizeof(propIDs[0]);

	PROPVARIANT properties[kNumPropsMax];
	for (int p = 0; p < 6; p++)
		properties[p].vt = VT_UI4;

	properties[0].ulVal = (UInt32)dictionary;
	properties[1].ulVal = (UInt32)posStateBits;
	properties[2].ulVal = (UInt32)litContextBits;
	properties[3].ulVal = (UInt32)litPosBits;
	properties[4].ulVal = (UInt32)algorithm;
	properties[5].ulVal = (UInt32)numFastBytes;

	properties[6].vt = VT_BSTR;
	properties[6].bstrVal = L"BT4";

	properties[7].vt = VT_BOOL;
	properties[7].boolVal = eos ? VARIANT_TRUE : VARIANT_FALSE;

	properties[8].vt = VT_UI4;
	properties[8].ulVal = (UInt32)numThreads;

	// it must be last in property list
	properties[9].vt = VT_UI4;
	properties[9].ulVal = (UInt32)matchFinderCycles;

	int numProps = kNumPropsMax;
	if (!matchFinderCyclesDefined)
		numProps--;

	if (encoderSpec->SetCoderProperties(propIDs, properties, numProps) != S_OK)
	{
		return 1;
	}
	encoderSpec->WriteCoderProperties(outStream);

	UInt64 fileSize = (UInt64)(Int64)uFileSize;

	for (int i = 0; i < 8; i++)
	{
		Byte b = Byte(fileSize >> (8 * i));
		if (outStream->Write(&b, 1, 0) != S_OK)
		{
			return 1;
		}
	}
	HRESULT result = encoder->Code(inStream, outStream, 0, 0, 0);
	if (result == E_OUTOFMEMORY)
	{
		return 1;
	}
	else if (result != S_OK)
	{
		return 1;
	}

	fp = UFopen(a_sOutputFileName, USTR("wb"), false);
	if (fp == nullptr)
	{
		return 1;
	}
	if (!sCompressedData.empty())
	{
		fwrite(sCompressedData.c_str(), 1, sCompressedData.size(), fp);
	}
	fclose(fp);
	return 0;
}

int UMain(int argc, UChar* argv[])
{
	if (argc != 4)
	{
		return 1;
	}
	if (UCslen(argv[1]) == 1)
	{
		switch (*argv[1])
		{
		case USTR('D'):
		case USTR('d'):
			return uncompressFile(argv[2], argv[3]);
		case USTR('E'):
		case USTR('e'):
			return compressFile(argv[2], argv[3]);
		default:
			break;
		}
	}
	return 1;
}
