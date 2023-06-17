#include <stdlib_c.h>

#include <debug.h>
#include <memory.h>
#include <string.h>

EXTERN void __kumir_cleanup_array_in_shape(__kumir_array* result)
{
    size_t size = (size_t)(result->size_right[0] - result->size_left[0] + 1);
    if (result->dim > 1u) {
        size *= (size_t)(result->size_right[1] - result->size_left[1] + 1);
    }
    if (result->dim > 2u) {
        size *= (size_t)(result->size_right[2] - result->size_left[2] + 1);
    }
    for (size_t i = 0; i < size; i++) {
        __kumir_scalar* s = result->data + i;
        __kumir_free_scalar(s);
    }
}

EXTERN void __kumir_create_array_1(__kumir_array* result,
    const __kumir_scalar* left_1,
    const __kumir_scalar* right_1
)
{
    __kumir_check_value_defined(left_1);
    __kumir_check_value_defined(right_1);
    result->dim = 1u;
    result->shape_left[0] = result->size_left[0] = left_1->data.i;
    result->shape_right[0] = result->size_right[0] = right_1->data.i;
    result->shape_left[1] = result->size_left[1] = 0;
    result->shape_right[1] = result->size_right[1] = 0;
    result->shape_left[2] = result->size_left[2] = 0;
    result->shape_right[2] = result->size_right[2] = 0;
    int isz = 1 + right_1->data.i - left_1->data.i;
    if (isz < 1) {
        __kumir_abort_on_error("Invalid table size");
    }
    else
    {
        size_t sz = (size_t)(isz);
        if (sz) {
            __kumir_scalar* scalars = (__kumir_scalar*)calloc(sz, sizeof(__kumir_scalar));
            result->data = scalars;
            for (size_t i = 0; i < sz; i++) {
                __kumir_create_undefined_scalar(&scalars[i]);
            }
        }
    }
}

EXTERN void __kumir_create_array_ref_1(__kumir_array* result,
    const __kumir_scalar* left_1,
    const __kumir_scalar* right_1
)
{
    __kumir_check_value_defined(left_1);
    __kumir_check_value_defined(right_1);
    if (left_1->data.i > right_1->data.i) {
        __kumir_abort_on_error("Invalid table size");
    }
    result->shape_left[0] = IMAX(result->shape_left[0], left_1->data.i);
    result->shape_right[0] = IMIN(result->shape_right[0], right_1->data.i);
}

EXTERN void __kumir_create_array_copy_1(__kumir_array* result,
    const __kumir_scalar* left_1,
    const __kumir_scalar* right_1
)
{
    __kumir_create_array_ref_1(result, left_1, right_1);

    const size_t source_start_x =
        (size_t)(result->shape_left[0] - result->size_left[0]);

    const size_t source_size_x =
        (size_t)(result->size_right[0] - result->size_left[0] + 1);

    const size_t target_size_x =
        (size_t)(result->shape_right[0] - result->shape_left[0] + 1);

    __kumir_assert_internal(target_size_x <= source_size_x, __LINE__);

    const size_t block_size = target_size_x;

    __kumir_scalar* const data = (__kumir_scalar*)calloc(block_size, sizeof(__kumir_scalar));

    for (size_t x = 0u; x < target_size_x; x++) {

        const size_t source_index = source_start_x + x;
        const size_t target_index = x;

        const __kumir_scalar* const source_element = result->data + source_index;
        __kumir_scalar* const target_element = data + target_index;

        target_element->defined = source_element->defined;
        target_element->type = source_element->type;
        target_element->data = __kumir_copy_variant(source_element->data,
            source_element->type);

    }

    result->data = data;
    result->size_left[0] = result->shape_left[0];
    result->size_right[0] = result->shape_right[0];
}

EXTERN void __kumir_create_array_2(__kumir_array* result,
    const __kumir_scalar* left_1,
    const __kumir_scalar* right_1,
    const __kumir_scalar* left_2,
    const __kumir_scalar* right_2
)
{
    __kumir_check_value_defined(left_1);
    __kumir_check_value_defined(right_1);
    __kumir_check_value_defined(left_2);
    __kumir_check_value_defined(right_2);
    result->dim = 2u;
    result->shape_left[0] = result->size_left[0] = left_1->data.i;
    result->shape_right[0] = result->size_right[0] = right_1->data.i;
    result->shape_left[1] = result->size_left[1] = left_2->data.i;
    result->shape_right[1] = result->size_right[1] = right_2->data.i;
    result->shape_left[2] = result->size_left[2] = 0;
    result->shape_right[2] = result->size_right[2] = 0;
    int isz1 = 1 + right_1->data.i - left_1->data.i;
    int isz2 = 1 + right_2->data.i - left_2->data.i;
    if (isz1 < 1 || isz2 < 1) {
        __kumir_abort_on_error("Invalid table size");
    }
    else {
        size_t sz = (size_t)(isz1 * isz2);
        if (sz) {
            __kumir_scalar* scalars = (__kumir_scalar*)calloc(sz, sizeof(__kumir_scalar));
            result->data = scalars;
            for (size_t i = 0; i < sz; i++) {
                __kumir_create_undefined_scalar(&scalars[i]);
            }
        }
    }
}

