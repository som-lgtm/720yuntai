#ifndef __MATRIX_H
#define __MATRIX_H

#define MATRIX_ID	134
#define KR_MAX_ID	66
#define SPESHI_ID	10

typedef struct{
	unsigned char *zhi;
	unsigned  char matrix[24];
}ZHI_MATRIX;

typedef struct{
	unsigned char *zhi;
	uint8_t matrix[128];
}SPECIAL_ZHI;


/////////////////////////////////////////////////////////////////////////
// ������ģ��                                                          //
//// Һ����ģ����: Font:windows font 18  ����24, ����ȡģ�Ҹ�λ,��������:�����Ҵ��ϵ���               //
// ��ģ3��ǿ������: ���峣��9�ţ���ɫ����Һ����ģ������ȡģ���ֽڵ���; ����12X12

/////////////////////////////////////////////////////////////////////////
const ZHI_MATRIX zhi_f16x16[MATRIX_ID]={ // 12X12
	"��", 0xFC,0x01,0x04,0x01,0xFC,0x01,0x04,0x01,0xFC,0x01,0x00,0x00,0xFF,0x07,0x20,0x00, //
			0xE4,0x03,0x24,0x00,0x2A,0x00,0xF1,0x07,
	
	"��", 0x02,0x00,0xE4,0x03,0x2F,0x02,0xA8,0x02,0xA4,0x02,0xAC,0x02,0xB6,0x02,0xA5,0x02, //
			0x44,0x01,0x44,0x01,0x24,0x05,0x14,0x06,
	
	"Ƶ", 0xC8,0x07,0x3A,0x01,0x8A,0x00,0xCA,0x07,0x7F,0x04,0x48,0x05,0x4A,0x05,0x6A,0x05, //
			0x51,0x05,0x08,0x01,0x84,0x02,0x43,0x04,
	
	"��", 0x00,0x06,0xEF,0x01,0x08,0x01,0x04,0x01,0x22,0x01,0x2E,0x07,0x28,0x01,0x28,0x01, //
			0x2A,0x01,0xE4,0x07,0x0A,0x00,0xF1,0x07,
	
	"ʱ", 0x00,0x02,0x0F,0x02,0x09,0x02,0xE9,0x07,0x09,0x02,0x0F,0x02,0x49,0x02,0x89,0x02, //
			0x89,0x02,0x0F,0x02,0x09,0x02,0x80,0x03,
	
	
	"��", 0xE2,0x01,0x24,0x01,0x20,0x01,0x20,0x01,0x17,0x06,0x04,0x00,0xF4,0x03,0x24,0x02, //
			0x44,0x01,0x8C,0x00,0x44,0x01,0x30,0x06,
	
	"��", 0xFE,0x03,0x52,0x02,0xFE,0x03,0x20,0x00,0xFF,0x07,0x20,0x00,0xFC,0x01,0x04,0x01, //
			0x04,0x01,0x24,0x01,0x24,0x01,0xFF,0x07,
	
	"��", 0x41,0x00,0xFA,0x07,0x40,0x00,0xF8,0x03,0x4B,0x02,0x4A,0x02,0xFA,0x03,0xE2,0x00, //
			0x52,0x01,0x4A,0x02,0x42,0x00,0xFD,0x07,
	
	"ͣ", 0x88,0x00,0xF8,0x07,0x04,0x00,0xE4,0x03,0x26,0x02,0xE5,0x03,0x04,0x00,0xF4,0x07, //
			0x14,0x04,0xE4,0x03,0x84,0x00,0xC4,0x00,
	
	"��", 0xF2,0x07,0x04,0x04,0x00,0x04,0xFA,0x05,0x0A,0x05,0x0A,0x05,0xFA,0x05,0x0A,0x05, //
			0x0A,0x05,0xFA,0x05,0x02,0x04,0x02,0x07,
	
	"��", 0x40,0x00,0x4F,0x02,0x48,0x01,0xC8,0x00,0x4F,0x00,0xE1,0x07,0x41,0x01,0x4F,0x01, //
			0x48,0x02,0x48,0x02,0xC8,0x04,0x46,0x00,
	
	"��", 0x88,0x00,0xAA,0x00,0x88,0x00,0xBF,0x07,0x4C,0x02,0x9A,0x02,0xA9,0x02,0x84,0x02, //
			0x9F,0x02,0x12,0x01,0x8C,0x02,0x53,0x04,
	
	"��", 0x20,0x00,0x20,0x00,0x20,0x00,0xFE,0x03,0x22,0x02,0x22,0x02,0x22,0x02,0xFE,0x03, //
			0x22,0x02,0x20,0x00,0x20,0x00,0x20,0x00,
	
	"��", 0x10,0x00,0x20,0x00,0xFF,0x07,0x04,0x01,0x04,0x01,0x88,0x00,0x88,0x00,0x50,0x00, //
			0x20,0x00,0x50,0x00,0x8C,0x01,0x03,0x06,
	
	"��", 0x80,0x00,0x8E,0x00,0x80,0x00,0xC0,0x07,0x9F,0x04,0x84,0x04,0x82,0x04,0x8A,0x04, //
			0x91,0x04,0x5F,0x04,0x40,0x04,0x20,0x03,
	
	"��", 0x20,0x00,0xE0,0x07,0x20,0x00,0x20,0x00,0xFE,0x03,0x02,0x02,0x02,0x02,0x02,0x02, //
			0xFE,0x03,0x00,0x00,0x4A,0x02,0x91,0x04,
		
	"��", 0xC0,0x03,0x3E,0x00,0x20,0x00,0x20,0x00,0xFE,0x03,0x20,0x00,0x20,0x00,0xFF,0x07, //
			0x20,0x00,0x20,0x00,0x20,0x00,0x38,0x00,
	
	"��", 0x02,0x00,0xE4,0x07,0x4F,0x04,0x48,0x04,0x44,0x04,0x54,0x04,0x4E,0x04,0x55,0x04, //
			0x44,0x04,0x24,0x04,0x24,0x04,0x14,0x03,
	
	"��", 0x48,0x00,0x48,0x00,0x44,0x04,0x44,0x02,0x46,0x01,0xC5,0x00,0x44,0x00,0x64,0x00, //
			0x54,0x00,0x44,0x04,0x44,0x04,0x84,0x07,
	
	"ѭ", 0x04,0x06,0xF2,0x01,0x11,0x01,0xF4,0x07,0x12,0x01,0xD3,0x07,0x52,0x04,0xD2,0x07, //
			0x52,0x04,0xD2,0x07,0x52,0x04,0xCA,0x07,
	
	"��", 0x00,0x00,0xEF,0x07,0x04,0x01,0x84,0x00,0x84,0x00,0xCF,0x02,0xA4,0x04,0x94,0x04, //
			0x84,0x00,0x8C,0x00,0x83,0x00,0x80,0x00,
		
	"��", 0x20,0x00,0x40,0x00,0xFC,0x07,0x04,0x04,0x04,0x04,0xFC,0x07,0x04,0x00,0xFC,0x07, //
			0x0C,0x04,0x0A,0x04,0xFA,0x07,0x09,0x04,
	
	"��", 0x20,0x00,0x10,0x00,0xFE,0x03,0x02,0x02,0x02,0x02,0xFE,0x03,0x02,0x02,0x02,0x02, //
			0xFE,0x03,0x02,0x02,0x02,0x02,0xFE,0x03,
	
	"��", 0x80,0x03,0x7C,0x00,0x04,0x00,0x04,0x00,0xFC,0x07,0x04,0x00,0x04,0x00,0xF4,0x07, //
			0x14,0x04,0x12,0x04,0xF2,0x07,0x11,0x04,
			
	"��", 0x40,0x01,0x40,0x02,0xFE,0x07,0x42,0x00,0x42,0x00,0x5E,0x02,0x52,0x02,0x52,0x01, //
			0x92,0x04,0x5A,0x05,0x22,0x06,0x11,0x04,
	
	"��", 0x04,0x03,0xDF,0x00,0x4A,0x00,0xDE,0x07,0x48,0x02,0x5F,0x02,0x28,0x02,0xFC,0x01, //
			0x04,0x01,0xFC,0x01,0x04,0x01,0xFC,0x01,
	
	"��", 0xF9,0x07,0x42,0x00,0xF0,0x03,0x20,0x02,0x23,0x02,0xFA,0x07,0x02,0x00,0xF2,0x03, //
			0x12,0x02,0x16,0x02,0xF2,0x03,0x10,0x02,
	
	"��", 0x10,0x00,0x20,0x00,0xFF,0x07,0x00,0x00,0xFE,0x03,0x00,0x00,0xFE,0x03,0x00,0x00, //
			0xFE,0x03,0x02,0x02,0xFE,0x03,0x02,0x02,

	"ʼ", 0x84,0x00,0x84,0x00,0x44,0x00,0x2F,0x02,0xEA,0x07,0x0A,0x04,0xEA,0x03,0x29,0x02, //
		0x2A,0x02,0x24,0x02,0xEA,0x03,0x29,0x02,
		
"��", 0xF0,0x03,0x0F,0x02,0x24,0x02,0x24,0x02,0x22,0x02,0xEE,0x07,0x0B,0x04,0x0A,0x04, //
		0xFA,0x05,0x0E,0x04,0x0A,0x04,0x00,0x03,

"��", 0x41,0x00,0x42,0x00,0xF8,0x07,0x20,0x00,0x90,0x00,0xF3,0x03,0x82,0x00,0x82,0x00, //
		0xFA,0x07,0x82,0x00,0x82,0x00,0xFD,0x07,

"��", 0x44,0x00,0x84,0x00,0xF4,0x07,0x2F,0x02,0x44,0x01,0xF4,0x07,0x8C,0x00,0xF7,0x07, //
		0x44,0x02,0x64,0x02,0x84,0x01,0x76,0x06,

"��", 0x41,0x00,0xFA,0x07,0x40,0x00,0xF0,0x03,0x43,0x00,0xFA,0x07,0x12,0x02,0xF2,0x03, //
		0x12,0x02,0xF2,0x03,0x16,0x02,0x12,0x03,

"��", 0x88,0x00,0x97,0x04,0xA4,0x02,0x84,0x00,0xEF,0x07,0x24,0x04,0xE6,0x07,0x2E,0x04, //
		0xE5,0x07,0x24,0x04,0x24,0x04,0x24,0x06,

"��", 0x10,0x00,0x20,0x00,0xFF,0x07,0x01,0x04,0x00,0x00,0xFF,0x07,0x20,0x00,0x24,0x00, //
		0xE4,0x03,0x24,0x00,0x2A,0x00,0xF1,0x07,

"��", 0x00,0x02,0x00,0x02,0x1F,0x02,0xD0,0x07,0x10,0x02,0x11,0x02,0x4A,0x02,0x8A,0x02, //
		0x84,0x02,0x0A,0x02,0x12,0x02,0x91,0x03,

"��", 0x20,0x00,0xFF,0x07,0x01,0x04,0x00,0x00,0xFC,0x01,0x00,0x00,0x00,0x00,0xFF,0x07, //
		0x88,0x00,0x88,0x00,0x84,0x04,0x03,0x07,

"��", 0x08,0x00,0xC8,0x07,0x9E,0x04,0x88,0x04,0x48,0x06,0x3F,0x00,0xC8,0x07,0x4A,0x04,
		0x7A,0x04,0xCA,0x07,0x0E,0x00,0xF9,0x07,

"ʧ", 0x20,0x00,0x24,0x00,0x24,0x00,0xFC,0x01,0x22,0x00,0x21,0x00,0x20,0x00,0xFF,0x07, //
		0x50,0x00,0x88,0x00,0x04,0x01,0x03,0x06,

"��", 0x80,0x00,0x9F,0x00,0x51,0x00,0xD5,0x07,0x35,0x02,0x55,0x02,0x55,0x02,0x55,0x02, //
		0x55,0x01,0x84,0x00,0x4A,0x01,0x31,0x06,

"ȷ", 0x40,0x00,0xCF,0x03,0x24,0x02,0x04,0x01,0xE2,0x07,0xAE,0x04,0xEB,0x07,0xAA,0x04, //
		0xEA,0x07,0xAE,0x04,0xAA,0x04,0x10,0x06,
	  
"��", 0x08,0x06,0xEA,0x01,0x2A,0x00,0x2A,0x00,0xFE,0x07,0xA2,0x04,0xA2,0x04,0xAE,0x02, //
		0xAA,0x02,0x2A,0x01,0x9A,0x02,0x49,0x04,

"��", 0x20,0x00,0x20,0x00,0xFF,0x07,0x20,0x00,0x70,0x00,0xA8,0x00,0xA8,0x00,0x24,0x01, //
		0x22,0x02,0xFD,0x05,0x20,0x00,0x20,0x00,
			
"��",0x20,0x00,0xFF,0x07,0x00,0x00,0xFC,0x01,0x04,0x01,0xFC,0x01,0x00,0x00,0xFF,0x07, //
0x01,0x04,0xFC,0x00,0x84,0x04,0x03,0x07,

"ת",0x82,0x00,0x82,0x00,0xF7,0x03,0x42,0x00,0x45,0x00,0xF5,0x07,0x2F,0x00,0xE4,0x03,
0x0C,0x02,0x67,0x01,0x84,0x01,0x04,0x02,

"��",0x88,0x00,0x88,0x00,0x84,0x00,0xF4,0x07,0x86,0x00,0xC5,0x01,0xC4,0x01,0xA4,0x02, //
0xA4,0x02,0x94,0x04,0x84,0x00,0x84,0x00,

"��",0xEF,0x07,0x29,0x04,0x29,0x04,0xEF,0x07,0x29,0x01,0x29,0x01,0xEF,0x07,0x29,0x01, //
0x29,0x01,0x2F,0x05,0x69,0x06,0x20,0x04,

"ģ",0x44,0x01,0xF4,0x07,0x44,0x01,0xEF,0x03,0x24,0x02,0xE6,0x03,0x2E,0x02,0xE5,0x03, //
0x84,0x00,0xF4,0x07,0x44,0x01,0x34,0x06,

"ʽ",0x80,0x02,0x80,0x04,0x80,0x00,0xFF,0x07,0x80,0x00,0x80,0x00,0xBE,0x00,0x88,0x00, //
0x08,0x01,0x08,0x05,0x78,0x06,0x07,0x04,

"ȫ",0x20,0x00,0x50,0x00,0x88,0x00,0x04,0x01,0x03,0x06,0xFC,0x01,0x20,0x00,0x20,0x00,
0xFE,0x03,0x20,0x00,0x20,0x00,0xFF,0x07,

"��",0xFC,0x01,0x04,0x01,0xFC,0x01,0x04,0x01,0xFC,0x01,0x20,0x00,0xFF,0x07,0x04,0x01,
0xFC,0x01,0x20,0x00,0x24,0x01,0x32,0x02,

"��",0x04,0x00,0xFC,0x00,0x82,0x00,0x41,0x00,0xFE,0x03,0x22,0x02,0xFE,0x03,0x22,0x02,
0xFE,0x03,0x22,0x02,0x22,0x02,0x01,0x03,

"��",0x10,0x00,0x10,0x00,0x10,0x00,0x10,0x00,0xF0,0x03,0x10,0x00,0x10,0x00,0x10,0x00,
0x10,0x00,0x10,0x00,0x10,0x00,0xFF,0x07,

"��",0x04,0x01,0x88,0x00,0xFE,0x03,0x22,0x02,0xFE,0x03,0x22,0x02,0xFE,0x03,0x20,0x00,
0xFF,0x07,0x20,0x00,0x20,0x00,0x20,0x00,
	
"��",0xFE,0x07,0x02,0x04,0xFE,0x07,0x02,0x00,0xFA,0x03,0x02,0x00,0x02,0x00,0xFE,0x07,
0x22,0x00,0x12,0x02,0xFA,0x07,0x01,0x04,

"Ԥ",0x00,0x00,0xCE,0x07,0x08,0x01,0x8A,0x00,0xC4,0x07,0x5F,0x04,0x54,0x05,0x44,0x05,
0x44,0x05,0x04,0x01,0x84,0x02,0x46,0x04,

"��",0x02,0x07,0xF4,0x00,0x10,0x00,0xF0,0x07,0x17,0x04,0x54,0x04,0x94,0x02,0x14,0x01,
0x94,0x02,0x4C,0x04,0x0A,0x00,0xF1,0x07,

"��",0x00,0x00,0xFE,0x07,0x02,0x04,0x02,0x04,0xF2,0x04,0x92,0x04,0x92,0x04,0xF2,0x04,
0x02,0x04,0x02,0x04,0xFE,0x07,0x02,0x04,

"��",0x00,0x00,0xFE,0x03,0x88,0x00,0x88,0x00,0x88,0x00,0x88,0x00,0xFF,0x07,0x88,0x00,
0x88,0x00,0x84,0x00,0x84,0x00,0x82,0x00,

"��",0x04,0x04,0x7C,0x04,0x12,0x05,0x4A,0x05,0x7B,0x05,0x52,0x05,0x12,0x05,0x7A,0x05,
0x12,0x05,0x72,0x04,0x1A,0x04,0x02,0x06,

"��",0x82,0x00,0x84,0x00,0x80,0x00,0x80,0x00,0x87,0x00,0xF4,0x07,0x84,0x00,0x84,0x00,
0x94,0x00,0x8C,0x00,0x84,0x00,0x80,0x00,

"��",0x04,0x00,0xC4,0x07,0x1F,0x01,0x84,0x00,0xD5,0x07,0x55,0x04,0x55,0x05,0x5F,0x05,
0x44,0x05,0x14,0x01,0x8C,0x02,0x44,0x04,

"��",0x42,0x00,0xF6,0x03,0x21,0x01,0xF8,0x07,0x07,0x00,0xF2,0x03,0x12,0x02,0xF7,0x03,
0x12,0x02,0xF2,0x03,0x26,0x05,0x1A,0x07,

"ͷ",0x40,0x00,0x48,0x00,0x50,0x00,0x42,0x00,0x44,0x00,0x40,0x00,0xFF,0x07,0x20,0x00,
0xA0,0x00,0x10,0x01,0x0C,0x02,0x03,0x04,

"��",0x88,0x00,0xFF,0x07,0x88,0x00,0x02,0x00,0xF4,0x03,0x10,0x02,0x11,0x02,0x12,0x02,
0x90,0x01,0x14,0x04,0x12,0x04,0xE1,0x07,

"Χ",0xFF,0x07,0x01,0x04,0x21,0x04,0xFD,0x05,0x21,0x04,0xF9,0x04,0x21,0x04,0xFD,0x05,
0x21,0x05,0xA1,0x05,0x21,0x04,0xFF,0x07,

"��",0xF7,0x07,0x05,0x00,0xE5,0x03,0x25,0x02,0xE3,0x03,0x03,0x00,0xF5,0x07,0x55,0x05,
0x95,0x04,0xD3,0x05,0x91,0x04,0x91,0x06,

"��",0x44,0x00,0x44,0x00,0xF4,0x03,0x4D,0x02,0x45,0x02,0x45,0x02,0xFD,0x07,0x44,0x00,
0xA4,0x00,0xA4,0x00,0x14,0x01,0x0C,0x06,

"��",0xF2,0x07,0x04,0x04,0x00,0x04,0x02,0x04,0x02,0x04,0x02,0x04,0x02,0x04,0x02,0x04,
0x02,0x04,0x02,0x04,0x02,0x04,0x02,0x07,

"ԭ",0x00,0x00,0xFE,0x07,0x22,0x00,0xFA,0x03,0x0A,0x02,0xFA,0x03,0x0A,0x02,0xFA,0x03,
0x42,0x00,0x52,0x01,0x4A,0x02,0x65,0x04,

"��",0x20,0x00,0xE0,0x07,0x20,0x00,0x20,0x00,0xFE,0x03,0x02,0x02,0x02,0x02,0x02,0x02,
0xFE,0x03,0x00,0x00,0x4A,0x02,0x91,0x04,

"��",0x04,0x01,0x88,0x00,0x00,0x00,0xFE,0x03,0x20,0x00,0x20,0x00,0xFF,0x07,0x20,0x00,
0x50,0x00,0x88,0x00,0x04,0x01,0x03,0x06,

"��",0x10,0x00,0xF0,0x01,0x88,0x00,0x44,0x00,0x2B,0x01,0x90,0x00,0xCC,0x07,0x23,0x02,
0x58,0x01,0xC0,0x00,0x30,0x00,0x0F,0x00,

"��",0x20,0x00,0x21,0x00,0xE2,0x07,0x10,0x04,0x50,0x02,0x48,0x00,0x40,0x00,0xA4,0x00,
0xA2,0x00,0x11,0x01,0x08,0x02,0x04,0x04,

"һ",0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0x07,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,

"��",0x40,0x00,0xFE,0x07,0x12,0x01,0xFE,0x07,0x12,0x01,0xF2,0x01,0x02,0x00,0xFA,0x03,
0x12,0x02,0x22,0x01,0xC2,0x00,0x39,0x07,

"��",0x60,0x00,0x60,0x00,0x60,0x00,0x60,0x00,0x60,0x00,0xFF,0x0F,0xFF,0x0F,0x60,0x00,
	0x60,0x00,0x60,0x00,0x60,0x00,0x60,0x00,

"��",0x08,0x00,0x04,0x01,0xFE,0x03,0x10,0x00,0xFF,0x07,0x44,0x01,0x32,0x02,0x8D,0x04,
0x60,0x00,0x1E,0x02,0xC0,0x01,0x3E,0x00,

"��",0x00,0x00,0xFF,0x07,0x10,0x00,0x10,0x00,0x50,0x00,0x90,0x00,0x10,0x01,0x10,0x02,
0x10,0x02,0x10,0x00,0x10,0x00,0x10,0x00,

"ҳ",0xFF,0x07,0x20,0x00,0x10,0x00,0xFE,0x03,0x02,0x02,0x02,0x02,0x22,0x02,0x22,0x02,
0x22,0x02,0xD0,0x00,0x0C,0x03,0x03,0x04,

"��",0xFE,0x03,0x22,0x02,0xFE,0x03,0x22,0x02,0xFE,0x03,0x20,0x00,0xD8,0x00,0x07,0x07,
0x88,0x00,0x88,0x00,0x84,0x00,0x82,0x00,

"��",0x00,0x00,0xFF,0x07,0x20,0x00,0x10,0x00,0xFE,0x07,0x92,0x04,0xF2,0x04,0x92,0x04,
0xF2,0x04,0x92,0x04,0xFE,0x07,0x02,0x04,

"ȡ",0x00,0x00,0x3F,0x00,0xD2,0x03,0x92,0x02,0x9E,0x02,0x92,0x02,0x9E,0x02,0x92,0x02,
0x32,0x01,0x1F,0x01,0x90,0x02,0x50,0x04,

"��",0x80,0x00,0x92,0x04,0xA4,0x02,0x80,0x00,0xF1,0x07,0x12,0x04,0xF0,0x07,0x10,0x04,
0xF4,0x07,0x12,0x04,0x11,0x04,0x10,0x06,

"��",0x82,0x00,0x84,0x00,0x80,0x00,0x80,0x00,0x87,0x00,0x84,0x00,0x44,0x01,0x44,0x01,
0x54,0x01,0x2C,0x02,0x24,0x02,0x10,0x04,

"��",0x60,0x00,0x60,0x00,0x60,0x00,0x60,0x00,0x60,0x00,0x60,0x00,0x60,0x00,0x66,0x06,
0x6C,0x03,0xF8,0x01,0xF0,0x00,0x60,0x00,

"��",0x60,0x00,0xF0,0x00,0xF8,0x01,0x6C,0x03,0x66,0x06,0x60,0x00,0x60,0x00,0x60,0x00,
0x60,0x00,0x60,0x00,0x60,0x00,0x60,0x00,

"��",0xFF,0x07,0x20,0x00,0xB0,0x01,0x2C,0x02,0x23,0x04,0x20,0x00,0x00,0x00,0xFE,0x03,
0x02,0x02,0x02,0x02,0xFE,0x03,0x02,0x02,

"Ҫ",0x00,0x00,0xFF,0x07,0x50,0x00,0xFE,0x03,0x52,0x02,0xFE,0x03,0x10,0x00,0xFF,0x07,
0x08,0x01,0x8C,0x00,0x70,0x00,0x8F,0x03,

"��",0xC0,0x03,0x3E,0x00,0x20,0x00,0xFF,0x07,0x22,0x02,0xFE,0x03,0x22,0x02,0xFE,0x03,
0x20,0x00,0xFE,0x03,0x20,0x00,0xFF,0x07,

"��",0x04,0x01,0x88,0x00,0xFE,0x03,0x02,0x02,0x02,0x02,0x02,0x02,0xFE,0x03,0x00,0x00,
0x20,0x02,0x4A,0x04,0x0A,0x05,0xF1,0x01,

"��",0x08,0x00,0x08,0x03,0xC8,0x00,0x38,0x00,0x08,0x00,0xFF,0x07,0x48,0x00,0x48,0x00,
0x88,0x00,0x28,0x01,0x18,0x06,0x08,0x00,

"��",0x00,0x00,0xEF,0x07,0x49,0x04,0x49,0x04,0x2F,0x03,0x19,0x00,0xE9,0x07,0x29,0x04,
0xEF,0x07,0x00,0x00,0x4A,0x02,0x91,0x04,

"Ƭ",0x80,0x00,0x84,0x00,0x84,0x00,0x84,0x00,0xFC,0x07,0x04,0x00,0x04,0x00,0xFC,0x01,
0x04,0x01,0x02,0x01,0x02,0x01,0x01,0x01,

"��",0x04,0x01,0x24,0x01,0x62,0x05,0xA9,0x03,0x27,0x01,0xE4,0x07,0x22,0x01,0xAF,0x03,
0x60,0x05,0x2C,0x01,0x23,0x01,0xE0,0x07,

"��",0x84,0x00,0xE4,0x03,0x82,0x00,0xE9,0x07,0x07,0x04,0x44,0x01,0xA2,0x01,0x4F,0x01,
0xF0,0x07,0x8C,0x00,0x43,0x02,0x30,0x04,

"��",0x00,0x00,0xFE,0x03,0x20,0x00,0x20,0x00,0x20,0x00,0x24,0x00,0xE4,0x03,0x24,0x00,
0x24,0x00,0x24,0x00,0x24,0x00,0xFF,0x07,

"��",0x10,0x00,0x10,0x00,0xFF,0x07,0x08,0x00,0x88,0x00,0x84,0x00,0x86,0x00,0xE5,0x03,
0x84,0x00,0x84,0x00,0x84,0x00,0xF4,0x07,

"��",0x44,0x01,0x44,0x02,0x44,0x00,0xDF,0x07,0x64,0x00,0x44,0x02,0x5C,0x02,0x47,0x01,
0x84,0x04,0x44,0x05,0x24,0x06,0x16,0x04,

"��",0x42,0x00,0xDE,0x07,0x29,0x01,0x20,0x00,0xFE,0x03,0x20,0x00,0xFF,0x07,0x80,0x00,
0xFF,0x07,0x84,0x00,0x88,0x00,0xE0,0x00,

"��",0x08,0x04,0x10,0x04,0x7E,0x05,0x08,0x05,0x44,0x05,0x3E,0x05,0x10,0x05,0x48,0x05,
0x26,0x05,0x10,0x04,0x28,0x04,0x47,0x06,

"��",0x84,0x00,0x94,0x04,0xA4,0x02,0x8F,0x00,0xF4,0x07,0x06,0x04,0x0E,0x04,0xE5,0x07,
0x04,0x04,0x04,0x04,0xF4,0x07,0x04,0x04,

"��",0x04,0x06,0xF4,0x01,0x52,0x04,0xA9,0x02,0xF7,0x07,0x84,0x00,0xF2,0x07,0x47,0x00,
0xE0,0x03,0x54,0x02,0x8B,0x01,0x70,0x06,

"��",0x00,0x00,0xFE,0x03,0x00,0x02,0x00,0x02,0x02,0x02,0x02,0x02,0xFE,0x03,0x02,0x00,
0x02,0x00,0x02,0x04,0x02,0x04,0xFC,0x07,

"��",0x08,0x04,0xAA,0x03,0x9D,0x00,0x89,0x00,0xBF,0x07,0x89,0x02,0x9D,0x02,0xAB,0x02,
0x89,0x02,0x89,0x02,0xBF,0x02,0x40,0x02,

"��",0x44,0x00,0x84,0x00,0xFA,0x07,0x02,0x00,0xF3,0x03,0x02,0x00,0xF2,0x03,0x02,0x00,
0xF2,0x03,0x12,0x02,0xF2,0x03,0x12,0x02,

"��",0xFC,0x01,0x04,0x01,0x04,0x01,0xFC,0x01,0x00,0x00,0xFF,0x07,0x08,0x00,0x04,0x00,
0xFC,0x03,0x00,0x02,0x00,0x02,0x80,0x01,

"��",0x9E,0x07,0x10,0x04,0x10,0x04,0x9E,0x07,0x82,0x00,0x9E,0x07,0x51,0x04,0x92,0x04,
0x14,0x05,0x92,0x04,0x51,0x04,0x0C,0x03,

"��",0x24,0x00,0xFC,0x03,0x22,0x00,0xFF,0x07,0x04,0x01,0xFC,0x01,0x50,0x00,0xDF,0x07,
0x50,0x00,0xDF,0x07,0x50,0x00,0x50,0x00,

"��",0x02,0x03,0xE4,0x00,0x20,0x00,0x20,0x00,0xE7,0x07,0x24,0x01,0x24,0x01,0x24,0x01,
0x24,0x01,0x14,0x01,0x0A,0x00,0xF1,0x07,

"��",0x84,0x00,0x04,0x01,0xE4,0x07,0x2F,0x04,0x84,0x00,0x84,0x00,0xEC,0x07,0x47,0x02,
0x44,0x02,0x84,0x01,0x44,0x02,0x36,0x04,

"��",0x82,0x00,0x84,0x00,0x8F,0x00,0x88,0x00,0x84,0x01,0x94,0x02,0x8E,0x04,0x95,0x04,
0x84,0x00,0x84,0x00,0x84,0x00,0x84,0x00,

"��",0x10,0x00,0x20,0x00,0xFF,0x07,0x08,0x00,0x08,0x00,0xF8,0x03,0x08,0x02,0x08,0x02,
0x04,0x02,0x04,0x02,0x02,0x02,0xC1,0x01,

"��",0x10,0x00,0x08,0x00,0xFF,0x07,0x01,0x04,0x01,0x04,0xF9,0x04,0x89,0x04,0x89,0x04,
0x89,0x04,0xF9,0x04,0x01,0x04,0x01,0x07,

"һ",0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0x07,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,

"��",0xF4,0x07,0x82,0x00,0x41,0x00,0xF0,0x07,0x14,0x04,0x92,0x04,0x91,0x04,0x90,0x04,
0x94,0x04,0x42,0x01,0x21,0x02,0x18,0x04,

"��",0x20,0x00,0x20,0x00,0x20,0x00,0x20,0x00,0xFF,0x07,0x20,0x00,0x50,0x00,0x50,0x00,
0x88,0x00,0x88,0x00,0x04,0x01,0x03,0x06,

"��",0xFE,0x03,0x20,0x00,0x20,0x00,0x20,0x00,0x20,0x00,0xFF,0x07,0x20,0x00,0x20,0x00,
0x20,0x00,0x20,0x00,0x20,0x00,0x38,0x00,

"��",0x04,0x01,0x84,0x00,0xE4,0x07,0x2F,0x04,0x24,0x04,0x24,0x04,0xEC,0x07,0x27,0x04,
0x24,0x04,0x24,0x04,0xE4,0x07,0x26,0x04,

"��",0x10,0x06,0xCE,0x01,0x42,0x00,0x42,0x00,0xDE,0x07,0x52,0x02,0x52,0x02,0x5E,0x02,
0x52,0x02,0x42,0x02,0x22,0x02,0x11,0x02,

"��",0x10,0x00,0x10,0x00,0xFF,0x07,0x08,0x00,0xFC,0x03,0x06,0x02,0xFD,0x03,0x04,0x02,
0xFC,0x03,0x04,0x02,0x04,0x02,0x04,0x03,

"��",0xF4,0x07,0x24,0x02,0xE4,0x03,0x2F,0x02,0xE4,0x03,0x24,0x06,0xFC,0x03,0x07,0x02,
0x74,0x07,0x54,0x05,0x24,0x02,0x56,0x05,

"��",0x84,0x00,0x84,0x00,0xF2,0x07,0x89,0x00,0x87,0x00,0xE4,0x03,0x02,0x00,0xEF,0x03,
0x20,0x02,0x2C,0x02,0xE3,0x03,0x20,0x02,

"��",0x20,0x00,0xFF,0x07,0x20,0x00,0xFE,0x03,0x22,0x02,0x22,0x02,0xFE,0x03,0x70,0x00,
0xA8,0x00,0x24,0x01,0x23,0x06,0x20,0x00,

"ֹ",0x40,0x00,0x40,0x00,0x40,0x00,0x44,0x00,0xC4,0x03,0x44,0x00,0x44,0x00,0x44,0x00,
0x44,0x00,0x44,0x00,0x44,0x00,0xFF,0x07,

"��",0x10,0x00,0x10,0x00,0xFF,0x07,0x10,0x00,0x08,0x00,0x08,0x00,0xF8,0x03,0x44,0x00,
0x44,0x00,0x42,0x00,0x41,0x00,0xFC,0x07,

"��",0x20,0x00,0x20,0x00,0xFF,0x07,0x10,0x00,0x10,0x00,0x08,0x00,0xF8,0x03,0x0C,0x02,
0x0A,0x02,0x09,0x02,0xF8,0x03,0x08,0x02,

"��",0x48,0x00,0xC7,0x03,0x24,0x02,0x54,0x01,0x8F,0x00,0x44,0x01,0xA6,0x07,0x4E,0x04,
0xA5,0x02,0x04,0x01,0xC4,0x00,0x34,0x00,

"��",0x00,0x04,0x7F,0x04,0x04,0x05,0x22,0x05,0x7F,0x05,0x48,0x05,0x08,0x05,0x7F,0x05,
0x08,0x05,0x08,0x04,0x78,0x04,0x07,0x06,

"��",0x82,0x00,0xC6,0x03,0x99,0x02,0xF0,0x07,0x97,0x02,0xCA,0x03,0x9A,0x00,0xF7,0x03,
0x92,0x00,0xFA,0x07,0x96,0x00,0xEA,0x07,

"  ",
	  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,

"AB",0x00,0x00,0x00,0x00,0xC4,0x03,0x84,0x04,0x8C,0x04,0x8A,0x03,0x8A,0x04,0x9E,0x04,
0x92,0x04,0xF3,0x03,0x00,0x00,0x00,0x00,

"A ",0x00,0x00,0x00,0x00,0x04,0x00,0x04,0x00,0x0C,0x00,0x0A,0x00,0x0A,0x00,0x1E,0x00,
0x12,0x00,0x33,0x00,0x00,0x00,0x00,0x00,

"B ",0x00,0x00,0x00,0x00,0x0F,0x00,0x12,0x00,0x12,0x00,0x0E,0x00,0x12,0x00,0x12,0x00,
0x12,0x00,0x0F,0x00,0x00,0x00,0x00,0x00,
}; 


