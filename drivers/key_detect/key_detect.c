/****************************************************************************
** key_detect.c
**
** Copyright (c) Siargo Ltd. 2020
** All Rights Reserved.
**
** BY HYY
**
** ʵ�ְ�����⹦��
**
**
** �ļ���ʷ
** 2020-04-27 ����Ԫ����
** 2020-07-17 �޸��߼���װ�ɽӿ�
*****************************************************************************/

/* Includes ----------------------------------------------------------------*/
#include "key_detect.h"
#include <stdarg.h>
#include <string.h>
#include <stdio.h>


/** 
  * @brief key_detect����汾��
  */
#define  Version     "V1.0"


/* ������� ----------------------------------------------------------------*/ 
/** 
  * @brief 	������Ϣ�ṹ��
  */
typedef struct
{
	unsigned short Click_Count;													/*!< ���� */
	unsigned short Count;																/*!< ������ֵ */
	unsigned char *Key;																	/*!< ����״̬ */
	Trig_Mode_TypeDef Trig_Mode_E;											/*!< ��������ģʽ */
	Key_Mode_TypeDef  Key_Mode_E;												/*!< ����ģʽ */
	unsigned char Do_S;																	/*!< �ص�ִ��״̬ */
	void (*Key_Click_CallBack)(void);										/*!< ��������ص�������ָ�뺯���� */
}Key_Mess_TypeDef;

/** 
  * @brief �����ṹ��
  */
typedef struct
{
  Key_Mess_TypeDef Key_Mess_S[Key_Num_Max];						/*!< Key_Mess_TypeDef�ṹ�� */
  unsigned char KEY_register_Num;													/*!< ע�ᰴ������ */
  void (*Update_Key_CallBack)(void);									/*!< ���°���״̬�ص�������ָ�뺯���� */
  void (*Debug_CallBack)(unsigned char *debug_mess);	/*!< Key_Detect���Իص�������ָ�뺯���� */
}Key_TypeDef;


/** 
  * @brief Key�ṹ��
  */
static Key_TypeDef Key;

/* �������� ----------------------------------------------------------------*/



/* --------------------------- �û����� ------------------------------------*/
/** 
  * @name   Print_Debug_Mess
  *
  * @brief  ��ӡKey_Detect������Ϣ(���һ���Դ�ӡ255���ֽ�)
  *
  * @param  debug_mess��ringBuffer������Ϣ
  *
  * @retval ��
  */
static void Print_Debug_Mess(const char *debug_mess,...)
{
	va_list ap;
	unsigned char buf[255];
	unsigned char *pStr = buf;
	
	if(Key.Debug_CallBack == NULL) return;
	va_start(ap, debug_mess);
	vsnprintf((char *)buf, sizeof(buf), debug_mess, ap);				
	va_end(ap);
	Key.Debug_CallBack(pStr);
}

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
char KEY_struct_init(void (*Update_Key_CallBack)(void),void (*Debug_CallBack)(unsigned char *debug_mess))
{
	unsigned int count=0;																/* ���� */
	if(Update_Key_CallBack == NULL) return 1;						/* Update_Key_CallBackNULL */
	
	for(count=0;count<Key_Num_Max;count++)							/* ����Key_Mess_S���� */
	{
		Key.Key_Mess_S[count].Click_Count=0;							/* ���� */
		Key.Key_Mess_S[count].Count=0;										/* ����������ֵ */
		Key.Key_Mess_S[count].Do_S=0;											/* ��ʼ����������ص���������״̬ */
		Key.Key_Mess_S[count].Key_Click_CallBack=NULL;		/* ��ʼ����������ص����� */
		Key.Key_Mess_S[count].Key_Mode_E=N_Click;					/* ��ʼ������ģʽ */
		Key.Key_Mess_S[count].Key=NULL;										/* ��ʼ������״̬ */
		Key.Key_Mess_S[count].Trig_Mode_E=N_Trig;					/* ��ʼ����������ģʽ */
	}
	Key.Reg_Key_Num = 0;																/* ����ע��İ�������Ϊ0 */
	Key.Update_Key_CallBack = Update_Key_CallBack;			/* ���ø��°���״̬�ص����� */
	Key.Debug_CallBack = Debug_CallBack;								/* ��ӵ��Իص����� */
	return 0;
}

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
	*					2��Key.Reg_Key_Num > Key_Num_Max��
  */
