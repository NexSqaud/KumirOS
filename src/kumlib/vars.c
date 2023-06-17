#include <stdlib_c.h>

#include <debug.h>
#include <stddef.h>
#include <memory.h>
#include <utf8.h>

//inline __kumir_int IMAX(const __kumir_int a, const __kumir_int b)
//{
//    return a < b ? b : a;
//}
//
//inline __kumir_int IMIN(const __kumir_int a, const __kumir_int b)
//{
//    return a > b ? b : a;
//}

__kumir_variant __kumir_get_record_field(const __kumir_scalar* record, size_t fieldIndex)
{
    __kumir_check_value_defined(record);

    if (fieldIndex >= record->data.u.nfields)
    {
        __kumir_abort_on_error("Try to get record field with index more or equal to nfields");
    }

    __kumir_variant* fields = (__kumir_variant*)record->data.u.fields;
    return fields[fieldIndex];
}

EXTERN void __kumir_create_undefined_scalar(__kumir_scalar* result)
{
    result->defined = false;
    result->data.s = NULL;
}

EXTERN void __kumir_create_undefined_array(__kumir_array* result)
{
    result->data = NULL;
    result->dim = 0;
}

EXTERN void __kumir_create_defined_scalar(__kumir_scalar* lvalue, const __kumir_scalar* value)
{
    __kumir_copy_scalar(lvalue, value);
}

EXTERN void __kumir_create_int(__kumir_scalar* result, const __kumir_int value)
{
    result->defined = true;
    result->type = __KUMIR_INT;
    result->data.i = value;
}

EXTERN void __kumir_create_real(__kumir_scalar* result, const __kumir_real value)
{
    result->defined = true;
    result->type = __KUMIR_REAL;
    result->data.r = value;
}

EXTERN void __kumir_create_bool(__kumir_scalar* result, const __kumir_bool value)
{
    result->defined = true;
    result->type = __KUMIR_BOOL;
    result->data.b = value;
}

static void __kumir_create_char_internal(__kumir_scalar* result, const __kumir_char value)
{
    result->defined = true;
    result->type = __KUMIR_CHAR;
    result->data.c = value;
}

EXTERN void __kumir_create_char(__kumir_scalar* result, const char* utf8)
{
    result->defined = true;
    result->type = __KUMIR_CHAR;

    size_t size = utf8ToWChar(utf8, SIZE_MAX, NULL, 0);
    result->data.s = malloc(size * sizeof(wchar_t));
    utf8ToWChar(utf8, SIZE_MAX, result->data.s, size);

    __kumir_char ch = result->data.s[0];
    free(result->data.s);
    result->data.c = ch;
}

EXTERN void __kumir_create_string_from_wstring(__kumir_scalar* result, const wchar_t* wstring)
{
    result->defined = true;
    result->type = __KUMIR_STRING;
    result->data.s = wstring;
}

EXTERN void __kumir_create_string(__kumir_scalar* result, const char* utf8)
{
    result->defined = true;
    result->type = __KUMIR_STRING;
    size_t size = utf8ToWChar(utf8, SIZE_MAX, NULL, 0);
    result->data.s = malloc(size * sizeof(wchar_t));
    utf8ToWChar(utf8, SIZE_MAX, result->data.s, size);
}

EXTERN void __kumir_create_record(__kumir_scalar* result, int32_t fieldsCount)
{
    result->defined = true;
    result->type = __KUMIR_RECORD;
    result->data.u.nfields = fieldsCount;
    result->data.u.fields = malloc(fieldsCount * sizeof(__kumir_variant));
    result->data.u.types = malloc(fieldsCount * sizeof(__kumir_scalar_type));
}

EXTERN void __kumir_add_record_field(__kumir_scalar* record, int32_t fieldIndex, __kumir_scalar* value)
{
    __kumir_check_value_defined(record);
    __kumir_check_value_defined(value);

    if (fieldIndex < 0 || fieldIndex >= record->data.u.nfields)
    {
        __kumir_abort_on_error("Trying to add more fields than nfields");
    }
    __kumir_variant* fields = (__kumir_variant*)record->data.u.fields;
    fields[fieldIndex] = value->data;
    record->data.u.types[fieldIndex] = value->type;
}


