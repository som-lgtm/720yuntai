#ifndef PWM_H
#define PWM_H
#include "stm32f0xx.h"
#include "common.h"

#define TB6612_ACTIVE	GPIO_SetBits(GPIOB, GPIO_Pin_1)
#define TB6612_STANDBY	GPIO_ResetBits(GPIOB, GPIO_Pin_1)

#define START_WAY	0XFF

void timer3_pwm_init(void);
void pwm1_on(uint8_t duty_c);
void pwm1_off(void);
void pwm2_on(uint8_t duty_c);
void pwm2_off(void);
void pwm3_on(uint8_t duty_c);
void pwm3_off(void);
void pwm4_on(uint8_t duty_c);
void pwm4_off(void);
void orbital_adust_speed(uint8_t p_speed);
void orbital_forward_run(uint8_t speed);
void orbital_inversion_run(uint8_t speed);
void yunta_inversion_run(uint8_t speed);
void yunta_forward_run(uint8_t speed);
void orbital_stop_run(void);
void yunta_stop_run(void);
void capture_init(void);
void motor_force_stop(void);
void motor_run(void);
void motor_speed_adjust(uint8_t p_speed);
void motor_force_remove(void);
void motor_change_frequency(void);


#endif