// ������ģ��                                                          //
//// Һ����ģ����: Font:windows font 18  ����24, ����ȡģ�Ҹ�λ,��������:�����Ҵ��ϵ���               //
// ��ģ3��ǿ������: ���峣��24�ţ���ɫ����Һ����ģ������ȡģ���ֽڵ���; ����32X33
const unsigned char LOGO720YUN[] = {
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0x03,0xF8,0x3F,0xE0,0x07,
	0xC0,0x03,0xC0,0xFF,0xFF,0x03,0xF8,0x3F,0x10,0x1C,0x60,0x04,0x00,0x00,0x00,0x00,
	0x08,0x10,0x08,0x18,0x30,0x0C,0x00,0x00,0x00,0x00,0x0C,0x08,0x04,0x30,0x18,0x18,
	0x00,0x00,0x00,0x00,0x04,0x08,0x04,0x30,0x18,0x18,0x00,0x00,0x00,0x00,0x04,0x04,
	0x0C,0x30,0x18,0x10,0x00,0x00,0x00,0x00,0x00,0x04,0x0C,0x30,0x0C,0x30,0x00,0x00,
	0x00,0x08,0x00,0x02,0x00,0x30,0x0C,0x30,0x00,0x00,0x00,0x1C,0x00,0x02,0x00,0x18,
	0x0C,0x30,0xFC,0xFF,0xFF,0x3F,0x00,0x02,0x00,0x18,0x0C,0x30,0x00,0xE0,0x00,0x00,
	0x00,0x01,0x00,0x0C,0x0C,0x30,0x00,0xE0,0x00,0x00,0x00,0x01,0x00,0x06,0x0C,0x30,
	0x00,0x60,0x00,0x00,0x80,0x00,0x00,0x03,0x0C,0x30,0x00,0x30,0x00,0x00,0x80,0x00,
	0x80,0x01,0x0C,0x30,0x00,0x30,0x08,0x00,0x80,0x00,0xC0,0x00,0x0C,0x30,0x00,0x18,
	0x10,0x00,0x80,0x00,0x40,0x00,0x0C,0x30,0x00,0x0C,0x20,0x00,0xC0,0x00,0x20,0x20,
	0x18,0x10,0x00,0x0C,0x60,0x00,0xC0,0x00,0x10,0x20,0x18,0x18,0x00,0x06,0xC0,0x00,
	0xC0,0x00,0x08,0x20,0x18,0x18,0x00,0x03,0x80,0x01,0xC0,0x00,0x04,0x30,0x30,0x0C,
	0x80,0x01,0x80,0x03,0xC0,0x00,0xFC,0x1F,0x60,0x04,0xC0,0x00,0xFF,0x07,0xC0,0x00,
	0xFC,0x1F,0xC0,0x03,0xE0,0xFF,0x03,0x07,0x00,0x00,0x00,0x00,0x00,0x00,0xC0,0x07,
	0x00,0x06,0x00,0x00,0x00,0x00,0x00,0x00,0x40,0x00,0x00,0x02,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
};

