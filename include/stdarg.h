#ifndef _STDARG_H
#define _STDARG_H

typedef uint8_t* va_list;
#define va_start(list, param) (list = (((va_list)&param) + sizeof(param)))
#define va_arg(list, type)    (*(type *)((list += sizeof(type)) - sizeof(type)))

#endif