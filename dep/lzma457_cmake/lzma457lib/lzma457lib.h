#ifndef LZMA457LIB_LZMA457LIB_H_
#define LZMA457LIB_LZMA457LIB_H_

#include <string>

class CLzma457
{
public:
	enum EMatchFinder
	{
		kMatchFinderNone,

		kMatchFinderBT2,
		kMatchFinderBT3,
		kMatchFinderBT4,
		kMatchFinderHC4,

		kMatchFinderMax
	};
	struct SProperties
	{
		/**
		 * Algorithm
		 * algo
		 *   0 means fast method
		 *   1 means normal method
		 * [0, 1], default: 1 (max)
		 */
		int A;
		/**
		 * DictionarySize
		 * dictSize - The dictionary size in bytes. The maximum value is
		 *         128 MB = (1 << 27) bytes for 32-bit version
		 *           1 GB = (1 << 30) bytes for 64-bit version
		 *      The default value is 8 MB = (1 << 23) bytes.
		 *      It's recommended to use the dictionary that is larger than 4 KB and
		 *      that can be calculated as (1 << N) or (3 << N) sizes.
		 * [0,30], default: 23 (8MB)
		 */
		int D;
		/**
		 * NumFastBytes
		 * fb - Word size (the number of fast bytes).
		 *      It can be in the range from 5 to 273. The default value is 32.
		 *      Usually, a big number gives a little bit better compression ratio and
		 *      slower compression process.
		 * [5, 273], default: 128
		 */
		int FB;
		/**
		 * MatchFinderCycles
		 * mc
		 * default: 0
		 */
		int MC;
		/**
		 * LitContextBits
		 * lc - The number of literal context bits (high bits of previous literal).
		 *      It can be in the range from 0 to 8. The default value is 3.
		 *      Sometimes lc=4 gives the gain for big files.
		 * [0, 8], default: 3
		 */
		int LC;
		/**
		 * LitPosBits
		 * lp - The number of literal pos bits (low bits of current position for literals).
		 *      It can be in the range from 0 to 4. The default value is 0.
		 *      The lp switch is intended for periodical data when the period is equal to 2^lp.
		 *      For example, for 32-bit (4 bytes) periodical data you can use lp=2. Often it's
		 *      better to set lc=0, if you change lp switch.
		 * [0, 4], default: 0
		 */
		int LP;
		/**
		 * PosStateBits
		 * pb - The number of pos bits (low bits of current position).
		 *      It can be in the range from 0 to 4. The default value is 2.
		 *      The pb switch is intended for periodical data when the period is equal 2^pb.
		 * [0, 4], default: 2
		 */
		int PB;
		/**
		 * MatchFinder
		 * mf
		 * [bt2, bt3, bt4, hc4], default: bt4
		 */
		int MF;
		SProperties();
		void InitDefault();
		void Normalize();
	};
	static bool Compress(const std::string& a_sUncompressedData, std::string& a_sCompressedData, SProperties a_Properties = SProperties());
	static bool Uncompress(const std::string& a_sCompressedData, std::string& a_sUncompressedData);
	static bool GetProperties(const std::string& a_sCompressedData, SProperties& a_Properties);
private:
	CLzma457();
};

#endif	// LZMA457LIB_LZMA457LIB_H_
