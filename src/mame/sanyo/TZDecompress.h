#ifndef __TZ_DECOMPRESS_H__
#define __TZ_DECOMPRESS_H__

#ifndef ZIP7_INC_LZMA2_DEC_H
#include "Lzma2Dec.h"
#endif

#include <stdint.h>
#include <stdlib.h>

class TZDecompress
{
public:
    TZDecompress(const uint8_t* pSrc, size_t len);

    ~TZDecompress();

    // Disable object copy
    TZDecompress(const TZDecompress&) = delete;
    TZDecompress& operator=(const TZDecompress&) = delete;

    bool GetNextByte(uint8_t& out_byte);

private:
    CLzma2Dec m_deccompressor;
    ISzAlloc m_allocInfo;
    const uint8_t* m_pSource;
    size_t m_remainingSourceBytes;
    bool m_bIsValid;

    static void* SzAlloc(ISzAllocPtr, size_t size);
    static void SzFree(ISzAllocPtr, void* address);
};

#endif // __TZ_DECOMPRESS_H__