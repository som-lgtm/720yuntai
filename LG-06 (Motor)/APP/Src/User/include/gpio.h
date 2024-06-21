#ifndef __GPIO_H
#define __GPIO_H
#include "stm32f0xx.h"
#include "common.h"
#include "stm32f0xx_ll_gpio.h"

typedef enum
{
  RESOLUTION_8 = 1,
  RESOLUTION_16,
  RESOLUTION_32,
  RESOLUTION_64,
}MOTOR_RESOLUTION;




#define RED_LED_OFF	LL_GPIO_SetOutputPin(GPIOC, LL_GPIO_PIN_13) //RED
#define RED_LED_ON	LL_GPIO_ResetOutputPin(GPIOC, LL_GPIO_PIN_13) //RED

#define GREEN_LED_OFF	LL_GPIO_ResetOutputPin(GPIOB, LL_GPIO_PIN_9) //GREEN
#define GREEN_LED_ON	LL_GPIO_SetOutputPin(GPIOB, LL_GPIO_PIN_9) //GREEN

#define SHUTTER_ON	LL_GPIO_SetOutputPin(GPIOB, LL_GPIO_PIN_8) //
#define SHUTTER_OFF	LL_GPIO_ResetOutputPin(GPIOB, LL_GPIO_PIN_8) //

#define BLE_SHUT_ON	LL_GPIO_SetOutputPin(GPIOA, LL_GPIO_PIN_10) //
#define BLE_SHUT_OFF	LL_GPIO_ResetOutputPin(GPIOA, LL_GPIO_PIN_10) //

#define BLE_PWR_UP	LL_GPIO_SetOutputPin(GPIOA, LL_GPIO_PIN_9) //
#define BLE_PWR_DOWN	LL_GPIO_ResetOutputPin(GPIOA, LL_GPIO_PIN_9) //

//#define MOTOR_SLEEP	LL_GPIO_ResetOutputPin(GPIOA, LL_GPIO_PIN_4) //
//#define MOTOR_WACKUP	LL_GPIO_SetOutputPin(GPIOA, LL_GPIO_PIN_4) //
//#define MOTOR_DIAG		LL_GPIO_IsInputPinSet(GPIOF, LL_GPIO_PIN_13)
#define MOTOR_PND_OFF	LL_GPIO_ResetOutputPin(GPIOB, LL_GPIO_PIN_10) //
#define MOTOR_PND_ON	LL_GPIO_SetOutputPin(GPIOB, LL_GPIO_PIN_10) //

#define MOTORVV_PND_OFF	LL_GPIO_ResetOutputPin(GPIOB, LL_GPIO_PIN_10) //
#define MOTORVV_PND_ON	LL_GPIO_SetOutputPin(GPIOB, LL_GPIO_PIN_10) //

#define BATTER_MOS_ON LL_GPIO_SetOutputPin(GPIOA, LL_GPIO_PIN_7)
#define BATTER_MOS_OFF LL_GPIO_ResetOutputPin(GPIOA, LL_GPIO_PIN_7)

#define USB_MOS_ON LL_GPIO_SetOutputPin(GPIOA, LL_GPIO_PIN_5)
#define USB_MOS_OFF LL_GPIO_ResetOutputPin(GPIOA, LL_GPIO_PIN_5)


//#define CHARGE_STATU LL_GPIO_IsInputPinSet(GPIOC, LL_GPIO_PIN_14)

#define MOTOR_PWR_ON LL_GPIO_SetOutputPin(GPIOB, LL_GPIO_PIN_2)// HT7178 EN
#define MOTOR_PWR_OFF LL_GPIO_ResetOutputPin(GPIOB, LL_GPIO_PIN_2)// HT7178 EN



#define USB_INT LL_GPIO_IsInputPinSet(GPIOA, LL_GPIO_PIN_4)
#define BLE_STATU LL_GPIO_IsInputPinSet(GPIOA, LL_GPIO_PIN_11)



void Gpio_init(void);
void Resolution_Configure(MOTOR_RESOLUTION Preassigned);
void GPIO_OUT_init(GPIO_TypeDef *GPIOx, uint32_t PinMask);
void ShutDown_AllIO_High_resistance_mode(void);
void pwrKey_exti_interrupt(void);
void ChangeKey_exti_interrupt(void);
void LED_indication(uint8_t flash_if);
void led_status(uint8_t status, uint8_t indexs);
void BLE_PWR_TurnOn(void);
void Green_led_onORoff(uint8_t status);

#endif