char KEY_register(unsigned char *key_s,const unsigned short count, 
	Trig_Mode_TypeDef Trig_Mode_E, Key_Mode_TypeDef  Key_Mode_E, void (*Key_Click_CallBack)(void))
{
	unsigned int temp_count=0;																									/* ���� */
						
	if(Key_Click_CallBack == NULL)																							/* CallBackΪNULL����2 */
	{
		Print_Debug_Mess("Regist_Key:Key_Click_CallBack = NULL\r\n");
		return 1;	
	}	

	for(temp_count=0;temp_count<Key_Num_Max;temp_count++)												/* ����Key_Mess_S���� */
	{
		if(Key.Key_Mess_S[temp_count].Key_Click_CallBack != NULL)									/* �жϰ���״̬��ַ�Ƿ�ΪNULL */
		{
			if(Key_Click_CallBack == Key.Key_Mess_S[temp_count].Key_Click_CallBack)
			{
				Key.Key_Mess_S[temp_count].Click_Count=0;															/* ������������ */
				Key.Key_Mess_S[temp_count].Count=count;																/* ����������ֵ */
				Key.Key_Mess_S[temp_count].Key=key_s;																	/* ����״̬ */
				Key.Key_Mess_S[temp_count].Trig_Mode_E=Trig_Mode_E;										/* ��������ģʽ */
				Key.Key_Mess_S[temp_count].Key_Mode_E = Key_Mode_E;										/* ����ģʽ */
				Key.Key_Mess_S[temp_count].Key_Click_CallBack=Key_Click_CallBack;			/* ���������ص����� */
				Key.Key_Mess_S[temp_count].Do_S=0;																		/* �����ص�ִ��״̬ */
				return 0;
			}
		}
	}
	
	if(Key.Reg_Key_Num > Key_Num_Max)																						/* ����ע���������ڰ���ע�����������ֵ */
	{
		Print_Debug_Mess("Regist_Key:Key.Reg_Key_Num > Key_Num_Max\r\n");
		return 2;	
	}

	Key.Key_Mess_S[Key.Reg_Key_Num].Click_Count=0;															/* ������������ */
	Key.Key_Mess_S[Key.Reg_Key_Num].Count=count;																/* ����������ֵ */
	Key.Key_Mess_S[Key.Reg_Key_Num].Key=key_s;																	/* ����״̬ */
	Key.Key_Mess_S[Key.Reg_Key_Num].Trig_Mode_E=Trig_Mode_E;										/* ��������ģʽ */
	Key.Key_Mess_S[Key.Reg_Key_Num].Key_Mode_E = Key_Mode_E;										/* ����ģʽ */
	Key.Key_Mess_S[Key.Reg_Key_Num].Key_Click_CallBack=Key_Click_CallBack;			/* ���������ص����� */
	Key.Key_Mess_S[Key.Reg_Key_Num].Do_S=0;																			/* �����ص�ִ��״̬ */
	
	Key.Reg_Key_Num++;

	return 0;																																		/* ����key�ɹ� */															
}

/** 
  * @name   Clear_Click_Num
  *
  * @brief  �������ĵ��������Ϊ0
  *
  * @param  Key_Number���ڼ�������
  *
  * @retval ��
  */
static void Clear_Click_Num(unsigned char key_num)
{
	if(key_num >= Key_Num_Max) 																									/* ��鰴���ṹ�����������Ƿ���ڰ������������ֵ */
	{
		Print_Debug_Mess("Clear_Click_Num:key_num > Key_Num_Max\r\n");
	}		
	
	Key.Key_Mess_S[key_num].Click_Count=0;																			/* ��հ������� */
	Key.Key_Mess_S[key_num].Do_S=0;																							/* ��հ����ص�ִ��״̬ */
}

/** 
  * @name   Key_Click
  *
  * @brief  ���������¼�
  *
  * @param  key_num���ڼ�������
  *
  * @retval ��
  */
