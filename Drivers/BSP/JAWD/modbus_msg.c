/*
*********************************************************************************************************
*
*	模块名称 : 消息处理模块
*	文件名称 : bsp_msg.c
*	版    本 : V1.0
*	说    明 : 消息处理机制。
*
*	修改记录 :
*		版本号  日期        作者     说明
*		V1.0    2015-03-27 armfly  正式发布
*
*	Copyright (C), 2014-2015, 安富莱电子 www.armfly.com
*
*********************************************************************************************************
*/

#include "modbus_msg.h"

MSG_FIFO_T g_tMsg;

/*
*********************************************************************************************************
*	函 数 名: bsp_InitMsg
*	功能说明: 初始化消息缓冲区
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void bsp_InitMsg(void)
{
	bsp_ClearMsg();
}

/*
*********************************************************************************************************
*	函 数 名: bsp_PutMsg
*	功能说明: 将1个消息压入消息FIFO缓冲区。
*	形    参:  _MsgCode : 消息代码
*			  _pMsgParam : 消息参数，一般指向某个特定的结构体. 或者是0
*	返 回 值: 无
*********************************************************************************************************
*/
void bsp_PutMsg(uint16_t _MsgCode, uint32_t _MsgParam)
{
	g_tMsg.Buf[g_tMsg.Write].MsgCode = _MsgCode;
	g_tMsg.Buf[g_tMsg.Write].MsgParam = _MsgParam;

	if (++g_tMsg.Write  >= MSG_FIFO_SIZE)
	{
		g_tMsg.Write = 0;
	}
}

/*
*********************************************************************************************************
*	函 数 名: bsp_GetMsg
*	功能说明: 从消息FIFO缓冲区读取一个键值。
*	形    参:  无
*	返 回 值: 0 表示无消息； 1表示有消息
*********************************************************************************************************
*/
uint8_t bsp_GetMsg(MSG_T *_pMsg)
{
	MSG_T *p;

	if (g_tMsg.Read == g_tMsg.Write)
	{
		return 0;
	}
	else
	{
		p = &g_tMsg.Buf[g_tMsg.Read];

		if (++g_tMsg.Read >= MSG_FIFO_SIZE)
		{
			g_tMsg.Read = 0;
		}
		
		_pMsg->MsgCode = p->MsgCode;
		_pMsg->MsgParam = p->MsgParam;
		return 1;
	}
}


/*
*********************************************************************************************************
*	函 数 名: bsp_GetMsg2
*	功能说明: 从消息FIFO缓冲区读取一个键值。使用第2个读指针。可以2个进程同时访问消息区。
*	形    参:  无
*	返 回 值: 0 表示无消息； 1表示有消息
*********************************************************************************************************
*/
uint8_t bsp_GetMsg2(MSG_T *_pMsg)
{
	MSG_T *p;

	if (g_tMsg.Read2 == g_tMsg.Write)
	{
		return 0;
	}
	else
	{
		p = &g_tMsg.Buf[g_tMsg.Read2];

		if (++g_tMsg.Read2 >= MSG_FIFO_SIZE)
		{
			g_tMsg.Read2 = 0;
		}
		
		_pMsg->MsgCode = p->MsgCode;
		_pMsg->MsgParam = p->MsgParam;
		return 1;
	}
}

/*
*********************************************************************************************************
*	函 数 名: bsp_ClearMsg
*	功能说明: 清空消息FIFO缓冲区
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void bsp_ClearMsg(void)
{
	g_tMsg.Read = g_tMsg.Write;
}

/***************************** 安富莱电子 www.armfly.com (END OF FILE) *********************************/
