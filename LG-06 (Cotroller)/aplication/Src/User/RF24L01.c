
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
    RF24L01_CSN_OFF;		//片选
	
    drv_spi_read_write_byte( NRF_WRITE_REG | RegAddr );	//写命令 地址
    drv_spi_read_write_byte( Value );			//写数据
	
    RF24L01_CSN_ON;		//取消片选
}

void NRF24L01_Set_TxAddr( uint8_t *pAddr, uint8_t len )
{
	len = ( len > 5 ) ? 5 : len;					//��ַ���Ȳ�����5 ���ֽ�
    NRF24L01_Write_Buf( TX_ADDR, pAddr, len );	//д��ַ
}


void NRF24L01_Set_RxAddr( uint8_t PipeNum, uint8_t *pAddr, uint8_t Len )
{
    Len = ( Len > 5 ) ? 5 : Len;
    PipeNum = ( PipeNum > 5 ) ? 5 : PipeNum;		// ͨ��������5�� ��ַ���Ȳ�����5 ���ֽ�

    NRF24L01_Write_Buf( RX_ADDR_P0 + PipeNum, pAddr, Len );	// д���ַ
}

uint8_t NRF24L01_Read_Reg( uint8_t RegAddr )
{
    uint8_t btmp;
	
    RF24L01_CSN_OFF;			//д�Ĵ���ʱʧЧ�շ�����
	
    drv_spi_read_write_byte( NRF_READ_REG | RegAddr );	// write the addres of register and commands
    btmp = drv_spi_read_write_byte( 0xFF );				//read data
	
    RF24L01_CSN_ON;			//取消片选
	
    return btmp;
}

/**
 * @brief :ģʽ���л�
 * @param :
 *		   @Mode:����ģʽ���ս�ģʽ
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
  * @brief :NRF24L01 ��������
  * @param :
  *			@rxbuf: �������ݴ�ŵ�ַ
  * @note  :无
  * @retval: �������ݸ���
  */ 
uint8_t NRF24L01_RxPacket( uint8_t *rxbuf )
{
	uint8_t l_Status = 0, l_RxLength = 0;
	
//	RF24L01_CSN_OFF;		//片选
//	drv_spi_read_write_byte( FLUSH_RX );
//	RF24L01_CSN_ON;
//	RF24L01_CE_ON;
	if( 0 == RF24L01_GET_IRQ_STATUS( )) // ��while ��������Ϸ��ͷ�һ��ᷢһ�����ݣ�����3��û�յ����ݾ����³�ʼ��ģ��
	{
		RF24L01_CE_OFF;
		l_Status = NRF24L01_Read_Reg( STATUS ); 	//��״̬�Ĵ���
		NRF24L01_Write_Reg( STATUS,l_Status );		//���жϱ�־
		if( l_Status & RX_OK)	//���յ�����
		{
			l_RxLength = NRF24L01_Read_Reg( R_RX_PL_WID );		//��ȡ���յ������ݸ���
			NRF24L01_Read_Buf( RD_RX_PLOAD,rxbuf,l_RxLength );	//���յ�����  
			NRF24L01_Write_Reg( FLUSH_RX,0xff );				//���RX FIFO
			return l_RxLength; 
		}	
	}
	
	
	return 0;				//û���յ�����	
}

/**
  * @brief :NRF24L01дָ�����ȵ�����
  * @param :
  *			@reg:��ַ
  *			@pBuf:д������ݵ�ַ
  *			@len:���ݳ���
  * @note  :���ݳ��Ȳ�����255����ַ���豸����Ч
  * @retval:д״̬
  */
void NRF24L01_Write_Buf( uint8_t RegAddr, uint8_t *pBuf, uint8_t len )
{
    uint8_t i;
	
    RF24L01_CSN_OFF;		//Ƭѡ����ʧ��
	
    drv_spi_read_write_byte( NRF_WRITE_REG | RegAddr );	//写命令 地址
    for( i = 0; i < len; i ++ )
    {
        drv_spi_read_write_byte( *( pBuf + i ) );		//写数据
    }
	
    RF24L01_CSN_ON;		//ʹ�ܣ���ʼ��������
}


