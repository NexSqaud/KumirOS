#include <debug.h>
#include <exceptions_handlers.h>
#include <idt.h>
#include <graphics.h>
#include <keyboard.h>
#include <rtc.h>
#include <stddef.h>
#include <stdint.h>

#include <stdlib_c.h>

extern uint32_t video_buffer;
extern int main();

void __attribute__((section(".text.main"), noreturn)) _init(void)
{
    debugInit();
    interruptsInit();

    interruptAddHandler(0, divisionError);
    interruptAddHandler(6, invalidOpcode);
    interruptAddHandler(11, segmentNotPresent);
    interruptAddHandler(13, generalProtectionFault);
    interruptAddHandler(14, pageFault);

    rtcInit();
    keyboardInit();
    vgaInit(video_buffer);

    __asm__("finit");

    DEBUG_PRINT("Kernel initialized!");

    int code = main();

    DEBUG_PRINTF("Kumir program finished with code %d", code);

    vgaFlipBuffer();

    while (true)
    {
        __asm__ volatile("hlt");
    }
}