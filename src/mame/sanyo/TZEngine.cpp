#include "pico/stdlib.h"

#include "TZEngine.h"

TZStack TZEngine::m_stack;


bool TZEngine::ExecuteOpCode(OpCode opcode)
{
    switch (opcode)
    {
        case OpCode::Drop : Drop(); break;
        case OpCode::Swap : Swap(); break;
        case OpCode::Dup : Dup(); break;
        case OpCode::Drop2 : Drop2(); break;
        case OpCode::Swap2 : Swap2(); break;
        case OpCode::Dup2 : Dup2(); break;
        case OpCode::DropFloat : DropFloat(); break;
        case OpCode::SwapFloat : SwapFloat(); break;
        case OpCode::DupFloat : DupFloat(); break;
        case OpCode::Clear : Clear(); break;
        case OpCode::Depth8 : Depth8(); break;
        case OpCode::Mul_8x8 : Mul_8x8(); break;
        case OpCode::Mul_16x8 : Mul_16x8(); break;
        case OpCode::Div_16_8 : Div_16_8(); break;
        case OpCode::LShift_8 : LShift_8(); break;
        case OpCode::LShift_16 : LShift_16(); break;
        case OpCode::RShift_8 : RShift_8(); break;
        case OpCode::RShift_16 : RShift_16(); break;
        case OpCode::Conv_8_float : Conv_8_float(); break;
        case OpCode::Conv_16_float : Conv_16_float(); break;
        case OpCode::Float_Minus : Float_Minus(); break;
        case OpCode::Float_Mul_8 : Float_Mul_8(); break;
        case OpCode::Float_Mul_Float : Float_Mul_Float(); break;
        default : return false;
    }
    return true;
}
    

void TZEngine::Drop2()
{
    m_stack.Drop();
    m_stack.Drop();
}

void TZEngine::Swap2()
{
    uint16_t val1 = m_stack.Pop16();
    uint16_t val2 = m_stack.Pop16();
    Push(val1);
    Push(val2);
}

void TZEngine::Dup2()
{
    uint16_t val = m_stack.Pop16();
    Push(val);
    Push(val);
}


void TZEngine::DropFloat()
{
    m_stack.Drop();
    m_stack.Drop();
    m_stack.Drop();
    m_stack.Drop();
}

void TZEngine::SwapFloat()
{
    float val1 = m_stack.PopFloat();
    float val2 = m_stack.PopFloat();
    Push(val1);
    Push(val2);
}

void TZEngine::DupFloat()
{
    float val = m_stack.PopFloat();
    Push(val);
    Push(val);
}

void TZEngine::Clear()
{
    m_stack.Clear();
}

void TZEngine::Depth8()
{
    Push((uint8_t)m_stack.GetSize());
}

void TZEngine::Mul_8x8()
{
    if (GetStackSize() >= 2)
    {
        Push((uint16_t)((uint16_t)Pop8() * (uint16_t)Pop8()));
    }
}

void TZEngine::Mul_16x8()
{
    if (GetStackSize() >= 3)
    {
        Push((uint16_t)(((uint16_t)Pop8()) * ((uint16_t)Pop8())));
    }
}

void TZEngine::Div_16_8()
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

void TZEngine::LShift_8()
{
    if (GetStackSize() >= 2)
    {
        uint8_t shift = Pop8();
        Push((uint8_t)(((uint8_t)Pop8()) << shift));
    }
}

void TZEngine::LShift_16()
{
    if (GetStackSize() >= 3)
    {
        uint8_t shift = Pop8();
        Push((uint16_t)(Pop16() << shift));
    }
}

void TZEngine::RShift_8()
{
    if (GetStackSize() >= 2)
    {
        uint8_t shift = Pop8();
        Push((uint8_t)(((uint8_t)Pop8()) >> shift));
    }
}

void TZEngine::RShift_16()
{
    if (GetStackSize() >= 3)
    {
        uint8_t shift = Pop8();
        Push((uint16_t)(Pop16() >> shift));
    }
}

void TZEngine::Conv_8_float()
{
    if (GetStackSize() >= 1)
    {
        Push((float)Pop8());
    }
}

void TZEngine::Conv_16_float()
{
    if (GetStackSize() >= 1)
    {
        Push((float)(int16_t)Pop16());
    }
}

void TZEngine::Float_Minus()
{
    if (GetStackSize() >= 4)
    {
        Push(PopFloat()*(-1));
    }
}

void TZEngine::Float_Mul_8()
{
    if (GetStackSize() >= 4 + 1)
    {
        Push(((float)Pop8()) * PopFloat());
    }
}

void TZEngine::Float_Mul_Float()
{
    if (GetStackSize() >= 4 + 4)
    {
        Push(PopFloat() * PopFloat());
    }
}
