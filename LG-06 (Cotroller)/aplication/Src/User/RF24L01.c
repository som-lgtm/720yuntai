
#include "RF24L01.h"
#include "stm32f0xx_ll_spi.h"
#include "stm32f0xx_ll_gpio.h"
#include "stm32f0xx_ll_bus.h"
#include "common.h"
#include "gpio.h"
#include "stm32f0xx_ll_exti.h"
#include "stm32f0xx_ll_system.h"
#include "stdio.h"
#include "string.h"
#include "uart.h"
#include "oled.h"

// gpio.c
void GPIO_INUP_Init(GPIO_TypeDef *GPIOx, uint32_t PinMask);

uint8_t g_RF24L01RxBuffer[ 32 ] = { 0 }; 

#ifdef USE_HARD_SPI

void RF24L01_GPIO_Init(void)
{
	LL_GPIO_InitTypeDef GPIO_InitStruct;
	LL_SPI_InitTypeDef SPI_InitStruct;
	
	/* Peripheral clock enable */
	LL_APB1_GRP2_EnableClock(LL_APB1_GRP2_PERIPH_SPI1);
	
	/**USART2 GPIO Configuration  
	PA2   ------> USART2_TX
	PA3   ------> USART2_RX 
	*/
	GPIO_InitStruct.Pin = LL_GPIO_PIN_5 ; // sck
	GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
	GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
	GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
	GPIO_InitStruct.Pull = LL_GPIO_PULL_UP;
	GPIO_InitStruct.Alternate = LL_GPIO_AF_0;
	LL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	
	GPIO_InitStruct.Pin = LL_GPIO_PIN_7; // mosi
	GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
	GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
	GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
	GPIO_InitStruct.Pull = LL_GPIO_PULL_UP;
	GPIO_InitStruct.Alternate = LL_GPIO_AF_0;
	LL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = LL_GPIO_PIN_7; // miso
	GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
	GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_HIGH;
	GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
	GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
	GPIO_InitStruct.Alternate = LL_GPIO_AF_0;
	LL_GPIO_Init(GPIOA, &GPIO_InitStruct);

/////////////////////////////////////////////////////////////////////////////////////
	SPI_InitStruct.TransferDirection = LL_SPI_FULL_DUPLEX; // Rx and Tx transfer on 2 lines
	SPI_InitStruct.Mode = LL_SPI_MODE_MASTER;
	SPI_InitStruct.DataWidth = LL_SPI_DATAWIDTH_8BIT;
	SPI_InitStruct.ClockPolarity = LL_SPI_POLARITY_LOW;
	SPI_InitStruct.ClockPhase = LL_SPI_PHASE_1EDGE;
	SPI_InitStruct.NSS = LL_SPI_NSS_SOFT;
	SPI_InitStruct.BaudRate = LL_SPI_BAUDRATEPRESCALER_DIV2;
	SPI_InitStruct.BitOrder = LL_SPI_MSB_FIRST;
	SPI_InitStruct.CRCCalculation = LL_SPI_CRCCALCULATION_DISABLE;
	//SPI_InitStruct.CRCPoly = 7;
	LL_SPI_Init(SPI1, &SPI_InitStruct);
	LL_SPI_SetStandard(SPI1, LL_SPI_PROTOCOL_MOTOROLA);
	LL_SPI_DisableNSSPulseMgt(SPI1);
	 
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
	LL_SPI_Enable(SPI1);

	
	//GPIO_OUT_init(GPIOB, LL_GPIO_PIN_6); //IRQ
	GPIO_OUT_init(GPIOB, LL_GPIO_PIN_0); // CSN
	GPIO_OUT_init(GPIOA, LL_GPIO_PIN_4); // CE

	Irq_gpio_init(); //IRQ
}

