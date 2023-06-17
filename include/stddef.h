#include <stdint.h>

#ifndef _STDDEF_H
#define _STDDEF_H

// check size in compile time, cause warning
#define COMPILE_WARN_SIZEOF(x) char (*__kaboom)[sizeof(x)] = 1

#define NULL (void*)0

typedef uint8_t bool;
#define true 1
#define false 0

#define POSITIVE_INFINITY (1.0/0.0)
#define NEGATIVE_INFINITY -POSITIVE_INFINITY

#define stringify(x) __internal_str(x)
#define __internal_str(x) #x

#define UNUSED(x) (void)(x)

#endif