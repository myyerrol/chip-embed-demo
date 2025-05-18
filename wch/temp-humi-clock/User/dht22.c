#include <dht22.h>

void DHT22_GPIO_INIT(GPIOMode_TypeDef mode) {
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);
    GPIO_InitStructure.GPIO_Mode  = mode;
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_0;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_30MHz;
    GPIO_Init(GPIOD, &GPIO_InitStructure);
}

uint8_t DHT22_START(void) {
    DHT22_GPIO_INIT(GPIO_Mode_Out_PP); // output
    WR_GPIO_DATA_0;
    Delay_Ms(1);
    WR_GPIO_DATA_1;
    Delay_Us(30);

    DHT22_GPIO_INIT(GPIO_Mode_IN_FLOATING); // input

    uint8_t bit = RD_GPIO_DATA;

    if (!bit) {
        while (!RD_GPIO_DATA);
        while ( RD_GPIO_DATA);
        return 1;
    }
    return 0;
}

uint8_t DHT22_Get_Byte_Data(void) {
    uint8_t temp = 0;
    for (uint8_t i = 0; i < 8; i++) {
        temp <<= 1;
        while (!RD_GPIO_DATA);
        Delay_Us(28);
        RD_GPIO_DATA ? (temp |= 0x01) : (temp &= ~0x01);
        while (RD_GPIO_DATA);
    }
    return temp;
}

uint8_t DHT22_Get_Temp_Humi_Data(uint8_t buf[]) {
    if (DHT22_START()) {
        buf[0] = DHT22_Get_Byte_Data();
        buf[1] = DHT22_Get_Byte_Data();
        buf[2] = DHT22_Get_Byte_Data();
        buf[3] = DHT22_Get_Byte_Data();
        buf[4] = DHT22_Get_Byte_Data();
    }
    return (buf[0] + buf[1] + buf[2] + buf[3] == buf[4]) ? 1 : 0;
}