uint8_t drv_spi_read_write_byte(uint8_t TxByte)
{
	uint16_t l_WaitTime = 0;
	uint8_t l_Data = 0;
	
	while(!LL_SPI_IsActiveFlag_TXE(SPI1)) // check if TX fifo is empty
	{
		if( SPI_WAIT_TIMEOUT == ++l_WaitTime ) // Waiting for the timing
		{
			break;			//if time out to  exit
		}
	}

	LL_SPI_TransmitData8(SPI1, TxByte); // begin to sed data

	l_WaitTime = SPI_WAIT_TIMEOUT / 2; // 
	
	while(!LL_SPI_IsActiveFlag_RXNE(SPI1)) // check if Rx buffer is not empty
	{
		if( SPI_WAIT_TIMEOUT == ++l_WaitTime )// Waiting for the timing
		{
			break;			//if time out to  exit
		}
	}

	l_Data = LL_SPI_ReceiveData8(SPI1); // read data

	return l_Data;
}


#endif


#ifdef USE_SOFT_SPI


void RF24L01_GPIO_Init(void)
{
	GPIO_OUT_init(GPIOA, LL_GPIO_PIN_5); //SCK
	GPIO_OUT_init(GPIOA, LL_GPIO_PIN_7); // MOSI
	GPIO_INUP_Init(GPIOA, LL_GPIO_PIN_6); // MISO
	Irq_gpio_init(); //IRQ
	GPIO_OUT_init(GPIOB, LL_GPIO_PIN_0); // CSN
	GPIO_OUT_init(GPIOA, LL_GPIO_PIN_4); // CE
}

uint8_t drv_spi_read_write_byte(uint8_t TxByte)
{
	uint8_t i = 0, Data = 0;
	
	RF24L01_SCK_OFF;

	for(i = 0; i< 8; i++)
	{
		if( 0x80 == ( TxByte & 0x80 ))
		{
			RF24L01_MOSI_ON;
		}
		else
		{
			RF24L01_MOSI_OFF;
		}

		TxByte <<= 1;

		RF24L01_SCK_ON;

//		__nop( );
//		__nop( );

		Data <<= 1;

		if(RF24L01_MISO) // check if r miso is not empty
		{
			Data |= 0x01;
		}
		
		RF24L01_SCK_OFF;
//		__nop( );
//		__nop( );
	}

	return Data;
}

#endif

#if (IRQ_READ_STATUS == 1)
void Irq_gpio_init(void)
{
	/*LL_EXTI_InitTypeDef EXTI_InitStruct = {0};
	
	GPIO_INUP_Init(GPIOB, LL_GPIO_PIN_3); // DIAG
	
	EXTI_InitStruct.Line_0_31 = LL_GPIO_PIN_3;
	EXTI_InitStruct.LineCommand = ENABLE;
	EXTI_InitStruct.Mode = LL_EXTI_MODE_IT;
	EXTI_InitStruct.Trigger = LL_EXTI_TRIGGER_RISING;
	LL_EXTI_Init(&EXTI_InitStruct);
	
	LL_SYSCFG_SetEXTISource(LL_SYSCFG_EXTI_PORTB, LL_SYSCFG_EXTI_LINE13);
	
	NVIC_SetPriority(EXTI2_3_IRQn, 0);
	NVIC_EnableIRQ(EXTI2_3_IRQn);*/
}

#elif(IRQ_READ_STATUS == 0)

void Irq_gpio_init(void)
{
	GPIO_INUP_Init(GPIOB, LL_GPIO_PIN_2); // IRQ
}

#endif

void Nrf24L01_init(void)
{
	WIFI_PWR_OFF;
	time_delay_ms(1000);
	WIFI_PWR_ON;
	time_delay_ms(100);
	RF24L01_GPIO_Init( );
	NRF24L01_check( );			
	RF24L01_Init( );
	RF24L01_Set_Mode( MODE_RX );
	RF24L01_CE_ON;
}

void NRF24L01_Write_Reg( uint8_t RegAddr, uint8_t Value )
{
    RF24L01_CSN_OFF;		//ÁâáÈÄâ
	
    drv_spi_read_write_byte( NRF_WRITE_REG | RegAddr );	//ÂÜôÂëΩ‰ª§ Âú∞ÂùÄ
    drv_spi_read_write_byte( Value );			//ÂÜôÊï∞ÊçÆ
	
    RF24L01_CSN_ON;		//ÂèñÊ∂àÁâáÈÄâ
}

