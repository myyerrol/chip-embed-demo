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

        OLED_ShowString( 0,  0, "Temp: ", 12);
        OLED_ShowString( 0, 24, "Humi: ", 12);
        OLED_ShowString(42,  0, temp,     12);
        OLED_ShowString(42, 24, humi,     12);
        OLED_Refresh();

        Delay_Ms(2000);
    }

    return 0;
}