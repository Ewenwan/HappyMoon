/******************** (C) COPYRIGHT 2015-2017 Xiluna Tech ************************
 * 作者 	:Xiluna Tech
 * 文件名 :mcpwm.c
 * 描述   :电机PWM配置函数
 * 官网   :http://xiluna.com/
 * 公众号 :XilunaTech
**********************************************************************************/
#include "mcpwm.h"

//定时器初始化函数
void PWM_Init(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8, ENABLE); 
	
	GPIO_InitTypeDef GPIO_InitStructure;
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE); 

  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9;		   
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; 
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOC, &GPIO_InitStructure);

  GPIO_PinAFConfig(GPIOC,GPIO_PinSource6,GPIO_AF_TIM8);
  GPIO_PinAFConfig(GPIOC,GPIO_PinSource7,GPIO_AF_TIM8);
  GPIO_PinAFConfig(GPIOC,GPIO_PinSource8,GPIO_AF_TIM8);
  GPIO_PinAFConfig(GPIOC,GPIO_PinSource9,GPIO_AF_TIM8);	
	
  TIM_TimeBaseStructure.TIM_Period = 2000;      							//PWM周期
  TIM_TimeBaseStructure.TIM_Prescaler = 168-1;    						//设置预分频
  TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;			//设置时钟分频系数：不分频
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //向上计数模式
  TIM_TimeBaseInit(TIM8, &TIM_TimeBaseStructure);

  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;						//配置为PWM模式1
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;		//计数值小于该值时为高电平
 
	TIM_OCInitStructure.TIM_Pulse = 1000;
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;		
	TIM_OC1Init(TIM8, &TIM_OCInitStructure);	 //使能通道1
  TIM_OC1PreloadConfig(TIM8, TIM_OCPreload_Enable);
		
	TIM_OCInitStructure.TIM_Pulse = 1000;	 	
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OC2Init(TIM8, &TIM_OCInitStructure);	  //使能通道2
  TIM_OC2PreloadConfig(TIM8, TIM_OCPreload_Enable);
	
	TIM_OCInitStructure.TIM_Pulse = 1000;	 	
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OC3Init(TIM8, &TIM_OCInitStructure);	 //使能通道3
  TIM_OC3PreloadConfig(TIM8, TIM_OCPreload_Enable);
	
	TIM_OCInitStructure.TIM_Pulse = 1000;	 	
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OC4Init(TIM8, &TIM_OCInitStructure);	//使能通道4
  TIM_OC4PreloadConfig(TIM8, TIM_OCPreload_Enable);
  
	TIM_ARRPreloadConfig(TIM8, ENABLE);			 // 使能TIM8重载寄存器ARR
	TIM_CtrlPWMOutputs(TIM8, ENABLE);
  TIM_Cmd(TIM8, ENABLE);                   //使能定时器8
}



