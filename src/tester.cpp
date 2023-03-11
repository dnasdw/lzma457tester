#include <lzma457lib/lzma457lib.h>
#include <sdw.h>

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

	if (!CLzma457::Uncompress(sCompressedData, sUncompressedData))
	{
		return 1;
	}
	sCompressedData.swap(string(""));

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

	CLzma457::SProperties properties;
	properties.D = 19;
	properties.FB = 273;
	if (!CLzma457::Compress(sUncompressedData, sCompressedData, properties))
	{
		return 1;
	}
	sUncompressedData.swap(string(""));

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
