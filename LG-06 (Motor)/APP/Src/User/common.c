
#include "common.h"
#include "uart.h"
#include "string.h"
#include "gpio.h"
#include "MOTOR.h"
#include "flash.h"

CONSTANT con_b={0};

FIND_P find_pata={0};
FIND_P find_pataVV={0};
MANAUL_SLOW Manul_p={0};

uint8_t mode_backup =0;
uint8_t p_upORdown=0;
uint8_t move_begin = 0;
uint8_t move_begin_backup =0;
uint8_t m_start = 0;
uint8_t angle_snedt_tag = 0; // 实时发送角度标志位
uint8_t angle_snedt_time = 0; // 实时发送角度标时间间隔
uint8_t speed_Gear[4]={0};
uint8_t ManulHH_dir_cehck=0;
uint8_t ManulVV_dir_cehck=0;

uint16_t dynamic_speed=0;
uint16_t slow_time = 0;

uint32_t boot_time=0;
uint32_t p_move_time  = 0;
uint32_t p_amount=0;

double Angle_basic = 0;


void para_init_set(void)
{
	Angle_basic = 0.002818; // 0.001874;
}

void time_count(void)
{
	if(boot_time)boot_time--;
	if(move_begin>1)p_move_time++;
	if(mode_backup == PREINSTALL_MODE)if(con_b.begin>1)p_move_time++;
	if(slow_time)slow_time--;
	if(con_b.slw_time)con_b.slw_time--;
	if(video_p.locusH.slow_time)video_p.locusH.slow_time--;
	//if(video_p.locusV.slow_time)video_p.locusV.slow_time--;
	if(Manul_p.HH_Ttime)Manul_p.HH_Ttime--;
	//if(Manul_p.VV_Time)Manul_p.VV_Time--;
	if(video_p.activate)video_p.activate--;
	if(delay_p.activate)delay_p.activate--;
	//if(delay_p.locuVV.ramTime)delay_p.locuVV.ramTime--;
	if(delay_p.locuHH.ramTime)delay_p.locuHH.ramTime--;
	if(angle_snedt_time)angle_snedt_time--;
}
void time_delay_ms(uint16_t counts)
{
	boot_time = counts;
	while(boot_time)
	{
		Feed_IWDG();
	}
}

double PULSE_SUM(uint16_t anglel)
{
	//float temp=0;
	double temp11=0;
	temp11 = (double)anglel / Angle_basic; 

	/*temp11 = temp*10;
	temp11 = temp11 % 10;

	if(temp11)
	{
		temp11 = temp + 1;
	}
	else
	{
		temp11 = temp;
	}*/

	return temp11;
}


uint8_t check_sum_add(uint8_t index, uint8_t *spt)
{
	uint8_t i;
	uint16_t temp=0;

	spt[index] = 0;
	
	for(i=0;i<index; i++)
	{
		temp += (uint16_t)spt[i];
	}

	return (temp & 0xff);
}

void slider_upORdown(uint8_t p_type)
{
	p_upORdown = p_type;
}

//慢启停的间隔时间
void set_slow_time_data(void)
{
	if(mode_backup == VIDEO_MODE )
	{
		slow_time = SLOW_BASIC_TIME11 ;//SLOW_THRESHOLD - dynamic_speed + 2;
	}
	else if(mode_backup == PREINSTALL_MODE)
	{
		slow_time = SLOW_BASIC_TIME33;
	}
	else
	{
		standard_p.HH_Ttime = SLOW_BASIC_TIME44 ;//SLOW_THRESHOLD - dynamic_speed + 2;
	}
}

uint8_t Check_Buffer_Empty(USART11_SHADOW *sptt)
{
	uint8_t i;
	for(i=0;i<LOCUS_BUFFER_MAX;i++)
	{
		if(sptt[i].shadow_send_size==0)
		{
			return i;
		}
	/*	if(sptt[con_b.wf_pk_id].shadow_send_size)
		{
			if(con_b.wf_pk_id < (LOCUS_BUFFER_MAX-1))
			{
				con_b.wf_pk_id += 1;
			}
			else
			{
				con_b.wf_pk_id = 0;
			}
		}
		else
		{
			return con_b.wf_pk_id;
		}*/
	}

	return 0xff;
	
/*	for(i = 1; i< SEND_BUFFER_MAX; i++)
	{
		if(sptt[i].shadow_send_size == 0)return i;
	}
	
	return NOT_EMPTY;*/
}

