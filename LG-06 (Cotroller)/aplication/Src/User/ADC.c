

#include "stm32f030x8.h"
#include "stm32f0xx_ll_adc.h"
#include "stm32f0xx_ll_dma.h"
#include "stm32f0xx_ll_gpio.h"
#include "stm32f0xx_ll_bus.h"
#include "stm32f0xx_ll_usart.h"

#include "ADC.h"
#include "uart.h"
#include "motor_dri.h"
#include "oled.h"
#include "gpio.h"
#include "low_power.h"

#define CALIBRATION_TIMEOUT       ((uint32_t)0x0000F000)
#define ADC_TAG	 1
__IO uint16_t ADCConvertedValue=0;       // ADC为12位模数转换器，只有ADCConvertedValue的低12位有效

uint16_t adc_send_time=0;

float adc_buffer[50] = {0};
float adc_buffer22[50] = {0};
float adc_back = 0;
float adc_cenrent = 0;
float adc_active = 0;
uint16_t low_voltage=0;
uint8_t adc_init = 0;

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

void Dma_int(void)
{
	LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_DMA1);
	//LL_DMA_SetPeriphRequest(DMA1, LL_DMA_CHANNEL_1, 0);
	LL_DMA_SetPeriphAddress(DMA1, LL_DMA_CHANNEL_1, ADC1_DR_Address);
	LL_DMA_SetMemoryAddress(DMA1, LL_DMA_CHANNEL_1, (uint32_t)&ADCConvertedValue);
	LL_DMA_SetDataLength(DMA1, LL_DMA_CHANNEL_1, ADC_TAG); //总的采集通道数量ADC_TAG

	LL_DMA_SetDataTransferDirection(DMA1, LL_DMA_CHANNEL_1, LL_DMA_DIRECTION_PERIPH_TO_MEMORY);
	
	LL_DMA_SetChannelPriorityLevel(DMA1, LL_DMA_CHANNEL_1, LL_DMA_PRIORITY_LOW);
	
	LL_DMA_SetPeriphIncMode(DMA1, LL_DMA_CHANNEL_1, LL_DMA_PERIPH_NOINCREMENT);
	
	LL_DMA_SetMemoryIncMode(DMA1, LL_DMA_CHANNEL_1, LL_DMA_MEMORY_INCREMENT); //ENABLE 后移
	
	LL_DMA_SetPeriphSize(DMA1, LL_DMA_CHANNEL_1, LL_DMA_PDATAALIGN_HALFWORD);
	
	LL_DMA_SetMemorySize(DMA1, LL_DMA_CHANNEL_1, LL_DMA_MDATAALIGN_HALFWORD);
	LL_DMA_SetMode(DMA1, LL_DMA_CHANNEL_1, LL_DMA_MODE_CIRCULAR);
	LL_DMA_EnableChannel(DMA1, LL_DMA_CHANNEL_1);
}

uint32_t ADC_GetCalibrationFactor(ADC_TypeDef* ADCx)
{
  uint32_t tmpreg = 0, calibrationcounter = 0, calibrationstatus = 0;
  
  /* Set the ADC calibartion */
 LL_ADC_StartCalibration(ADC1);
  
  /* Wait until no ADC calibration is completed */
  do
  {
    calibrationstatus = ADCx->CR & ADC_CR_ADCAL;
    calibrationcounter++;  
  } while((calibrationcounter != CALIBRATION_TIMEOUT) && (calibrationstatus != 0x00));
    
  if((uint32_t)(ADCx->CR & ADC_CR_ADCAL) == RESET)
  {
    /*Get the calibration factor from the ADC data register */
    tmpreg = ADCx->DR;
  }
  else
  {
    /* Error factor */
    tmpreg = 0x00000000;
  }
  return tmpreg;
}


void Adc_function_init(void)
{
	LL_ADC_REG_SetSequencerChannels(ADC1, LL_ADC_CHANNEL_0);
//	LL_ADC_REG_SetSequencerChannels(ADC1, LL_ADC_CHANNEL_0|LL_ADC_CHANNEL_1);

	LL_ADC_SetResolution(ADC1, LL_ADC_RESOLUTION_12B);
	LL_ADC_REG_SetContinuousMode(ADC1, LL_ADC_REG_CONV_CONTINUOUS);
	LL_ADC_REG_SetTriggerEdge(ADC1, LL_ADC_REG_TRIG_EXT_None);
	LL_ADC_SetDataAlignment(ADC1, LL_ADC_DATA_ALIGN_RIGHT);
	LL_ADC_REG_SetSequencerScanDirection(ADC1, LL_ADC_REG_SEQ_SCAN_DIR_BACKWARD);
	LL_ADC_SetSamplingTimeCommonChannels(ADC1, LL_ADC_SAMPLINGTIME_239CYCLES_5);
	
	ADC_GetCalibrationFactor(ADC1);//校验	
	LL_ADC_ClearFlag_ADRDY(ADC1);
	LL_ADC_REG_SetDMATransfer(ADC1, LL_ADC_REG_DMA_TRANSFER_UNLIMITED);
	LL_ADC_Enable(ADC1);
	
	/* Wait the ADRDY flag */
	while(!LL_ADC_IsActiveFlag_ADRDY(ADC1)); 
	
	LL_ADC_REG_StartConversion(ADC1);
}


