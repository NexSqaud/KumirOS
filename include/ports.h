#include <stdint.h>

#ifndef _PORTS_H
#define _PORTS_H

void portWriteByte(uint16_t port, uint8_t data);
void portWriteWord(uint16_t port, uint16_t data);
void portWriteLong(uint16_t port, uint32_t data);

uint8_t portReadByte(uint16_t port);
uint16_t portReadWord(uint16_t port);
uint32_t portReadLong(uint16_t port);

#endif