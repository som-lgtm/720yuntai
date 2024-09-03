

#include "oled.h"
#include "stdlib.h"
#include "stdio.h"
#include "matrix.h"  	 
#include "common.h"
#include "gpio.h"
#include "keyscan.h"
#include "orbital.h"

u16 BACK_COLOR=0, POINT_COLOR=0;   //����ɫ������ɫ
uint8_t *z_buffer = 0;
uint8_t Battery_percentage=0;

uint32_t battery_colour = 0;
uint32_t m_battery_colour = 0;

void The_page_processing(uint8_t row, uint8_t *spt)
{
		uint8_t x_size = 0;
	//	uint8_t *srtp = 0;
		uint8_t yend = 0, ysta = 0;
		uint8_t xend = 0, xsta = 0;
		
		/*if(glob_para.language_sel == FRENCH || glob_para.language_sel == SPANISH || glob_para.language_sel == GERMAN)
		{
			x_size = Check_String(spt, ENGLISH);
		}
		else*/
		{
			x_size = Check_String(spt, wifi_id.language_sel);
		}
		xend = (LCD_W - x_size) / 2;
		xsta = xend + x_size ;//+ SCREEN_MIGRATION;
		ysta = row * (ZHI_H+ ROWLEDGE) + TITLE;
		yend = ysta+ZHI_H;
		
		LCD_Fill(0, ysta, xend, yend, BLACK);
		LCD_Fill(xsta, ysta, LCD_W, yend, BLACK);

		if(wifi_id.language_sel == FRENCH || wifi_id.language_sel == SPANISH || wifi_id.language_sel == GERMAN)
		{
			Oled_EnlPrint(xend, row, spt,spt, ENGLISH);
		}
		else
		{
			Oled_EnlPrint(xend, row, spt,spt, wifi_id.language_sel);
		}
}

void chinese_page_dis(uint8_t rows)
{	
	switch(page_id)
	{		
		case LOCUS_PAGE:
		{
			if(rows == 0){
				The_page_processing(rows, "                    ");}
			else if(rows == 1){
				The_page_processing(rows,"                    ");}
			else if(rows == 2){
				The_page_processing(rows,	"�����ѶϿ�  ���Ժ�");}
			else if(rows == 3){
				The_page_processing(rows,"�ź����뿿����̨");}
			else if(rows == 4){
				The_page_processing(rows,	"                    ");}
			else if(rows == 5){
				The_page_processing(rows,	"                    ");}
			else if(rows == 6){
				The_page_processing(rows,	"                    ");}
			else if(rows == 7){
				The_page_processing(rows,"                    ");}
			break;
		}
		case SET_A_POINT:
		{
			if(rows == 0){
				The_page_processing(rows, "                    ");}
			else if(rows == 1){
				The_page_processing(rows,"                    ");}
			else if(rows == 2){
				The_page_processing(rows,	"                    ");}
			else if(rows == 3){
				The_page_processing(rows,"A ������");}
			else if(rows == 4){
				The_page_processing(rows,	"                    ");}
			else if(rows == 5){
				The_page_processing(rows,	"                    ");}
			else if(rows == 6){
				The_page_processing(rows,	"                    ");}
			else if(rows == 7){
				The_page_processing(rows,"                    ");}
			break;
		}
		case SET_B_POINT:
		{
			if(rows == 0){
				The_page_processing(rows,"                    ");}
			else if(rows == 1){
				The_page_processing(rows,"                    ");}
			else if(rows == 2){
				The_page_processing(rows,"B ������");}
			else if(rows == 3){
				The_page_processing(rows,"                    ");}
			else if(rows == 4){
				The_page_processing(rows,	"����Ƕ�        ");}
			else if(rows == 5){
				The_page_processing(rows,	"                    ");}
			else if(rows == 6){
				The_page_processing(rows,	"                    ");}
			else if(rows == 7){
				The_page_processing(rows,"                    ");}
			break;
		}		
		case ORIGIN_SET_MOVE:
		{
			if(rows == 0){
				The_page_processing(rows,"                    ");}
			else if(rows == 1){
				The_page_processing(rows,"                    ");}
			else if(rows == 2){
				The_page_processing(rows,	"                    ");}
			else if(rows == 3){
				The_page_processing(rows,"ԭ������");}
			else if(rows == 4){
				The_page_processing(rows,	"                    ");}
			else if(rows == 5){
				The_page_processing(rows,	"                    ");}
			else if(rows == 6){
				The_page_processing(rows,	"                    ");}
			else if(rows == 7){
				The_page_processing(rows,"                    ");}
				
			break;
		}
		case MAIN_ID:
		{
			if(rows == 0){
				The_page_processing(rows, "                    ");}
			else if(rows == 1){
				The_page_processing(rows,	"  ԭ������  ");}
			else if(rows == 2){
				The_page_processing(rows,	"  ȫ��ģʽ  ");}
			else if(rows == 3){
				The_page_processing(rows,	"  ��Ӱģʽ  ");}
			else if(rows == 4){
				The_page_processing(rows,"  ��Ƶģʽ  ");}
			else if(rows == 5){
				The_page_processing(rows,	"  ��ʱģʽ  ");}
			else if(rows == 6){
				The_page_processing(rows,	"  �ֶ�ģʽ  ");}
			else if(rows == 7){
				The_page_processing(rows,"    ����    ");}
			break;
		}
		case GROUP_PHOTO:
		{
			if(rows == 0){
				The_page_processing(rows,"�������          ");}
			else if(rows == 1){
				The_page_processing(rows,	"��ͷ����          ");}
			else if(rows == 2){
				The_page_processing(rows,"�ص���            ");}
			else if(rows == 3){
				The_page_processing(rows,"����ʱ��          ");}
			else if(rows == 4){
				The_page_processing(rows,	"����              ");} // �Ӻ�
			else if(rows == 5){
				The_page_processing(rows,"�ֶ�����          ");}
			else if(rows == 6){
				The_page_processing(rows,	"  ����  ");}
			else if(rows == 7){
				The_page_processing(rows,"  AB������  ");}
			break;
		}	
		/*case GROUP_PHOTO_MOVE:
		{
			if(rows == 0){
				The_page_processing(rows,	"��Ӱģʽ");}
			else if(rows == 1){
				The_page_processing(rows,	"                    ");}
			else if(rows == 2){
				The_page_processing(rows,"��ɶ�            ");}
			else if(rows == 3){
				The_page_processing(rows,	"                    ");}
			else if(rows == 4){
				The_page_processing(rows,"��Ƭ              ");}
			else if(rows == 5){
				The_page_processing(rows,"��ͣ");}
			else if(rows == 6){
				The_page_processing(rows,"  ����  ");}
			else if(rows == 7){
				The_page_processing(rows,"                    ");}
			break;
		}	*/	
		case PREINSTALL_MODE:
		{
			if(rows == 0){
				The_page_processing(rows,"ȫ��ģʽ");}
			else if(rows == 1){
				The_page_processing(rows, "                    ");}
			else if(rows == 2){
				The_page_processing(rows, "                    ");}
			else if(rows == 3){
				The_page_processing(rows, "                    ");}
			else if(rows == 4){
				The_page_processing(rows, "                    ");}
			else if(rows == 5){
				The_page_processing(rows, "                    ");}
			else if(rows == 6){
				The_page_processing(rows, "                    ");}
			else if(rows == 7){
				The_page_processing(rows, "                    ");}
			break;
		}		
		case PREINSTALL_SET:
		{
			if(rows == 0){
				The_page_processing(rows,"ȫ��ģʽ����");}
			else if(rows == 1){
				The_page_processing(rows, "                    ");}
			else if(rows == 2){
				The_page_processing(rows, "                    ");}
			else if(rows == 3){
				The_page_processing(rows, "                    ");}
			else if(rows == 4){
				The_page_processing(rows, "                    ");}
			else if(rows == 5){
				The_page_processing(rows, "                    ");}
			else if(rows == 6){
				The_page_processing(rows, "                    ");}
			else if(rows == 7){
				The_page_processing(rows, " 	                  ");}
			break;
		}		
		case PREINSTALL_SET1:
		{
			if(rows == 0){
				The_page_processing(rows,"ȫ��ģʽ��������");}
			else if(rows == 1){
				The_page_processing(rows, "�Ƕ�              ");}
			else if(rows == 2){
				The_page_processing(rows, "���Ŵ���          ");}
			else if(rows == 3){
				The_page_processing(rows, "����              ");}
			else if(rows == 4){
				The_page_processing(rows, "�����ٶ�          ");}
			else if(rows == 5){
				The_page_processing(rows, "��ͣʱ��          ");}
			else if(rows == 6){
				The_page_processing(rows, "��ʱʱ��          ");}
			else if(rows == 7){
				The_page_processing(rows, "                  ");}
			break;
		}
		case PREINSTALL_MOVE:
		{
			if(rows == 0){
				The_page_processing(rows, "ȫ��ģʽ��������");}
			else if(rows == 1){
				The_page_processing(rows, "��ʱ����          ");}
			else if(rows == 2){
				The_page_processing(rows, "�ֶ�����          ");}
			else if(rows == 3){
				The_page_processing(rows, "  ����  ");}
			else if(rows == 4){
				The_page_processing(rows,	 "  ����  ");}
			else if(rows == 5){
				The_page_processing(rows, "                    ");} // ��ʾ��ǰ����
			else if(rows == 6){
				The_page_processing(rows, "��������          ");} // �붨ʱ���Ƹ���
			else if(rows == 7){
				The_page_processing(rows,	 "                    ");}
			break;
		}		
		case PREINSTALL_SESHOT:
		{
			if(rows == 0){
				The_page_processing(rows, "ȫ��ģʽ���Ľ���");}
			else if(rows == 1){
				The_page_processing(rows,	 "                    ");}
			else if(rows == 2){
				The_page_processing(rows, "�������ѡ��");}
			else if(rows == 3){
				The_page_processing(rows, "                    ");}
			else if(rows == 4){
				The_page_processing(rows,	 "                    ");}
			else if(rows == 5){
				The_page_processing(rows,	 "��һ��    ��һ��");}
			else if(rows == 6){
				The_page_processing(rows, "                    ");} // �붨ʱ���Ƹ���
			else if(rows == 7){
				The_page_processing(rows,	 "��  �˵���  �˳�");}
			break;
		}
		case VIDEO_MODE:
		{
			if(rows == 0){
				The_page_processing(rows,"��Ƶģʽ");}
			else if(rows == 1){
				The_page_processing(rows,"����              ");}
			else if(rows == 2){
				The_page_processing(rows,"ʱ��              ");}
			else if(rows == 3){
				The_page_processing(rows,	"ѭ��              ");}
			else if(rows == 4){
				The_page_processing(rows,"  ����  ");}
			else if(rows == 5){
				The_page_processing(rows,"  AB������  ");}
			else if(rows == 6){
				The_page_processing(rows,"                    ");}
			else if(rows == 7){
				The_page_processing(rows,"                    ");}
			break;
		}				
		case DELAY_SET:
		{
			if(rows == 0){
				The_page_processing(rows,	"��ʱģʽ");}
			else if(rows == 1){
				The_page_processing(rows,"��ͣʱ��          ");}
			else if(rows == 2){
				The_page_processing(rows,"����              ");}
			else if(rows == 3){
				The_page_processing(rows,"����              ");}
			else if(rows == 4){
				The_page_processing(rows,"����              ");}
			else if(rows == 5){
				The_page_processing(rows,"  ����  ");}
			else if(rows == 6){
				The_page_processing(rows,"  AB������  ");}
			else if(rows == 7){
				The_page_processing(rows,"                    ");}
			break;
		}		
		case DELAY_DIS:
		{
			if(rows == 0){
				The_page_processing(rows,	"��ʱģʽ");}
			else if(rows == 1){
				The_page_processing(rows,	"                    ");}
			else if(rows == 2){
				The_page_processing(rows,"��ɶ�            ");}
			else if(rows == 3){
				The_page_processing(rows,	"                    ");}
			else if(rows == 4){
				The_page_processing(rows,"��Ƭ              ");}
			else if(rows == 5){
				The_page_processing(rows,"��ͣ");}
			else if(rows == 6){
				The_page_processing(rows,"  ����  ");}
			else if(rows == 7){
				The_page_processing(rows, "                    ");}
			break;
		}
		case CONFIG_ID:
		{
			if(rows == 0){
				The_page_processing(rows,"                    ");	}
			else if(rows == 1){
				The_page_processing(rows,"����            ����");}
			else if(rows == 2){
				The_page_processing(rows,"��ʼ��              ");}
			else if(rows == 3){
				The_page_processing(rows,"����                ");}
			else if(rows == 4){
				The_page_processing(rows,"����                ");}
			else if(rows == 5){
				The_page_processing(rows,"�汾                ");}
			else if(rows == 6){
				The_page_processing(rows,	"                    ");}
			else if(rows == 7){
				The_page_processing(rows,	"                    ");}
			break;
		}
		case REVERSE_MODE:
		{
			if(rows == 0){
				The_page_processing(rows,"                    ");}
			else if(rows == 1){
				The_page_processing(rows,"                    ");}
			else if(rows == 2){
				The_page_processing(rows,	"�Ƿ�ȷ��Ҫ����A B ��");}
			else if(rows == 3){
				The_page_processing(rows,	"  ��  ");}
			else if(rows == 4){
				The_page_processing(rows,"  ��  ");}
			else if(rows == 5){
				The_page_processing(rows,	"                    ");}
			else if(rows == 6){
				The_page_processing(rows,	"                    ");}
			else if(rows == 7){
				The_page_processing(rows,	"                    ");}
			break;
		}		
		case FINDING_ORIGIN:
		{
			if(rows == 0){
				The_page_processing(rows, "                    ");}
			else if(rows == 1){
				The_page_processing(rows, "                    ");}
			else if(rows == 2){
				The_page_processing(rows, "                    ");}
			else if(rows == 3){
				The_page_processing(rows, "  �����Զ���ԭ��  ");}
			else if(rows == 4){
				The_page_processing(rows, "  ���Ե�Ƭ��  ");}
			else if(rows == 5){
				The_page_processing(rows, "                    ");}
			else if(rows == 6){
				The_page_processing(rows, "                    ");}
			else if(rows == 7){
				The_page_processing(rows, "                    ");}
			break;
		}
		case MANUAL_MODE:
		{
			if(rows == 0){
				The_page_processing(rows, "�ֶ�ģʽ");}
			else if(rows == 1){
				The_page_processing(rows, "                    ");}
			else if(rows == 2){
				The_page_processing(rows, "�ٶ�            ��");}
			else if(rows == 3){
				The_page_processing(rows, "                    ");}
			else if(rows == 4){
				The_page_processing(rows, "�Ƕ�              ");}
			else if(rows == 5){
				The_page_processing(rows, "                    ");}
			else if(rows == 6){
				The_page_processing(rows, "                    ");}
			else if(rows == 7){
				The_page_processing(rows, "                    ");}
			break;
		}		
		case ORIGIN_ACK:
		{
			if(rows == 0){
				The_page_processing(rows,"                    ");}
			else if(rows == 1){
				The_page_processing(rows,"                    ");}
			else if(rows == 2){
				The_page_processing(rows,	"�Ƿ�ȷ��Ҫ����ԭ��  ");}
			else if(rows == 3){
				The_page_processing(rows,	"  ��  ");}
			else if(rows == 4){
				The_page_processing(rows,"  ��  ");}
			else if(rows == 5){
				The_page_processing(rows,	"                    ");}
			else if(rows == 6){
				The_page_processing(rows,	"                    ");}
			else if(rows == 7){
				The_page_processing(rows,"                    ");}
			break;
		}
	}
}

