#include <stdint.h>
#include <stddef.h>

#ifndef _WSTRING_H
#define _WSTRING_H

// TODO: move all wide strings to WString
typedef struct
{
    wchar_t* buffer;
    size_t count;
    size_t size;
} WString;

size_t wideStringLength(const wchar_t* str);
int32_t wideStringFind(const wchar_t* source, const wchar_t* pattern, size_t startIndex);
void wideStringReplace(const wchar_t* source, const wchar_t* before, const wchar_t* after, const wchar_t* buffer, bool each);

#endif