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
        _Drop = 0,
        _Swap = 1,
        _Dup = 2,
        _Drop2 = 3,
        _Swap2 = 4,
        _Dup2 = 5,
        _DropFloat = 6,
        _SwapFloat = 7,
        _DupFloat = 8,
        _Clear = 9,
        u8_Depth = 10,
        u16_Depth = 11,

        u16_Mul_u8_u8 = 20,
        u16_Mul_u16_u8 = 21,
        u16_Div_u16_u8 = 22,
        u8_LShift_u8_u8 = 23,
        u16_LShift_u16_u8 = 24,
        u8_RShift_u8_u8 = 25,
        u16_RShift_u16_u8 = 26,

        fl_Conv_u8_float = 30,
        fl_Conv_u16_float = 31,
        fl_Minus_fl = 32,
        fl_Mul_fl_u8 = 33,
        fl_Mul_fl_fl = 34,

        _lzma2_Store_content_u16 = 35,
        u16_lzma2_DecompressAll = 36
        //ENUM_END
    };

    static void SetPushHandler(TZStackPushHandler handler) { m_stack.SetPushHandler(handler); }
    static inline void Push(uint8_t val) { m_stack.Push(val); }
    static inline void Push(uint16_t val) { m_stack.Push(val); }
    static inline void Push(float val) { m_stack.Push(val); }
    static uint8_t Pop8() { return m_stack.Pop8(); }
    static uint16_t Pop16() { return m_stack.Pop16(); }
    static float PopFloat() { return m_stack.PopFloat(); }
    static inline int GetStackSize() { return m_stack.GetSize(); }
    static inline bool IsEmpty() { return m_stack.IsEmpty(); }
    static bool ExecuteOpCode(OpCode opcode);
    // Stack operations
    static inline void _Drop() { m_stack.Drop(); } // Expects one argument - drop it
    static inline void _Swap() { m_stack.Swap(); } // Expects 2 aguments - swaps them
    static inline void _Dup() { m_stack.Dup(); } // Expects 1 argument - duplicate it
    static void _Drop2(); // Expects two arguments - drop tgem
    static void _Swap2(); // Expects 4 aguments - swaps 2 byte groups
    static void _Dup2(); // Expects 4 arguments - duplicate 2 byte groups
    static void _DropFloat(); // Expects one float argument (4 bytes) - drop it
    static void _SwapFloat(); // Expects 2 float aguments (2x4 bytes) - swaps them
    static void _DupFloat(); // Expects 1 float argument (4 bytes) - duplicate it
    static void _Clear(); // Clears the stack
    static void u8_Depth(); // Pushed stack size (1 byte)
    static void u16_Depth(); // Pushed stack size (2 bytes)
    // Arithmetic operations
    static void u16_Mul_u8_u8(); // Expects 2 bytes - multiply them => 16 bits
    static void u16_Mul_u16_u8(); // Expects 1 short and 1 byte - multiply them => 16 bits
    static void u16_Div_u16_u8(); // Expects 1 short and 1 byte - divide short by byte => 16 bits
    static void u8_LShift_u8_u8(); // Expects 1 byte and 1 byte - left shift the first second value times
    static void u16_LShift_u16_u8(); // Expects 1 short and 1 byte - left shift the first second value times
    static void u8_RShift_u8_u8(); // Expects 1 byte and 1 byte - right shift the first second value times
    static void u16_RShift_u16_u8(); // Expects 1 short and 1 byte - right shift the first second value times
    // Float operations
    static void fl_Conv_u8_float(); // Expects 1 byte (unsigned) - convert it to a float
    static void fl_Conv_u16_float(); // Expects 1 signed short - convert it to a float
    static void fl_Minus_fl(); // Expects 1 float - returns -1 * val
    static void fl_Mul_fl_u8(); // Expects 1 float and 1 byte - returns val * byte
    static void fl_Mul_fl_fl(); // Expects 2 floats - multiplies them
    // Lzma2 operations
    static void _lzma2_Store_content_u16(); // Store content on the stack as LZMA2 compressed data - size on the top of the stack
    static void u16_lzma2_DecompressAll(); // Decompress the whole file and return final size
protected:
    static TZStack m_stack;
    static uint8_t * m_pLzma2File;
    static uint16_t m_nLzma2FileLength;

#ifndef TEST_CONTEXT
public:
    static inline TZStack &GetStack() { return m_stack; }
    static inline uint8_t *GetLzma2File() { return m_pLzma2File; }
    static inline uint16_t GetLzma2FileLength() { return m_nLzma2FileLength; }
#endif

};

#endif
