#include <dht22.h>

/******************************************************************************
 * @fn      DHT22_Init
 *
 * @brief   Initializes DHT22 gpio register.
 *
 * @param   mode - input (floating) or output (push-pull).
 *
 * @return  none
 */
void DHT22_Init(GPIOMode_TypeDef mode) {
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_APB2PeriphClockCmd(DHT22_APB2, ENABLE);
    GPIO_InitStructure.GPIO_Mode  = mode;
    GPIO_InitStructure.GPIO_Pin   = DHT22_PIN;
    GPIO_InitStructure.GPIO_Speed = DHT22_SPD;
    GPIO_Init(DHT22_PORT, &GPIO_InitStructure);
}

/******************************************************************************
 * @fn      DHT22_Start
 *
 * @brief   Start a DHT22 read operation.
 *
 * @param   none
 *
 * @return  Is DHT22 ready.
 */
uint8_t DHT22_START(void) {

    DHT22_Init(GPIO_Mode_Out_PP);
    DHT22_WR_DATA_0;
    Delay_Ms(1);
    DHT22_WR_DATA_1;
    Delay_Us(30);

    DHT22_Init(GPIO_Mode_IN_FLOATING);

    if (!DHT22_RD_DATA) {
        while (!DHT22_RD_DATA);
        while ( DHT22_RD_DATA);
        return 1;
    }
    return 0;
}

/******************************************************************************
 * @fn      DHT22_GetDataOne
 *
 * @brief   Get one byte (8 bit) data from DHT22.
 *
 * @param   none
 *
 * @return  data.
 */
uint8_t DHT22_GetDataOne(void) {
    uint8_t temp = 0;
    for (uint8_t i = 0; i < 8; i++) {
        temp <<= 1;
        while (!DHT22_RD_DATA);
        Delay_Us(26);
        DHT22_RD_DATA ? (temp |= 0x01) : (temp &= ~0x01);
        while (DHT22_RD_DATA);
    }
    return temp;
}

/******************************************************************************
 * @fn      DHT22_GetDataAll
 *
 * @brief   Get all bytes (40 bit) data from DHT22.
 *
 * @param   *temp - temperature data (return by pointer)
 *          *humi - huymidity   data (return by pointer)
 *
 * @return  Is data valid.
 */
uint8_t DHT22_GetDataAll(uint8_t *temp, uint8_t *humi) {
    uint8_t buf[5] = {0};
    if (DHT22_START()) {
        buf[0] = DHT22_GetDataOne();
        buf[1] = DHT22_GetDataOne();
        buf[2] = DHT22_GetDataOne();
        buf[3] = DHT22_GetDataOne();
        buf[4] = DHT22_GetDataOne();
    }
    if (buf[0] + buf[1] + buf[2] + buf[3] == buf[4]) {
        *temp = ((buf[2] << 8) + buf[3]) / 10;
        *humi = ((buf[0] << 8) + buf[1]) / 10;
        return 1;
    }
    else {
        return 0;
    }
}