static void Key_Click(unsigned char key_num)
{
	if(key_num >= Key_Num_Max) 																								/* ��鰴���ṹ�����������Ƿ���ڰ������������ֵ */
	{
		Print_Debug_Mess("Key_Click:key_num > Key_Num_Max\r\n");
		return;	
	}
			
	if(Key.Key_Mess_S[key_num].Trig_Mode_E==L_Trig)														/* �͵�ƽ���� */
	{
		if(*Key.Key_Mess_S[key_num].Key==0)																			/* �������� */
		{
			if(Key.Key_Mess_S[key_num].Click_Count<Key.Key_Mess_S[key_num].Count)	/* �жϰ����͵�ƽ���� */
			{
				Key.Key_Mess_S[key_num].Click_Count++;															/* �����͵�ƽ���� */
			}
			if(Key.Key_Mess_S[key_num].Click_Count>=Key.Key_Mess_S[key_num].Count)/* �жϰ����͵�ƽ���� */
			{
				if(Key.Key_Mess_S[key_num].Do_S==0 )
				{
					Key.Key_Mess_S[key_num].Key_Click_CallBack();											/* ���ð����ص����� */
					Key.Key_Mess_S[key_num].Do_S=1;																		/* �����ص�����ִ��״̬��1 */
				}
			}
		}
		else if(*Key.Key_Mess_S[key_num].Key==1)																/* �����ɿ� */
		{
			Clear_Click_Num(key_num);																							/* �������ĵ��������Ϊ0 */
		}
	}
	else if(Key.Key_Mess_S[key_num].Trig_Mode_E==H_Trig)											/* �ߵ�ƽ���� */
	{
		if(*Key.Key_Mess_S[key_num].Key==1)																			/* �������� */
		{
			if(Key.Key_Mess_S[key_num].Click_Count<Key.Key_Mess_S[key_num].Count)	/* �жϰ����͵�ƽ���� */
			{
				Key.Key_Mess_S[key_num].Click_Count++;															/* �����͵�ƽ���� */
			}
			if(Key.Key_Mess_S[key_num].Click_Count>=Key.Key_Mess_S[key_num].Count)/* �жϰ����͵�ƽ���� */
			{
				if(Key.Key_Mess_S[key_num].Do_S==0 )
				{
					Key.Key_Mess_S[key_num].Key_Click_CallBack();											/* ���ð����ص����� */
					Key.Key_Mess_S[key_num].Do_S=1;																		/* �����ص�����ִ��״̬��1 */
				}
			}
		}
		else if(*Key.Key_Mess_S[key_num].Key==0)																/* �����ɿ� */
		{
			Clear_Click_Num(key_num);																							/* �������ĵ��������Ϊ0 */
		}
	}
}

/** 
  * @name   Key_D_Click
  *
  * @brief  ����˫���¼�
  *
  * @param  key_num���ڼ�������
  *
  * @retval ��
  */
