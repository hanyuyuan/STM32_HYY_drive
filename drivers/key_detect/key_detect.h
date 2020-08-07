/****************************************************************************
** key_detect.h
**
** Copyright (c) Siargo Ltd. 2020
** All Rights Reserved.
**
** BY HYY
**
** key_detect.c的宏定义和声明
**
**
** 文件历史
** 2020-07-17 韩堉元创建
*****************************************************************************/
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __KEY_DETECT_H
#define __KEY_DETECT_H
#ifdef __cplusplus
 extern "C" {
#endif


/* Includes ------------------------------------------------------------------*/
#include "GlobeDef.h"

/* 宏定义 ----------------------------------------------------------------*/ 
/** 
  * @brief 注册按键最大个数
  */
#define Key_Num_Max		10


/** 
  * @brief 按键触发模式状态枚举
  */
typedef enum
{
	N_Trig = 0, 																				/*!< 0 空 */		
	L_Trig ,																						/*!< 1 低电平触发 */			
	H_Trig,																							/*!< 2 高电平触发 */
}Trig_Mode_TypeDef;

/** 
  * @brief 按键模式状态枚举
  */
typedef enum
{
	N_Click = 0,																				/*!< 0 空 */	
	S_Click ,																						/*!< 1 单击 */			
	D_Click,																						/*!< 2 双击 */
	L_Press,																						/*!< 3 长按 */
}Key_Mode_TypeDef;


/* 变量声明 ----------------------------------------------------------------*/ 

/* 函数声明 ----------------------------------------------------------------*/ 

/** 
  * @name   KEY_struct_init
  *
  * @brief  初始化按键
  *
  * @param  Update_Key_CallBack：更新按键状态
  * @param  Debug_CallBack：打印按键调试信息
  *
  * @retval 0：成功；
	*					1：Update_Key_CallBack == NULL；
  */
char KEY_struct_init(void (*Update_Key_CallBack)(void),void (*Debug_CallBack)(unsigned char *debug_mess));

/** 
  * @name   KEY_register
  *
  * @brief  添加注册按键（注：如果按键已经注册过，那么再次注册会覆盖之前注册过的相同的按键）
  *
  * @param  key_s：按键状态
  * @param  count：按键计数
  * @param  Trig_Mode_E：按键触发模式
  * @param  Key_Mode_E：按键模式
  * @param  Key_Click_CallBack：按键触发回调
  *
  * @retval 0：成功；
	*					1：Key_Click_CallBack == NULL；
	*					2：Key.KEY_register_Num > Key_Num_Max；
  */
char KEY_register(unsigned char *key_s,const unsigned short count, 
	Trig_Mode_TypeDef Trig_Mode_E, Key_Mode_TypeDef  Key_Mode_E, void (*Key_Click_CallBack)(void));

/** 
  * @name   KEY_detect
  *
  * @brief  按键检测
  *
  * @param  无
  *
  * @retval 0：成功；
	*					1：Key.Update_Key_CallBack == NULL；
  */
char KEY_detect(void);

/** 
  * @name   Get_Version_Mess
  *
  * @brief  打印Key_Detect组件版本信息
  *
  * @param  无
  *
  * @retval 返Key_Detect组件版本信息
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
