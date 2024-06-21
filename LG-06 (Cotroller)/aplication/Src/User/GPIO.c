
#include "gpio.h"
#include "stm32f0xx_ll_bus.h"
#include "stm32f0xx_ll_SYSTEM.h"
#include "stm32f0xx_ll_exti.h"
//#include "common.h"

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


void changeIO_Init(void)
{
	GPIO_INUP_Init(GPIOB, LL_GPIO_PIN_8); // charge
}

void motor_enter_sleep(void)
{
	//LL_GPIO_ResetOutputPin(GPIOB, LL_GPIO_PIN_13); //sleep
}

void motor_exit_sleep(void)
{
	//LL_GPIO_SetOutputPin(GPIOB, LL_GPIO_PIN_13); //sleep
}

void motor_direction_change(void)
{
	
	if(glob_para.orbital_dir)
	{
		LL_GPIO_SetOutputPin(GPIOB, LL_GPIO_PIN_10); //left
	}
	else
	{
		LL_GPIO_ResetOutputPin(GPIOB, LL_GPIO_PIN_10); //right
	}
}

void CH3400N_init(void)
{
	GPIO_OUT_init(GPIOB,LL_GPIO_PIN_9);
	LL_GPIO_SetOutputPin(GPIOB, LL_GPIO_PIN_9);
}

void Led_init(void)
{
	GPIO_OUT_init(GPIOB, LL_GPIO_PIN_12); // led
}

void shutter_gpio_init(void)
{
	GPIO_OUT_init(GPIOB, LL_GPIO_PIN_3); // shutter
}

void oled_gpio_init(void)
{
	GPIO_OUT_init(GPIOA,LL_GPIO_PIN_8|LL_GPIO_PIN_9);
	GPIO_OUT_init(GPIOB,LL_GPIO_PIN_14);
	OLED_PWR_ON;
}

void wifi_init(void)
{
	GPIO_OUT_init(GPIOB, LL_GPIO_PIN_10); // 2.4 power
	WIFI_PWR_ON;
}

void Spi_init(void)
{
	LL_GPIO_InitTypeDef GPIO_InitStruct;
	LL_SPI_InitTypeDef SPI_InitStruct;
	
	/* Peripheral clock enable */
	LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_SPI2);
	
	/**USART2 GPIO Configuration  
	PA2   ------> USART2_TX
	PA3   ------> USART2_RX 
	*/
	GPIO_InitStruct.Pin = LL_GPIO_PIN_13 ; // 5: sck;  7: mosi
	GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
	GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
	GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
	GPIO_InitStruct.Pull = LL_GPIO_PULL_UP;
	GPIO_InitStruct.Alternate = LL_GPIO_AF_0;
	LL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	
	GPIO_InitStruct.Pin = LL_GPIO_PIN_15; // 5: sck;  7: mosi
	GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
	GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
	GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
	GPIO_InitStruct.Pull = LL_GPIO_PULL_UP;
	GPIO_InitStruct.Alternate = LL_GPIO_AF_0;
	LL_GPIO_Init(GPIOB, &GPIO_InitStruct);

/*	GPIO_InitStruct.Pin = LL_GPIO_PIN_14; // miso
	GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
	GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_HIGH;
	GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
	GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
	GPIO_InitStruct.Alternate = LL_GPIO_AF_0;
	LL_GPIO_Init(GPIOB, &GPIO_InitStruct);*/