static void Key_D_Click(unsigned char key_num)
{
	if(key_num >= Key_Num_Max) 																										/* ��鰴���ṹ�����������Ƿ���ڰ������������ֵ */
	{
		Print_Debug_Mess("Key_D_Click:key_num > Key_Num_Max\r\n");
		return;	
	}	
			
	if(Key.Key_Mess_S[key_num].Trig_Mode_E==L_Trig)																/* �͵�ƽ���� */
	{
		if(*Key.Key_Mess_S[key_num].Key==0)																					/* �жϰ����Ƿ��ǵ͵�ƽ */
		{
			/* ���� */
			if(Key.Key_Mess_S[key_num].Click_Count<Key.Key_Mess_S[key_num].Count)			/* ��鰴���͵�ƽ������� */
			{
				Key.Key_Mess_S[key_num].Click_Count++;																	/* �����ߵ�ƽ�������++ */
			}
			if(Key.Key_Mess_S[key_num].Click_Count>=Key.Key_Mess_S[key_num].Count)		/* �жϰ����͵�ƽ������� */
			{
				if(Key.Key_Mess_S[key_num].Do_S==1)
				{
					Key.Key_Mess_S[key_num].Key_Click_CallBack();													/* ���ð����ص����� */
					Key.Key_Mess_S[key_num].Do_S=2;																				/* �����ص�����ִ��״̬��1 */
				}
			}
		}
		else if(*Key.Key_Mess_S[key_num].Key==1)																		/* �жϰ���״̬�Ƿ�Ϊ�ߵ�ƽ */
		{
			/* ���� */
			if(Key.Key_Mess_S[key_num].Click_Count<Key.Key_Mess_S[key_num].Count && 
				Key.Key_Mess_S[key_num].Do_S>0)																					/* ��鰴���͵�ƽ������� */
			{
				Key.Key_Mess_S[key_num].Click_Count++;																	/* �����ߵ�ƽ�������++ */
			}
			if(Key.Key_Mess_S[key_num].Click_Count > 0 && 
				Key.Key_Mess_S[key_num].Click_Count <= Key.Key_Mess_S[key_num].Count &&
				Key.Key_Mess_S[key_num].Do_S==0)																				/* �жϰ����ߵ�ƽ�ĵ�������Ƿ����0 */
			{
				Key.Key_Mess_S[key_num].Do_S=1;
			}
			else if(Key.Key_Mess_S[key_num].Click_Count >= Key.Key_Mess_S[key_num].Count)
			{
				Clear_Click_Num(key_num);																								/* �������ĵ��������Ϊ0 */
			}
		}
	}
	else if(Key.Key_Mess_S[key_num].Trig_Mode_E==H_Trig)													/* �ߵ�ƽ���� */
	{
		if(*Key.Key_Mess_S[key_num].Key==1)																					/* �жϰ����Ƿ��Ǹߵ�ƽ */
		{
			/* ���� */
			if(Key.Key_Mess_S[key_num].Click_Count<Key.Key_Mess_S[key_num].Count)			/* ��鰴���͵�ƽ������� */
			{
				Key.Key_Mess_S[key_num].Click_Count++;																	/* �����ߵ�ƽ�������++ */
			}
			if(Key.Key_Mess_S[key_num].Click_Count>=Key.Key_Mess_S[key_num].Count)		/* �жϰ����ߵ�ƽ�������*/
			{
				if(Key.Key_Mess_S[key_num].Do_S==1)
				{
					Key.Key_Mess_S[key_num].Key_Click_CallBack();													/* ���ð����ص����� */
					Key.Key_Mess_S[key_num].Do_S=2;																				/* �����ص�����ִ��״̬��1 */
				}
			}
		}
		else if(*Key.Key_Mess_S[key_num].Key==0)																		/* �жϰ���״̬�Ƿ�Ϊ�͵�ƽ */
		{
			/* ���� */
			if(Key.Key_Mess_S[key_num].Click_Count<Key.Key_Mess_S[key_num].Count && 
				Key.Key_Mess_S[key_num].Do_S>0)																					/* ��鰴���͵�ƽ������� */
			{
				Key.Key_Mess_S[key_num].Click_Count++;																	/* �����ߵ�ƽ�������++ */
			}
			if(Key.Key_Mess_S[key_num].Click_Count > 0 && 
				Key.Key_Mess_S[key_num].Click_Count <= Key.Key_Mess_S[key_num].Count &&
				Key.Key_Mess_S[key_num].Do_S==0)																				/* �жϰ����ߵ�ƽ�ĵ�������Ƿ����0 */
			{
				Key.Key_Mess_S[key_num].Do_S=1;
			}
			else if(Key.Key_Mess_S[key_num].Click_Count >= Key.Key_Mess_S[key_num].Count)
			{
				Clear_Click_Num(key_num);																								/* �������ĵ��������Ϊ0 */
			}
		}
	}
}

/** 
  * @name   Key_L_Press
  *
  * @brief  ���������¼�
  *
  * @param  ��
  * @param  key_num���ڼ�������
  *
  * @retval ��
  */
