#ifndef __TZIOWRAPPER_H__
#define __TZIOWRAPPER_H__

#include "TZEngine.h"

class TZIOWrapper
{
public:
    enum class Port : uint8_t
    {
        Command = 0,
        Data = 1
    };
    static void Setup();
    static uint8_t PortRead(Port port);
    static bool PortWrite(Port port, uint8_t val);
private:
    static TZEngine m_engine;
};


#endif