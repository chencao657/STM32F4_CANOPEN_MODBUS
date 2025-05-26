/*
*********************************************************************************************************
*
*	ģ������ : �ŷ��������ٶ�ģʽ
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
#include "JAWD_SPEED.h"
#include "canopen_operate.h"
#include "usart.h"
#include "modbus_slave.h"

/* ���id��Ĭ��a��Ϊ0x01��b��Ϊ0x02 */
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
*	�� �� ��:enable_motor(void)
*	����˵��:���ʹ�ܺ���
*	��    ��: 
*	�� �� ֵ: 
*********************************************************************************************************
*/
static void enable_motor(void)
{	
		
		//��ʼ���豸
		g_ucTempBuf[0]=0x01;
		g_ucTempBuf[1]=0x00;
		CO_SDO_abortCode_t abortCode1 = write_SDO(CO->SDOclient, CANopenSlaveID1, 0x6040, 0x00, g_ucTempBuf, 2);
		if (abortCode1 != CO_SDO_AB_NONE) {
				// �������
				printf("�ڵ�1��ʼ��ʧ��, ������: %lu\n", (unsigned long)abortCode1);
		}		
		CO_SDO_abortCode_t abortCode2 = write_SDO(CO->SDOclient, CANopenSlaveID2, 0x6040, 0x00, g_ucTempBuf, 2);
		if (abortCode2 != CO_SDO_AB_NONE) {
				// �������
				printf("�ڵ�2��ʼ��ʧ��, ������: %lu\n", (unsigned long)abortCode2);
		}		
		//��ɲ��
		g_ucTempBuf[0]=0x03;
		g_ucTempBuf[1]=0x00;
		CO_SDO_abortCode_t abortCode3 = write_SDO(CO->SDOclient, CANopenSlaveID1, 0x6040, 0x00, g_ucTempBuf, 2);
		if (abortCode3 != CO_SDO_AB_NONE) {
				// �������
				printf("�ڵ�1��ɲ��ʧ��, ������: %lu\n", (unsigned long)abortCode3);
		}
		CO_SDO_abortCode_t abortCode4 = write_SDO(CO->SDOclient, CANopenSlaveID2, 0x6040, 0x00, g_ucTempBuf, 2);
		if (abortCode4 != CO_SDO_AB_NONE) {
				// �������
				printf("�ڵ�2��ɲ��ʧ��, ������: %lu\n", (unsigned long)abortCode4);
		}		
		//ʹ�ܵ��
		g_ucTempBuf[0]=0x0F;
		g_ucTempBuf[1]=0x00;
		CO_SDO_abortCode_t abortCode5=write_SDO(CO->SDOclient, CANopenSlaveID1, 0x6040, 0x00, g_ucTempBuf, 2);
		if (abortCode5 != CO_SDO_AB_NONE) {
				// �������
				printf("�ڵ�1ʹ��ʧ��, ������: %lu\n", (unsigned long)abortCode5);
		}
		CO_SDO_abortCode_t abortCode6=write_SDO(CO->SDOclient, CANopenSlaveID2, 0x6040, 0x00, g_ucTempBuf, 2);
		if (abortCode6 != CO_SDO_AB_NONE) {
				// �������
				printf("�ڵ�2ʹ��ʧ�ܰ�, ������: %lu\n", (unsigned long)abortCode6);
		}
		
		motorspeed_state.enable_flag= 1;
		printf("���ʹ�ܳɹ�,��ȴ��ٶ�ģʽ��ʼ��");
		
}	