EXTERN void __kumir_convert_char_to_string(__kumir_scalar* result, const __kumir_scalar* source)
{
    UNUSED(result);
    UNUSED(source);
}

EXTERN void __kumir_convert_int_to_real(__kumir_scalar* result, const __kumir_scalar* source)
{
    __kumir_check_value_defined(source);

    result->type = __KUMIR_REAL;
    result->defined = true;
    result->data.r = (float)source->data.i;
}

EXTERN __kumir_variant __kumir_copy_variant(const __kumir_variant rvalue, __kumir_scalar_type type)
{
    __kumir_variant result;
    switch (type)
    {
    case __KUMIR_INT:
    case __KUMIR_REAL:
    case __KUMIR_BOOL:
    case __KUMIR_CHAR:
        result = rvalue;
        break;
    case __KUMIR_RECORD:
        result.u.nfields = rvalue.u.nfields;
        result.u.types = (__kumir_scalar_type*)calloc(result.u.nfields, sizeof(__kumir_scalar_type));
        result.u.fields = (__kumir_variant*)calloc(result.u.nfields, sizeof(__kumir_variant));
        __kumir_variant* lfields = (__kumir_variant*)result.u.fields;
        __kumir_variant* rfields = (__kumir_variant*)rvalue.u.fields;
        for (size_t i = 0; i < rvalue.u.nfields; i++)
        {
            __kumir_scalar_type type = result.u.types[i] = rvalue.u.types[i];
            lfields[i] = __kumir_copy_variant(rfields[i], type);
        }
        break;
    default:
        break;
    }
    return result;
}

EXTERN void __kumir_copy_scalar(__kumir_scalar* lvalue, const __kumir_scalar* rvalue)
{
    lvalue->defined = rvalue->defined;
    lvalue->type = rvalue->type;
    __kumir_check_value_defined(rvalue);
    if (lvalue->defined)
    {
        lvalue->data = __kumir_copy_variant(rvalue->data, rvalue->type);
    }
}

EXTERN void __kumir_move_scalar(__kumir_scalar* lvalue, __kumir_scalar* rvalue)
{
    lvalue->defined = rvalue->defined;
    lvalue->type = rvalue->type;
    memcpy(&(lvalue->data), &(rvalue->data), sizeof(rvalue->data));
}

EXTERN void __kumir_store_scalar(__kumir_scalar** lvalue_ptr, const __kumir_scalar* rvalue)
{
    __kumir_scalar* lvalue = *lvalue_ptr;
    __kumir_copy_scalar(lvalue, rvalue);
}

EXTERN void __kumir_modify_string(__kumir_stringref* lvalue, const __kumir_scalar* rvalue)
{
    UNUSED(lvalue);
    UNUSED(rvalue);
}

EXTERN inline void __kumir_check_value_defined(const __kumir_scalar* value)
{
    if (value == NULL || !value->defined)
    {
        __kumir_abort_on_error("Value not defined!");
    }
}

EXTERN __kumir_bool __kumir_scalar_as_bool(const __kumir_scalar* scalar)
{
    __kumir_check_value_defined(scalar);
    return scalar->data.b;
}

EXTERN void __kumir_free_scalar(__kumir_scalar* scalar)
{
    if (scalar->defined && scalar->type == __KUMIR_STRING && scalar->data.s)
    {
        free(scalar->data.s);
        scalar->data.s = NULL;
    }
    else if (scalar->defined && scalar->type == __KUMIR_RECORD)
    {
        __kumir_scalar_type* types = scalar->data.u.types;
        __kumir_variant* fields = (__kumir_variant*)scalar->data.u.fields;
        for (size_t i = 0; i < scalar->data.u.nfields; i++)
        {
            if (types[i] == __KUMIR_STRING)
            {
                free(fields[i].s);
            }
        }
        free(scalar->data.u.types);
        free(scalar->data.u.fields);
        scalar->data.u.types = 0;
        scalar->data.u.fields = 0;
    }
    scalar->defined = false;
}
