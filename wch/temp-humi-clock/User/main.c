#include <string.h>

#include <dht22.h>
#include <ds1302.h>
#include <ssd1309.h>

void MAIN_InitSys() {
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
    SystemCoreClockUpdate();
    Delay_Init();
    USART_Printf_Init(115200);
    Delay_Ms(2000);
}

void MAIN_InitMod() {
    DHT22_Init(GPIO_Mode_Out_PP);
    DS1302_Init();
    SSD1309_Init();
}

/******************************************************************************
 * @fn      main
 * @brief   Main program.
 * @return  None.
 */
int main(void) {
    MAIN_InitSys();
    MAIN_InitMod();

    uint8_t size = 12;
    char    date[10];
    uint8_t week_map[10] = {0, 5, 6, 7, 8, 9, 10, 2};
    uint8_t week         = week_map[1];
    char    time[10];

    uint8_t temp_sig = 0;
    uint8_t temp_int = 0;
    uint8_t temp_dec = 0;
    uint8_t humi_int = 0;
    uint8_t humi_dec = 0;
    char temp[10];
    char humi[10];

#ifdef DS1302_SETTIME
    DS1302_InitTypeDef time_struct = {
        2025, 6, 9, 1, 1, 0, 8, 52, 30
    };
    DS1302_SetTime(&time_struct);
#else
    DS1302_InitTypeDef time_struct = {0};
#endif

    while (1) {
        DS1302_GetTime(&time_struct);
        sprintf(date, "%d-%02d-%02d",
                time_struct.DS1302_Year,
                time_struct.DS1302_Month,
                time_struct.DS1302_Date);
        week = (time_struct.DS1302_Day >= 1 && time_struct.DS1302_Day <= 7) ?
                week_map[time_struct.DS1302_Day] :
                week_map[1];
        sprintf(time, "%02d:%02d:%02d",
                time_struct.DS1302_Hour,
                time_struct.DS1302_Min,
                time_struct.DS1302_Sec);

        size = 12;
        SSD1309_ShowString(0, 0, date, size);
        SSD1309_ShowChinese(SSD1309_DISPLAY_W - size * 3, 0, 3,    size);
        SSD1309_ShowChinese(SSD1309_DISPLAY_W - size * 2, 0, 4,    size);
        SSD1309_ShowChinese(SSD1309_DISPLAY_W - size * 1, 0, week, size);

        size = 24;
        SSD1309_ShowString((SSD1309_DISPLAY_W - strlen(time) * size / 2) / 2,
                         16,
                         time,
                         size);

        DHT22_GetDataAll(&temp_sig,
                         &temp_int, &temp_dec,
                         &humi_int, &humi_dec,
                          temp,
                          humi);
        size = 12;
        SSD1309_ShowChinese(0,             (SSD1309_DISPLAY_H - size - 4), 11,   size);
        SSD1309_ShowChinese(0 + size * 1,  (SSD1309_DISPLAY_H - size - 4), 13,   size);
        SSD1309_ShowString (0 + size * 2,  (SSD1309_DISPLAY_H - size - 4), temp, size);
        SSD1309_ShowChinese(74,            (SSD1309_DISPLAY_H - size - 4), 12,   size);
        SSD1309_ShowChinese(74 + size * 1, (SSD1309_DISPLAY_H - size - 4), 13,   size);
        SSD1309_ShowString (74 + size * 2, (SSD1309_DISPLAY_H - size - 4), humi, size);

        SSD1309_Refresh();

        Delay_Ms(1000);
    }

    return 0;
}