// ������ģ��                                                          //
//// Һ����ģ����: Font:windows font 18  ����24, ����ȡģ�Ҹ�λ,��������:�����Ҵ��ϵ���               //
// ��ģ3��ǿ������: ΢���źڳ���12�ţ���ɫ����Һ����ģ������ȡģ���ֽڵ���; ����16X21
const unsigned char Biaoyu[] = {
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0xC0,0x00,0x00,0x00,0x08,0x00,0x20,0x04,0x0C,0x06,0x10,0x08,0xE0,0x01,0xFC,0x3F,
	0x08,0x00,0x20,0x04,0xFC,0x7E,0x30,0x08,0x30,0x03,0x04,0x20,0x04,0x7E,0x20,0x44,
	0x12,0x09,0x20,0x04,0x10,0x06,0xFC,0x3F,0x7F,0x42,0x24,0x24,0xA5,0x08,0xFC,0x3F,
	0x0C,0x0C,0x04,0x20,0x44,0x42,0xE4,0x17,0xEC,0x3F,0x84,0x20,0x06,0x38,0xFC,0x3F,
	0x44,0x42,0x24,0x0C,0x08,0x20,0xFC,0x3F,0xFB,0x6F,0x80,0x01,0x42,0x42,0x24,0x04,
	0xF2,0x27,0x84,0x20,0x80,0x00,0xFE,0x7F,0x22,0x42,0x24,0x04,0x12,0x24,0x84,0x20,
	0x80,0x00,0x00,0x00,0x22,0x42,0x24,0x04,0x12,0x24,0xFC,0x3F,0xFC,0x1F,0xFC,0x3F,
	0x24,0x42,0x24,0x44,0xF2,0x27,0x84,0x20,0x80,0x00,0x04,0x20,0x18,0x42,0x24,0x44,
	0x12,0x24,0x80,0x00,0x80,0x00,0xFC,0x3F,0x38,0x42,0xA4,0x45,0x12,0x24,0xFF,0xFF,
	0x80,0x00,0x30,0x0D,0x6C,0x7E,0x7C,0x44,0xF2,0x27,0x80,0x00,0xFE,0x3F,0x1C,0x79,
	0xC6,0x42,0x06,0x3C,0x12,0x20,0x80,0x00,0x00,0x00,0xE6,0xC1,0x03,0x42,0x00,0x00,
	0x02,0x1C,0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,

};
//��ͷ���ͼ�ε�������
//const unsigned char arrows[]={0x80,0x80,0x80,0xA0,0xE0,0xC0,0x80,0x00,0x01,0x01,0x01,0x05,0x07,0x03,0x01,0x00};/* type1",0*/
//const unsigned char arrows[]={0xC0,0xC0,0xC8,0xD8,0xF8,0xF0,0xC0,0x00,0x01,0x01,0x09,0x0D,0x0F,0x07,0x01,0x00};//type2
//const unsigned char arrows[]={0xE0,0xE0,0xE0,0xE0,0xE0,0xC0,0xC0,0x80,0x03,0x03,0x03,0x03,0x03,0x01,0x01,0x00}; //type3
//const unsigned char arrows[]={0xE0,0xE0,0xE0,0xE0,0xC0,0xC0,0x80,0x80,0x07,0x07,0x07,0x07,0x03,0x03,0x01,0x01}; //type4
const unsigned char arrows[] = {0x00,0x80,0xC0,0xE0,0xE0,0xC0,0x80,0x00,0x01,0x03,0x07,0x0F,0x0F,0x07,0x03,0x01};/*"δ�����ļ�",0*/
/*
const unsigned char battery[] = {
	0x3C,0x3C,0x3C,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
	0x3C,0x3C,0x3C,0xFF,0x81,0x81,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
	0x3C,0x3C,0x3C,0xFF,0x81,0x81,0x81,0x81,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
	0x3C,0x3C,0x3C,0xFF,0x81,0x81,0x81,0x81,0x81,0x81,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
	0x3C,0x3C,0x3C,0xFF,0x81,0x81,0x81,0x81,0x81,0x81,0x81,0x81,0xFF,0xFF,0xFF,0xFF,
	0x3C,0x3C,0x3C,0xFF,0x81,0x81,0x81,0x81,0x81,0x81,0x81,0x81,0x81,0x81,0xFF,0xFF,
};*/
const unsigned char battery[] = {
	/*------------------------------------------------------------------------------
	;  Դ�ļ� / ���� : C:\Users\Administrator\Desktop\111.bmp��ģ
	;  �����ߣ����أ�: 32��20
	------------------------------------------------------------------------------*/
		0xFF,0xFF,0xFF,
		0xFF,0xFF,0xFF,
		0x00,0x00,0xF0,
		0x00,0x00,0xF0,
		0x00,0x00,0x00,
		0x00,0x00,0x00,
		0x00,0x00,0x00,
		0x00,0x00,0x00,
		0x00,0x00,0x00,
		0x00,0x00,0x00,
		0x00,0x00,0xF0,
		0x00,0x00,0xF0,
		0xFF,0xFF,0xFF,
		0xFF,0xFF,0xFF,

};/*"�����",0*/

