/****************************************************************************
** e22_driver.h
**
** Copyright (c) Siargo Ltd. 2020
** All Rights Reserved.
**
** BY HYY
**
** e22_driver.c的宏定义和声明
**
**
** 文件历史
** 2020-07-23 韩堉元创建
*****************************************************************************/
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __e22_driver_H
#define __e22_driver_H
#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "GlobeDef.h"

/* 宏定义 ----------------------------------------------------------------*/
#define E22_M0_SET_0 HAL_GPIO_WritePin(LORA_M0_GPIO_Port, LORA_M0_Pin, GPIO_PIN_RESET);
#define E22_M0_SET_1 HAL_GPIO_WritePin(LORA_M0_GPIO_Port, LORA_M0_Pin, GPIO_PIN_SET);
#define E22_M1_SET_0 HAL_GPIO_WritePin(LORA_M1_GPIO_Port, LORA_M1_Pin, GPIO_PIN_RESET);
#define E22_M1_SET_1 HAL_GPIO_WritePin(LORA_M1_GPIO_Port, LORA_M1_Pin, GPIO_PIN_SET);

#define E22_M0_GET	 HAL_GPIO_ReadPin(LORA_M0_GPIO_Port, LORA_M0_Pin)
#define E22_M1_GET	 HAL_GPIO_ReadPin(LORA_M1_GPIO_Port, LORA_M1_Pin)

#define E22_NORMAL_REG              0XC0
#define E22_READ_REG              	0XC1
#define E22_TEMP_REG                0XC2
#define E22_WIRELESS_REG            0XCF

#define E22_REG0_BAUDREAD_1200      0<<5
#define E22_REG0_BAUDREAD_2400      1<<5
#define E22_REG0_BAUDREAD_4800      2<<5
#define E22_REG0_BAUDREAD_9600      3<<5
#define E22_REG0_BAUDREAD_19200     4<<5
#define E22_REG0_BAUDREAD_38400     5<<5
#define E22_REG0_BAUDREAD_57600     6<<5
#define E22_REG0_BAUDREAD_115200    7<<5

#define E22_REG0_PARITY_8N1         0<<3
#define E22_REG0_PARITY_8O1         1<<3
#define E22_REG0_PARITY_8E1         2<<3

#define E22_REG0_AIRRATE_0_3K       0
#define E22_REG0_AIRRATE_1_2K       1
#define E22_REG0_AIRRATE_2_4K       2
#define E22_REG0_AIRRATE_4_8K       3
#define E22_REG0_AIRRATE_9_6K       4
#define E22_REG0_AIRRATE_19_2K      5
#define E22_REG0_AIRRATE_38_4K      6
#define E22_REG0_AIRRATE_62_5K      7


#define E22_REG1_FRAME_240BYTE      0<<6
#define E22_REG1_FRAME_128BYTE      1<<6
#define E22_REG1_FRAME_64BYTE       2<<6
#define E22_REG1_FRAME_32BYTE       3<<6

#define E22_REG1_RSSI_DISABLE       0<<5
#define E22_REG1_RSSI_ENABLE        1<<5

#define E22_REG1_TP_22DBM           0
#define E22_REG1_TP_17DBM           1
#define E22_REG1_TP_13DBM           2
#define E22_REG1_TP_10DBM           3


#define E22_REG3_RSSIBYTE_DISABLE   0<<7
#define E22_REG3_RSSIBYTE_ENABLE    1<<7

#define E22_REG3_TM_OPEN            0<<6
#define E22_REG3_TM_POINT           1<<6

#define E22_REG3_RELAY_DISABLE      0<<5
#define E22_REG3_RELAY_ENABLE       1<<5

#define E22_REG3_LBT_DISABLE        0<<4
#define E22_REG3_LBT_ENABLE         1<<4

#define E22_REG3_WORMOD_RX          0<<3
#define E22_REG3_WORMOD_TX          1<<3

#define E22_REG3_WORTIME_500MS      0
#define E22_REG3_WORTIME_1000MS     1
#define E22_REG3_WORTIME_1500MS     2
#define E22_REG3_WORTIME_2000MS     3
#define E22_REG3_WORTIME_2500MS     4
#define E22_REG3_WORTIME_3000MS     5
#define E22_REG3_WORTIME_3500MS     6
#define E22_REG3_WORTIME_4000MS     7


enum E22_REG
{
	E22_REG_ADDH = 0X00,
	E22_REG_ADDL,
	E22_REG_NETID,
	E22_REG_REG0,
	E22_REG_REG1,
	E22_REG_CH,
	E22_REG_REG3,
	E22_REG_CRYPT_H,
	E22_REG_CRYPT_L,
	E22_REG_NUM,
	E22_REG_PID = 0X80
};
enum E22_MODE
{
	E22_MODE_NORMAL = 0X00,
	E22_MODE_WOR,
	E22_MODE_SET,
	E22_MODE_SLEEP
};
/* 变量声明 ----------------------------------------------------------------*/ 
typedef struct __LORA_InitTypeDef
{
	u16 ADD;
	u8 	Netid;
	u8 	BaudRate;
	u8 	Parity;
	u8 	AirRate;
	u8 	Frame;
	u8 	RSSI;
	u8 	TransmitPower;
	u8 	Channel;
	u8 	RSSIByte;
	u8 	TransferMethod;
	u8 	Relay;
	u8 	LBT;
	u8 	WORMod;
	u8 	WORTime;
	u16 CRYPT;
	
} LORA_InitTypeDef;
extern u8 LORA_RXBuf[COMM_MAX_SIZE];
/* 函数声明 ----------------------------------------------------------------*/ 
HAL_StatusTypeDef 		E22_usart_tx(uint8_t *pData, uint16_t Size);
HAL_StatusTypeDef 		E22_usart_rx_enable(uint8_t *pData, uint16_t Size);
HAL_StatusTypeDef 		E22_usart_rx_disable(void);
HAL_StatusTypeDef 		E22_reg_set(u8 RegClass, enum E22_REG Reg, u8 *pData, u16 Size);
HAL_StatusTypeDef 		E22_reg_get(enum E22_REG Reg, u8 *pData, u16 Size);
void 									E22_mode_set(enum E22_MODE mode);
enum 									E22_MODE E22_mode_get(void);
HAL_StatusTypeDef 		E22_init(LORA_InitTypeDef Lora_Init);
HAL_StatusTypeDef 		E22_data_send(u8 *pData, u16 Size);


#ifdef __cplusplus
}
#endif
#endif /*__ e22_driver_H */

/**
  * @}
  */

/**
  * @}
  */

/************************ Copyright (c) Siargo Ltd. 2020 *****END OF FILE****/