/*
*********************************************************************************************************
*	�� �� ��:motorspeed_init(motor_speed)
*	����˵��: ���õ��Ϊ�ٶ�ģʽ���������ٶ�
*	��    ��: �ٶ�motor_speed
*	�� �� ֵ: 
*********************************************************************************************************
*/
void motorspeed_init(void)
{
		if(motorspeed_state.mode == IDLE)
		{
			/* �ȴ���ʼ����� */
			if( motorspeed_state.enable_flag == 0)
			{
				printf("�ȴ����ʹ�ܳɹ�");
				return;
				
			}
			//��ͣ�������
			g_ucTempBuf[0]=0x0F;
			g_ucTempBuf[1]=0x01;
			write_SDO(CO->SDOclient, CANopenSlaveID1, 0x6040, 0x00, g_ucTempBuf, 2);
			write_SDO(CO->SDOclient, CANopenSlaveID2, 0x6040, 0x00, g_ucTempBuf, 2);

			//����Ϊ�ٶ�ģʽ
			g_ucTempBuf[0]=0x03;
			g_ucTempBuf[1]=0x00;
			write_SDO(CO->SDOclient, CANopenSlaveID1, 0x6060, 0x00, g_ucTempBuf, 1);
			write_SDO(CO->SDOclient, CANopenSlaveID2, 0x6060, 0x00, g_ucTempBuf, 1);

			//����Ĭ�ϼ��ٶ�
			g_ucTempBuf[0]=0xE8;
			g_ucTempBuf[1]=0x03;
			write_SDO(CO->SDOclient, CANopenSlaveID1, 0x6083, 0x00, g_ucTempBuf, 2);
			write_SDO(CO->SDOclient, CANopenSlaveID2, 0x6083, 0x00, g_ucTempBuf, 2);
			//����Ĭ�ϼ��ٶ�
			g_ucTempBuf[0]=0xE8;
			g_ucTempBuf[1]=0x03;
			write_SDO(CO->SDOclient, CANopenSlaveID1, 0x6084, 0x00, g_ucTempBuf, 2);
			write_SDO(CO->SDOclient, CANopenSlaveID2, 0x6084, 0x00, g_ucTempBuf, 2);
			
			g_ucTempBuf[0]=0x0F;
			g_ucTempBuf[1]=0x01;
			write_SDO(CO->SDOclient, CANopenSlaveID1, 0x6040, 0x00, g_ucTempBuf, 2);
			write_SDO(CO->SDOclient, CANopenSlaveID2, 0x6040, 0x00, g_ucTempBuf, 2);
			//�����ٶ�,Ĭ������Ϊ100��ʵ��Ϊ10r/s��600r/s
			g_ucTempBuf[0]=0x64;
			g_ucTempBuf[1]=0x00;
			g_ucTempBuf[2]=0x00;
			g_ucTempBuf[3]=0x00;
			
			CO_SDO_abortCode_t abortCode3 = write_SDO(CO->SDOclient, CANopenSlaveID1, 0x6081, 0x00, g_ucTempBuf, 4);
			if (abortCode3 != CO_SDO_AB_NONE)
				{
				// �������
				printf("���1��ʼ�ٶ����ô���, ������: %lu\n", (unsigned long)abortCode3);
				
				}		
			CO_SDO_abortCode_t abortCode4 = write_SDO(CO->SDOclient, CANopenSlaveID2, 0x6081, 0x00, g_ucTempBuf, 4);
			if (abortCode4 != CO_SDO_AB_NONE) 
				{
				// �������
				printf("���2��ʼ�ٶ����ô���, ������: %lu\n", (unsigned long)abortCode4);
				}		
			motorspeed_state.mode=1;
			printf("�ٶ�ģʽ������ɣ���������");
		}
		else if(motorspeed_state.mode == SPEED)
		{
			printf("�ٶ�ģʽ������");
		}
	
}
		
		


/*
*********************************************************************************************************
*	�� �� ��:motorstart(void)
*	����˵��: �������
*	��    ��: 
*	�� �� ֵ: 
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
		printf("���������");
	}
	else if(motorspeed_state.mode == IDLE)
	{
		if(motorspeed_state.enable_flag==0)
		{
			printf("���δʹ��");
		}	
		else
		{
		printf("�����ʹ�ܣ���δ����ģʽ");
		}
	}
}

/*
*********************************************************************************************************
*	�� �� ��:motorstop(void)
*	����˵��:��ͣ���
*	��    ��: 
*	�� �� ֵ: 
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
	printf("�������ͣ");
		motorspeed_state.run_flag = 0;
	}
	else if(motorspeed_state.run_flag == 0)
	{
	printf("���δ����");
	}
}

/*
*********************************************************************************************************
*	�� �� ��:setspeed(int16_t motor_speed)
*	����˵��: ͬʱ�����������Ŀ���ٶ�ֵ
*	��    ��: �ٶ�ֵ
*	�� �� ֵ: 
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
	g_ucTempBuf[0] = BYTE_0(speed_tem); // ��8λ
  g_ucTempBuf[1] = BYTE_1(speed_tem); // ��8λ
  g_ucTempBuf[2] = BYTE_2(speed_tem);
  g_ucTempBuf[3] = BYTE_3(speed_tem);
	write_SDO(CO->SDOclient, CANopenSlaveID1, 0x6081, 0x00, g_ucTempBuf, 4);
	write_SDO(CO->SDOclient, CANopenSlaveID2, 0x6081, 0x00, g_ucTempBuf, 4);
	printf("�ٶ�����Ϊ%d",speed_tem);

}
/*
*********************************************************************************************************
*	�� �� ��:setspeed1(int16_t motor_speed)
*	����˵��: ����1�ŵ��Ŀ���ٶ�ֵ
*	��    ��: �ٶ�ֵ
*	�� �� ֵ: 
*********************************************************************************************************
*/
void setspeed_1(int16_t motor_speed)
{
		uint32_t speed_tem = 0; 
	 // �������ٶ�
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
    // �����ٶ�
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

    // ��32λ�������ֽڴ洢����������
    g_ucTempBuf[0] = BYTE_0(speed_tem); // ��8λ
    g_ucTempBuf[1] = BYTE_1(speed_tem); // ��������8λ
    g_ucTempBuf[2] = BYTE_2(speed_tem); // ��������8λ
    g_ucTempBuf[3] = BYTE_3(speed_tem); // ��8λ

    // ���ٶ�д��SDO
    write_SDO(CO->SDOclient, CANopenSlaveID1, 0x6081, 0x00, g_ucTempBuf, 4);

    printf("1�ŵ���ٶ�����Ϊ%d",speed_tem);
	}
