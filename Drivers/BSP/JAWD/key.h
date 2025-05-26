/*
*********************************************************************************************************
*
*	ģ������ : 
*	�ļ����� : 
*	��    �� : 
*	˵    �� : 
*
*	�޸ļ�¼ :
*		�汾��      ����         ����         ˵��
*		V1.0    2025-05-14   shisanxiang1       0
*
*********************************************************************************************************
*/
#ifndef __KEY_H__
#define __KEY_H__


#include <stdint.h>

/*�������Ŷ��� */
#define KEY1_GPIO_PIN                 key1_Pin
#define KEY1_GPIO                     GPIOG
#define KEY1_DOWN_LEVEL               0  /* ����ԭ��ͼ��ƣ�KEY1����ʱ����Ϊ�͵�ƽ��������������Ϊ0 */

#define KEY2_GPIO_PIN                 key2_Pin
#define KEY2_GPIO                     GPIOG
#define KEY2_DOWN_LEVEL               0  /* ����ԭ��ͼ��ƣ�KEY2����ʱ����Ϊ�͵�ƽ��������������Ϊ0 */

#define KEY3_GPIO_PIN                 key3_Pin
#define KEY3_GPIO                     GPIOG
#define KEY3_DOWN_LEVEL               0  /* ����ԭ��ͼ��ƣ�KEY3����ʱ����Ϊ�͵�ƽ��������������Ϊ0 */

#define KEY4_GPIO_PIN                 key4_Pin
#define KEY4_GPIO                     GPIOG
#define KEY4_DOWN_LEVEL               0  /* ����ԭ��ͼ��ƣ�KEY4����ʱ����Ϊ�͵�ƽ��������������Ϊ0 */

/* ����fifo�����С���� */
#define KEY_FIFO_SIZE	10


/* ����fifo�ṹ������ */
typedef struct
{
	uint8_t Buf[KEY_FIFO_SIZE];		/* ��ֵ������ */
	uint8_t Read;									/* ��������ָ��1 */
	uint8_t Write;								/* ������дָ�� */
}KEY_FIFO_T;
/* �ⲿ���������ṹ�� */
extern KEY_FIFO_T s_tKey;

/* �������� */
void key_fifoinit(KEY_FIFO_T *s_tKey);
void bsp_PutKey(uint8_t _KeyCode);
uint8_t bsp_GetKey(void);



/*�����ֵ����*/
typedef enum
{
	KEY_NONE = 0,			/* 0 ��ʾ�����¼� */

	KEY_1_DOWN,				/* 1������ */

	KEY_2_DOWN,				/* 2������ */

	KEY_3_DOWN,				/* 3������ */

	KEY_4_DOWN,				/* 4������ */

}KEY_ENUM;

#endif
