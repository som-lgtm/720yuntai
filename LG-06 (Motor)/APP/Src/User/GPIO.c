
#include "gpio.h"
#include "common.h"
#include "stm32f0xx_ll_adc.h"
#include "stm32f0xx_ll_bus.h"
#include "stm32f0xx_ll_TIM.h"
#include "stm32f0xx_ll_exti.h"
#include "stm32f0xx_ll_system.h"

uint16_t led_time=0;
uint8_t brightness = 0;

void Gpio_time_count(void)
{
	if(led_time)led_time--;
}

extern uint8_t battery_percent;

void GPIO_OUT_init(GPIO_TypeDef *GPIOx, uint32_t PinMask)
{
	LL_GPIO_InitTypeDef GPIO_InitStruct;
	
	GPIO_InitStruct.Pin = PinMask;
	GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
	GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
	GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
	GPIO_InitStruct.Pull = LL_GPIO_PULL_UP;
	LL_GPIO_Init(GPIOx, &GPIO_InitStruct);
}

void GPIO_INUP_Init(GPIO_TypeDef *GPIOx, uint32_t PinMask)
{
	LL_GPIO_InitTypeDef GPIO_InitStruct;

	// GPIO INPUT INIT
	GPIO_InitStruct.Pin = PinMask;
	GPIO_InitStruct.Mode = LL_GPIO_MODE_INPUT;
	GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
	GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
	GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
	LL_GPIO_Init(GPIOx, &GPIO_InitStruct);

}

void gpio_pulldown_int(GPIO_TypeDef *GPIOx, uint32_t PinMask)
{
	LL_GPIO_InitTypeDef GPIO_InitStruct;
	
	GPIO_InitStruct.Pin = PinMask;
	GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
	GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
	GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_OPENDRAIN;
	GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
	LL_GPIO_Init(GPIOx, &GPIO_InitStruct);
}


void TMC22xx_Init(void)
{
	GPIO_OUT_init(GPIOB, LL_GPIO_PIN_0); //MOTOR H dir
//	GPIO_OUT_init(GPIOB, LL_GPIO_PIN_12); //MOTOR H PND
//	GPIO_OUT_init(GPIOB, LL_GPIO_PIN_13); //MOTOR H MS1
	
//	GPIO_OUT_init(GPIOB, LL_GPIO_PIN_14); //motor V dir
//	GPIO_OUT_init(GPIOA, LL_GPIO_PIN_12); //MOTOR V PND
//	GPIO_OUT_init(GPIOF, LL_GPIO_PIN_6); //MOTOR V MS1
}

void Tmc22xx_SetStatu(void)
{
//	Resolution_Configure(MRESOLUTION_32);
//	MOTOR_PND_ON;
//	MOTOR_PWR_OFF;
//	LL_GPIO_ResetOutputPin(GPIOB, LL_GPIO_PIN_13);//MOTOR H MS1

//	MOTORVV_PND_OFF;
//	MOTOR_PND_OFF;
//	LL_GPIO_ResetOutputPin(GPIOF, LL_GPIO_PIN_6);//MOTOR V MS1

}

void motor_reset(void)
{
/*	if(MOTOR_DIAG) // ?ì2aμ??úê?·?ê§2?
	{
		MOTOR_SLEEP;
		time_delay_ms(10);
		MOTOR_WACKUP;
		//time_delay_ms(10);
		//TMC2208_init();
	}*/
}

void Led_init(void)
{
	GPIO_OUT_init(GPIOB, LL_GPIO_PIN_9); // GREEN
	GPIO_OUT_init(GPIOC, LL_GPIO_PIN_13); // RED
	RED_LED_ON;
	//GREEN_LED_ON;
}


void shutter_gpio_init(void)
{
//	LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOC);
	GPIO_OUT_init(GPIOB, LL_GPIO_PIN_8); // shutter
}

void shutdown_motor_pwr(void)
{
	BATTER_MOS_OFF;
	USB_MOS_OFF;
}

