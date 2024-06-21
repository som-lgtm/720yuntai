
#include "common.h"
#include "stm32f0xx_ll_adc.h"
#include "stm32f0xx_ll_bus.h"
#include "GPIO.h"

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
	GPIO_InitStruct.Pull = LL_GPIO_PULL_UP;
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
	GPIO_OUT_init(GPIOA, LL_GPIO_PIN_7); // battery mos switch
	GPIO_OUT_init(GPIOA, LL_GPIO_PIN_5); // usb mos switch

	GPIO_INUP_Init(GPIOA, LL_GPIO_PIN_4); // USB READ STATUS
	BATTER_MOS_OFF;
	USB_MOS_OFF;
}

void Tmc22xx_SetStatu(void)
{
	Resolution_Configure(MRESOLUTION_32);
	//LL_GPIO_SetOutputPin(GPIOB, LL_GPIO_PIN_10);
	//LL_GPIO_SetOutputPin(GPIOB, LL_GPIO_PIN_2);
}

void Led_init(void)
{
	LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOC);
	GPIO_OUT_init(GPIOB, LL_GPIO_PIN_9); // GREEN
	GPIO_OUT_init(GPIOC, LL_GPIO_PIN_13); // PWR_LED
	LED_OFF;
	RED_LED_ON;
}

void shutter_gpio_init(void)
{
	GPIO_OUT_init(GPIOB, LL_GPIO_PIN_14); // shutter
}

void ble_init(void)
{
	GPIO_OUT_init(GPIOA, LL_GPIO_PIN_8); // ble pwr
	GPIO_INUP_Init(GPIOA, LL_GPIO_PIN_11); // statu
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
	GPIO_InitStruct.Pin = LL_GPIO_PIN_1;
	GPIO_InitStruct.Mode = LL_GPIO_MODE_ANALOG;
	GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
	LL_GPIO_Init(GPIOA, &GPIO_InitStruct);
}

void STM32f030_pf_init(void)
{
	LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOF);
	GPIO_OUT_init(GPIOF, LL_GPIO_PIN_6); // red
	GPIO_OUT_init(GPIOF, LL_GPIO_PIN_7); // green
	LL_GPIO_ResetOutputPin(GPIOF, LL_GPIO_PIN_6);
	LL_GPIO_SetOutputPin(GPIOF, LL_GPIO_PIN_7);
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

void Gpio_init(void)
{  
LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOA);
  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOB);
	TMC22xx_Init();
	Led_init();
	ble_init();
	key_gpio_init();
	//shutter_gpio_init();
	//STM32f030_pf_init();
	//Tmc22xx_SetStatu();
}

void motor_direction_change(uint8_t dir)
{
	
	if(dir)
	{
		LL_GPIO_SetOutputPin(GPIOA, LL_GPIO_PIN_7); //left
	}
	else
	{
		LL_GPIO_ResetOutputPin(GPIOA, LL_GPIO_PIN_7); //right
	}
}

void motor_enter_sleep(void)
{
	//LL_GPIO_ResetOutputPin(GPIOB, LL_GPIO_PIN_13); //sleep
}

void motor_exit_sleep(void)
{
	//LL_GPIO_SetOutputPin(GPIOB, LL_GPIO_PIN_13); //sleep
}


void Resolution_Configure(MOTOR_RESOLUTION Preassigned)
{
	switch(Preassigned)
	{
		case MRESOLUTION_8:
		{
			LL_GPIO_ResetOutputPin(GPIOB, LL_GPIO_PIN_2); // MS2
			LL_GPIO_ResetOutputPin(GPIOB, LL_GPIO_PIN_10); //MS1
			break;
		}
		case MRESOLUTION_16:
		{
			LL_GPIO_SetOutputPin(GPIOB, LL_GPIO_PIN_2); // MS2	
			LL_GPIO_SetOutputPin(GPIOB, LL_GPIO_PIN_10); //MS1
			break;
		}
		case MRESOLUTION_32:
		{
			LL_GPIO_ResetOutputPin(GPIOB, LL_GPIO_PIN_2); // MS2
			LL_GPIO_SetOutputPin(GPIOB, LL_GPIO_PIN_10); //MS1
			break;
		}
		case MRESOLUTION_64:
		{
			LL_GPIO_SetOutputPin(GPIOB, LL_GPIO_PIN_2); // MS2
			LL_GPIO_ResetOutputPin(GPIOB, LL_GPIO_PIN_10); //MS1
			break;
		}
	}
}

uint8_t key_check(void)
{
	uint32_t times = 0;
	uint32_t debug = 0;
	
	if(KeyPowr==0)
	{
		for(times=0;times<22000000;times++)
		{
			debug += 1;
			if(debug == 200000)
			{
				debug = 0;
				if(KeyPowr)return 0;
			}

		}
		
		if(KeyPowr == 0)
		{
			force_updata = 0xcc;
			return 1;
		}
	}
	return 0;
}


