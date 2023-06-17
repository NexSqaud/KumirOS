#include <stdlib_c.h>

#include <memory.h>
#include <string.h>
#include <wstring.h>

EXTERN void String_Utilities_find_after(__kumir_scalar* result, const __kumir_scalar* ot, const __kumir_scalar* fragment, const __kumir_scalar* thestring)
{
    __kumir_check_value_defined(ot);
    __kumir_check_value_defined(fragment);
    __kumir_check_value_defined(thestring);

    int from = ot->data.i;
    wchar_t* pattern = fragment->data.s;
    wchar_t* source = thestring->data.s;

    int32_t index = wideStringFind(source, pattern, from - 1);

    __kumir_create_int(result, index);
}

EXTERN void String_Utilities_find(__kumir_scalar* result, const __kumir_scalar* fragment, const __kumir_scalar* thestring)
{
    __kumir_check_value_defined(fragment);
    __kumir_check_value_defined(thestring);

    wchar_t* pattern = fragment->data.s;
    wchar_t* source = thestring->data.s;

    int32_t index = wideStringFind(source, pattern, 0);

    __kumir_create_int(result, index);
}

EXTERN void String_Utilities_index_after(__kumir_scalar* result, const __kumir_scalar* ot, const __kumir_scalar* fragment, const __kumir_scalar* thestring)
{
    String_Utilities_find_after(result, ot, fragment, thestring);
}

EXTERN void String_Utilities_index(__kumir_scalar* result, const __kumir_scalar* fragment, const __kumir_scalar* thestring)
{
    String_Utilities_find(result, fragment, thestring);
}

EXTERN void String_Utilities_lowercase(__kumir_scalar* result, const __kumir_scalar* thestring)
{
    result = thestring;
}

EXTERN void String_Utilities_uppercase(__kumir_scalar* result, const __kumir_scalar* thestring)
{
    result = thestring;
}

EXTERN void String_Utilities_insert(const __kumir_scalar* fragment, __kumir_scalar* thestring, const __kumir_scalar* position)
{
    __kumir_check_value_defined(fragment);
    __kumir_check_value_defined(thestring);
    __kumir_check_value_defined(position);

    wchar_t* string = thestring->data.s;
    wchar_t* frag = fragment->data.s;
    
    size_t stringLength = wideStringLength(string);
    size_t fragLength = wideStringLength(frag);

    size_t newStringSize = (stringLength + fragLength) * sizeof(wchar_t) + 1;
    wchar_t* newBuffer = (wchar_t*)malloc(newStringSize);
    
    size_t stringFirstLength = position->data.i - 1;
    size_t stringRemaningLength = stringLength - stringFirstLength;

    memcpy(newBuffer, string, stringFirstLength * sizeof(wchar_t));
    memcpy((newBuffer + stringFirstLength), frag, fragLength * sizeof(wchar_t));
    memcpy((newBuffer + stringFirstLength + fragLength), (string + stringFirstLength), stringRemaningLength * sizeof(wchar_t));
    newBuffer[newStringSize - 1] = 0;

    __kumir_free_scalar(thestring);
    __kumir_create_string_from_wstring(thestring, newBuffer);
}

EXTERN void String_Utilities_remove(__kumir_scalar* source, const __kumir_scalar* position, const __kumir_scalar* thelength)
{
    __kumir_check_value_defined(source);
    __kumir_check_value_defined(position);
    __kumir_check_value_defined(thelength);

    wchar_t* string = source->data.s;

    size_t stringLength = wideStringLength(string);
    size_t remaningLength = stringLength - thelength->data.i + 1;

    wchar_t* newBuffer = (wchar_t*)malloc(remaningLength * sizeof(wchar_t));
    
    memcpy(newBuffer, string, position->data.i * sizeof(wchar_t));
    memcpy((newBuffer + position->data.i), (string + position->data.i + thelength->data.i), (remaningLength - position->data.i) * sizeof(wchar_t));

    __kumir_free_scalar(source);
    __kumir_create_string_from_wstring(source, newBuffer);
}

EXTERN void String_Utilities_replace(__kumir_scalar* source, const __kumir_scalar* before, const __kumir_scalar* after, const __kumir_scalar* each)
{
    UNUSED(source);
    UNUSED(before);
    UNUSED(after);
    UNUSED(each);
}
