#ifndef __KEYSCAN_H
#define __KEYSCAN_H

#define KeyPowr		LL_GPIO_IsInputPinSet(GPIOB, LL_GPIO_PIN_7)//

#define KEY_POWR_MASK		0x01
#define POWER_ON	0
#define POWER_OFF	0XFF

void keyscan_pin_init(void);
void exti_init_beforEnterStop(uint8_t type);
void check_the_powers_status(void);
void press_key_continue(void);
uint8_t return_powers(void);




#endif
