#include <utils.h>

#include <rtc.h>

static uint32_t nextRand = 1;

uint32_t rand(void)
{
    nextRand = nextRand * 1103515245 + 12345;
    return (uint32_t)(nextRand / 65536) % 32768;
}

void srand(uint32_t seed)
{
    nextRand = seed;
}

uint64_t getTicksCount(void)
{
    uint32_t lowTick = 0;
    uint32_t highTick = 0;

    __asm__("rdtsc" : "=d"(highTick), "=a"(lowTick));

    return (uint64_t)(highTick) << 32 | lowTick;
}

void sleep(uint32_t ticks)
{
    uint64_t beginTicks = rtcGetTicksCount();

    while (rtcGetTicksCount() - beginTicks < ticks)
    {
        __asm__ volatile("nop");
    }
}
