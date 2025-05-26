/*
*********************************************************************************************************
*
*	模块名称 : 伺服驱动器速度模式
*	文件名称 : 
*	版    本 : 
*	说    明 : 
*
*	修改记录 :
*		版本号      日期         作者         说明
*		V1.0    2025-05-14   shisanxiang1       0
*
*********************************************************************************************************
*/
#include "JAWD_SPEED.h"
#include "canopen_operate.h"
#include "usart.h"
#include "modbus_slave.h"

/* 电机id，默认a轴为0x01，b轴为0x02 */
#define CANopenSlaveID1   0x01
#define CANopenSlaveID2   0x02


motor_speedstate_t motorspeed_state = {
.mode = IDLE,
.run_flag = 0,
.enable_flag =0,
};

uint8_t g_ucTempBuf[20];



/*
*********************************************************************************************************
*	函 数 名:enable_motor(void)
*	功能说明:电机使能函数
*	形    参: 
*	返 回 值: 
*********************************************************************************************************
*/
static void enable_motor(void)
{	
		
		//初始化设备
		g_ucTempBuf[0]=0x01;
		g_ucTempBuf[1]=0x00;
		CO_SDO_abortCode_t abortCode1 = write_SDO(CO->SDOclient, CANopenSlaveID1, 0x6040, 0x00, g_ucTempBuf, 2);
		if (abortCode1 != CO_SDO_AB_NONE) {
				// 处理错误
				printf("节点1初始化失败, 错误码: %lu\n", (unsigned long)abortCode1);
		}		
		CO_SDO_abortCode_t abortCode2 = write_SDO(CO->SDOclient, CANopenSlaveID2, 0x6040, 0x00, g_ucTempBuf, 2);
		if (abortCode2 != CO_SDO_AB_NONE) {
				// 处理错误
				printf("节点2初始化失败, 错误码: %lu\n", (unsigned long)abortCode2);
		}		
		//打开刹车
		g_ucTempBuf[0]=0x03;
		g_ucTempBuf[1]=0x00;
		CO_SDO_abortCode_t abortCode3 = write_SDO(CO->SDOclient, CANopenSlaveID1, 0x6040, 0x00, g_ucTempBuf, 2);
		if (abortCode3 != CO_SDO_AB_NONE) {
				// 处理错误
				printf("节点1打开刹车失败, 错误码: %lu\n", (unsigned long)abortCode3);
		}
		CO_SDO_abortCode_t abortCode4 = write_SDO(CO->SDOclient, CANopenSlaveID2, 0x6040, 0x00, g_ucTempBuf, 2);
		if (abortCode4 != CO_SDO_AB_NONE) {
				// 处理错误
				printf("节点2打开刹车失败, 错误码: %lu\n", (unsigned long)abortCode4);
		}		
		//使能电机
		g_ucTempBuf[0]=0x0F;
		g_ucTempBuf[1]=0x00;
		CO_SDO_abortCode_t abortCode5=write_SDO(CO->SDOclient, CANopenSlaveID1, 0x6040, 0x00, g_ucTempBuf, 2);
		if (abortCode5 != CO_SDO_AB_NONE) {
				// 处理错误
				printf("节点1使能失败, 错误码: %lu\n", (unsigned long)abortCode5);
		}
		CO_SDO_abortCode_t abortCode6=write_SDO(CO->SDOclient, CANopenSlaveID2, 0x6040, 0x00, g_ucTempBuf, 2);
		if (abortCode6 != CO_SDO_AB_NONE) {
				// 处理错误
				printf("节点2使能失败败, 错误码: %lu\n", (unsigned long)abortCode6);
		}
		
		motorspeed_state.enable_flag= 1;
		printf("电机使能成功,请等待速度模式初始化");
		
}	