// 24 * 24
const unsigned char chargingB[] = {
	0x00,0x00,0x00,
		0x00,0x00,0x00,
		0x00,0x00,0x00,
		0x00,0x00,0x00,
	0x00,0xC0,0x01,
		0x00,0xE0,0x03,
		0x00,0xF0,0x03,
		0x00,0xF8,0xFF,
		0x00,0xFC,0xFF,
		0x00,0xFE,0xFF,
		0xFF,0xFF,0x03,
		0xFF,0xFF,0x03,
		0xFF,0xFF,0x03,
		0xFF,0xFF,0x03,
		0x00,0xFE,0xFF,
		0x00,0xFC,0xFF,
		0x00,0xF8,0xFF,
		0x00,0xF0,0x03,
		0x00,0xE0,0x03,
		0x00,0xC0,0x01,
		0x00,0x00,0x00,
		0x00,0x00,0x00,
		0x00,0x00,0x00,
		0x00,0x00,0x00,
};/*"�����",0*/


const unsigned char Angle[]={0x00,0x00,0x0C,0x12,0x0C,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};/*"δ�����ļ�",0*/


// ------------------  ASCII��ģ�����ݱ� ------------------------ //
// �����0x20~0x7e												  //
// ������ģȡ��£���ɫ����Һ����ģ������ȡģ���ֽڵ���
// H=12; W=6
// -------------------------------------------------------------- //
const unsigned char F8X16[]=	  
{
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,  // - -
		
	
		0x00,0x00,0x04,0x04,0x04,0x04,0x04,0x00,0x00,0x04,0x00,0x00,  // -!-
	
		0x14,0x14,0x0A,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,  // -"-
	
		0x00,0x00,0x0A,0x0A,0x1F,0x0A,0x0A,0x1F,0x0A,0x0A,0x00,0x00,  // -#-
	
		0x00,0x04,0x0E,0x15,0x05,0x06,0x0C,0x14,0x15,0x0E,0x04,0x00,  // -$-
	
		0x00,0x00,0x12,0x15,0x0D,0x15,0x2E,0x2C,0x2A,0x12,0x00,0x00,  // -%-
	
		0x00,0x00,0x04,0x0A,0x0A,0x36,0x15,0x15,0x29,0x16,0x00,0x00,  // -&-
	
		0x02,0x02,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,  // -'-
	
		0x10,0x08,0x08,0x04,0x04,0x04,0x04,0x04,0x08,0x08,0x10,0x00,  // -(-
	
		0x02,0x04,0x04,0x08,0x08,0x08,0x08,0x08,0x04,0x04,0x02,0x00,  // -)-
	
		0x00,0x00,0x00,0x04,0x15,0x0E,0x0E,0x15,0x04,0x00,0x00,0x00,  // -*-
	
		0x00,0x00,0x00,0x08,0x08,0x3E,0x08,0x08,0x00,0x00,0x00,0x00,  // -+-
	
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x02,0x02,0x01,0x00,  // -,-
	
		0x00,0x00,0x00,0x00,0x00,0x3F,0x00,0x00,0x00,0x00,0x00,0x00,  // ---
	
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x02,0x00,0x00,  // -.-
	
		0x00,0x20,0x10,0x10,0x08,0x08,0x04,0x04,0x02,0x02,0x01,0x00,  // -/-
	
		0x00,0x00,0x0E,0x11,0x11,0x11,0x11,0x11,0x11,0x0E,0x00,0x00,  // -0-
	
		0x00,0x00,0x04,0x06,0x04,0x04,0x04,0x04,0x04,0x0E,0x00,0x00,  // -1-
	
		0x00,0x00,0x0E,0x11,0x11,0x08,0x04,0x02,0x01,0x1F,0x00,0x00,  // -2-
	
		0x00,0x00,0x0E,0x11,0x10,0x0C,0x10,0x10,0x11,0x0E,0x00,0x00,  // -3-
	
		0x00,0x00,0x08,0x0C,0x0C,0x0A,0x09,0x1F,0x08,0x1C,0x00,0x00,  // -4-
	
		0x00,0x00,0x1F,0x01,0x01,0x0F,0x11,0x10,0x11,0x0E,0x00,0x00,  // -5-
	
		0x00,0x00,0x0C,0x12,0x01,0x0D,0x13,0x11,0x11,0x0E,0x00,0x00,  // -6-
	
		0x00,0x00,0x1E,0x10,0x08,0x08,0x04,0x04,0x04,0x04,0x00,0x00,  // -7-
	
		0x00,0x00,0x0E,0x11,0x11,0x0E,0x11,0x11,0x11,0x0E,0x00,0x00,  // -8-
	
		0x00,0x00,0x0E,0x11,0x11,0x19,0x16,0x10,0x09,0x06,0x00,0x00,  // -9-
	
		0x00,0x00,0x00,0x00,0x04,0x00,0x00,0x00,0x00,0x04,0x00,0x00,  // -:-
	
		0x00,0x00,0x00,0x00,0x00,0x04,0x00,0x00,0x00,0x04,0x04,0x02,  // -;-
	
		0x00,0x00,0x10,0x08,0x04,0x02,0x02,0x04,0x08,0x10,0x00,0x00,  // -<-
	
		0x00,0x00,0x00,0x00,0x3F,0x00,0x3F,0x00,0x00,0x00,0x00,0x00, // -=-
	
		0x00,0x00,0x02,0x04,0x08,0x10,0x10,0x08,0x04,0x02,0x00,0x00,  // ->-
	
		0x00,0x00,0x0E,0x11,0x11,0x08,0x04,0x04,0x00,0x04,0x00,0x00,  // -?-
	
		0x00,0x00,0x1C,0x22,0x29,0x2D,0x2D,0x1D,0x22,0x1C,0x00,0x00,  // -@-
	
		0x00,0x00,0x04,0x04,0x0C,0x0A,0x0A,0x1E,0x12,0x33,0x00,0x00,  // -A-
	
		0x00,0x00,0x0F,0x12,0x12,0x0E,0x12,0x12,0x12,0x0F,0x00,0x00,  // -B-
	
		0x00,0x00,0x1E,0x11,0x01,0x01,0x01,0x01,0x11,0x0E,0x00,0x00,  // -C-
	
		0x00,0x00,0x0F,0x12,0x12,0x12,0x12,0x12,0x12,0x0F,0x00,0x00,  // -D-
	
		0x00,0x00,0x1F,0x12,0x0A,0x0E,0x0A,0x02,0x12,0x1F,0x00,0x00,  // -E-
	
		0x00,0x00,0x1F,0x12,0x0A,0x0E,0x0A,0x02,0x02,0x07,0x00,0x00,  // -F-
	
		0x00,0x00,0x1C,0x12,0x01,0x01,0x39,0x11,0x12,0x0C,0x00,0x00,  // -G-
	
		0x00,0x00,0x33,0x12,0x12,0x1E,0x12,0x12,0x12,0x33,0x00,0x00,  // -H-
	
		0x00,0x00,0x1F,0x04,0x04,0x04,0x04,0x04,0x04,0x1F,0x00,0x00,  // -I-
	
		0x00,0x00,0x3E,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x09,0x07,  // -J-
	
		0x00,0x00,0x37,0x12,0x0A,0x06,0x0A,0x12,0x12,0x37,0x00,0x00,  // -K-
	
		0x00,0x00,0x07,0x02,0x02,0x02,0x02,0x02,0x22,0x3F,0x00,0x00,  // -L-
	
		0x00,0x00,0x3B,0x1B,0x1B,0x1B,0x15,0x15,0x15,0x35,0x00,0x00,  // -M-
	
		0x00,0x00,0x3B,0x12,0x16,0x16,0x1A,0x1A,0x12,0x17,0x00,0x00,  // -N-
	
		0x00,0x00,0x0E,0x11,0x11,0x11,0x11,0x11,0x11,0x0E,0x00,0x00,  // -O-
	
		0x00,0x00,0x0F,0x12,0x12,0x0E,0x02,0x02,0x02,0x07,0x00,0x00,  // -P-
	
		0x00,0x00,0x0E,0x11,0x11,0x11,0x11,0x17,0x19,0x0E,0x18,0x00,  // -Q-
	
		0x00,0x00,0x0F,0x12,0x12,0x0E,0x0A,0x12,0x12,0x37,0x00,0x00,  // -R-
	
		0x00,0x00,0x1E,0x11,0x01,0x06,0x08,0x10,0x11,0x0F,0x00,0x00,  // -S-
	
		0x00,0x00,0x1F,0x15,0x04,0x04,0x04,0x04,0x04,0x0E,0x00,0x00,  // -T-
	
		0x00,0x00,0x33,0x12,0x12,0x12,0x12,0x12,0x12,0x0C,0x00,0x00,  // -U-
	
		0x00,0x00,0x33,0x12,0x12,0x0A,0x0A,0x0C,0x04,0x04,0x00,0x00,  // -V-
	
		0x00,0x00,0x15,0x15,0x15,0x15,0x0E,0x0A,0x0A,0x0A,0x00,0x00,  // -W-
	
		0x00,0x00,0x1B,0x0A,0x0A,0x04,0x04,0x0A,0x0A,0x1B,0x00,0x00,  // -X-
	
		0x00,0x00,0x1B,0x0A,0x0A,0x0A,0x04,0x04,0x04,0x0E,0x00,0x00,  // -Y-
	
		0x00,0x00,0x1F,0x09,0x08,0x04,0x04,0x02,0x12,0x1F,0x00,0x00,  // -Z-
	
		0x1C,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x1C,0x00,  // -[-
	
		0x00,0x02,0x02,0x04,0x04,0x04,0x08,0x08,0x08,0x10,0x10,0x00,  // -\-
	
		0x0E,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x0E,0x00,  // -]-
	
		0x04,0x0A,0x11,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,  // -^-
	
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x3F,  // -_-
	
		0x02,0x04,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,  // -`-
	
		0x00,0x00,0x00,0x00,0x00,0x0C,0x12,0x1C,0x12,0x3C,0x00,0x00,  // -a-
	
		0x00,0x02,0x02,0x02,0x02,0x0E,0x12,0x12,0x12,0x0E,0x00,0x00,  // -b-
	
		0x00,0x00,0x00,0x00,0x00,0x0C,0x12,0x02,0x12,0x0C,0x00,0x00,  // -c-
	
		0x00,0x18,0x10,0x10,0x10,0x1C,0x12,0x12,0x12,0x3C,0x00,0x00,  // -d-
	
		0x00,0x00,0x00,0x00,0x00,0x0C,0x12,0x1E,0x02,0x1C,0x00,0x00,  // -e-
	
		0x00,0x18,0x24,0x04,0x04,0x1E,0x04,0x04,0x04,0x1E,0x00,0x00,  // -f-
	
		0x00,0x00,0x00,0x00,0x00,0x3C,0x12,0x0C,0x02,0x1C,0x22,0x1C,  // -g-
	
		0x00,0x03,0x02,0x02,0x02,0x0E,0x12,0x12,0x12,0x37,0x00,0x00,  // -h-
	
		0x00,0x04,0x04,0x00,0x00,0x06,0x04,0x04,0x04,0x0E,0x00,0x00,  // -i-
	
		0x00,0x08,0x08,0x00,0x00,0x0C,0x08,0x08,0x08,0x08,0x08,0x07,  // -j-
	
		0x00,0x03,0x02,0x02,0x02,0x1A,0x0A,0x06,0x0A,0x13,0x00,0x00,  // -k-
	
		0x00,0x07,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x1F,0x00,0x00,  // -l-
	
		0x00,0x00,0x00,0x00,0x00,0x0F,0x15,0x15,0x15,0x15,0x00,0x00,  // -m-
	
		0x00,0x00,0x00,0x00,0x00,0x0F,0x12,0x12,0x12,0x37,0x00,0x00,  // -n-
	
		0x00,0x00,0x00,0x00,0x00,0x0C,0x12,0x12,0x12,0x0C,0x00,0x00,  // -o-
	
		0x00,0x00,0x00,0x00,0x00,0x0F,0x12,0x12,0x12,0x0E,0x02,0x07,  // -p-
	
		0x00,0x00,0x00,0x00,0x00,0x1C,0x12,0x12,0x12,0x1C,0x10,0x38,  // -q-
	
		0x00,0x00,0x00,0x00,0x00,0x1B,0x06,0x02,0x02,0x07,0x00,0x00,  // -r-
	
		0x00,0x00,0x00,0x00,0x00,0x1E,0x02,0x0C,0x10,0x1E,0x00,0x00,  // -s-
	
		0x00,0x00,0x00,0x04,0x04,0x1E,0x04,0x04,0x04,0x1C,0x00,0x00,  // -t-
	
		0x00,0x00,0x00,0x00,0x00,0x1B,0x12,0x12,0x12,0x3C,0x00,0x00,  // -u-
	
		0x00,0x00,0x00,0x00,0x00,0x1B,0x0A,0x0A,0x04,0x04,0x00,0x00,  // -v-
	
		0x00,0x00,0x00,0x00,0x00,0x15,0x15,0x0E,0x0A,0x0A,0x00,0x00,  // -w-
	
		0x00,0x00,0x00,0x00,0x00,0x1B,0x0A,0x04,0x0A,0x1B,0x00,0x00,  // -x-
	
		0x00,0x00,0x00,0x00,0x00,0x33,0x12,0x12,0x0C,0x08,0x04,0x03,  // -y-
	
		0x00,0x00,0x00,0x00,0x00,0x1E,0x08,0x04,0x04,0x1E,0x00,0x00,  // -z-
	
		0x18,0x08,0x08,0x08,0x08,0x0C,0x08,0x08,0x08,0x08,0x18,0x00,  // -{-
	
		0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,  // -|-
	
		0x06,0x04,0x04,0x04,0x04,0x08,0x04,0x04,0x04,0x04,0x06,0x00,  // -}-
	
		0x16,0x09,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,  // -~-
	
//		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x20,0x00,0x20,0x00,0x70,0x00,0x50,0x00,  // --
//		0xD8,0x00,0x88,0x00,0x8C,0x01,0x04,0x01,0x06,0x03,0x02,0x02,0x03,0x06,0xFF,0x07,
//		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
};


#endif