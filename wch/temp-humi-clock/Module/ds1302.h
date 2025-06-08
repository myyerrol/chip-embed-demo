#ifndef DS1302_H
#define DS1302_H

#include <debug.h>

#define DS1302_DEBUG
// #define DS1302_SETTIME

// The highest bit CH is a clock stop flag. If it is 1, it means the clock is
// abnormal. The highest 3 bits of the 7 bits are the tens of seconds, and the
// lowest 4 bits are the ones of seconds.
#define DS1302_WR_SEC  0x80
// The highest digit is not used. The highest 3 digits of the remaining 7
// digits are the tens digit of the minute, and the lowest 4 digits are the o
// nes digit of the minute.
#define DS1302_WR_MIN  0x82
// If bit7 is 1, it represents the 12-hour system, and 0 represents the 24-hour
// system; bit6 is fixed to 0, and bit5 represents morning in the 12-hour
// system and afternoon in the 12-hour system. In the 24-hour system, bit7 and
// bit4 together represent the tens digit of the hour. In the 12-hour system,
// bit4 represents the tens digit of the hour, and the lower 4 bits represent
//the ones digit of the hour.
#define DS1302_WR_HOUR 0x84
// The upper 2 bits are fixed to 0, bit5 and bit4 are the tens digit of the
// date, and the lower 4 bits are the ones digit of the date¡£
#define DS1302_WR_DATE 0x86
// The upper 3 bits are fixed to 0, bit 4 is the tens digit of the month, and
// the lower 4 bits are the ones digit of the month.
#define DS1302_WR_MON  0x88
// The high 5 bits are fixed to 0, and the low 3 bits represent the day of the
// week.
#define DS1302_WR_DAY  0x8A
// The upper 4 digits represent the tens digit of the year, and the lower 4
// digits represent the ones digit of the year. Please note that 00 to 99 here
// refers to the year 2000 to 2099.
#define DS1302_WR_YEAR 0x8C
// The highest bit is a write protection bit. If this bit is 1, it is forbidden
// to write data to any other register or the 31 bytes of RAM. Therefore,
// before writing data, this bit must be written to 0.
#define DS1302_WR_CTRL 0x8E

// Trickle charger
#define DS1302_WR_CHARGER  0x90
// Clock burst
#define DS1302_WR_CLKBURST 0xBE

#define DS1302_PIN_CLK GPIO_Pin_4
#define DS1302_PIN_DAT GPIO_Pin_6
#define DS1302_PIN_RST GPIO_Pin_0

#define DS1302_SPD  GPIO_Speed_30MHz
#define DS1302_PORT GPIOD
#define DS1302_APB2 RCC_APB2Periph_GPIOD

#define DS1302_RD_DAT   GPIO_ReadInputDataBit(DS1302_PORT, DS1302_PIN_DAT)
#define DS1302_WR_CLK_1 GPIO_SetBits         (DS1302_PORT, DS1302_PIN_CLK)
#define DS1302_WR_CLK_0 GPIO_ResetBits       (DS1302_PORT, DS1302_PIN_CLK)
#define DS1302_WR_DAT_1 GPIO_SetBits         (DS1302_PORT, DS1302_PIN_DAT)
#define DS1302_WR_DAT_0 GPIO_ResetBits       (DS1302_PORT, DS1302_PIN_DAT)
#define DS1302_WR_RST_1 GPIO_SetBits         (DS1302_PORT, DS1302_PIN_RST)
#define DS1302_WR_RST_0 GPIO_ResetBits       (DS1302_PORT, DS1302_PIN_RST)

#define BOOL  uint8_t
#define TRUE  1
#define FALSE 0

#define BCD2DEC(bcd) ((uint8_t)((10 * (((bcd) & 0xF0) >> 4)) + ((bcd) & 0x0F)))
#define DEC2BCD(dec) (((uint8_t)((dec) / 10) << 4) | ((dec) % 10))

typedef struct {
    uint16_t DS1302_Year;  // 2000 - 2099
    uint8_t  DS1302_Month; // 1 - 12
    uint8_t  DS1302_Date;  // 1 - 31
    uint8_t  DS1302_Day;   // 1 - 7
    BOOL     DS1302_Is24;  // TRUE or FALSE
    BOOL     DS1302_IsAM;  // TRUE or FALSE, This has no effect if DS1302_Is24 is TRUE
    uint8_t  DS1302_Hour;  // DS1302_Is24 is TRUE: 0 - 23, DS1302_Is24 is FALSE: 1 - 12 (with DS1302_IsAM)
    uint8_t  DS1302_Min;   // 00 - 59
    uint8_t  DS1302_Sec;   // 00 - 59
} DS1302_InitTypeDef;

void DS1302_GetTime(DS1302_InitTypeDef *DS1302_InitStructure);
void DS1302_GetTimeBurst(DS1302_InitTypeDef *DS1302_InitStructure);
void DS1302_Init();
void DS1302_SetTime(DS1302_InitTypeDef *DS1302_InitStructure);
void DS1302_SetTimeBurst(DS1302_InitTypeDef *DS1302_InitStructure);

#endif
