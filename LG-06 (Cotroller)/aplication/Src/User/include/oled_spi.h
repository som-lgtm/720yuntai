#ifndef __OLED_SPI_H
#define __OLED_SPI_H

#include "stm32f0xx_ll_gpio.h"


//#define I2C_WITH_FW
//#define I2C_WITH_HW
#define OLED_SPI
#define X_WIDTH 128
#define Y_WIDTH 64


#ifdef OLED_SPI
#define OLED_POWER_ON 		LL_GPIO_SetOutputPin(GPIOA, LL_GPIO_PIN_8)
#define OLED_POWER_OFF 	LL_GPIO_ResetOutputPin(GPIOA, LL_GPIO_PIN_8)

#define OLED_CS_ON 		LL_GPIO_SetOutputPin(GPIOB, LL_GPIO_PIN_14)
#define OLED_CS_OFF 	LL_GPIO_ResetOutputPin(GPIOB, LL_GPIO_PIN_14)

#define OLED_DC_ON 		LL_GPIO_SetOutputPin(GPIOB, LL_GPIO_PIN_15)
#define OLED_DC_OFF 	LL_GPIO_ResetOutputPin(GPIOB, LL_GPIO_PIN_15);

#define OLED_SDA_ON 		LL_GPIO_SetOutputPin(GPIOA, LL_GPIO_PIN_11)
#define OLED_SDA_OFF 	LL_GPIO_ResetOutputPin(GPIOA, LL_GPIO_PIN_11);

#define OLED_SCL_ON 		LL_GPIO_SetOutputPin(GPIOA, LL_GPIO_PIN_12)
#define OLED_SCL_OFF 	LL_GPIO_ResetOutputPin(GPIOA, LL_GPIO_PIN_12);

void oled_gpio_init(void);
void Oled_EnlPrint(uint8_t x, uint8_t y, uint8_t *en,uint8_t *ch, uint8_t language);
void oled_spi_test(void);
void LCD_Fill(uint8_t bmp_data);
void LCD_PutPixel(uint8_t x,uint8_t y, uint8_t p_data);
void arrows_display(uint8_t x, uint8_t y, uint8_t status);
void battery_display(/*uint8_t x, uint8_t y, */uint16_t status);
void battery_data_display(float data);
void oled_display_6x8str(uint8_t x, uint8_t y, uint8_t *en);
void led_gpio_init(void);
void Angle_desplay(uint8_t x, uint8_t y);
void LCD_CLS_y(char y, uint8_t status);
void LCD_CLS_line_area(uint8_t start_x,uint8_t start_y,uint8_t width, uint8_t p_data);
void LCD_Put_Column(uint8_t x,uint8_t y,uint8_t data);
void LCD_Set_Pos(uint8_t x, uint8_t y);
void battery_display_off(void);


#endif
void i2c_test(void);
void I2C_Configuration(void);
void OLED_Init(void);

#endif