void NRF24L01_Set_TxAddr( uint8_t *pAddr, uint8_t len )
{
	len = ( len > 5 ) ? 5 : len;					//µÿ÷∑≥§∂»≤ª¥Û”⁄5 ∏ˆ◊÷Ω⁄
    NRF24L01_Write_Buf( TX_ADDR, pAddr, len );	//–¥µÿ÷∑
}


void NRF24L01_Set_RxAddr( uint8_t PipeNum, uint8_t *pAddr, uint8_t Len )
{
    Len = ( Len > 5 ) ? 5 : Len;
    PipeNum = ( PipeNum > 5 ) ? 5 : PipeNum;		// Õ®µ¿≤ª¥Û”⁄5£¨ µÿ÷∑≥§∂»≤ª¥Û”⁄5 ∏ˆ◊÷Ω⁄

    NRF24L01_Write_Buf( RX_ADDR_P0 + PipeNum, pAddr, Len );	// –¥»Îµÿ÷∑
}

uint8_t NRF24L01_Read_Reg( uint8_t RegAddr )
{
    uint8_t btmp;
	
    RF24L01_CSN_OFF;			//–¥ºƒ¥Ê∆˜ ± ß–ß ’∑¢π¶ƒ‹
	
    drv_spi_read_write_byte( NRF_READ_REG | RegAddr );	// write the addres of register and commands
    btmp = drv_spi_read_write_byte( 0xFF );				//read data
	
    RF24L01_CSN_ON;			//ÂèñÊ∂àÁâáÈÄâ
	
    return btmp;
}

/**
 * @brief :ƒ£ Ωµƒ«–ªª
 * @param :
 *		   @Mode:∑¢ÀÕƒ£ ΩªÚ ’Ω”ƒ£ Ω
 */
void RF24L01_Set_Mode( nRf24l01ModeType Mode )
{
    uint8_t controlreg = 0;
	controlreg = NRF24L01_Read_Reg( CONFIG );
	
    if( Mode == MODE_TX )       
	{
		controlreg &= ~( 1<< PRIM_RX );
	}
    else 
	{
		if( Mode == MODE_RX )  
		{ 
			controlreg |= ( 1<< PRIM_RX ); 
		}
	}

    NRF24L01_Write_Reg( CONFIG, controlreg );
}

/**
  * @brief :NRF24L01 Ω” ’ ˝æ›
  * @param :
  *			@rxbuf: Ω” ’ ˝æ›¥Ê∑≈µÿ÷∑
  * @note  :Êó†
  * @retval: Ω” ’ ˝æ›∏ˆ ˝
  */ 
uint8_t NRF24L01_RxPacket( uint8_t *rxbuf )
{
	uint8_t l_Status = 0, l_RxLength = 0;
	
//	RF24L01_CSN_OFF;		//ÁâáÈÄâ
//	drv_spi_read_write_byte( FLUSH_RX );
//	RF24L01_CSN_ON;
//	RF24L01_CE_ON;
	if( 0 == RF24L01_GET_IRQ_STATUS( )) // ¥Àwhile π¶ƒ‹ «≈‰∫œ∑¢ÀÕ∑Ω“ª√Îª·∑¢“ª¥Œ ˝æ›£¨À˘“‘3√Î√ª ’µΩ ˝æ›æÕ÷ÿ–¬≥ı ºªØƒ£øÈ
	{
		RF24L01_CE_OFF;
		l_Status = NRF24L01_Read_Reg( STATUS ); 	//∂¡◊¥Ã¨ºƒ¥Ê∆˜
		NRF24L01_Write_Reg( STATUS,l_Status );		//«Â÷–∂œ±Í÷æ
		if( l_Status & RX_OK)	//Ω” ’µΩ ˝æ›
		{
			l_RxLength = NRF24L01_Read_Reg( R_RX_PL_WID );		//∂¡»°Ω” ’µΩµƒ ˝æ›∏ˆ ˝
			NRF24L01_Read_Buf( RD_RX_PLOAD,rxbuf,l_RxLength );	//Ω” ’µΩ ˝æ›  
			NRF24L01_Write_Reg( FLUSH_RX,0xff );				//«Â≥˝RX FIFO
			return l_RxLength; 
		}	
	}
	
	
	return 0;				//√ª”– ’µΩ ˝æ›	
}

