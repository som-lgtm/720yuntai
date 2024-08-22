#ifndef __LOW_POWER_H
#define __LOW_POWER_H


void Forbidden_Used(void);
void Adc_Gpio_init(void);
void disable_interrupt(void);
void enable_interrupt(void);
//rtc.c
void RTC_AlarmConfig(void);
void RTC_disable(void);

#endif