void usb_status_initerrupt_init(void)
{
	LL_SYSCFG_SetEXTISource(LL_SYSCFG_EXTI_PORTA,  LL_SYSCFG_EXTI_LINE4); // key
	LL_EXTI_EnableIT_0_31(LL_EXTI_LINE_4);
	LL_EXTI_EnableFallingTrig_0_31(LL_EXTI_LINE_4);
	LL_EXTI_EnableRisingTrig_0_31(LL_EXTI_LINE_4);
	NVIC_SetPriority(EXTI4_15_IRQn, 0);
	NVIC_EnableIRQ(EXTI4_15_IRQn);
}

void motor_pwr_init(void)
{
	LL_GPIO_InitTypeDef GPIO_InitStruct;

	// GPIO INPUT INIT
	GPIO_InitStruct.Pin = LL_GPIO_PIN_4;// USB READ STATUS
	GPIO_InitStruct.Mode = LL_GPIO_MODE_INPUT;
	//GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
	GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
	GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
	LL_GPIO_Init(GPIOA, &GPIO_InitStruct);

//	GPIO_OUT_init(GPIOF, LL_GPIO_PIN_6); // CH340N pwr
	GPIO_OUT_init(GPIOA, LL_GPIO_PIN_7); // battery mos switch
	GPIO_OUT_init(GPIOA, LL_GPIO_PIN_5); // usb mos switch
//	GPIO_INUP_Init(GPIOC, LL_GPIO_PIN_13); // USB READ STATUS
	shutdown_motor_pwr();
	usb_status_initerrupt_init();
	BATTER_MOS_ON;
}




void time_delay_us(uint32_t tttt)
{
	uint8_t i=0;
	uint32_t j=0;
	for(i=0; i<100; i++)
	{
		for(j=0; j<tttt; j++)
			{}
	}
}

void usb_protection(void)
{
/*	static uint8_t voltage = 0;
	{
		if(voltage !=2)
		{
			voltage = 2;
			time_delay_us(200);
			if(!USB_INT)
			{
				//if(return_battery() == 0 ||return_battery() > SUPERVOTAGE)return;
				USB_MOS_OFF;
				time_delay_us(300);
				BATTER_MOS_ON;
			}
		}
	}*/
}

void ble_init(void)
{
	LL_GPIO_InitTypeDef GPIO_InitStruct;

	// GPIO INPUT INIT
	GPIO_InitStruct.Pin = LL_GPIO_PIN_11; // statu
	GPIO_InitStruct.Mode = LL_GPIO_MODE_INPUT;
	GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
	GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
	GPIO_InitStruct.Pull = LL_GPIO_PULL_DOWN;
	LL_GPIO_Init(GPIOA, &GPIO_InitStruct);
	
	GPIO_OUT_init(GPIOA, LL_GPIO_PIN_10); // ble SHUT
	//GPIO_INUP_Init(GPIOB, LL_GPIO_PIN_15); // statu
	GPIO_OUT_init(GPIOA, LL_GPIO_PIN_9); // pwr key
	GPIO_OUT_init(GPIOA, LL_GPIO_PIN_8); // pwr GND
	BLE_SHUT_ON;
	BLE_PWR_UP;
	LL_GPIO_SetOutputPin(GPIOA, LL_GPIO_PIN_8);
}

void Adc_Gpio_init(void)
{
	LL_GPIO_InitTypeDef GPIO_InitStruct;

	/* Peripheral clock enable */
	LL_APB1_GRP2_EnableClock(LL_APB1_GRP2_PERIPH_ADC1);
	
	/**ADC GPIO Configuration  
	PA0   ------> ADC_IN0 
	*/
	GPIO_InitStruct.Pin = LL_GPIO_PIN_0;
	GPIO_InitStruct.Mode = LL_GPIO_MODE_ANALOG;
	GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
	LL_GPIO_Init(GPIOA, &GPIO_InitStruct);
}

void STM32f030_pf_init(void)
{
//	LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOF);
//	GPIO_OUT_init(GPIOF, LL_GPIO_PIN_6); // red
//	GPIO_OUT_init(GPIOF, LL_GPIO_PIN_7); // green
//	LL_GPIO_ResetOutputPin(GPIOF, LL_GPIO_PIN_6);
//	LL_GPIO_SetOutputPin(GPIOF, LL_GPIO_PIN_7);
}


