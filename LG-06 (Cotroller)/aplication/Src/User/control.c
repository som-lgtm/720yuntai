
uint8_t con_buffer[16] = {0};
uint8_t ramp_flag=0;
uint8_t slow_tag=0;
uint8_t dir_change_ramp = 0; // used for chageing dierections of ramp star and stop
uint8_t advi_time_limit = 0;

uint32_t max_pix_num = 0; // 最大张数
uint32_t slow_tal_time = 0; // 缓起缓停需要的总时间
uint32_t adv_speed_back=0;

USART11_SHADOW Usart11_shadow = {0};


uint8_t check_sum_add(uint8_t index, uint8_t *spt)
{
	uint8_t i;
	uint16_t temp=0;

	spt[0] = 0;
	
	for(i=1;i<index; i++)
	{
		temp += (uint16_t)spt[i];
	}

	//uart1_s_buffer[0] = temp & 0xff;
	//display_test11(uart1_s_buffer[0]);

	return (temp & 0xff);
}


