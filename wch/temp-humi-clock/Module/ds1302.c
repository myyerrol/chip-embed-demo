#include <ds1302.h>

/******************************************************************************
 * @fn      DS1302_FormatStructData
 * @brief   Format structured data.
 * @param  *DS1302_InitStructure - Original structured data.
 * @return  Formatted structured data.
 */
static DS1302_InitTypeDef DS1302_FormatStructData(DS1302_InitTypeDef
                                                 *DS1302_InitStructure) {
    if (DS1302_InitStructure->DS1302_Sec < 0 ||
        DS1302_InitStructure->DS1302_Sec > 59) {
        DS1302_InitStructure->DS1302_Sec = 0;
    }

    if (DS1302_InitStructure->DS1302_Min < 0 ||
        DS1302_InitStructure->DS1302_Min > 59) {
        DS1302_InitStructure->DS1302_Min = 0;
    }

    if (DS1302_InitStructure->DS1302_Is24) {
        if (DS1302_InitStructure->DS1302_Hour < 0 ||
            DS1302_InitStructure->DS1302_Hour > 23) {
            DS1302_InitStructure->DS1302_Hour = 0;
        }
    }
    else {
        if (DS1302_InitStructure->DS1302_Hour < 1 ||
            DS1302_InitStructure->DS1302_Hour > 12) {
            DS1302_InitStructure->DS1302_Hour = 1;
        }
    }

    if (DS1302_InitStructure->DS1302_Date < 1 ||
        DS1302_InitStructure->DS1302_Date > 31) {
        DS1302_InitStructure->DS1302_Date = 1;
    }

    if (DS1302_InitStructure->DS1302_Day < 1 ||
        DS1302_InitStructure->DS1302_Day > 7) {
        DS1302_InitStructure->DS1302_Day = 1;
    }

    if (DS1302_InitStructure->DS1302_Month < 1 ||
        DS1302_InitStructure->DS1302_Month > 12) {
        DS1302_InitStructure->DS1302_Month = 1;
    }

    if (DS1302_InitStructure->DS1302_Year < 2000 ||
        DS1302_InitStructure->DS1302_Year > 2099) {
        DS1302_InitStructure->DS1302_Year = 00;
    }
    else {
        DS1302_InitStructure->DS1302_Year -= 2000;
    }

    uint8_t hour = 0;
    if (DS1302_InitStructure->DS1302_Is24) {
        hour |= DEC2BCD(DS1302_InitStructure->DS1302_Hour) & 0x3F;
    }
    else {
        hour |= 0x80;
        if (!DS1302_InitStructure->DS1302_IsAM) {
            hour |= 0x20;
        }
        hour |= DEC2BCD(DS1302_InitStructure->DS1302_Hour) & 0x1F;
    }
    DS1302_InitStructure->DS1302_Hour = hour;

    return *DS1302_InitStructure;
}

/******************************************************************************
 * @fn      DS1302_SetRdMode
 * @brief   Set the DAT pin to input mode.
 * @param   None.
 * @return  None.
 */
static void DS1302_SetRdMode(void) {
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin   = DS1302_PIN_DAT;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN_FLOATING;
    GPIO_Init(DS1302_PORT, &GPIO_InitStructure);
}

/******************************************************************************
 * @fn      DS1302_SetWrMode
 * @brief   Set the DAT pin to output mode.
 * @param   None.
 * @return  None.
 */
static void DS1302_SetWrMode(void) {
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin   = DS1302_PIN_DAT;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
    GPIO_Init(DS1302_PORT, &GPIO_InitStructure);
}

/******************************************************************************
 * @fn      DS1302_InitPin
 * @brief   Initializes the pin.
 * @param   *GPIOx    - Pin port.
 *           GPIO_Pin - Pin number.
 * @return  None.
 */
static void DS1302_InitPin(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin) {
    GPIO_InitTypeDef GPIO_InitStruct;
    RCC_APB2PeriphClockCmd(DS1302_APB2, ENABLE);
    GPIO_InitStruct.GPIO_Pin   = GPIO_Pin;
    GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_Out_PP;
    GPIO_InitStruct.GPIO_Speed = DS1302_SPD;
    GPIO_Init(GPIOx, &GPIO_InitStruct);
}

/******************************************************************************
 * @fn      DS1302_RdByte
 * @brief   Read byte from the DAT pin.
 * @param   None.
 * @return  Data.
 */
static uint8_t DS1302_RdByte() {
    uint8_t data = 0;
    uint8_t detect;
    for (detect = 0x01; detect != 0; detect <<= 1) {
        if (DS1302_RD_DAT != 0) {
            data |= detect;
        }
        DS1302_WR_CLK_1;
        DS1302_WR_CLK_0;
    }
    return data;
}

/******************************************************************************
 * @fn      DS1302_WrByte
 * @brief   Write byte to the DAT pin.
 * @param   None.
 * @return  Data.
 */
