



void multilayer_get_from_controller(uint8_t *fofiss)
{
	if(fofiss[4] == 1) //����
	{
		glob_para.SLayer.Amount = (uint16_t)fofiss[5]|((uint16_t)fofiss[6]<<8);

		if(glob_para.SLayer.Amount<1)glob_para.SLayer.Amount=1;

	}
	else if(fofiss[4] == 2) //��ͣʱ��
	{
		glob_para.SLayer.ramp_stop = (uint16_t)fofiss[5]|((uint16_t)fofiss[6]<<8);
		if(glob_para.SLayer.ramp_stop<500)glob_para.SLayer.ramp_stop=500;
		
		
	}
	else if(fofiss[4] == 3) // �����ٶ�
	{
		glob_para.SLayer.st_time_s = (uint16_t)fofiss[5]|((uint16_t)fofiss[6]<<8);
		if(glob_para.SLayer.st_time_s<500)glob_para.SLayer.st_time_s=500;
	}
	else if(fofiss[4] == 4) // ��ʱ����
	{
		glob_para.SLayer.delay_time = (uint16_t)fofiss[5]|((uint16_t)fofiss[6]<<8);
	}
	else if(fofiss[4] == 5) // �ֶ�����
	{
		glob_para.SLayer.Manual = fofiss[5];
	}
	else if(fofiss[4] == 6) // ���Ŵ���
	{
		glob_para.SLayer.shutTimes = fofiss[5];
		if(glob_para.SLayer.shutTimes<1)glob_para.SLayer.shutTimes=1;
	}
	else if(fofiss[4] == 7) // ��ʱʱ��
	{
		glob_para.SLayer.sys_stop = (uint16_t)fofiss[5]|((uint16_t)fofiss[6]<<8);
		if(glob_para.SLayer.sys_stop<500)glob_para.SLayer.sys_stop=500;
	}
	else if(fofiss[4] == 8) // �ٶȵ�λ
	{
		glob_para.SLayer.speed_range = fofiss[5];
		if(glob_para.SLayer.speed_range<1)glob_para.SLayer.speed_range=1;
	}
	
}