void english_page_dis(uint8_t rows)
{	
	switch(page_id)
	{
		case LOCUS_PAGE:
		{
			if(rows == 0){
				The_page_processing(rows,"                    ");}
			else if(rows == 1){
				The_page_processing(rows,"                    ");}
			else if(rows == 2){
				The_page_processing(rows,	"Disconnected");}
			else if(rows == 3){
				The_page_processing(rows,"Signal's weak,please");}
			else if(rows == 4){
				The_page_processing(rows,	"approach the product");}
			else if(rows == 5){
				The_page_processing(rows,	"                    ");}
			else if(rows == 6){
				The_page_processing(rows,	"                    ");}
			else if(rows == 7){
				The_page_processing(rows,"                    ");}
			break;
		}
		case SET_A_POINT:
		{
			if(rows == 0){
				The_page_processing(rows,"                    ");}
			else if(rows == 1){
				The_page_processing(rows,"                    ");}
			else if(rows == 2){
				The_page_processing(rows,	"                    ");}
			else if(rows == 3){
				The_page_processing(rows,"Set Point A");}
			else if(rows == 4){
				The_page_processing(rows,	"                    ");}
			else if(rows == 5){
				The_page_processing(rows,	"                    ");}
			else if(rows == 6){
				The_page_processing(rows,	"                    ");}
			else if(rows == 7){
				The_page_processing(rows,"                    ");}
			break;
		}
		case SET_B_POINT:
		{
			if(rows == 0){
				The_page_processing(rows,"                    ");}
			else if(rows == 1){
				The_page_processing(rows,"                    ");}
			else if(rows == 2){
				The_page_processing(rows,"Set Point B");}
			else if(rows == 3){
				The_page_processing(rows,"                    ");}
			else if(rows == 4){
				The_page_processing(rows,	"Angle             ");}
			else if(rows == 5){
				The_page_processing(rows,	"                    ");}
			else if(rows == 6){
				The_page_processing(rows,	"                    ");}
			else if(rows == 7){
				The_page_processing(rows,"                    ");}
			break;
		}
		case ORIGIN_SET_MOVE:
		{
			if(rows == 0){
				The_page_processing(rows,"                    ");}
			else if(rows == 1){
				The_page_processing(rows,"                    ");}
			else if(rows == 2){
				The_page_processing(rows,	"                    ");}
			else if(rows == 3){
				The_page_processing(rows,"Origin setting");}
			else if(rows == 4){
				The_page_processing(rows,	"                    ");}
			else if(rows == 5){
				The_page_processing(rows,	"                    ");}
			else if(rows == 6){
				The_page_processing(rows,	"                    ");}
			else if(rows == 7){
				The_page_processing(rows,"                    ");}
			break;
		}
		case MAIN_ID:
		{
			if(rows == 0){
				The_page_processing(rows,	"                    ");
				} // manual
			else if(rows == 1){
				The_page_processing(rows,	"Origin set");}
			else if(rows == 2){
				The_page_processing(rows,"Panorama mode");}
			else if(rows == 3){
				The_page_processing(rows,	"Video mode");}
			else if(rows == 4){
				The_page_processing(rows,	"Time-lapse");} // manual mode
			else if(rows == 5){
				The_page_processing(rows,	"Manual mode");}
			else if(rows == 6){
				The_page_processing(rows,	"Group mode");}
			else if(rows == 7){
				The_page_processing(rows,"Settings");}
			break;
		}		
		case GROUP_PHOTO:
		{
			if(rows == 0){
				The_page_processing(rows,"Camera placement  ");}
			else if(rows == 1){
				The_page_processing(rows,	"Lens              ");}
			else if(rows == 2){
				The_page_processing(rows,"Roverlap          ");}
			else if(rows == 3){
				The_page_processing(rows,"Exposure          ");}
			else if(rows == 4){
				The_page_processing(rows,	"diraction         ");} // �Ӻ�
			else if(rows == 5){
				The_page_processing(rows,"Manual            ");}
			else if(rows == 6){
				The_page_processing(rows,"                    ");}
			else if(rows == 7){
				The_page_processing(rows,"                    ");}
			break;
		}
		case DELAY_SET:
		{
			if(rows == 0){
				The_page_processing(rows,	"Timelapse");}
			else if(rows == 1){
				The_page_processing(rows,"Interval            ");}
			else if(rows == 2){
				The_page_processing(rows,"Exposure            ");}
			else if(rows == 3){
				The_page_processing(rows,	"Frames              ");}
			else if(rows == 4){
				The_page_processing(rows,"diraction           ");}
			else if(rows == 5){
				The_page_processing(rows,"                    ");}
			else if(rows == 6){
				The_page_processing(rows,"AB point reset");}
			else if(rows == 7){
				The_page_processing(rows,"                    ");}
			break;
		}		
		case DELAY_DIS:
		{
			if(rows == 0){
				The_page_processing(rows,"Timelapse");}
			else if(rows == 1){
				The_page_processing(rows,	"                    ");}
			else if(rows == 2){
				The_page_processing(rows,"Progress            ");}
			else if(rows == 3){
				The_page_processing(rows,	"                    ");}
			else if(rows == 4){
				The_page_processing(rows,"Pic                 ");}
			else if(rows == 5){
				The_page_processing(rows,"Pause");}
			else if(rows == 6){
				The_page_processing(rows,"                    ");}
			else if(rows == 7){
				The_page_processing(rows,"                    ");}
			break;
		}		
		case CONFIG_ID:
		{
			if(rows == 0){
				The_page_processing(rows,"                    ");}
			else if(rows == 1){
				The_page_processing(rows,"Language     English");}
			else if(rows == 2){
				The_page_processing(rows,"Initialize          ");}
			else if(rows == 3){
				The_page_processing(rows,"Brightness          ");}
			else if(rows == 4){
				The_page_processing(rows,"Sleep               ");}
			else if(rows == 5){
				The_page_processing(rows,"Version             ");}
			else if(rows == 6){
				The_page_processing(rows,"                    ");}
			else if(rows == 7){
				The_page_processing(rows,"                    ");}
			break;
		}		
		case VIDEO_MODE:
		{
			if(rows == 0){
				The_page_processing(rows,"Video mode");}
			else if(rows == 1){
				The_page_processing(rows,	"direction           ");}
			else if(rows == 2){
				The_page_processing(rows,"Times               ");}
			else if(rows == 3){
				The_page_processing(rows,	"Loop                ");}
			else if(rows == 4){
				The_page_processing(rows,"                    ");}
			else if(rows == 5){
				The_page_processing(rows,"AB point reset");}
			else if(rows == 6){
				The_page_processing(rows,"                    ");}
			else if(rows == 7){
				The_page_processing(rows,"                    ");}
			break;
		}
	/*	case SINGLE_PANORAMA:
		{
			if(rows == 0){
				The_page_processing(rows,"Standar mdoe");}
			else if(rows == 1){
				The_page_processing(rows,	"Interval            ");}
			else if(rows == 2){
				The_page_processing(rows,	"Exposure            ");}
			else if(rows == 3){
				The_page_processing(rows,"Delay               ");}
			else if(rows == 4){
				The_page_processing(rows,	"Manual              ");}
			else if(rows == 5){
				The_page_processing(rows,"                    ");}
			else if(rows == 6){
				The_page_processing(rows,"Countdown           ");}
			else if(rows == 7){
				The_page_processing(rows,"Pic                 ");}
			break;
		}*/
		case PREINSTALL_MODE:
		{
			if(rows == 0){
				The_page_processing(rows,"Panorama Mode");}
			else if(rows == 1){
				The_page_processing(rows,	"                    ");}
			else if(rows == 2){
				The_page_processing(rows,"                    ");}
			else if(rows == 3){
				The_page_processing(rows,	"                    ");}
			else if(rows == 4){
				The_page_processing(rows,"                    ");}
			else if(rows == 5){
				The_page_processing(rows,"                    ");}
			else if(rows == 6){
				The_page_processing(rows,"                    ");}
			else if(rows == 7){
				The_page_processing(rows,"                    ");}
			break;
		}
		case PREINSTALL_SET:
		{
			if(rows == 0){
				The_page_processing(rows,"Panorama Mode(Para)");}
			else if(rows == 1){
				The_page_processing(rows,	"                    ");}
			else if(rows == 2){
				The_page_processing(rows,"                    ");}
			else if(rows == 3){
				The_page_processing(rows,	"                    ");}
			else if(rows == 4){
				The_page_processing(rows,"                    ");}
			else if(rows == 5){
				The_page_processing(rows,"                    ");}
			else if(rows == 6){
				The_page_processing(rows,"                    ");}
			else if(rows == 7){
				The_page_processing(rows,"                    ");}
			break;
		}
		case PREINSTALL_SET1:
		{
			if(rows == 0){
				The_page_processing(rows,"Expert Mode(Set)");}
			else if(rows == 1){
				The_page_processing(rows,	"Angle             ");}
			else if(rows == 2){
				The_page_processing(rows,"Shut count        ");}
			else if(rows == 3){
				The_page_processing(rows,	"Frames            ");}
			else if(rows == 4){
				The_page_processing(rows,"Exposure          ");}
			else if(rows == 5){
				The_page_processing(rows,"Interval          ");}
			else if(rows == 6){
				The_page_processing(rows,"Delay             ");}
			else if(rows == 7){
				The_page_processing(rows,"                    ");}
			break;
		}		
		case PREINSTALL_MOVE:
		{
			if(rows == 0){
				The_page_processing(rows,"Panorama Mode(move)");}
			else if(rows == 1){
				The_page_processing(rows,	"Timed start       ");}
			else if(rows == 2){
				The_page_processing(rows,"Manual            ");}
			else if(rows == 3){
				The_page_processing(rows,	" Start ");}
			else if(rows == 4){
				The_page_processing(rows,"Reshoot");}
			else if(rows == 5){
				The_page_processing(rows,"                    ");}
			else if(rows == 6){
				The_page_processing(rows,"Pic               ");}
			else if(rows == 7){
				The_page_processing(rows,"                    ");}
			break;
		}
		case PREINSTALL_SESHOT:
		{
			if(rows == 0){
				The_page_processing(rows, "Panorama Mode(Reshot");}
			else if(rows == 1){
				The_page_processing(rows,	 "                    ");}
			else if(rows == 2){
				The_page_processing(rows, "Press arrow keys to ");}
			else if(rows == 3){
				The_page_processing(rows, "select              ");}
			else if(rows == 4){
				The_page_processing(rows,	 "                    ");}
			else if(rows == 5){
				The_page_processing(rows,	 "Previous    Next");}
			else if(rows == 6){
				The_page_processing(rows, "                    ");} // �붨ʱ���Ƹ���
			else if(rows == 7){
				The_page_processing(rows,	 "Press menu key  exit");}
			break;
		}
		case REVERSE_MODE:
		{
			if(rows == 0){
				The_page_processing(rows,"                    ");}
			else if(rows == 1){
				The_page_processing(rows,"Confirm to reset    ");}
			else if(rows == 2){
				The_page_processing(rows,"A/B point?          ");}
			else if(rows == 3){
				The_page_processing(rows,	"  No  ");}
			else if(rows == 4){
				The_page_processing(rows,"  Yes ");}
			else if(rows == 5){
				The_page_processing(rows,	"                    ");}
			else if(rows == 6){
				The_page_processing(rows,	"                    ");}
			else if(rows == 7){
				The_page_processing(rows,"                    ");}
			break;
		}		
		case FINDING_ORIGIN:
		{
			if(rows == 0){
				The_page_processing(rows, "                    ");}
			else if(rows == 1){
				The_page_processing(rows, "                    ");}
			else if(rows == 2){
				The_page_processing(rows, "Automatic origin fi-");}
			else if(rows == 3){
				The_page_processing(rows, "nding,Please weite..");}
			else if(rows == 4){
				The_page_processing(rows, "                    ");}
			else if(rows == 5){
				The_page_processing(rows, "                    ");}
			else if(rows == 6){
				The_page_processing(rows, "                    ");}
			else if(rows == 7){
				The_page_processing(rows, "                    ");}
			break;
		}
		case MANUAL_MODE:
		{
			if(rows == 0){
				The_page_processing(rows, "Manual Mode");}
			else if(rows == 1){
				The_page_processing(rows, "                    ");}
			else if(rows == 2){
				The_page_processing(rows, "Speed           Gear");}
			else if(rows == 3){
				The_page_processing(rows, "                    ");}
			else if(rows == 4){
				The_page_processing(rows,	 "Angle             ");}
			else if(rows == 5){
				The_page_processing(rows, "                    ");}
			else if(rows == 6){
				The_page_processing(rows, "                    ");}
			else if(rows == 7){
				The_page_processing(rows, "                    ");}
			break;
		}		
		case ORIGIN_ACK:
		{
			if(rows == 0){
				The_page_processing(rows,"                    ");}
			else if(rows == 1){
				The_page_processing(rows,"Confirm to reset    ");}
			else if(rows == 2){
				The_page_processing(rows,"origin point?       ");}
			else if(rows == 3){
				The_page_processing(rows,	"  No  ");}
			else if(rows == 4){
				The_page_processing(rows,"  Yes ");}
			else if(rows == 5){
				The_page_processing(rows,	"                    ");}
			else if(rows == 6){
				The_page_processing(rows,	"                    ");}
			else if(rows == 7){
				The_page_processing(rows,"                    ");}
			break;
		}
	}
}

