#include <stdlib_c.h>

#include <debug.h>
#include <memory.h>
#include <rtc.h>

EXTERN void __kumir_init_stdlib()
{
    rtcAddHandler(Painter_update_screen);
}

EXTERN __kumir_real __kumir_scalar_as_real(const __kumir_scalar* scalar)
{
    __kumir_real result = 0.;
    if (__KUMIR_REAL == scalar->type) {
        result = scalar->data.r;
    }
    else if (__KUMIR_INT == scalar->type) {
        result = (__kumir_real)(scalar->data.i);
    }
    return result;
}

EXTERN void __kumir_output_stdout_ii(const __kumir_scalar* value, const __kumir_scalar_type type, const int format1, const int format2)
{
    UNUSED(format2);

    __kumir_check_value_defined(value);
    switch (type)
    {
    case __KUMIR_INT:
        debugPrintDec(value->data.i);
        break;
    case __KUMIR_REAL:
        debugPrintFloat(__kumir_scalar_as_real(value), format1);
        break;
    case __KUMIR_BOOL:
        debugPrintFormated(value->data.b ? "true" : "false");
        break;
    case __KUMIR_CHAR:
        debugPrintChar(value->data.c);
        break;
    case __KUMIR_STRING:
        debugPrintFormated("%w", value->data.s);
        break;
    default:
        break;
    }
}

EXTERN void __kumir_output_stdout(const char* utf8)
{
    __kumir_scalar value;
    __kumir_create_string(&value, utf8);
    __kumir_output_stdout_ii(&value, __KUMIR_STRING, 0, 0);
    __kumir_free_scalar(&value);
}

EXTERN void __kumir_output_stdout_is(const __kumir_scalar* value, const __kumir_scalar_type type, const int format1, const __kumir_scalar* format2)
{
    __kumir_check_value_defined(format2);
    __kumir_output_stdout_ii(value, type, format1, format2->data.i);
}

EXTERN void __kumir_output_stdout_si(const __kumir_scalar* value, const __kumir_scalar_type type, const __kumir_scalar* format1, const int format2)
{
    __kumir_check_value_defined(format1);
    __kumir_output_stdout_ii(value, type, format1->data.i, format2);
}

EXTERN void __kumir_output_stdout_ss(const __kumir_scalar* value, const __kumir_scalar_type type, const __kumir_scalar* format1, const __kumir_scalar* format2)
{
    __kumir_check_value_defined(format1);
    __kumir_check_value_defined(format2);
    __kumir_output_stdout_ii(value, type, format1->data.i, format2->data.i);
}

EXTERN void __kumir_output_file_ii(const __kumir_scalar* handle, const __kumir_scalar* value, const __kumir_scalar_type type, const int format1, const int format2)
{
    UNUSED(handle);
    UNUSED(value);
    UNUSED(type);
    UNUSED(format1);
    UNUSED(format2);
}

EXTERN void __kumir_output_file_is(const __kumir_scalar* handle, const __kumir_scalar* value, const __kumir_scalar_type type, const int format1, const __kumir_scalar* format2)
{
    UNUSED(handle);
    UNUSED(value);
    UNUSED(type);
    UNUSED(format1);
    UNUSED(format2);
}

EXTERN void __kumir_output_file_si(const __kumir_scalar* handle, const __kumir_scalar* value, const __kumir_scalar_type type, const __kumir_scalar* format1, const int format2)
{
    UNUSED(handle);
    UNUSED(value);
    UNUSED(type);
    UNUSED(format1);
    UNUSED(format2);
}

EXTERN void __kumir_output_file_ss(const __kumir_scalar* handle, const __kumir_scalar* value, const __kumir_scalar_type type, const __kumir_scalar* format1, const __kumir_scalar* format2)
{
    UNUSED(handle);
    UNUSED(value);
    UNUSED(type);
    UNUSED(format1);
    UNUSED(format2);
}

EXTERN void __kumir_input_stdin(const __kumir_int format, __kumir_scalar** ptr)
{
    UNUSED(format);
    UNUSED(ptr);
}