/////////////////////////////////////////////////////////////////////////////////////
	SPI_InitStruct.TransferDirection = LL_SPI_HALF_DUPLEX_TX;
	SPI_InitStruct.Mode = LL_SPI_MODE_MASTER;
	SPI_InitStruct.DataWidth = LL_SPI_DATAWIDTH_8BIT;
	SPI_InitStruct.ClockPolarity = LL_SPI_POLARITY_HIGH;
	SPI_InitStruct.ClockPhase = LL_SPI_PHASE_2EDGE;
	SPI_InitStruct.NSS = LL_SPI_NSS_SOFT;
	SPI_InitStruct.BaudRate = LL_SPI_BAUDRATEPRESCALER_DIV2;
	SPI_InitStruct.BitOrder = LL_SPI_MSB_FIRST;
	SPI_InitStruct.CRCCalculation = LL_SPI_CRCCALCULATION_DISABLE;
	//SPI_InitStruct.CRCPoly = 7;
	LL_SPI_Init(SPI2, &SPI_InitStruct);
	LL_SPI_SetStandard(SPI2, LL_SPI_PROTOCOL_MOTOROLA);
	LL_SPI_DisableNSSPulseMgt(SPI2);
	 
/*	LL_SPI_SetMode(SPI1, LL_SPI_MODE_MASTER);
	LL_SPI_SetBaudRatePrescaler(SPI1, LL_SPI_BAUDRATEPRESCALER_DIV16);

	LL_SPI_SetStandard(SPI1, LL_SPI_PROTOCOL_MOTOROLA);
	LL_SPI_SetTransferDirection(SPI1, LL_SPI_FULL_DUPLEX);
	LL_SPI_SetDataWidth(SPI1, LL_SPI_DATAWIDTH_8BIT);
	LL_SPI_SetClockPolarity(SPI1, LL_SPI_POLARITY_HIGH);
	LL_SPI_SetClockPhase(SPI1, LL_SPI_PHASE_2EDGE);
	LL_SPI_SetTransferBitOrder(SPI1, LL_SPI_MSB_FIRST);
	LL_SPI_SetRxFIFOThreshold(SPI1, LL_SPI_RX_FIFO_TH_QUARTER);
	LL_SPI_SetCRCWidth(SPI1, LL_SPI_CRC_8BIT);
	LL_SPI_DisableCRC(SPI1);
	LL_SPI_SetNSSMode(SPI1, LL_SPI_NSS_SOFT);
	//LL_SPI_SetRxFIFOThreshold(SPI1, );*/
	LL_SPI_Enable(SPI2);
}

void Spi_write_data(uint8_t datat)
{
	while(!LL_SPI_IsActiveFlag_TXE(SPI2));
	//static uint8_t temp = 0;
	
	//if(LL_SPI_IsActiveFlag_TXE(SPI2))
	{
		//temp = ~temp;
		//if(temp)LED_ON;
		//else LED_OFF;
		LL_SPI_TransmitData8(SPI2, datat);
	}
	
}

void keyscan_pin_init(void)
{
	 LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOA);
	 LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOB);
	 LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOF);
	 GPIO_INUP_Init(GPIOA, LL_GPIO_PIN_11|LL_GPIO_PIN_12|LL_GPIO_PIN_15); //p12:down; p15: power key
	 GPIO_INUP_Init(GPIOB, LL_GPIO_PIN_3|LL_GPIO_PIN_4|LL_GPIO_PIN_5|LL_GPIO_PIN_6|LL_GPIO_PIN_7); // p3: ok key; p4: main key; p5: shut/pease key
	// GPIO_INUP_Init(GPIOF, LL_GPIO_PIN_6);// up key
	 
}

void STM32f030_pf_init(void)
{
	//LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOF);
	//GPIO_OUT_init(GPIOF, LL_GPIO_PIN_6); // red
	///GPIO_OUT_init(GPIOF, LL_GPIO_PIN_7); // green
	//LL_GPIO_ResetOutputPin(GPIOF, LL_GPIO_PIN_6);
	//LL_GPIO_SetOutputPin(GPIOF, LL_GPIO_PIN_7);
}

void Boost_init(void)
{
	GPIO_OUT_init(GPIOA,LL_GPIO_PIN_10);
	BOOST_PWR_ON;
}

void Device_init(void)
{
	//TMC2300_Init();
	//Led_init();
	Boost_init();
	changeIO_Init();
	oled_gpio_init();
	Spi_init();
	wifi_init();
	keyscan_pin_init();
}