void changeCheck_IOInit(void)
{
	LL_GPIO_InitTypeDef GPIO_InitStruct;

	// GPIO INPUT INIT
	GPIO_InitStruct.Pin = LL_GPIO_PIN_14;
	GPIO_InitStruct.Mode = LL_GPIO_MODE_INPUT;
	//GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
	GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
	GPIO_InitStruct.Pull = LL_GPIO_PULL_UP;
	LL_GPIO_Init(GPIOC, &GPIO_InitStruct);
	
	//GPIO_INUP_Init(GPIOC, LL_GPIO_PIN_14); // charge
}

void key_gpio_init(void)
{
	LL_GPIO_InitTypeDef GPIO_InitStruct;

	// GPIO INPUT INIT
	GPIO_InitStruct.Pin = LL_GPIO_PIN_7;
	GPIO_InitStruct.Mode = LL_GPIO_MODE_INPUT;
	GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
	GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
	GPIO_InitStruct.Pull = LL_GPIO_PULL_UP;
	LL_GPIO_Init(GPIOB, &GPIO_InitStruct);
}

void Device_init(void)
{
	motor_pwr_init();
	TMC22xx_Init();
	Led_init();
	ble_init();
	shutter_gpio_init();
	STM32f030_pf_init();
	Tmc22xx_SetStatu();
	key_gpio_init();
	//usb_voltage_tatus();
}



void usb_voltage_tatus(void)
{
	if(USB_STATUS)
	{
		time_delay_us(500);
		if(USB_STATUS)
		{
			BATTER_MOS_OFF;
			time_delay_us(300);
			USB_MOS_ON;
		}
	}
	else
	{
		time_delay_us(500);
		if(!USB_STATUS)
		{
			//if(return_battery() == 0 ||return_battery() > SUPERVOTAGE)return;
			USB_MOS_OFF;
			time_delay_us(300);
			BATTER_MOS_ON;
		}
	}
}


void Resolution_Configure(MOTOR_RESOLUTION Preassigned)
{
/*	switch(Preassigned)
	{
		case MRESOLUTION_8:
		{
			LL_GPIO_ResetOutputPin(GPIOA, LL_GPIO_PIN_7); // MS2
			LL_GPIO_ResetOutputPin(GPIOB, LL_GPIO_PIN_0); //MS1
			break;
		}
		case MRESOLUTION_16:
		{
			LL_GPIO_SetOutputPin(GPIOA, LL_GPIO_PIN_7); // MS2	
			LL_GPIO_SetOutputPin(GPIOB, LL_GPIO_PIN_0); //MS1
			break;
		}
		case MRESOLUTION_32:
		{
			LL_GPIO_ResetOutputPin(GPIOA, LL_GPIO_PIN_7); // MS2
			LL_GPIO_SetOutputPin(GPIOB, LL_GPIO_PIN_0); //MS1
			break;
		}
		case MRESOLUTION_64:
		{
			LL_GPIO_SetOutputPin(GPIOA, LL_GPIO_PIN_7); // MS2
			LL_GPIO_ResetOutputPin(GPIOB, LL_GPIO_PIN_0); //MS1
			break;
		}
	}*/
}

void input_opendrain(GPIO_TypeDef *GPIOx, uint32_t PinMask)
{
	LL_GPIO_InitTypeDef GPIO_InitStruct;

	// GPIO INPUT INIT
	GPIO_InitStruct.Pin = PinMask;
	GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
	GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
	GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_OPENDRAIN;
	GPIO_InitStruct.Pull =LL_GPIO_PULL_DOWN;
	LL_GPIO_Init(GPIOx, &GPIO_InitStruct);
}