void A_d_c_int(void)
{
	Adc_Gpio_init();
	Adc_function_init();
	Dma_int();
}


void read_ADC_value(void)
{
		uint8_t i;
	//	uint8_t tag=0;
		float spr=0;
		float temp=0;
		float Adc_value=0;
		//float Adc_value22=0;
		float basic_data=0;
		float aaa = 3300;
		float bbb = 4096;
		//float Voltage_reference22 = 5.578;
		float Voltage_reference = 5.61;
		static uint8_t ii=0;

		//static uint8_t tag = 1 , tag11 = 0;;
	
	//	if(BATERRY_STATA == 0)return;
		
		if(adc_send_time == 0)
		{		
		
			if(adc_init == 0)adc_send_time = 20;
			else {
			if(CHARGE_STATUS == CHARGING)adc_send_time = 500;
			else adc_send_time = 1000;
			
			}
			//read_ADC_value22();
			basic_data = aaa/bbb;
			Adc_value = ADCConvertedValue;
			Adc_value = (float)Adc_value * basic_data / 1000.0;
			/////////////////////////////////////////////////////
			
			for(i=9;i>0;i--)
			{
				adc_buffer[i] = adc_buffer[i-1];
				spr += adc_buffer[i];
				/////////////////////////////////
				adc_buffer22[i] = adc_buffer22[i-1];
				temp += adc_buffer22[i];
			}
			
			adc_buffer[0] = Adc_value;//ADCConvertedValue;
			Adc_value = (spr + adc_buffer[0]) / 10.0;
			/////////////////////////////////////////////
			if(adc_init == 0)
			{
				ii++;
				if(ii > 10)
				{
					ii=0;
					adc_init = 1;
				}
				else
				{
					return;
				}
			}
			else
			{
			//	ii++;
			}
			
			Adc_value = Adc_value * Voltage_reference;
			///////////////////////////////////////
			low_voltage = Adc_value*100;
			adc_cenrent = Adc_value;
			filtering();
			percentage_get();
			//String_Printf((uint8_t *)&low_voltage, 2);
			if(Adc_value < 3.2) //shutdown
			{
				//bornd_code_dis(1,2);
				if(CHARGE_STATUS == CHARGING)
				{
					Charging_Dis();
					TurnOff_ShutDonw_Battery_dis(0, adc_active, 0);
					battery_data_display(adc_active, 0, CONTROLLER);
					return;
				}
				else
				{
					if(return_powers())return;
					ShutDown();
				}
				//
			}
			else
			{
				if(CHARGE_STATUS == CHARGING)
				{
					Charging_Dis();
					TurnOff_ShutDonw_Battery_dis(0, adc_active, 0);
				}
				battery_data_display(adc_active, 0, CONTROLLER);
			/*	temp11 = 20;
				if(Adc_value >= 3.3)temp22 = (Adc_value - 3.3) * temp11;
				else temp22 = 2;
				
				if(temp22 > 18)temp22 = 19;
				
				battery_display(temp22, CONTROLLER);*/
			}

			
				
	}
}

void filtering(void)
{
	static uint8_t check_temp = 0;
	if(CHARGE_STATUS == CHARGING)
	{
		/*if(adc_cenrent > adc_back)
		{
			if((adc_cenrent-adc_back)>=0.07)
			{
				adc_active = adc_cenrent - adc_back - 0.07 + adc_back;
			}
			else
			{
				adc_active = adc_back;
			}
			
		}*/

		if(adc_cenrent > adc_active)
		{
			if(adc_active < 2)
			{
				adc_active = adc_cenrent-0.06;
				
				adc_back = adc_active;
			}
			if((adc_cenrent - adc_active)>=0.008)
			{
				check_temp += 1;
				if(check_temp > 200)
				{
					check_temp = 0;
					adc_active += 0.008;
				}
			}
		}

		//if(adc_active<temp)adc_active = temp;
	}
	else
	{
		check_temp = 0;
		if(adc_active == 0.00)
		{
			adc_active = adc_cenrent;
			//adc_back = adc_cenrent;
		}
		else
		{	
			if(adc_cenrent < adc_active)
			{
				adc_active = adc_cenrent;
			}
			else
			{
			
			}
		}	
		Set_adc_back();
	}
}

void percentage_get(void)
{
	float datat;
	if(adc_active >= 3.3)
	{
		datat = adc_active - 3.3;
	
		if(datat > BATTER_FZ)
		{
			Battery_percentage = 100;
		}
		else
		{
			Battery_percentage = datat / BATTER_FZ * 100;
	
			if(Battery_percentage < 1)Battery_percentage = 1;
		}
	}
	else
	{
		Battery_percentage = 1;
	}
}
void Set_adc_back(void)
{
	adc_back = adc_active;
}

void adc_cout(void)
{
	if(adc_send_time)adc_send_time--;
}


