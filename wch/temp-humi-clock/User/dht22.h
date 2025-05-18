#ifndef DHT22
#define DHT22

#include <debug.h>

#define RD_GPIO_DATA   GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_0)
#define WR_GPIO_DATA_0 GPIO_ResetBits       (GPIOD, GPIO_Pin_0)
#define WR_GPIO_DATA_1 GPIO_SetBits         (GPIOD, GPIO_Pin_0)

void DHT22_GPIO_INIT(GPIOMode_TypeDef mode);
uint8_t DHT22_START(void);
uint8_t DHT22_Get_Byte_Data(void);
uint8_t DHT22_Get_Temp_Humi_Data(uint8_t buf[]);

#endif
