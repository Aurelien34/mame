#ifndef __TZSTACK_H__
#define __TZSTACK_H__

#define TZ_STACK_MAX_SIZE 128

#include <stdint.h>

typedef void (*TZStackPushHandler)(uint8_t);

class TZStack
{
public:
    TZStack();
    void SetPushHandler(TZStackPushHandler handler) { m_pushHandler = handler; }
    void Push(uint8_t val);
    void Push(uint16_t val);
    void Push(float val);
    uint8_t Pop8();
    uint16_t Pop16();
    float PopFloat();

    inline int GetSize() { return m_topIndex; }
    inline bool IsEmpty() { return !GetSize(); }

    void Drop(); // Expects one argument - drop it
    void Swap(); // Expects 2 aguments - swaps them
    void Dup(); // Expects 1 argument - duplicate it
    void Clear(); // Clears the stack

protected:
    inline void PropagateTopValue() { if (m_pushHandler != 0) { if (m_topIndex > 0) m_pushHandler(m_aData[m_topIndex - 1]); else m_pushHandler(0); }}
    inline void PropagateTopValue(uint8_t val) { if (m_pushHandler != 0) { m_pushHandler(val); }}
    uint8_t m_aData[TZ_STACK_MAX_SIZE];
    int m_topIndex;
    TZStackPushHandler m_pushHandler;

#ifndef TEST_CONTEXT
public:
    inline uint8_t *GetData() { return m_aData; }
#endif

};

#endif
