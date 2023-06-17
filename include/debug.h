#include <stdint.h>
#include <string.h>

#ifndef _DEBUG_H
#define _DEBUG_H

#define DEBUG_PRINT(x) \
    debugPrintFormated("[%cs:%d] %cs\n", __FILE__, __LINE__, x);

#define DEBUG_PRINTF(format, ...) \
    debugPrintFormated("[%cs:%d] " format "\n", __FILE__, __LINE__, __VA_ARGS__)

#define PRINT_REGISTER(x) \
    printString(stringify(x) ": "); \
    printHex(regs.x); \
    printString("\n")

void debugInit(void);
void debugPrintChar(char ch);
void debugPrintString(String* str);
void debugPrintDec(uint32_t value);
void debugPrintHex(uint32_t value);
void debugPrintFloat(float value, uint8_t afterPoint);
void debugPrintFormated(const char* format, ...);

#endif