void German_page_dis(uint8_t rows)
{	
	switch(page_id)
	{
		case LOCUS_PAGE:
		{
			if(rows == 0){
				The_page_processing(rows,"                    ");}
			else if(rows == 1){
				The_page_processing(rows,"                    ");}
			else if(rows == 2){
				The_page_processing(rows,	"Verbinden");} //connecting
			else if(rows == 3){
				The_page_processing(rows,"Bitte warten");} // please wait
			else if(rows == 4){
				The_page_processing(rows,	"                    ");}
			else if(rows == 5){
				The_page_processing(rows,	"                    ");}
			else if(rows == 6){
				The_page_processing(rows,	"                    ");}
			break;
		}
		case SET_A_POINT:
		{
			if(rows == 0){
				The_page_processing(rows,"                    ");}
			else if(rows == 1){
				The_page_processing(rows,"                    ");}
			else if(rows == 2){
				The_page_processing(rows,	"                    ");}
			else if(rows == 3){
				The_page_processing(rows,"Sollwert A");} // set point a
			else if(rows == 4){
				The_page_processing(rows,	"                    ");}
			else if(rows == 5){
				The_page_processing(rows,	"                    ");}
			else if(rows == 6){
				The_page_processing(rows,	"                    ");}
			break;
		}
		case SET_B_POINT:
		{
			if(rows == 0){
				The_page_processing(rows,"                    ");}
			else if(rows == 1){
				The_page_processing(rows,"                    ");}
			else if(rows == 2){
				The_page_processing(rows,	"                    ");}
			else if(rows == 3){
				The_page_processing(rows,"Sollwert B");} // set point a
			else if(rows == 4){
				The_page_processing(rows,	"                    ");}
			else if(rows == 5){
				The_page_processing(rows,	"                    ");}
			else if(rows == 6){
				The_page_processing(rows,	"                    ");}
			break;
		}
		case MAIN_ID:
		{
			if(rows == 0){
				The_page_processing(rows,	"                    ");
				} // manual
			else if(rows == 1){
				The_page_processing(rows,"Video Modus");} // video mode
			else if(rows == 2){
				The_page_processing(rows,	"Zeitraffer");} // timelapse
			else if(rows == 3){
				The_page_processing(rows,	"R cksetzpunkt A/B"); // Reset AB Point
				Oled_EnlPrint(19, rows,"��","", wifi_id.language_sel); // Set Point A
				}
			else if(rows == 4){
				The_page_processing(rows,	"Einstellungen");} // setting
			else if(rows == 5){
				The_page_processing(rows,	"                    ");}
			else if(rows == 6){
				The_page_processing(rows,	"                    ");}
			break;
		}
		case DELAY_SET:
		{
			if(rows == 0){
				The_page_processing(rows,	"Intervall           ");} // Interval
			else if(rows == 1){
				The_page_processing(rows,"Exposition          ");} // Exposure
			else if(rows == 2){
				The_page_processing(rows,"FPS                 ");} // Frames
			else if(rows == 3){
				The_page_processing(rows,"Feste Aufnahmen     ");} // focus
			else if(rows == 4){
				The_page_processing(rows,"Ziehen nach         ");}// move to
			else if(rows == 5){
				The_page_processing(rows,	"Verz gern           ");
				Oled_EnlPrint(28, rows,"O ","", wifi_id.language_sel);} //delay
			else if(rows == 6){
				The_page_processing(rows," Start ");} // start
			else if(rows == 7){
				The_page_processing(rows,	"Dauer                ");} // duration
			break;
		}		
		case DELAY_DIS:
		{
			if(rows == 0){
				The_page_processing(rows,"                    ");}
			else if(rows == 1){
				The_page_processing(rows,"Fortschritt         ");} // Progress
			else if(rows == 2){
				The_page_processing(rows,	"                    ");}
			else if(rows == 3){
				The_page_processing(rows,"FPS                 ");} // pic
			else if(rows == 4){
				The_page_processing(rows,"Pause");} // pause
			else if(rows == 5){
				The_page_processing(rows,"                    ");}
			else if(rows == 6){
				The_page_processing(rows,	"                    ");}
			break;
		}		
		case CONFIG_ID:
		{
			if(rows == 0){
				The_page_processing(rows,"                    ");}
			else if(rows == 1){
				The_page_processing(rows,"Sprache      Deutsch");} // Language
			else if(rows == 2){
				The_page_processing(rows,"Initialisieren      ");} // Initialize
			else if(rows == 3){
				The_page_processing(rows,"Helligkeit          ");} // Brightness
			else if(rows == 4){
				The_page_processing(rows,"Schlafen            ");} // sleep
			else if(rows == 5){
				The_page_processing(rows,"Versionen           ");} // version
			else if(rows == 6){
				The_page_processing(rows,	"                    ");}
			break;
		}
	}
}

void Japanese_page_dis(uint8_t rows)
{	
	switch(page_id)
	{
		case LOCUS_PAGE:
		{
			if(rows == 0){
				The_page_processing(rows,"                    ");}
			else if(rows == 1){
				The_page_processing(rows,"                    ");}
			else if(rows == 2){
				The_page_processing(rows,	"�ӾA��");} //connecting
			else if(rows == 3){
				The_page_processing(rows,"��������������");} // please wait
			else if(rows == 4){
				The_page_processing(rows,	"                    ");}
			else if(rows == 5){
				The_page_processing(rows,	"                    ");}
			else if(rows == 6){
				The_page_processing(rows,	"                    ");}
			break;
		}
		case SET_A_POINT:
		{
			if(rows == 0){
				The_page_processing(rows,"                    ");}
			else if(rows == 1){
				The_page_processing(rows,"                    ");}
			else if(rows == 2){
				The_page_processing(rows,	"                    ");}
			else if(rows == 3){
				The_page_processing(rows,"A ����O��");} // set point a
			else if(rows == 4){
				The_page_processing(rows,	"                    ");}
			else if(rows == 5){
				The_page_processing(rows,	"                    ");}
			else if(rows == 6){
				The_page_processing(rows,	"                    ");}
			break;
		}
		case SET_B_POINT:
		{
			if(rows == 0){
				The_page_processing(rows,"                    ");}
			else if(rows == 1){
				The_page_processing(rows,"                    ");}
			else if(rows == 2){
				The_page_processing(rows,	"                    ");}
			else if(rows == 3){
				The_page_processing(rows,"B ����O��");} // set point a
			else if(rows == 4){
				The_page_processing(rows,	"                    ");}
			else if(rows == 5){
				The_page_processing(rows,	"                    ");}
			else if(rows == 6){
				The_page_processing(rows,	"                    ");}
			break;
		}
		case MAIN_ID:
		{
			if(rows == 0){
				The_page_processing(rows,	"                    ");
				} // manual
			else if(rows == 1){
				The_page_processing(rows,"�ӥǥ���`��");} // video mode
			else if(rows == 2){
				The_page_processing(rows,	"�������ץ�");} // timelapse
			else if(rows == 3){
				The_page_processing(rows,	"A B ���ꥻ�å�"); // Reset AB Point
				}
			else if(rows == 4){
				The_page_processing(rows,	"  �O��  ");} // setting
			else if(rows == 5){
				The_page_processing(rows,	"                    ");}
			else if(rows == 6){
				The_page_processing(rows,	"                    ");}
			break;
		}		
		case DELAY_SET:
		{
			if(rows == 0){
				The_page_processing(rows,	"�g��                ");} // Interval
			else if(rows == 1){
				The_page_processing(rows,"¶��                ");} // Exposure
			else if(rows == 2){
				The_page_processing(rows,"ö��                ");} // Frames
			else if(rows == 3){
				The_page_processing(rows,"�̶����Ӱ          ");} // Fixed shot
			else if(rows == 4){
				The_page_processing(rows,"���Ƅ�              ");} // move to
			else if(rows == 5){
				The_page_processing(rows,	"����                ");}// Delay
			else if(rows == 6){
				The_page_processing(rows,"�����`��");} // start
			else if(rows == 7){
				The_page_processing(rows,	"�־A�r�g            ");} // duration
			break;
		}		
		case DELAY_DIS:
		{
			if(rows == 0){
				The_page_processing(rows,"                    ");}
			else if(rows == 1){
				The_page_processing(rows,"�M��                ");} // Progress
			else if(rows == 2){
				The_page_processing(rows,	"                    ");}
			else if(rows == 3){
				The_page_processing(rows,"����                ");} // pic
			else if(rows == 4){
				The_page_processing(rows,"һ�rֹͣ");} // pause
			else if(rows == 5){
				The_page_processing(rows,"                    ");}
			else if(rows == 6){
				The_page_processing(rows,	"                    ");}
			break;
		}				
		case CONFIG_ID:
		{
			if(rows == 0){
				The_page_processing(rows,"                    ");}
			else if(rows == 1){
				The_page_processing(rows,"���Z          �ձ��Z");} // Language
			else if(rows == 2){
				The_page_processing(rows,"���ڻ�              ");} // Initialize
			else if(rows == 3){
				The_page_processing(rows,"���뤵              ");} // Brightness
			else if(rows == 4){
				The_page_processing(rows,"����`��            ");} // sleep
			else if(rows == 5){
				The_page_processing(rows,"�Щ`�����          ");} // version
			else if(rows == 6){
				The_page_processing(rows,	"                    ");}
			break;
		}
	}
}

