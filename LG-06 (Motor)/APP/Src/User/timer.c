#include "stm32f10x_tim.h"


void NVIC_Configuration(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);                   //����������
	NVIC_InitStructure.NVIC_IRQChannel =TIM2_IRQn ;           //TIM2�ж�ѡͨ
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;      //��ռ���ȼ�
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;           //�����ȼ�
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;         //ʹ���жϿ���
	NVIC_Init(&NVIC_InitStructure);
}

void time_2_init(void)
{	
	//TIM_DeInit(TIM1);
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure; 
	
	NVIC_Configuration();

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
	
	//	��ʱʱ��T���㹫ʽ�� 		   
	//  T=(TIM_Period+1)*(TIM_Prescaler+1)/TIMxCLK=(35999+1)*(1999+1)/48MHz=1s
	TIM_TimeBaseStructure.TIM_Period = 23999;
	TIM_TimeBaseStructure.TIM_Prescaler = 1;
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; 
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	
	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseStructure); 
	TIM_ClearFlag(TIM2,TIM_FLAG_Update);
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE); //��ʱ�������ж�ʹ��
	TIM_Cmd(TIM2,ENABLE);
	
}


