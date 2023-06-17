#include <stdint.h>

#ifndef _RTC_H
#define _RTC_H

#define RTC_CLOCK_FREQUENCY 1024

typedef void(*rtcClockHandler)();

void rtcInit(void);
uint32_t rtcGetTicksCount(void);
void rtcAddHandler(rtcClockHandler handler);

#endif