EXTERN void __kumir_create_array_ref_2(__kumir_array* result,
    const __kumir_scalar* left_1,
    const __kumir_scalar* right_1,
    const __kumir_scalar* left_2,
    const __kumir_scalar* right_2
)
{
    __kumir_check_value_defined(left_1);
    __kumir_check_value_defined(right_1);
    __kumir_check_value_defined(left_2);
    __kumir_check_value_defined(right_2);
    if (right_1->data.i < left_1->data.i) {
        __kumir_abort_on_error("Invalid table size");
    }
    if (right_2->data.i < left_2->data.i) {
        __kumir_abort_on_error("Invalid table size");
    }
    result->shape_left[0] = IMAX(result->shape_left[0], left_1->data.i);
    result->shape_right[0] = IMIN(result->shape_right[0], right_1->data.i);
    result->shape_left[1] = IMAX(result->shape_left[1], left_2->data.i);
    result->shape_right[1] = IMIN(result->shape_right[1], right_2->data.i);
}

EXTERN void __kumir_create_array_copy_2(__kumir_array* result,
    const __kumir_scalar* left_1,
    const __kumir_scalar* right_1,
    const __kumir_scalar* left_2,
    const __kumir_scalar* right_2
)
{
    __kumir_create_array_ref_2(result, left_1, right_1, left_2, right_2);

    const size_t source_start_y =
        (size_t)(result->shape_left[0] - result->size_left[0]);

    const size_t source_size_y =
        (size_t)(result->size_right[0] - result->size_left[0] + 1);

    const size_t target_size_y =
        (size_t)(result->shape_right[0] - result->shape_left[0] + 1);

    __kumir_assert_internal(target_size_y <= source_size_y, __LINE__);

    const size_t source_start_x =
        (size_t)(result->shape_left[1] - result->size_left[1]);

    const size_t source_size_x =
        (size_t)(result->size_right[1] - result->size_left[1] + 1);

    const size_t target_size_x =
        (size_t)(result->shape_right[1] - result->shape_left[1] + 1);

    __kumir_assert_internal(target_size_x <= source_size_x, __LINE__);

    const size_t block_size = target_size_y * target_size_x;

    __kumir_scalar* const data = (__kumir_scalar*)calloc(block_size, sizeof(__kumir_scalar));


    for (size_t y = 0u; y < target_size_y; y++) {
        for (size_t x = 0u; x < target_size_x; x++) {

            const size_t source_index =
                (source_start_y + y) * source_size_x +
                source_start_x + x;

            const size_t target_index =
                target_size_x * y +
                x;

            const __kumir_scalar* const source_element = result->data + source_index;
            __kumir_scalar* const target_element = data + target_index;

            target_element->defined = source_element->defined;
            target_element->type = source_element->type;
            target_element->data = __kumir_copy_variant(source_element->data,
                source_element->type);
        }
    }

    result->data = data;
    result->size_left[0] = result->shape_left[0];
    result->size_left[1] = result->shape_left[1];
    result->size_right[0] = result->shape_right[0];
    result->size_right[1] = result->shape_right[1];
}

