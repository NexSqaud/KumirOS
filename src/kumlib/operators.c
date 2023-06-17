#include <stdlib_c.h>

#include <debug.h>
#include <math.h>

static signed char __kumir_compare_scalars(const __kumir_scalar* left, const __kumir_scalar* right)
{
    signed char result = 0;
    if (__KUMIR_INT == left->type && __KUMIR_INT == right->type) {
        const __kumir_int l = left->data.i;
        const __kumir_int r = right->data.i;
        if (l < r) result = 1;
        else if (l > r) result = -1;
        else if (l == r) result = 0;
    }
    else if (__KUMIR_REAL == left->type || __KUMIR_REAL == right->type) {
        const __kumir_real l = __kumir_scalar_as_real(left);
        const __kumir_real r = __kumir_scalar_as_real(right);
        if (l < r) result = 1;
        else if (l > r) result = -1;
        else if (l == r) result = 0;
    }
    else if (__KUMIR_BOOL == left->type && __KUMIR_BOOL == right->type) {
        const __kumir_bool l = left->data.b;
        const __kumir_bool r = right->data.b;
        if (l == r) result = 0;
        else result = 1;
    }
    else if (__KUMIR_STRING == left->type || __KUMIR_STRING == right->type) {
        //const std::wstring l = __kumir_scalar_as_wstring(left);
        //const std::wstring r = __kumir_scalar_as_wstring(right);
        //if (l < r) result = 1;
        //else if (l > r) result = -1;
        //else if (l == r) result = 0;
    }
    else if (__KUMIR_CHAR == left->type && __KUMIR_CHAR == right->type) {
        const __kumir_char l = left->data.c;
        const __kumir_char r = right->data.c;
        if (l < r) result = 1;
        else if (l > r) result = -1;
        else if (l == r) result = 0;
    }
    else if (__KUMIR_RECORD == left->type && __KUMIR_RECORD == right->type &&
        left->data.u.nfields == right->data.u.nfields)
    {
        // Check for equality only, otherwise return 1
        result = 0;
        const __kumir_variant* L = (const __kumir_variant*)(left->data.u.fields);
        const __kumir_variant* R = (const __kumir_variant*)(right->data.u.fields);
        const __kumir_scalar_type* LT = left->data.u.types;
        const __kumir_scalar_type* RT = right->data.u.types;
        for (size_t i = 0u; i < left->data.u.nfields; i++) {
            __kumir_scalar l, r;
            l.defined = true;
            l.data = L[i];
            l.type = LT[i];
            r.defined = true;
            r.data = R[i];
            r.type = RT[i];
            signed char field_result = __kumir_compare_scalars(&l, &r);
            if (field_result != 0) {
                return 1;
            }
        }
    }
    return result;
}

EXTERN void __kumir_operator_eq(__kumir_scalar* result, const __kumir_scalar* left, const __kumir_scalar* right)
{
    __kumir_check_value_defined(left);
    __kumir_check_value_defined(right);
    result->defined = true;
    result->type = __KUMIR_BOOL;
    signed char v = __kumir_compare_scalars(left, right);
    result->data.b = v == 0;
}

EXTERN void __kumir_operator_ls(__kumir_scalar* result, const __kumir_scalar* left, const __kumir_scalar* right)
{
    __kumir_check_value_defined(left);
    __kumir_check_value_defined(right);

    result->defined = true;
    result->type = __KUMIR_BOOL;
    result->data.b = __kumir_compare_scalars(left, right) == 1;
}

EXTERN void __kumir_operator_gt(__kumir_scalar* result, const __kumir_scalar* left, const __kumir_scalar* right)
{
    __kumir_check_value_defined(left);
    __kumir_check_value_defined(right);

    result->defined = true;
    result->type = __KUMIR_BOOL;
    result->data.b = __kumir_compare_scalars(left, right) == -1;
}

EXTERN void __kumir_operator_lq(__kumir_scalar* result, const __kumir_scalar* left, const __kumir_scalar* right)
{
    __kumir_check_value_defined(left);
    __kumir_check_value_defined(right);

    result->defined = true;
    result->type = __KUMIR_BOOL;
    result->data.b = __kumir_compare_scalars(left, right) >= 0;
}

EXTERN void __kumir_operator_gq(__kumir_scalar* result, const __kumir_scalar* left, const __kumir_scalar* right)
{
    __kumir_check_value_defined(left);
    __kumir_check_value_defined(right);

    result->defined = true;
    result->type = __KUMIR_BOOL;
    result->data.b = __kumir_compare_scalars(left, right) <= 0;
}

EXTERN void __kumir_operator_neq(__kumir_scalar* result, const __kumir_scalar* left, const __kumir_scalar* right)
{
    __kumir_operator_eq(result, left, right);
    result->data.b = !result->data.b;
}

