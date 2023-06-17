#include <rtc.h>

#include <debug.h>
#include <idt.h>
#include <ports.h>
#include <utils.h>

uint32_t clockTicks = 0;

volatile rtcClockHandler clockHandler = NULL;

volatile void clockInterruptHandler(Registers regs)
{
    (void)regs;
    clockTicks++;

    if(clockHandler) clockHandler();

    portWriteByte(0x70, 0x0C);
    portReadByte(0x71);
}

void rtcInit(void)
{
    interruptAddHandler(0x28, clockInterruptHandler);

    portWriteByte(0x70, portReadByte(0x70) | 0x80);
    portReadByte(0x71);

    portWriteByte(0x70, 0x8B);
    uint8_t previous = portReadByte(0x71);
    portWriteByte(0x70, 0x8B);
    portWriteByte(0x71, previous | 0x40);

    portWriteByte(0x70, portReadByte(0x70) & 0x7F);
    portReadByte(0x71);
}

uint32_t rtcGetTicksCount(void)
{
    return clockTicks;
}

void rtcAddHandler(rtcClockHandler handler)
{
    clockHandler = handler;
}