/**
  * @brief :NRF24L01–¥÷∏∂®≥§∂»µƒ ˝æ›
  * @param :
  *			@reg:µÿ÷∑
  *			@pBuf:–¥»Îµƒ ˝æ›µÿ÷∑
  *			@len: ˝æ›≥§∂»
  * @note  : ˝æ›≥§∂»≤ª≥¨π˝255£¨µÿ÷∑‘⁄…Ë±∏÷–”––ß
  * @retval:–¥◊¥Ã¨
  */
void NRF24L01_Write_Buf( uint8_t RegAddr, uint8_t *pBuf, uint8_t len )
{
    uint8_t i;
	
    RF24L01_CSN_OFF;		//∆¨—°£¨œ» ßƒ‹
	
    drv_spi_read_write_byte( NRF_WRITE_REG | RegAddr );	//ÂÜôÂëΩ‰ª§ Âú∞ÂùÄ
    for( i = 0; i < len; i ++ )
    {
        drv_spi_read_write_byte( *( pBuf + i ) );		//ÂÜôÊï∞ÊçÆ
    }
	
    RF24L01_CSN_ON;		// πƒ‹£¨ø™ º∑¢ÀÕ ˝æ›
}


/**
  * @brief :NRF24L01∂¡÷∏∂®≥§∂»µƒ ˝æ›
  * @param :
  *			@reg:µÿ÷∑
  *			@pBuf: ˝æ›¥Ê∑≈µÿ÷∑
  *			@len: ˝æ›≥§∂»
  * @note  : ˝æ›≥§∂»≤ª≥¨π˝255£¨µÿ÷∑‘⁄…Ë±∏÷–”––ß
  * @retval:∂¡»°◊¥Ã¨
  */
void NRF24L01_Read_Buf( uint8_t RegAddr, uint8_t *pBuf, uint8_t len )
{
    uint8_t btmp;
	
    RF24L01_CSN_OFF;		//∆¨—°£¨œ» ßƒ‹
	
    drv_spi_read_write_byte( NRF_READ_REG | RegAddr );	//ËØªÂëΩ‰ª§ Âú∞ÂùÄ
    for( btmp = 0; btmp < len; btmp ++ )
    {
        *( pBuf + btmp ) = drv_spi_read_write_byte( 0xFF );	//ËØªÊï∞ÊçÆ
    }
	
    RF24L01_CSN_ON;		// πƒ‹£¨ø™ º∑¢ÀÕ ˝æ›
}

void drv_uart_tx_bytes( uint8_t* TxBuffer, uint8_t Length )
{
/*	while( Length-- )
	{
		while(!LL_USART_IsActiveFlag_TXE(USART1));
		LL_USART_TransmitData8(USART1, *TxBuffer);
		
		TxBuffer++;
	}*/
}


void NRF24L01_check( void )
{
	static uint8_t ttemp = 0;
	uint8_t i;
	uint8_t buf[5]={ 0XA5, 0XA5, 0XA5, 0XA5, 0XA5 };
	uint8_t read_buf[ 5 ] = { 0 };
	 
	while( 1 )
	{
		Feed_IWDG();
		NRF24L01_Write_Buf( TX_ADDR, buf, 5 );			//–¥»Î5 ∏ˆ◊÷Ω⁄µƒµÿ÷∑
		NRF24L01_Read_Buf( TX_ADDR, read_buf, 5 );		//∂¡≥ˆ–¥»Îµƒµÿ÷∑
		for( i = 0; i < 5; i++ )
		{
			if( buf[ i ] != read_buf[ i ] )
			{
				break;
			}	
		} 
		
		if( 5 == i )
		{
			//Oled_EnlPrint(0, 1, "RF24L01 is Yes...", "", ENGLISH);
			//RED_LED_ON;
			//LED_OFF;
			break;
		}
		else
		{
//			RED_LED_ON;
//			LED_ON;
			//drv_uart_tx_bytes( "RF24L01 is not find !...", 26 );
			ttemp += 1;
			if(ttemp >= 5)Oled_EnlPrint(0, 1, "RF24L01 is not find", "", ENGLISH);
			
		}
		
		time_delay_ms(200);
	}
}

