#include "TZIOWrapper.h"

void TZIOWrapper::Setup()
{
    
}

uint8_t TZIOWrapper::PortRead(TZIOWrapper::Port port)
{
    if (port == TZIOWrapper::Port::Data)
    {
        // Data should have already been sent to the PIO FIFO at the stack level (via the push handler)
        return m_engine.Pop8(); 
    }
    else
    {
        // Room for future state
        return 0;
    }
}

bool TZIOWrapper::PortWrite(Port port, uint8_t val)
{
    if (port == TZIOWrapper::Port::Data)
    {
        m_engine.Push(val);
        return true;
    }
    else
    {
        return m_engine.ExecuteOpCode((TZEngine::OpCode)val);
    }
}