void Package_dataBufer(uint8_t txsize, uint8_t *buffer)
{
	uint8_t i;
	i = Check_Buffer_Empty(Usart11_shadow);
	
	if(i==0xff)return;
	//	if(mode_backup == PANORAMA_MOED)
		{
			memcpy(&Usart11_shadow[i].usart11_shadow_buffer[0], buffer, txsize); // load data to DMA send buffer
		}
		
		Usart11_shadow[i].shadow_send_size = txsize;
		Usart11_Interval_set();
}


void Send_connect_data_to_controller(void)
{
	uint8_t buffers[20] = {0};
	uint8_t sizes = 0;

	buffers[0] = 0x09;
	buffers[1] = 0x00;
	buffers[2] = 0x05;
	buffers[3] = mode_backup;

	if(mode_backup == VIDEO_MODE) // 视频模式
	{
		buffers[4] = con_b.move_time_s;
		buffers[5] = con_b.move_time_m;
		buffers[6] = con_b.move_time_h;
		buffers[7] = video_p.loop;
		buffers[8] = video_p.locusH.dir;
		buffers[9] = con_b.vvAB_set;
		buffers[10] = 0;
		buffers[11] = video_p.Pause;
		buffers[12] = check_sum_add(12, buffers);
		sizes = 13;
	}
	else if(mode_backup == DELAY_SET) // 延时模式
	{
		buffers[4] = delay_p.interval;
		buffers[5] = delay_p.interval>>8;
		buffers[6] = delay_p.exposure;
		buffers[7] = delay_p.exposure>>8;
		buffers[8] = delay_p.amount;
		buffers[9] = delay_p.amount>>8;
		buffers[10] = delay_p.m_start;
		buffers[11] = delay_p.ab_set_if;
		buffers[12] = delay_p.locuHH.dir;
		buffers[13] = check_sum_add(13, buffers);
		sizes = 14;
	}
	else if(mode_backup == SET_ORIGIN) // 进入原点设置
	{
		buffers[4] = check_sum_add(4, buffers);
		sizes = 5;
	}
	else if(mode_backup == MAIN_ID) //主菜单主界面
	{
		buffers[4] = check_sum_add(4, buffers);
		sizes = 5;
	}
	else if(mode_backup == STANDAR_MODE) // 标准模式
	{
		buffers[4] = con_b.delay_time_s;
		buffers[5] = m_start;
		buffers[6] = con_b.taltol_amount;
		buffers[7] = con_b.exposure;
		buffers[8] = con_b.exposure>>8;
		buffers[9] = con_b.std_manul;
		buffers[10] = con_b.interval;
		buffers[11] = con_b.interval>>8;
		buffers[12] = check_sum_add(12, buffers);
		sizes = 13;
	}
	else if(mode_backup == WIDE_ANGLE_MODE) // 广角模式
	{
		buffers[4] = con_b.wid_time_s;
		buffers[5] = m_start;
		buffers[6] = con_b.taltol_amount;
		buffers[7] = con_b.exposure;
		buffers[8] = con_b.exposure>>8;
		buffers[9] = con_b.wid_manul;
		buffers[10] = con_b.interval;
		buffers[11] = con_b.interval>>8;
		buffers[12] = check_sum_add(12, buffers);
		sizes = 13;
	}
	else if(mode_backup == PANORAMA_MAIN)
	{
		buffers[4] = check_sum_add(4, buffers);
		sizes = 5;
	}
	else if(mode_backup == PREINSTALL_MODE)
	{
		Sync_sendt = glob_para.mode_id;
		if(Sync_sendt>=MODE_ID_MAX)Sync_sendt=MODE_ID_MAX-1;
		//String_Printf(USART_2,"CC",2);
		//String_Printf(USART_2,&Sync_sendt,1);
		//Specialty_Send_data();
		//Specialty_modeID_sendt();
		return;
	}
	else if(mode_backup == MANUAL_MODE)
	{
		buffers[4] = glob_para.speed;
		buffers[5] = check_sum_add(5, buffers);
		sizes = 6;
	}
	
	Package_dataBufer(sizes, buffers);
}


