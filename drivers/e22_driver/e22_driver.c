/****************************************************************************
** e22_driver.c
**
** Copyright (c) Siargo Ltd. 2020
** All Rights Reserved.
**
** BY HYY
**
** ʵ��e22_driver�ĵײ�����
**
**
** �ļ���ʷ
** 2020-07-23 ����Ԫ����
** 2020-07-24 ���ģʽ�жϺ����ݷ��ͺ���
*****************************************************************************/

/* Includes ----------------------------------------------------------------*/
#include "e22_driver.h"
#include "usart.h"
#include "delay.h"
/* ������� ----------------------------------------------------------------*/ 
u8 LORA_RXBuf[COMM_MAX_SIZE];
/* �������� ----------------------------------------------------------------*/


/* --------------------------- �û����� ------------------------------------*/

//////////////////////////////////////////////////////////////////
//�������´���,֧��printf����,������Ҫѡ��use MicroLIB	  
#if 0
#pragma import(__use_no_semihosting)             
//��׼����Ҫ��֧�ֺ���                 
struct __FILE 
{ 
	int handle; 

}; 

FILE __stdout;       
//����_sys_exit()�Ա���ʹ�ð�����ģʽ    
void _sys_exit(int x) 
{ 
	x = x; 
} 
//�ض���fputc���� 
int fputc(int ch, FILE *f)
{      
	while((USART1->ISR&0X40)==0);//ѭ������,ֱ���������   
    USART1->TDR = (uint8_t) ch;      
	return ch;
}
#endif 


/**
	* @brief 	E22���ڷ���
  * @note   ȷ��E22�Ķ�Ӧ���ںʹ��ڷ��ͺ�������ֲʱ��Ҫ�޸�
  * @param 	��Ҫ���͵�����ָ��
  * @param 	��Ҫ���͵��ֽڳ���
  * @retval HAL status
  */
HAL_StatusTypeDef E22_usart_tx(uint8_t *pData, uint16_t Size)
{
	return HAL_UART_Transmit(&huart2, pData, Size, 1000);
}

/**
	* @brief 	E22���ڽ���ʹ��
  * @note   ȷ��E22�Ķ�Ӧ���ںʹ��ڽ���ʹ�ܺ�������ֲʱ��Ҫ�޸�
  * @param 	�������ݵ�����ָ��
  * @param 	���յ��ֽڳ���
  * @retval HAL status
  */
HAL_StatusTypeDef E22_usart_rx_enable(uint8_t *pData, uint16_t Size)
{
	return HAL_UART_Receive_DMA(&huart2, pData, Size);
}

/**
	* @brief 	E22���ڽ���ʧ��
  * @note   ȷ��E22�Ķ�Ӧ���ںʹ��ڽ���ʧ�ܺ�������ֲʱ��Ҫ�޸�
  * @retval HAL status
  */
HAL_StatusTypeDef E22_usart_rx_disable(void)
{
	return HAL_UART_DMAStop(&huart2);
}

/**
	* @brief E22���üĴ���
  * @note   ����E22�ļĴ�������
  * @param �Ĵ�������
  *          This parameter can be one of the following values:
  *            @arg E22_NORMAL_REG
  *            @arg E22_READ_REG
  *            @arg E22_TEMP_REG
  *            @arg E22_WIRELESS_REG
  * @param �Ĵ�����ʼ��ַ
  * @param �Ĵ�������
  * @param �Ĵ�������
  * @retval HAL status
  */
HAL_StatusTypeDef E22_reg_set(u8 RegClass, enum E22_REG Reg, u8 *pData, u16 Size)
{
	u8	HeadBuf[64];
	u8  HeadBufSize, i;
	if(E22_mode_get() != E22_MODE_SET)
	{
		E22_mode_set(E22_MODE_SET);
	}
		
	switch(RegClass)
	{
		case E22_NORMAL_REG: 
			HeadBufSize = 3;
			HeadBuf[0] = E22_NORMAL_REG;
			HeadBuf[1] = Reg;
			HeadBuf[2] = Size;
			break;
		case E22_TEMP_REG: 
			HeadBufSize = 3;
			HeadBuf[0] = E22_TEMP_REG;
			HeadBuf[1] = Reg;
			HeadBuf[2] = Size;
			break;
		case E22_WIRELESS_REG: 
			HeadBufSize = 5;
			HeadBuf[0] = E22_WIRELESS_REG;
			HeadBuf[1] = E22_WIRELESS_REG;
			HeadBuf[2] = E22_NORMAL_REG;
			HeadBuf[3] = Reg;
			HeadBuf[4] = Size;
			break;
		default : return HAL_ERROR;
	} 
	
	for(i = 0; i < Size; i++)
	{
		HeadBuf[HeadBufSize] = *(pData + i);
		HeadBufSize++;
	}
	
	if(E22_usart_tx(HeadBuf, HeadBufSize) != HAL_OK)
		return HAL_ERROR;
	Delay_ms(100);
	if((LORA_RXBuf[0] == 0XFF) && (LORA_RXBuf[1] == 0XFF) && (LORA_RXBuf[2] == 0XFF))
		return HAL_ERROR;
//	printf(" E22 SetReg\r\n");
	return HAL_OK;
}