void Italiano_page_dis(uint8_t rows)
{	
	switch(page_id)
	{
		case LOCUS_PAGE:
		{
			if(rows == 0){
				The_page_processing(rows,"                    ");}
			else if(rows == 1){
				The_page_processing(rows,"                    ");}
			else if(rows == 2){
				The_page_processing(rows,	"Connessione");} //connecting
			else if(rows == 3){
				The_page_processing(rows,"Attendere ");} // please wait
			else if(rows == 4){
				The_page_processing(rows,	"                    ");}
			else if(rows == 5){
				The_page_processing(rows,	"                    ");}
			else if(rows == 6){
				The_page_processing(rows,	"                    ");}
			break;
		}
		case SET_A_POINT:
		{
			if(rows == 0){
				The_page_processing(rows,"                    ");}
			else if(rows == 1){
				The_page_processing(rows,"                    ");}
			else if(rows == 2){
				The_page_processing(rows,	"                    ");}
			else if(rows == 3){
				The_page_processing(rows,"Impostare punto A");} // set point a
			else if(rows == 4){
				The_page_processing(rows,	"                    ");}
			else if(rows == 5){
				The_page_processing(rows,	"                    ");}
			else if(rows == 6){
				The_page_processing(rows,	"                    ");}
			break;
		}
		case SET_B_POINT:
		{
			if(rows == 0){
				The_page_processing(rows,"                    ");}
			else if(rows == 1){
				The_page_processing(rows,"                    ");}
			else if(rows == 2){
				The_page_processing(rows,	"                    ");}
			else if(rows == 3){
				The_page_processing(rows,"Impostare punto B");} // set point a
			else if(rows == 4){
				The_page_processing(rows,	"                    ");}
			else if(rows == 5){
				The_page_processing(rows,	"                    ");}
			else if(rows == 6){
				The_page_processing(rows,	"                    ");}
			break;
		}
		case MAIN_ID:
		{
			if(rows == 0){
				The_page_processing(rows,	"                    ");
				} // manual
			else if(rows == 1){
				The_page_processing(rows,"Modalita' Video");} // video mode
			else if(rows == 2){
				The_page_processing(rows,	"Time-lapse");} // timelapse
			else if(rows == 3){
				The_page_processing(rows,	"Reset punto A/B"); // Reset AB Point
				}
			else if(rows == 4){
				The_page_processing(rows,	"Impostazioni");} // setting
			else if(rows == 5){
				The_page_processing(rows,	"                    ");}
			else if(rows == 6){
				The_page_processing(rows,	"                    ");}
			break;
		}
		case DELAY_SET:
		{
			if(rows == 0){
				The_page_processing(rows,	"Interv              ");} // Interval
			else if(rows == 1){
				The_page_processing(rows,"Expo                ");} // Exposure
			else if(rows == 2){
				The_page_processing(rows,"Scatti              ");} // Frames
			else if(rows == 3){
				The_page_processing(rows,"Ripresa fissa       ");} // Fixed shot
			else if(rows == 4){
				The_page_processing(rows,"Sposta in           ");} // move to
			else if(rows == 5){
				The_page_processing(rows,	"Ritardare           ");}// Delay
			else if(rows == 6){
				The_page_processing(rows,"Avvio");} // start
			else if(rows == 7){
				The_page_processing(rows,	"Durata              ");} // duration
			break;
		}		
		case DELAY_DIS:
		{
			if(rows == 0){
				The_page_processing(rows,"                    ");}
			else if(rows == 1){
				The_page_processing(rows,"Progresso           ");} // Progress
			else if(rows == 2){
				The_page_processing(rows,	"                    ");}
			else if(rows == 3){
				The_page_processing(rows,"FPS                 ");} // pic
			else if(rows == 4){
				The_page_processing(rows,"Pausa");} // pause
			else if(rows == 5){
				The_page_processing(rows,"                    ");}
			else if(rows == 6){
				The_page_processing(rows,	"                    ");}
			break;
		}		
		case CONFIG_ID:
		{
			if(rows == 0){
				The_page_processing(rows,"                    ");}
			else if(rows == 1){
				The_page_processing(rows,"Lingua      Italiano");} // Language
			else if(rows == 2){
				The_page_processing(rows,"Inizializzare       ");} // Initialize
			else if(rows == 3){
				The_page_processing(rows,"Luminosita'         ");} // Brightness
				///Oled_EnlPrint(60, rows,"��","", wifi_id.language_sel);} // Brightness
			else if(rows == 4){
				The_page_processing(rows,"Pausa               ");} // sleep
			else if(rows == 5){
				The_page_processing(rows,"Versione            ");} // version
			else if(rows == 6){
				The_page_processing(rows,	"                    ");}
			break;
		}
	}
}

void Spanish_page_dis(uint8_t rows)
{	
	switch(page_id)
	{
		case LOCUS_PAGE:
		{
			if(rows == 0){
				The_page_processing(rows,"                    ");}
			else if(rows == 1){
				The_page_processing(rows,"                    ");}
			else if(rows == 2){
				The_page_processing(rows,	"Conectando");} //connecting
			else if(rows == 3){
				The_page_processing(rows,"Espere ");} // please wait
			else if(rows == 4){
				The_page_processing(rows,	"                    ");}
			else if(rows == 5){
				The_page_processing(rows,	"                    ");}
			else if(rows == 6){
				The_page_processing(rows,	"                    ");}
			break;
		}
		case SET_A_POINT:
		{
			if(rows == 0){
				The_page_processing(rows,"                    ");}
			else if(rows == 1){
				The_page_processing(rows,"                    ");}
			else if(rows == 2){
				The_page_processing(rows,	"                    ");}
			else if(rows == 3){
				The_page_processing(rows,"Punto de ajuste A");} // set point a
			else if(rows == 4){
				The_page_processing(rows,	"                    ");}
			else if(rows == 5){
				The_page_processing(rows,	"                    ");}
			else if(rows == 6){
				The_page_processing(rows,	"                    ");}
			break;
		}
		case SET_B_POINT:
		{
			if(rows == 0){
				The_page_processing(rows,"                    ");}
			else if(rows == 1){
				The_page_processing(rows,"                    ");}
			else if(rows == 2){
				The_page_processing(rows,	"                    ");}
			else if(rows == 3){
				The_page_processing(rows,"Punto de ajuste B");} // set point a
			else if(rows == 4){
				The_page_processing(rows,	"                    ");}
			else if(rows == 5){
				The_page_processing(rows,	"                    ");}
			else if(rows == 6){
				The_page_processing(rows,	"                    ");}
			break;
		}
		case MAIN_ID:
		{
			if(rows == 0){
				The_page_processing(rows,	"                    ");
				} // manual
			else if(rows == 1){
				The_page_processing(rows,"Modo de v deo"); // Live motion
				Oled_EnlPrint(79, rows,"��","", wifi_id.language_sel);} // Set Point A
			else if(rows == 2){
				The_page_processing(rows,	"Time-lapse");} // timelapse
			else if(rows == 3){
				The_page_processing(rows,	"Reset P A/B"); // Reset AB Point
				}
			else if(rows == 4){
				The_page_processing(rows,	"Ajustes");} // setting
			else if(rows == 5){
				The_page_processing(rows,	"                    ");}
			else if(rows == 6){
				The_page_processing(rows,	"                    ");}
			break;
		}		
		case DELAY_SET:
		{
			if(rows == 0){
				The_page_processing(rows,	"Interv              ");} // Interval
			else if(rows == 1){
				The_page_processing(rows,"Expo                ");} // Exposure
			else if(rows == 2){
				The_page_processing(rows,"Frames              ");} // Frames
			else if(rows == 3){
				The_page_processing(rows,"Disp.fijo           ");} // Fixed shot
			else if(rows == 4){
				The_page_processing(rows,"Mover a             ");} // move to
			else if(rows == 5){
				The_page_processing(rows,	"Retraso             ");}// Delay
			else if(rows == 6){
				The_page_processing(rows,"Inicio");} // start
			else if(rows == 7){
				The_page_processing(rows,	"Duraci n            "); // duration
				Oled_EnlPrint(40, rows,"��","", wifi_id.language_sel);} //
			break;
		}		
		case DELAY_DIS:
		{
			if(rows == 0){
				The_page_processing(rows,"                    ");}
			else if(rows == 1){
				The_page_processing(rows,"Progreso            ");} // Progress
			else if(rows == 2){
				The_page_processing(rows,	"                    ");}
			else if(rows == 3){
				The_page_processing(rows,"FPS                 ");} // pic
			else if(rows == 4){
				The_page_processing(rows,"Pausa");} // pause
			else if(rows == 5){
				The_page_processing(rows,"                    ");}
			else if(rows == 6){
				The_page_processing(rows,	"                    ");}
			break;
		}		
		case CONFIG_ID:
		{
			if(rows == 0){
				The_page_processing(rows,"                    ");}
			else if(rows == 1){
				The_page_processing(rows,"Idioma       Espa ol"); // Language:  English
				Oled_EnlPrint(106, rows,"n ","", wifi_id.language_sel);} // 
			else if(rows == 2){
				The_page_processing(rows,"Inicializar         ");} // Initialize
			else if(rows == 3){
				The_page_processing(rows,"Brillo              ");} // Brightness
			else if(rows == 4){
				The_page_processing(rows,"Sleep               ");} // sleep
				//Oled_EnlPrint(58, rows,"��","", wifi_id.language_sel);} // 
			else if(rows == 5){
				The_page_processing(rows,"Versi n             "); // version
				Oled_EnlPrint(34, rows,"��","", wifi_id.language_sel);} //
			else if(rows == 6){
				The_page_processing(rows,	"                    ");}
			break;
		}
	}
}

void French_page_dis(uint8_t rows)
{	
	switch(page_id)
	{
		case LOCUS_PAGE:
		{
			if(rows == 0){
				The_page_processing(rows,"                    ");}
			else if(rows == 1){
				The_page_processing(rows,"                    ");}
			else if(rows == 2){
				The_page_processing(rows,	"Connexion");} //connecting
			else if(rows == 3){
				The_page_processing(rows,"Attendez svp ");} // please wait
			else if(rows == 4){
				The_page_processing(rows,	"                    ");}
			else if(rows == 5){
				The_page_processing(rows,	"                    ");}
			else if(rows == 6){
				The_page_processing(rows,	"                    ");}
			break;
		}
		case SET_A_POINT:
		{
			if(rows == 0){
				The_page_processing(rows,"                    ");}
			else if(rows == 1){
				The_page_processing(rows,"                    ");}
			else if(rows == 2){
				The_page_processing(rows,	"                    ");}
			else if(rows == 3){
				The_page_processing(rows,"D finir le point A"); // Set Point A
				Oled_EnlPrint(16, rows,"��","", wifi_id.language_sel);} // Set Point A
			else if(rows == 4){
				The_page_processing(rows,	"                    ");}
			else if(rows == 5){
				The_page_processing(rows,	"                    ");}
			else if(rows == 6){
				The_page_processing(rows,	"                    ");}
			break;
		}
		case SET_B_POINT:
		{
			if(rows == 0){
				The_page_processing(rows,"                    ");}
			else if(rows == 1){
				The_page_processing(rows,"                    ");}
			else if(rows == 2){
				The_page_processing(rows,	"                    ");}
			else if(rows == 3){
				The_page_processing(rows,"D finir le point B"); // Set Point B
				Oled_EnlPrint(16, rows,"��","", wifi_id.language_sel);} // Set Point B
			else if(rows == 4){
				The_page_processing(rows,	"                    ");}
			else if(rows == 5){
				The_page_processing(rows,	"                    ");}
			else if(rows == 6){
				The_page_processing(rows,	"                    ");}
			break;
		}
		case MAIN_ID:
		{
			if(rows == 0){
				The_page_processing(rows,	"                    ");
				} // manual
			else if(rows == 1){
				The_page_processing(rows,"Mode vid o"); // Live motion
				Oled_EnlPrint(82, rows,"��","", wifi_id.language_sel);} // Live motion
			else if(rows == 2){
				The_page_processing(rows,	"Time-lapse");} // timelapse
			else if(rows == 3){
				The_page_processing(rows,	"RAZ le point AB"); // Reset AB Point
				}
			else if(rows == 4){
				The_page_processing(rows,	"Param tres"); // Setting
				Oled_EnlPrint(64, rows,"��","", wifi_id.language_sel);} // Set Point A
			else if(rows == 5){
				The_page_processing(rows,	"                    ");}
			else if(rows == 6){
				The_page_processing(rows,	"                    ");}
			break;
		}
		case DELAY_SET:
		{
			if(rows == 0){
				The_page_processing(rows,	"Intervalle          ");} // Interval
			else if(rows == 1){
				The_page_processing(rows,"Exposition          ");} // Exposure
			else if(rows == 2){
				The_page_processing(rows,"Prises              ");} // Frames
			else if(rows == 3){
				The_page_processing(rows,"PDV point fixe      ");} // Fixed shot
			else if(rows == 4){
				The_page_processing(rows,	"D placer            "); // Move to
				Oled_EnlPrint(10, rows,"��","", wifi_id.language_sel); // Move to
				Oled_EnlPrint(58, rows,"��","", wifi_id.language_sel);} //Move to
			else if(rows == 5){
				The_page_processing(rows,"D lai               "); // Delay
				Oled_EnlPrint(10, rows,"��","", wifi_id.language_sel);} // Set Point A
			else if(rows == 6){
				The_page_processing(rows,	"D marrer"); // Start
				Oled_EnlPrint(46, rows,"��","", wifi_id.language_sel);} // Start
			else if(rows == 7){
				The_page_processing(rows,"Dur es              "); // Duration:
				Oled_EnlPrint(22, rows,"��","", wifi_id.language_sel);} //Duration
			break;
		}		
		case DELAY_DIS:
		{
			if(rows == 0){
				The_page_processing(rows,"                    ");}
			else if(rows == 1){
				The_page_processing(rows,"Progression         ");} // Progress
			else if(rows == 2){
				The_page_processing(rows,	"                    ");}
			else if(rows == 3){
				The_page_processing(rows,"FPS                 ");} // pic
			else if(rows == 4){
				The_page_processing(rows,"Pause");} // pause
			else if(rows == 5){
				The_page_processing(rows,"                    ");}
			else if(rows == 6){
				The_page_processing(rows,	"                    ");}
			break;
		}		
		case CONFIG_ID:
		{
			if(rows == 0){
				The_page_processing(rows,"                    ");}
			else if(rows == 1){
				The_page_processing(rows,"Langue      Fran ais"); // Language:  English
				Oled_EnlPrint(100, rows,"c ","", wifi_id.language_sel);} // 
			else if(rows == 2){
				The_page_processing(rows,"Initialiser         ");} // Initialize
			else if(rows == 3){
				The_page_processing(rows,"Luminosit           "); // Brightness
				Oled_EnlPrint(58, rows,"��","", wifi_id.language_sel);} // 
			else if(rows == 4){
				The_page_processing(rows,"Mode veille         ");} // sleep
			else if(rows == 5){
				The_page_processing(rows,"Version             ");} // version
			else if(rows == 6){
				The_page_processing(rows,	"                    ");}
			break;
		}
	}
}

