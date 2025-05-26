#ifndef __MODBUY_SLAVE_H
#define __MODBUY_SLAVE_H


#include "stdint.h"
#include "user_lib.h"
#include "modbus_usart.h"
#include "modbus_tim.h"
#include "modbus_msg.h"
#include "JAWD_SPEED.h"

/* 从站站号设置 */
#define SADDR485	1
#define SBAUD485	UART3_BAUD

/* 01H 读强制单线圈 */
/* 05H 写强制单线圈 */
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

/* 02H 读取输入状态 */
#define REG_T01		0x0201
#define REG_T02		0x0202
#define REG_T03		0x0203
#define REG_TXX		REG_T03

/* 03H 读保持寄存器 */
/* 06H 写保持寄存器 */
/* 10H 写多个保存寄存器 */

#define SLAVE_REG_P01		0x0001    //1号电机速度
#define SLAVE_REG_P02		0x0002    //2号电机速度
#define SLAVE_REG_P03		0x0003		//加速度
#define SLAVE_REG_P04		0x0004		//减速度
#define SLAVE_REG_P05		0x0005		//1号电机实际速度
#define SLAVE_REG_P06		0x0006		//2号电机实际速度

/* 04H 读取输入寄存器(模拟信号) */
#define REG_A01		0x0401
#define REG_AXX		REG_A01


/* RTU 应答代码 */
#define RSP_OK				0		/* 成功 */
#define RSP_ERR_CMD			0x01	/* 不支持的功能码 */
#define RSP_ERR_REG_ADDR	0x02	/* 寄存器地址错误 */
#define RSP_ERR_VALUE		0x03	/* 数据值域错误 */
#define RSP_ERR_WRITE		0x04	/* 写入失败 */

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
	/* 03H 06H 读写保持寄存器 */
	int16_t P01;
	int16_t P02;
	int16_t P03;
	int16_t P04;	
	int16_t P05;
	int16_t P06;
	/* 04H 读取模拟量寄存器 */
	uint16_t A01;

	/* 01H 05H 读写单个强制线圈 */
	uint16_t D00;				//NMT操作状态使能
	uint16_t D01;				//NMT操作状态使能
	uint16_t D02;				//电机使能
	uint16_t D03;				//电机配置为速度模式
	uint16_t D04;				//电机启动
	uint16_t D05;				//电机暂停
	uint16_t D06;
	uint16_t D07;
	uint16_t D08;
}VAR_T;

void MODS_Poll(void);

extern MODS_T g_tModS;
extern VAR_T g_tVar;
#endif

/***************************** 安富莱电子 www.armfly.com (END OF FILE) *********************************/
