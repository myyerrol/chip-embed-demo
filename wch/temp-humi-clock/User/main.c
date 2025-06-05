#include <core.h>
#include <dht22.h>
#include <oled.h>
#include <oled_bmp.h>

/******************************************************************************
 * @fn      main
 *
 * @brief   Main program.
 *
 * @return  none
 */
int main(void) {
    CORE_Init();
    OLED_Init();

    uint8_t temp_sig = 0;
    uint8_t temp_int = 0;
    uint8_t temp_dec = 0;
    uint8_t humi_int = 0;
    uint8_t humi_dec = 0;
    char temp[20];
    char humi[20];

    while(1) {
        DHT22_GetDataAll(&temp_sig,
                         &temp_int, &temp_dec,
                         &humi_int, &humi_dec,
                          temp,
                          humi);
        uint8_t size = 12;
        OLED_ShowString(0, 0, "2025-06-04", size);
        // 116 = 128 - 12
        OLED_ShowChinese(116 - 12 * 2, 0, 3, size);
        OLED_ShowChinese(116 - 12, 0, 4, size);
        OLED_ShowChinese(116, 0, 7, size);

        size = 24;
        OLED_ShowString((128 - 8 * 12) / 2, 10, "23:00:00", size);

        size = 12;
        OLED_ShowChinese(0,             (64 - size - 4), 11, size);
        OLED_ShowChinese(0 + size * 1,  (64 - size - 4), 13, size);
        OLED_ShowString (0 + size * 2,  (64 - size - 4), temp, size);
        OLED_ShowChinese(74,            (64 - size - 4), 12, size);
        OLED_ShowChinese(74 + size * 1, (64 - size - 4), 13, size);
        OLED_ShowString (74 + size * 2, (64 - size - 4), humi, size);

        size = 12;
        OLED_ShowChinese((128 - 4 * size) / 2,            (64 - size - 16), 14, size);
        OLED_ShowChinese((128 - 4 * size) / 2 + size * 1, (64 - size - 16), 15, size);
        OLED_ShowChinese((128 - 4 * size) / 2 + size * 2, (64 - size - 16), 16, size);
        OLED_ShowChinese((128 - 4 * size) / 2 + size * 3, (64 - size - 16), 17, size);

        OLED_Refresh();

        Delay_Ms(2000);
    }

    return 0;
}