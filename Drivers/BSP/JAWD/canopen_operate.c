/*
*********************************************************************************************************
*
*	ģ������ : canopenЭ��ջ����
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
#include "canopen_operate.h"
#include "can.h"
#include "usart.h"
#include "CO_app_STM32.h"

int8_t NMT_STATE = 0;
/*
*********************************************************************************************************
*	�� �� ��:SendNMTCommand
*	����˵��: Ԥ����������״̬
*	��    ��: 
*	�� �� ֵ: �޷���ֵ
*********************************************************************************************************
*/
void SendNMTCommand(void)
{

CO_ReturnError_t result = CO_NMT_sendCommand(CO->NMT, CO_NMT_ENTER_OPERATIONAL, 0);
if(result == CO_ERROR_NO)
{
	NMT_STATE = 1;
}
else
{
	printf("NMT�������\r\n");
}

}

/*
*********************************************************************************************************
*        �� �� ��: write_SDO
*        ����˵��: SDOд��
*        ��    �Σ�-
*        �� �� ֵ: -
*********************************************************************************************************
*/
CO_SDO_abortCode_t write_SDO(CO_SDOclient_t *SDO_C, uint8_t nodeId,
                             uint16_t index, uint8_t subIndex,
                             uint8_t *data, size_t dataSize)
{
    CO_SDO_return_t SDO_ret;
    bool_t bufferPartial = false;
 
    // setup client (this can be skipped, if remote device is the same)
    SDO_ret = CO_SDOclient_setup(SDO_C,
                                 CO_CAN_ID_SDO_CLI + nodeId,
                                 CO_CAN_ID_SDO_SRV + nodeId,
                                 nodeId);
    if (SDO_ret != CO_SDO_RT_ok_communicationEnd) {
        return -1;
    }
 
    // initiate download
    SDO_ret = CO_SDOclientDownloadInitiate(SDO_C, index, subIndex,
                                           dataSize, 1000, false);
    if (SDO_ret != CO_SDO_RT_ok_communicationEnd) {
        return -1;
    }
 
    // fill data
    size_t nWritten = CO_SDOclientDownloadBufWrite(SDO_C, data, dataSize);
    if (nWritten < dataSize) {
        bufferPartial = true;
        // If SDO Fifo buffer is too small, data can be refilled in the loop.
    }
 
    //download data
    do {
        uint32_t timeDifference_us = 10000;
        CO_SDO_abortCode_t abortCode = CO_SDO_AB_NONE;
 
        SDO_ret = CO_SDOclientDownload(SDO_C,
                                       timeDifference_us,
                                       false,
                                       bufferPartial,
                                       &abortCode,
                                       NULL, NULL);
        if (SDO_ret < 0) {
            return abortCode;
        }
 
        HAL_Delay(timeDifference_us/1000);
    } while(SDO_ret > 0);
		
    return CO_SDO_AB_NONE;
}

/*
*********************************************************************************************************
*        �� �� ��: read_SDO
*        ����˵��: SDO��ȡ
*        ��    �Σ�-
*        �� �� ֵ: -
*********************************************************************************************************
*/
CO_SDO_abortCode_t read_SDO(CO_SDOclient_t *SDO_C, uint8_t nodeId,
                            uint16_t index, uint8_t subIndex,
                            uint8_t *buf, size_t bufSize, size_t *readSize)
{
    CO_SDO_return_t SDO_ret;
 
    // setup client (this can be skipped, if remote device don't change)
    SDO_ret = CO_SDOclient_setup(SDO_C,
                                 CO_CAN_ID_SDO_CLI + nodeId,
                                 CO_CAN_ID_SDO_SRV + nodeId,
                                 nodeId);
    if (SDO_ret != CO_SDO_RT_ok_communicationEnd) {
        return CO_SDO_AB_GENERAL;
    }
 
    // initiate upload
    SDO_ret = CO_SDOclientUploadInitiate(SDO_C, index, subIndex, 1000, false);
    if (SDO_ret != CO_SDO_RT_ok_communicationEnd) {
        return CO_SDO_AB_GENERAL;
    }
 
    // upload data
    do {
        uint32_t timeDifference_us = 10000;
        CO_SDO_abortCode_t abortCode = CO_SDO_AB_NONE;
 
        SDO_ret = CO_SDOclientUpload(SDO_C,
                                     timeDifference_us,
                                     false,
                                     &abortCode,
                                     NULL, NULL, NULL);
        if (SDO_ret < 0) {
            return abortCode;
        }
 
       HAL_Delay(timeDifference_us/1000);
    } while(SDO_ret > 0);
 
    // copy data to the user buffer (for long data function must be called
    // several times inside the loop)
    *readSize = CO_SDOclientUploadBufRead(SDO_C, buf, bufSize);
 
    return CO_SDO_AB_NONE;
}