void Netherlands_page_dis(uint8_t rows)
{	
	switch(page_id)
	{
		case LOCUS_PAGE:
		{
			if(rows == 0){
				The_page_processing(rows,"                    ");}
			else if(rows == 1){
				The_page_processing(rows,"                    ");}
			else if(rows == 2){
				The_page_processing(rows,	"Verbinden");} //connecting
			else if(rows == 3){
				The_page_processing(rows,"Wacht aub ");} // please wait
			else if(rows == 4){
				The_page_processing(rows,	"                    ");}
			else if(rows == 5){
				The_page_processing(rows,	"                    ");}
			else if(rows == 6){
				The_page_processing(rows,	"                    ");}
			break;
		}
		case SET_A_POINT:
		{
			if(rows == 0){
				The_page_processing(rows,"                    ");}
			else if(rows == 1){
				The_page_processing(rows,"                    ");}
			else if(rows == 2){
				The_page_processing(rows,	"                    ");}
			else if(rows == 3){
				The_page_processing(rows,"Instelpunt A");} // set point a
			else if(rows == 4){
				The_page_processing(rows,	"                    ");}
			else if(rows == 5){
				The_page_processing(rows,	"                    ");}
			else if(rows == 6){
				The_page_processing(rows,	"                    ");}
			break;
		}
		case SET_B_POINT:
		{
			if(rows == 0){
				The_page_processing(rows,"                    ");}
			else if(rows == 1){
				The_page_processing(rows,"                    ");}
			else if(rows == 2){
				The_page_processing(rows,	"                    ");}
			else if(rows == 3){
				The_page_processing(rows,"Instelpunt B");} // set point a
			else if(rows == 4){
				The_page_processing(rows,	"                    ");}
			else if(rows == 5){
				The_page_processing(rows,	"                    ");}
			else if(rows == 6){
				The_page_processing(rows,	"                    ");}
			break;
		}
		case MAIN_ID:
		{
			if(rows == 0){
				The_page_processing(rows,	"                    ");
				} // manual
			else if(rows == 1){
				The_page_processing(rows,"Videomodus");} // Live motion
			else if(rows == 2){
				The_page_processing(rows,	"Time-lapse");} // timelapse
			else if(rows == 3){
				The_page_processing(rows,	"A/B-punt reset"); // Reset AB Point
				}
			else if(rows == 4){
				The_page_processing(rows,	"Instellingen");} // setting
			else if(rows == 5){
				The_page_processing(rows,	"                    ");}
			else if(rows == 6){
				The_page_processing(rows,	"                    ");}
			break;
		}
		case DELAY_SET:
		{
			if(rows == 0){
				The_page_processing(rows,	"Interval            ");} // Interval
			else if(rows == 1){
				The_page_processing(rows,"Belichting          ");} // Exposure
			else if(rows == 2){
				The_page_processing(rows,"Frames              ");} // Frames
			else if(rows == 3){
				The_page_processing(rows,"Vastpuntopname      ");} // Fixed shot
			else if(rows == 4){
				The_page_processing(rows,"Verplaatsen         ");} // move to
			else if(rows == 5){
				The_page_processing(rows,	"uitstel             ");}// Delay
			else if(rows == 6){
				The_page_processing(rows,"Start");} // start
			else if(rows == 7){
				The_page_processing(rows,	"Duur                ");} // duration
			break;
		}		
		case DELAY_DIS:
		{
			if(rows == 0){
				The_page_processing(rows,"                    ");}
			else if(rows == 1){
				The_page_processing(rows,"Voortgang           ");} // Progress
			else if(rows == 2){
				The_page_processing(rows,	"                    ");}
			else if(rows == 3){
				The_page_processing(rows,"Foto                ");} // pic
			else if(rows == 4){
				The_page_processing(rows,"Pauze");} // pause
			else if(rows == 5){
				The_page_processing(rows,"                    ");}
			else if(rows == 6){
				The_page_processing(rows,	"                    ");}
			break;
		}		
		case CONFIG_ID:
		{
			if(rows == 0){
				The_page_processing(rows,"                    ");}
			else if(rows == 1){
				The_page_processing(rows,"Taal      Nederlands");} // Language:  English
			else if(rows == 2){
				The_page_processing(rows,"Initialiseren       ");} // Initialize
			else if(rows == 3){
				The_page_processing(rows,"Helderheid          ");} // Brightness
			else if(rows == 4){
				The_page_processing(rows,"Slaap               ");} // sleep
			else if(rows == 5){
				The_page_processing(rows,"Versie              ");} // version
			else if(rows == 6){
				The_page_processing(rows,	"                    ");}
			break;
		}
	}
}

void page_display(uint8_t rowd)
{
	switch(wifi_id.language_sel)
	{
		case CHINESE:
		{
			chinese_page_dis(rowd);
			break;
		}
		case ENGLISH:
		{
			english_page_dis(rowd);
			break;
		}
		/*case NELANDS:
		{
			Netherlands_page_dis(rowd);
			break;
		}
		case FRENCH:
		{
			French_page_dis(rowd);
			break;
		}
		case SPANISH:
		{
			Spanish_page_dis(rowd);
			break;
		}
		case ITALIANO:
		{
			Italiano_page_dis(rowd);
			break;
		}		
		case GERMAN:
		{
			German_page_dis(rowd);
			break;
		}
		case JAPANESE:
		{
			Japanese_page_dis(rowd);
			break;
		}*/
	}
}

uint8_t Check_String_Size(uint8_t *strp)
{
	uint8_t i =0 ;
	uint8_t temp = 0;

	for(i = 0; i<21; i++)
	{
		if(strp[i] == '\0')
		{
			temp = i;
			break;
		}
	}


	return temp;
}

uint8_t Check_String(uint8_t *Estrp, uint8_t LNG/*, uint8_t *Cstrp, uint8_t *Kstrp*/)
{
//	uint8_t buff[20] = {0};
	uint8_t z_size = 0;
	
	z_buffer = Estrp;
	z_size = Check_String_Size(Estrp) * 6;

	return z_size;
}

// ��ʾһ��
void TFT_Line_Dis(uint8_t *srtp, uint8_t *csrtp, uint8_t row)
{
	uint8_t x_size = 0;
	uint8_t *buff = 0;
	uint8_t yend = 0, ysta = 0;
	uint8_t xend = 0, xsta = 0;

	if(wifi_id.language_sel == ENGLISH)buff = srtp;
	else if(wifi_id.language_sel == CHINESE)buff = csrtp;
	
	x_size = Check_String(buff, wifi_id.language_sel);
	xend = (LCD_W - x_size) / 2;
	xsta = xend + x_size;
	ysta = row * (24 + 5) + 34;
	yend = ysta+24;
	
	LCD_Fill(0, ysta, xend, yend, BLACK);
	LCD_Fill(xsta, ysta, 240, yend, BLACK);
	
	Oled_EnlPrint(xend, row, z_buffer, z_buffer, wifi_id.language_sel);
}


void LCD_Writ_Bus(char dat)   //��������д�룬дһ���ֽ�
{	
	Spi_write_data(dat);
}

void LCD_WR_DATA8(char da) //��������-8λ����
{	//OLED_CS_Clr();
    OLED_DC_Set();
	LCD_Writ_Bus(da);  
	//OLED_CS_Set();
}  
 void LCD_WR_DATA(int da)
{//	OLED_CS_Clr();
    OLED_DC_Set();
	LCD_Writ_Bus(da>>8);
    LCD_Writ_Bus(da);
	//OLED_CS_Set();
}	  
void LCD_WR_REG(char da)	 
{	//	OLED_CS_Clr();
    OLED_DC_Clr();
	LCD_Writ_Bus(da);
	//OLED_CS_Set();
}
 void LCD_WR_REG_DATA(int reg,int da)
{	//OLED_CS_Clr();
    LCD_WR_REG(reg);
	LCD_WR_DATA(da);
	//OLED_CS_Set();
}

void Address_set(unsigned int x1,unsigned int y1,unsigned int x2,unsigned int y2)
{ 
	LCD_WR_REG(0x2a);
   LCD_WR_DATA8(x1>>8);
   LCD_WR_DATA8(x1);
   LCD_WR_DATA8(x2>>8);
   LCD_WR_DATA8(x2);
  
   LCD_WR_REG(0x2b);
   LCD_WR_DATA8(y1>>8);
   LCD_WR_DATA8(y1);
   LCD_WR_DATA8(y2>>8);
   LCD_WR_DATA8(y2);

   LCD_WR_REG(0x2C);					 						 
}