void Set_origin_move(uint8_t dirs, uint8_t active)
{
	uint8_t typs = 0;
	//if(mode_backup != SET_ORIGIN && mode_backup != SET_ORIGIN)
	if(dirs == 1) // left
	{
		if(active)motorHH_p.DIR = B_TO_A;
		typs = 1;
	}
	else if(dirs == 2) // right
	{
		if(active)motorHH_p.DIR = A_TO_B;
		typs = 1;
	}
	else if(dirs == 3) // up
	{
		//if(active)motorVV_p.DIR = A_TO_B;
		if(active)motorVV_p.DIR = A_TO_B;
		typs = 2;
	}
	else if(dirs == 4) // down
	{
		//if(active)motorVV_p.DIR = B_TO_A;
		if(active)motorVV_p.DIR = B_TO_A;
		typs = 2;
	}

	if(active)
	{
		angle_snedt_tag = 0xff;
		angle_snedt_time = 100;
		
		if(typs == 1)
		{
			if(mode_backup == VIDEO_MODE )
			{
				if(con_b.vvAB_set)
				{
					if(con_b.vvAB_set < 3)
					{
						if((motorHH_p.DVpulse_count >= B_POINT_END) )
						{
							if(motorHH_p.DIR == A_TO_B)return;
						}
						else if(motorHH_p.DVpulse_count <= A_POINT_END)
						{
							if(motorHH_p.DIR == B_TO_A)return;
						}
					}
				}
			}
			else if(mode_backup == DELAY_SET)
			{
				if(delay_p.ab_set_if)
				{
					if(delay_p.ab_set_if < 3)
					{
						if((motorHH_p.DLLpulse_count >= B_POINT_END))
						{
							if(motorHH_p.DIR == A_TO_B)return;
						}
						else if(motorHH_p.DLLpulse_count <= A_POINT_END)
						{
							if(motorHH_p.DIR == B_TO_A)return;
						}
					}
				}
			}
			
			Manual_mode_dir_set(typs);
			motorHH_direction_change(motorHH_p.DIR);
			Manul_SartOrStop(MOTOR_HORITAL,0xff);
			//motorHH_speed_set(speed_calculat(95));
		}
		else if(typs == 2)
		{
			if(mode_backup == VIDEO_MODE )
			{
				if(con_b.vvAB_set)
				{
					if(con_b.vvAB_set < 3)
					{
						if((motorVV_p.DVpulse_count >= B_POINT_END) )
						{
							if(motorVV_p.DIR == A_TO_B)return;
						}
						else if(motorVV_p.DVpulse_count <= A_POINT_END)
						{
							if(motorVV_p.DIR == B_TO_A)return;
						}
					}
				}
			}
			else if(mode_backup == DELAY_SET)
			{
				if(delay_p.ab_set_if)
				{
					if(delay_p.ab_set_if < 3)
					{
						if((motorVV_p.DLLpulse_count >= B_POINT_END))
						{
							if(motorVV_p.DIR == A_TO_B)return;
						}
						else if(motorVV_p.DLLpulse_count <= A_POINT_END)
						{
							if(motorVV_p.DIR == B_TO_A)return;
						}
					}
				}
			}
			Manual_mode_dir_set(typs);
			//motorVV_direction_change(motorVV_p.DIR);
			Manul_SartOrStop(MOTOR_Vertical,0xff);
			//motorVV_speed_set(speed_calculat(95));
			//motorVV_start();
		}
	}
	else
	{
		if(typs == 1)
		{
			Manul_SartOrStop(MOTOR_HORITAL,0);
		}
		else if(typs == 2)
		{
		//	Manul_SartOrStop(MOTOR_Vertical,0);
		}

		if(mode_backup == MANUAL_MODE)
		{
			angle_snedt_time = 0;
		}
		else
		{
			angle_snedt_time = 0;
			Angle_real_time_transmission(1);
			angle_snedt_tag = 0;
		}
		angle_snedt_tag = 0;
	}
}

void Set_origin_OK(uint8_t types)
{
	if(mode_backup == SET_ORIGIN)
	{
		motorHH_p.pulse_count = ORIGIN_POINT;
		motorVV_p.pulse_count = ORIGIN_POINT;
		mode_backup = MAIN_ID;
	}
	else if(mode_backup == VIDEO_MODE)
	{
		Video_abPoint_set(types);
	}
	else if	(mode_backup == DELAY_SET)
	{
		Delay_set_abPoint(types);
	}
	//Send_connect_data_to_controller();
}

