#include <dht22.h>

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
    uint8_t data = 0;

    for (uint8_t i = 0; i < 8; i++) {
        data <<= 1;
        // Wait for 50us read data preparation cycle.
        while (!DHT22_RD_DATA);
        // If after a delay of 26-28us, the bus is 0, the data is 0, and the
        // bus is 1, the data is 1.
        Delay_Us(26);
        DHT22_RD_DATA ? (data |= 0x01) : (data &= ~0x01);
        // Wait for data to be pulled low (only for the case where the data is 1)
        while (DHT22_RD_DATA);
    }

    return data;
}

/******************************************************************************
 * @fn      DHT22_GetDataAll
 *
 * @brief   Get all bytes (40 bit) data from DHT22.
 *
 * @param   *temp_int - temperature integer data
 *          *temp_dec - temperature integer data
 *          *humi_int - huymidity   decimal data
 *          *humi_dec - huymidity   decimal data
 *          *temp     - temperature string
 *          *humi     - huymidity   string
 *
 * @return  Is data valid.
 */
uint8_t DHT22_GetDataAll(uint8_t *temp_sig,
                         uint8_t *temp_int, uint8_t *temp_dec,
                         uint8_t *humi_int, uint8_t *humi_dec,
                         char *temp,
                         char *humi) {
    uint8_t buf[5] = {0};

    if (DHT22_START()) {
        // High 8bit data of huymidity data.
        buf[0] = DHT22_GetDataOne();
        // Low  8bit data of huymidity data.
        buf[1] = DHT22_GetDataOne();
        // High 8bit data of temperature data.
        buf[2] = DHT22_GetDataOne();
        // Low  8bit data of temperature data.
        buf[3] = DHT22_GetDataOne();
        // Checksum.
        buf[4] = DHT22_GetDataOne();
    }

    // Calculate temperature and huymidity.
    if (buf[0] + buf[1] + buf[2] + buf[3] == buf[4]) {
        *temp_sig = '+';
        if (buf[2] & 0x80) {
            buf[2] &= ~0x80;
            *temp_sig = '-';
        }
        *temp_int = ((buf[2] << 8) + buf[3]) / 10;
        *temp_dec = ((buf[2] << 8) + buf[3]) % 10;
        *humi_int = ((buf[0] << 8) + buf[1]) / 10;
        *humi_dec = ((buf[0] << 8) + buf[1]) % 10;

        sprintf(temp, "%c%d.%d¡ãC", *temp_sig, *temp_int, *temp_dec);
        sprintf(humi, " %d.%d%%RH",           *humi_int, *humi_dec);

#ifdef DHT22_DEBUG
        printf("Temp: %s  Humi: %s\n", temp, humi);
#endif
        return 1;
    }
    else {
        return 0;
    }
}

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
    // Master sends a read request by pulling the bus low for 0.8ms-20ms
    // (typical is 1ms).
    DHT22_Init(GPIO_Mode_Out_PP);
    DHT22_WR_DATA_0;
    Delay_Ms(1);
    // Master releases the bus by pulling the bus high for 25us-45us
    // (typical is 30us).
    DHT22_WR_DATA_1;
    Delay_Us(30);

    // Slave performs a read response operation.
    DHT22_Init(GPIO_Mode_IN_FLOATING);
    if (!DHT22_RD_DATA) {
        // Slave sends a low level of 80us as a response signal.
        while (!DHT22_RD_DATA);
        // Slave sends a high level of 80us to notify that it is ready to
        // receive data.
        while ( DHT22_RD_DATA);
        return 1;
    }

    return 0;
}