EXTERN void __kumir_create_array_3(__kumir_array* result,
    const __kumir_scalar* left_1,
    const __kumir_scalar* right_1,
    const __kumir_scalar* left_2,
    const __kumir_scalar* right_2,
    const __kumir_scalar* left_3,
    const __kumir_scalar* right_3
)
{
    __kumir_check_value_defined(left_1);
    __kumir_check_value_defined(right_1);
    __kumir_check_value_defined(left_2);
    __kumir_check_value_defined(right_2);
    __kumir_check_value_defined(left_3);
    __kumir_check_value_defined(right_3);
    result->dim = 3u;
    result->shape_left[0] = result->size_left[0] = left_1->data.i;
    result->shape_right[0] = result->size_right[0] = right_1->data.i;
    result->shape_left[1] = result->size_left[1] = left_2->data.i;
    result->shape_right[1] = result->size_right[1] = right_2->data.i;
    result->shape_left[2] = result->size_left[2] = left_3->data.i;
    result->shape_right[2] = result->size_right[2] = right_3->data.i;
    int isz1 = 1 + right_1->data.i - left_1->data.i;
    int isz2 = 1 + right_2->data.i - left_2->data.i;
    int isz3 = 1 + right_3->data.i - left_3->data.i;
    if (isz1 < 1 || isz2 < 1 || isz3 < 1) {
        __kumir_abort_on_error("Invalid table size");
    }
    else {
        size_t sz = (size_t)(isz1 * isz2 * isz3);
        if (sz) {
            __kumir_scalar* scalars = (__kumir_scalar*)calloc(sz, sizeof(__kumir_scalar));
            result->data = scalars;
            for (size_t i = 0; i < sz; i++) {
                __kumir_create_undefined_scalar(&scalars[i]);
            }
        }
    }
}

EXTERN void __kumir_create_array_ref_3(__kumir_array* result,
    const __kumir_scalar* left_1,
    const __kumir_scalar* right_1,
    const __kumir_scalar* left_2,
    const __kumir_scalar* right_2,
    const __kumir_scalar* left_3,
    const __kumir_scalar* right_3
)
{
    __kumir_check_value_defined(left_1);
    __kumir_check_value_defined(right_1);
    __kumir_check_value_defined(left_2);
    __kumir_check_value_defined(right_2);
    __kumir_check_value_defined(left_3);
    __kumir_check_value_defined(right_3);
    if (right_1->data.i < left_1->data.i) {
        __kumir_abort_on_error("Invalid table size");
    }
    if (right_2->data.i < left_2->data.i) {
        __kumir_abort_on_error("Invalid table size");
    }
    if (right_3->data.i < left_3->data.i) {
        __kumir_abort_on_error("Invalid table size");
    }
    result->shape_left[0] = IMAX(result->shape_left[0], left_1->data.i);
    result->shape_right[0] = IMIN(result->shape_right[0], right_1->data.i);
    result->shape_left[1] = IMAX(result->shape_left[1], left_2->data.i);
    result->shape_right[1] = IMIN(result->shape_right[1], right_2->data.i);
    result->shape_left[2] = IMAX(result->shape_left[2], left_3->data.i);
    result->shape_right[2] = IMIN(result->shape_right[2], right_3->data.i);
}

EXTERN void __kumir_create_array_copy_3(__kumir_array* result,
    const __kumir_scalar* left_1,
    const __kumir_scalar* right_1,
    const __kumir_scalar* left_2,
    const __kumir_scalar* right_2,
    const __kumir_scalar* left_3,
    const __kumir_scalar* right_3
)
{
    __kumir_create_array_ref_3(result, left_1, right_1, left_2, right_2, left_3, right_3);

    const size_t source_start_z =
        (size_t)(result->shape_left[0] - result->size_left[0]);

    const size_t source_size_z =
        (size_t)(result->size_right[0] - result->size_left[0] + 1);

    const size_t target_size_z =
        (size_t)(result->shape_right[0] - result->shape_left[0] + 1);

    __kumir_assert_internal(target_size_z <= source_size_z, __LINE__);

    const size_t source_start_y =
        (size_t)(result->shape_left[1] - result->size_left[1]);

    const size_t source_size_y =
        (size_t)(result->size_right[1] - result->size_left[1] + 1);

    const size_t target_size_y =
        (size_t)(result->shape_right[1] - result->shape_left[1] + 1);

    __kumir_assert_internal(target_size_y <= source_size_y, __LINE__);

    const size_t source_start_x =
        (size_t)(result->shape_left[2] - result->size_left[2]);

    const size_t source_size_x =
        (size_t)(result->size_right[2] - result->size_left[2] + 1);

    const size_t target_size_x =
        (size_t)(result->shape_right[2] - result->shape_left[2] + 1);

    __kumir_assert_internal(target_size_x <= source_size_x, __LINE__);

    const size_t block_size = target_size_z * target_size_y * target_size_x;

    __kumir_scalar* const data = (__kumir_scalar*)calloc(block_size, sizeof(__kumir_scalar));

    for (size_t z = 0u; z < target_size_z; z++) {
        for (size_t y = 0u; y < target_size_y; y++) {
            for (size_t x = 0u; x < target_size_x; x++) {
                const size_t source_index =
                    (source_start_z + z) * source_size_y * source_size_x +
                    (source_start_y + y) * source_size_x +
                    source_start_x + x;

                const size_t target_index =
                    target_size_x * target_size_y * z +
                    target_size_x * y +
                    x;

                const __kumir_scalar* const source_element = result->data + source_index;
                __kumir_scalar* const target_element = data + target_index;

                target_element->defined = source_element->defined;
                target_element->type = source_element->type;
                target_element->data = __kumir_copy_variant(source_element->data,
                    source_element->type);
            }
        }
    }

    result->data = data;
    result->size_left[0] = result->shape_left[0];
    result->size_left[1] = result->shape_left[1];
    result->size_left[2] = result->shape_left[2];
    result->size_right[0] = result->shape_right[0];
    result->size_right[1] = result->shape_right[1];
    result->size_right[2] = result->shape_right[2];
}

