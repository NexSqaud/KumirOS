#ifndef _STDINT_H
#define _STDINT_H

#define BYTE_MAX 0x7F
#define BYTE_MIN 0xFF

#define SBYTE_MAX 0xFF
#define SBYTE_MIN 0

#define SHORT_MAX 0x7FFF
#define SHORT_MIN 0xFFFF

#define USHORT_MAX 0xFFFF
#define USHORT_MIN 0

#define INT_MAX 0x7FFFFFFF
#define INT_MIN 0xFFFFFFFF

#define UINT_MAX 0xFFFFFFFF
#define UINT_MIN 0

#define SIZE_MAX UINT_MAX
#define SIZE_MIN 0

typedef unsigned char uint8_t;
typedef char int8_t;

typedef unsigned short uint16_t;
typedef short int16_t;

typedef unsigned int uint32_t;
typedef int int32_t;

typedef unsigned long long uint64_t;
typedef long long int64_t;

typedef __WCHAR_TYPE__ wchar_t;
typedef uint32_t size_t;

#endif