#include <ds1302.h>

uint8_t DEC2BCD(uint8_t dec) {
    const uint8_t tens = dec / 10;
    const uint8_t ones = dec % 10;
    return (tens << 4u) | ones;
}

uint8_t BCD2DEC(uint8_t bcd) {
    return (10 * ((bcd & 0xF0) >> 4) + (bcd & 0x0F));
}

/**
 * �޸� dat ���Ϊд��ģʽ
 */
static void dat_pin_write_mode(void) {
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_APB2PeriphClockCmd(DS1302_APB2, ENABLE);
    GPIO_InitStructure.GPIO_Pin = DS1302_PIN_DAT;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = DS1302_SPD;
    GPIO_Init(DS1302_PORT, &GPIO_InitStructure);

    DS1302_WR_DAT_0;
}

/**
 * �޸� dat ���Ϊ��ȡģʽ
 */
static void dat_pin_read_mode(void) {
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_APB2PeriphClockCmd(DS1302_APB2, ENABLE);
    GPIO_InitStructure.GPIO_Pin = DS1302_PIN_DAT;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_InitStructure.GPIO_Speed = DS1302_SPD;
    GPIO_Init(DS1302_PORT, &GPIO_InitStructure);

    DS1302_WR_DAT_0;
}

/**
 * ��ʼ�����
 * @param GPIOx �����
 * @param GPIO_Pin �����������
 */
void ds1302_pin_init(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin) {
    GPIO_InitTypeDef GPIO_InitStruct;

    RCC_APB2PeriphClockCmd(DS1302_APB2, ENABLE);
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStruct.GPIO_Speed = DS1302_SPD;
    GPIO_Init(GPIOx, &GPIO_InitStruct);
}



/**
 * �� DS1302 д������
 * @param data ��Ҫд�������
 */
void ds1302_byte_write(uint8_t data) {
    uint8_t detect;
    for (detect = 0x01; detect != 0; detect <<= 1) {
        if ((data & detect) != 0) { // �� dat �˿�д������
            DS1302_WR_DAT_1;
        } else {
            DS1302_WR_DAT_0;
        }

        DS1302_WR_CLK_1; // ���� clk ʱ��
        DS1302_WR_CLK_0; // ���� clk ʱ��
    }
    DS1302_WR_DAT_1;
}

/**
 * �� DS1302 ��ȡ����
 * @return ��ȡ��������
 */
uint8_t ds1302_byte_read() {
    uint8_t data = 0;
    uint8_t detect;
    for (detect = 0x01; detect != 0; detect <<= 1) {
        if (DS1302_RD_DAT != 0) { // ��ȡ dat �˿�����
            data |= detect;
        }

        DS1302_WR_CLK_1; // ���� clk ʱ��
        DS1302_WR_CLK_0; // ���� clk ʱ��
    }
    return data;
}

/**
 * �� DS1302 д��һ������
 * @param addr �Ĵ�����ַ
 * @param data ��Ҫд�������
 */
void ds1302_single_write(uint8_t addr, uint8_t data) {
    DS1302_WR_RST_0;
    DS1302_WR_CLK_0;

    DS1302_WR_RST_1; // ���� rst ���Կ�ʼʹ�� DS1302

    dat_pin_write_mode(); // �л� dat �˿�Ϊ���ģʽ

    ds1302_byte_write(addr); // ���� DS1302 Ҫд��ļĴ�����ַ
    ds1302_byte_write(data); // ���� DS1302 Ҫд�������

    DS1302_WR_RST_0;
}

/**
 * �� DS1302 ��ȡһ������
 * @param addr �Ĵ�����ַ
 * @return data ��Ҫ��ȡ������
 */
