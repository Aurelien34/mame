#include <stdio.h>
#include "TZDecompress.h"

void* TZDecompress::SzAlloc(ISzAllocPtr, size_t size)
{ 
    return new uint8_t[size]; 
}

void TZDecompress::SzFree(ISzAllocPtr, void* address)
{ 
    if (address)
    {
        delete[] static_cast<uint8_t*>(address); 
    }
}

TZDecompress::TZDecompress(const uint8_t* pSrc, size_t len) 
    : m_pSource(pSrc), m_remainingSourceBytes(len), m_bIsValid(false) 
{
    if (len < 1) return;

    Lzma2Dec_Construct(&m_deccompressor);
    m_allocInfo.Alloc = SzAlloc;
    m_allocInfo.Free = SzFree;

    
    // Extract properties byte
    uint8_t prop;
    if (pSrc[0] >= 0x80 || pSrc[0] == 0x01 || pSrc[0] == 0x02)
    {
        prop = 0;
    }
    else
    {
        prop = pSrc[0];
        m_pSource++;
        m_remainingSourceBytes--;
    }

    // Allocate dictionary (ex: 64 KB, depending on compression options)
    if (Lzma2Dec_Allocate(&m_deccompressor, prop, &m_allocInfo) == SZ_OK)
    {
        Lzma2Dec_Init(&m_deccompressor);
        m_bIsValid = true;
    }
}

TZDecompress::~TZDecompress()
{
    if (m_bIsValid) {
        Lzma2Dec_Free(&m_deccompressor, &m_allocInfo);
    }
}

bool TZDecompress::GetNextByte(uint8_t& out_byte)
{
    if (!m_bIsValid || m_remainingSourceBytes == 0) return false;

    SizeT dest_len = 1;
    SizeT src_chunk_len = m_remainingSourceBytes;
    ELzmaStatus status;

    SRes res = Lzma2Dec_DecodeToBuf(
        &m_deccompressor, 
        &out_byte, &dest_len, 
        m_pSource, &src_chunk_len, 
        LZMA_FINISH_ANY, &status
    );

    m_pSource += src_chunk_len;
    m_remainingSourceBytes -= src_chunk_len;

    return (res == SZ_OK && dest_len == 1);
}