EXTERN void __kumir_operator_sum(__kumir_scalar* result, const __kumir_scalar* left, const __kumir_scalar* right)
{
    __kumir_check_value_defined(left);
    __kumir_check_value_defined(right);

    if (__KUMIR_INT == left->type && __KUMIR_INT == right->type) {
        //if (!Kumir::Math::checkSumm(left->data.i, right->data.i)) {
        //    __kumir_abort_on_error("Integer overflow");
        //}
        __kumir_create_int(result, left->data.i + right->data.i);
    }
    else if (__KUMIR_REAL == left->type || __KUMIR_REAL == right->type) {
        const __kumir_real l = __kumir_scalar_as_real(left);
        const __kumir_real r = __kumir_scalar_as_real(right);
        const __kumir_real res = l + r;
        //if (!Kumir::Math::isCorrectReal(res)) {
        //    __kumir_abort_on_error("Float overflow");
        //}
        __kumir_create_real(result, res);
    }
    else if (__KUMIR_STRING == left->type || __KUMIR_CHAR == left->type) {
        //const std::wstring l = __kumir_scalar_as_wstring(left);
        //const std::wstring r = __kumir_scalar_as_wstring(right);
        //const std::wstring res = l + r;
        //__kumir_create_string(result, res);
    }
}

EXTERN void __kumir_operator_sub(__kumir_scalar* result, const __kumir_scalar* left, const __kumir_scalar* right)
{
    __kumir_check_value_defined(left);
    __kumir_check_value_defined(right);

    if (__KUMIR_INT == left->type && __KUMIR_INT == right->type) {
        //if (!Kumir::Math::checkDiff(left->data.i, right->data.i)) {
        //    __kumir_abort_on_error("Integer overflow");
        //}
        __kumir_create_int(result, left->data.i - right->data.i);
    }
    else if (__KUMIR_REAL == left->type || __KUMIR_REAL == right->type) {
        const __kumir_real l = __kumir_scalar_as_real(left);
        const __kumir_real r = __kumir_scalar_as_real(right);
        const __kumir_real res = l - r;
        //if (!Kumir::Math::isCorrectReal(res)) {
        //    __kumir_abort_on_error("Float overflow");
        //}
        __kumir_create_real(result, res);
    }
}

EXTERN void __kumir_operator_mul(__kumir_scalar* result, const __kumir_scalar* left, const __kumir_scalar* right)
{
    __kumir_check_value_defined(left);
    __kumir_check_value_defined(right);

    if (__KUMIR_INT == left->type && __KUMIR_INT == right->type) {
        //if (!Kumir::Math::checkProd(left->data.i, right->data.i)) {
        //    __kumir_abort_on_error("Integer overflow");
        //}
        __kumir_create_int(result, left->data.i * right->data.i);
    }
    else if (__KUMIR_REAL == left->type || __KUMIR_REAL == right->type) {
        const __kumir_real l = __kumir_scalar_as_real(left);
        const __kumir_real r = __kumir_scalar_as_real(right);
        __kumir_create_real(result, l * r);
        //if (!Kumir::Math::isCorrectReal(result->data.r)) {
        //    __kumir_abort_on_error("Float overflow");
        //}
    }
}

EXTERN void __kumir_operator_div(__kumir_scalar* result, const __kumir_scalar* left, const __kumir_scalar* right)
{
    __kumir_check_value_defined(left);
    __kumir_check_value_defined(right);

    if ((__KUMIR_INT == right->type && 0 == right->data.i) || (__KUMIR_REAL == right->type && 0. == right->data.r)) {
        __kumir_abort_on_error("Divide by zero");
    }

    const __kumir_real l = __kumir_scalar_as_real(left);
    const __kumir_real r = __kumir_scalar_as_real(right);
    __kumir_create_real(result, l / r);
    //if (!Kumir::Math::isCorrectReal(result->data.r)) {
    //    __kumir_abort_on_error("Float overflow");
    //}
}

EXTERN void __kumir_operator_pow(__kumir_scalar* result, const __kumir_scalar* left, const __kumir_scalar* right)
{
    __kumir_check_value_defined(left);
    __kumir_check_value_defined(right);

    if (__KUMIR_INT == left->type && __KUMIR_INT == right->type) {
        __kumir_create_int(result, pow(left->data.i, right->data.i));
    }
    else {
        const __kumir_real l = __kumir_scalar_as_real(left);
        const __kumir_real r = __kumir_scalar_as_real(right);
        __kumir_create_real(result, powf(l, r));
    }
}

EXTERN void __kumir_operator_neg(__kumir_scalar* result, const __kumir_scalar* left)
{
    __kumir_check_value_defined(left);
    result->defined = true;
    result->type = left->type;
    if (__KUMIR_BOOL == left->type) {
        result->data.b = !left->data.b;
    }
    else if (__KUMIR_INT == left->type) {
        result->data.i = -left->data.i;
    }
    else if (__KUMIR_REAL == left->type) {
        result->data.r = 0.0 - left->data.r;
    }
}
