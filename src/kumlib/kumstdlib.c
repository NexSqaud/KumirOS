#include <stdlib_c.h>

#include <math.h>
#include <rtc.h>
#include <string.h>
#include <utils.h>
#include <wstring.h>

EXTERN void Kumir_Standard_Library_div(__kumir_scalar* result, const __kumir_scalar* left, const __kumir_scalar* right)
{
    __kumir_check_value_defined(left);
    __kumir_check_value_defined(right);

    __kumir_create_int(result, left->data.i / right->data.i);
}

EXTERN void Kumir_Standard_Library_mod(__kumir_scalar* result, const __kumir_scalar* left, const __kumir_scalar* right)
{
    __kumir_check_value_defined(left);
    __kumir_check_value_defined(right);

    __kumir_create_int(result, left->data.i % right->data.i);
}

EXTERN void Kumir_Standard_Library_ln(__kumir_scalar* result, const __kumir_scalar* value)
{
    __kumir_check_value_defined(value);
    UNUSED(result);
}

EXTERN void Kumir_Standard_Library_lg(__kumir_scalar* result, const __kumir_scalar* value)
{
    __kumir_check_value_defined(value);
    UNUSED(result);
}

EXTERN void Kumir_Standard_Library_exp(__kumir_scalar* result, const __kumir_scalar* value)
{
    __kumir_check_value_defined(value);
    UNUSED(result);
}

EXTERN void Kumir_Standard_Library_rnd(__kumir_scalar* result, const __kumir_scalar* value)
{
    __kumir_check_value_defined(value);

    __kumir_create_real(result, rand() % (int32_t)value->data.r);
}

EXTERN void Kumir_Standard_Library_irnd(__kumir_scalar* result, const __kumir_scalar* value)
{
    __kumir_check_value_defined(value);

    __kumir_create_int(result, rand() % value->data.i);
}

EXTERN void Kumir_Standard_Library_rand(__kumir_scalar* result, const __kumir_scalar* a, const __kumir_scalar* b)
{
    __kumir_check_value_defined(a);
    __kumir_check_value_defined(b);

    __kumir_create_real(result, a->data.r + (rand() % (int32_t)(b->data.r - a->data.r)));
}

EXTERN void Kumir_Standard_Library_irand(__kumir_scalar* result, const __kumir_scalar* a, const __kumir_scalar* b)
{
    __kumir_check_value_defined(a);
    __kumir_check_value_defined(b);

    __kumir_create_int(result, a->data.i + (rand() % (b->data.i - a->data.i)));
}

EXTERN void Kumir_Standard_Library_iabs(__kumir_scalar* result, const __kumir_scalar* value)
{
    __kumir_check_value_defined(value);

    __kumir_create_int(result, value->data.i < 0 ? -value->data.i : value->data.i);
}

EXTERN void Kumir_Standard_Library_abs(__kumir_scalar* result, const __kumir_scalar* value)
{
    __kumir_check_value_defined(value);

    __kumir_create_real(result, value->data.r < 0 ? -value->data.r : value->data.r);
}

EXTERN void Kumir_Standard_Library_sign(__kumir_scalar* result, const __kumir_scalar* value)
{
    __kumir_check_value_defined(value);

    __kumir_create_int(result, value->data.r < 0 ? -1 : (value->data.r == 0 ? 0 : 1));
}

EXTERN void Kumir_Standard_Library_int(__kumir_scalar* result, const __kumir_scalar* value)
{
    __kumir_check_value_defined(value);

    __kumir_create_int(result, (int32_t)value->data.r);
}

EXTERN void Kumir_Standard_Library_arcsin(__kumir_scalar* result, const __kumir_scalar* value)
{
    __kumir_check_value_defined(value);
    UNUSED(result);
}

EXTERN void Kumir_Standard_Library_arccos(__kumir_scalar* result, const __kumir_scalar* value)
{
    __kumir_check_value_defined(value);
    UNUSED(result);
}

EXTERN void Kumir_Standard_Library_arctg(__kumir_scalar* result, const __kumir_scalar* value)
{
    __kumir_check_value_defined(value);
    UNUSED(result);
}

EXTERN void Kumir_Standard_Library_arcctg(__kumir_scalar* result, const __kumir_scalar* value)
{
    __kumir_check_value_defined(value);
    UNUSED(result);
}

EXTERN void Kumir_Standard_Library_tg(__kumir_scalar* result, const __kumir_scalar* value)
{
    __kumir_check_value_defined(value);
    UNUSED(result);
}

EXTERN void Kumir_Standard_Library_ctg(__kumir_scalar* result, const __kumir_scalar* value)
{
    __kumir_check_value_defined(value);
    UNUSED(result);
}

EXTERN void Kumir_Standard_Library_sin(__kumir_scalar* result, const __kumir_scalar* value)
{
    __kumir_check_value_defined(value);
    UNUSED(result);
}

