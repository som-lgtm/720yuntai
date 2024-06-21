#ifndef __MOTOR_DRI_H
#define __MOTOR_DRI_H

#include "stm32f0xx.h"
//#define A_to_B 0
//#define B_to_A 0xff
#define ORBITAL_SPEED_MAX 108
#define DELAY_M_SPEED			85

//#define LED_ON	LL_GPIO_SetOutputPin(GPIOB, LL_GPIO_PIN_15) //
//#define LED_OFF	LL_GPIO_ResetOutputPin(GPIOB, LL_GPIO_PIN_15) //

//#define SHUTTER_ON	LL_GPIO_SetOutputPin(GPIOB, LL_GPIO_PIN_3) //
//#define SHUTTER_OFF	LL_GPIO_ResetOutputPin(GPIOB, LL_GPIO_PIN_3) //

//#define SHUTTER_ON11	LL_GPIO_SetOutputPin(GPIOA, LL_GPIO_PIN_7) //
//#define SHUTTER_OFF11	LL_GPIO_ResetOutputPin(GPIOA, LL_GPIO_PIN_7) //

//#define LED_ON	LL_GPIO_SetOutputPin(GPIOB, LL_GPIO_PIN_0) //
//#define LED_OFF	LL_GPIO_ResetOutputPin(GPIOB, LL_GPIO_PIN_0) //

//#define LED_OFF11	LL_GPIO_ResetOutputPin(GPIOB, LL_GPIO_PIN_6) //
//#define LED_ON11	LL_GPIO_SetOutputPin(GPIOB, LL_GPIO_PIN_6) //


//#define BLUE_STATUS	LL_GPIO_IsInputPinSet(GPIOA, LL_GPIO_PIN_11)

void delay_mode_init(void);
void check_bule_sataus(void);
void set_slowly_now(uint8_t datat);
void motor_speed_set(uint32_t speedd);
uint32_t speed_calculat(uint8_t p_speed);
void yuntai_speed_write(uint32_t speedd);


typedef enum{
	PRESCALE_1 = 1,
	PRESCALE_2,	
	PRESCALE_4,
	PRESCALE_8,
	PRESCALE_16,
	PRESCALE_32,
	PRESCALE_64,
	PRESCALE_128	
}PRESCALE_EMUN;


#endif

