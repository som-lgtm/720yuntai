
#include "common.h"
#include "oled.h"
#include "orbital.h"
#include "keyscan.h"
#include "stdio.h"
#include "uart.h"
#include "string.h"
#include "stdio.h"


SPECIALTY_PARA spe_para[8]={0};
uint8_t id_add=0;
uint8_t id_add_back=0; // Ҫ���ĵĲ�����
uint8_t mode_id = 0;
uint8_t mid_cr_back = 0; // ��ǰģʽ��
uint8_t mid_cr = 0; // ��ǰģʽ��
uint8_t pid_cr = 0; // ��ǰ������
uint8_t fanye = 0; // ҳ���ר��0Ϊ��ҳ��1Ϊ�ڶ�ҳ
uint16_t enter_time = 0;
uint8_t Reshot = 0;
uint8_t sp_start = 0; // רҵģʽ��������ȫ������

typedef struct{
	short int ele_angle; // ����
	uint16_t lev_angle; //ˮƽ��
	uint8_t amout;
	uint8_t exposure;
	uint8_t interval;
}SPECIALTY_LINP;

SPECIALTY_LINP sp_lip={0};

void splty_mode_time_count(void)
{
	if(enter_time)enter_time--;
}

void Set_enter_time(void)
{
	enter_time = 6000;
}

void specialty_para_clear(void)
{
	fanye = 0;
}

void specialty_mode_para_clear(void)
{
	memset((void *)&spe_para, 0, sizeof(spe_para));
	 mode_id = 0;
	mid_cr_back = 0; // ��ǰģʽ��
	mid_cr = 0; // ��ǰģʽ��
	pid_cr = 0; // ��ǰ������
	fanye = 0; // ҳ���ר��0Ϊ��ҳ��1Ϊ�ڶ�ҳ
	enter_time = 0;
}