static void DS1302_WrByte(uint8_t data) {
    uint8_t detect;
    for (detect = 0x01; detect != 0; detect <<= 1) {
        if ((data & detect) != 0) {
            DS1302_WR_DAT_1;
        }
        else {
            DS1302_WR_DAT_0;
        }
        DS1302_WR_CLK_1;
        DS1302_WR_CLK_0;
    }
    DS1302_WR_DAT_1;
}

/******************************************************************************
 * @fn      DS1302_RdSingle
 * @brief   Read single data from the DAT pin.
 * @param   addr - Address.
 * @return  Data.
 */
static uint8_t DS1302_RdSingle(uint8_t addr) {
    addr = addr | 0x01;

    DS1302_WR_RST_0;
    DS1302_WR_CLK_0;

    DS1302_WR_RST_1;

    DS1302_SetWrMode();

    DS1302_WrByte(addr);

    DS1302_SetRdMode();

    uint8_t data = DS1302_RdByte();
    DS1302_WR_RST_0;

    return data;
}

/******************************************************************************
 * @fn      DS1302_WrSingle
 * @brief   Write single data to the DAT pin.
 * @param   addr - Address.
 *          data - Data.
 * @return  None.
 */
static void DS1302_WrSingle(uint8_t addr, uint8_t data) {
    DS1302_WR_RST_0;
    DS1302_WR_CLK_0;

    DS1302_WR_RST_1;

    DS1302_SetWrMode();

    DS1302_WrByte(addr);
    DS1302_WrByte(data);

    DS1302_WR_RST_0;
}

/******************************************************************************
 * @fn      DS1302_RdBurst
 * @brief   Read burst data from the DAT pin.
 * @param  *data - Data.
 * @return  None.
 */
static void DS1302_RdBurst(uint8_t *data) {
    DS1302_WR_RST_0;
    DS1302_WR_CLK_0;

    DS1302_WR_RST_1;

    DS1302_SetWrMode();

    DS1302_WrByte(DS1302_WR_CLKBURST | 0x01);

    DS1302_SetRdMode();
    for (int i = 0; i < 8; i++) {
        data[i] = DS1302_RdByte();
    }

    DS1302_WR_RST_0;
}

/******************************************************************************
 * @fn      DS1302_WrBurst
 * @brief   Write burst data to the DAT pin.
 * @param  *data - Data.
 * @return  None.
 */
static void DS1302_WrBurst(uint8_t *data) {
    DS1302_WR_RST_0;
    DS1302_WR_CLK_0;

    DS1302_WR_RST_1;

    DS1302_SetWrMode();

    DS1302_WrByte(DS1302_WR_CLKBURST);
    for (int i = 0; i < 8; i++) {
        DS1302_WrByte(data[i]);
    }

    DS1302_WR_RST_0;
}

/******************************************************************************
 * @fn      DS1302_GetTime
 * @brief   Get data from DS1302.
 * @param  *DS1302_InitStructure - Data.
 * @return  None.
 */
void DS1302_GetTime(DS1302_InitTypeDef *DS1302_InitStructure) {
    uint8_t data;

    data = DS1302_RdSingle(DS1302_WR_YEAR);
    DS1302_InitStructure->DS1302_Year = BCD2DEC(data) + 2000;
    data = DS1302_RdSingle(DS1302_WR_MON);
    DS1302_InitStructure->DS1302_Month = BCD2DEC(data);
    data = DS1302_RdSingle(DS1302_WR_DAY);
    DS1302_InitStructure->DS1302_Day = BCD2DEC(data);
    data = DS1302_RdSingle(DS1302_WR_DATE);
    DS1302_InitStructure->DS1302_Date = BCD2DEC(data);
    uint8_t hour = DS1302_RdSingle(DS1302_WR_HOUR);
    data = DS1302_RdSingle(DS1302_WR_MIN);
    DS1302_InitStructure->DS1302_Min = BCD2DEC(data);
    data = DS1302_RdSingle(DS1302_WR_SEC) & 0x7F;
    DS1302_InitStructure->DS1302_Sec = BCD2DEC(data);
    DS1302_InitStructure->DS1302_IsAM = FALSE;

    if (hour & 0x80) {
        DS1302_InitStructure->DS1302_Is24 = FALSE;
        if (hour & 0x20) {
            DS1302_InitStructure->DS1302_IsAM = FALSE;
        }
        else {
            DS1302_InitStructure->DS1302_IsAM = TRUE;
        }
        DS1302_InitStructure->DS1302_Hour = BCD2DEC(hour & 0x1F);
    }
    else {
        DS1302_InitStructure->DS1302_Is24 = TRUE;
        DS1302_InitStructure->DS1302_Hour = BCD2DEC((hour & 0x1F) |
                                                    (hour & 0x20));
    }

#ifdef DS1302_DEBUG
        printf("Year: %d ",  DS1302_InitStructure->DS1302_Year);
        printf("Month: %d ", DS1302_InitStructure->DS1302_Month);
        printf("Date: %d ",  DS1302_InitStructure->DS1302_Date);
        printf("Day: %d ",   DS1302_InitStructure->DS1302_Day);
        printf("Is24: %d ",  DS1302_InitStructure->DS1302_Is24);
        printf("IsAM: %d ",  DS1302_InitStructure->DS1302_IsAM);
        printf("Hour: %d ",  DS1302_InitStructure->DS1302_Hour);
        printf("Min: %d ",   DS1302_InitStructure->DS1302_Min);
        printf("Sec: %d ",   DS1302_InitStructure->DS1302_Sec);
        printf("\n");
#endif
}

