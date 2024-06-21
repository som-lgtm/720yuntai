#ifndef __CONTROL_H
#define __CONTROL_H




typedef struct{
	uint8_t usart11_shadow_buffer[21];
	//uint8_t app_read_buffer[20];
	uint8_t shadow_send_size;
	uint8_t shadow_read_size;
}USART11_SHADOW;

#endif