static void Key_L_Press(unsigned char key_num)
{
	if(key_num >= Key_Num_Max) 																									/* ��鰴���ṹ�����������Ƿ���ڰ������������ֵ */
	{
		Print_Debug_Mess("Key_L_Press:key_num > Key_Num_Max\r\n");
		return;	
	}	
	
	if(Key.Key_Mess_S[key_num].Trig_Mode_E==L_Trig)															/* �͵�ƽ���� */
	{
		if(*Key.Key_Mess_S[key_num].Key==0)																				/* �������� */
		{
			if(Key.Key_Mess_S[key_num].Click_Count<Key.Key_Mess_S[key_num].Count) 	/* �жϰ����͵�ƽ���� */
			{
				Key.Key_Mess_S[key_num].Click_Count++;																/* �����͵�ƽ���� */
			}
			if(Key.Key_Mess_S[key_num].Click_Count>=Key.Key_Mess_S[key_num].Count)	/* �жϰ����͵�ƽ���� */
			{
				Key.Key_Mess_S[key_num].Key_Click_CallBack();													/* ���ð����ص����� */
			}
		}
		else if(*Key.Key_Mess_S[key_num].Key==1)																	/* �����ɿ� */
		{
			Clear_Click_Num(key_num);																								/* �������ĵ��������Ϊ0 */
		}
	}
	else if(Key.Key_Mess_S[key_num].Trig_Mode_E==H_Trig)												/* �ߵ�ƽ���� */
	{
		if(*Key.Key_Mess_S[key_num].Key==1)																				/* �������� */
		{
			if(Key.Key_Mess_S[key_num].Click_Count<Key.Key_Mess_S[key_num].Count) 	/* �жϰ����͵�ƽ���� */
			{
				Key.Key_Mess_S[key_num].Click_Count++;																/* �����͵�ƽ���� */
			}
			if(Key.Key_Mess_S[key_num].Click_Count>=Key.Key_Mess_S[key_num].Count) 	/* �жϰ����͵�ƽ���� */
			{
				Key.Key_Mess_S[key_num].Key_Click_CallBack(); 												/* ���ð����ص����� */
			}
		}
		else if(*Key.Key_Mess_S[key_num].Key==0)																	/* �����ɿ� */
		{
			Clear_Click_Num(key_num);																								/* �������ĵ��������Ϊ0 */
		}
	}
}

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
char KEY_detect(void)
{
	unsigned char count=0;																	/* ���� */

	if(Key.Update_Key_CallBack == NULL)											/* Key.Update_Key_CallBackNULL */
	{
		Print_Debug_Mess("Key_Detect:Key.Update_Key_CallBack = NULL\r\n");
		return 2;	
	}		

	Key.Update_Key_CallBack();															/* ���°���״̬ */
	for(count=0;count<Key.Reg_Key_Num;count++)
	{
		if(Key.Key_Mess_S[count].Key == NULL) continue;				/* �жϴ������İ����Ƿ�ע���˰��� */
		if(Key.Key_Mess_S[count].Trig_Mode_E==L_Trig)
		{
			switch (Key.Key_Mess_S[count].Key_Mode_E)
			{
				case S_Click:																			/*!< ���� */	
					Key_Click(count);
					break;
					
				case D_Click:																			/*!< ˫�� */
					Key_D_Click(count);
					break;
					
				case L_Press:																			/*!< ���� */
					Key_L_Press(count);
					break;
					
				default:
					break;
			}
		}
		else if(Key.Key_Mess_S[count].Trig_Mode_E==H_Trig)
		{
			switch (Key.Key_Mess_S[count].Key_Mode_E)
			{
				case S_Click:																			/*!< ���� */	
					Key_Click(count);
					break;
					
				case D_Click:																			/*!< ˫�� */
					Key_D_Click(count);
					break;
					
				case L_Press:																			/*!< ���� */
					Key_L_Press(count);
					break;

				default:
					break;
			}
		}
	}
	return 0;
}

/** 
  * @name   Get_Version_Mess
  *
  * @brief  ��ӡKey_Detect����汾��Ϣ
  *
  * @param  ��
  *
  * @retval ��Key_Detect����汾��Ϣ
  */
char *Get_Version_Mess(void)
{
	return Version;
}


/************************ Copyright (c) Siargo Ltd. 2020 *****END OF FILE****/