/**
  * @brief :NRF24L01∂¡◊¥Ã¨ºƒ¥Ê∆˜
  * @param :Œﬁ
  * @note  :Œﬁ
  * @retval:RF24L01◊¥Ã¨
  */

uint8_t NRF24L01_Read_Status_Register( void )
{
    uint8_t Status;
	
    RF24L01_CSN_OFF;		// π”√∆¨—°
	
    Status = drv_spi_read_write_byte( NRF_READ_REG + STATUS );	//∂¡◊¥Ã¨ºƒ¥Ê∆˜
	
    RF24L01_CSN_ON;		//ÂèñÊ∂àÁâáÈÄâ
	
    return Status;
}

uint8_t NRF24L01_Clear_IRQ_Flag( uint8_t IRQ_Source )
{
    uint8_t btmp = 0;

    IRQ_Source &= ( 1 << RX_DR ) | ( 1 << TX_DS ) | ( 1 << MAX_RT );	//÷–∂œ±Í÷æ¥¶¿Ì
    btmp = NRF24L01_Read_Status_Register( );			//∂¡◊¥Ã¨ºƒ¥Ê∆˜
			
	RF24L01_CSN_OFF;
    drv_spi_read_write_byte( NRF_WRITE_REG + STATUS );	//–¥◊¥Ã¨ºƒ¥Ê∆˜√¸
    drv_spi_read_write_byte( IRQ_Source | btmp );		//«Âœ‡”¶÷–∂œ±Í÷æ
    RF24L01_CSN_ON;
	
    return ( NRF24L01_Read_Status_Register( ));			//∑µªÿ◊¥Ã¨ºƒ¥Ê∆˜◊¥Ã¨
    
}

uint8_t RF24L01_Read_IRQ_Status( void )
{
    return ( NRF24L01_Read_Status_Register( ) & (( 1 << RX_DR ) | ( 1 << TX_DS ) | ( 1 << MAX_RT )));	//ËøîÂõû‰∏≠Êñ≠Áä∂ÊÄÅ
}