uint8_t ds1302_single_read(uint8_t addr) {
    addr = addr | 0x01; // ����Ĵ�����ַ

    DS1302_WR_RST_0;
    DS1302_WR_CLK_0;

    DS1302_WR_RST_1;// ���� rst ���Կ�ʼʹ�� DS1302

    dat_pin_write_mode();// �л� dat �˿�Ϊ���ģʽ

    ds1302_byte_write(addr);// ���� DS1302 Ҫ��ȡ�ļĴ�����ַ

    dat_pin_read_mode();// �л� dat �˿�Ϊ����ģʽ

    uint8_t data = ds1302_byte_read();// �� DS1302 ��ȡ����
    DS1302_WR_RST_0;

    return data;
}

/**
 * У�鲢ת�� DS1302_InitStructure
 * @param DS1302_InitStructure
 * @return
 */
DS1302_InitTypeDef convert_time_struct(DS1302_InitTypeDef *DS1302_InitStructure) {
    if (DS1302_InitStructure->DS1302_Sec < 0 || DS1302_InitStructure->DS1302_Sec > 59) {
        DS1302_InitStructure->DS1302_Sec = 0;
    }
    if (DS1302_InitStructure->DS1302_Min < 0 || DS1302_InitStructure->DS1302_Min > 59) {
        DS1302_InitStructure->DS1302_Min = 0;
    }
    if (DS1302_InitStructure->DS1302_Is24) {
        if (DS1302_InitStructure->DS1302_Hour < 0 || DS1302_InitStructure->DS1302_Hour > 23) {
            DS1302_InitStructure->DS1302_Hour = 0;
        }
    } else {
        if (DS1302_InitStructure->DS1302_Hour < 1 || DS1302_InitStructure->DS1302_Hour > 12) {
            DS1302_InitStructure->DS1302_Hour = 1;
        }
    }
    if (DS1302_InitStructure->DS1302_Date < 1 || DS1302_InitStructure->DS1302_Date > 31) {
        DS1302_InitStructure->DS1302_Date = 1;
    }
    if (DS1302_InitStructure->DS1302_Day < 1 || DS1302_InitStructure->DS1302_Day > 7) {
        DS1302_InitStructure->DS1302_Day = 1;
    }
    if (DS1302_InitStructure->DS1302_Month < 1 || DS1302_InitStructure->DS1302_Month > 12) {
        DS1302_InitStructure->DS1302_Month = 1;
    }
    if (DS1302_InitStructure->DS1302_Year < 2000 || DS1302_InitStructure->DS1302_Year > 2099) {
        DS1302_InitStructure->DS1302_Year = 00;
    } else {
        DS1302_InitStructure->DS1302_Year -= 2000;
    }

    uint8_t hour = 0;
    if (DS1302_InitStructure->DS1302_Is24) {
        hour |= DEC2BCD(DS1302_InitStructure->DS1302_Hour) & 0x3F;
    } else {
        hour |= 0x80;
        if (!DS1302_InitStructure->DS1302_IsAM) {
            hour |= 0x20;
        }
        hour |= DEC2BCD(DS1302_InitStructure->DS1302_Hour) & 0x1F;
    }
    DS1302_InitStructure->DS1302_Hour = hour;

    return *DS1302_InitStructure;
}

/**
 * ʹ��ͻ��ģʽ�� DS1302 д��ʱ��
 * @param data ʱ������
 */
void ds1302_burst_write(uint8_t *data) {
    DS1302_WR_RST_0;
    DS1302_WR_CLK_0;

    DS1302_WR_RST_1; // ���� rst ���Զ�ȡ DS1302

    dat_pin_write_mode(); // �л� dat �˿�Ϊ���ģʽ

    ds1302_byte_write(DS1302_WR_CLKBURST); // ���� DS1302 Ҫд��ļĴ�����ַ
    for (int i = 0; i < 8; i++) {
        ds1302_byte_write(data[i]); // ���� DS1302 Ҫд�������
    }

    DS1302_WR_RST_0;
}

/**
 * ʹ��ͻ��ģʽ�� DS1302 ��ȡʱ��
 * @param data ����ʱ������� len: 8
 */