// 2.4G模块需要把(IQR)A2 AND (MOSI)A7设置为高阻状
void ShutDown_AllIO_High_resistance_mode(void)
{
	/*LL_GPIO_InitTypeDef GPIO_InitStruct;

	// GPIO INPUT INIT
	GPIO_InitStruct.Pin = LL_GPIO_PIN_6;
	GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
	GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
	GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
	GPIO_InitStruct.Pull = LL_GPIO_PULL_DOWN;
	LL_GPIO_Init(GPIOA, &GPIO_InitStruct);*/
		input_opendrain(GPIOA,LL_GPIO_PIN_ALL&(~(LL_GPIO_PIN_14|LL_GPIO_PIN_13|LL_GPIO_PIN_8)));
		input_opendrain(GPIOB,LL_GPIO_PIN_ALL&(~(LL_GPIO_PIN_7)));	
		input_opendrain(GPIOC,LL_GPIO_PIN_ALL&(~(LL_GPIO_PIN_13)));
		input_opendrain(GPIOF,LL_GPIO_PIN_ALL);
		
		LL_GPIO_SetOutputPin(GPIOA, LL_GPIO_PIN_ALL&(~(LL_GPIO_PIN_14|LL_GPIO_PIN_13|LL_GPIO_PIN_8)));
		LL_GPIO_SetOutputPin(GPIOB,LL_GPIO_PIN_ALL&(~(LL_GPIO_PIN_7)));
		LL_GPIO_SetOutputPin(GPIOC,LL_GPIO_PIN_ALL&(~(LL_GPIO_PIN_13)));
		LL_GPIO_SetOutputPin(GPIOF,LL_GPIO_PIN_ALL);
}

void pwrKey_exti_interrupt(void)
{
	LL_SYSCFG_SetEXTISource(LL_SYSCFG_EXTI_PORTB,  LL_SYSCFG_EXTI_LINE7); // power key
	//LL_SYSCFG_SetEXTISource(LL_SYSCFG_EXTI_PORTC,  LL_SYSCFG_EXTI_LINE13); // CHARGE key
	LL_EXTI_EnableIT_0_31(LL_EXTI_LINE_7);
	LL_EXTI_EnableFallingTrig_0_31(LL_EXTI_LINE_7);
	
	//LL_EXTI_EnableFallingTrig_0_31(LL_EXTI_LINE_8);
	NVIC_SetPriority(EXTI4_15_IRQn, 0);
	NVIC_EnableIRQ(EXTI4_15_IRQn);
}

void ChangeKey_exti_interrupt(void)
{
	LL_SYSCFG_SetEXTISource(LL_SYSCFG_EXTI_PORTB,  LL_SYSCFG_EXTI_LINE7); // CHARGE key
	LL_EXTI_EnableIT_0_31(LL_EXTI_LINE_7);
	LL_EXTI_EnableFallingTrig_0_31(LL_EXTI_LINE_7);
	
	NVIC_SetPriority(EXTI4_15_IRQn, 0);
	NVIC_EnableIRQ(EXTI4_15_IRQn);
}

void led_status(uint8_t status, uint8_t indexs)
{
	if(indexs == 0)
	{
	//	if(status)LED_ON;
	//	else LED_OFF;
	}
	else if(indexs == 1)
	{
		if(status)
		{
			RED_LED_ON;
		}
		else
		{
			RED_LED_OFF;
		}
	}
	else if(indexs == 2)
	{
		if(status)
		{
			RED_LED_ON;
		}
		else
		{
			RED_LED_OFF;
		}
	}
	else if(indexs == 3)
	{
		if(status)
		{
			RED_LED_ON;
		}
		else
		{
			RED_LED_OFF;
		}
	}
}

void bluetooth_pair_led(void)
{
	static uint8_t p_status=0;
//	static uint8_t C_status=0;

/*	if(!CHARGE_STATU || !USB_INT)
	{
		if(!CHARGE_STATU)
		{
			if(C_status != 1)
			{
				C_status = 1;
				RED_LED_ON;
				GREEN_LED_OFF;
			}
		}
		else
		{
			if(C_status != 2)
			{
				C_status = 2;
				RED_LED_OFF;
				GREEN_LED_ON;
			}
		}
		
		return;
	}
	else */
	{
		if(find_pataVV.HHfind_Apoint || find_pata.HHfind_Apoint)return;
		if(mode_backup == PREINSTALL_MODE)if(con_b.begin==4)return;
		if(mode_backup == STANDAR_MODE || mode_backup == WIDE_ANGLE_MODE)if(move_begin==4)return;
		if(mode_backup == DELAY_SET)if(move_begin==3)return;
		//C_status = 0;
		if(Battery_percentage > 10)
		{
			//if(BLE_STATU==0)
			{
				if(led_time==0)
				{
					p_status = ~p_status;
					if(p_status)
					{
						led_time = 500;
						Green_led_onORoff(1);
						RED_LED_ON;
					}
					else
					{
						led_time = 2000;
						Green_led_onORoff(0);
					}
				}
			}
		}
		else
		{
			if(led_time == 0)
			{
				led_time = 200;
				p_status = ~p_status;
				if(p_status)
				{
					RED_LED_ON;
					Green_led_onORoff(1);
				}
				else
				{ 
					RED_LED_OFF;
					Green_led_onORoff(0);
				}
			}
		}
	}
}

