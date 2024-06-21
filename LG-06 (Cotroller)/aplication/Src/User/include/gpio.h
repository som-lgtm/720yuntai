#ifndef __GPIO_H
#define __GPIO_H
#include "stm32f0xx.h"
#include "common.h"
#include "stm32f0xx_ll_spi.h"
#include "stm32f0xx_ll_gpio.h"

typedef enum
{
  RESOLUTION_8 = 1,
  RESOLUTION_16,
  RESOLUTION_32,
  RESOLUTION_64,
}MOTOR_RESOLUTION;


void Gpio_init(void);
void Spi_write_data(uint8_t datat);
void Resolution_Configure(MOTOR_RESOLUTION Preassigned);
void GPIO_INUP_Init(GPIO_TypeDef *GPIOx, uint32_t PinMask);
void ShutDown_AllIO_High_resistance_mode(void);
void IOHigh_mode(void);
void IO_recover(void);
void key_interrupt(uint8_t type);
void ChangeKey_exti_interrupt(void);
void pwrKey_exti_interrupt(void);
	
//#define OLED_SCLK_Clr() LL_GPIO_ResetOutputPin(GPIOA, LL_GPIO_PIN_12)//CLK
//#define OLED_SCLK_Set() LL_GPIO_SetOutputPin(GPIOA, LL_GPIO_PIN_12)

//#define OLED_SDIN_Clr() LL_GPIO_ResetOutputPin(GPIOA, LL_GPIO_PIN_11)//DIN
//#define OLED_SDIN_Set() LL_GPIO_SetOutputPin(GPIOA, LL_GPIO_PIN_11)

#define OLED_RST_Clr() LL_GPIO_ResetOutputPin(GPIOB, LL_GPIO_PIN_14)//RES
#define OLED_RST_Set() LL_GPIO_SetOutputPin(GPIOB, LL_GPIO_PIN_14)

#define OLED_DC_Clr() LL_GPIO_ResetOutputPin(GPIOA, LL_GPIO_PIN_8)//DC
#define OLED_DC_Set() LL_GPIO_SetOutputPin(GPIOA, LL_GPIO_PIN_8)
 		     
#define OLED_BLK_Clr()  LL_GPIO_ResetOutputPin(GPIOB, LL_GPIO_PIN_1)//CS
#define OLED_BLK_Set()  LL_GPIO_SetOutputPin(GPIOB, LL_GPIO_PIN_1)

#define SHUTTER_ON	LL_GPIO_SetOutputPin(GPIOB, LL_GPIO_PIN_3) //
#define SHUTTER_OFF	LL_GPIO_ResetOutputPin(GPIOB, LL_GPIO_PIN_3) //

#define LED_ON	LL_GPIO_SetOutputPin(GPIOB, LL_GPIO_PIN_12) //
#define LED_OFF	LL_GPIO_ResetOutputPin(GPIOB, LL_GPIO_PIN_12) //

#define OLED_PWR_ON LL_GPIO_ResetOutputPin(GPIOA, LL_GPIO_PIN_9)
#define OLED_PWR_OFF LL_GPIO_SetOutputPin(GPIOA, LL_GPIO_PIN_9)

#define CH340N_PWR_OFF LL_GPIO_ResetOutputPin(GPIOB, LL_GPIO_PIN_9)
#define CH340N_PWR_ON LL_GPIO_SetOutputPin(GPIOB, LL_GPIO_PIN_9)

#define ENTER_ATS LL_GPIO_ResetOutputPin(GPIOB, LL_GPIO_PIN_11) // 2.4g MODE ENTER INTO AT STATUS
#define EXIT_ATS LL_GPIO_SetOutputPin(GPIOB, LL_GPIO_PIN_11) // 2.4g MODE EXIT INTO AT STATUS

#define BLE_WK_ON	LL_GPIO_SetOutputPin(GPIOB, LL_GPIO_PIN_2) // 2.4g MODE ENTER INTO AT STATUS
#define BLE_WK_OFF LL_GPIO_ResetOutputPin(GPIOB, LL_GPIO_PIN_2); // 2.4g MODE EXIT INTO AT STATUS

#define WIFI_PWR_ON	LL_GPIO_ResetOutputPin(GPIOB, LL_GPIO_PIN_10) // 2.4g MODE pwr on
#define WIFI_PWR_OFF LL_GPIO_SetOutputPin(GPIOB, LL_GPIO_PIN_10); // 2.4g MODE pwr off

#define BOOST_PWR_OFF LL_GPIO_ResetOutputPin(GPIOA, LL_GPIO_PIN_10)
#define BOOST_PWR_ON LL_GPIO_SetOutputPin(GPIOA, LL_GPIO_PIN_10)

#define CHARGE_STATUS LL_GPIO_IsInputPinSet(GPIOB, LL_GPIO_PIN_8)

#endif
