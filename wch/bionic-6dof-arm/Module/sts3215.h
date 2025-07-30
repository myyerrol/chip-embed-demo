#ifndef STS3215_H
#define STS3215_H

#include <stdint.h>

#define STS3215_1M     0
#define STS3215_0_5M   1
#define STS3215_250K   2
#define STS3215_128K   3
#define STS3215_115200 4
#define STS3215_76800  5
#define STS3215_57600  6
#define STS3215_38400  7

//内存表定义
//-------EPROM(只读)--------
#define STS3215_MODEL_L 3
#define STS3215_MODEL_H 4

//-------EPROM(读写)--------
#define STS3215_ID 5
#define STS3215_BAUD_RATE 6
#define STS3215_MIN_ANGLE_LIMIT_L 9
#define STS3215_MIN_ANGLE_LIMIT_H 10
#define STS3215_MAX_ANGLE_LIMIT_L 11
#define STS3215_MAX_ANGLE_LIMIT_H 12
#define STS3215_CW_DEAD 26
#define STS3215_CCW_DEAD 27
#define STS3215_OFS_L 31
#define STS3215_OFS_H 32
#define STS3215_MODE 33

//-------SRAM(读写)--------
#define STS3215_TORQUE_ENABLE 40
#define STS3215_ACC 41
#define STS3215_GOAL_POSITION_L 42
#define STS3215_GOAL_POSITION_H 43
#define STS3215_GOAL_TIME_L 44
#define STS3215_GOAL_TIME_H 45
#define STS3215_GOAL_SPEED_L 46
#define STS3215_GOAL_SPEED_H 47
#define STS3215_LOCK 55

//-------SRAM(只读)--------
#define STS3215_PRESENT_POSITION_L 56
#define STS3215_PRESENT_POSITION_H 57
#define STS3215_PRESENT_SPEED_L 58
#define STS3215_PRESENT_SPEED_H 59
#define STS3215_PRESENT_LOAD_L 60
#define STS3215_PRESENT_LOAD_H 61
#define STS3215_PRESENT_VOLTAGE 62
#define STS3215_PRESENT_TEMPERATURE 63
#define STS3215_MOVING 66
#define STS3215_PRESENT_CURRENT_L 69
#define STS3215_PRESENT_CURRENT_H 70

int WritePosEx(uint8_t ID, int16_t Position, uint16_t Speed, uint8_t ACC);//普通写位置指令
int RegWritePosEx(uint8_t ID, int16_t Position, uint16_t Speed, uint8_t ACC);//异步写位置指令
void SyncWritePosEx(uint8_t ID[], uint8_t IDN, int16_t Position[], uint16_t Speed[], uint8_t ACC[]);//同步写位置指令
int WheelMode(uint8_t ID);//恒速模式
int WriteSpe(uint8_t ID, int16_t Speed, uint8_t ACC);//恒速模式控制指令
int CalibrationOfs(uint8_t ID);//中位校准
int unLockEpromEx(uint8_t ID);
int LockEpromEx(uint8_t ID);

#endif
