#include "TZStack.h"

TZStack::TZStack()
    : m_topIndex(0),
    m_pushHandler(0)
{

}

void TZStack::Push(uint8_t val)
{
    if (m_topIndex < TZ_STACK_MAX_SIZE)
    {
        m_aData[m_topIndex++] = val;
        PropagateTopValue();
    }
}

void TZStack::Push(uint16_t val)
{
    if (m_topIndex < TZ_STACK_MAX_SIZE - 1)
    {
        m_aData[m_topIndex++] = (uint8_t)val;
        m_aData[m_topIndex++] = (uint8_t)(val >> 8);
        PropagateTopValue();
    }
}

void TZStack::Push(float val)
{
    if (m_topIndex < TZ_STACK_MAX_SIZE - 3)
    {
        uint8_t *valptr = (uint8_t *)&val;
        m_aData[m_topIndex++] = valptr[0];
        m_aData[m_topIndex++] = valptr[1];
        m_aData[m_topIndex++] = valptr[2];
        m_aData[m_topIndex++] = valptr[3];
        PropagateTopValue();
    }
}

uint8_t TZStack::Pop8()
{
    if (m_topIndex > 0)
    {
        uint8_t val = m_aData[--m_topIndex];
        if (m_topIndex > 0)
        {
            PropagateTopValue(m_aData[m_topIndex-1]);
        }
        else
        {
            PropagateTopValue(0);
        }
        return val;
    }
    else
    {
        return 0;
    }
}

uint16_t TZStack::Pop16()
{
    if (m_topIndex > 1)
    {
        uint16_t val = m_aData[--m_topIndex] << 8;
        val += m_aData[--m_topIndex];
        PropagateTopValue(m_aData[m_topIndex]);
        return val;
    }
    else
    {
        return 0;
    }
}

float TZStack::PopFloat()
{
    if (m_topIndex > 3)
    {
        float val;
        uint8_t *valptr = (uint8_t *)&val;
        valptr[3] = m_aData[--m_topIndex];
        valptr[2] = m_aData[--m_topIndex];
        valptr[1] = m_aData[--m_topIndex];
        valptr[0] = m_aData[--m_topIndex];
        PropagateTopValue(valptr[0]);
        return val;
    }
    else
    {
        return 0;
    }
}

void TZStack::Drop()
{
    if (m_topIndex > 0)
    {
        m_topIndex--;
        PropagateTopValue();
    }
}

void TZStack::Swap()
{
    if (m_topIndex > 1)
    {
        uint8_t top = m_aData[m_topIndex - 1];
        m_aData[m_topIndex - 1] = m_aData[m_topIndex - 2];
        m_aData[m_topIndex - 2] = top;
        PropagateTopValue();
    }
}

void TZStack::Dup()
{
    if (m_topIndex > 0)
    {
        uint8_t top = m_aData[m_topIndex - 1];
        Push(top);
        PropagateTopValue();
    }
}

void TZStack::Clear()
{
    m_topIndex = 0;
    PropagateTopValue();
}
