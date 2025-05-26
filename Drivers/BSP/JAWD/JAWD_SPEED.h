#ifndef __JAWD_SPEED__
#define __JAWD_SPEED__

#include "stdint.h"

#define IDLE   0
#define SPEED  1

#define BYTE_0(x) ((x) & 0xFF)
#define BYTE_1(x) (((x) >> 8) & 0xFF)
#define BYTE_2(x) (((x) >> 16) & 0xFF)
#define BYTE_3(x) (((x) >> 24) & 0xFF)

#define SPEED_MAX        (400)
#define SPEED_MIN        (-400)
#define MOTOR_SPEED      (1000)
/* 电机状态结构体 */
typedef struct
{
uint8_t mode;
uint8_t run_flag;
uint8_t enable_flag;
	
}motor_speedstate_t;

void motorspeed_init(void);
void motorstop(void);
void motorstart(void);	
void setspeed(int16_t motor_speed);
void setspeed_1(int16_t motor_speed);
void setspeed_2(int16_t motor_speed);
int32_t readspeed(uint8_t nodeId);
int16_t readspeed_taget(uint8_t nodeId);
void setacc(uint16_t motor_acc);
void setdcc(uint16_t motor_dcc);
uint16_t readacc(void);
uint16_t readdcc(void);
void motor_05h(void);
#endif