void Lcd_Init(void)
{
	//OLED_CS_Clr();  //��Ƭѡʹ��
	 OLED_RST_Clr();
	time_delay_ms(20);
	OLED_RST_Set();
	time_delay_ms(20);
	//OLED_BLK_Set();
	//time_delay_ms(20);
	
	LCD_WR_REG(0x11);//Sleep exit 
	time_delay_ms (120);
	LCD_WR_REG(0x13);// Normal Display Mode On
		
	//ST7735R Frame Rate
	LCD_WR_REG(0xB1); 
	LCD_WR_DATA8(0x01); 
	LCD_WR_DATA8(0x2C); 
	LCD_WR_DATA8(0x2D); 
	
	LCD_WR_REG(0xB2); 
	LCD_WR_DATA8(0x01); 
	LCD_WR_DATA8(0x2C); 
	LCD_WR_DATA8(0x2D); 
	
	LCD_WR_REG(0xB3); 
	LCD_WR_DATA8(0x01); 
	LCD_WR_DATA8(0x2C); 
	LCD_WR_DATA8(0x2D); 
	LCD_WR_DATA8(0x01); 
	LCD_WR_DATA8(0x2C); 
	LCD_WR_DATA8(0x2D); 
	
	LCD_WR_REG(0xB4); //Column inversion 
	LCD_WR_DATA8(0x07); 
	
	//ST7735R Power Sequence
	LCD_WR_REG(0xC0); 
	LCD_WR_DATA8(0xA2); 
	LCD_WR_DATA8(0x02); 
	LCD_WR_DATA8(0x84); 
	LCD_WR_REG(0xC1); 
	LCD_WR_DATA8(0xC5); 
	
	LCD_WR_REG(0xC2); 
	LCD_WR_DATA8(0x0A); 
	LCD_WR_DATA8(0x00); 
	
	LCD_WR_REG(0xC3); 
	LCD_WR_DATA8(0x8A); 
	LCD_WR_DATA8(0x2A); 
	LCD_WR_REG(0xC4); 
	LCD_WR_DATA8(0x8A); 
	LCD_WR_DATA8(0xEE); 
	
	LCD_WR_REG(0xC5); //VCOM 
	LCD_WR_DATA8(0x0E); 
	
	LCD_WR_REG(0x36); //MX, MY, RGB mode 
	//LCD_WR_DATA8(0xC0); 
	LCD_WR_DATA8(0x0); 
	
	//ST7735R Gamma Sequence
	LCD_WR_REG(0xe0); 
	LCD_WR_DATA8(0x0f); 
	LCD_WR_DATA8(0x1a); 
	LCD_WR_DATA8(0x0f); 
	LCD_WR_DATA8(0x18); 
	LCD_WR_DATA8(0x2f); 
	LCD_WR_DATA8(0x28); 
	LCD_WR_DATA8(0x20); 
	LCD_WR_DATA8(0x22); 
	LCD_WR_DATA8(0x1f); 
	LCD_WR_DATA8(0x1b); 
	LCD_WR_DATA8(0x23); 
	LCD_WR_DATA8(0x37); 
	LCD_WR_DATA8(0x00); 
	LCD_WR_DATA8(0x07); 
	LCD_WR_DATA8(0x02); 
	LCD_WR_DATA8(0x10); 
	
	LCD_WR_REG(0xe1); 
	LCD_WR_DATA8(0x0f); 
	LCD_WR_DATA8(0x1b); 
	LCD_WR_DATA8(0x0f); 
	LCD_WR_DATA8(0x17); 
	LCD_WR_DATA8(0x33); 
	LCD_WR_DATA8(0x2c); 
	LCD_WR_DATA8(0x29); 
	LCD_WR_DATA8(0x2e); 
	LCD_WR_DATA8(0x30); 
	LCD_WR_DATA8(0x30); 
	LCD_WR_DATA8(0x39); 
	LCD_WR_DATA8(0x3f); 
	LCD_WR_DATA8(0x00); 
	LCD_WR_DATA8(0x07); 
	LCD_WR_DATA8(0x03); 
	LCD_WR_DATA8(0x10);  
	
	LCD_WR_REG(0x2a);
	LCD_WR_DATA8(0x00);
	LCD_WR_DATA8(0x00);
	LCD_WR_DATA8(0x00);
	LCD_WR_DATA8(0x7f);
	
	LCD_WR_REG(0x2b);
	LCD_WR_DATA8(0x00);
	LCD_WR_DATA8(0x00);
	LCD_WR_DATA8(0x00);
	LCD_WR_DATA8(0x9f);
	
	LCD_WR_REG(0xF0); //Enable test command  
	LCD_WR_DATA8(0x01); 
	LCD_WR_REG(0xF6); //Disable ram power save mode 
	LCD_WR_DATA8(0x00); 
	
	LCD_WR_REG(0x3A); //65k mode 
	LCD_WR_DATA8(0x05); 
	
	
	LCD_WR_REG(0x29);//Display on	 
	LCD_Clear(BLACK);
	time_delay_ms (40);
 

 //time_delay_ms(10000);

//// LCD_Fill(0, 0, LCD_W-1, 16-1, LGRAY);
// Feed_IWDG();//η��
// Logo_display();
// LCD_Fill(0, 220, 240, 240, LGRAY);
//// battery_frame_display(CONTROLLER);

//// LCD_Fill(LCD_W-10, 6, LCD_W-6-1, 6+6, BLUE);
// LCD_Fill(0, 24, LCD_W, 87, BLACK);
 //Logo_display();
 //LCD_Fill(0, 134, LCD_W, LCD_H-1, BLACK);
 
 //Oled_EnlPrint(2, 5, "Finding AB point","�����Զ�Ѱ��A B ��", CHINESE);
// time_delay_ms(5000);

}  

//��������
//Color:Ҫ���������ɫ
void LCD_Clear(u16 Color)
{
	u16 i,j;  	
	Address_set(0,0,LCD_W+1,LCD_H+3);
    for(i=0;i<LCD_W+1;i++)
	 {
		Feed_IWDG();//η��
	  for (j=0;j<LCD_H+3;j++)
	   	{
        	LCD_WR_DATA(Color);	 			 
	    }

	  }
}



//��ָ��λ����ʾһ������(32*33��С)
//dcolorΪ������ɫ��gbcolorΪ������ɫ
/*void showhanzi(unsigned int x,unsigned int y,unsigned char index)	
{  
	unsigned char i,j;
	unsigned char *temp=hanzi;    
    Address_set(x,y,x+31,y+31); //��������      
	temp+=index*128;	
	for(j=0;j<128;j++)
	{
		for(i=0;i<8;i++)
		{ 		     
		 	if((*temp&(1<<i))!=0)
			{
				LCD_WR_DATA(POINT_COLOR);
			} 
			else
			{
				LCD_WR_DATA(BACK_COLOR);
			}   
		}
		temp++;
	 }
}*/
//����
//POINT_COLOR:�˵����ɫ
void LCD_DrawPoint(u16 x,u16 y)
{
	Address_set(x,y,x,y);//���ù��λ�� 
	LCD_WR_DATA(POINT_COLOR); 	    
} 	 
//��һ�����
//POINT_COLOR:�˵����ɫ
void LCD_DrawPoint_big(u16 x,u16 y)
{
	LCD_Fill(x-1,y-1,x+1,y+1,POINT_COLOR);
} 
//��ָ�����������ָ����ɫ
//�����С:
//  (xend-xsta)*(yend-ysta)
void LCD_Fill(u16 xsta,u16 ysta,u16 xend,u16 yend,u16 color)
{          
	u16 i,j; 

	if(xsta >= LCD_W)return;
	if(ysta >= LCD_H)return;
	if(xsta == xend)return;
	if(ysta == yend)return;

	if(xend < xsta)return;
	if(yend < ysta)return;
	
	Address_set(xsta,ysta,xend,yend);      //���ù��λ�� 
	for(i=ysta;i<=yend;i++)
	{													   	 	
		for(j=xsta;j<=xend;j++)LCD_WR_DATA(color);//���ù��λ�� 	    
	} 					  	    
}  
//����
//x1,y1:�������
//x2,y2:�յ�����  
void LCD_DrawLine(u16 x1, u16 y1, u16 x2, u16 y2)
{
	u16 t; 
	int xerr=0,yerr=0,delta_x,delta_y,distance; 
	int incx,incy,uRow,uCol; 

	delta_x=x2-x1; //������������ 
	delta_y=y2-y1; 
	uRow=x1; 
	uCol=y1; 
	if(delta_x>0)incx=1; //���õ������� 
	else if(delta_x==0)incx=0;//��ֱ�� 
	else {incx=-1;delta_x=-delta_x;} 
	if(delta_y>0)incy=1; 
	else if(delta_y==0)incy=0;//ˮƽ�� 
	else{incy=-1;delta_y=-delta_y;} 
	if( delta_x>delta_y)distance=delta_x; //ѡȡ�������������� 
	else distance=delta_y; 
	for(t=0;t<=distance+1;t++ )//������� 
	{  
		LCD_DrawPoint(uRow,uCol);//���� 
		xerr+=delta_x ; 
		yerr+=delta_y ; 
		if(xerr>distance) 
		{ 
			xerr-=distance; 
			uRow+=incx; 
		} 
		if(yerr>distance) 
		{ 
			yerr-=distance; 
			uCol+=incy; 
		} 
	}  
}    
//������
void LCD_DrawRectangle(u16 x1, u16 y1, u16 x2, u16 y2)
{
	LCD_DrawLine(x1,y1,x2,y1);
	LCD_DrawLine(x1,y1,x1,y2);
	LCD_DrawLine(x1,y2,x2,y2);
	LCD_DrawLine(x2,y1,x2,y2);
}
//��ָ��λ�û�һ��ָ����С��Բ
//(x,y):���ĵ�
//r    :�뾶
void Draw_Circle(u16 x0,u16 y0,u8 r)
{
	int a,b;
	int di;
	a=0;b=r;	  
	di=3-(r<<1);             //�ж��¸���λ�õı�־
	while(a<=b)
	{
		LCD_DrawPoint(x0-b,y0-a);             //3           
		LCD_DrawPoint(x0+b,y0-a);             //0           
		LCD_DrawPoint(x0-a,y0+b);             //1       
		LCD_DrawPoint(x0-b,y0-a);             //7           
		LCD_DrawPoint(x0-a,y0-b);             //2             
		LCD_DrawPoint(x0+b,y0+a);             //4               
		LCD_DrawPoint(x0+a,y0-b);             //5
		LCD_DrawPoint(x0+a,y0+b);             //6 
		LCD_DrawPoint(x0-b,y0+a);             
		a++;
		//ʹ��Bresenham�㷨��Բ     
		if(di<0)di +=4*a+6;	  
		else
		{
			di+=10+4*(a-b);   
			b--;
		} 
		LCD_DrawPoint(x0+a,y0+b);
	}
} 
//��ָ��λ����ʾһ���ַ�

//num:Ҫ��ʾ���ַ�:" "--->"~"
//mode:���ӷ�ʽ(1)���Ƿǵ��ӷ�ʽ(0)
//��ָ��λ����ʾһ���ַ�

//num:Ҫ��ʾ���ַ�:" "--->"~"

//mode:���ӷ�ʽ(1)���Ƿǵ��ӷ�ʽ(0)
void LCD_ShowChar(u16 x,u16 y,u8 num,u8 mode)
{
    u8 temp;
    u8 pos,t;
	u16 x0=x;
	u16 colortemp=POINT_COLOR;      
    if(x>LCD_W-16||y>LCD_H-16)return;	    
	//���ô���		   
	num=num-' ';//�õ�ƫ�ƺ��ֵ
	Address_set(x,y,x+8-1,y+16-1);      //���ù��λ�� 
	if(!mode) //�ǵ��ӷ�ʽ
	{
		for(pos=0;pos<16;pos++)
		{ 
			temp=F8X16[(u16)num*16+pos];		 //����1608����
			for(t=0;t<8;t++)
		    {                 
		        if(temp&0x01)POINT_COLOR=WHITE;
				else POINT_COLOR=BLACK;
				LCD_WR_DATA(POINT_COLOR);	
				temp>>=1; 
				x++;
		    }
			x=x0;
			y++;
		}	
	}else//���ӷ�ʽ
	{
		for(pos=0;pos<16;pos++)
		{
		    temp=F8X16[(u16)num*16+pos];		 //����1608����
			for(t=0;t<8;t++)
		    {                 
		        if(temp&0x01)LCD_DrawPoint(x+t,y+pos);//��һ����     
		        temp>>=1; 
		    }
		}
	}
	POINT_COLOR=colortemp;	    	   	 
}   
//m^n����
u32 mypow(u8 m,u8 n)
{
	u32 result=1;	 
	while(n--)result*=m;    
	return result;
}			 
//��ʾ2������
//x,y :�������	 
//len :���ֵ�λ��
//color:��ɫ
//num:��ֵ(0~4294967295);	
void LCD_ShowNum(u16 x,u16 y,u32 num,u8 len)
{         	
	u8 t,temp;
	u8 enshow=0;
	num=(u16)num;
	for(t=0;t<len;t++)
	{
		temp=(num/mypow(10,len-t-1))%10;
		if(enshow==0&&t<(len-1))
		{
			if(temp==0)
			{
				LCD_ShowChar(x+8*t,y,' ',0);
				continue;
			}else enshow=1; 
		 	 
		}
	 	LCD_ShowChar(x+8*t,y,temp+48,0); 
	}
} 
//��ʾ2������
//x,y:�������
//num:��ֵ(0~99);	 
void LCD_Show2Num(u16 x,u16 y,u16 num,u8 len)
{         	
	u8 t,temp;						   
	for(t=0;t<len;t++)
	{
		temp=(num/mypow(10,len-t-1))%10;
	 	LCD_ShowChar(x+8*t,y,temp+'0',0); 
	}
} 
//��ʾ�ַ���
//x,y:�������  
//*p:�ַ�����ʼ��ַ
//��16����
void LCD_ShowString(u16 x,u16 y,const u8 *p)
{         
    while(*p!='\0')
    {       
        if(x>LCD_W-16){x=0;y+=16;}
        if(y>LCD_H-16){y=x=0;LCD_Clear(RED);}
        LCD_ShowChar(x,y,*p,0);
        x+=8;
        p++;
    }  
}

