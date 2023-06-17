#include <stddef.h>
#include <stdint.h>

#ifndef _STRING_H
#define _STRING_H

// TODO: move all strings to String
typedef struct
{
    char* buffer;
    uint32_t count;
    uint32_t length;
} String;

String stringNew(uint32_t length);
String stringNewFromOld(const char* str);
void stringRealloc(String* str, uint32_t newLength);

void stringAppend(String* str1, String str2);
void stringAppendChar(String* str, char ch);
void stringAppendPtr(String* str1, const char* str2);

uint32_t stringRemainingLength(const String* str);

bool stringIsEqual(String* str1, String* str2);
bool srtingIsEqualConst(String* strNew, const char* strOld);

void stringClear(String* str);
void stringFree(String str);

char* stringifyDec(uint64_t value);
char* stringifyHex(uint64_t value);

bool stringIsEqualOld(const char* str1, const char* str2);
uint32_t stringLength(const char* str);

#endif