void ds1302_burst_read(uint8_t *data) {
    DS1302_WR_RST_0;
    DS1302_WR_CLK_0;

    DS1302_WR_RST_1;// ���� rst ���Զ�ȡ DS1302

    dat_pin_write_mode();// �л� dat �˿�Ϊ���ģʽ

    ds1302_byte_write(DS1302_WR_CLKBURST | 0x01);// ���� DS1302 Ҫ��ȡ�ļĴ�����ַ

    dat_pin_read_mode();// �л� dat �˿�Ϊ����ģʽ
    for (int i = 0; i < 8; i++) {
        data[i] = ds1302_byte_read();// �� DS1302 ��ȡ����
    }
    DS1302_WR_RST_0;
}

















/**
 * ��ȡ DS1302 ʱ��
 * @deprecated ż������������ܻ�������
 * @param DS1302_InitStructure ʱ��ṹ
 */
void DS1302_GetTime(DS1302_InitTypeDef *DS1302_InitStructure) {
    uint8_t tmp;

    tmp = ds1302_single_read(DS1302_WR_YEAR);
    DS1302_InitStructure->DS1302_Year = BCD2DEC(tmp) + 2000;
    tmp = ds1302_single_read(DS1302_WR_MON);
    DS1302_InitStructure->DS1302_Month = BCD2DEC(tmp);
    tmp = ds1302_single_read(DS1302_WR_DAY);
    DS1302_InitStructure->DS1302_Day = BCD2DEC(tmp);
    tmp = ds1302_single_read(DS1302_WR_DATE);
    DS1302_InitStructure->DS1302_Date = BCD2DEC(tmp);
    uint8_t hour = ds1302_single_read(DS1302_WR_HOUR);
    tmp = ds1302_single_read(DS1302_WR_MIN);
    DS1302_InitStructure->DS1302_Min = BCD2DEC(tmp);
    tmp = ds1302_single_read(DS1302_WR_SEC) & 0x7f;
    DS1302_InitStructure->DS1302_Sec = BCD2DEC(tmp);
    DS1302_InitStructure->DS1302_IsAM = FALSE;

    if (hour & 0x80) {
        DS1302_InitStructure->DS1302_Is24 = FALSE;
        if (hour & 0x20) {
            DS1302_InitStructure->DS1302_IsAM = FALSE;
        } else {
            DS1302_InitStructure->DS1302_IsAM = TRUE;
        }
        DS1302_InitStructure->DS1302_Hour = BCD2DEC(hour & 0x1F);
    } else {
        DS1302_InitStructure->DS1302_Is24 = TRUE;
        DS1302_InitStructure->DS1302_Hour = BCD2DEC((hour & 0x1F) | (hour & 0x20));
    }

#ifdef DS1302_DEBUG
        printf("DS1302_Year: %d ", DS1302_InitStructure->DS1302_Year);
        printf("DS1302_Month: %d ", DS1302_InitStructure->DS1302_Month);
        printf("DS1302_Date: %d ", DS1302_InitStructure->DS1302_Date);
        printf("DS1302_Day: %d ", DS1302_InitStructure->DS1302_Day);
        printf("DS1302_Is24: %d ", DS1302_InitStructure->DS1302_Is24);
        printf("isAm: %d ", DS1302_InitStructure->DS1302_IsAM);
        printf("hour: %d ", DS1302_InitStructure->DS1302_Hour);
        printf("min: %d ", DS1302_InitStructure->DS1302_Min);
        printf("sec: %d ", DS1302_InitStructure->DS1302_Sec);
        printf("\n");
#endif
}


/**
 * ��ȡ DS1302 ʱ�� [�Ƽ�]
 * @param DS1302_InitStructure ʱ��ṹ
 */