/*
*********************************************************************************************************
*	函 数 名:motorspeed_init(motor_speed)
*	功能说明: 设置电机为速度模式，并设置速度
*	形    参: 速度motor_speed
*	返 回 值: 
*********************************************************************************************************
*/
void motorspeed_init(void)
{
		if(motorspeed_state.mode == IDLE)
		{
			/* 等待初始化完成 */
			if( motorspeed_state.enable_flag == 0)
			{
				printf("等待电机使能成功");
				return;
				
			}
			//暂停电机运行
			g_ucTempBuf[0]=0x0F;
			g_ucTempBuf[1]=0x01;
			write_SDO(CO->SDOclient, CANopenSlaveID1, 0x6040, 0x00, g_ucTempBuf, 2);
			write_SDO(CO->SDOclient, CANopenSlaveID2, 0x6040, 0x00, g_ucTempBuf, 2);

			//设置为速度模式
			g_ucTempBuf[0]=0x03;
			g_ucTempBuf[1]=0x00;
			write_SDO(CO->SDOclient, CANopenSlaveID1, 0x6060, 0x00, g_ucTempBuf, 1);
			write_SDO(CO->SDOclient, CANopenSlaveID2, 0x6060, 0x00, g_ucTempBuf, 1);

			//设置默认加速度
			g_ucTempBuf[0]=0xE8;
			g_ucTempBuf[1]=0x03;
			write_SDO(CO->SDOclient, CANopenSlaveID1, 0x6083, 0x00, g_ucTempBuf, 2);
			write_SDO(CO->SDOclient, CANopenSlaveID2, 0x6083, 0x00, g_ucTempBuf, 2);
			//设置默认减速度
			g_ucTempBuf[0]=0xE8;
			g_ucTempBuf[1]=0x03;
			write_SDO(CO->SDOclient, CANopenSlaveID1, 0x6084, 0x00, g_ucTempBuf, 2);
			write_SDO(CO->SDOclient, CANopenSlaveID2, 0x6084, 0x00, g_ucTempBuf, 2);
			
			g_ucTempBuf[0]=0x0F;
			g_ucTempBuf[1]=0x01;
			write_SDO(CO->SDOclient, CANopenSlaveID1, 0x6040, 0x00, g_ucTempBuf, 2);
			write_SDO(CO->SDOclient, CANopenSlaveID2, 0x6040, 0x00, g_ucTempBuf, 2);
			//设置速度,默认设置为100，实际为10r/s，600r/s
			g_ucTempBuf[0]=0x64;
			g_ucTempBuf[1]=0x00;
			g_ucTempBuf[2]=0x00;
			g_ucTempBuf[3]=0x00;
			
			CO_SDO_abortCode_t abortCode3 = write_SDO(CO->SDOclient, CANopenSlaveID1, 0x6081, 0x00, g_ucTempBuf, 4);
			if (abortCode3 != CO_SDO_AB_NONE)
				{
				// 处理错误
				printf("电机1初始速度设置错误, 错误码: %lu\n", (unsigned long)abortCode3);
				
				}		
			CO_SDO_abortCode_t abortCode4 = write_SDO(CO->SDOclient, CANopenSlaveID2, 0x6081, 0x00, g_ucTempBuf, 4);
			if (abortCode4 != CO_SDO_AB_NONE) 
				{
				// 处理错误
				printf("电机2初始速度设置错误, 错误码: %lu\n", (unsigned long)abortCode4);
				}		
			motorspeed_state.mode=1;
			printf("速度模式设置完成，可以启动");
		}
		else if(motorspeed_state.mode == SPEED)
		{
			printf("速度模式已设置");
		}
	
}
		
		


/*
*********************************************************************************************************
*	函 数 名:motorstart(void)
*	功能说明: 启动电机
*	形    参: 
*	返 回 值: 
*********************************************************************************************************
*/
void motorstart(void)
{
	if (motorspeed_state.mode == SPEED)
	{
		g_ucTempBuf[0]=0x0F;
		g_ucTempBuf[1]=0x00;
		write_SDO(CO->SDOclient, CANopenSlaveID1, 0x6040, 0x00, g_ucTempBuf, 2);
		write_SDO(CO->SDOclient, CANopenSlaveID2, 0x6040, 0x00, g_ucTempBuf, 2);
		motorspeed_state.run_flag= 1;
		printf("电机已启动");
	}
	else if(motorspeed_state.mode == IDLE)
	{
		if(motorspeed_state.enable_flag==0)
		{
			printf("电机未使能");
		}	
		else
		{
		printf("电机已使能，但未配置模式");
		}
	}
}

/*
*********************************************************************************************************
*	函 数 名:motorstop(void)
*	功能说明:暂停电机
*	形    参: 
*	返 回 值: 
*********************************************************************************************************
*/
void motorstop(void)
{
	if (motorspeed_state.run_flag == 1)
	{
	g_ucTempBuf[0]=0x0F;
	g_ucTempBuf[1]=0x01;
	write_SDO(CO->SDOclient, CANopenSlaveID1, 0x6040, 0x00, g_ucTempBuf, 2);	
	write_SDO(CO->SDOclient, CANopenSlaveID2, 0x6040, 0x00, g_ucTempBuf, 2);
	printf("电机已暂停");
		motorspeed_state.run_flag = 0;
	}
	else if(motorspeed_state.run_flag == 0)
	{
	printf("电机未启动");
	}
}

