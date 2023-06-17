#include <stdint.h>

#ifndef _IDT_H
#define _IDT_H

typedef struct
{
    uint32_t edi;
    uint32_t esi;
    uint32_t ebp;
    uint32_t esp;
    uint32_t ebx;
    uint32_t edx;
    uint32_t ecx;
    uint32_t eax;
    uint32_t value;
    uint32_t eflags;
    uint32_t cs;
    uint32_t eip;
    uint32_t errorCode;
} __attribute__((packed)) Registers;

typedef struct {
    uint16_t handlerLow;
    uint16_t selector;
    uint8_t padding;
    uint8_t flags;
    uint16_t handlerHigh;
} __attribute__((packed)) IdtEntry;

typedef struct {
    uint16_t size;
    IdtEntry* base;
} __attribute__((packed)) IdtPtr;

typedef void(*InterruptHandler)(Registers regs);

void interruptsInit(void);
void interruptAddHandler(uint8_t index, InterruptHandler handler);
void interruptRemoveHandler(uint8_t index);

#endif