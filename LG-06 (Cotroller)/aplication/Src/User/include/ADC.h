#ifndef __ADC_H
#define __ADC_H

#include "common.h"

#define ACDIN	GPIO_Pin_4
#define ADC_SWSTART                  ((uint32_t)0x000F0000) /*!< For ADC1, ADC2 and ADC3 */
#define ADC1_DR_Address    ((uint32_t)0x40012440) //ADC1 DR¼Ä´æÆ÷»ùµØÖ·
void battery_display_off(void);

void Baterry_Percentage_Dis(float valuee);
void Charging_Dis(void);
void filtering(void);
void percentage_get(void);

#endif
