#ifndef __TMC2208_H
#define __TMC2208_H
#include "stm32f0xx.h"


#define TMC2208_REGISTER_COUNT   128
#define TMC_ACCESS_NONE        0x00
#define TMC_ACCESS_READ        0x01
#define TMC_ACCESS_WRITE       0x02

#define TMC2208_SYNC	0x05
#define TMC2208_SLAVE_ADDR	0x01
#define TMC2208_WRITE_BIT	0x80

//TMC2208 Register
#define TMC2208_GCONF	0x00
#define TMC2208_GSTAT	0x01
#define TMC2208_IFCNT	0x02
#define TMC2208_SLAVECONF	0x03
#define TMC2208_OTP_PROG	0x04
#define TMC2208_OTP_READ	0x05
#define TMC2208_IOIN	0x06	
#define TMC2208_FACTORY_CONF	0x07
#define TMC2208_IHOLD_IRUN		0x10
#define TMC2208_TPOWER_DOWN		0x11
#define TMC2208_TSTEP		0x12
#define TMC2208_TPWMTHRS		0x13
#define TMC2208_TCOOLTHRS		0x14
#define TMC2208_VACTUAL		0x22
#define TMC2208_SGTHRS		0x40
#define TMC2208_COOLCONF		0x42
#define TMC2208_MSCNT		0x6A
#define TMC2208_MSCURACT		0x6B
#define TMC2208_CHOPCONF		0x6C
#define TMC2208_DRV_STATUS		0x6F
#define TMC2208_PWMCONF		0x70
#define TMC2208_PWM_SCALE		0x71
#define TMC2208_PWM_AUTO		0x72


// Default Register values
#define R00 0x00000141  // GCONF
#define R10 0x00011F10  // IHOLD_IRUN
#define R6C 0x14010053  // CHOPCONF
#define R70 0xC80d0e24  // PWMCONF

typedef enum{
	PRESCALE_256 = 0,
	PRESCALE_128,	
	PRESCALE_64,
	PRESCALE_32,
	PRESCALE_16,
	PRESCALE_8,
	PRESCALE_4,
	PRESCALE_2	
}PRESCALE_EMUN;

#define MSTEP_REGI_RESOLUTION	(1<<7)
#define MS1_MS2_RESOLUTION		(0<<7)
#define RESOLUTION_256 		((uint32_t)PRESCALE_256<<24)
#define RESOLUTION_128 		((uint32_t)PRESCALE_128<<24)
#define RESOLUTION_64 		((uint32_t)PRESCALE_64<<24)
#define RESOLUTION_32 		((uint32_t)PRESCALE_32<<24)
#define RESOLUTION_16 		((uint32_t)PRESCALE_16<<24)
#define RESOLUTION_8 		((uint32_t)PRESCALE_8<<24)
#define RESOLUTION_4 		((uint32_t)PRESCALE_4<<24)
#define RESOLUTION_2 		((uint32_t)PRESCALE_2<<24)

#define MOTOR_STANDBY_CURENT	0x15 // STANDBY CURENT IS 2/3
#define MOTOR_RUN_CURENT		(0x1A<<8)
#define STEP_FILTERING	(1<8)
#define PWM_GRAD	(0x40<<8)
// Helper macros
#define TMC_IS_READABLE(x)    (x & TMC_ACCESS_READ)
#define TMC_IS_WRITABLE(x)    (x & TMC_ACCESS_WRITE)

#define TMC2208_SET_BIT(X, Y)	((X) |= (Y))
#define TMC2208_CLEAR_BIT(X, Y)	((X) &= ~(Y))

typedef struct{
	uint8_t Sync;
	uint8_t Slave_addr;
	uint8_t Regi_addr;
	uint8_t Value3;
	uint8_t Value2;
	uint8_t Value1;
	uint8_t Value0;
	uint8_t Crc;
}TMC2208_Data;

typedef union{
	TMC2208_Data Tmc_value;
	uint8_t Datagram_Str[8];
}DATAGRAM_STRUCTURE;

#endif

