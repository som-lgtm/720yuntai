#ifndef __KEYSCAN_H
#define __KEYSCAN_H

#define CHARGING	0

void Forbidden_Used(void);
void Adc_Gpio_init(void);
void RTC_AlarmConfig(void);
void set_power_on_int(uint8_t tatus);
void enable_interrupt(void);
void disable_interrupt(void);
extern uint8_t yun_start;

#endif

