#ifndef __TZENGINE_H__
#define __TZENGINE_H__

#include "TZStack.h"
#include <stdint.h>

class TZEngine
{
public:
    enum class OpCode : uint8_t
    {
        //ENUM_START
        Drop = 0,
        Swap = 1,
        Dup = 2,
        Drop2 = 3,
        Swap2 = 4,
        Dup2 = 5,
        DropFloat = 6,
        SwapFloat = 7,
        DupFloat = 8,
        Clear = 9,
        Depth8 = 10,

        Mul_8x8 = 20,
        Mul_16x8 = 21,
        Div_16_8 = 22,
        LShift_8 = 23,
        LShift_16 = 24,
        RShift_8 = 25,
        RShift_16 = 26,

        Conv_8_float = 30,
        Conv_16_float = 31,
        Float_Minus = 32,
        Float_Mul_8 = 33,
        Float_Mul_Float = 34
        //ENUM_END
    };

    static void SetPushHandler(TZStackPushHandler handler) { m_stack.SetPushHandler(handler); }
    static inline void Push(uint8_t val) { m_stack.Push(val); }
    static inline void Push(uint16_t val) { m_stack.Push(val); }
    static inline void Push(float val) { m_stack.Push(val); }
    static uint8_t Pop8() { return m_stack.Pop8(); }
    static inline uint8_t Pop8Fast() { return m_stack.Pop8Fast(); }
    static uint16_t Pop16() { return m_stack.Pop16(); }
    static inline uint16_t Pop16Fast() { return m_stack.Pop16Fast(); }
    static float PopFloat() { return m_stack.PopFloat(); }
    static inline int GetStackSize() { return m_stack.GetSize(); }
    static inline bool IsEmpty() { return m_stack.IsEmpty(); }
    static bool ExecuteOpCode(OpCode opcode);
    // Stack operations
    static inline void Drop() { m_stack.Drop(); } // Expects one argument - drop it
    static inline void Swap() { m_stack.Swap(); } // Expects 2 aguments - swaps them
    static inline void Dup() { m_stack.Dup(); } // Expects 1 argument - duplicate it
    static void Drop2(); // Expects two arguments - drop tgem
    static void Swap2(); // Expects 4 aguments - swaps 2 byte groups
    static void Dup2(); // Expects 4 arguments - duplicate 2 byte groups
    static void DropFloat(); // Expects one float argument (4 bytes) - drop it
    static void SwapFloat(); // Expects 2 float aguments (2x4 bytes) - swaps them
    static void DupFloat(); // Expects 1 float argument (4 bytes) - duplicate it
    static void Clear(); // Clears the stack
    static void Depth8(); // Pushed stack size (1 byte)
    // Arithmetic operations
    static void Mul_8x8(); // Expects 2 bytes - multiply them => 16 bits
    static void Mul_16x8(); // Expects 1 short and 1 byte - multiply them => 16 bits
    static void Div_16_8(); // Expects 1 short and 1 byte - divide short by byte => 16 bits
    static void LShift_8(); // Expects 1 byte and 1 byte - left shift the first second value times
    static void LShift_16(); // Expects 1 short and 1 byte - left shift the first second value times
    static void RShift_8(); // Expects 1 byte and 1 byte - right shift the first second value times
    static void RShift_16(); // Expects 1 short and 1 byte - right shift the first second value times
    // Float operations
    static void Conv_8_float(); // Expects 1 byte (unsigned) - convert it to a float
    static void Conv_16_float(); // Expects 1 signed short - convert it to a float
    static void Float_Minus(); // Expects 1 float - returns -1 * val
    static void Float_Mul_8(); // Expects 1 float and 1 byte - returns val * byte
    static void Float_Mul_Float(); // Expects 2 floats - multiplies them
protected:
    static TZStack m_stack;

#ifndef TEST_CONTEXT
public:
    static inline TZStack &GetStack() { return m_stack; }
#endif

};

#endif