void Get_data_from_controller(uint8_t *fifos)
{
//	uint16_t temp11 = 0;
	if(fifos[0] == 0x90)
	{
		if(fifos[1] == 0x00)
		{
			if(fifos[3] == 1) // video mode
			{
				Video_Get_data_from_controller(con_buffer);
			}
			else if(fifos[3] == 2) // timelapse
			{
				Delay_get_data_from_controller(con_buffer);
			}
			else if(fifos[3] == 0x04) // 单层全景模式设置参数
			{
				Standard_Get_data_from_controller(con_buffer);
			}
			else if(fifos[3] == 0x06) // Specialty mode 专业模式
			{
				Specialty_Get_data_from_controller(con_buffer);
			}
			else if(fifos[3] == 0x07) // manual mode
			{
				if(fifos[4] == 0x01)
				{
					if(fifos[5] > 3)return;
					glob_para.speed = fifos[5];
					if_write_flash();
				}
			}
			else if(fifos[3] == 0x03) // set origin or ab point
			{
				//mode_backup = SET_ORIGIN;
				if(fifos[4] == 0x01) // SET MOVE
				{
					Set_origin_move(fifos[5], fifos[6]);	
				}
				else if(fifos[4] == 0x02) // set OK
				{
					Set_origin_OK(fifos[5]);
					//motorHH_speed_set(3);
					//motorHH_start();
					//motorVV_speed_set(3);
					//motorVV_start();
				}
			}
		}
		if(fifos[1] == 0x04)
		{
			if(fifos[2] == 0x09)
			{
				if(fifos[3] == 0x01)
				{
					if(mode_backup == 0)mode_backup = SET_ORIGIN;
					Send_connect_data_to_controller();
					if(mode_backup == DELAY_SET)Delay_total_time_calculate();
					if(mode_backup == MANUAL_MODE)
					{
						if(motorHH_p.pulse_count == ORIGIN_POINT)ManulHH_dir_cehck = A_TO_B;
						else ManulHH_dir_cehck = motorHH_p.DIR;
						
						if(motorVV_p.pulse_count == ORIGIN_POINT)ManulVV_dir_cehck = A_TO_B;
						else ManulVV_dir_cehck = motorVV_p.DIR;
						angle_snedt_time = 0;
						Angle_real_time_transmission(0);
					}
					else if(mode_backup == PREINSTALL_MODE)
					{
						//Specialty_Send_reshot_to_controller(0);
					}
				}
			}
		}
		else if(fifos[1] == 0x06)
		{
			if(fifos[3] == 1) // inter video mode
			{
				if(mode_backup != VIDEO_MODE)
				{
					mode_backup = VIDEO_MODE;
					Send_connect_data_to_controller();
					video_p.DIR = A_TO_B;
					video_p.locusH.dir = video_p.DIR;
					video_p.locusV.dir = video_p.DIR;
					video_Dir_check(MOTOR_HORITAL);
					video_Dir_check(MOTOR_Vertical);
					
					Video_find_Apoint();
					//Panorama_mode_find_Apoint();
					//Video_MotorHH_Start_check_diretion();
					//Video_MotorVV_Start_check_diretion();
				}
			}
			else if(fifos[3] == 2)
			{
				if(mode_backup != DELAY_SET)
				{
					mode_backup = DELAY_SET;
					delay_p.DIR = A_TO_B;
					delay_p.locuHH.dir = delay_p.DIR;
					delay_p.locuVV.dir = delay_p.DIR;
					Delay_mode_Dir_check(MOTOR_HORITAL);
					Delay_mode_Dir_check(MOTOR_Vertical);
					Delay_mode_find_Apoint();
					//Send_connect_data_to_controller();
				}
				Send_connect_data_to_controller();
				if(delay_p.ab_set_if)Delay_total_time_calculate();
			}
			else if(fifos[3] == 0x04) // 主菜界面
			{
				mode_backup = MAIN_ID;
				motorVV_stop();
				motorHH_stop();
				standard_mode_para_init();
				Video_mode_para_clear();
				Delay_mode_para_clear();
				spe_mode_clear();
				write_flash_active();
				//para_read_from_flash();
				Manul_p.HH_UpRoDown = 0;
				Manul_p.VV_UpRoDown = 0;
			}
			else if(fifos[3] == 0x05) //进入标准模式
			{
				Standard_Get_data_from_controller(con_buffer);
			}
			else if(fifos[3] == 0x06) // 进入广角模式
			{
				wideAngle_Get_data_from_controller(con_buffer);
			}
			else if(fifos[3] == 0x03) // 进入原点设置
			{
				mode_backup = SET_ORIGIN;
			}
			else if(fifos[3] == 0x07) // 进入专业模式
			{
				if(mode_backup != PREINSTALL_MODE)
				{
					mode_backup = PREINSTALL_MODE;
					Sp_para_start_init();
					Panorama_mode_find_Apoint();
					//mp_Speed_Load(speed_calculat(100));
					Send_connect_data_to_controller();
					Specialty_Send_reshot_to_controller(1);
					Reshot_Clear();
				}
				
			}
			else if(fifos[3] == 0x08) // 进入全景模式主界面
			{
				if(mode_backup != PANORAMA_MAIN)
				{
					mode_backup = PANORAMA_MAIN;
					Panorama_mode_find_Apoint();
				}
			}
			else if(fifos[3] == 0x09) // 进入AB 点设置模式
			{
				if(mode_backup == VIDEO_MODE)
				{
					video_p.locusH.pluses = 0;
					video_p.locusV.pluses = 0;
					con_b.vvAB_set = 0;
					video_p.locusV.point_a = 0;
					video_p.locusH.point_a = 0;
				}
				else if(mode_backup == DELAY_SET)
				{
					delay_p.ab_set_if = 0;
					delay_p.locuHH.pulse = 0;
					delay_p.locuVV.pulse = 0;
					delay_p.locuHH.or_point = 0;
					delay_p.locuVV.or_point = 0;
				}
			}
			else if(fifos[3] == 0x0a)
			{
				mode_backup = MANUAL_MODE;
				if(motorHH_p.pulse_count == ORIGIN_POINT)ManulHH_dir_cehck = A_TO_B;
				else ManulHH_dir_cehck = motorHH_p.DIR;

				if(motorVV_p.pulse_count == ORIGIN_POINT)ManulVV_dir_cehck = A_TO_B;
				else ManulVV_dir_cehck = motorVV_p.DIR;
				
				Manaul_speed_conversion();
				Send_connect_data_to_controller();
				angle_snedt_tag = 1;
				angle_snedt_time = 0;
				Angle_real_time_transmission(1);
				angle_snedt_tag = 0;
			}
		}
		else if(fifos[1] == 0x09) // 初始化所有参数
		{
			specialty_mode_para_clear();
			delay_mode_para_clear();
			vidoe_mode_para_clear();
			if_write_flash();
			write_flash_active();
		}
		else if(fifos[1] == 0x0b) // 开关快门
		{
			Manul_Turn_the_shuter();
		}
	}
}