/*
*********************************************************************************************************
*	函 数 名:setspeed(int16_t motor_speed)
*	功能说明: 同时设置两个电机目标速度值
*	形    参: 速度值
*	返 回 值: 
*********************************************************************************************************
*/
void setspeed(int16_t motor_speed)
{
	uint16_t speed_tem=0;
	if(motor_speed >SPEED_MAX)
	{
		speed_tem = SPEED_MAX;
	}
	speed_tem =motor_speed;
	g_ucTempBuf[0] = BYTE_0(speed_tem); // 低8位
  g_ucTempBuf[1] = BYTE_1(speed_tem); // 高8位
  g_ucTempBuf[2] = BYTE_2(speed_tem);
  g_ucTempBuf[3] = BYTE_3(speed_tem);
	write_SDO(CO->SDOclient, CANopenSlaveID1, 0x6081, 0x00, g_ucTempBuf, 4);
	write_SDO(CO->SDOclient, CANopenSlaveID2, 0x6081, 0x00, g_ucTempBuf, 4);
	printf("速度设置为%d",speed_tem);

}
/*
*********************************************************************************************************
*	函 数 名:setspeed1(int16_t motor_speed)
*	功能说明: 设置1号电机目标速度值
*	形    参: 速度值
*	返 回 值: 
*********************************************************************************************************
*/
void setspeed_1(int16_t motor_speed)
{
		uint32_t speed_tem = 0; 
	 // 处理正速度
    if (motor_speed > 0)
    {
        if (motor_speed > SPEED_MAX)
        {
            speed_tem = SPEED_MAX;
        }
        else
        {
            speed_tem = motor_speed;
        }
    }
    // 处理负速度
    else if (motor_speed < 0)
    {
       if (motor_speed < SPEED_MIN)
        {
            speed_tem = (uint32_t)(int32_t)SPEED_MIN;
        }
        else
        {
            speed_tem = (uint32_t)(int32_t)motor_speed;
        }
    }

    // 将32位整数的字节存储到缓冲区中
    g_ucTempBuf[0] = BYTE_0(speed_tem); // 低8位
    g_ucTempBuf[1] = BYTE_1(speed_tem); // 接下来的8位
    g_ucTempBuf[2] = BYTE_2(speed_tem); // 接下来的8位
    g_ucTempBuf[3] = BYTE_3(speed_tem); // 高8位

    // 将速度写入SDO
    write_SDO(CO->SDOclient, CANopenSlaveID1, 0x6081, 0x00, g_ucTempBuf, 4);

    printf("1号电机速度设置为%d",speed_tem);
	}
/*
*********************************************************************************************************
*	函 数 名:setspeed2(int16_t motor_speed)
*	功能说明: 设置2号电机目标速度值
*	形    参: 速度值
*	返 回 值: 
*********************************************************************************************************
*/
void setspeed_2(int16_t motor_speed)
{
			uint32_t speed_tem = 0; 
	 // 处理正速度
    if (motor_speed > 0)
    {
        if (motor_speed > SPEED_MAX)
        {
            speed_tem = SPEED_MAX;
        }
        else
        {
            speed_tem = motor_speed;
        }
    }
    // 处理负速度
    else if (motor_speed < 0)
    {
        if (motor_speed < SPEED_MIN)
        {
            speed_tem = (uint32_t)(int32_t)SPEED_MIN;
        }
        else
        {
            speed_tem = (uint32_t)(int32_t)motor_speed;
        }
    }

    // 将32位整数的字节存储到缓冲区中
    g_ucTempBuf[0] = BYTE_0(speed_tem); // 低8位
    g_ucTempBuf[1] = BYTE_1(speed_tem); // 接下来的8位
    g_ucTempBuf[2] = BYTE_2(speed_tem); // 接下来的8位
    g_ucTempBuf[3] = BYTE_3(speed_tem); // 高8位

    // 将速度写入SDO
    write_SDO(CO->SDOclient, CANopenSlaveID2, 0x6081, 0x00, g_ucTempBuf, 4);

    printf("2号电机速度设置为%d", speed_tem);
}


uint32_t g_uiReadSize;

