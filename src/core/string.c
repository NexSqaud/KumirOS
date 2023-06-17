#include <string.h>

#include <debug.h>
#include <memory.h>

static const char conversionTable[] = "0123456789ABCDEF";

String stringNew(uint32_t length)
{
    return (String)
    {
        .buffer = malloc(length),
            .count = 0,
            .length = length
    };
}

String stringNewFromOld(const char* str)
{
    uint32_t length = stringLength(str);
    char* buffer = malloc(length);

    for (uint32_t i = 0; i < length; i++)
    {
        buffer[i] = str[i];
    }

    return (String)
    {
        .buffer = buffer,
            .count = length,
            .length = length
    };
}

void stringRealloc(String* str, uint32_t newLength)
{
    if (str == NULL)
    {
        return;
    }

    char* buffer = malloc(newLength);

    for (uint32_t i = 0; i < str->count; i++)
    {
        buffer[i] = str->buffer[i];
    }

    free(str->buffer);

    str->buffer = buffer;
    str->length = newLength;
}

void stringAppend(String* str1, String str2)
{
    if (str1 == NULL)
    {
        return;
    }

    if (stringRemainingLength(str1) < str2.count)
    {
        uint32_t additionalLength = str2.count - stringRemainingLength(str1);
        stringRealloc(str1, str1->length + additionalLength);
    }

    for (uint32_t i = 0; i < str2.count; i++)
    {
        str1->buffer[str1->count + i] = str2.buffer[i];
    }

    str1->count += str2.count;
}

void stringAppendChar(String* str, char ch)
{
    if (stringRemainingLength(str) == 0)
    {
        stringRealloc(str, str->length + 1);
    }

    str->buffer[str->count] = ch;
    str->count++;
}

void stringAppendPtr(String* str1, const char* str2)
{
    String newStr2 = stringNewFromOld(str2);
    stringAppend(str1, newStr2);
}

uint32_t stringRemainingLength(const String* str)
{
    return str->length - str->count;
}

bool stringIsEqual(String* str1, String* str2)
{
    DEBUG_PRINTF("Comparing \"%ns\" and \"%ns\"", str1, str2);

    if (str1->count != str2->count) return false;

    for (uint32_t i = 0; i < str1->count; i++)
    {
        if (str1->buffer[i] != str2->buffer[i]) return false;
    }

    return true;
}

bool srtingIsEqualConst(String* strNew, const char* strOld)
{
    String oldToNew = stringNewFromOld(strOld);
    bool isEqual = stringIsEqual(strNew, &oldToNew);
    stringFree(oldToNew);
    return isEqual;
}

void stringClear(String* str)
{
    str->count = 0;
}

void stringFree(String str)
{
    free(str.buffer);
}

char* stringifyDec(uint64_t value)
{
    char* buffer = malloc(21);
    buffer[20] = 0;

    uint64_t n, d = 1000000000000000000;
    uint32_t i = 0;

    while ((value / d == 0) && (d >= 10)) {
        d /= 10;
    }
    n = value;

    while (d >= 9) {
        buffer[i] = conversionTable[n / d];
        n = n % d;
        d /= 10;
        i++;
    }
    buffer[i] = conversionTable[n];

    return buffer;
}

char* stringifyHex(uint64_t value)
{
    char* buffer = malloc(21);
    buffer[20] = 0;

    uint64_t n, d = 0x1000000000000000;
    uint32_t i = 0;

    while ((value / d == 0) && (d >= 0x10)) {
        d /= 0x10;
    }
    n = value;

    while (d >= 0xF) {
        buffer[i] = conversionTable[n / d];
        n = n % d;
        d /= 0x10;
        i++;
    }
    buffer[i] = conversionTable[n];

    return buffer;
}

bool stringIsEqualOld(const char* str1, const char* str2)
{
    while (*str1 && *str2)
    {
        if (*str1 != *str2)
        {
            return false;
        }
    }

    if (*str1 == *str2)
    {
        return true;
    }

    return false;
}

uint32_t stringLength(const char* str)
{
    if (str == NULL) return 0;

    uint32_t length = 0;
    while (*str++) length++;
    return length;
}