EXTERN void __kumir_link_array(__kumir_array* result, const __kumir_array* from)
{
    memcpy(result, from, sizeof(__kumir_array));
}

EXTERN void __kumir_free_array(__kumir_array* array)
{
    if (array->dim > 0u && array->data) {
        const __kumir_scalar* first = array->data;
        if (__KUMIR_STRING == first->type) {
            size_t elems = 0u;
            elems = (size_t)(1 + array->size_right[0] - array->size_left[0]);
            if (array->dim > 1u) {
                elems *= (size_t)(1 + array->size_right[1] - array->size_left[1]);
            }
            if (array->dim > 2u) {
                elems *= (size_t)(1 + array->size_right[2] - array->size_left[2]);
            }
            for (size_t i = 0; i < elems; i++) {
                __kumir_free_scalar(&(array->data[i]));
            }
        }
        free(array->data);
    }
}

#define EAT(type, lvalue, rvalue) \
    const char** __EAT_PDATA = &rvalue;                    \
    const char* __EAT_DATA = *__EAT_PDATA;              \
    const type* __EAT_IDATA = (const type*)__EAT_DATA;    \
    type __EAT_VALUE = __EAT_IDATA[0];                     \
    __EAT_IDATA++;                                         \
    __EAT_DATA = (const char*)__EAT_IDATA;                \
    *__EAT_PDATA = __EAT_DATA;                            \
    lvalue = __EAT_VALUE

static void __kumir_fill_array_1(__kumir_array* array, const char* data, const __kumir_scalar_type type)
{
    const char* p = data;
    EAT(__kumir_int, __kumir_int isz, p);
    size_t index = 0u;
    __kumir_scalar* adata = (__kumir_scalar*)(array->data);
    while (isz) {
        EAT(__kumir_bool, adata[index].defined, p);
        adata[index].type = type;
        if (__KUMIR_INT == type) {
            EAT(__kumir_int, adata[index].data.i, p);
        }
        else if (__KUMIR_REAL == type) {
            EAT(__kumir_real, adata[index].data.r, p);
        }
        else if (__KUMIR_BOOL == type) {
            EAT(__kumir_bool, adata[index].data.b, p);
        }
        else if (__KUMIR_CHAR == type) {
            EAT(uint16_t, uint16_t code, p);
            adata[index].data.c = code;
        }
        isz--;
        index++;
    }
}

static unsigned char __kumir_hex_to_byte(const char c)
{
    switch (c) {
    case '0': return 0x0u;
    case '1': return 0x1u;
    case '2': return 0x2u;
    case '3': return 0x3u;
    case '4': return 0x4u;
    case '5': return 0x5u;
    case '6': return 0x6u;
    case '7': return 0x7u;
    case '8': return 0x8u;
    case '9': return 0x9u;
    case 'a': case 'A': return 0xAu;
    case 'b': case 'B': return 0xBu;
    case 'c': case 'C': return 0xCu;
    case 'd': case 'D': return 0xDu;
    case 'e': case 'E': return 0xEu;
    case 'f': case 'F': return 0xFu;
    default: return 0u;
    }
}

static char* __kumir_deserialize_hex(const char* s)
{
    const size_t buffer_size = (stringLength(s) + 1) / 3;
    char* bytes = (char*)malloc(buffer_size);
    for (size_t i = 0; i < buffer_size; ++i) {
        unsigned char h = __kumir_hex_to_byte(s[i * 3]);
        unsigned char l = __kumir_hex_to_byte(s[i * 3 + 1]);
        unsigned char byte = h << 4 | l;
        bytes[i] = byte;
    }
    return bytes;
}

