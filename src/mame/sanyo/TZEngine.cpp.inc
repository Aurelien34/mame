#include "TZEngine.h"
#include "TZDecompress.h"

TZStack TZEngine::m_stack;
uint8_t * TZEngine::m_pLzma2File = 0;
uint16_t TZEngine::m_nLzma2FileLength = 0;

bool TZEngine::ExecuteOpCode(OpCode opcode)
{
    switch (opcode)
    {
        case OpCode::_Drop : _Drop(); break;
        case OpCode::_Swap : _Swap(); break;
        case OpCode::_Dup : _Dup(); break;
        case OpCode::_Drop2 : _Drop2(); break;
        case OpCode::_Swap2 : _Swap2(); break;
        case OpCode::_Dup2 : _Dup2(); break;
        case OpCode::_DropFloat : _DropFloat(); break;
        case OpCode::_SwapFloat : _SwapFloat(); break;
        case OpCode::_DupFloat : _DupFloat(); break;
        case OpCode::_Clear : _Clear(); break;
        case OpCode::u8_Depth : u8_Depth(); break;
        case OpCode::u16_Depth : u16_Depth(); break;
        case OpCode::u16_Mul_u8_u8 : u16_Mul_u8_u8(); break;
        case OpCode::u16_Mul_u16_u8 : u16_Mul_u16_u8(); break;
        case OpCode::u16_Div_u16_u8 : u16_Div_u16_u8(); break;
        case OpCode::u8_LShift_u8_u8 : u8_LShift_u8_u8(); break;
        case OpCode::u16_LShift_u16_u8 : u16_LShift_u16_u8(); break;
        case OpCode::u8_RShift_u8_u8 : u8_RShift_u8_u8(); break;
        case OpCode::u16_RShift_u16_u8 : u16_RShift_u16_u8(); break;
        case OpCode::fl_Conv_u8_float : fl_Conv_u8_float(); break;
        case OpCode::fl_Conv_u16_float : fl_Conv_u16_float(); break;
        case OpCode::fl_Minus_fl : fl_Minus_fl(); break;
        case OpCode::fl_Mul_fl_u8 : fl_Mul_fl_u8(); break;
        case OpCode::fl_Mul_fl_fl : fl_Mul_fl_fl(); break;
        case OpCode::_lzma2_Store_content_u16 : _lzma2_Store_content_u16(); break;
        case OpCode::u16_lzma2_DecompressAll : u16_lzma2_DecompressAll(); break;
        default : return false;
    }
    return true;
}
    

void TZEngine::_Drop2()
{
    m_stack.Drop();
    m_stack.Drop();
}

void TZEngine::_Swap2()
{
    uint16_t val1 = m_stack.Pop16();
    uint16_t val2 = m_stack.Pop16();
    Push(val1);
    Push(val2);
}

void TZEngine::_Dup2()
{
    uint16_t val = m_stack.Pop16();
    Push(val);
    Push(val);
}


void TZEngine::_DropFloat()
{
    m_stack.Drop();
    m_stack.Drop();
    m_stack.Drop();
    m_stack.Drop();
}

void TZEngine::_SwapFloat()
{
    float val1 = m_stack.PopFloat();
    float val2 = m_stack.PopFloat();
    Push(val1);
    Push(val2);
}

void TZEngine::_DupFloat()
{
    float val = m_stack.PopFloat();
    Push(val);
    Push(val);
}

void TZEngine::_Clear()
{
    m_stack.Clear();
}

void TZEngine::u8_Depth()
{
    Push((uint8_t)m_stack.GetSize());
}

void TZEngine::u16_Depth()
{
    Push((uint16_t)m_stack.GetSize());
}

void TZEngine::u16_Mul_u8_u8()
{
    if (GetStackSize() >= 2)
    {
        Push((uint16_t)((uint16_t)Pop8() * (uint16_t)Pop8()));
    }
}

void TZEngine::u16_Mul_u16_u8()
{
    if (GetStackSize() >= 3)
    {
        Push((uint16_t)(((uint16_t)Pop8()) * ((uint16_t)Pop16())));
    }
}

void TZEngine::u16_Div_u16_u8()
{
    if (GetStackSize() >= 3)
    {
        uint16_t divisor = (uint16_t)Pop8();
        if (divisor != 0)
        {
            Push((uint16_t)(((uint16_t)Pop16()) / divisor));
        }
        else
        {
            Push((uint16_t)0);
        }
    }
}

void TZEngine::u8_LShift_u8_u8()
{
    if (GetStackSize() >= 2)
    {
        uint8_t shift = Pop8();
        Push((uint8_t)(((uint8_t)Pop8()) << shift));
    }
}

void TZEngine::u16_LShift_u16_u8()
{
    if (GetStackSize() >= 3)
    {
        uint8_t shift = Pop8();
        Push((uint16_t)(Pop16() << shift));
    }
}

void TZEngine::u8_RShift_u8_u8()
{
    if (GetStackSize() >= 2)
    {
        uint8_t shift = Pop8();
        Push((uint8_t)(((uint8_t)Pop8()) >> shift));
    }
}

void TZEngine::u16_RShift_u16_u8()
{
    if (GetStackSize() >= 3)
    {
        uint8_t shift = Pop8();
        Push((uint16_t)(Pop16() >> shift));
    }
}

void TZEngine::fl_Conv_u8_float()
{
    if (GetStackSize() >= 1)
    {
        Push((float)Pop8());
    }
}

void TZEngine::fl_Conv_u16_float()
{
    if (GetStackSize() >= 1)
    {
        Push((float)(int16_t)Pop16());
    }
}

void TZEngine::fl_Minus_fl()
{
    if (GetStackSize() >= 4)
    {
        Push(PopFloat()*(-1));
    }
}

void TZEngine::fl_Mul_fl_u8()
{
    if (GetStackSize() >= 4 + 1)
    {
        Push(((float)Pop8()) * PopFloat());
    }
}

void TZEngine::fl_Mul_fl_fl()
{
    if (GetStackSize() >= 4 + 4)
    {
        Push(PopFloat() * PopFloat());
    }
}

void TZEngine::_lzma2_Store_content_u16()
{
    if (GetStackSize() >= 2)
    {
        uint16_t size = Pop16();
        if (size > 0)
        {
            if (m_pLzma2File != 0)
            {
                delete[] m_pLzma2File;
                m_nLzma2FileLength = 0;
            }
            if (size <= GetStackSize())
            {
                m_pLzma2File = new uint8_t[size];
                m_nLzma2FileLength = size;
                for (uint16_t i = size-1; i != 0xffff; --i)
                {
                    m_pLzma2File[i] = Pop8();
                }
            }
        }
    }
}

void TZEngine::u16_lzma2_DecompressAll()
{
    if (m_nLzma2FileLength > 0)
    {
        TZDecompress decompressor(m_pLzma2File, m_nLzma2FileLength);
        uint8_t out_byte;
        uint16_t size = 0;
        while (decompressor.GetNextByte(out_byte))
        {
            Push(out_byte);
            ++size;
        }
        m_stack.Reverse(size);
        Push((uint16_t)size);
    }
}