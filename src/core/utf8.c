#include <utf8.h>

#include <debug.h>
#include <stddef.h>
#include <string.h>
#include <wstring.h>

#define UTF8_MASK8(char) ((uint8_t)(0xFF & (char)))

size_t utf8ToWChar(const char* utf8, size_t inputSize, wchar_t* output, size_t outputSize)
{
    uint8_t* start = (uint8_t*)utf8;
    if (inputSize == SIZE_MAX)
    {
        inputSize = stringLength(utf8);
    }

    uint8_t* end = start + inputSize;

    size_t destLength = utf8Distance(start, end);

    if (destLength > outputSize)
    {
        if (outputSize != 0)
        {
            *output = 0;
        }

        return destLength + 1;
    }

    if (sizeof(wchar_t) == 4)
    {
        utf8ToUtf32(start, end, (uint32_t*)output, outputSize);
    }
    else
    {
        // TODO: utf8ToUtf16
    }

    return destLength + 1;
}

size_t wcharToUtf8(const wchar_t* buffer, size_t inputSize, char* output, size_t outputSize)
{
    const wchar_t* start = buffer;
    if (inputSize == SIZE_MAX)
    {
        inputSize = wideStringLength(buffer);
    }

    const wchar_t* end = start + inputSize;

    if (sizeof(wchar_t) == 4)
    {
        return utf32ToUtf8((uint32_t*)start, (uint32_t*)end, (uint8_t*)output, outputSize);
    }
    else
    {
        // TODO: utf16ToUtf8
    }

    return 0;
}

size_t utf8ToUtf32(uint8_t* start, uint8_t* end, uint32_t* output, size_t outputSize)
{
    uint32_t* it = output;

    for (; start < end; it++)
    {
        *it = utf8Next(&start, &outputSize);
    }

    *it = 0;

    return (size_t)(output - it);
}

size_t utf32ToUtf8(uint32_t* start, uint32_t* end, uint8_t* output, size_t outputSize)
{
    uint8_t* it = output;

    for (; start != end; start++)
    {
        it = utf8Append(*start, it, &outputSize);
    }

    if (outputSize != 0)
    {
        *it = 0;
    }

    it++;
    return (size_t)(output - it);
}

size_t utf8Distance(uint8_t* start, uint8_t* end)
{
    size_t result;
    for (result = 0; start < end; result++)
    {
        utf8Next(&start, NULL);
    }
    return result;
}

uint8_t* utf8Append(uint32_t codePoint, uint8_t* result, size_t* remain)
{
    size_t charLength = utf32CodepointLength(codePoint);

    if (*remain < charLength)
    {
        *remain = 0;
        return result + charLength;
    }

    if (codePoint < 0x80)
    {
        *(result++) = (uint8_t)codePoint;
    }
    else if (codePoint < 0x800)
    {
        *(result++) = (uint8_t)((codePoint >> 6) | 0xC0);
        *(result++) = (uint8_t)((codePoint & 0x3F) | 0x80);
    }
    else if (codePoint < 0x10000)
    {
        *(result++) = (uint8_t)((codePoint >> 12) | 0xE0);
        *(result++) = (uint8_t)(((codePoint >> 6) & 0x3F) | 0x80);
        *(result++) = (uint8_t)((codePoint & 0x3F) | 0x80);
    }
    else
    {
        *(result++) = (uint8_t)((codePoint >> 18) | 0xF0);
        *(result++) = (uint8_t)(((codePoint >> 12) & 0x3F) | 0x80);
        *(result++) = (uint8_t)(((codePoint >> 6) & 0x3F) | 0x80);
        *(result++) = (uint8_t)((codePoint & 0x3F) | 0x80);
    }

    return result;
}

uint32_t utf8Next(uint8_t** it, uint8_t* remain)
{
    uint32_t codePoint = UTF8_MASK8(**it);
    size_t length = utf8SequenceLength(*it);

    if (remain)
    {
        *remain -= length;
    }

    switch (length)
    {
    case 1:
        break;
    case 2:
        (*it)++;
        codePoint = ((codePoint << 6) & 0x7FF) + (UTF8_MASK8(**it) & 0x3F);
        break;
    case 3:
        (*it)++;
        codePoint = ((codePoint << 12) & 0xFFFF) + ((UTF8_MASK8(**it) << 6) & 0xFFF);
        (*it)++;
        codePoint += (**it) & 0x3F;
        break;
    case 4:
        (*it)++;
        codePoint = ((codePoint << 18) & 0x1FFFFF) + ((UTF8_MASK8(**it) << 12) & 0x3FFFF);
        (*it)++;
        codePoint += (UTF8_MASK8(**it) << 6) & 0xFFF;
        (*it)++;
        codePoint += (**it) & 0x3F;
        break;
    }
    (*it)++;
    return codePoint;
}

size_t utf8SequenceLength(uint8_t* sequence)
{
    uint8_t lead = UTF8_MASK8(*sequence);
    if (lead < 0x80)
    {
        return 1;
    }
    else if ((lead >> 5) == 0x6)
    {
        return 2;
    }
    else if ((lead >> 4) == 0xe)
    {
        return 3;
    }
    else if ((lead >> 3) == 0x1e)
    {
        return 4;
    }
    return 0;
}

size_t utf32CodepointLength(uint32_t codePoint)
{
    if (codePoint < 0x80)
    {
        return 1;
    }
    else if (codePoint < 0x800)
    {
        return 2;
    }
    else if (codePoint < 0x10000)
    {
        return 3;
    }
    else
    {
        return 4;
    }
}