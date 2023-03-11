set(sdw_lzma457_add_definitions_UNIX_OR_MINGW "-Wno-logical-op-parentheses" PARENT_SCOPE)

set(sdw_lzma457_src
  "${CMAKE_CURRENT_SOURCE_DIR}/lzma457/C/7zCrc.c"
  "${CMAKE_CURRENT_SOURCE_DIR}/lzma457/C/7zCrc.h"
  "${CMAKE_CURRENT_SOURCE_DIR}/lzma457/C/Alloc.c"
  "${CMAKE_CURRENT_SOURCE_DIR}/lzma457/C/Alloc.h"
  "${CMAKE_CURRENT_SOURCE_DIR}/lzma457/C/IStream.h"
  "${CMAKE_CURRENT_SOURCE_DIR}/lzma457/C/Types.h"

  "${CMAKE_CURRENT_SOURCE_DIR}/lzma457/C/Compress/Lz/LzHash.h"
  "${CMAKE_CURRENT_SOURCE_DIR}/lzma457/C/Compress/Lz/MatchFinder.c"
  "${CMAKE_CURRENT_SOURCE_DIR}/lzma457/C/Compress/Lz/MatchFinder.h"

  "${CMAKE_CURRENT_SOURCE_DIR}/lzma457/CPP/7zip/ICoder.h"
  "${CMAKE_CURRENT_SOURCE_DIR}/lzma457/CPP/7zip/IDecl.h"
  "${CMAKE_CURRENT_SOURCE_DIR}/lzma457/CPP/7zip/IStream.h"

  "${CMAKE_CURRENT_SOURCE_DIR}/lzma457/CPP/7zip/Common/InBuffer.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/lzma457/CPP/7zip/Common/InBuffer.h"
  "${CMAKE_CURRENT_SOURCE_DIR}/lzma457/CPP/7zip/Common/OutBuffer.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/lzma457/CPP/7zip/Common/OutBuffer.h"
  "${CMAKE_CURRENT_SOURCE_DIR}/lzma457/CPP/7zip/Common/StdAfx.h"
  "${CMAKE_CURRENT_SOURCE_DIR}/lzma457/CPP/7zip/Common/StreamUtils.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/lzma457/CPP/7zip/Common/StreamUtils.h"

  "${CMAKE_CURRENT_SOURCE_DIR}/lzma457/CPP/7zip/Compress/LZ/LZOutWindow.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/lzma457/CPP/7zip/Compress/LZ/LZOutWindow.h"
  "${CMAKE_CURRENT_SOURCE_DIR}/lzma457/CPP/7zip/Compress/LZ/StdAfx.h"

  "${CMAKE_CURRENT_SOURCE_DIR}/lzma457/CPP/7zip/Compress/LZMA/LZMA.h"
  "${CMAKE_CURRENT_SOURCE_DIR}/lzma457/CPP/7zip/Compress/LZMA/LZMADecoder.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/lzma457/CPP/7zip/Compress/LZMA/LZMADecoder.h"
  "${CMAKE_CURRENT_SOURCE_DIR}/lzma457/CPP/7zip/Compress/LZMA/LZMAEncoder.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/lzma457/CPP/7zip/Compress/LZMA/LZMAEncoder.h"
  "${CMAKE_CURRENT_SOURCE_DIR}/lzma457/CPP/7zip/Compress/LZMA/StdAfx.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/lzma457/CPP/7zip/Compress/LZMA/StdAfx.h"

  "${CMAKE_CURRENT_SOURCE_DIR}/lzma457/CPP/7zip/Compress/RangeCoder/RangeCoder.h"
  "${CMAKE_CURRENT_SOURCE_DIR}/lzma457/CPP/7zip/Compress/RangeCoder/RangeCoderBit.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/lzma457/CPP/7zip/Compress/RangeCoder/RangeCoderBit.h"
  "${CMAKE_CURRENT_SOURCE_DIR}/lzma457/CPP/7zip/Compress/RangeCoder/RangeCoderBitTree.h"
  "${CMAKE_CURRENT_SOURCE_DIR}/lzma457/CPP/7zip/Compress/RangeCoder/RangeCoderOpt.h"
  "${CMAKE_CURRENT_SOURCE_DIR}/lzma457/CPP/7zip/Compress/RangeCoder/StdAfx.h"

  "${CMAKE_CURRENT_SOURCE_DIR}/lzma457/CPP/Common/CRC.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/lzma457/CPP/Common/Defs.h"
  "${CMAKE_CURRENT_SOURCE_DIR}/lzma457/CPP/Common/MyCom.h"
  "${CMAKE_CURRENT_SOURCE_DIR}/lzma457/CPP/Common/MyException.h"
  "${CMAKE_CURRENT_SOURCE_DIR}/lzma457/CPP/Common/MyGuidDef.h"
  "${CMAKE_CURRENT_SOURCE_DIR}/lzma457/CPP/Common/MyInitGuid.h"
  "${CMAKE_CURRENT_SOURCE_DIR}/lzma457/CPP/Common/MyUnknown.h"
  "${CMAKE_CURRENT_SOURCE_DIR}/lzma457/CPP/Common/MyWindows.h"
  "${CMAKE_CURRENT_SOURCE_DIR}/lzma457/CPP/Common/NewHandler.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/lzma457/CPP/Common/NewHandler.h"
  "${CMAKE_CURRENT_SOURCE_DIR}/lzma457/CPP/Common/StdAfx.h"
  "${CMAKE_CURRENT_SOURCE_DIR}/lzma457/CPP/Common/Types.h"

  "${CMAKE_CURRENT_SOURCE_DIR}/CMakeLists.txt"

  PARENT_SCOPE
)