void shutter_processing(uint8_t satuss)
{
	if(satuss)
	{
		SHUTTER_ON;
	}
	else
	{
		SHUTTER_OFF;
	}
}

void BLE_PWR_TurnOn(void)
{
	uint16_t tims=0;
	while(!BLE_STATU)
	{
		BLE_PWR_DOWN;
		time_delay_ms(10);
		tims += 1;
		if(tims > 300)break;
	}

	BLE_PWR_UP;
	
//	String_Printf(USART_2, &glob_para.speed, 1);
}

void Green_led_onORoff(uint8_t status)
{
	if(status)
	{
		brightness = 1;
	}
	else
	{
		brightness = 0;
		GREEN_LED_OFF;
	}
}

void Green_Led_Brightness(void)
{
	static uint8_t ness = 0;
	if(brightness == 0)return;
	ness += 1;
	
	if(ness < 2)
	{
		GREEN_LED_ON;
	}
	else
	{
		GREEN_LED_OFF;
	}

	if(ness == 100)
	{
		ness = 0;
	}
}

void AllIO_High_resistance_mode(void)
{
	//gpio_pulldown_int(GPIOA,LL_GPIO_PIN_ALL);
	//LL_GPIO_SetOutputPin(GPIOA, LL_GPIO_PIN_ALL);

//	gpio_pulldown_int(GPIOB,LL_GPIO_PIN_ALL);
	//LL_GPIO_SetOutputPin(GPIOB, LL_GPIO_PIN_ALL);
////	input_opendrain(GPIOA,LL_GPIO_PIN_ALL&(~(LL_GPIO_PIN_13|LL_GPIO_PIN_14)));
	input_opendrain(GPIOB,LL_GPIO_PIN_ALL&(~(LL_GPIO_PIN_7 | LL_GPIO_PIN_6)));	
	//input_opendrain(GPIOC,LL_GPIO_PIN_13);
	input_opendrain(GPIOF,LL_GPIO_PIN_ALL);
}

void exti_init_beforEnterStop(uint8_t type)
{
	if(type)
	{
		shutdown_motor_pwr();
		RED_LED_OFF;
		GREEN_LED_OFF;
		SHUTTER_OFF;
		BLE_PWR_DOWN;
		A3_3_PWR_DOWN;
	//	RF24L01_CE_OFF;
		AllIO_High_resistance_mode();
		//LL_EXTI_DisableIT_0_31(LL_EXTI_LINE_4);
		//LL_EXTI_DisableFallingTrig_0_31(LL_EXTI_LINE_4);		
		
		LL_SYSCFG_SetEXTISource(LL_SYSCFG_EXTI_PORTB,  LL_SYSCFG_EXTI_LINE7); // key
		LL_EXTI_EnableIT_0_31(LL_EXTI_LINE_7);
		LL_EXTI_EnableFallingTrig_0_31(LL_EXTI_LINE_7);
		NVIC_SetPriority(EXTI4_15_IRQn, 0);
		NVIC_EnableIRQ(EXTI4_15_IRQn);
	}
	else
	{
		LL_EXTI_DisableIT_0_31(LL_EXTI_LINE_7);
		LL_EXTI_DisableFallingTrig_0_31(LL_EXTI_LINE_7);		
		NVIC_DisableIRQ(EXTI4_15_IRQn);
	}
}

