#include <wstring.h>

#include <stddef.h>

size_t wideStringLength(const wchar_t* str)
{
    if (str == NULL) return 0;

    size_t length = 0;
    while (*str++) length++;
    return length;
}

int32_t wideStringFind(const wchar_t* source, const wchar_t* pattern, size_t startIndex)
{
    if(source == NULL || pattern == NULL) return;

    size_t patternLength = wideStringLength(pattern);
    size_t sourceLength = wideStringLength(source);

    int32_t index = -1;

    for(size_t i = startIndex; i < sourceLength; i++)
    {
        for(size_t j = 0; j < patternLength; j++)
        {
            if(source[i + j] != pattern[j])
            {
                break;
            }

            if(j == patternLength - 1)
            {
                index = i + j;
                break;
            }
        }

        if(index != -1)
        {
            break;
        }
    }

    return index;
}

void wideStringReplace(const wchar_t* source, const wchar_t* before, const wchar_t* after, const wchar_t* buffer, bool each)
{
    
}