void RF24L01_Init( void )
{
    //uint8_t addr[5] = {INIT_ADDR};

    RF24L01_CE_OFF;
    NRF24L01_Clear_IRQ_Flag( IRQ_ALL );
#if DYNAMIC_PACKET == 1
/*
 πƒ‹Ω” ’π‹µ¿x ∂ØÃ¨∏∫‘ÿ≥§∂»(–ËÕ¨ ±≈‰÷√
FEATURE÷–µƒEN_DPLº∞EN_AA÷–µƒ◊‘∂Ø”¶¥ENAA_P5)
…Ë÷√Œ™∂ØÃ¨ ˝æ›≥§∂»∫Ûø…“‘Ω” ’√ø∞¸ ˝æ›µƒ≤ªÕ¨≥§∂»
*/
    NRF24L01_Write_Reg( DYNPD, ( 1 << 0 ) ); // πƒ‹Õ®µ¿0∂ØÃ¨ ˝æ›≥§∂»
    
    // π NO_ACK ŒªŒ™ 1 –Ë“™œ»≈‰÷√ FEATURE ºƒ¥Ê∆˜÷–µƒ EN_DYN_ACK ŒªŒ™ 1£¨«“ π”√
//W_TX_PAYLOAD_NOACK √¸¡Ó–¥ FIFO°£µ± ’µΩ“ª∏ˆ’‚—˘µƒ∞¸∫Û£¨Ω” ’∂À≤ªª·∑¢ÀÕ ACK »∑»œ
//–≈∫≈∏¯∑¢…‰∑Ω°££®º¥ πΩ” ’∂Àπ§◊˜‘⁄ ACK Ω” ’ƒ£ Ω£©°£
//	5°¢Si24R1 ”Î NRF2401P ªÏ∫œÕ®–≈Œ Ã‚£∫
//	L01_WriteSingleReg(L01REG_FEATURE,0x07); 
//	∏ƒ≥…
//	L01_WriteSingleReg(L01REG_FEATURE,0x04); 
//	Àµ√˜£∫
//	(1)¥”≥Ã–Ú…œø¥£¨√ª”–”√µΩ Payload With ACK, æÕ «Ω” ’∂Àªÿ∏¥ ACK  ±≤ª¥¯ Payload£¨“Ú¥À£¨
//	Bit 1£®0x1D.1£©Œ™ 0 
//	(2)¥”≥Ã–Ú…œø¥£¨Œ™ ACK Õ®–≈ƒ£ Ω£¨“Ú¥À£¨Bit 0(0x1D.0) Œ™ 0

   // NRF24L01_Write_Reg( FEATRUE, 0x07 ); // 
    NRF24L01_Write_Reg( FEATRUE, 0x04 ); // ≤ª¥¯∏∫‘ÿµƒackª˙÷∆
    
    NRF24L01_Read_Reg( DYNPD );
    NRF24L01_Read_Reg( FEATRUE );
	
#elif DYNAMIC_PACKET == 0
    
    L01_WriteSingleReg( L01REG_RX_PW_P0, FIXED_PACKET_LEN ); //πÃ∂® ˝æ›≥§∂»
    
	
#endif	//DYNAMIC_PACKET

    RF24L01_CE_OFF;
	NRF24L01_Write_Reg( CONFIG,0x08 );	//
	time_delay_ms(10);
    NRF24L01_Write_Reg( CONFIG, /*( 1<<MASK_RX_DR ) |*/			//Ω” ’÷–∂œ
    
                                      ( 1 << EN_CRC ) |     // πƒ‹CRC 1∏ˆ◊÷Ω⁄
                                      ( 0 << CRCO) |			// CRC8bit
                                      ( 1 << PWR_UP ) );    	//ø™∆Ù…Ë±∏
                                      
    NRF24L01_Write_Reg( EN_AA, ( 1 << ENAA_P0 ) );   		//Õ®µ¿0 ◊‘∂Ø”¶¥
    NRF24L01_Write_Reg( EN_RXADDR, ( 1 << ERX_P0 ) );		// πƒ‹Õ®µ¿0 Ω” ’
    NRF24L01_Write_Reg( SETUP_AW, AW_5BYTES );     			//µÿ÷∑øÌ∂»
    NRF24L01_Write_Reg( SETUP_RETR, ARD_4000US |( REPEAT_CNT & 0x0F ) );         	//÷ÿ∏¥µ»¥˝ ±º‰250us
    
    NRF24L01_Write_Reg( RF_CH, wifi_id.wifi_rfc );             			//≥ı ºªØÕ®µ¿
//    NRF24L01_Write_Reg( RF_CH, 60 );             			//≥ı ºªØÕ®µ¿
    NRF24L01_Write_Reg( RF_SETUP, 0x27 );					//–ª∆µ ˝æ›¬ Œ™250K,¥À÷µ‘Ω∏ﬂ≤‡∑¢ÀÕæ‡¿Î‘ΩΩ¸∑Ò≤‡‘Ω‘∂
															// ∑¢ÀÕπ¶ƒ‹Œ™7dbm
    NRF24L01_Set_TxAddr( &wifi_id.TX_adress[0], 5 );                      //…Ë÷√TX µÿ÷∑
    NRF24L01_Set_RxAddr( 0, &wifi_id.TX_adress[0], 5 );                   //…Ë÷√P0π‹µ¿RX µÿ÷∑”√”⁄Ω” ’ACK”¶¥
    //NRF24L01_Set_TxAddr( &addr[0], 5 );                      //…Ë÷√TX µÿ÷∑
    //NRF24L01_Set_RxAddr( 0, &addr[0], 5 );                   //…Ë÷√P0π‹µ¿RX µÿ÷∑”√”⁄Ω” ’ACK”¶¥
//	NRF24L01_Write_Reg( FLUSH_TX,0xff );	//«Â≥˝FIFO
//	NRF24L01_Write_Reg( STATUS,0xff );	//«Â≥˝STATUS

}

