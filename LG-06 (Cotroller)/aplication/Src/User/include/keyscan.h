#ifndef __KEYSCAN_H
#define __KEYSCAN_H

#define KEY_UP_MASK	0x01
#define KEY_DOWN_MASK	0x02
#define KEY_LEFT_MASK	0x04
#define KEY_RIGHT_MASK	0x08
#define KEY_OK_MASK		0x10
#define KEY_PWR_MASK		 0x80
#define KEY_MAIN_MASK		0x40
#define SHUTTER_KEY_MASK	0x20
#define POWER_ON	0
#define POWER_OFF	0XFF

#define KEY_DOWN_VALUE	80
#define KEY_UP_VALUE		200
#define SHUTTER		LL_GPIO_IsInputPinSet(GPIOB, LL_GPIO_PIN_5)//
#define MAIN_KEY		LL_GPIO_IsInputPinSet(GPIOB, LL_GPIO_PIN_4)//
#define PWR_KEY		LL_GPIO_IsInputPinSet(GPIOA, LL_GPIO_PIN_15)//

#define KeyUp		LL_GPIO_IsInputPinSet(GPIOA, LL_GPIO_PIN_12)//
#define KeyDown		LL_GPIO_IsInputPinSet(GPIOA, LL_GPIO_PIN_11)//ADCIN4
#define KeyLeft		LL_GPIO_IsInputPinSet(GPIOB, LL_GPIO_PIN_6)//ADCIN1
#define KeyRight	LL_GPIO_IsInputPinSet(GPIOB, LL_GPIO_PIN_7)//ADCIN0
#define KeyOk		LL_GPIO_IsInputPinSet(GPIOB, LL_GPIO_PIN_3)//ADCIN2

void key_scan_init_function(void);
void page_return(void);
void combination_key_deal(void);
void press_ok_key_deal(void);
void delay_page_ok(void);
void press_OK_function(void);
void LowBattery_wakeup(void);
void press_key_continue(uint8_t dir);
void check_the_powers_status(void);
void Pwr_key_continue(void);
void set_power_on_int(uint8_t tatus);


#endif
