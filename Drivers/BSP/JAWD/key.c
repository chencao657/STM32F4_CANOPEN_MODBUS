/*
*********************************************************************************************************
*
*	ģ������ : ������������ģ�� 
*	�ļ����� : key.c
*	��    �� : V1.0
*	˵    �� : 1.�ⲿ�жϼ�ⰴ���Ƿ���
*						 2.�ص������н�����״̬���밴��fifo
*						 3.����getkey��ȡ����fifo
*
*	�޸ļ�¼ :
*		�汾��     ����        ����      
*		V1.0    2025-05-14  shisanxiang1
*
*********************************************************************************************************
*/
#include "key.h"
#include "gpio.h"
#include "usart.h"

/* ����fifo�ṹ�嶨��*/
KEY_FIFO_T s_tKey;
/*
*********************************************************************************************************
*	�� �� ��:HAL_GPIO_EXTI_Callbac
*	����˵��: hal���ⲿ�жϻص�����������bsp_PutKey���밴��ֵ
*	��    ��: 
*********************************************************************************************************
*/
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  if(GPIO_Pin==KEY1_GPIO_PIN)
  {
    if(HAL_GPIO_ReadPin(KEY1_GPIO,KEY1_GPIO_PIN)==KEY1_DOWN_LEVEL)
    {
			bsp_PutKey(1);
    }

  }
  else if(GPIO_Pin==KEY2_GPIO_PIN)
  {
    if(HAL_GPIO_ReadPin(KEY2_GPIO,KEY2_GPIO_PIN)==KEY2_DOWN_LEVEL)
    {
			bsp_PutKey(2);
    }

  }
  else if(GPIO_Pin==KEY3_GPIO_PIN)
  {
    if(HAL_GPIO_ReadPin(KEY3_GPIO,KEY3_GPIO_PIN)==KEY3_DOWN_LEVEL)
    {
			bsp_PutKey(3);
    }

  }
  else if(GPIO_Pin==KEY4_GPIO_PIN)
  {
    if(HAL_GPIO_ReadPin(KEY4_GPIO,KEY4_GPIO_PIN)==KEY4_DOWN_LEVEL)
    {
			bsp_PutKey(4);
    }
  }
}

/*
*********************************************************************************************************
*	�� �� ��:key_fifoinit
*	����˵��: fifo��ʼ��������������Ҫ����һ��
*	��    ��: &s_tKey
*********************************************************************************************************
*/
void key_fifoinit(KEY_FIFO_T *s_tKey)
{
/* �԰���FIFO��дָ������ */
	s_tKey->Read = 0;
	s_tKey->Write = 0;
}


/*
*********************************************************************************************************
*	�� �� ��: bsp_PutKey
*	����˵��: ��1����ֵѹ�밴��FIFO��������
*	��    ��:  _KeyCode : ��������
*	�� �� ֵ: ��
*********************************************************************************************************
*/

void bsp_PutKey(uint8_t _KeyCode)
{
    uint8_t nextWrite = (s_tKey.Write + 1) % KEY_FIFO_SIZE;
    
    if (nextWrite != s_tKey.Read)  // ���FIFO�Ƿ�����
    {
        s_tKey.Buf[s_tKey.Write] = _KeyCode;
        s_tKey.Write = nextWrite;
    }
    else
    {
      /* fifo������� */
			// ��ִ���κβ������ɶ������¼�
    }
}
/*
*********************************************************************************************************
*	�� �� ��: bsp_GetKey
*	����˵��: �Ӱ���FIFO��������ȡһ����ֵ��
*	��    ��: ��
*	�� �� ֵ: ��������
*********************************************************************************************************
*/

uint8_t bsp_GetKey(void)
{
    uint8_t ret = KEY_NONE;

    if (s_tKey.Read != s_tKey.Write)  // ���FIFO�Ƿ�Ϊ��
    {
        ret = s_tKey.Buf[s_tKey.Read];
        s_tKey.Read = (s_tKey.Read + 1) % KEY_FIFO_SIZE;
    }
    return ret;
}