uint8_t NRF24L01_TxPacket( uint8_t *txbuf, uint8_t Length )
{
	uint8_t l_Status = 0;
	uint16_t l_MsTimes = 0;
	
	RF24L01_CSN_OFF;
	drv_spi_read_write_byte( FLUSH_TX );
	RF24L01_CSN_ON;
	
    RF24L01_CE_OFF;
	NRF24L01_Write_Buf( WR_TX_PLOAD, txbuf, Length );	//Ω´“™∑¢ÀÕµƒ ˝æ›∂®»Îfifo
    RF24L01_CE_ON;
	while( 0 != RF24L01_GET_IRQ_STATUS( ))
	{
		Feed_IWDG();
		if(NRF24L01_Read_Reg(STATUS) & MAX_TX)break; // ÷ÿ∑¢¥Œ ˝µΩ¥Ô∫ÛÕÀ≥ˆwhile
		
		time_delay_ms( 1 );
		if( 200 == l_MsTimes++ )						//∑¢ÀÕ≥¨ ±
		{
			//RF24L01_GPIO_Init( );
			//RF24L01_Init( );
			//RF24L01_Set_Mode( MODE_TX );
			break;
		}
	}
	l_Status = NRF24L01_Read_Reg(STATUS);						//∂¡»°◊¥Ã¨ºƒ¥Ê∆˜≤Œ ˝ 
	NRF24L01_Write_Reg( STATUS, l_Status );						//«Â≥˝◊¥Ã¨ºƒ¥Ê∆˜±Í÷æŒª
	
	if( l_Status & MAX_TX )	//÷ÿ∑¢¥Œ ˝“—¬˙÷–∂œ
	{
		NRF24L01_Write_Reg( FLUSH_TX,0xff );	//«Â≥˝FIFO
		NRF24L01_Write_Reg( STATUS,0xff );	//«Â≥˝STATUS
		return MAX_TX; 
	}
	if( l_Status & TX_OK )	// ∑¢ÀÕ ˝æ›≥…π¶
	{
		return TX_OK;
	}
	
	return 0xFF;	//∑¢ÀÕ≥¨ ±
}

