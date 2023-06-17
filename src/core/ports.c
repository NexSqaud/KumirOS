#include <ports.h>

void portWriteByte(uint16_t port, uint8_t data)
{
    __asm("outb %0, %1" :: "a"(data), "Nd"(port));
}

void portWriteWord(uint16_t port, uint16_t data)
{
    __asm("outw %0, %1" :: "a"(data), "Nd"(port));
}

void portWriteLong(uint16_t port, uint32_t data)
{
    __asm("outl %0, %1" :: "a"(data), "Nd"(port));
}

uint8_t portReadByte(uint16_t port)
{
    uint8_t value;
    __asm volatile("inb %1, %0" : "=a"(value) : "Nd"(port));
    return value;
}

uint16_t portReadWord(uint16_t port)
{
    uint8_t value;
    __asm volatile("inb %1, %0" : "=a"(value) : "Nd"(port));
    return value;
}

uint32_t portReadLong(uint16_t port)
{
    uint8_t value;
    __asm volatile("inb %1, %0" : "=a"(value) : "Nd"(port));
    return value;
}