void battery_send_to_controller(uint16_t bats)
{
	static uint8_t timess = 0;
	uint8_t bufer[8] ={0};
if(Sync_sendt || Alon_sendt)
{
	timess = 0;
	return;
}
	timess += 1;

	if(timess > 2)
	{
		timess = 0;
		bufer[0] = 0x09;
		bufer[1] = 0x03;
		bufer[2] = 0x06;
		bufer[3] = 0x00;
		bufer[4] = bats;
		bufer[5] = bats>>8;
		bufer[6] = Battery_percentage;
		bufer[7] = check_sum_add(7, bufer);
		Package_dataBufer(8, bufer);
	}
}

void set_abpoint_slow_stopORstart(MOTOR_TYPE motor_t)
{
	if(motor_t == MOTOR_HORITAL)
	{
		if(find_pata.HHfind_Apoint==0)return;

		//if(LEFT_REACTION == 0)return;
		if(find_pata.find_abpoint == 0)return;
		
		if(standard_p.HH_Ttime)return;
		
		standard_p.HH_Ttime = 15;
		
		if(find_pata.find_abpoint == SLOW_START)
		{
			//motor_speed_set(dynamic_speed);
			
			if(find_pata.speed_add > BASE_SPEED)
			{
				find_pata.speed_add--;
				motorHH_speed_set(find_pata.speed_add);
			}
			else 
			{
				find_pata.find_abpoint = 0;
			}
			find_pata.slow_pulse += (float)15 / ((float)(find_pata.speed_add + 1) * 501.0 / 48000.0);
		}
		else if(find_pata.find_abpoint == SLOW_STOP)
		{
			if(find_pata.speed_add < SLOW_THRESHOLD)
			{
				find_pata.speed_add++;
				motorHH_speed_set(find_pata.speed_add);
			}
			else 
			{
				find_pata.find_abpoint = 0;
			}
		}
	}
	/*else if(motor_t == MOTOR_Vertical)
	{
		if(find_pataVV.HHfind_Apoint==0)return;

		//if(LEFT_REACTION == 0)return;
		if(find_pataVV.find_abpoint == 0)return;
		
		if(standard_p.VV_Time)return;
		
		standard_p.VV_Time = 15;
		
		if(find_pataVV.find_abpoint == SLOW_START)
		{
			//motor_speed_set(dynamic_speed);
			
			if(find_pataVV.speed_add > BASE_SPEED)
			{
				find_pataVV.speed_add--;
				motorVV_speed_set(find_pataVV.speed_add);
			}
			else 
			{
				find_pataVV.find_abpoint = 0;
			}
			find_pataVV.slow_pulse += (float)15 / ((float)(find_pataVV.speed_add + 1) * 501.0 / 48000.0);
		}
		else if(find_pataVV.find_abpoint == SLOW_STOP)
		{
			if(find_pataVV.speed_add < SLOW_THRESHOLD)
			{
				find_pataVV.speed_add++;
				motorVV_speed_set(find_pataVV.speed_add);
			}
			else 
			{
				find_pataVV.find_abpoint = 0;
			}
		}
	}*/

}