/**
	* @brief E22���Ĵ���
  * @note   ��E22�ļĴ�������
  * @param �Ĵ�����ʼ��ַ
  * @param �Ĵ�������
  * @param �Ĵ�������
  * @retval HAL status
  */
HAL_StatusTypeDef E22_reg_get(enum E22_REG Reg, u8 *pData, u16 Size)
{
	u8	HeadBuf[3] = {E22_READ_REG, Reg, Size};
	u8  HeadBufSize = 3;
	u8 i;
	if(E22_mode_get() != E22_MODE_SET)
	{
		E22_mode_set(E22_MODE_SET);
	}
	
	if(E22_usart_tx(HeadBuf, HeadBufSize) != HAL_OK)
		return HAL_ERROR;
	if(E22_usart_tx(pData, Size) != HAL_OK)
		return HAL_ERROR;
	Delay_ms(100);
	for(i = 0; i < Size; i++)
	{
		*(pData+i) = LORA_RXBuf[i];
	}
	return HAL_OK;
}

/**
	* @brief E22����ģʽ
  * @note   ����E22ģʽ
  * @param E22ģʽ
  *          This parameter can be one of the following values:
  *            @arg E22_MODE_NORMAL
  *            @arg E22_MODE_WOR
  *            @arg E22_MODE_SET
  *            @arg E22_MODE_SLEEP
  * @retval void
  */
void E22_mode_set(enum E22_MODE mode)
{
	switch(mode)
	{
		case E22_MODE_NORMAL: E22_M0_SET_0 E22_M1_SET_0 break;
		case E22_MODE_WOR: 		E22_M0_SET_1 E22_M1_SET_0 break;
		case E22_MODE_SET: 		E22_M0_SET_0 E22_M1_SET_1 break;
		case E22_MODE_SLEEP: 	E22_M0_SET_1 E22_M1_SET_1 break;
		default : return;
		
	}
//	printf(" E22 ModeSet\r\n");
	Delay_ms(1000);
	
}

/**
	* @brief E22��ȡģʽ����
  * @note   ��ȡģ��ģʽ
  * @retval ģ��ģʽ
  */
enum E22_MODE E22_mode_get(void)
{
	if(E22_M0_GET == GPIO_PIN_RESET)
	{
		if(E22_M1_GET == GPIO_PIN_RESET)
			return E22_MODE_NORMAL;
		else
			return E22_MODE_SET;
	}
	else
	{
		if(E22_M1_GET == GPIO_PIN_RESET)
			return E22_MODE_WOR;
		else
			return E22_MODE_SLEEP;
	}
}

/**
	* @brief E22��ʼ������
  * @note   ��ʼ��E22����
  * @param E22��ʼ���ṹ��
  * @retval HAL status
  */
HAL_StatusTypeDef E22_init(LORA_InitTypeDef Lora_Init)
{
	u8 InitTxBuff[] = {(u8)(Lora_Init.ADD>>8), (u8)Lora_Init.ADD, Lora_Init.Netid, 
									Lora_Init.BaudRate| Lora_Init.Parity| Lora_Init.AirRate, 
									Lora_Init.Frame| Lora_Init.RSSI| Lora_Init.TransmitPower, 
									Lora_Init.Channel, Lora_Init.RSSIByte| Lora_Init.TransmitPower| Lora_Init.Relay| Lora_Init.LBT| Lora_Init.WORMod| Lora_Init.WORTime, 
									(u8)(Lora_Init.CRYPT>>8), (u8)Lora_Init.CRYPT};
	
	if(E22_reg_set(E22_NORMAL_REG, E22_REG_ADDH, InitTxBuff, sizeof(InitTxBuff)) != HAL_OK)
		return HAL_ERROR;
	E22_mode_set(E22_MODE_NORMAL);
//	printf(" E22 Init is Finish\r\n");
	return HAL_OK;
}

/**
	* @brief E22��������
	* @note   ��������
	* @param ��������
	* @param ���ͳ���
  * @retval HAL status
  */
HAL_StatusTypeDef E22_data_send(u8 *pData, u16 Size)
{
		if(E22_mode_get() != E22_MODE_NORMAL)
	{
		E22_mode_set(E22_MODE_NORMAL);
	}
//	printf(" E22 SentData\r\n");
	return E22_usart_tx(pData, Size);
}

/************************ Copyright (c) Siargo Ltd. 2020 *****END OF FILE****/
