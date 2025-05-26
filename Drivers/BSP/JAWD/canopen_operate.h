#ifndef __CANOPEN_OPERATE__
#define __CANOPEN_OPERATE__



#include "CO_app_STM32.h"
#include "CO_SDOclient.h"

extern CO_t* CO;
void SendNMTCommand(void);
//sdowrite
CO_SDO_abortCode_t write_SDO(CO_SDOclient_t *SDO_C, uint8_t nodeId,
                             uint16_t index, uint8_t subIndex,
                             uint8_t *data, size_t dataSize);
														 
														 
CO_SDO_abortCode_t read_SDO(CO_SDOclient_t *SDO_C, uint8_t nodeId,
                            uint16_t index, uint8_t subIndex,
                            uint8_t *buf, size_t bufSize, size_t *readSize);
														 
extern int8_t NMT_STATE;
#endif
