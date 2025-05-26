#ifndef __MODBUY_SLAVE_H
#define __MODBUY_SLAVE_H


#include "stdint.h"
#include "user_lib.h"
#include "modbus_usart.h"
#include "modbus_tim.h"
#include "modbus_msg.h"
#include "JAWD_SPEED.h"

/* ��վվ������ */
#define SADDR485	1
#define SBAUD485	UART3_BAUD

/* 01H ��ǿ�Ƶ���Ȧ */
/* 05H дǿ�Ƶ���Ȧ */
#define REG_D00		0x0000	
#define REG_D01		0x0001		
#define REG_D02		0x0002
#define REG_D03		0x0003
#define REG_D04		0x0004
#define REG_D05		0x0005
#define REG_D06		0x0006	
#define REG_D07		0x0007	
#define REG_D08		0x0008	
#define REG_DXX 	REG_D08

/* 02H ��ȡ����״̬ */
#define REG_T01		0x0201
#define REG_T02		0x0202
#define REG_T03		0x0203
#define REG_TXX		REG_T03

/* 03H �����ּĴ��� */
/* 06H д���ּĴ��� */
/* 10H д�������Ĵ��� */

#define SLAVE_REG_P01		0x0001    //1�ŵ���ٶ�
#define SLAVE_REG_P02		0x0002    //2�ŵ���ٶ�
#define SLAVE_REG_P03		0x0003		//���ٶ�
#define SLAVE_REG_P04		0x0004		//���ٶ�
#define SLAVE_REG_P05		0x0005		//1�ŵ��ʵ���ٶ�
#define SLAVE_REG_P06		0x0006		//2�ŵ��ʵ���ٶ�

/* 04H ��ȡ����Ĵ���(ģ���ź�) */
#define REG_A01		0x0401
#define REG_AXX		REG_A01


/* RTU Ӧ����� */
#define RSP_OK				0		/* �ɹ� */
#define RSP_ERR_CMD			0x01	/* ��֧�ֵĹ����� */
#define RSP_ERR_REG_ADDR	0x02	/* �Ĵ�����ַ���� */
#define RSP_ERR_VALUE		0x03	/* ����ֵ����� */
#define RSP_ERR_WRITE		0x04	/* д��ʧ�� */

#define S_RX_BUF_SIZE		30
#define S_TX_BUF_SIZE		128

typedef struct
{
	uint8_t RxBuf[S_RX_BUF_SIZE];
	uint8_t RxCount;
	uint8_t RxStatus;
	uint8_t RxNewFlag;

	uint8_t RspCode;

	uint8_t TxBuf[S_TX_BUF_SIZE];
	uint8_t TxCount;
}MODS_T;

typedef struct
{
	/* 03H 06H ��д���ּĴ��� */
	int16_t P01;
	int16_t P02;
	int16_t P03;
	int16_t P04;	
	int16_t P05;
	int16_t P06;
	/* 04H ��ȡģ�����Ĵ��� */
	uint16_t A01;

	/* 01H 05H ��д����ǿ����Ȧ */
	uint16_t D00;				//NMT����״̬ʹ��
	uint16_t D01;				//NMT����״̬ʹ��
	uint16_t D02;				//���ʹ��
	uint16_t D03;				//�������Ϊ�ٶ�ģʽ
	uint16_t D04;				//�������
	uint16_t D05;				//�����ͣ
	uint16_t D06;
	uint16_t D07;
	uint16_t D08;
}VAR_T;

void MODS_Poll(void);

extern MODS_T g_tModS;
extern VAR_T g_tVar;
#endif

/***************************** ���������� www.armfly.com (END OF FILE) *********************************/
