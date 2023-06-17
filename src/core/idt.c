#include <idt.h>

#include <stddef.h>
#include <ports.h>

#define SET_DEFAULT_HANDLER(index) \
    setInterruptGate(index, (uint32_t)int ## index, 0x08, 0x8E)

#define DEFAULT_INTERRUPT_HANDLER(index) \
void __attribute__((naked)) int ## index (void)     \
{                                                   \
    __asm__ volatile("cli");                        \
    __asm__ volatile("pushal");                     \
    __asm__ volatile("push %%eax" :: "a"(index));   \
    __asm__ volatile("call handleInterrupt");       \
    __asm__ volatile("pop %%eax" ::: "eax");        \
    __asm__ volatile("popal");                      \
    __asm__ volatile("sti");                        \
    __asm__ volatile("iret");                       \
}                                                   \

DEFAULT_INTERRUPT_HANDLER(0)
DEFAULT_INTERRUPT_HANDLER(1)
DEFAULT_INTERRUPT_HANDLER(2)
DEFAULT_INTERRUPT_HANDLER(3)
DEFAULT_INTERRUPT_HANDLER(4)
DEFAULT_INTERRUPT_HANDLER(5)
DEFAULT_INTERRUPT_HANDLER(6)
DEFAULT_INTERRUPT_HANDLER(7)
DEFAULT_INTERRUPT_HANDLER(8)
DEFAULT_INTERRUPT_HANDLER(9)
DEFAULT_INTERRUPT_HANDLER(10)
DEFAULT_INTERRUPT_HANDLER(11)
DEFAULT_INTERRUPT_HANDLER(12)
DEFAULT_INTERRUPT_HANDLER(13)
DEFAULT_INTERRUPT_HANDLER(14)
DEFAULT_INTERRUPT_HANDLER(15)
DEFAULT_INTERRUPT_HANDLER(16)
DEFAULT_INTERRUPT_HANDLER(17)
DEFAULT_INTERRUPT_HANDLER(18)
DEFAULT_INTERRUPT_HANDLER(19)
DEFAULT_INTERRUPT_HANDLER(20)
DEFAULT_INTERRUPT_HANDLER(21)
DEFAULT_INTERRUPT_HANDLER(22)
DEFAULT_INTERRUPT_HANDLER(23)
DEFAULT_INTERRUPT_HANDLER(24)
DEFAULT_INTERRUPT_HANDLER(25)
DEFAULT_INTERRUPT_HANDLER(26)
DEFAULT_INTERRUPT_HANDLER(27)
DEFAULT_INTERRUPT_HANDLER(28)
DEFAULT_INTERRUPT_HANDLER(29)
DEFAULT_INTERRUPT_HANDLER(30)
DEFAULT_INTERRUPT_HANDLER(31)
DEFAULT_INTERRUPT_HANDLER(32)
DEFAULT_INTERRUPT_HANDLER(33)
DEFAULT_INTERRUPT_HANDLER(34)
DEFAULT_INTERRUPT_HANDLER(35)
DEFAULT_INTERRUPT_HANDLER(36)
DEFAULT_INTERRUPT_HANDLER(37)
DEFAULT_INTERRUPT_HANDLER(38)
DEFAULT_INTERRUPT_HANDLER(39)
DEFAULT_INTERRUPT_HANDLER(40)
DEFAULT_INTERRUPT_HANDLER(41)
DEFAULT_INTERRUPT_HANDLER(42)
DEFAULT_INTERRUPT_HANDLER(43)
DEFAULT_INTERRUPT_HANDLER(44)
DEFAULT_INTERRUPT_HANDLER(45)
DEFAULT_INTERRUPT_HANDLER(46)
DEFAULT_INTERRUPT_HANDLER(47)

DEFAULT_INTERRUPT_HANDLER(0x80)

__attribute__((aligned(0x10)))
static InterruptHandler handlers[256];

__attribute__((aligned(0x10)))
static IdtEntry entries[256];
static IdtPtr idtPtr;

void handleInterrupt(int index, Registers regs);

void setInterruptGate(uint8_t index, uint32_t handler, uint16_t selector, uint16_t flags)
{
    entries[index].handlerLow = handler & 0xFFFF;
    entries[index].handlerHigh = (handler >> 16) & 0xFFFF;
    entries[index].selector = selector;
    entries[index].flags = flags;
}

void waitIO(void)
{
    portWriteByte(0x80, 0x0);
}

void interruptsInit(void)
{
    portWriteByte(0x20, 0x11);
    waitIO();
    portWriteByte(0xA0, 0x11);
    waitIO();

    portWriteByte(0x21, 0x20);
    waitIO();
    portWriteByte(0xA1, 0x28);
    waitIO();

    portWriteByte(0x21, 4);
    waitIO();
    portWriteByte(0xA1, 2);
    waitIO();

    portWriteByte(0x21, 0x1);
    waitIO();
    portWriteByte(0xA1, 0x1);
    waitIO();

    portWriteByte(0x21, 0x00);
    portWriteByte(0xA1, 0x00);

    idtPtr.size = (sizeof(IdtEntry) * 256) - 1;
    idtPtr.base = entries;

    SET_DEFAULT_HANDLER(0);
    SET_DEFAULT_HANDLER(1);
    SET_DEFAULT_HANDLER(2);
    SET_DEFAULT_HANDLER(3);
    SET_DEFAULT_HANDLER(4);
    SET_DEFAULT_HANDLER(5);
    SET_DEFAULT_HANDLER(6);
    SET_DEFAULT_HANDLER(7);
    SET_DEFAULT_HANDLER(8);
    SET_DEFAULT_HANDLER(9);
    SET_DEFAULT_HANDLER(10);
    SET_DEFAULT_HANDLER(11);
    SET_DEFAULT_HANDLER(12);
    SET_DEFAULT_HANDLER(13);
    SET_DEFAULT_HANDLER(14);
    SET_DEFAULT_HANDLER(15);
    SET_DEFAULT_HANDLER(16);
    SET_DEFAULT_HANDLER(17);
    SET_DEFAULT_HANDLER(18);
    SET_DEFAULT_HANDLER(19);
    SET_DEFAULT_HANDLER(20);
    SET_DEFAULT_HANDLER(21);
    SET_DEFAULT_HANDLER(22);
    SET_DEFAULT_HANDLER(23);
    SET_DEFAULT_HANDLER(24);
    SET_DEFAULT_HANDLER(25);
    SET_DEFAULT_HANDLER(26);
    SET_DEFAULT_HANDLER(27);
    SET_DEFAULT_HANDLER(28);
    SET_DEFAULT_HANDLER(29);
    SET_DEFAULT_HANDLER(30);
    SET_DEFAULT_HANDLER(31);
    SET_DEFAULT_HANDLER(32);
    SET_DEFAULT_HANDLER(33);
    SET_DEFAULT_HANDLER(34);
    SET_DEFAULT_HANDLER(35);
    SET_DEFAULT_HANDLER(36);
    SET_DEFAULT_HANDLER(37);
    SET_DEFAULT_HANDLER(38);
    SET_DEFAULT_HANDLER(39);
    SET_DEFAULT_HANDLER(40);
    SET_DEFAULT_HANDLER(41);
    SET_DEFAULT_HANDLER(42);
    SET_DEFAULT_HANDLER(43);
    SET_DEFAULT_HANDLER(44);
    SET_DEFAULT_HANDLER(45);
    SET_DEFAULT_HANDLER(46);
    SET_DEFAULT_HANDLER(47);

    SET_DEFAULT_HANDLER(0x80);

    __asm__ volatile("lidt %0" :: "m"(idtPtr));
    __asm__ volatile("sti");
}

void interruptAddHandler(uint8_t index, InterruptHandler handler)
{
    handlers[index] = handler;
}

void interruptRemoveHandler(uint8_t index)
{
    handlers[index] = NULL;
}

void handleInterrupt(int index, Registers regs)
{
    if (handlers[index])
    {
        handlers[index](regs);
    }

    if (index >= 0x20 && index < 0x30)
    {
        if (index >= 0x28 && index < 0x30)
        {
            portWriteByte(0xA0, 0x20);
        }
        portWriteByte(0x20, 0x20);
    }

}