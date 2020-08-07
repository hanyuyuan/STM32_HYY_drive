/****************************************************************************
** delay.c
**
** Copyright (c) Siargo Ltd. 2020
** All Rights Reserved.
**
** BY HYY
**
** ʵ����ʱ����,���ô��ӳٺ������Խ���ж��е���HAL_Delay()������bug
**
**
** �ļ���ʷ
** 2020-07-28 ����Ԫ����
*****************************************************************************/

/* Includes ----------------------------------------------------------------*/
#include "delay.h"
/* ������� ----------------------------------------------------------------*/ 
static u32 fac_us=0;							//us��ʱ������
/* �������� ----------------------------------------------------------------*/


/* --------------------------- �û����� ------------------------------------*/
/**
	* @brief 	delay������ʼ������
  * @note  	�����Ҫʹ�ô�delay��������Ҫ�ȳ�ʼ��
  * @param 	ϵͳʱ�ӣ���λM��
  * @retval ��
  */
void delay_init(u8 SYSCLK)
{
	HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);
	fac_us = SYSCLK;
}

/**
	* @brief 	΢�뼶��ʱ
  * @note  	��ʱn΢��
  * @param 	��ʱʱ�䣨us��
  * @retval ��
  */
void delay_us(u32 nus)
{		
	u32 ticks;
	u32 told,tnow,tcnt=0;
	u32 reload=SysTick->LOAD;				//LOAD��ֵ	    	 
	ticks=nus*fac_us; 						//��Ҫ�Ľ����� 
	told=SysTick->VAL;        				//�ս���ʱ�ļ�����ֵ
	while(1) {
		tnow=SysTick->VAL;	
		if(tnow!=told) {	    
			if(tnow<told)
				tcnt+=told-tnow;	//����ע��һ��SYSTICK��һ���ݼ��ļ������Ϳ�����.
			else 
				tcnt+=reload-tnow+told;	    
			told=tnow;
			if(tcnt>=ticks)
				break;			//ʱ�䳬��/����Ҫ�ӳٵ�ʱ��,���˳�.
		}  
	}
}

/**
	* @brief 	���뼶��ʱ
  * @note  	��ʱn����
  * @param 	��ʱʱ�䣨ms��
  * @retval ��
  */
void delay_ms(u16 nms)
{
	u32 i;
	for(i=0;i<nms;i++) 
		delay_us(1000);
}
/************************ Copyright (c) Siargo Ltd. 2020 *****END OF FILE****/
