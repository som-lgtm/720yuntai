
#include "TMC2208.h"
#include "common.h"
#include "UART.h"
//#include "stm32f0xx_ll_gpio.h"
extern void usart_fw_tx(uint8_t TX_size, uint8_t *value_da);

const uint8_t tmc2208_defaultRegisterAccess[TMC2208_REGISTER_COUNT] =
{
//	0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F
	3, 3, 1, 2, 2, 1, 1, 3, 0, 0, 0, 0, 0, 0, 0, 0, // 0x00 - 0x0F
	2, 2, 1, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 0x10 - 0x1F
	0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 0x20 - 0x2F
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 0x30 - 0x3F
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 0x40 - 0x4F
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 0x50 - 0x5F
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 3, 0, 0, 1, // 0x60 - 0x6F
	3, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0  // 0x70 - 0x7F
};

// Register default value
const uint32_t tmc2208_defaultRegisterResetState[TMC2208_REGISTER_COUNT] =
{
//	0    	 1        2       3       4       5       6       7       8       9       A      B       C       D       E       F
	R00, 0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, // 0x00 - 0x0F
	R10, 0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, // 0x10 - 0x1F
	0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, // 0x20 - 0x2F
	0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, // 0x30 - 0x3F
	0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, // 0x40 - 0x4F
	0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, // 0x50 - 0x5F
	0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   R6C, 0,   0,   0, // 0x60 - 0x6F
	R70, 0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0  // 0x70 - 0x7F
};

static uint8_t swuart_calcCRC(uint8_t* datagram, uint8_t datagramLength)
{
	int i,j;
	uint8_t* crc = datagram + (datagramLength-1); // CRC located in last byte of message
	uint8_t currentByte;
	*crc = 0;
	for (i=0; i<(datagramLength-1); i++) { // Execute for all bytes of a message
		currentByte = datagram[i]; // Retrieve a byte to be sent from Array
		for (j=0; j<8; j++) {
			if ((*crc >> 7) ^ (currentByte&0x01)) // update CRC based result of XOR operation
			{
			*crc = (*crc << 1) ^ 0x07;
			}
			else
			{
			*crc = (*crc << 1);
			}
			currentByte = currentByte >> 1;
		} // for CRC bit
	} // for message byte

	return *crc;
}


void TCM2208Register_Write(uint8_t address, uint32_t value)
{
	DATAGRAM_STRUCTURE value_d;
	
	if((address >= TMC2208_REGISTER_COUNT) || (!TMC_IS_WRITABLE(tmc2208_defaultRegisterAccess[address])))
		return;
	
	value_d.Tmc_value.Sync = TMC2208_SYNC;
	value_d.Tmc_value.Slave_addr = TMC2208_SLAVE_ADDR;
	value_d.Tmc_value.Regi_addr = address | TMC2208_WRITE_BIT;
	value_d.Tmc_value.Value3 = value >> 24;
	value_d.Tmc_value.Value2 = value >> 16;
	value_d.Tmc_value.Value1 = value >> 8;
	value_d.Tmc_value.Value0 = value & 0xff;
	value_d.Tmc_value.Crc = swuart_calcCRC(value_d.Datagram_Str, 8);

	//Usart11_Send_Handling(8, value_d.Datagram_Str);
//	usart_fw_tx(8, value_d.Datagram_Str);
}

void TCM2208Register_Read(uint8_t address)
{
		uint8_t writeData[4] = {0};
		
		if((address >= TMC2208_REGISTER_COUNT) || (TMC_IS_READABLE(tmc2208_defaultRegisterAccess[address])))
			return;
		
		writeData[0] = 0x05;						 // Sync byte
		writeData[1] = 0x00;						 // Slave address
		writeData[2] = address;  // Register address with write bit set
		writeData[3] = swuart_calcCRC(writeData, 4);	 // Cyclic redundancy check
		
		Usart11_Send_Handling(4, writeData);
}

void TMC2208_Resolution_Set(uint32_t resolution)
{
	uint32_t temp =0; 
	temp = tmc2208_defaultRegisterResetState[TMC2208_GCONF];
	TCM2208Register_Write(TMC2208_GCONF, temp | MSTEP_REGI_RESOLUTION);

	temp = tmc2208_defaultRegisterResetState[TMC2208_CHOPCONF];
	TMC2208_CLEAR_BIT(temp, (0x0F<<24));
	TMC2208_SET_BIT(temp, resolution);
	TCM2208Register_Write(TMC2208_CHOPCONF, temp);
}

void TMC2208_Standby_curent(void)
{
	uint32_t temp =0; 
	
	temp = tmc2208_defaultRegisterResetState[TMC2208_IHOLD_IRUN];
	
//	TMC2208_CLEAR_BIT(temp, MOTOR_STANDBY_CURENT);
	TMC2208_SET_BIT(temp, MOTOR_STANDBY_CURENT);

	TCM2208Register_Write(TMC2208_IHOLD_IRUN, temp);
}

// status=0;->No filtering of STEP pulses
//status=1:->filtering of STEP pulses
void TMC2208_StepFilt_If(uint8_t status)
{
	uint32_t temp =tmc2208_defaultRegisterResetState[TMC2208_GCONF]; 
	
	if(status)
	{
		TMC2208_SET_BIT(temp, STEP_FILTERING);
	}
	else
	{
		TMC2208_CLEAR_BIT(temp, STEP_FILTERING);
	}

	TCM2208Register_Write(TMC2208_GCONF, temp);
}

void TMC2208_init(void)
{
//	TCM2208Register_Write(TMC2208_GCONF, 0x000001c1);
//	TCM2208Register_Write(TMC2208_CHOPCONF, 0x10010053);
	TMC2208_Resolution_Set(RESOLUTION_256);
	TMC2208_Standby_curent();
//	TCM2208Register_Write(TMC2208_TPWMTHRS, 0xffffffff);
//	TMC2208_StepFilt_If(0);

}