void wifi24G_send_data(void)
{
	uint8_t i;
	uint8_t index = 0;
	uint8_t Send_status = 0;
	static uint8_t send_times = 0; //‘Ÿ∑¢¥Œ ˝
	static uint8_t send_overTime = 0; //∑¢ÀÕ≥¨ ±¥Œ ˝
	
	if(usart22_send.send_interval22)return; // Sending interval
	for(index = 0; index < LOCUS_BUFFER_MAX; index++)
	{
		if(App_Buffer[index].app_send_size)break;
	}
	
	if(index >= LOCUS_BUFFER_MAX)return;
//	String_Printf( &App_Buffer[index].app_send_buffer[0], App_Buffer[index].app_send_size);
	if(NRF24L01_Read_Reg( STATUS ) == RX_OK || RF24L01_GET_IRQ_STATUS()==0)
	{
		usart22_send.send_interval22 = 60;
		return; // ’˝‘⁄Ω” ’ ˝æ› ±≤ªƒ‹∑¢ÀÕ
	}
	
	RF24L01_CE_OFF;
	RF24L01_Set_Mode( MODE_TX );		//∑¢ÀÕƒ£ Ω
	
	//memcpy(&Cache.Cache_SBf[0], Usart11_shadow[index].usart11_shadow_buffer, Usart11_shadow[index].shadow_send_size); // load data to DMA send buffer
	Send_status = NRF24L01_TxPacket( &App_Buffer[index].app_send_buffer[0], App_Buffer[index].app_send_size );
	if(Send_status == TX_OK)
	{
		App_Buffer[index].app_send_size = 0;
		usart22_send.send_interval22 = 60;
		RF24L01_CE_OFF;
		RF24L01_Set_Mode( MODE_RX );		//∑¢ÀÕ≥…π¶∫Û«–ªªŒ™Ω” ’ƒ£ ΩΩ¯»ÎΩ” ’º‡Ã˝◊¥Ã¨
		RF24L01_CE_ON;
		send_times = 0;
		send_overTime = 0;
	}
	else if(Send_status == MAX_TX)//»Áπ˚ «÷ÿ∑¢¥Œ ˝≥¨¡ÀæÕ‘Ÿ∑¢
	{
		App_Buffer[index].app_send_size = 0;
		
		send_times += 1;
		if(send_times >= 10)
		{
			App_Buffer[index].app_send_size = 0;
			send_times = 0;
		}
		
		RF24L01_CE_OFF;
		RF24L01_Set_Mode( MODE_RX );		//∑¢ÀÕ ≥¨ ˝∫Û«–ªªŒ™Ω” ’ƒ£ ΩΩ¯»ÎΩ” ’º‡Ã˝◊¥Ã¨
		RF24L01_CE_ON;
		usart22_send.send_interval22 = 60;
	}
	else //»Áπ˚ «∑¢ÀÕ≥¨ ±÷ÿ–¬≥ı ºªØ∫Û÷ÿ∑¢ ˝æ›
	{
		if(send_overTime >= 5)
		{
			send_overTime = 0;
			RF24L01_CE_OFF;
		//	Nrf24L01_init();
			for(i=0; i<= LOCUS_BUFFER_MAX; i++)
			{
				App_Buffer[index].app_send_size = 0;
			}
		}
		send_overTime += 1;
	}
}

uint8_t checksum_verify(uint8_t *spt, uint8_t lenght)
{
	uint8_t i;
	uint16_t temp =0;
	uint8_t checksum =0;
	
	for(i=0; i< lenght; i++)
	{
		temp += (uint16_t)spt[i];
	}

	checksum = temp;
	
	if(checksum != spt[lenght])
	{
		return FALSE;
	}

	return TRUE;
}


void wifi24G_read_data(void)
{
	uint8_t sizes = 0;
//	if(usart_send.Tx_tag)return;
	sizes = NRF24L01_RxPacket(app_read_buffer);
	
	RF24L01_CE_ON;
	
	if(sizes)
	{
		//String_Printf(app_read_buffer, sizes);
		if(checksum_verify(app_read_buffer, sizes-1) == FALSE)
		{
			app_read_buffer[1] = 0;
			return; 
		}
		String_Printf(app_read_buffer, sizes);
		receiver_data_from_A650();
		usart22_send.send_interval22 = 15;												
		app_read_buffer[1] = 0;
	}
}

void wifi24G_data_Dispose(void)
{
	wifi24G_send_data();
	wifi24G_read_data();
}

void wifi_sleep(void)
{
	RF24L01_CE_OFF;
	RF24L01_CSN_OFF;
	NRF24L01_Write_Reg( FLUSH_TX,0xff );	//«Â≥˝FIFO
	NRF24L01_Write_Reg( STATUS,0xff );	//«Â≥˝STATUS
 //   NRF24L01_Write_Reg( CONFIG, /*( 1<<MASK_RX_DR ) |*/			//Ω” ’÷–∂œ
 //   
 //                                     ( 1 << EN_CRC ) |     // πƒ‹CRC 1∏ˆ◊÷Ω⁄
 //                                     ( 0 << CRCO) |			// CRC8bit
 //                                     (  SHUTDOWN<<1 ) );    	// shutdown
}

void wifi_wakeup(void)
{
    NRF24L01_Write_Reg( CONFIG, /*( 1<<MASK_RX_DR ) |*/			//Ω” ’÷–∂œ
    
                                      ( 1 << EN_CRC ) |     // πƒ‹CRC 1∏ˆ◊÷Ω⁄
                                      ( 0 << CRCO) |			// CRC8bit
                                      (  PWR_UP << 1) );    	// wakeup
}

