#include <debug.h>
#include <idt.h>
#include <stddef.h>

#ifndef _EXCEPTIONS_HANDLER_H
#define _EXCEPTIONS_HANDLER_H

#define NORETURN __attribute__((noreturn))
#define HALT() while(1) __asm__ volatile("hlt")
    
#define LOG_EXCEPTION() \
                debugPrintFormated("EXCEPTION: %s\n"    \
                "EAX: %x\n"                             \
                "ECX: %x\n"                             \
                "EDX: %x\n"                             \
                "EBX: %x\n"                             \
                "ESP: %x\n"                             \
                "EBP: %x\n"                             \
                "ESI: %x\n"                             \
                "EDI: %x\n",                            \
                __func__,                               \
                regs.eax, regs.ecx, regs.edx, regs.ebx, \
                regs.esp, regs.ebp, regs.esi, regs.edi) 

#define DEFINE_DEFAULT_HANDLER(x) \
void NORETURN x(Registers regs)     \
{                                   \
    LOG_EXCEPTION();                \
                                    \
    HALT();                         \
}                                   \

DEFINE_DEFAULT_HANDLER(generalProtectionFault)
DEFINE_DEFAULT_HANDLER(divisionError)
DEFINE_DEFAULT_HANDLER(doubleFault)
DEFINE_DEFAULT_HANDLER(segmentNotPresent)
DEFINE_DEFAULT_HANDLER(invalidOpcode)
DEFINE_DEFAULT_HANDLER(overflow)
DEFINE_DEFAULT_HANDLER(pageFault)

#endif