/*
*********************************************************************************************************
*	函 数 名:readspeed(uint8_t nodeId)
*	功能说明: 读取实际速度值
*	形    参: 节点id
*	返 回 值: 
*********************************************************************************************************
*/
int32_t readspeed(uint8_t nodeId)
{
read_SDO(CO->SDOclient, nodeId, 0x606C, 0x00, g_ucTempBuf, sizeof(g_ucTempBuf), &g_uiReadSize);	
int32_t actual_speed = (int32_t)((g_ucTempBuf[0]) | (g_ucTempBuf[1] << 8) | (g_ucTempBuf[2] << 16) | (g_ucTempBuf[3] << 24));
printf("%d号电机实际速度值为：%d",nodeId,actual_speed);
return actual_speed;
}
/*
*********************************************************************************************************
*	函 数 名: readspeed_taget
*	功能说明: 读取目标速度值
*	形    参: 节点id
*	返 回 值: 
*********************************************************************************************************
*/
int16_t readspeed_taget(uint8_t nodeId)
{
read_SDO(CO->SDOclient, nodeId, 0x6081, 0x00, g_ucTempBuf, sizeof(g_ucTempBuf), &g_uiReadSize);	

int16_t taget_speed = (int16_t)((g_ucTempBuf[1] << 8) | g_ucTempBuf[0]);
	printf("%d号电机目标速度值为：%d",nodeId,taget_speed);
return taget_speed;
}
/*
*********************************************************************************************************
*	函 数 名:setacc(uint16_t motor_acc)、setdcc(uint16_t motor_dcc)
*	功能说明: 设置加减速度
*	形    参: 
*	返 回 值: 
*********************************************************************************************************
*/
void setacc(uint16_t motor_acc)
{
		uint16_t acc_tem=0;
		acc_tem =motor_acc;
		g_ucTempBuf[0] = BYTE_0(acc_tem); // 低8位
		g_ucTempBuf[1] = BYTE_1(acc_tem); // 高8位
		g_ucTempBuf[2] = BYTE_2(acc_tem);
		g_ucTempBuf[3] = BYTE_3(acc_tem);
		write_SDO(CO->SDOclient, CANopenSlaveID1, 0x6083, 0x00, g_ucTempBuf, 2);
		write_SDO(CO->SDOclient, CANopenSlaveID2, 0x6083, 0x00, g_ucTempBuf, 2);
		printf("电机加速度为：%d",acc_tem);
}
void setdcc(uint16_t motor_dcc)
{
		uint16_t dcc_tem=0;
		dcc_tem =motor_dcc;
		g_ucTempBuf[0] = BYTE_0(dcc_tem); // 低8位
		g_ucTempBuf[1] = BYTE_1(dcc_tem); // 高8位
		g_ucTempBuf[2] = BYTE_2(dcc_tem);
		g_ucTempBuf[3] = BYTE_3(dcc_tem);
		write_SDO(CO->SDOclient, CANopenSlaveID1, 0x6084, 0x00, g_ucTempBuf, 2);
		write_SDO(CO->SDOclient, CANopenSlaveID2, 0x6084, 0x00, g_ucTempBuf, 2);
	printf("电机减速度为：%d",dcc_tem);
}

uint16_t readacc(void)
{
read_SDO(CO->SDOclient, CANopenSlaveID1, 0x6083, 0x00, g_ucTempBuf, sizeof(g_ucTempBuf), &g_uiReadSize);	
uint16_t motor_acc = (int16_t)((g_ucTempBuf[1] << 8) | g_ucTempBuf[0]);
return motor_acc;
}
uint16_t readdcc(void)
{
read_SDO(CO->SDOclient, CANopenSlaveID1, 0x6084, 0x00, g_ucTempBuf, sizeof(g_ucTempBuf), &g_uiReadSize);	
uint16_t motor_dcc = (int16_t)((g_ucTempBuf[1] << 8) | g_ucTempBuf[0]);
return motor_dcc;
}

/*
*********************************************************************************************************
*	函 数 名: motor_05h
*	功能说明: 处理05H指令
*	形    参：无
*	返 回 值: 错误代码(无需处理)
*********************************************************************************************************
*/

void motor_05h(void)
{
	if (g_tVar.D01 == 0xFF00) 
	{
		if(NMT_STATE==0)
			{
			SendNMTCommand();
			}
	}
	else
	{
		
	}
	
	if (g_tVar.D02 == 0xFF00) 
	{
		if(motorspeed_state.enable_flag==0)
		{
		enable_motor();
		}
	}
	else
	{
		
	}	
	
	if (g_tVar.D03 == 0xFF00) 
	{
		if(motorspeed_state.mode==IDLE)
		{
		motorspeed_init();
		}
	}
	else
	{
		
	}	
	
	if (g_tVar.D04 == 0xFF00) 
	{
		if(motorspeed_state.run_flag==0)
		{
		motorstart();
		}
	}
	else
	{
		if(motorspeed_state.run_flag==1)
		{
		motorstop();
		}
		
	}	
	if (g_tVar.D05 == 0xFF00) 
	{
		if(motorspeed_state.mode==SPEED)
		{
		setspeed_1(50);
		setspeed_2(150);
		}
		
		
	}
	else
	{
		if(motorspeed_state.mode==SPEED)
		{
		readspeed(1);
			readspeed(2);
		}
		
	}	
}
