/**********************************************************************************************************
 * @文件     board.c
 * @说明     硬件初始化
 * @作者     YuyingJin
 * @网站     https://yuyingjin0111.github.io/
 * @日期     2018 ~
**********************************************************************************************************/
#include "Board.h"
/**********************************************************************************************************
*函 数 名: BoardInit
*功能说明: 飞控硬件初始化
*形    参: 无
*返 回 值: 无
**********************************************************************************************************/
void Board_Init(void){
	// 时钟初始化
	Systick_Init(168);								                       
	// 中断分组配置
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	
	// 延时2s等待上电
	delay_ms(2000);
	// ADC初始化
	Adc_Init();
	delay_ms(100);
	// 用于与高性能板载ARM或板载PC通信 波特率230400
	Usart1toOnboardPC_Init(230400); 
	delay_ms(100);
	// 蓝牙串口打开，用于与地面站通信
	Usart3toBluetooth_Init(115200);
	delay_ms(100);
	// TOF传感器串口打开
	Uart4toTOF_Init(115200);
	delay_ms(100);
	// GPIO初始化 一些通用GPIO口可以在这里配置
	GeneralGpio_Init(); 	
	delay_ms(100);
#ifdef SpeedyBeeF4
	// SPI1初始化 用于陀螺仪和加速计读取 MPU6000
	SPI1_Configuration();
#else
	// SPI3初始化 用于陀螺仪和加速计读取 MPU6500
	SPI3_Configuration();
#endif
	delay_ms(100);
//	// USB转串口初始化
//  Usb_Init();
}