void ChangeKey_exti_interrupt(void)
{
	LL_SYSCFG_SetEXTISource(LL_SYSCFG_EXTI_PORTB,  LL_SYSCFG_EXTI_LINE8); // CHARGE key
	LL_EXTI_EnableIT_0_31(LL_EXTI_LINE_8);
	LL_EXTI_EnableFallingTrig_0_31(LL_EXTI_LINE_8);
	
	NVIC_SetPriority(EXTI4_15_IRQn, 0);
	NVIC_EnableIRQ(EXTI4_15_IRQn);
}

void pwrKey_exti_interrupt(void)
{
	LL_SYSCFG_SetEXTISource(LL_SYSCFG_EXTI_PORTA,  LL_SYSCFG_EXTI_LINE15); // key
	LL_SYSCFG_SetEXTISource(LL_SYSCFG_EXTI_PORTB,  LL_SYSCFG_EXTI_LINE8); // CHARGE key
	LL_EXTI_EnableIT_0_31(LL_EXTI_LINE_15|LL_EXTI_LINE_8);
	LL_EXTI_EnableRisingTrig_0_31(LL_EXTI_LINE_15);
	
	LL_EXTI_EnableFallingTrig_0_31(LL_EXTI_LINE_8);
	NVIC_SetPriority(EXTI4_15_IRQn, 0);
	NVIC_EnableIRQ(EXTI4_15_IRQn);
}

void input_opendrain(GPIO_TypeDef *GPIOx, uint32_t PinMask)
{
	LL_GPIO_InitTypeDef GPIO_InitStruct;

	// GPIO INPUT INIT
	GPIO_InitStruct.Pin = PinMask;
	GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
	GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
	GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_OPENDRAIN;
	GPIO_InitStruct.Pull = LL_GPIO_PULL_DOWN;
	LL_GPIO_Init(GPIOx, &GPIO_InitStruct);
}

// 2.4G模块需要把(IQR)A2 AND (MOSI)A7设置为高阻状
void ShutDown_AllIO_High_resistance_mode(void)
{
	LL_GPIO_InitTypeDef GPIO_InitStruct;

	// GPIO INPUT INIT
	GPIO_InitStruct.Pin = LL_GPIO_PIN_0;
	GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
	GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
	GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
	GPIO_InitStruct.Pull = LL_GPIO_PULL_DOWN;
	LL_GPIO_Init(GPIOA, &GPIO_InitStruct);
	input_opendrain(GPIOA,LL_GPIO_PIN_ALL&(~(LL_GPIO_PIN_14|LL_GPIO_PIN_13|LL_GPIO_PIN_7|LL_GPIO_PIN_9|LL_GPIO_PIN_3|LL_GPIO_PIN_2|LL_GPIO_PIN_15)));
	input_opendrain(GPIOB,LL_GPIO_PIN_ALL&(~(LL_GPIO_PIN_2|LL_GPIO_PIN_8)));	
	//input_opendrain(GPIOC,LL_GPIO_PIN_13);
	input_opendrain(GPIOF,LL_GPIO_PIN_ALL);
	LL_GPIO_SetOutputPin(GPIOA, LL_GPIO_PIN_ALL&(~(LL_GPIO_PIN_14|LL_GPIO_PIN_13|LL_GPIO_PIN_7|LL_GPIO_PIN_9|LL_GPIO_PIN_3|LL_GPIO_PIN_2|LL_GPIO_PIN_15)));
	LL_GPIO_SetOutputPin(GPIOF,LL_GPIO_PIN_ALL);
	LL_GPIO_SetOutputPin(GPIOB,LL_GPIO_PIN_ALL&(~(LL_GPIO_PIN_2|LL_GPIO_PIN_8)));
}