EXTERN void __kumir_input_file(const __kumir_scalar* handle, const __kumir_int format, __kumir_scalar** ptr)
{
    UNUSED(handle);
    UNUSED(format);
    UNUSED(ptr);
}

// ------------------------------------------------------------

EXTERN void __kumir_get_scalar_argument(const char* argName, const __kumir_int format, __kumir_scalar* res)
{
    UNUSED(argName);
    UNUSED(format);
    UNUSED(res);
}

EXTERN void __kumir_get_array_argument(const char* argName, const __kumir_int format, __kumir_array* res)
{
    UNUSED(argName);
    UNUSED(format);
    UNUSED(res);
}

EXTERN void __kumir_set_main_arguments(int argc, char** argv)
{
    UNUSED(argc);
    UNUSED(argv);
}

typedef struct {
    int32_t counter;
    int32_t from;
    int32_t to;
    int32_t step;
} for_spec;

static uint32_t for_counters_count = 0;
static uint32_t for_counters_size = 0;
static for_spec** for_counters = NULL;

static void __add_for_spec(for_spec spec)
{
    if (for_counters_count + 1 >= for_counters_size)
    {
        for_spec** newList = malloc(IMAX(for_counters_size, 1) * sizeof(for_spec*) * 2);
        for_counters_size = IMAX(for_counters_size, 1) * 2;

        for (size_t i = 0; i < for_counters_count; i++)
        {
            newList[i] = for_counters[i];
        }

        free(for_counters);
        for_counters = newList;
    }

    for_counters[for_counters_count] = malloc(sizeof(for_spec));
    for_counters[for_counters_count]->counter = spec.counter;
    for_counters[for_counters_count]->from = spec.from;
    for_counters[for_counters_count]->to = spec.to;
    for_counters[for_counters_count]->step = spec.step;

    for_counters_count++;
}

static for_spec* __get_for_spec(void)
{
    if (for_counters_count == 0)
    {
        __kumir_abort_on_error("Loop counter list is empty");
    }

    return for_counters[for_counters_count - 1];
}

static void __remove_for_spec(void)
{
    if (for_counters_count == 0)
    {
        return;
    }

    free(for_counters[for_counters_count--]);
}

EXTERN void __kumir_loop_for_from_to_init_counter(const __kumir_scalar* from, const __kumir_scalar* to)
{
    for_spec spec;
    spec.counter = from->data.i - 1;
    spec.from = from->data.i;
    spec.to = to->data.i;
    spec.step = 1;
    __add_for_spec(spec);
}

EXTERN void __kumir_loop_for_from_to_step_init_counter(const __kumir_scalar* from, const __kumir_scalar* to, const __kumir_scalar* step)
{
    for_spec spec;
    spec.counter = from->data.i - step->data.i;
    spec.from = from->data.i;
    spec.to = to->data.i;
    spec.step = step->data.i;
    __add_for_spec(spec);
}

EXTERN __kumir_bool __kumir_loop_for_check_counter(__kumir_scalar* variable)
{
    for_spec* spec = __get_for_spec();
    int32_t* i = &spec->counter;
    const int32_t f = spec->from;
    const int32_t t = spec->to;
    const int32_t s = spec->step;
    *i += s;
    bool result = s >= 0
        ? f <= *i && *i <= t
        : t <= *i && *i <= f;
    if (result) {
        variable->data.i = *i;
        variable->defined = true;
        variable->type = __KUMIR_INT;
    }
    return result;
}

EXTERN void __kumir_loop_times_init_counter(const __kumir_scalar* from)
{
    for_spec spec;
    spec.counter = from->data.i;
    __add_for_spec(spec);
}

EXTERN __kumir_bool __kumir_loop_times_check_counter()
{
    for_spec* times = __get_for_spec();
    if (times->counter > 0) {
        times->counter--;
        return true;
    }
    else {
        return false;
    }
}

EXTERN void __kumir_loop_end_counter()
{
    __remove_for_spec();
}