void DS1302_GetTimeBurst(DS1302_InitTypeDef *DS1302_InitStructure) {
    uint8_t times[8];

    ds1302_burst_read(times);

    DS1302_InitStructure->DS1302_Year = BCD2DEC(times[6]) + 2000;
    DS1302_InitStructure->DS1302_Month = BCD2DEC(times[4]);
    DS1302_InitStructure->DS1302_Day = BCD2DEC(times[5]);
    DS1302_InitStructure->DS1302_Date = BCD2DEC(times[3]);
    uint8_t hour = times[2];
    DS1302_InitStructure->DS1302_Min = BCD2DEC(times[1]);
    DS1302_InitStructure->DS1302_Sec = BCD2DEC(times[0] & 0x7f);
    DS1302_InitStructure->DS1302_IsAM = FALSE;

    if (hour & 0x80) {
        DS1302_InitStructure->DS1302_Is24 = FALSE;
        if (hour & 0x20) {
            DS1302_InitStructure->DS1302_IsAM = FALSE;
        } else {
            DS1302_InitStructure->DS1302_IsAM = TRUE;
        }
        DS1302_InitStructure->DS1302_Hour = BCD2DEC(hour & 0x1F);
    } else {
        DS1302_InitStructure->DS1302_Is24 = TRUE;
        DS1302_InitStructure->DS1302_Hour = BCD2DEC((hour & 0x1F) | (hour & 0x20));
    }
}



/**
 * ��ʼ�� DS1302 ������
 */
void DS1302_Init() {
    /*Configure GPIO pins : PBPin PBPin PBPin */
    ds1302_pin_init(DS1302_PORT, DS1302_PIN_CLK);
    ds1302_pin_init(DS1302_PORT, DS1302_PIN_DAT);
    ds1302_pin_init(DS1302_PORT, DS1302_PIN_RST);

    DS1302_WR_CLK_0;
    DS1302_WR_DAT_0;
    DS1302_WR_RST_0;

//    uint8_t sec = ds1302_single_read(DS1302_WR_SEC);
//    if ((sec & 0x80) != 0) {
//        ds1302_single_write(DS1302_WR_CTRL, 0x00); // �������
//        // todo ��ʼ��ʱ��
//    }
}




/**
 * ���� DS1302 ʱ��
 * @deprecated ż������������ܻ�������
 * @param DS1302_InitStructure ʱ��ṹ
 */
void DS1302_SetTime(DS1302_InitTypeDef *DS1302_InitStructure) {
    DS1302_InitTypeDef time = convert_time_struct(DS1302_InitStructure);

    ds1302_single_write(DS1302_WR_CTRL, 0x00); // ��� DS1302 д�뱣��

    ds1302_single_write(DS1302_WR_YEAR, DEC2BCD(time.DS1302_Year));
    ds1302_single_write(DS1302_WR_MON, DEC2BCD(time.DS1302_Month));
    ds1302_single_write(DS1302_WR_DAY, DEC2BCD(time.DS1302_Day));
    ds1302_single_write(DS1302_WR_DATE, DEC2BCD(time.DS1302_Date));
    ds1302_single_write(DS1302_WR_HOUR, time.DS1302_Hour);
    ds1302_single_write(DS1302_WR_MIN, DEC2BCD(time.DS1302_Min));
    ds1302_single_write(DS1302_WR_SEC, DEC2BCD(time.DS1302_Sec & 0x7f));

    ds1302_single_write(DS1302_WR_CTRL, 0x80); // ���� DS1302 д�뱣��
}

/**
 * ���� DS1302 ʱ�� [�Ƽ�]
 * @param DS1302_InitStructure ʱ��ṹ
 */
void DS1302_SetTimeBurst(DS1302_InitTypeDef *DS1302_InitStructure) {
    DS1302_InitTypeDef time = convert_time_struct(DS1302_InitStructure);

    uint8_t times[] = {
            DEC2BCD(time.DS1302_Sec & 0x7f),
            DEC2BCD(time.DS1302_Min),
            time.DS1302_Hour,
            DEC2BCD(time.DS1302_Date),
            DEC2BCD(time.DS1302_Month),
            DEC2BCD(time.DS1302_Day),
            DEC2BCD(time.DS1302_Year),
            0x80
    };
    ds1302_single_write(DS1302_WR_CTRL, 0x00); // ��� DS1302 д�뱣��
    ds1302_burst_write(times);
    ds1302_single_write(DS1302_WR_CTRL, 0x80); // ���� DS1302 д�뱣��
}