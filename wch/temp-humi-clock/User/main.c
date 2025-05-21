#include <core.h>
#include <dht22.h>
#include <oled.h>
#include <oled_bmp.h>
#include <string.h>

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
        char   *strs = "2025-06-04";
        OLED_ShowString(0, 0, strs, size);
        OLED_ShowChinese(OLED_DISPLAY_W - size * 3, 0, 3, size);
        OLED_ShowChinese(OLED_DISPLAY_W - size * 2, 0, 4, size);
        OLED_ShowChinese(OLED_DISPLAY_W - size * 1, 0, 7, size);

        size = 24;
        strs = "23:00:00";
        OLED_ShowString((OLED_DISPLAY_W - strlen(strs) * size / 2) / 2, 16, strs, size);

        size = 12;
        OLED_ShowChinese(0,             (OLED_DISPLAY_H - size - 4), 11,   size);
        OLED_ShowChinese(0 + size * 1,  (OLED_DISPLAY_H - size - 4), 13,   size);
        OLED_ShowString (0 + size * 2,  (OLED_DISPLAY_H - size - 4), temp, size);
        OLED_ShowChinese(74,            (OLED_DISPLAY_H - size - 4), 12,   size);
        OLED_ShowChinese(74 + size * 1, (OLED_DISPLAY_H - size - 4), 13,   size);
        OLED_ShowString (74 + size * 2, (OLED_DISPLAY_H - size - 4), humi, size);

        // size = 12;
        // OLED_ShowChinese((OLED_DISPLAY_W - 4 * size) / 2,
        //                  (OLED_DISPLAY_H - size - 16), 14, size);
        // OLED_ShowChinese((OLED_DISPLAY_W - 4 * size) / 2 + size * 1,
        //                  (OLED_DISPLAY_H - size - 16), 15, size);
        // OLED_ShowChinese((OLED_DISPLAY_W - 4 * size) / 2 + size * 2,
        //                  (OLED_DISPLAY_H - size - 16), 16, size);
        // OLED_ShowChinese((OLED_DISPLAY_W - 4 * size) / 2 + size * 3,
        //                  (OLED_DISPLAY_H - size - 16), 17, size);

        OLED_Refresh();

        Delay_Ms(2000);
    }

    return 0;
}