void angle_accuracy_test(void)
{
	static uint32_t test_angle = 0;

	test_angle += 1;

	if(test_angle >= 192000)
	{
		test_angle = 0;
		//motorHH_stop();
		motorVV_stop();
	}
}

void HHangle_accuracy_test(void)
{
	static uint32_t test_angleS = 0;

	test_angleS += 1;

	if(test_angleS >= 192100)
	{
		test_angleS = 0;
		motorHH_stop();
		//motorVV_stop();
	}
}

void para_init(void)
{
	memset((void *)&glob_para.read_fisrt, 0, sizeof(glob_para));
	sp_Factory_default();
}

// 两个值相减，取绝对值
uint32_t Absolute_value_calculation(uint32_t a, uint32_t b)
{
	uint32_t temps = 0;
	if(a > b)
	{
		temps = a - b;
	}
	else
	{
		temps = b - a;
	}

	return temps;
}	

void Send_para_data_to_controller(uint8_t ipcode, uint8_t datas)
{
	uint8_t buffers[20] = {0};
	uint8_t sizes = 0;

	buffers[0] = 0x09;
	buffers[1] = ipcode;
	buffers[2] = 0x05;
	buffers[3] = datas;

	if(ipcode == 1) // ab 点 设置状态
	{
		if(datas == 1)
		{
			if(mode_backup == VIDEO_MODE)
			{
				buffers[4] = con_b.vvAB_set;
			}
			else
			{
				buffers[4] = delay_p.ab_set_if;
			}
			buffers[5] = check_sum_add(5, buffers);
			sizes = 6;
		}
	}
	
	Package_dataBufer(sizes, buffers);
}

////////////////  manul mode ////////////////////////////

// 手动模式缓起停的时间变化
void manaul_slow_timeSet(MOTOR_TYPE motor_t)
{
	if(motor_t == MOTOR_HORITAL)
	{
		if(Manul_p.HH_speed < 4)
		{
			Manul_p.HH_Ttime = 100;
		}
		else if(Manul_p.HH_speed < 9)
		{
			Manul_p.HH_Ttime = 80;
		}
		else if(Manul_p.HH_speed < 16)
		{
			Manul_p.HH_Ttime = 55;
		}
		else if(Manul_p.HH_speed < 33)
		{
			Manul_p.HH_Ttime = 35;
		}
		else
		{
			Manul_p.HH_Ttime = 10;
		}
	}
	else
	{
		if(Manul_p.VV_speed < 4)
		{
			Manul_p.VV_Time = 100;
		}
		else if(Manul_p.VV_speed < 9)
		{
			Manul_p.VV_Time = 80;
		}
		else if(Manul_p.VV_speed < 16)
		{
			Manul_p.VV_Time = 55;
		}
		else if(Manul_p.VV_speed < 33)
		{
			Manul_p.VV_Time = 35;
		}
		else
		{
			Manul_p.VV_Time = 10;
		}
	}
}

void Manual_HH_slowStartOrStop(void)
{
	uint8_t hhspeed_t = 0;
	if(mode_backup != MANUAL_MODE && mode_backup != SET_ORIGIN && mode_backup != VIDEO_MODE && mode_backup != DELAY_SET)return;
//	if(Manul_p.VVfind_Apoint || find_pata.HHfind_Apoint)
	{
		//if(LEFT_REACTION == 0)return;
		if(Manul_p.HH_UpRoDown == 0)return;
		
		if(Manul_p.HH_Ttime)return;
		
		if(mode_backup == MANUAL_MODE)
		{
			if((glob_para.speed < 1) || (glob_para.speed>4))return;
			hhspeed_t = speed_Gear[glob_para.speed-1];
		}
		else
		{
			hhspeed_t = BASE_SPEED;
		}

		if(Manul_p.HH_UpRoDown == SLOW_START)
		{
			//motor_speed_set(dynamic_speed);
			
			if(Manul_p.HH_speed > hhspeed_t)
			{
				Manul_p.HH_speed--;
				//motorVV_speed_set(find_pata.speed_add);
				motorHH_speed_set(Manul_p.HH_speed);
			}
			else 
			{
				Manul_p.HH_UpRoDown = 0;
			}
		}
		else if(Manul_p.HH_UpRoDown == SLOW_STOP)
		{
			if(Manul_p.HH_speed < SLOW_THRESHOLD)
			{
				Manul_p.HH_speed++;
				//motorVV_speed_set(find_pata.speed_add);
				motorHH_speed_set(Manul_p.HH_speed);
				Angle_real_time_transmission(0);
			}
			else 
			{
				Manul_p.HH_UpRoDown = 0;
				//if(mode_backup == MANUAL_MODE)motorHH_stop();
				motorHH_stop();
				angle_snedt_time = 0;
				Angle_real_time_transmission(0);
			}
		}
	}
	
	manaul_slow_timeSet(MOTOR_HORITAL);
}