//��������ַ���
void LCD_P16x16Str(uint8_t x,uint8_t y,uint8_t *ch,const ZHI_MATRIX *strp)
{
	uint8_t wm=0,ii = 0;
	//uint16_t adder=1; 
	uint8_t i, j,k;
	uint8_t index = 0;
	uint8_t matrix_idd=0;
	uint8_t temp= 0;

	if(wifi_id.language_sel == CHINESE)matrix_idd = MATRIX_ID;
//	else matrix_idd = JP_MAX_ID;
	
    if(x>LCD_W-ZHI_W||y>LCD_H-ZHI_H)return;	    

	while(*(ch+ii) != '\0')
	{
		for(i=0;i<matrix_idd;i++)
		{
			if(*(ch+ii) == *(strp[i].zhi+ii) )
			{
				if(*(ch+ii+1) == *(strp[i].zhi+ii+1) )
				{
					index = i;
					break;
				}
			}
		}
		
		Feed_IWDG();//η��
		Address_set(x,y,x+ZHI_W-1,y+ZHI_H-1);		//���ù��λ�� 
		
		for(wm = 0;wm < 24;wm++)               
  		{
  			temp = strp[index].matrix[wm];
			if((wm%2) == 0)
			{
				for(j=0; j< 8; j++)
				{
					if(temp & (0x01 << j))
					{
						LCD_WR_DATA(WHITE);	
					}
					else
					{
						if(Inverse_Dis)LCD_WR_DATA(BLUE);		
						else LCD_WR_DATA(BLACK);	
					}
				}
			}
			else
			{
				for(k=0; k<4; k++)
				{
					if(temp & (0x01 << k))
					{
						LCD_WR_DATA(WHITE);
					}
					else
					{
						if(Inverse_Dis)LCD_WR_DATA(BLUE);		
						else LCD_WR_DATA(BLACK);	
					}
				}
			}
			
  		}
  		
	/*	LCD_Set_Pos(x,y + 1); 

		for(wm = 16;wm < 32;wm++)               
  		{
  			if(Inverse_Dis)Oled_Write_Data(~strp[index].matrix[wm]);	
  			else Oled_Write_Data(strp[index].matrix[wm]);	
  			//adder += 1;
  		}
*/
		ii +=2;
	}
}


//==============================================================
//��������LCD_P8x16Str(u8 x,u8 y,u8 *p)
//����������д��һ���׼ASCII�ַ���
//��������ʾ��λ�ã�x,y����yΪҳ��Χ0��63��Ҫ��ʾ���ַ���
//���أ���
//==============================================================  
void LCD_P8x16Str(uint8_t x,uint8_t y,uint8_t *ch,const uint8_t *F8x16)
{
  uint8_t c=0,i=0,j=0, k;
  uint8_t temp = 0;
        
  while (*(ch+j)!='\0')
  {    
  	Feed_IWDG();
	
    c =*(ch+j)-32;
	
    if(x>LCD_W-CHAR_W||y>LCD_H-ZHI_H)return;	    
	
	Address_set(x,y,x+CHAR_W-1,y+ZHI_H-1);      //���ù��λ�� 
	
  	for(i=0;i<12;i++)
	{
		temp = *(F8x16+c*12+i);

		//if( (i%2) == 0)
		{
			for(k=0; k<6; k++)
			{
				if(temp & (0x01 << k))
				{
					LCD_WR_DATA(WHITE);
				}
				else
				{
					if(Inverse_Dis)LCD_WR_DATA(BLUE);		
					else LCD_WR_DATA(BLACK);	
				}
			}
		}
		
	}
	
/*  	LCD_Set_Pos(x,y+1);  
	
  	for(i=8;i<16;i++)
	{
		if(Inverse_Dis)Oled_Write_Data(~(*(F8x16+c*16+i) ));
		else Oled_Write_Data(*(F8x16+c*16+i));
	}
	*/
  	x+=6;
  	j++;
  }
}

// �����Ҵ��ϵ��µ�ɨ�跽����ʾ
void Oled_EnlPrint(uint8_t x, uint8_t y, uint8_t *en,uint8_t *ch, uint8_t language)
{
	uint8_t ch2[3];
	uint8_t en2[2];
	uint8_t ii=0;   
	
	y = y * (ZHI_H + ROWLEDGE) + TITLE; // 3: �м��; ZHI_H: ÿ���ֵĸ߶�; 16 : ��ʼ��ʾ����ʼ��ַ

	if(language == CHINESE/* || language == JAPANESE*/)
	{
		while(*(ch+ii) != '\0')
		{
		//	if(ii+16 > 127)ii=128-16;
			
			Feed_IWDG();
			
			if(x>LCD_W-ZHI_W){x=0;y+=ZHI_H;}
			if(y>LCD_H-ZHI_H){y=x=0;LCD_Clear(RED);}
			ch2[0] = *(ch+ii);
	 		ch2[1] = *(ch+ii+1);
			ch2[2] = '\0';			//����Ϊ�����ֽ�
			LCD_P16x16Str(x , y, ch2, zhi_f16x16);	//��ʾ����
			//else if(language == JAPANESE)LCD_P16x16Str(x , y, ch2, JPA_f16x16);
			x += ZHI_W;
			ii += 2;
		}
	}
	else if(language == ENGLISH || language == NELANDS || language == ITALIANO)
	{
		while(*(en+ii) != '\0')
		{
			//if(ii > 127)ii=127;
			
			Feed_IWDG();
		
			if(x>LCD_W-CHAR_W){x=0;y+=ZHI_H;}
			if(y>LCD_H-ZHI_H){y=x=0;LCD_Clear(RED);}
			en2[0] = *(en+ii);	
			en2[1] = '\0';			//��ĸռһ���ֽ�
			LCD_P8x16Str(x , y ,en2,F8X16);	//��ʾ��ĸ
			x += CHAR_W;
			ii+= 1;
		}
	}		
}

void oled_print_page(uint8_t cur)
{
	//TFTDis_Selet(cur);
	page_display(cur);
}


void oled_print_screen(void)
{
	uint8_t i;

	if(page_id < PAGE_MAX)
	{
		if(page_id <= 1)
		{
			for(i=0;i<8;i++)
			{
				//TFTDis_Selet(i);
				page_display(i);
			}
		}
		else 
		{
			for(i=0;i<8;i++)
			{
				//TFTDis_Selet(i);
				page_display(i);
				param_display(i);
			}
		}
	}

 }



void battery_init(void)
{
	if(return_powers())return;

	LCD_Fill(0, 40, LCD_W-1, 135, BLACK);

	 LCD_Fill(0, 0, LCD_W-1, 16-1, LGRAY);
	// LCD_Fill(0, 220, 240, 240, LGRAY);
	 battery_frame_display(CONTROLLER);
	 LCD_Fill(LCD_W-10, 6, LCD_W-6-1, 6+5, LIGHTGREEN);
	 The_motor_battery_dis_init();
	 change_page();
	//time_delay_ms(6000);
}

 // logo: COMAN
 void Logo_display(void)
 {
	 uint8_t j,i, k;	 
	 uint16_t temp = 0;
	 uint16_t indxe = 0;
	 
	 LCD_Fill(0, 24, 240, 87, BLACK);
	 Address_set(24,51,24+80-1,51+33-1);	  //���ù��λ�� 
	 
	 for(j=0;j<20;j++)
	 {
		// Feed_IWDG();//η��
		indxe = j*16;
		
		 for(i=0; i<16; i++)
		 {
			 temp = LOGO720YUN[indxe + i];
			 
			 for(k=0; k<8; k++)
			 {
				 if(temp & (0x01 << k))
				 {
					 LCD_WR_DATA(WHITE);
				 }
				 else
				 {
					 LCD_WR_DATA(BLACK);	 
				 }
			 }
		 }
			 
	 }

	Address_set(16,51+33,16+96-1,51+33+21-1); 	 //���ù��λ�� 
	for(j=0;j<16;j++)
	{
	   // Feed_IWDG();//η��
	   indxe = j*16;
	   
		for(i=0; i<16; i++)
		{
			temp = Biaoyu[indxe + i];
			
			for(k=0; k<8; k++)
			{
				if(temp & (0x01 << k))
				{
					LCD_WR_DATA(WHITE);
				}
				else
				{
					LCD_WR_DATA(BLACK); 	
				}
			}
		}
			
	}

	//LCD_Fill(0, 134, 240, 240-1, BLACK);
	indxe = Check_String("LG-06", ENGLISH);
	indxe = (LCD_W - indxe) / 2;
	Oled_EnlPrint(indxe, 5, "LG-06","",ENGLISH);
	time_delay_ms(3000);
	LCD_Fill(0, 24, 240, 87, BLACK);
 }


 void battery_frame_display(DEVICE_P type)
 {
	 uint8_t i,j,k;
	 uint8_t temp = 0;
	 uint16_t index = 0;
	 
	 if(type == CONTROLLER)Address_set(X_START,Y_START,X_END,Y_END+2);	 //���ù��λ�� 
	 else if(type == MOTOR)Address_set(M_X_START,Y_START,M_X_END,Y_END+2);	 //���ù��λ�� 
 
	 for(j=0; j<14; j++)
	 {
		 Feed_IWDG();//η��
		 index = j*3;
		 
		 for(i=0; i<3; i++)
		 {
			 temp = battery[index + i];
			 
			 for(k=0; k<8; k++)
			 {
				 if(temp & (0x01 << k))
				 {
					 LCD_WR_DATA(LGRAY);
				 }
				 else
				 {
					 LCD_WR_DATA(BLACK);	 
				 }
			 }
		 }
	 }
	 
 }

void battery_display(uint8_t id, DEVICE_P type)
{
	uint8_t xsta = 0, xend = 0;
//	uint8_t ysta = 0, yend = 0;
	uint8_t temp = 0;
	uint32_t colour = 0;

	if(id > BATERRY_SIZE)return;

	if(type == CONTROLLER)
	{
		xsta = X_START+1;
		colour = battery_colour;
	}
	else if(type == MOTOR)
	{
		xsta = M_X_START+1;
		colour = m_battery_colour;
	}
	
	xend = xsta +id -1;
	
	LCD_Fill(xsta, Y_START+3, xend, Y_END, colour);
	
	temp = BATERRY_SIZE - id;
	xsta = xsta + id;
	xend = xsta + temp -1;
	
	if(type == CONTROLLER)LCD_Fill(xsta, Y_START+3, xend, Y_END , BLACK);
	else if(type == MOTOR)LCD_Fill(xsta, Y_START+3, xend, Y_END , BLACK);

	
}


void Charging_Dis(void)
{
	static uint8_t iiii;
	uint8_t temp = 0;
//	uint16_t index = 0;
	if(Battery_percentage >= 100)return;
	if(return_powers()) return;
	iiii += 10;
	temp = iiii;
	if(temp > 100)
	{
		temp = 10;
		iiii = 10;
	}
	battery_display(temp*BATERRY_SIZE/100, CONTROLLER);
}

