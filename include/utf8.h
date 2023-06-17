#include <stdint.h>

#ifndef __UTF8_H
#define __UTF8_H

size_t utf8ToWChar(const char* utf8, size_t inputSize, wchar_t* output, size_t outputSize);
size_t wcharToUtf8(const wchar_t* buffer, size_t inputSize, char* output, size_t outputSize);
size_t utf8ToUtf32(uint8_t* start, uint8_t* end, uint32_t* output, size_t outputSize);
size_t utf32ToUtf8(uint32_t* start, uint32_t* end, uint8_t* output, size_t outputSize);
size_t utf8Distance(uint8_t* start, uint8_t* end);
uint8_t* utf8Append(uint32_t codePoint, uint8_t* result, size_t* remain);
uint32_t utf8Next(uint8_t** it, uint8_t* remain);
size_t utf8SequenceLength(uint8_t* sequence);
size_t utf32CodepointLength(uint32_t codePoint);

#endif