void Manual_VV_slowStartOrStop(void)
{
	uint8_t speed_t = 0;
	if(mode_backup != MANUAL_MODE && mode_backup != SET_ORIGIN && mode_backup != VIDEO_MODE && mode_backup != DELAY_SET)return;
//	if(Manul_p.VVfind_Apoint || find_pata.HHfind_Apoint)
	{
		//if(LEFT_REACTION == 0)return;
		if(Manul_p.VV_UpRoDown == 0)return;
		
		if(Manul_p.VV_Time)return;
		
		if(mode_backup == MANUAL_MODE)
		{
			if((glob_para.speed < 1) || (glob_para.speed>4))return;
			speed_t = speed_Gear[glob_para.speed-1];
		}
		else
		{
			speed_t = BASE_SPEED;
		}

		if(Manul_p.VV_UpRoDown == SLOW_START)
		{
			//motor_speed_set(dynamic_speed);
			
			if(Manul_p.VV_speed > speed_t)
			{
				Manul_p.VV_speed--;
				//motorVV_speed_set(find_pata.speed_add);
				motorVV_speed_set(Manul_p.VV_speed);
				//String_Printf(USART_2,&Manul_p.VV_speed,1);
			}
			else 
			{
				Manul_p.VV_UpRoDown = 0;
			}
		}
		else if(Manul_p.VV_UpRoDown == SLOW_STOP)
		{
			if(Manul_p.VV_speed < SLOW_THRESHOLD)
			{
				Manul_p.VV_speed++;
				//motorVV_speed_set(find_pata.speed_add);
				motorVV_speed_set(Manul_p.VV_speed);
				Angle_real_time_transmission(0);
			}
			else 
			{
				Manul_p.VV_UpRoDown = 0;
				if(mode_backup == MANUAL_MODE)motorVV_stop();
				angle_snedt_time = 0;
				Angle_real_time_transmission(0);
			}
		}
	}
	
	//angle_snedt_time = 0;
	manaul_slow_timeSet(MOTOR_Vertical);
}

//固定速度换算
void Manaul_speed_conversion(void)
{
	float timess = 0;
	
	// 按360 度的长度计算对应的时间换算成定时器的p
	timess = 6100 / PULSE_SUM(360); // 6.1 秒换算成速度p, 未加缓起停时间
	speed_Gear[3] = timess * (double)48000 / (double)501 - 1;
	
	timess = 21100 / PULSE_SUM(360);
	speed_Gear[2] = timess * (double)48000 / (double)501 - 1;
	
	timess = 36100 / PULSE_SUM(360);
	speed_Gear[1] = timess * (double)48000 / (double)501 - 1;
	
	timess = 51100 / PULSE_SUM(360);
	speed_Gear[0] = timess * (double)48000 / (double)501 - 1;
	
	//String_Printf(USART_2,speed_Gear,4);
}

void Manul_SartOrStop(MOTOR_TYPE motor_t, uint8_t status)
{
	if(motor_t == MOTOR_HORITAL)
	{
		if(status)
		{
			//if(mode_backup == VIDEO_MODE)
			{
				Manul_p.HH_speed = SLOW_THRESHOLD;
				Manul_p.HH_UpRoDown = SLOW_START;
				motorHH_speed_set(Manul_p.HH_speed);
				motorHH_start();
				//String_Printf(USART_2,"AA",2);
			}
		}
		else
		{
			//if(mode_backup == MANUAL_MODE)
			{
				//Manul_p.HH_speed = SLOW_THRESHOLD;
				Manul_p.HH_UpRoDown = SLOW_STOP;
			}
			/*else
			{
				motorHH_stop();
				Manul_p.HH_UpRoDown = 0;
			}*/
		}
	}
	/*else
	{
		if(status)
		{
				Manul_p.VV_speed = SLOW_THRESHOLD;
				Manul_p.VV_UpRoDown = SLOW_START;
				motorVV_speed_set(Manul_p.VV_speed);
				motorVV_start();
				//String_Printf(USART_2,"AA",2);
		}
		else
		{
			if(mode_backup == MANUAL_MODE)
			{
				//Manul_p.HH_speed = SLOW_THRESHOLD;
				Manul_p.VV_UpRoDown = SLOW_STOP;
			}
			else
			{
				motorVV_stop();
				Manul_p.VV_UpRoDown = 0;
			}
		}
	}*/
}