/*
*********************************************************************************************************
*	�� �� ��:setspeed2(int16_t motor_speed)
*	����˵��: ����2�ŵ��Ŀ���ٶ�ֵ
*	��    ��: �ٶ�ֵ
*	�� �� ֵ: 
*********************************************************************************************************
*/
void setspeed_2(int16_t motor_speed)
{
			uint32_t speed_tem = 0; 
	 // �������ٶ�
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
    // �����ٶ�
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

    // ��32λ�������ֽڴ洢����������
    g_ucTempBuf[0] = BYTE_0(speed_tem); // ��8λ
    g_ucTempBuf[1] = BYTE_1(speed_tem); // ��������8λ
    g_ucTempBuf[2] = BYTE_2(speed_tem); // ��������8λ
    g_ucTempBuf[3] = BYTE_3(speed_tem); // ��8λ

    // ���ٶ�д��SDO
    write_SDO(CO->SDOclient, CANopenSlaveID2, 0x6081, 0x00, g_ucTempBuf, 4);

    printf("2�ŵ���ٶ�����Ϊ%d", speed_tem);
}


uint32_t g_uiReadSize;

/*
*********************************************************************************************************
*	�� �� ��:readspeed(uint8_t nodeId)
*	����˵��: ��ȡʵ���ٶ�ֵ
*	��    ��: �ڵ�id
*	�� �� ֵ: 
*********************************************************************************************************
*/
int32_t readspeed(uint8_t nodeId)
{
read_SDO(CO->SDOclient, nodeId, 0x606C, 0x00, g_ucTempBuf, sizeof(g_ucTempBuf), &g_uiReadSize);	
int32_t actual_speed = (int32_t)((g_ucTempBuf[0]) | (g_ucTempBuf[1] << 8) | (g_ucTempBuf[2] << 16) | (g_ucTempBuf[3] << 24));
printf("%d�ŵ��ʵ���ٶ�ֵΪ��%d",nodeId,actual_speed);
return actual_speed;
}
/*
*********************************************************************************************************
*	�� �� ��: readspeed_taget
*	����˵��: ��ȡĿ���ٶ�ֵ
*	��    ��: �ڵ�id
*	�� �� ֵ: 
*********************************************************************************************************
*/
int16_t readspeed_taget(uint8_t nodeId)
{
read_SDO(CO->SDOclient, nodeId, 0x6081, 0x00, g_ucTempBuf, sizeof(g_ucTempBuf), &g_uiReadSize);	

int16_t taget_speed = (int16_t)((g_ucTempBuf[1] << 8) | g_ucTempBuf[0]);
	printf("%d�ŵ��Ŀ���ٶ�ֵΪ��%d",nodeId,taget_speed);
return taget_speed;
}
/*
*********************************************************************************************************
*	�� �� ��:setacc(uint16_t motor_acc)��setdcc(uint16_t motor_dcc)
*	����˵��: ���üӼ��ٶ�
*	��    ��: 
*	�� �� ֵ: 
*********************************************************************************************************
*/
void setacc(uint16_t motor_acc)
{
		uint16_t acc_tem=0;
		acc_tem =motor_acc;
		g_ucTempBuf[0] = BYTE_0(acc_tem); // ��8λ
		g_ucTempBuf[1] = BYTE_1(acc_tem); // ��8λ
		g_ucTempBuf[2] = BYTE_2(acc_tem);
		g_ucTempBuf[3] = BYTE_3(acc_tem);
		write_SDO(CO->SDOclient, CANopenSlaveID1, 0x6083, 0x00, g_ucTempBuf, 2);
		write_SDO(CO->SDOclient, CANopenSlaveID2, 0x6083, 0x00, g_ucTempBuf, 2);
		printf("������ٶ�Ϊ��%d",acc_tem);
}
void setdcc(uint16_t motor_dcc)
{
		uint16_t dcc_tem=0;
		dcc_tem =motor_dcc;
		g_ucTempBuf[0] = BYTE_0(dcc_tem); // ��8λ
		g_ucTempBuf[1] = BYTE_1(dcc_tem); // ��8λ
		g_ucTempBuf[2] = BYTE_2(dcc_tem);
		g_ucTempBuf[3] = BYTE_3(dcc_tem);
		write_SDO(CO->SDOclient, CANopenSlaveID1, 0x6084, 0x00, g_ucTempBuf, 2);
		write_SDO(CO->SDOclient, CANopenSlaveID2, 0x6084, 0x00, g_ucTempBuf, 2);
	printf("������ٶ�Ϊ��%d",dcc_tem);
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
*	�� �� ��: motor_05h
*	����˵��: ����05Hָ��
*	��    �Σ���
*	�� �� ֵ: �������(���账��)
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