/**
  * @brief :NRF24L01��ָ�����ȵ�����
  * @param :
  *			@reg:��ַ
  *			@pBuf:���ݴ�ŵ�ַ
  *			@len:���ݳ���
  * @note  :���ݳ��Ȳ�����255����ַ���豸����Ч
  * @retval:��ȡ״̬
  */
void NRF24L01_Read_Buf( uint8_t RegAddr, uint8_t *pBuf, uint8_t len )
{
    uint8_t btmp;
	
    RF24L01_CSN_OFF;		//Ƭѡ����ʧ��
	
    drv_spi_read_write_byte( NRF_READ_REG | RegAddr );	//读命令 地址
    for( btmp = 0; btmp < len; btmp ++ )
    {
        *( pBuf + btmp ) = drv_spi_read_write_byte( 0xFF );	//读数据
    }
	
    RF24L01_CSN_ON;		//ʹ�ܣ���ʼ��������
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
		NRF24L01_Write_Buf( TX_ADDR, buf, 5 );			//д��5 ���ֽڵĵ�ַ
		NRF24L01_Read_Buf( TX_ADDR, read_buf, 5 );		//����д��ĵ�ַ
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
  * @brief :NRF24L01��״̬�Ĵ���
  * @param :��
  * @note  :��
  * @retval:RF24L01״̬
  */

uint8_t NRF24L01_Read_Status_Register( void )
{
    uint8_t Status;
	
    RF24L01_CSN_OFF;		//ʹ��Ƭѡ
	
    Status = drv_spi_read_write_byte( NRF_READ_REG + STATUS );	//��״̬�Ĵ���
	
    RF24L01_CSN_ON;		//取消片选
	
    return Status;
}

uint8_t NRF24L01_Clear_IRQ_Flag( uint8_t IRQ_Source )
{
    uint8_t btmp = 0;

    IRQ_Source &= ( 1 << RX_DR ) | ( 1 << TX_DS ) | ( 1 << MAX_RT );	//�жϱ�־����
    btmp = NRF24L01_Read_Status_Register( );			//��״̬�Ĵ���
			
	RF24L01_CSN_OFF;
    drv_spi_read_write_byte( NRF_WRITE_REG + STATUS );	//д״̬�Ĵ�����
    drv_spi_read_write_byte( IRQ_Source | btmp );		//����Ӧ�жϱ�־
    RF24L01_CSN_ON;
	
    return ( NRF24L01_Read_Status_Register( ));			//����״̬�Ĵ���״̬
    
}

uint8_t RF24L01_Read_IRQ_Status( void )
{
    return ( NRF24L01_Read_Status_Register( ) & (( 1 << RX_DR ) | ( 1 << TX_DS ) | ( 1 << MAX_RT )));	//返回中断状态
}

void RF24L01_Init( void )
{
    //uint8_t addr[5] = {INIT_ADDR};

    RF24L01_CE_OFF;
    NRF24L01_Clear_IRQ_Flag( IRQ_ALL );
#if DYNAMIC_PACKET == 1
/*
ʹ�ܽ��չܵ�x ��̬���س���(��ͬʱ����
FEATURE�е�EN_DPL��EN_AA�е��Զ�Ӧ��ENAA_P5)
����Ϊ��̬���ݳ��Ⱥ���Խ���ÿ�����ݵĲ�ͬ����
*/
    NRF24L01_Write_Reg( DYNPD, ( 1 << 0 ) ); //ʹ��ͨ��0��̬���ݳ���
    
    //ʹ NO_ACK λΪ 1 ��Ҫ������ FEATURE �Ĵ����е� EN_DYN_ACK λΪ 1����ʹ��
//W_TX_PAYLOAD_NOACK ����д FIFO�����յ�һ�������İ��󣬽��ն˲��ᷢ�� ACK ȷ��
//�źŸ����䷽������ʹ���ն˹����� ACK ����ģʽ����
//	5��Si24R1 �� NRF2401P ���ͨ�����⣺
//	L01_WriteSingleReg(L01REG_FEATURE,0x07); 
//	�ĳ�
//	L01_WriteSingleReg(L01REG_FEATURE,0x04); 
//	˵����
//	(1)�ӳ����Ͽ���û���õ� Payload With ACK, ���ǽ��ն˻ظ� ACK ʱ���� Payload����ˣ�
//	Bit 1��0x1D.1��Ϊ 0 
//	(2)�ӳ����Ͽ���Ϊ ACK ͨ��ģʽ����ˣ�Bit 0(0x1D.0) Ϊ 0

   // NRF24L01_Write_Reg( FEATRUE, 0x07 ); // 
    NRF24L01_Write_Reg( FEATRUE, 0x04 ); // �������ص�ack����
    
    NRF24L01_Read_Reg( DYNPD );
    NRF24L01_Read_Reg( FEATRUE );
	
#elif DYNAMIC_PACKET == 0
    
    L01_WriteSingleReg( L01REG_RX_PW_P0, FIXED_PACKET_LEN ); //�̶����ݳ���
    
	
#endif	//DYNAMIC_PACKET

    RF24L01_CE_OFF;
	NRF24L01_Write_Reg( CONFIG,0x08 );	//
	time_delay_ms(10);
    NRF24L01_Write_Reg( CONFIG, /*( 1<<MASK_RX_DR ) |*/			//�����ж�
    
                                      ( 1 << EN_CRC ) |     //ʹ��CRC 1���ֽ�
                                      ( 0 << CRCO) |			// CRC8bit
                                      ( 1 << PWR_UP ) );    	//�����豸
                                      
    NRF24L01_Write_Reg( EN_AA, ( 1 << ENAA_P0 ) );   		//ͨ��0 �Զ�Ӧ��
    NRF24L01_Write_Reg( EN_RXADDR, ( 1 << ERX_P0 ) );		//ʹ��ͨ��0 ����
    NRF24L01_Write_Reg( SETUP_AW, AW_5BYTES );     			//��ַ���
    NRF24L01_Write_Reg( SETUP_RETR, ARD_4000US |( REPEAT_CNT & 0x0F ) );         	//�ظ��ȴ�ʱ��250us
    
    NRF24L01_Write_Reg( RF_CH, wifi_id.wifi_rfc );             			//��ʼ��ͨ��
//    NRF24L01_Write_Reg( RF_CH, 60 );             			//��ʼ��ͨ��
    NRF24L01_Write_Reg( RF_SETUP, 0x27 );					//лƵ������Ϊ250K,��ֵԽ�߲෢�;���Խ�����ԽԶ
															// ���͹���Ϊ7dbm
    NRF24L01_Set_TxAddr( &wifi_id.TX_adress[0], 5 );                      //����TX ��ַ
    NRF24L01_Set_RxAddr( 0, &wifi_id.TX_adress[0], 5 );                   //����P0�ܵ�RX ��ַ���ڽ���ACKӦ��
    //NRF24L01_Set_TxAddr( &addr[0], 5 );                      //����TX ��ַ
    //NRF24L01_Set_RxAddr( 0, &addr[0], 5 );                   //����P0�ܵ�RX ��ַ���ڽ���ACKӦ��
//	NRF24L01_Write_Reg( FLUSH_TX,0xff );	//���FIFO
//	NRF24L01_Write_Reg( STATUS,0xff );	//���STATUS

}

uint8_t NRF24L01_TxPacket( uint8_t *txbuf, uint8_t Length )
{
	uint8_t l_Status = 0;
	uint16_t l_MsTimes = 0;
	
	RF24L01_CSN_OFF;
	drv_spi_read_write_byte( FLUSH_TX );
	RF24L01_CSN_ON;
	
    RF24L01_CE_OFF;
	NRF24L01_Write_Buf( WR_TX_PLOAD, txbuf, Length );	//��Ҫ���͵����ݶ���fifo
    RF24L01_CE_ON;
	while( 0 != RF24L01_GET_IRQ_STATUS( ))
	{
		Feed_IWDG();
		if(NRF24L01_Read_Reg(STATUS) & MAX_TX)break; // �ط�����������˳�while
		
		time_delay_ms( 1 );
		if( 200 == l_MsTimes++ )						//���ͳ�ʱ
		{
			//RF24L01_GPIO_Init( );
			//RF24L01_Init( );
			//RF24L01_Set_Mode( MODE_TX );
			break;
		}
	}
	l_Status = NRF24L01_Read_Reg(STATUS);						//��ȡ״̬�Ĵ������� 
	NRF24L01_Write_Reg( STATUS, l_Status );						//���״̬�Ĵ�����־λ
	
	if( l_Status & MAX_TX )	//�ط����������ж�
	{
		NRF24L01_Write_Reg( FLUSH_TX,0xff );	//���FIFO
		NRF24L01_Write_Reg( STATUS,0xff );	//���STATUS
		return MAX_TX; 
	}
	if( l_Status & TX_OK )	// �������ݳɹ�
	{
		return TX_OK;
	}
	
	return 0xFF;	//���ͳ�ʱ
}

void wifi24G_send_data(void)
{
	uint8_t i;
	uint8_t index = 0;
	uint8_t Send_status = 0;
	static uint8_t send_times = 0; //�ٷ�����
	static uint8_t send_overTime = 0; //���ͳ�ʱ����
	
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
		return; // ���ڽ�������ʱ���ܷ���
	}
	
	RF24L01_CE_OFF;
	RF24L01_Set_Mode( MODE_TX );		//����ģʽ
	
	//memcpy(&Cache.Cache_SBf[0], Usart11_shadow[index].usart11_shadow_buffer, Usart11_shadow[index].shadow_send_size); // load data to DMA send buffer
	Send_status = NRF24L01_TxPacket( &App_Buffer[index].app_send_buffer[0], App_Buffer[index].app_send_size );
	if(Send_status == TX_OK)
	{
		App_Buffer[index].app_send_size = 0;
		usart22_send.send_interval22 = 60;
		RF24L01_CE_OFF;
		RF24L01_Set_Mode( MODE_RX );		//���ͳɹ����л�Ϊ����ģʽ������ռ���״̬
		RF24L01_CE_ON;
		send_times = 0;
		send_overTime = 0;
	}
	else if(Send_status == MAX_TX)//������ط��������˾��ٷ�
	{
		App_Buffer[index].app_send_size = 0;
		
		send_times += 1;
		if(send_times >= 10)
		{
			App_Buffer[index].app_send_size = 0;
			send_times = 0;
		}
		
		RF24L01_CE_OFF;
		RF24L01_Set_Mode( MODE_RX );		//���� �������л�Ϊ����ģʽ������ռ���״̬
		RF24L01_CE_ON;
		usart22_send.send_interval22 = 60;
	}
	else //����Ƿ��ͳ�ʱ���³�ʼ�����ط�����
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
	NRF24L01_Write_Reg( FLUSH_TX,0xff );	//���FIFO
	NRF24L01_Write_Reg( STATUS,0xff );	//���STATUS
 //   NRF24L01_Write_Reg( CONFIG, /*( 1<<MASK_RX_DR ) |*/			//�����ж�
 //   
 //                                     ( 1 << EN_CRC ) |     //ʹ��CRC 1���ֽ�
 //                                     ( 0 << CRCO) |			// CRC8bit
 //                                     (  SHUTDOWN<<1 ) );    	// shutdown
}

void wifi_wakeup(void)
{
    NRF24L01_Write_Reg( CONFIG, /*( 1<<MASK_RX_DR ) |*/			//�����ж�
    
                                      ( 1 << EN_CRC ) |     //ʹ��CRC 1���ֽ�
                                      ( 0 << CRCO) |			// CRC8bit
                                      (  PWR_UP << 1) );    	// wakeup
}