void Red_led_tack(void)
{
	//RED_LED_ON;
	//Green_led_onORoff(0);
}

//视频模式和延时模式设置AB点时的实时角度发送
void Angle_real_time_transmission(uint8_t tpes)
{
	uint32_t V_pulse = 0;
	uint32_t H_pulse = 0;
	uint8_t buffers[10] = {0};
	
	if(mode_backup != VIDEO_MODE && mode_backup != DELAY_SET && mode_backup != MANUAL_MODE)return;
	if(tpes)if(angle_snedt_tag==0)return;
	if(angle_snedt_time)return;

	angle_snedt_time = 200;
	if(mode_backup == VIDEO_MODE)
	{
	//	V_pulse = Absolute_value_calculation(video_p.locusV.point_pulse_a, motorVV_p.DVpulse_count) * Angle_basic;
		H_pulse = Absolute_value_calculation(video_p.locusH.point_pulse_a, motorHH_p.DVpulse_count) * Angle_basic;
	}
	else if(mode_backup == DELAY_SET)
	{
	//	V_pulse = Absolute_value_calculation(delay_p.locuVV.point_a , motorVV_p.DLLpulse_count) * Angle_basic;
		H_pulse = Absolute_value_calculation(delay_p.locuHH.point_a, motorHH_p.DLLpulse_count) * Angle_basic;
	}
	else
	{
	/*	if(ManulVV_dir_cehck == A_TO_B)
		{
			V_pulse = Absolute_value_calculation(ORIGIN_POINT , motorVV_p.pulse_count) * Angle_basic;
		}
		else
		{
			V_pulse = Absolute_value_calculation(B_POINT_END , motorVV_p.pulse_count) * Angle_basic;
		}
*/
		
		if(ManulHH_dir_cehck == A_TO_B)
		{
			H_pulse = Absolute_value_calculation(ORIGIN_POINT, motorHH_p.pulse_count) * Angle_basic;
		}
		else
		{
			H_pulse = Absolute_value_calculation(B_POINT_END, motorHH_p.pulse_count) * Angle_basic;
		}
	}

	buffers[0] = 0x09;
	buffers[1] = 0x08;
	buffers[2] = 0x09;
	buffers[3] = mode_backup;
	buffers[4] = V_pulse;
	buffers[5] = V_pulse>>8;
	
	buffers[6] = H_pulse;
	buffers[7] = H_pulse>>8;
	
	buffers[8] = check_sum_add(8, buffers);
	Package_dataBufer(9, buffers);
}

/////////////////////////////////  手动模式//////////////////////////////
void Manual_mode_dir_set(uint8_t ttemp)
{
	if(mode_backup != MANUAL_MODE)return;

	if(ttemp == 2)
	{
		if(motorVV_p.pulse_count == ORIGIN_POINT)
		{
			ManulVV_dir_cehck = motorVV_p.DIR;
		}
	}
	else
	{
		if(motorHH_p.pulse_count == ORIGIN_POINT)
		{
			ManulHH_dir_cehck = motorHH_p.DIR;
		}
	}
}
void Manu_mode_dir_check(MOTOR_TYPE motor_t)
{
	if(motor_t == MOTOR_HORITAL)
	{
		if(motorHH_p.DIR == A_TO_B)
		{
			ManulHH_dir_cehck = A_TO_B;
		}
		else
		{
			ManulHH_dir_cehck = B_TO_A;
		}
	}
	else if(motor_t == MOTOR_Vertical)
	{
		if(motorVV_p.DIR == A_TO_B)
		{
			ManulVV_dir_cehck = A_TO_B;
		}
		else
		{
			ManulVV_dir_cehck = B_TO_A;
		}
	}
}
void ManulHH_mode_pulse_check(void)
{
	if(mode_backup != MANUAL_MODE)return;
	if(motorHH_p.pulse_count == ORIGIN_POINT)
	{
		Manu_mode_dir_check(MOTOR_HORITAL);
	}
}

void ManulVV_mode_pulse_check(void)
{
	if(mode_backup != MANUAL_MODE)return;
	if(motorVV_p.pulse_count == ORIGIN_POINT)
	{
		Manu_mode_dir_check(MOTOR_Vertical);
	}
}

void All_slow_check(void)
{
	Panorama_find_end_point_slowChcek(MOTOR_HORITAL);
	Panorama_find_end_point_slowChcek(MOTOR_Vertical);
	
	Video_find_Origin_slow_check(MOTOR_HORITAL);
	Video_find_Origin_slow_check(MOTOR_Vertical);
	
	Delay_find_Origin_slow_check(MOTOR_HORITAL);
	Delay_find_Origin_slow_check(MOTOR_Vertical);
}

