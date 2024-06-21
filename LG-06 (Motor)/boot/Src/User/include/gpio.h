#ifndef __GPIO_H
#define __GPIO_H
#include "stm32f0xx.h"
#include "common.h"
#include "stm32f0xx_ll_gpio.h"

typedef enum
{
  MRESOLUTION_8 = 1,
  MRESOLUTION_16,
  MRESOLUTION_32,
  MRESOLUTION_64,
}MOTOR_RESOLUTION;

void Gpio_init(void);
uint8_t key_check(void);
void Resolution_Configure(MOTOR_RESOLUTION Preassigned);
void GPIO_INUP_Init(GPIO_TypeDef *GPIOx, uint32_t PinMask);

#define LED_ON	LL_GPIO_SetOutputPin(GPIOB, LL_GPIO_PIN_9) //GREEN
#define LED_OFF	LL_GPIO_ResetOutputPin(GPIOB, LL_GPIO_PIN_9) //GREEN

#define RED_LED_OFF	LL_GPIO_SetOutputPin(GPIOC, LL_GPIO_PIN_13) //PWR_LED
#define RED_LED_ON	LL_GPIO_ResetOutputPin(GPIOC, LL_GPIO_PIN_13) //PWR_LED

#define SHUTTER_ON	LL_GPIO_SetOutputPin(GPIOB, LL_GPIO_PIN_14) //
#define SHUTTER_OFF	LL_GPIO_ResetOutputPin(GPIOB, LL_GPIO_PIN_14) //
#define USB_MOS_ON LL_GPIO_SetOutputPin(GPIOA, LL_GPIO_PIN_5)
#define USB_MOS_OFF LL_GPIO_ResetOutputPin(GPIOA, LL_GPIO_PIN_5)
#define BATTER_MOS_ON LL_GPIO_SetOutputPin(GPIOA, LL_GPIO_PIN_7)
#define BATTER_MOS_OFF LL_GPIO_ResetOutputPin(GPIOA, LL_GPIO_PIN_7)

//#define SHUTTER_ON11	LL_GPIO_SetOutputPin(GPIOB, LL_GPIO_PIN_15) //
//#define SHUTTER_OFF11	LL_GPIO_ResetOutputPin(GPIOB, LL_GPIO_PIN_15) //
#define KeyPowr		LL_GPIO_IsInputPinSet(GPIOB, LL_GPIO_PIN_7)//

#define BLUE_STATUS	LL_GPIO_IsInputPinSet(GPIOA, LL_GPIO_PIN_4)

#endif
