//  �� �� ��   : v2.0
//  ��    ��   : HuangKai
//  ��������   : 2014-0101
//  ����޸�   : 
//  ��������   : OLED 4�ӿ���ʾ����(51ϵ��)
//              ˵��: 
//              ----------------------------------------------------------------
//              GND    ��Դ��
//              VCC  ��5V��3.3v��Դ
//              CLK   ��PA5��SCL��
//              MOSI   ��PA7��SDA��
//              RES  ��PB0
//              DC   ��PB1 
//							BLK  ��A4 ��������
//							MISO ���Բ���
//              ----------------------------------------------------------------
//******************************************************************************/
#ifndef __OLED_H
#define __OLED_H			  	 
//#include "sys.h"
#include "stdlib.h"	   
#include "stm32f030x8.h"
#define BATTER_FZ (float)(0.8)

#define LCD_W 128
#define LCD_H 160
#define ZHI_H 12 // �ָ߶�
#define ZHI_W 12
#define CHAR_W 6
#define SCREEN_MIGRATION	(2)//(4)// 2:�������ƫ�ƣ�4: �ֿ�12*10=120; ����128 -120=8/2
#define ROWLEDGE	5 //�и�
#define TITLE (16+ROWLEDGE) //������
#define	u8 unsigned char
#define	u16 unsigned int
#define	u32 unsigned long
#define TIMEE	1
//OLEDģʽ����
//0:4�ߴ���ģʽ
//1:����8080ģʽ
#define OLED_MODE 0
#define SIZE 16
#define XLevelL		0x00
#define XLevelH		0x10
#define Max_Column	128
#define Max_Row		64
#define X_WIDTH 	128
#define Y_WIDTH 	64	  

/// ��������ص����궨��
#define BATERRY_MODULE		23 // ��ص���ģ�ο��
#define X_START		(LCD_W-30)
#define Y_START		(2)
#define X_END		(X_START+BATERRY_MODULE)
#define Y_END		(12)
#define WORD_SPACE	1
#define BATERRY_SIZE	18 // ��ص�������ʾ���

////////����˵�ص����궨��
#define M_X_START		(27)
#define M_X_END		(M_X_START+BATERRY_MODULE)


//-----------------OLED�˿ڶ���----------------  					   

#define OLED_CMD  0	//д����
#define OLED_DATA 1	//д����

extern  u16 BACK_COLOR, POINT_COLOR;   //����ɫ������ɫ
extern uint8_t *z_buffer;

typedef enum
{
	CONTROLLER = 1,
	MOTOR,
	
}DEVICE_P;


typedef struct
{
	uint8_t *chanese[3];
	//uint8_t *english;
	//uint8_t *korean_h;
}DIS_PARAM;


void LCD_Clear(u16 Color);
void Address_set(unsigned int x1,unsigned int y1,unsigned int x2,unsigned int y2);
void LCD_WR_DATA8(char da); //��������-8λ����
void LCD_WR_DATA(int da);
void LCD_WR_REG(char da);

void LCD_DrawPoint(u16 x,u16 y);//����
void LCD_DrawPoint_big(u16 x,u16 y);//��һ�����
u16  LCD_ReadPoint(u16 x,u16 y); //����
void Draw_Circle(u16 x0,u16 y0,u8 r);
void LCD_DrawLine(u16 x1, u16 y1, u16 x2, u16 y2);
void LCD_DrawRectangle(u16 x1, u16 y1, u16 x2, u16 y2);		   
void LCD_Fill(u16 xsta,u16 ysta,u16 xend,u16 yend,u16 color);
void LCD_ShowChar(u16 x,u16 y,u8 num,u8 mode);//��ʾһ���ַ�
void LCD_ShowNum(u16 x,u16 y,u32 num,u8 len);//��ʾ����
void LCD_Show2Num(u16 x,u16 y,u16 num,u8 len);//��ʾ2������
void LCD_ShowString(u16 x,u16 y,const u8 *p);		 //��ʾһ���ַ���,16����
 
void showhanzi(unsigned int x,unsigned int y,unsigned char index);
void test_init(void);

void Oled_EnlPrint(uint8_t x, uint8_t y, uint8_t *en,uint8_t *ch, uint8_t language);
void oled_print_screen(void);
uint8_t Check_String(uint8_t *Estrp, uint8_t LNG/*, uint8_t *Cstrp, uint8_t *Kstrp*/);
void oled_print_page(uint8_t cur);
void TFT_Line_Dis(uint8_t *srtp, uint8_t *csrtp, uint8_t row);
void The_motor_battery_dis_init(void);
void battery_data_display(float  data, uint8_t type, DEVICE_P pct);
void battery_display(uint8_t id, DEVICE_P type);
void motor_battery_dis(float bat);
void page_display(uint8_t rowd);
void bornd_code_dis(uint8_t row, uint8_t codeID);
void LCD_shi_fen_miao_display(uint8_t x,uint8_t y,uint8_t *ch);

// ADC.c
void battery_frame_display(DEVICE_P type);

//������ɫ
#define WHITE         	 0xFFFF
#define BLACK         	 0x0000	  
#define BLUE         	 0x001F  
#define BRED             0XF81F
#define GRED 			 0XFFE0
#define GBLUE			 0X07FF
#define RED           	 0xF800
#define MAGENTA       	 0xF81F
#define GREEN         	 0x07E0
#define CYAN          	 0x7FFF
#define YELLOW        	 0xFFE0
#define BROWN 			 0XBC40 //��ɫ
#define BRRED 			 0XFC07 //�غ�ɫ
#define GRAY  			 0X8430 //��ɫ
//GUI��ɫ

#define DARKBLUE      	 0X01CF	//����ɫ
#define LIGHTBLUE      	 0X7D7C	//ǳ��ɫ  
#define GRAYBLUE       	 0X5458 //����ɫ
//������ɫΪPANEL����ɫ 
 
#define LIGHTGREEN     	 0X01ff //ǳ��ɫ
#define LGRAY 			 0XC618 //ǳ��ɫ(PANNEL),���屳��ɫ

#define LGRAYBLUE        0XA651 //ǳ����ɫ(�м����ɫ)
#define LBBLUE           0X2B12 //ǳ����ɫ(ѡ����Ŀ�ķ�ɫ)

					  		 
#endif  
	 
	 



