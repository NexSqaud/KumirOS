#include <stdlib_c.h>
#include <debug.h>
#include <memory.h>

#define KUMIR_MAX_CALL_STACK_SIZE 4096

static int __kumir_call_stack_size = 0;
static int32_t __kumir_current_line_number = -1;

EXTERN void __kumir_internal_debug(int32_t code)
{
    DEBUG_PRINTF("Debug %d", code);
}

EXTERN void __kumir_check_call_stack()
{
    __kumir_call_stack_size++;
    if (__kumir_call_stack_size >= KUMIR_MAX_CALL_STACK_SIZE)
    {
        __kumir_abort_on_error("Call stack overflow!");
    }
}

EXTERN void __kumir_pop_call_stack_counter()
{
    __kumir_call_stack_size--;
}

EXTERN void __kumir_handle_abort()
{
    if (__kumir_current_line_number == -1)
    {
        DEBUG_PRINT("Runtime error: _getError");
    }
    else
    {
        DEBUG_PRINTF("Runtime error at %d: _getError", __kumir_current_line_number);
    }

    __kumir_halt();
}

EXTERN void __kumir_halt()
{
    DEBUG_PRINT("HALT!");

    while (true)
    {
        __asm__("hlt");
    }
}

EXTERN void __kumir_set_current_line_number(const int32_t line_no)
{
    __kumir_current_line_number = line_no;
}

EXTERN void __kumir_assert(const __kumir_scalar* assumption)
{
    __kumir_check_value_defined(assumption);
    bool value = assumption->data.b;
    if (!value) {
        DEBUG_PRINT("Assumption false!");
        __kumir_halt();
    }
}

EXTERN void __kumir_assert_internal(const bool assumption, int line)
{
    if (!assumption)
    {
        DEBUG_PRINTF("Assumption false at: %d", line);
        __kumir_halt();
    }
}

EXTERN void __kumir_abort_on_error(const char* message)
{
    DEBUG_PRINTF("Abort (line %d): %cs", __kumir_current_line_number, message);
    __kumir_halt();
}