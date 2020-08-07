/****************************************************************************
** delay.c
**
** Copyright (c) Siargo Ltd. 2020
** All Rights Reserved.
**
** BY HYY
**
** 实现延时函数,利用此延迟函数可以解决中断中调用HAL_Delay()卡死的bug
**
**
** 文件历史
** 2020-07-28 韩堉元创建
*****************************************************************************/

/* Includes ----------------------------------------------------------------*/
#include "delay.h"
/* 定义变量 ----------------------------------------------------------------*/ 
static u32 fac_us=0;							//us延时倍乘数
/* 函数定义 ----------------------------------------------------------------*/


/* --------------------------- 用户代码 ------------------------------------*/
/**
	* @brief 	delay函数初始化程序
  * @note  	如果需要使用此delay函数则需要先初始化
  * @param 	系统时钟（单位M）
  * @retval 无
  */
void delay_init(u8 SYSCLK)
{
	HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);
	fac_us = SYSCLK;
}

/**
	* @brief 	微秒级延时
  * @note  	延时n微秒
  * @param 	延时时间（us）
  * @retval 无
  */
void delay_us(u32 nus)
{		
	u32 ticks;
	u32 told,tnow,tcnt=0;
	u32 reload=SysTick->LOAD;				//LOAD的值	    	 
	ticks=nus*fac_us; 						//需要的节拍数 
	told=SysTick->VAL;        				//刚进入时的计数器值
	while(1) {
		tnow=SysTick->VAL;	
		if(tnow!=told) {	    
			if(tnow<told)
				tcnt+=told-tnow;	//这里注意一下SYSTICK是一个递减的计数器就可以了.
			else 
				tcnt+=reload-tnow+told;	    
			told=tnow;
			if(tcnt>=ticks)
				break;			//时间超过/等于要延迟的时间,则退出.
		}  
	}
}

/**
	* @brief 	毫秒级延时
  * @note  	延时n毫秒
  * @param 	延时时间（ms）
  * @retval 无
  */
void delay_ms(u16 nms)
{
	u32 i;
	for(i=0;i<nms;i++) 
		delay_us(1000);
}
/************************ Copyright (c) Siargo Ltd. 2020 *****END OF FILE****/
