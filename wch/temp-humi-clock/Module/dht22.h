#ifndef DHT22_H
#define DHT22_H

#include <debug.h>

#define DHT22_DEBUG

#define DHT22_PIN                             GPIO_Pin_4
#define DHT22_SPD                             GPIO_Speed_30MHz
#define DHT22_PORT                            GPIOD
#define DHT22_APB2                            RCC_APB2Periph_GPIOD
#define DHT22_RD_DATA   GPIO_ReadInputDataBit(DHT22_PORT, DHT22_PIN)
#define DHT22_WR_DATA_0 GPIO_ResetBits       (DHT22_PORT, DHT22_PIN)
#define DHT22_WR_DATA_1 GPIO_SetBits         (DHT22_PORT, DHT22_PIN)

void    DHT22_Init(GPIOMode_TypeDef mode);
uint8_t DHT22_START(void);
uint8_t DHT22_GetDataOne(void);
uint8_t DHT22_GetDataAll(uint8_t *temp_sig,
                         uint8_t *temp_int, uint8_t *temp_dec,
                         uint8_t *humi_int, uint8_t *humi_dec);

#endif