EXTERN void __kumir_fill_array_i(__kumir_array* array, const char* data)
{
    char* buffer = __kumir_deserialize_hex(data);
    if (1u == array->dim) __kumir_fill_array_1(array, buffer, __KUMIR_INT);
    free(buffer);
}

EXTERN void __kumir_fill_array_r(__kumir_array* array, const char* data)
{
    char* buffer = __kumir_deserialize_hex(data);
    if (1u == array->dim) __kumir_fill_array_1(array, buffer, __KUMIR_REAL);
    free(buffer);
}

EXTERN void __kumir_fill_array_b(__kumir_array* array, const char* data)
{
    char* buffer = __kumir_deserialize_hex(data);
    if (1u == array->dim) __kumir_fill_array_1(array, buffer, __KUMIR_BOOL);
    free(buffer);
}

EXTERN void __kumir_fill_array_c(__kumir_array* array, const char* data)
{
    char* buffer = __kumir_deserialize_hex(data);
    if (1u == array->dim) __kumir_fill_array_1(array, buffer, __KUMIR_CHAR);
    free(buffer);
}

EXTERN void __kumir_fill_array_s(__kumir_array* array, const char* data)
{
    char* buffer = __kumir_deserialize_hex(data);
    if (1u == array->dim) __kumir_fill_array_1(array, buffer, __KUMIR_STRING);
    free(buffer);
}

EXTERN void __kumir_get_array_1_element(__kumir_scalar** result,
    bool value_expected,
    __kumir_array* array,
    const __kumir_scalar* x)
{
    __kumir_scalar* data = (__kumir_scalar*)(array->data);
    __kumir_check_value_defined(x);
    __kumir_int xx = x->data.i;
    if (xx < array->shape_left[0] || xx > array->shape_right[0]) {
        __kumir_abort_on_error("Out of bounds");
    }
    else {
        const size_t index = (size_t)(xx - array->size_left[0]);
        if (value_expected && !data[index].defined) {
            __kumir_abort_on_error("Table element value undefined");
        }
        *result = data + index;
    }
}

EXTERN void __kumir_get_array_2_element(__kumir_scalar** result,
    bool value_expected,
    __kumir_array* array,
    const __kumir_scalar* y,
    const __kumir_scalar* x)
{
    __kumir_scalar* data = (__kumir_scalar*)(array->data);
    __kumir_check_value_defined(x);
    __kumir_check_value_defined(y);
    __kumir_int xx = x->data.i;
    __kumir_int yy = y->data.i;
    if (yy < array->shape_left[0] || yy > array->shape_right[0]) {
        __kumir_abort_on_error("Out of bounds");
    }
    else if (xx < array->shape_left[1] || xx > array->shape_right[1]) {
        __kumir_abort_on_error("Out of bounds");
    }
    else {
        const size_t size_x = (size_t)(1 + array->size_right[1] - array->size_left[1]);
        const size_t index = (size_t)(
            (size_x * (yy - array->size_left[0])) +
            (xx - array->size_left[1])
            );
        if (value_expected && !data[index].defined) {
            __kumir_abort_on_error("Table element value undefined");
        }
        *result = &data[index];
    }
}

EXTERN void __kumir_get_array_3_element(__kumir_scalar** result,
    bool value_expected,
    __kumir_array* array,
    const __kumir_scalar* z,
    const __kumir_scalar* y,
    const __kumir_scalar* x)
{
    __kumir_scalar* data = (__kumir_scalar*)(array->data);
    __kumir_check_value_defined(x);
    __kumir_check_value_defined(y);
    __kumir_check_value_defined(z);
    __kumir_int xx = x->data.i;
    __kumir_int yy = y->data.i;
    __kumir_int zz = z->data.i;
    if (zz < array->shape_left[0] || zz > array->shape_right[0]) {
        __kumir_abort_on_error("Out of bounds");
    }
    else if (yy < array->shape_left[1] || yy > array->shape_right[1]) {
        __kumir_abort_on_error("Out of bounds");
    }
    else if (xx < array->shape_left[2] || xx > array->shape_right[2]) {
        __kumir_abort_on_error("Out of bounds");
    }
    else {
        const size_t size_y = (size_t)(1 + array->size_right[1] - array->size_left[1]);
        const size_t size_x = (size_t)(1 + array->size_right[2] - array->size_left[2]);
        const size_t index = (size_t)(
            (size_y * size_x * (zz - array->size_left[0])) +
            (size_x * (yy - array->size_left[1])) +
            (xx - array->size_left[2])
            );
        if (value_expected && !data[index].defined) {
            __kumir_abort_on_error("Table element value undefined");
        }
        *result = &data[index];
    }
}