/******************************************************************************
 * @fn      DS1302_GetTimeBurst
 * @brief   Get data from the DS1302.
 * @param  *DS1302_InitStructure - Data.
 * @return  None.
 */
void DS1302_GetTimeBurst(DS1302_InitTypeDef *DS1302_InitStructure) {
    uint8_t times[8];

    DS1302_RdBurst(times);

    DS1302_InitStructure->DS1302_Year  = BCD2DEC(times[6]) + 2000;
    DS1302_InitStructure->DS1302_Month = BCD2DEC(times[4]);
    DS1302_InitStructure->DS1302_Day   = BCD2DEC(times[5]);
    DS1302_InitStructure->DS1302_Date  = BCD2DEC(times[3]);
    uint8_t hour = times[2];
    DS1302_InitStructure->DS1302_Min   = BCD2DEC(times[1]);
    DS1302_InitStructure->DS1302_Sec   = BCD2DEC(times[0] & 0x7F);
    DS1302_InitStructure->DS1302_IsAM  = FALSE;

    if (hour & 0x80) {
        DS1302_InitStructure->DS1302_Is24 = FALSE;
        if (hour & 0x20) {
            DS1302_InitStructure->DS1302_IsAM = FALSE;
        }
        else {
            DS1302_InitStructure->DS1302_IsAM = TRUE;
        }
        DS1302_InitStructure->DS1302_Hour = BCD2DEC(hour & 0x1F);
    }
    else {
        DS1302_InitStructure->DS1302_Is24 = TRUE;
        DS1302_InitStructure->DS1302_Hour = BCD2DEC((hour & 0x1F) |
                                                    (hour & 0x20));
    }
}

/******************************************************************************
 * @fn      DS1302_Init
 * @brief   Initializes the DS1302.
 * @param   None.
 * @return  None.
 */
void DS1302_Init() {
    DS1302_InitPin(DS1302_PORT, DS1302_PIN_CLK);
    DS1302_InitPin(DS1302_PORT, DS1302_PIN_DAT);
    DS1302_InitPin(DS1302_PORT, DS1302_PIN_RST);

    DS1302_WR_CLK_0;
    DS1302_WR_DAT_0;
    DS1302_WR_RST_0;
}

/******************************************************************************
 * @fn      DS1302_SetTime
 * @brief   Set data to the DS1302.
 * @param  *DS1302_InitStructure - Data.
 * @return  None.
 */
void DS1302_SetTime(DS1302_InitTypeDef *DS1302_InitStructure) {
    DS1302_InitTypeDef time = DS1302_FormatStructData(DS1302_InitStructure);

    DS1302_WrSingle(DS1302_WR_CTRL, 0x00);
    DS1302_WrSingle(DS1302_WR_YEAR, DEC2BCD(time.DS1302_Year));
    DS1302_WrSingle(DS1302_WR_MON,  DEC2BCD(time.DS1302_Month));
    DS1302_WrSingle(DS1302_WR_DAY,  DEC2BCD(time.DS1302_Day));
    DS1302_WrSingle(DS1302_WR_DATE, DEC2BCD(time.DS1302_Date));
    DS1302_WrSingle(DS1302_WR_HOUR, time.DS1302_Hour);
    DS1302_WrSingle(DS1302_WR_MIN,  DEC2BCD(time.DS1302_Min));
    DS1302_WrSingle(DS1302_WR_SEC,  DEC2BCD(time.DS1302_Sec & 0x7F));
    DS1302_WrSingle(DS1302_WR_CTRL, 0x80);
}

/******************************************************************************
 * @fn      DS1302_SetTimeBurst
 * @brief   Set data to the DS1302.
 * @param  *DS1302_InitStructure - Data.
 * @return  None.
 */
void DS1302_SetTimeBurst(DS1302_InitTypeDef *DS1302_InitStructure) {
    DS1302_InitTypeDef time = DS1302_FormatStructData(DS1302_InitStructure);

    uint8_t times[] = {
        DEC2BCD(time.DS1302_Sec & 0x7F),
        DEC2BCD(time.DS1302_Min),
                time.DS1302_Hour,
        DEC2BCD(time.DS1302_Date),
        DEC2BCD(time.DS1302_Month),
        DEC2BCD(time.DS1302_Day),
        DEC2BCD(time.DS1302_Year),
        0x80
    };

    DS1302_WrSingle(DS1302_WR_CTRL, 0x00);
    DS1302_WrBurst(times);
    DS1302_WrSingle(DS1302_WR_CTRL, 0x80);
}