void key_interrupt(uint8_t type)
{
	if(type)
	{
		LL_SYSCFG_SetEXTISource(LL_SYSCFG_EXTI_PORTA,  LL_SYSCFG_EXTI_LINE12); // down key
		LL_SYSCFG_SetEXTISource(LL_SYSCFG_EXTI_PORTA,  LL_SYSCFG_EXTI_LINE15); // pwr key
		LL_SYSCFG_SetEXTISource(LL_SYSCFG_EXTI_PORTA,  LL_SYSCFG_EXTI_LINE11); // up key
		LL_SYSCFG_SetEXTISource(LL_SYSCFG_EXTI_PORTB,  LL_SYSCFG_EXTI_LINE8); // charge key
		LL_SYSCFG_SetEXTISource(LL_SYSCFG_EXTI_PORTB,  LL_SYSCFG_EXTI_LINE3); // OK key
		LL_SYSCFG_SetEXTISource(LL_SYSCFG_EXTI_PORTB,  LL_SYSCFG_EXTI_LINE4); // main key
		LL_SYSCFG_SetEXTISource(LL_SYSCFG_EXTI_PORTB,  LL_SYSCFG_EXTI_LINE5); // shut key
		LL_SYSCFG_SetEXTISource(LL_SYSCFG_EXTI_PORTB,  LL_SYSCFG_EXTI_LINE6); // right key
		LL_SYSCFG_SetEXTISource(LL_SYSCFG_EXTI_PORTB,  LL_SYSCFG_EXTI_LINE7); // left key
		LL_EXTI_EnableIT_0_31(LL_EXTI_LINE_15|LL_EXTI_LINE_11|LL_EXTI_LINE_12|LL_EXTI_LINE_3|LL_EXTI_LINE_4|LL_EXTI_LINE_5|LL_EXTI_LINE_6|LL_EXTI_LINE_7|LL_EXTI_LINE_8);
		LL_EXTI_EnableRisingTrig_0_31(LL_EXTI_LINE_15|LL_EXTI_LINE_11|LL_EXTI_LINE_12|LL_EXTI_LINE_3|LL_EXTI_LINE_4|LL_EXTI_LINE_5|LL_EXTI_LINE_6|LL_EXTI_LINE_7);
		LL_EXTI_EnableFallingTrig_0_31(LL_EXTI_LINE_8);

		NVIC_SetPriority(EXTI4_15_IRQn, 0);
		NVIC_EnableIRQ(EXTI4_15_IRQn);
		
		NVIC_SetPriority(EXTI2_3_IRQn, 0);
		NVIC_EnableIRQ(EXTI2_3_IRQn);
	}
	else
	{
		LL_EXTI_DisableIT_0_31(LL_EXTI_LINE_15|LL_EXTI_LINE_11|LL_EXTI_LINE_12|LL_EXTI_LINE_3|LL_EXTI_LINE_4|LL_EXTI_LINE_5|LL_EXTI_LINE_6|LL_EXTI_LINE_7|LL_EXTI_LINE_8);
		LL_EXTI_DisableRisingTrig_0_31(LL_EXTI_LINE_15|LL_EXTI_LINE_11|LL_EXTI_LINE_12|LL_EXTI_LINE_3|LL_EXTI_LINE_4|LL_EXTI_LINE_5|LL_EXTI_LINE_6|LL_EXTI_LINE_7);		
		LL_EXTI_DisableFallingTrig_0_31(LL_EXTI_LINE_8);
		NVIC_DisableIRQ(EXTI2_3_IRQn);
		NVIC_DisableIRQ(EXTI4_15_IRQn);
	}
}

