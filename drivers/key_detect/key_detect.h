/****************************************************************************
** key_detect.h
**
** Copyright (c) Siargo Ltd. 2020
** All Rights Reserved.
**
** BY HYY
**
** key_detect.c�ĺ궨�������
**
**
** �ļ���ʷ
** 2020-07-17 ����Ԫ����
*****************************************************************************/
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __KEY_DETECT_H
#define __KEY_DETECT_H
#ifdef __cplusplus
 extern "C" {
#endif


/* Includes ------------------------------------------------------------------*/
#include "GlobeDef.h"

/* �궨�� ----------------------------------------------------------------*/ 
/** 
  * @brief ע�ᰴ��������
  */
#define Key_Num_Max		10


/** 
  * @brief ��������ģʽ״̬ö��
  */
typedef enum
{
	N_Trig = 0, 																				/*!< 0 �� */		
	L_Trig ,																						/*!< 1 �͵�ƽ���� */			
	H_Trig,																							/*!< 2 �ߵ�ƽ���� */
}Trig_Mode_TypeDef;

/** 
  * @brief ����ģʽ״̬ö��
  */
typedef enum
{
	N_Click = 0,																				/*!< 0 �� */	
	S_Click ,																						/*!< 1 ���� */			
	D_Click,																						/*!< 2 ˫�� */
	L_Press,																						/*!< 3 ���� */
}Key_Mode_TypeDef;


/* �������� ----------------------------------------------------------------*/ 

/* �������� ----------------------------------------------------------------*/ 

/** 
  * @name   KEY_struct_init
  *
  * @brief  ��ʼ������
  *
  * @param  Update_Key_CallBack�����°���״̬
  * @param  Debug_CallBack����ӡ����������Ϣ
  *
  * @retval 0���ɹ���
	*					1��Update_Key_CallBack == NULL��
  */
char KEY_struct_init(void (*Update_Key_CallBack)(void),void (*Debug_CallBack)(unsigned char *debug_mess));

/** 
  * @name   KEY_register
  *
  * @brief  ���ע�ᰴ����ע����������Ѿ�ע�������ô�ٴ�ע��Ḳ��֮ǰע�������ͬ�İ�����
  *
  * @param  key_s������״̬
  * @param  count����������
  * @param  Trig_Mode_E����������ģʽ
  * @param  Key_Mode_E������ģʽ
  * @param  Key_Click_CallBack�����������ص�
  *
  * @retval 0���ɹ���
	*					1��Key_Click_CallBack == NULL��
	*					2��Key.KEY_register_Num > Key_Num_Max��
  */
char KEY_register(unsigned char *key_s,const unsigned short count, 
	Trig_Mode_TypeDef Trig_Mode_E, Key_Mode_TypeDef  Key_Mode_E, void (*Key_Click_CallBack)(void));

/** 
  * @name   KEY_detect
  *
  * @brief  �������
  *
  * @param  ��
  *
  * @retval 0���ɹ���
	*					1��Key.Update_Key_CallBack == NULL��
  */
char KEY_detect(void);

/** 
  * @name   Get_Version_Mess
  *
  * @brief  ��ӡKey_Detect����汾��Ϣ
  *
  * @param  ��
  *
  * @retval ��Key_Detect����汾��Ϣ
  */
char *Get_Version_Mess(void);


#ifdef __cplusplus
}
#endif
#endif /*__KEY_DETECT_H */

/**
  * @}
  */

/**
  * @}
  */

/************************ Copyright (c) Siargo Ltd. 2020 *****END OF FILE****/