// �ٷֱ�
void battery_data_display(float  data, uint8_t type, DEVICE_P pct)
{
	uint8_t buff[10];
	uint8_t x_size = 0;
	uint8_t temp = 0;
	uint8_t c=0,i=0,j=0, k;
	uint8_t index = 0;
//	float value = 0;
//	float basic_bat = 0;
	uint8_t zhi_sizes = 0;
	uint32_t color = 0;
	if(return_powers()) return;

	if(pct == CONTROLLER)
	{
		/*if(pct == CONTROLLER)basic_bat = 3.3;
		
		if(type == 1)
		{
			value = 1.793;
		}
		else if(type == 0)
		{
			if(pct == CONTROLLER)value = BATTER_FZ;
		}
		
		if(data >= basic_bat)
		{
			data = data - basic_bat;
		
			if(data > value)
			{
				Battery_percentage = 100;
			}
			else
			{
				Battery_percentage = data / value * 100;

				if(Battery_percentage < 1)Battery_percentage = 1;
			}
		}
		else
		{
			Battery_percentage = 1;
		}*/

		if(Battery_percentage <= 10)
		{
			battery_colour = RED;
		}
		else
		{
			battery_colour = LIGHTGREEN;
		}
		color = battery_colour;
		temp = Battery_percentage;
	}
	else // motor
	{
		temp = data;
		if(temp <= 10)
		{
			m_battery_colour = RED;
		}
		else
		{
			m_battery_colour = LIGHTGREEN;
		}

		color = m_battery_colour;
	}
	
	if((CHARGE_STATUS == N_CHARGE)/* && (Battery_percentage<100)*/)
	{
		battery_display(temp*BATERRY_SIZE/100, pct);
	}
	else
	{
		if(pct == MOTOR)
		{
			battery_display(temp*BATERRY_SIZE/100, pct);
		}
		else
		{
			if((Battery_percentage>=100))
			{
				battery_display(temp*BATERRY_SIZE/100, pct);
			}
		}
	}
	

	
	if(temp < 10)
	{
		sprintf( (char *)buff,"  %d%%", temp);
	}
	else if(temp<100)
	{
		sprintf( (char *)buff," %d%%", temp);
	}
	else 
	{
		sprintf( (char *)buff,"%d%%", temp);
	}
	
	zhi_sizes = Check_String(buff, ENGLISH);

	if(type == 1) //���ͼ����ʾ
	{
		x_size = LCD_W - 30 - 6 - 24 -4 - 48; // 36: �ٷֱ���ʾ���; 24 ���ͼ����; 32: �����ʾ���
	}
	else if(type == 0) // �����ػ�����������ʾ
	{
		
		if(pct == CONTROLLER)x_size = X_START-WORD_SPACE - zhi_sizes; // 31=30+1: 30�����ʾ��+1�Ǽ��; 24 �ٷֱȿ�; 
		else if(pct == MOTOR)x_size = M_X_START-(CHAR_W*4+WORD_SPACE);
	}
	
//	Oled_EnlPrint(x_size, 2, buff, "", ENGLISH);
	  while (buff[+j] !='\0')
	  {    
		Feed_IWDG();
		
		c = buff[+j]-32;
		
	//	if(x>LCD_W-12||y>LCD_H-24)return;		
	Address_set(x_size,Y_START,x_size+CHAR_W-1,ZHI_H-1);      //���ù��λ�� 
		
		for(i=0;i<12;i++)
		{
			index = *(F8X16+c*12+i);
	
			//if( (i%2) == 0)
			{
				for(k=0; k<6; k++)
				{
					if(index & (0x01 << k))
					{
						LCD_WR_DATA(color);
					}
					else
					{
					LCD_WR_DATA(LGRAY);	
					}
				}
			}
			/*else
			{
				for(k=0; k<4; k++)
				{
					if(index & (0x01 << k))
					{
						LCD_WR_DATA(battery_colour);
					}
					else
					{
						LCD_WR_DATA(LGRAY);	
					}
				}
			}*/
		}
		
		x_size+=6;
		j++;
	  }

	
/*	if(data < 10)sprintf( (char *)buff,"%0.2fV ", data);
	else sprintf( (char *)buff,"%0.2fV", data);
	
	//oled_display_6x8str(71, 0, buff);
	Oled_EnlPrint(72, 4, buff, "", ENGLISH);*/
}



void The_motor_battery_dis_init(void)
{
	battery_data_display(1, 0, MOTOR);
	battery_frame_display(MOTOR);
	LCD_Fill(47, 6, 50, 6 + 5 , LIGHTGREEN);
}


void motor_battery_dis(float bat)
{
	battery_data_display(bat, 0, MOTOR);
}

void bornd_code_dis(uint8_t row, uint8_t codeID)
{
	if(wifi_id.language_sel == CHINESE)
	{
		if(codeID == 1){
			The_page_processing(row, "������ֵ          ");}
		else if(codeID == 2){
			The_page_processing(row, "������ֵ���      ");}
		else if(codeID == 3){
			The_page_processing(row, "������            ");}
		else if(codeID == 4){
			The_page_processing(row, "�������          ");}
		else if(codeID == 5){
			The_page_processing(row, "���볬ʱ          ");}
		else if(codeID == 6){
			The_page_processing(row, "�����Զ�Ѱ��A B ��");}
		else if(codeID == 7){
			The_page_processing(row, "����AB��ʧ��");
			The_page_processing(row+1, "����һ�������  ��");
			Oled_EnlPrint(96, row+1, "1","", ENGLISH);
			}
	}
	else if(wifi_id.language_sel == ENGLISH)
	{
		if(codeID == 1){
			The_page_processing(row, "Set Code          ");}
		else if(codeID == 2){
			The_page_processing(row, "Code Successful   ");}
		else if(codeID == 3){
			The_page_processing(row, "Pairing...        ");}
		else if(codeID == 4){
			The_page_processing(row, "Pair OK           ");} 
		else if(codeID == 5){
			The_page_processing(row, "Pairing over      ");} //���볬ʱ
		else if(codeID == 6){
			The_page_processing(row, "Finding AB point  ");}
		else if(codeID == 7){
			The_page_processing(row, "Failed to set A/B "); // ����AB��ʧ�� 
			The_page_processing(row+1, "point             ");} // ����AB��ʧ�� 
	}
/*	else if(wifi_id.language_sel == NELANDS)
	{
		if(codeID == 1){
			The_page_processing(row, "Set Code          ");}
		else if(codeID == 2){
			The_page_processing(row, "Code Successful   ");}
		else if(codeID == 3){
			The_page_processing(row, "Koppelen         ");}
		else if(codeID == 4){
			The_page_processing(row, "Gekoppeld        ");}
		else if(codeID == 5){
			The_page_processing(row, "Koppelen mislukt");}
		else if(codeID == 6){
			The_page_processing(row, "Zoeken naar punten");
			The_page_processing(row+1, "A / B");}
		else if(codeID == 7){
			The_page_processing(row, "Kan A/B-punt niet   ");
			The_page_processing(row+1, "instellen           ");}
	}
	else if(wifi_id.language_sel == SPANISH)
	{
		if(codeID == 1){
			The_page_processing(row, "Set Code          ");}
		else if(codeID == 2){
			The_page_processing(row, "Code Successful   ");}
		else if(codeID == 3){
			The_page_processing(row, "Emparejamiento");}
		else if(codeID == 4){
			The_page_processing(row, "Emparejado    ");}
		else if(codeID == 5){
			The_page_processing(row, "No se pudo emparejar");}
		else if(codeID == 6){
			The_page_processing(row, "Buscando puntos A/B");}
		else if(codeID == 7){
			The_page_processing(row, "No se pudo          ");
			The_page_processing(row+1, "establecer el punto ");
			The_page_processing(row+2, " A / B              ");}
	}	
	else if(wifi_id.language_sel == FRENCH)
	{
		if(codeID == 1){
			The_page_processing(row, "Set Code          ");}
		else if(codeID == 2){
			The_page_processing(row, "Code Successful   ");}
		else if(codeID == 3){
			The_page_processing(row, "Jumelage en cours ");}
		else if(codeID == 4){
			The_page_processing(row, "Jumel             ");
			Oled_EnlPrint(34, row,"��","", wifi_id.language_sel);} //
		else if(codeID == 5){
			The_page_processing(row, "Jumelage impossible");}
		else if(codeID == 6){
			The_page_processing(row, "Recherche du point  ");
			The_page_processing(row+1, "A / B             ");}
		else if(codeID == 7){
			The_page_processing(row, " chec du r glage du ");
			Oled_EnlPrint(10, row,"��","", wifi_id.language_sel); //
			Oled_EnlPrint(64, row,"��","", wifi_id.language_sel); //
			The_page_processing(row+1, "point A/B         ");}
	}
	else if(wifi_id.language_sel == GERMAN)
	{
		if(codeID == 1){
			The_page_processing(row, "Set Code          ");}
		else if(codeID == 2){
			The_page_processing(row, "Code Successful   ");}
		else if(codeID == 3){
			The_page_processing(row, "Paarung...        ");}
		else if(codeID == 4){
			The_page_processing(row, "Gepaart           ");}
		else if(codeID == 5){
			The_page_processing(row, "Fehler beim Koppeln");}
		else if(codeID == 6){
			The_page_processing(row, "Nach Punkten A / B ");
			The_page_processing(row+1, "suchen           ");}
		else if(codeID == 7){
			The_page_processing(row, "A/B-Punkt konnte   ");
			
			The_page_processing(row+1, "nicht festgelegt   ");	
			The_page_processing(row+2, "werden             ");	}
	}
	else if(wifi_id.language_sel == JAPANESE)
	{
		if(codeID == 1){
			The_page_processing(row, "�O��          ");}
		else if(codeID == 2){
			The_page_processing(row, "�O��");
			Oled_EnlPrint(28, row,"OK","", ENGLISH);} //
		else if(codeID == 3){
			The_page_processing(row, "�ڥ������");}
		else if(codeID == 4){
			The_page_processing(row, "�ڥ���󥰤���");}
		else if(codeID == 5){
			The_page_processing(row, "�ڥ����ʧ��");}
		else if(codeID == 6){
			The_page_processing(row, "A B ��������");}
		else if(codeID == 7){}
			//The_page_processing(row, "Set ab point fail ");}
	}		
	else if(wifi_id.language_sel == ITALIANO)
	{
		if(codeID == 1){
			The_page_processing(row, "Set Code          ");}
		else if(codeID == 2){
			The_page_processing(row, "Code Successful   ");}
		else if(codeID == 3){
			The_page_processing(row, "Abbinamento        ");}
		else if(codeID == 4){
			The_page_processing(row, "Abbinato           ");}
		else if(codeID == 5){
			The_page_processing(row, "Abbinamento fallito");}
		else if(codeID == 6){
			The_page_processing(row, "Ricerca dei punti AB");}
		else if(codeID == 7){
			The_page_processing(row, "Impostazione punto  ");
			The_page_processing(row+1, "A/B fallita         ");}
	}*/
}

// �ػ������ʾ����״̬
void TurnOff_ShutDonw_Battery_dis(uint8_t type, float datat, uint8_t bbb)
{
	static uint8_t pp = 0;
	uint8_t i;
//	float data;
//	uint8_t temp;
	uint8_t buff[6] = {0};
	uint8_t zhi_sizes = 0;
	if(return_powers()==0)return;
	//LCD_Fill(0, 24, 240, 87, BLACK);
	if(Battery_percentage >= 100)
	{
		LCD_Fill(42,64+3,86,64+25, LIGHTGREEN);
		
		sprintf( (char *)buff,"%d%%", Battery_percentage);
		
		zhi_sizes = Check_String(buff, ENGLISH);
		zhi_sizes = (LCD_W-zhi_sizes)/2;
		Oled_EnlPrint(zhi_sizes, 3, buff, "", ENGLISH);
		return;
	}
	
	if(wifi_id.wakaup_tag == 0)
	{
		if(CHARGE_STATUS == CHARGING)
		{
			if(bbb)pp=0;
			
			if(type)
			{
				LCD_Fill(40,64,40+48,64+1, LGRAY);

				for(i=0; i<26; i++)
				{
					Address_set(40,65+i,40+1,64+1);	 //���ù��λ�� 
					LCD_WR_DATA(LGRAY);
					Address_set(40+47,65+i,40+48,64+1);	 //���ù��λ�� 
					LCD_WR_DATA(LGRAY);
				}
				LCD_Fill(40,64+27,40+48,64+28, LGRAY);
				LCD_Fill(40+48,64+7,40+48+3,64+7+14, LGRAY);

			}
			else
			{
				pp += 3;
				if(pp > 42)
				{
					pp = 3;
					
					LCD_Fill(42,64+3,42+42,64+25, BLACK);
					//return;
				}
				i=39+pp;
				LCD_Fill(i,64+3,i+3,64+25, LIGHTGREEN);

				{
					sprintf( (char *)buff,"%d%%", Battery_percentage);
				}
				
				zhi_sizes = Check_String(buff, ENGLISH);
				zhi_sizes = (LCD_W-zhi_sizes)/2;
				Oled_EnlPrint(zhi_sizes, 3, buff, "", ENGLISH);
			}
		}
	}
}

void LCD_shi_fen_miao_display(uint8_t x,uint8_t y,uint8_t *ch)
{
  uint8_t c=0,i=0,j=0, k;
  uint8_t temp = 0;
        
y = y * (ZHI_H + ROWLEDGE) + TITLE; // 3: �м��; ZHI_H: ÿ���ֵĸ߶�; 16 : ��ʼ��ʾ����ʼ��ַ
  while (*(ch+j)!='\0')
  {    
  	Feed_IWDG();
	
    c =*(ch+j)-32;
	
    if(x>LCD_W-CHAR_W||y>LCD_H-ZHI_H)return;	    
	
	Address_set(x,y,x+CHAR_W-1,y+ZHI_H-1);      //���ù��λ�� 
	
  	for(i=0;i<12;i++)
	{
		temp = *(F8X16+c*12+i);

		//if( (i%2) == 0)
		{
			for(k=0; k<6; k++)
			{
				if(temp & (0x01 << k))
				{
					LCD_WR_DATA(RED);
				}
				else
				{
					if(Inverse_Dis)LCD_WR_DATA(BLUE);		
					else LCD_WR_DATA(BLACK);	
				}
			}
		}
		
	}
	
/*  	LCD_Set_Pos(x,y+1);  
	
  	for(i=8;i<16;i++)
	{
		if(Inverse_Dis)Oled_Write_Data(~(*(F8x16+c*16+i) ));
		else Oled_Write_Data(*(F8x16+c*16+i));
	}
	*/
  	x+=6;
  	j++;
  }
}

void version_dis(void)
{
	Oled_EnlPrint(LCD_W-42, 5, "V1.1.4", "", ENGLISH);
}