void IOHigh_mode(void)
{/*
	input_opendrain(GPIOA,LL_GPIO_PIN_ALL&(~(LL_GPIO_PIN_0|LL_GPIO_PIN_12|LL_GPIO_PIN_14|LL_GPIO_PIN_13|LL_GPIO_PIN_7|LL_GPIO_PIN_9|LL_GPIO_PIN_3|LL_GPIO_PIN_2|LL_GPIO_PIN_15|LL_GPIO_PIN_4|LL_GPIO_PIN_5|LL_GPIO_PIN_6|LL_GPIO_PIN_7)));
	input_opendrain(GPIOB,LL_GPIO_PIN_ALL&(~(LL_GPIO_PIN_0|LL_GPIO_PIN_2|LL_GPIO_PIN_3|LL_GPIO_PIN_4|LL_GPIO_PIN_5|LL_GPIO_PIN_6|LL_GPIO_PIN_7)));	
	//input_opendrain(GPIOC,LL_GPIO_PIN_13);
	//input_opendrain(GPIOF,LL_GPIO_PIN_ALL&(~(LL_GPIO_PIN_6|LL_GPIO_PIN_7)));
	LL_GPIO_SetOutputPin(GPIOA, LL_GPIO_PIN_ALL&(~(LL_GPIO_PIN_0|LL_GPIO_PIN_12|LL_GPIO_PIN_14|LL_GPIO_PIN_13|LL_GPIO_PIN_7|LL_GPIO_PIN_9|LL_GPIO_PIN_3|LL_GPIO_PIN_2|LL_GPIO_PIN_15|LL_GPIO_PIN_4|LL_GPIO_PIN_5|LL_GPIO_PIN_6|LL_GPIO_PIN_7)));
	//LL_GPIO_SetOutputPin(GPIOF,LL_GPIO_PIN_ALL&(~(LL_GPIO_PIN_6|LL_GPIO_PIN_7)));
	LL_GPIO_SetOutputPin(GPIOB,LL_GPIO_PIN_ALL&(~(LL_GPIO_PIN_0|LL_GPIO_PIN_2|LL_GPIO_PIN_3|LL_GPIO_PIN_4|LL_GPIO_PIN_5|LL_GPIO_PIN_6|LL_GPIO_PIN_7)));
*/
//		LL_GPIO_InitTypeDef GPIO_InitStruct;
		
		// GPIO INPUT INIT
		/*
		GPIO_InitStruct.Pin = LL_GPIO_PIN_0;
		GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
		GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
		GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
		GPIO_InitStruct.Pull = LL_GPIO_PULL_DOWN;
		LL_GPIO_Init(GPIOA, &GPIO_InitStruct);*/
		input_opendrain(GPIOA,LL_GPIO_PIN_ALL&(~(LL_GPIO_PIN_11|LL_GPIO_PIN_12|LL_GPIO_PIN_14|LL_GPIO_PIN_13|LL_GPIO_PIN_7|LL_GPIO_PIN_9|LL_GPIO_PIN_3|LL_GPIO_PIN_2|LL_GPIO_PIN_15)));
		input_opendrain(GPIOB,LL_GPIO_PIN_ALL&(~(LL_GPIO_PIN_8|LL_GPIO_PIN_2|LL_GPIO_PIN_3|LL_GPIO_PIN_4|LL_GPIO_PIN_5|LL_GPIO_PIN_6|LL_GPIO_PIN_7)));	
		//input_opendrain(GPIOC,LL_GPIO_PIN_13);
		input_opendrain(GPIOF,LL_GPIO_PIN_ALL);
		LL_GPIO_SetOutputPin(GPIOA, LL_GPIO_PIN_ALL&(~(LL_GPIO_PIN_11|LL_GPIO_PIN_12|LL_GPIO_PIN_14|LL_GPIO_PIN_13|LL_GPIO_PIN_7|LL_GPIO_PIN_9|LL_GPIO_PIN_3|LL_GPIO_PIN_2|LL_GPIO_PIN_15)));
		LL_GPIO_SetOutputPin(GPIOF,LL_GPIO_PIN_ALL);
		LL_GPIO_SetOutputPin(GPIOB,LL_GPIO_PIN_ALL&(~(LL_GPIO_PIN_8|LL_GPIO_PIN_2|LL_GPIO_PIN_3|LL_GPIO_PIN_4|LL_GPIO_PIN_5|LL_GPIO_PIN_6|LL_GPIO_PIN_7)));
}

void IO_recover(void)
{
	Boost_init();
	oled_gpio_init();
	Spi_init();
	key_interrupt(0);
	RF24L01_GPIO_Init();
	CH3400N_init();
}