EXTERN void Kumir_Standard_Library_cos(__kumir_scalar* result, const __kumir_scalar* value)
{
    __kumir_check_value_defined(value);
    UNUSED(result);
}

EXTERN void Kumir_Standard_Library_sqrt(__kumir_scalar* result, const __kumir_scalar* value)
{
    __kumir_check_value_defined(value);
    __kumir_create_real(result, sqrt(value->data.r));
}

EXTERN void Kumir_Standard_Library_max(__kumir_scalar* result, const __kumir_scalar* value1, const __kumir_scalar* value2)
{
    __kumir_check_value_defined(value1);
    __kumir_check_value_defined(value2);

    __kumir_create_real(result, value1->data.r > value2->data.r ? value1->data.r : value2->data.r);
}

EXTERN void Kumir_Standard_Library_min(__kumir_scalar* result, const __kumir_scalar* value1, const __kumir_scalar* value2)
{
    __kumir_check_value_defined(value1);
    __kumir_check_value_defined(value2);

    __kumir_create_real(result, value1->data.r < value2->data.r ? value1->data.r : value2->data.r);
}

EXTERN void Kumir_Standard_Library_imax(__kumir_scalar* result, const __kumir_scalar* value1, const __kumir_scalar* value2)
{
    __kumir_check_value_defined(value1);
    __kumir_check_value_defined(value2);

    __kumir_create_real(result, value1->data.i > value2->data.i ? value1->data.i : value2->data.i);
}

EXTERN void Kumir_Standard_Library_imin(__kumir_scalar* result, const __kumir_scalar* value1, const __kumir_scalar* value2)
{
    __kumir_check_value_defined(value1);
    __kumir_check_value_defined(value2);

    __kumir_create_real(result, value1->data.i < value2->data.i ? value1->data.i : value2->data.i);
}


EXTERN void Kumir_Standard_Library_time(__kumir_scalar* result)
{
    __kumir_create_int(result, rtcGetTicksCount());
}

EXTERN void Kumir_Standard_Library_wait(const __kumir_scalar* value)
{
    __kumir_check_value_defined(value);

    sleep(value->data.i);
}


EXTERN void Kumir_Standard_Library_string_of_int(__kumir_scalar* result, const __kumir_scalar* value)
{
    __kumir_check_value_defined(value);
    UNUSED(result);
}

EXTERN void Kumir_Standard_Library_string_of_real(__kumir_scalar* result, const __kumir_scalar* value)
{
    __kumir_check_value_defined(value);
    UNUSED(result);
}

EXTERN void Kumir_Standard_Library_length(__kumir_scalar* result, const __kumir_scalar* value)
{
    __kumir_check_value_defined(value);

    __kumir_create_int(result, wideStringLength(value->data.s));
}

EXTERN void Kumir_Standard_Library_string_to_real(__kumir_scalar* result, const __kumir_scalar* value, __kumir_scalar* success)
{
    __kumir_check_value_defined(value);
    UNUSED(result);
    UNUSED(success);
}

EXTERN void Kumir_Standard_Library_string_to_int(__kumir_scalar* result, const __kumir_scalar* value, __kumir_scalar* success)
{
    __kumir_check_value_defined(value);
    UNUSED(result);
    UNUSED(success);
}

EXTERN void Kumir_Standard_Library_unicode(__kumir_scalar* result, const __kumir_scalar* value)
{
    __kumir_check_value_defined(value);
    UNUSED(result);
}

EXTERN void Kumir_Standard_Library_code(__kumir_scalar* result, const __kumir_scalar* value)
{
    __kumir_check_value_defined(value);
    UNUSED(result);
}

EXTERN void Kumir_Standard_Library_symbol(__kumir_scalar* result, const __kumir_scalar* value)
{
    __kumir_check_value_defined(value);
    UNUSED(result);
}

EXTERN void Kumir_Standard_Library_unisymbol(__kumir_scalar* result, const __kumir_scalar* value)
{
    __kumir_check_value_defined(value);
    UNUSED(result);
}

EXTERN void Kumir_Standard_Library_Integer(__kumir_scalar* result, const __kumir_scalar* lexem, const __kumir_scalar* def)
{
    __kumir_check_value_defined(lexem);
    __kumir_check_value_defined(def);

    __kumir_create_int(result, def->data.i);
}

EXTERN void Kumir_Standard_Library_Real(__kumir_scalar* result, const __kumir_scalar* lexem, const __kumir_scalar* def)
{
    __kumir_check_value_defined(lexem);
    __kumir_check_value_defined(def);

    __kumir_create_real(result, def->data.r);
}

EXTERN void Kumir_Standard_Library_Boolean(__kumir_scalar* result, const __kumir_scalar* lexem, const __kumir_scalar* def)
{
    __kumir_check_value_defined(lexem);
    __kumir_check_value_defined(def);

    __kumir_create_bool(result, def->data.b);
}