//��ʾ�Ӻŵ�ɨ�������
void specialty_jiaHao_scan(uint8_t *srtp)
{
	uint8_t i,j,k;
	uint8_t temp= 0;
	
	for(i = 0;i< 24;i++)			   
	{
		temp = srtp[i];
		if((i%2) == 0)
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
}

// �Ӻŵ���ʾ��ͳһ��ʽ
void specialty_jiaHao_display(void)
{
	uint8_t x=0;
	uint8_t y=0;
//	uint8_t i;
//	uint8_t temp = 0;
	uint8_t indxe = 0;
	uint8_t t_kong[]={
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	};
	uint8_t jiahao[]={
		0x60,0x00,0x60,0x00,0x60,0x00,0x60,0x00,0x60,0x00,0xFF,0x0F,0xFF,0x0F,0x60,0x00,
		0x60,0x00,0x60,0x00,0x60,0x00,0x60,0x00,
	};
	
	if(page_id == PREINSTALL_MODE)
	{
		y = mode_id+1;
		if(mode_id>=7)return;
	}
	else if(page_id == PREINSTALL_SET)
	{
		if(spe_para[mid_cr].para_id >= (fanye*6+6))return; // ��ǰҳ���в�����������ʾ�Ӻ�

		y = spe_para[mid_cr].para_id+1;
		indxe = y;
		
		if(y > 6) //���ڵ���6�󣬷�ҳ��Ҫ��1�п�ʼ��ʾ
		{
			y = y - (fanye*6) ; //
		}
	}
	x = Check_String("  ��  ", CHINESE);
	x = (LCD_W - x) / 2;
	
//	if(y<7)Oled_EnlPrint(x, y, "  ��  ","  ��  ",CHINESE);
	if(y>7)return;
	if(indxe == cursor_id)
	{
		inverse_get_value(0xff); //�ӹ��
	}
	y = y * (ZHI_H + ROWLEDGE) + TITLE; // 3: �м��; ZHI_H: ÿ���ֵĸ߶�; 16 : ��ʼ��ʾ����ʼ��ַ
	Address_set(x,y,x+12-1,y+ZHI_H-1);		//���ù��λ�� 
	specialty_jiaHao_scan(t_kong);

	Address_set(x+12,y,x+12+12-1,y+ZHI_H-1);		//���ù��λ�� 
	specialty_jiaHao_scan(jiahao);

	Address_set(x+12+12,y,x+12+12+12-1,y+ZHI_H-1);		//���ù��λ�� 
	specialty_jiaHao_scan(t_kong);
	inverse_get_value(0);

}

void specialty_cursor_id_mode_dis(uint8_t cur)
{
	uint8_t x=0;
	uint8_t x1=0;
	uint8_t fifosb[5]={0};
	
	
	if(wifi_id.language_sel == ENGLISH)
	{
		sprintf( (char *)fifosb,"MODE %d", cur);
		x = Check_String(fifosb, wifi_id.language_sel);
		x = (LCD_W - x) / 2;
		Oled_EnlPrint(x, cur, z_buffer,z_buffer,wifi_id.language_sel);
	}
	else if(wifi_id.language_sel == CHINESE)
	{
	//	if(cur == 1)
		{
			x = Check_String("Ԥ��  ", wifi_id.language_sel);
			
		}

		
		x = (LCD_W - x) / 2;
		Oled_EnlPrint(x, cur, z_buffer,z_buffer,wifi_id.language_sel);
		sprintf( (char *)fifosb,"%d", cur);
		//x1 = Check_String(fifosb, ENGLISH);
		x1 = x+30;
		Oled_EnlPrint(x1, cur, fifosb,z_buffer,ENGLISH);
	}
	
	
}

// רҵģʽ�µ�����ģʽ��ʾ
void specialty_page_mode_dis(void)
{
	uint8_t i=0;
	if(page_id != PREINSTALL_MODE)return;

	for(i=0; i<mode_id; i++)
	{
		if(cursor_id == (i+1))
		{
			inverse_get_value(0xff);
		}
		specialty_cursor_id_mode_dis(i+1);
		inverse_get_value(0);
	}

	if(cursor_id == (i+1))
	{
		inverse_get_value(0xff);
	}
	specialty_jiaHao_display();
	inverse_get_value(0);
}

// ��Ӳ����������ʾ
void specialty_cursor_id_para_dis(uint8_t cur)
{
	uint8_t x=0;
	uint8_t fifos[22]={0};
	uint16_t ele_an=0;
	uint16_t lve_ang=0;
	uint8_t mounts=0;
	uint8_t shuts=0;
	uint8_t inter=0;
	uint8_t sys_stopss=0;

	ele_an = spe_para[mid_cr].modes[cur].ele_angle;
	shuts = spe_para[mid_cr].modes[cur].exposure;
	mounts = spe_para[mid_cr].modes[cur].amout;
	lve_ang = spe_para[mid_cr].modes[cur].shut_times;
	inter = spe_para[mid_cr].modes[cur].interval;
	sys_stopss = spe_para[mid_cr].modes[cur].sys_stop_t;
	
	sprintf( (char *)fifos,"(%d)%d,%d,%d,%d,%d,%d", cur,ele_an, lve_ang, mounts, shuts, inter,sys_stopss);
	
	x = Check_String(fifos, ENGLISH);
	x = (LCD_W - x) / 2;

	if(cur > 6)cur -= fanye*6;
	if(cur == 0)return;
	Oled_EnlPrint(x, cur, fifos,z_buffer,ENGLISH);
	
}

// רҵģʽ�¶�Ӧģʽ�����в�����ʾ
void specialty_page_para_dis(void)
{
	uint8_t i=0;
	uint8_t cur_b =0;

/*	cur_b = fanye*6;
	for(i=cur_b; i<=spe_para[mid_cr].para_id;i++) // ÿһҳ6 ����Ч������ʾ 
	{
		if(i > (cur_b+6))return;
		if(cursor_id == i)inverse_get_value(0xff);
		specialty_cursor_id_para_dis(i);
		inverse_get_value(0);
	}*/
	
	if(fanye == 0)
	{
		if(spe_para[mid_cr].para_id >= 6)cur_b = 6;
		else cur_b = spe_para[mid_cr].para_id;
		//if(para_id <= 6)
		{
			for(i=0; i<cur_b; i++)
			{
				if(i > 5)break;
				if(cursor_id == (i+1))
				{
					inverse_get_value(0xff);
				}
				specialty_cursor_id_para_dis(i+1);
				inverse_get_value(0);
			}
			
			if(i < 6)
			{ 
				if(cursor_id == (i+1))
				{
					inverse_get_value(0xff);
				}
				specialty_jiaHao_display();
				inverse_get_value(0);
			}
		}
	}
	else if(fanye == 1)
	{
		if(spe_para[mid_cr].para_id > 6)
		{
			if(spe_para[mid_cr].para_id > 6)cur_b = spe_para[mid_cr].para_id-6;
			
			for(i=0; i<(cur_b); i++)
			{
				if(i > 5)break;
				if((cursor_id - 6) == (i+1))
				{
					inverse_get_value(0xff);
				}
				specialty_cursor_id_para_dis(i+1+6);
				inverse_get_value(0);
			}
			
			if(i < 6)
			{ 
				if((cursor_id - 6) == (i+1))
				{
					inverse_get_value(0xff);
				}
				specialty_jiaHao_display();
				inverse_get_value(0);
			}
		}
	}	
	else if(fanye == 2)
	{
		if(spe_para[mid_cr].para_id > 12)
		{
			if(spe_para[mid_cr].para_id > 12)cur_b = spe_para[mid_cr].para_id-12;
			
			for(i=0; i<(cur_b); i++)
			{
				if(i > 5)break;
				if((cursor_id - 12) == (i+1))
				{
					inverse_get_value(0xff);
				}
				specialty_cursor_id_para_dis(i+1+12);
				inverse_get_value(0);
			}
			
			if(i < 6)
			{ 
				if((cursor_id - 12) == (i+1))
				{
					inverse_get_value(0xff);
				}
				specialty_jiaHao_display();
				inverse_get_value(0);
			}
		}

	}	
	else if(fanye == 3)
	{
		if(spe_para[mid_cr].para_id > 18)
		{
			if(spe_para[mid_cr].para_id > 18)cur_b = spe_para[mid_cr].para_id-18;
			
			for(i=0; i<(cur_b); i++)
			{
				if(i > 5)break;
				if((cursor_id - 18) == (i+1))
				{
					inverse_get_value(0xff);
				}
				specialty_cursor_id_para_dis(i+1+18);
				inverse_get_value(0);
			}
			
			if(i < 6)
			{ 
				if((cursor_id - 18) == (i+1))
				{
					inverse_get_value(0xff);
				}
				specialty_jiaHao_display();
				inverse_get_value(0);
			}
		}

	}
	
	
}

void ReturnOrNext_dis(void)
{
	uint8_t x=0;
	uint8_t curs=0;
	
	if(wifi_id.language_sel == ENGLISH)
	{
		x = 104;
	}
	else if(wifi_id.language_sel == CHINESE)
	{
		x = 92;
	}

	if(spe_para[mid_cr].para_id >= PARA_MAXID)
	{
		curs = spe_para[mid_cr].para_id+1;
	}
	else
	{
		curs = spe_para[mid_cr].para_id+2;
	}

	if(cursor_id == curs+1)inverse_get_value(0xff);
	Oled_EnlPrint(4, 7, "Return","����",wifi_id.language_sel);
	inverse_get_value(0);
	
	if(cursor_id ==curs)
	{
		specialty_jiaHao_display();
		inverse_get_value(0xff);
	}
	Oled_EnlPrint(x, 7, "Next","��һҳ",wifi_id.language_sel);
	inverse_get_value(0);
}

void ConfirmOrCancel_dis(void)
{
	uint8_t x=0;
	if(wifi_id.language_sel == ENGLISH)
	{
		x = 86;
	}
	else if(wifi_id.language_sel == CHINESE)
	{
		x = 104;
	}

	if(cursor_id ==8)inverse_get_value(0xff);
	Oled_EnlPrint(4+6, 7, "Cancel","ȡ��",wifi_id.language_sel);
	inverse_get_value(0);
	if(cursor_id ==7)inverse_get_value(0xff);
	Oled_EnlPrint(x, 7, "Confirm","ȷ��",wifi_id.language_sel);
	inverse_get_value(0);
}

// רҵģʽ���н���ķ�����һҳ��Ҫ��ʾ�Ĳ���
void Page_preinstall_move_return(void)
{
	if(spe_para[mid_cr].para_id < 24)cursor_id = spe_para[mid_cr].para_id+2;
	else cursor_id = spe_para[mid_cr].para_id+1;
	fanye = spe_para[mid_cr].para_id / 6;
	if(fanye>=4)fanye=3;
	change_page();
	specialty_page_para_dis();
	ReturnOrNext_dis();
	jiantou_display();
}

void specialty_mode_OKkey(void)
{
	uint8_t temp = 0;
	
	if(page_id == PREINSTALL_MODE)
	{
		if(enter_time==0)
		{
			specialty_get =0;
		}
		if(specialty_get)return; //���ڽ�������
		page_id = PREINSTALL_SET;
		if(mode_id < cursor_id)mode_id +=1;
		//mode_id = cursor_id;
		mid_cr = cursor_id;
		mid_cr_back = cursor_id;
		cursor_id = 1;
		change_page();
		specialty_page_para_dis();
		ReturnOrNext_dis();
		jiantou_display();
		mode_and_para_sendt(1);
		//if(m_start == 0)id_add = 1;
		
	}
	else if(page_id == PREINSTALL_SET)
	{
		if(spe_para[mid_cr].para_id < PARA_MAXID) //�й��
		{
			temp = spe_para[mid_cr].para_id +2; // ��2��ʾ���ͣ����һҳ
		}
		else // �޹��
		{
			temp = spe_para[mid_cr].para_id + 1;  // ��1��ʾ���ͣ����һҳ
		}

		if(cursor_id == temp) // ��һҳ��ʾ
		{	
			if(spe_para[mid_cr].para_id)
			{
				page_id = PREINSTALL_MOVE;
				cursor_id = 3;
				change_page();
				specialty_totaol_time_dis();
			}
		}
		else if(cursor_id == temp + 1) // ������һҳ
		{
			page_id = PREINSTALL_MODE;
			cursor_id = 1;
			fanye = 0;
			change_page();
			specialty_page_mode_dis();
		}
		else
		{
			page_id = PREINSTALL_SET1;
			//para_id = cursor_id;
			if(cursor_id > spe_para[mid_cr].para_id)
			{
				//������һ�����������ݵ��¼ӵĲ����б���
				memcpy((void *)&spe_para[mid_cr].modes[cursor_id ].ele_angle, (void *)&spe_para[mid_cr].modes[spe_para[mid_cr].para_id].ele_angle, sizeof(spe_para[mid_cr].modes[spe_para[mid_cr].para_id])); // shift the data
			}
			if(spe_para[mid_cr].para_id == 0)
			{
				spe_para[mid_cr].modes[cursor_id].ele_angle = 360;
				spe_para[mid_cr].modes[cursor_id].shut_times = 1;
				spe_para[mid_cr].modes[cursor_id].exposure = 1;
				spe_para[mid_cr].modes[cursor_id].interval = 1;
				spe_para[mid_cr].modes[cursor_id].amout = 1;
				spe_para[mid_cr].modes[cursor_id].sys_stop_t = 1;
			}
			memcpy(&sp_lip, &spe_para[mid_cr].modes[cursor_id], sizeof(sp_lip)); // load data to DMA send buffer
			pid_cr = cursor_id;
			cursor_id = 1;
			// ����һ���������Ƶ���ǰ�Ĳ�����
			change_page();
			//specialty_page_para_dis();
			ConfirmOrCancel_dis();
		}
	}
	else if(page_id == PREINSTALL_SET1)
	{
		if(cursor_id == 7) // confirm
		{
			specialty_para_sendTo_motor(cursor_id);
			if(spe_para[mid_cr].para_id <pid_cr)
			{
				spe_para[mid_cr].para_id += 1;
				cursor_id = pid_cr+1;
			}
			else
			{
				cursor_id = pid_cr;
			}
			mode_and_para_sendt(0);
			page_id = PREINSTALL_SET;
			oled_print_screen();
			/*if(cursor_id > (fanye*6+6)) // ���·�ҳ
			{
				fanye += 1;
			}*/
			specialty_cursor_id_dis(0);
			specialty_page_para_dis();
			specialty_jiaHao_display();
			ReturnOrNext_dis();
		}
		else if(cursor_id == 8) // ȡ��
		{
			memcpy(&spe_para[mid_cr].modes[pid_cr], &sp_lip, sizeof(sp_lip)); // load data to DMA send buffer
	//		specialty_para_sendTo_motor(cursor_id);//��Ҫ��ȡ����˲�������ͬ��
			page_id = PREINSTALL_SET;
			//mode_id = cursor_id;
			cursor_id = pid_cr;
			change_page();
			specialty_page_para_dis();
			ReturnOrNext_dis();
			jiantou_display();
		}
	}
	else if(page_id == PREINSTALL_MOVE)
	{
		if(cursor_id == 3) // start
		{
			m_start = ~m_start;
			specialty_para_sendTo_motor(cursor_id);
			specialty_move_dis(cursor_id);
			Current_Status_display(1);
			specialty_totaol_time_dis();
			specialty_move_dis(4);
		}
		else if(cursor_id == 4) // ���벹�Ľ���
		{
			if(m_start)return;
			page_id = PREINSTALL_SESHOT;
			specialty_para_sendTo_motor(1);
			change_page();
			Oled_EnlPrint(1, 4, "    <          >    ","", ENGLISH);
			Reshot = 1;
		}
	}
	else if(page_id == PREINSTALL_SESHOT)
	{
		/*if(cursor_id == 3)
		{
			
		}
		else if(cursor_id == 4)
		{
			page_id = PREINSTALL_MOVE;
			cursor_id = 3;
			change_page();
			specialty_totaol_time_dis();
		}*/
	}
	
}


void specialty_cursor_id_dis(uint8_t dir)
{
	if(page_id == PREINSTALL_MODE)
	{
		if(dir & KEY_DOWN_MASK)
		{
			if(cursor_id > mode_id)
			{
				inverse_get_value(0xff);
				specialty_jiaHao_display();
				inverse_get_value(0);
				specialty_cursor_id_mode_dis(cursor_id-1);
			}
			else
			{
				specialty_cursor_id_mode_dis(cursor_id-1);
				inverse_get_value(0xff);
				specialty_cursor_id_mode_dis(cursor_id);
				inverse_get_value(0);
			}
			
		}
		else if(dir & KEY_UP_MASK)
		{
			if((cursor_id+1) > mode_id)
			{
				specialty_jiaHao_display();
			}
			else
			{
				specialty_cursor_id_mode_dis(cursor_id+1);
			}
			
			inverse_get_value(0xff);
			specialty_cursor_id_mode_dis(cursor_id);
			inverse_get_value(0);
		}
	}
	else if(page_id == PREINSTALL_SET)
	{
		if(spe_para[mid_cr].para_id >= 6) // �������ﵽ6�ı�������Ҫ���Ƿ�ҳ
		{
			if(cursor_id > (fanye*6+6)) // ���·�ҳ
			{
				if(cursor_id < PARA_MAXID)
				{
					if(spe_para[mid_cr].para_id >= (fanye*6+6))
					{
						fanye += 1;
						change_page();
						specialty_page_para_dis();
						ReturnOrNext_dis();
						specialty_jiaHao_display();
						jiantou_display();
						return;
					}
				}
			}
			else if(cursor_id <= (fanye*6)) // ���Ϸ�ҳ
			{
				if(fanye)fanye -= 1;
				change_page();
				specialty_page_para_dis();
				ReturnOrNext_dis();
				specialty_jiaHao_display();
				jiantou_display();
				return;
			}
		}
		
		if(cursor_id <= spe_para[mid_cr].para_id) //����ڲ���֮�������ƶ�
		{
			if(dir & KEY_DOWN_MASK)
			{
				specialty_cursor_id_para_dis(cursor_id-1);
				inverse_get_value(0xff); //�ӹ��
				specialty_cursor_id_para_dis(cursor_id);
				inverse_get_value(0);
			}
			else if(dir & KEY_UP_MASK)
			{
				if((cursor_id+1)<=spe_para[mid_cr].para_id)specialty_cursor_id_para_dis(cursor_id+1);
				else {
					specialty_jiaHao_display();
					if((cursor_id+1)==25)ReturnOrNext_dis();
				}
				
				inverse_get_value(0xff); //�ӹ��
				specialty_cursor_id_para_dis(cursor_id);
				inverse_get_value(0);
			}

			
		}
		else // ����ڲ�����ʾ��֮���ƶ������ڼӺţ���һҳ�����ص�֮���ƶ�
		{
			if((cursor_id - 1) == spe_para[mid_cr].para_id)
			{
				if(spe_para[mid_cr].para_id > (fanye*6))
				{
					specialty_cursor_id_para_dis(cursor_id-1);
				}
			}
			specialty_jiaHao_display();
			ReturnOrNext_dis();
		}
	}	
	else if(page_id == PREINSTALL_SET1)
	{
		if(cursor_id < 7)
		{
			if(dir & KEY_DOWN_MASK)
			{
				oled_print_page(cursor_id-1);
				param_display(cursor_id-1);
			}
			
			if(dir & KEY_UP_MASK)
			{
				oled_print_page(cursor_id+1);
				param_display(cursor_id+1);
			}
		
			single_cursor_dis(cursor_id);
			if(cursor_id == 6)
			{
				ConfirmOrCancel_dis();
			}
		}
		else
		{
			if(cursor_id == 7)
			{
				oled_print_page(cursor_id-1);
				param_display(cursor_id-1);
			}
			
			ConfirmOrCancel_dis();
		}
	}	
	else if(page_id == PREINSTALL_MOVE)
	{
		if(dir & KEY_DOWN_MASK)
		{
			oled_print_page(cursor_id-1);
			param_display(cursor_id-1);
		}
		
		if(dir & KEY_UP_MASK)
		{
			oled_print_page(cursor_id+1);
			param_display(cursor_id+1);
		}
		
		single_cursor_dis(cursor_id);
	}
}
void specialty_cursor_id_shift(uint8_t dir)
{
	uint8_t cut_bt = 0;
	if(page_id == PREINSTALL_MODE)
	{
		if(mode_id < 7)
		{
		cursor_count(dir, mode_id+1, 1);
		}
		else
		{
			cursor_count(dir, mode_id, 1);
		}
	}
	else if(page_id == PREINSTALL_SET)
	{
		if(spe_para[mid_cr].para_id < PARA_MAXID)cut_bt = spe_para[mid_cr].para_id + 3;
		else cut_bt = spe_para[mid_cr].para_id + 2; //  ����һ���Ӻţ���������һ��
		cursor_count(dir, cut_bt, 1);
	}
}

void Angle_dis(short int data, uint8_t cur)
{
	uint8_t buff[5]={0};
	uint8_t x_size=0;
	uint8_t x=0;

	if(cur == 1)
	{
		if(data >= 0)
		{
			if(data<10)
			{
				sprintf( (char *)buff,"  %d", data);
			}
			else if(data<100)
			{
				sprintf( (char *)buff," %d", data);
			}
			else
			{
				sprintf( (char *)buff,"%d", data);
			}
		}
		else
		{
			if(data>-10)
			{
				sprintf( (char *)buff,"  %d", data);
			}
			else if(data>-100)
			{
				sprintf( (char *)buff," %d", data);
			}
			else
			{
				sprintf( (char *)buff,"%d", data);
			}
		}
	}
	else
	{
		if(data<10)
		{
			sprintf( (char *)buff,"  %d", data);
		}
		else if(data<100)
		{
			sprintf( (char *)buff," %d", data);
		}
		else
		{
			sprintf( (char *)buff,"%d", data);
		}
	}

	x_size = Check_String(buff, ENGLISH);
	x = LCD_W - (x_size + SCREEN_MIGRATION);
	Oled_EnlPrint(x, cur, buff, " ", ENGLISH);
}

// ��������Ĳ�����ʾ
void specielty_para_dis(uint8_t cur)
{
	if(page_id == PREINSTALL_SET1)
	{
		if(cur == 1) //�Ƕ���ʾ
		{
			Angle_dis(spe_para[mid_cr].modes[pid_cr].ele_angle,   cur);
			//Angle_dis(sp_lip.ele_angle,   cur);
		}
		else if(cur == 2) //����ʱ�䣬�����ٶ�
		{
			integer_display(spe_para[mid_cr].modes[pid_cr].shut_times, cur);
		}
		else if(cur == 3) //������ʾ
		{
			pecialty_pix_amt_display(cur);
		}
		else if(cur == 4)
		{
			secondes_time_dis(spe_para[mid_cr].modes[pid_cr].exposure, cur);
		}
		else if(cur == 5)
		{
			secondes_time_dis(spe_para[mid_cr].modes[pid_cr].interval, cur);
		}
		else if(cur == 6) //��ʱʱ�䣬���Źرպ�ĵȴ�ʱ��
		{
			integer_display(spe_para[mid_cr].modes[pid_cr].sys_stop_t, cur);
		}
	}
}


// רҵģʽ�����н�����ʾ
void specialty_move_dis(uint8_t cur)
{
	if(page_id == PREINSTALL_MOVE)
	{
		if(cur == 1)
		{
			secondes_time_dis(spe_para[mid_cr].delay_t, cur);
		}
		else if(cur == 2)
		{
			manulORauto_dis(spe_para[mid_cr].manul, cur);
		}
		else if(cur == 3)
		{
			status_display(cur);
		}
		else if(cur == 4)
		{
			//integer_position_display(id_add_back, cur, 28);
		}
		else if(cur == 5) //��ʾ��Ҫ���� �Ĳ���
		{
			//secondes_time_dis(spe_para[mid_cr].delay_t, cur);
		}
		else if(cur == 6)
		{
			Oled_EnlPrint(102, cur, "/", " ", ENGLISH);
			pecialty_pix_amt_display(cur);
		}
		else if(cur == 7) // ��ʾ��ǰ״̬
		{
			
		}
	}
	else if(page_id == PREINSTALL_SESHOT)
	{
		//integer_position_display(id_add_back, 1, 16);
	}
}

short int Symbol_para_add(short int data, uint8_t dir)
{
	if(dir & KEY_RIGHT_MASK)
	{
		if(data < 90)
		{
			data += 1;
			//data = data + addend_value;
		}
		else
		{
			//data = mix;
		}
	}
	else if(dir & KEY_LEFT_MASK)
	{
		if(data > -90)
		{
			//if(data)data--;
			
			data -= 1;
		}
		else
		{
			//data = max;
		}
	}

	return data;
}

void specielty_para_adjust(uint8_t dir)
{
	if(page_id == PREINSTALL_SET1)
	{
		inverse_get_value(0xff);
		if(cursor_id == 1) //�Ƕ���ʾ
		{
			spe_para[mid_cr].modes[pid_cr].ele_angle = data_count(spe_para[mid_cr].modes[pid_cr].ele_angle, dir,  360, 1);
		//	sp_lip.ele_angle = spe_para[mid_cr].modes[pid_cr].ele_angle;
		//	inverse_get_value(0xff);
			specielty_para_dis(cursor_id);
		//	inverse_get_value(0);
		}
		else if(cursor_id == 2) //���Ŵ���
		{
			spe_para[mid_cr].modes[pid_cr].shut_times = data_count(spe_para[mid_cr].modes[pid_cr].shut_times, dir,  10, 1);
			//sp_lip.exposure = spe_para[mid_cr].modes[pid_cr].exposure;
			inverse_get_value(0xff);
			specielty_para_dis(cursor_id);
			inverse_get_value(0);
		}
		else if(cursor_id == 3) // ��������
		{
			spe_para[mid_cr].modes[pid_cr].amout = data_count(spe_para[mid_cr].modes[pid_cr].amout, dir,  99, 1);
			//sp_lip.amout = spe_para[mid_cr].modes[pid_cr].amout;
			specielty_para_dis(cursor_id);
		}
		else if(cursor_id == 4) // �����ٶ�
		{
			spe_para[mid_cr].modes[pid_cr].exposure = data_count(spe_para[mid_cr].modes[pid_cr].exposure, dir,  99, 1);
			//sp_lip.amout = spe_para[mid_cr].modes[pid_cr].amout;
			specielty_para_dis(cursor_id);
		}
		else if(cursor_id == 5) //�������
		{
			spe_para[mid_cr].modes[pid_cr].interval = data_count(spe_para[mid_cr].modes[pid_cr].interval, dir,  99, 1);
			//sp_lip.interval = spe_para[mid_cr].modes[pid_cr].interval;
			
			specielty_para_dis(cursor_id);
		}
		else if(cursor_id == 6) //�رտ��ź�ĵȴ�ʱ��
		{
			spe_para[mid_cr].modes[pid_cr].sys_stop_t = data_count(spe_para[mid_cr].modes[pid_cr].sys_stop_t,dir, 99, 1);
			specielty_para_dis(cursor_id);
		}
		inverse_get_value(0);
	}
	else if(page_id == PREINSTALL_MOVE)
	{
		inverse_get_value(0xff);
		if(cursor_id == 1)
		{
			spe_para[mid_cr].delay_t = data_count(spe_para[mid_cr].delay_t, dir,  255, 0);
			specialty_move_dis(cursor_id);
			specialty_para_sendTo_motor(cursor_id);
		}
		else if(cursor_id == 2)
		{
			if(dir & KEY_RIGHT_MASK)
			{
				spe_para[mid_cr].manul = 0xff;
			}
			else if(dir & KEY_LEFT_MASK)
			{
				spe_para[mid_cr].manul = 0;
			}

			specialty_move_dis(cursor_id);
			specialty_para_sendTo_motor(cursor_id);
		}
	/*	else if(cursor_id == 3) //�ٶȵ�λ
		{
			spe_para[mid_cr].orbital_speed = data_count(spe_para[mid_cr].orbital_speed, dir,  4, 1);
			specialty_move_dis(cursor_id);
			specialty_para_sendTo_motor(cursor_id);
		}*/
		inverse_get_value(0);
	}
}

//��ͷ��ʾ
void jiantou_display(void)
{
	uint8_t x=0;

	if(fanye == 0)
	{
		if(spe_para[mid_cr].para_id >= 6)
		{
			if(cursor_id <= 6)
			{
				x = Check_String("��", CHINESE); // ���¼�ͷ
				x = (LCD_W - x) / 2;
				Oled_EnlPrint(x, 7, z_buffer,z_buffer,CHINESE);
			}
		}
	}
	else if(fanye == 1)
	{
		if(spe_para[mid_cr].para_id >= 6)
		{
			if(cursor_id > 6)
			{
				x = Check_String("��", CHINESE); // ���ϼ�ͷ
				x = (LCD_W - x) / 2;
				Oled_EnlPrint(x, 7, z_buffer,z_buffer,CHINESE);
			}
		}
	}
}

//������ʾ
void pecialty_pix_amt_display(uint8_t cur)
{
	uint8_t buff[10];
	uint8_t x=0;
	uint8_t x_size = 0;
	uint32_t amounts = 0;
	
	if(page_id == PREINSTALL_MOVE)
	{
		amounts = spe_para[mid_cr_back].total_aount;
	}
	else
	{
		amounts = spe_para[mid_cr].modes[pid_cr].amout;
	}

	if(amounts < 10)
	{
		sprintf( (char *)buff,"  %d", amounts);
	}
	else if(amounts < 100)
	{
		sprintf( (char *)buff," %d", amounts);
	}
	/*else if(amounts < 1000)
	{
		sprintf( (char *)buff,"%d  ", amounts);
	}
	else if(amounts < 10000)
	{
		sprintf( (char *)buff,"%d ", amounts);
	}*/
	else
	{
		sprintf( (char *)buff,"%d", amounts);
	}
	
	x_size = Check_String(buff, ENGLISH);
	x=LCD_W - (x_size + SCREEN_MIGRATION);
	Oled_EnlPrint(x, cur, buff, " ", ENGLISH);
}

// ����Ӻ�
void delete_jahao(void)
{
	uint8_t x = 0, y=0;

	if(page_id == PREINSTALL_MODE)
	{
		y=mode_id+1;
	}
	else if(page_id == PREINSTALL_SET)
	{
		y=spe_para[mid_cr].para_id+1;
		//if(spe_para[mid_cr].para_id >= (fanye*6+6))return; // ��ǰҳ���в�����������ʾ�Ӻ�

	//	y = spe_para[mid_cr].para_id+1;
		//indxe = y;
		
		if(y > 6) //���ڵ���6�󣬷�ҳ��Ҫ��1�п�ʼ��ʾ
		{
			y = y - (fanye*6) ; //
		}
	}
	
	x = Check_String("      ", CHINESE);
	x = (LCD_W - x) / 2;
	
	if(y<7)Oled_EnlPrint(x, y, z_buffer,z_buffer,CHINESE);
	
}

// ��ɾ��ģʽ�����ʱ�ƶ���Ӧ��ģʽ�����˳��
void paraORmode_shift(void)
{
	uint8_t i = 0;
	if(page_id == PREINSTALL_MODE)
	{
		for(i=cursor_id; i< mode_id; i++)
		{
			memcpy((void *)&spe_para[i].set_tag, (void *)&spe_para[i+1].set_tag, sizeof(spe_para[i])); // shift the data
		}

		memset((void *)&spe_para[i].set_tag, 0, sizeof(spe_para[mode_id]));
		delete_jahao();
		if(mode_id)mode_id -= 1;
	//	String_Printf("FFFF",4);
	//	String_Printf(&mode_id,1);
		specialty_page_mode_dis();
		//ReturnOrNext_dis();
		//jiantou_display();
	}
	else if(page_id == PREINSTALL_SET)
	{
		for(i=cursor_id; i< spe_para[mid_cr].para_id; i++)
		{
			memcpy((void *)&spe_para[mid_cr].modes[i].ele_angle, (void *)&spe_para[mid_cr].modes[i+1].ele_angle, sizeof(spe_para[mid_cr].modes[i])); // shift the data
			specilty_clear_han_display(i);
		}
		memset((void *)&spe_para[mid_cr].modes[spe_para[mid_cr].para_id].ele_angle, 0, sizeof(spe_para[mid_cr].modes[spe_para[mid_cr].para_id]));
		delete_jahao();
		specilty_clear_han_display(i);
		if(spe_para[mid_cr].para_id)spe_para[mid_cr].para_id -= 1;
		specialty_page_para_dis();
	}
}

void delet_paraORmode(void)
{
	uint8_t i;
	i = Check_Buffer_Empty();
	if(i == NOT_EMPTY)return;
	
	App_Buffer[i].app_send_buffer[0] = 0x90;
	App_Buffer[i].app_send_buffer[1] = 0;
	App_Buffer[i].app_send_buffer[2] = 0x09;
	App_Buffer[i].app_send_buffer[3] = 0x06;
	if(page_id == PREINSTALL_MODE)
	{
		if(cursor_id <= mode_id)
		{
			App_Buffer[i].app_send_buffer[4] = 0x0c;
			App_Buffer[i].app_send_buffer[5] = cursor_id;
			App_Buffer[i].app_send_buffer[6] = 0;
			App_Buffer[i].app_send_buffer[7] = 0;
			paraORmode_shift();
		}
	}
	else if(page_id == PREINSTALL_SET)
	{
		if(cursor_id <= spe_para[mid_cr].para_id)
		{
			App_Buffer[i].app_send_buffer[4] = 0x0d;
			App_Buffer[i].app_send_buffer[5] = cursor_id;
			App_Buffer[i].app_send_buffer[6] = 0;
			App_Buffer[i].app_send_buffer[7] = 0;
			paraORmode_shift();
		}
	}
	
	App_Buffer[i].app_send_buffer[8] = check_sum_add(8, App_Buffer[i].app_send_buffer);
	App_Buffer[i].app_send_size = 9;
}

/////////////////////////////  ������ݲ��� ��������� ////////////////////
void specialty_para_sendTo_motor(uint8_t curt)
{
	uint8_t i;
	i = Check_Buffer_Empty();
	if(i == NOT_EMPTY)return;
	
	App_Buffer[i].app_send_buffer[0] = 0x90;
	App_Buffer[i].app_send_buffer[1] = 0;
	App_Buffer[i].app_send_buffer[2] = 0x09;
	App_Buffer[i].app_send_buffer[3] = 0x06;

	if(page_id == PREINSTALL_SET1)
	{
		if(curt == 7) //ȷ�ϲ���
		{
			App_Buffer[i].app_send_buffer[4] = 1; //	�Ƕ�
			App_Buffer[i].app_send_buffer[5] = ((uint16_t)spe_para[mid_cr].modes[pid_cr].ele_angle);
			App_Buffer[i].app_send_buffer[6] = ((uint16_t)spe_para[mid_cr].modes[pid_cr].ele_angle)>>8;
			App_Buffer[i].app_send_buffer[7] = pid_cr;
			App_Buffer[i].app_send_buffer[8] = check_sum_add(8, App_Buffer[i].app_send_buffer);
			App_Buffer[i].app_send_size = 9;
			if(i<LOCUS_BUFFER_MAX-1)i += 1;
			else i=0;
			App_Buffer[i].app_send_buffer[0] = 0x90;
			App_Buffer[i].app_send_buffer[1] = 0;
			App_Buffer[i].app_send_buffer[2] = 0x09;
			App_Buffer[i].app_send_buffer[3] = 0x06;
			App_Buffer[i].app_send_buffer[4] = 2; //���Ŵ���
			App_Buffer[i].app_send_buffer[5] = ((uint16_t)spe_para[mid_cr].modes[pid_cr].shut_times);
			App_Buffer[i].app_send_buffer[6] = ((uint16_t)spe_para[mid_cr].modes[pid_cr].shut_times)>>8;
			App_Buffer[i].app_send_buffer[7] = pid_cr;
			App_Buffer[i].app_send_buffer[8] = check_sum_add(8, App_Buffer[i].app_send_buffer);
			App_Buffer[i].app_send_size = 9;
			if(i<LOCUS_BUFFER_MAX-1)i += 1;
			else i=0;
			App_Buffer[i].app_send_buffer[0] = 0x90;
			App_Buffer[i].app_send_buffer[1] = 0;
			App_Buffer[i].app_send_buffer[2] = 0x09;
			App_Buffer[i].app_send_buffer[3] = 0x06;
			App_Buffer[i].app_send_buffer[4] = 3; //	����
			App_Buffer[i].app_send_buffer[5] = (spe_para[mid_cr].modes[pid_cr].amout);
			App_Buffer[i].app_send_buffer[6] = 0;
			App_Buffer[i].app_send_buffer[7] = pid_cr;
			App_Buffer[i].app_send_buffer[8] = check_sum_add(8, App_Buffer[i].app_send_buffer);
			App_Buffer[i].app_send_size = 9;
			if(i<LOCUS_BUFFER_MAX-1)i += 1;
			else i=0;
			App_Buffer[i].app_send_buffer[0] = 0x90;
			App_Buffer[i].app_send_buffer[1] = 0;
			App_Buffer[i].app_send_buffer[2] = 0x09;
			App_Buffer[i].app_send_buffer[3] = 0x06;
			App_Buffer[i].app_send_buffer[4] = 4; //	�����ٶȼ����ع�ʱ��
			App_Buffer[i].app_send_buffer[5] = (spe_para[mid_cr].modes[pid_cr].exposure);
			App_Buffer[i].app_send_buffer[6] = 0;
			App_Buffer[i].app_send_buffer[7] = pid_cr;
			App_Buffer[i].app_send_buffer[8] = check_sum_add(8, App_Buffer[i].app_send_buffer);
			App_Buffer[i].app_send_size = 9;
			if(i<LOCUS_BUFFER_MAX-1)i += 1;
			else i=0;
			App_Buffer[i].app_send_buffer[0] = 0x90;
			App_Buffer[i].app_send_buffer[1] = 0;
			App_Buffer[i].app_send_buffer[2] = 0x09;
			App_Buffer[i].app_send_buffer[3] = 0x06;
			App_Buffer[i].app_send_buffer[4] = 5; //	���ʱ��
			App_Buffer[i].app_send_buffer[5] = (spe_para[mid_cr].modes[pid_cr].interval);
			App_Buffer[i].app_send_buffer[6] = 0;
			App_Buffer[i].app_send_buffer[7] = pid_cr;
			App_Buffer[i].app_send_buffer[8] = check_sum_add(8, App_Buffer[i].app_send_buffer);
			App_Buffer[i].app_send_size = 9;
			if(i<LOCUS_BUFFER_MAX-1)i += 1;
			else i=0;
			App_Buffer[i].app_send_buffer[0] = 0x90;
			App_Buffer[i].app_send_buffer[1] = 0;
			App_Buffer[i].app_send_buffer[2] = 0x09;
			App_Buffer[i].app_send_buffer[3] = 0x06;
			App_Buffer[i].app_send_buffer[4] = 0x0e; //	��ʱʱ��
			App_Buffer[i].app_send_buffer[5] = (spe_para[mid_cr].modes[pid_cr].sys_stop_t);
			App_Buffer[i].app_send_buffer[6] = 0;
			App_Buffer[i].app_send_buffer[7] = pid_cr;
			App_Buffer[i].app_send_buffer[8] = check_sum_add(8, App_Buffer[i].app_send_buffer);
			App_Buffer[i].app_send_size = 9;
			return;
		}
		/*else if(curt == 7) //ȡ������������ȡ����˵�ͬ������
		{
			App_Buffer[i].app_send_buffer[4] = 0x0b;
			App_Buffer[i].app_send_buffer[5] = pid_cr;
			App_Buffer[i].app_send_buffer[6] = 0;
			App_Buffer[i].app_send_buffer[7] = 0;
			App_Buffer[i].app_send_buffer[8] = check_sum_add(8, App_Buffer[i].app_send_buffer);
			App_Buffer[i].app_send_size = 9;
		}*/
		
	}
	else if(page_id == PREINSTALL_MOVE)
	{
		if(curt == 1) // ���Ͷ�ʱ����
		{
			App_Buffer[i].app_send_buffer[4] = 0x06;
			App_Buffer[i].app_send_buffer[5] = spe_para[mid_cr].delay_t;
			App_Buffer[i].app_send_buffer[6] = 0;
			App_Buffer[i].app_send_buffer[7] = mid_cr;
		}
		else if(curt == 2) // �����ֶ�ģʽ��־
		{
			App_Buffer[i].app_send_buffer[4] = 0x07;
			App_Buffer[i].app_send_buffer[5] = spe_para[mid_cr].manul;
			App_Buffer[i].app_send_buffer[6] = 0;
			App_Buffer[i].app_send_buffer[7] = mid_cr;
		}
		else if(curt == 4)
		{
			App_Buffer[i].app_send_buffer[4] = 0x11;
			App_Buffer[i].app_send_buffer[5] = id_add_back;
			App_Buffer[i].app_send_buffer[6] = mid_cr;
			App_Buffer[i].app_send_buffer[7] = 0;
		}
		else if(curt == 3) // start
		{
			App_Buffer[i].app_send_buffer[4] = 0x08;
			App_Buffer[i].app_send_buffer[5] = m_start;
			App_Buffer[i].app_send_buffer[6] = 0;
			App_Buffer[i].app_send_buffer[7] = mid_cr;
		}
		
		App_Buffer[i].app_send_buffer[8] = check_sum_add(8, App_Buffer[i].app_send_buffer);
		App_Buffer[i].app_send_size = 9;
	}
	else if(page_id == PREINSTALL_SESHOT)
	{
			App_Buffer[i].app_send_buffer[4] = 0x11;
			App_Buffer[i].app_send_buffer[5] = id_add_back;
			App_Buffer[i].app_send_buffer[6] = mid_cr;
			App_Buffer[i].app_send_buffer[7] = curt; // 1Ϊ�����Ĳ�״̬��2Ϊ�˳�����
		
		App_Buffer[i].app_send_buffer[8] = check_sum_add(8, App_Buffer[i].app_send_buffer);
		App_Buffer[i].app_send_size = 9;
	}
	
	
}

void mode_and_para_sendt(uint8_t types)
{
	uint8_t i;
	i = Check_Buffer_Empty();
	if(i == NOT_EMPTY)return;
	
	App_Buffer[i].app_send_buffer[0] = 0x90;
	App_Buffer[i].app_send_buffer[1] = 0;
	App_Buffer[i].app_send_buffer[2] = 0x09;
	App_Buffer[i].app_send_buffer[3] = 0x06;
	if(types) //ȷ�����ģʽ
	{
		App_Buffer[i].app_send_buffer[4] = 0x09;
		App_Buffer[i].app_send_buffer[5] = mid_cr;
		App_Buffer[i].app_send_buffer[6] = 0;
		App_Buffer[i].app_send_buffer[7] = 0;
	}
	else //ȷ����Ӳ���
	{
		App_Buffer[i].app_send_buffer[4] = 0x0a;
		App_Buffer[i].app_send_buffer[5] = mid_cr;
		App_Buffer[i].app_send_buffer[6] = spe_para[mid_cr].para_id;
		App_Buffer[i].app_send_buffer[7] = pid_cr;
	}
	App_Buffer[i].app_send_buffer[8] = check_sum_add(8, App_Buffer[i].app_send_buffer);
	App_Buffer[i].app_send_size = 9;
}

void specialty_totaol_time_dis(void)
{
	uint8_t i=0;
	if(page_id != PREINSTALL_MOVE)return;
	spe_para[mid_cr_back].total_aount = 0;
	
	/*for(i=0; i< spe_para[mid_cr_back].para_id ;i++)
	{
		spe_para[mid_cr_back].total_aount += spe_para[mid_cr_back].modes[i+1].amout;
	}*/
	
	spe_para[mid_cr_back].total_aount = spe_para[mid_cr_back].modes[id_add].amout;
	pecialty_pix_amt_display(6);
	Cunrent_para_display(id_add);
}

void Get_data_from_Motor(uint8_t *fofob)
{
	uint8_t temp_id = 0;
	if(mode_id < fofob[15])
	{
		mode_id += 1;
		specialty_page_mode_dis();
	}
	mid_cr = fofob[15];
	pid_cr = fofob[14];
	spe_para[mid_cr].para_id = pid_cr;

	spe_para[mid_cr].modes[pid_cr].shut_times = (uint16_t)fofob[6] | (uint16_t)fofob[7]<<8;
	spe_para[mid_cr].modes[pid_cr].ele_angle = (uint16_t)fofob[4] | (uint16_t)fofob[5]<<8;
	spe_para[mid_cr].modes[pid_cr].amout = fofob[8];
	spe_para[mid_cr].modes[pid_cr].exposure = fofob[9];
	spe_para[mid_cr].modes[pid_cr].interval = fofob[10];
	spe_para[mid_cr].delay_t = fofob[11];
	spe_para[mid_cr].manul = fofob[12];
	m_start = fofob[13];
	spe_para[mid_cr].modes[pid_cr].sys_stop_t = fofob[16];
	id_add = 1;
}

// ���һ�е���ʾ
void specilty_clear_han_display(uint8_t cur)
{
	uint8_t y=0;
//	x_size = Check_String(buff, ENGLISH);
//	x=LCD_W - (x_size + SCREEN_MIGRATION);
	if(cur > 6)
	{
		y= cur - (fanye * 6);
	}	
	else
	{
		y=cur;
	}
	if(y<7)Oled_EnlPrint(0, y, "                    ", " ", ENGLISH);
}

// �̰���Դ��������������
void pwr_key_enter_moveInterfer(void)
{
	if(page_id != PREINSTALL_SET)return;
	page_id = PREINSTALL_MOVE;
	cursor_id = 3;
	change_page();
	specialty_totaol_time_dis();
}

// ��ʾ��ǰ��������ǵڼ�������
void Cunrent_para_display(uint8_t cur)
{
	uint8_t x=0;
	uint8_t fifos[22]={0};
	uint16_t ele_an=0;
	uint16_t lve_ang=0;
	uint8_t mounts=0;
	uint8_t shuts=0;
	uint8_t inter=0;
	uint8_t sys_stopss=0;

	ele_an = spe_para[mid_cr].modes[cur].ele_angle;
	shuts = spe_para[mid_cr].modes[cur].exposure;
	mounts = spe_para[mid_cr].modes[cur].amout;
	lve_ang = spe_para[mid_cr].modes[cur].shut_times;
	inter = spe_para[mid_cr].modes[cur].interval;
	sys_stopss = spe_para[mid_cr].modes[cur].sys_stop_t;
	
	sprintf( (char *)fifos,"(%d)%d,%d,%d,%d,%d,%d", cur,ele_an, lve_ang, mounts, shuts, inter,sys_stopss);
	
	x = Check_String(fifos, ENGLISH);
	x = (LCD_W - x) / 2;

	//if(cur > 6)cur -= fanye*6;
	//if(cur == 0)return;
	Oled_EnlPrint(0, 5, "                    ",z_buffer,ENGLISH);
	Oled_EnlPrint(x, 5, fifos,z_buffer,ENGLISH);
	
}



//�����й��Ľ�������ң�������������ݲ�����ʾ
void shotting_Get_data_from_controller(uint8_t *sptt)
{
	uint8_t move_be = 0;
	
	//if(page_id != FINDING_ORIGIN)
	
	p_amount = (uint16_t)sptt[4] | (uint16_t)sptt[5]<<8; // ����
	sp_start = sptt[8];
	id_add = sptt[9];
	move_be = sptt[10];
	
	if(page_id != PREINSTALL_MOVE)return;
	
	if(m_start == 0)
	{
		specialty_move_dis(3);
		Current_Status_display(2);
	}
	else
	{
		if(move_be == 0)
		{
			/*if(id_add)
			{
				if(id_add - 1)id_add_back = id_add - 1;
				//else if(id_add == pid_cr)id_add_back = 1;
				else id_add_back = 1;
			}*/
			
			
			m_start = 0;
			status_display(3);
		//	pix_amt_compara_dis(3);
			specialty_totaol_time_dis();
			//Cunrent_para_display(id_add);
			
			Current_Status_display(0);
		}
	}
	
	pix_amt_compara_dis(6);
}

void Current_Status_display(uint8_t statuss)
{
	uint8_t x=0;
	uint8_t buff[20];
	uint8_t id_b=0;
	uint8_t x1=0;
	if(page_id != PREINSTALL_MOVE)return;
	
	if(statuss==1) // ����������
	{
		inverse_get_value(0xff);
		if(wifi_id.language_sel == CHINESE)
		{
			//sprintf( (char *)buff,"���� ����������", p_amount);

			sprintf( (char *)buff," (%d)", id_add);
			x1= Check_String(buff, ENGLISH);
			
			x = Check_String("��������������", ENGLISH);
			
			x = (LCD_W - x - x1) / 2;
			
			Oled_EnlPrint(x+x1, 7, buff,z_buffer,wifi_id.language_sel);
			Oled_EnlPrint(x, 7, buff,buff, ENGLISH);
		}
		else
		{
			//sprintf( (char *)buff,"Para 3 is shotting", p_amount);
			x = Check_String("Para     is shotting", ENGLISH);
			x = (LCD_W - x) / 2;
			Oled_EnlPrint(x, 7, z_buffer,buff,wifi_id.language_sel);
			sprintf( (char *)buff,"(%d)", id_add);
			Oled_EnlPrint(x+30, 7, buff,buff, ENGLISH);
		}
		inverse_get_value(0);
	}
	else if(statuss == 2) // 
	{
		if(sp_start)return;
		inverse_get_value(0xff);
		x = Check_String("���в��������ѽ���", ENGLISH);
		
		x = (LCD_W - x - x1) / 2;
		
		Oled_EnlPrint(x, 7, buff,z_buffer,wifi_id.language_sel);
		inverse_get_value(0);
	}
	else
	{
		if(id_add <=1)id_b=1;
		//else if(id_add == pid_cr)id_b=pid_cr;
		else id_b=id_add-1;
		
		if(wifi_id.language_sel == CHINESE)
		{
			sprintf( (char *)buff," (%d)", id_b);
			x1 = Check_String(buff, ENGLISH);
			
			x = Check_String("���������ѽ���", ENGLISH);
			x = (LCD_W - x - x1) / 2;
			
			Oled_EnlPrint(x+x1, 7, buff,z_buffer,wifi_id.language_sel);
			
			Oled_EnlPrint(x, 7, buff,buff, ENGLISH);
		}
		else
		{
			//sprintf( (char *)buff,"Para 3 is finished", p_amount);
			x = Check_String("Para     is finished", ENGLISH);
			x = (LCD_W - x) / 2;
			Oled_EnlPrint(x, 7, z_buffer,buff,wifi_id.language_sel);
			sprintf( (char *)buff,"(%d)", id_b);
			Oled_EnlPrint(x+30, 7, buff,buff, ENGLISH);
		}
	}
	//inverse_get_value(0);
}

void cear_the_id_add(uint8_t datas)
{
	id_add = datas;
}

void specilty_keyscan_send(uint8_t dir)
{
	uint8_t i;
	if(page_id != PREINSTALL_SESHOT)return;
	//if(m_start==0)return;
	//if(cursor_id != 3)return;
	if(Reshot == 0)return;
	//if(spe_para[mid_cr].manul==0)return;
	
	i = Check_Buffer_Empty();
	if(i == NOT_EMPTY)return;

	App_Buffer[i].app_send_buffer[0] = 0x90;
	App_Buffer[i].app_send_buffer[1] = 0;
	App_Buffer[i].app_send_buffer[2] = 0x09;
	App_Buffer[i].app_send_buffer[3] = 6;
	App_Buffer[i].app_send_buffer[4] = 0x10;
	
	if(dir & KEY_LEFT_MASK)
	{
		App_Buffer[i].app_send_buffer[5] = 0x01;
		App_Buffer[i].app_send_buffer[6] = B_TO_A;
		controller_send_data_to_motor(0,0x06, 0x10);
	}
	else if( dir & KEY_RIGHT_MASK)
	{
		App_Buffer[i].app_send_buffer[5] = 0x02;
		App_Buffer[i].app_send_buffer[6] = 0xff;
	//	controller_send_data_to_motor(0,0x06, 0x10);
		
	}
	/*else
	{
		App_Buffer[i].app_send_buffer[5] = 0x01;
		App_Buffer[i].app_send_buffer[6] = 0;
	//	controller_send_data_to_motor(0,0x06, 0x10);
	}*/
	
	App_Buffer[i].app_send_buffer[7] = 0;
	App_Buffer[i].app_send_buffer[8] = check_sum_add(8, App_Buffer[i].app_send_buffer);
	App_Buffer[i].app_send_size = 9;
}

void specilty_page_return(void)
{
	if(page_id == PREINSTALL_SET)
	{
		//page_id_back = page_id;
		page_id = PREINSTALL_MODE;
		//cursor_id = 1;
		cursor_id = mid_cr_back;
		change_page();
		specialty_page_mode_dis();
	}
	else if(page_id == PREINSTALL_SET1 || page_id == PREINSTALL_MOVE)
	{
		page_id = PREINSTALL_SET;
		cursor_id = pid_cr;
		change_page();
		specialty_page_para_dis();
		ReturnOrNext_dis();
		jiantou_display();
		mode_and_para_sendt(1);
	}
	else if(page_id == PREINSTALL_SESHOT)
	{
		specialty_para_sendTo_motor(0);
		page_id = PREINSTALL_MOVE;
		cursor_id = 3;
		change_page();
		specialty_totaol_time_dis();
		specialty_move_dis(4);
		Reshot = 0;
	}
}

void specilty_get_reshot_form_motor(uint8_t datas)
{
	id_add_back = datas;
	specialty_move_dis(4);
}

