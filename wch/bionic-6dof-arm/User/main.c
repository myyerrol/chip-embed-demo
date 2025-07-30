#include <debug.h>
#include <sts3215_comm.h>
#include <sts3215.h>

void SYS_Init() {
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
    SystemCoreClockUpdate();
    Delay_Init();
    Delay_Ms(2000);
}

void DRV_Init() {
    GPIO_InitTypeDef  GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD | RCC_APB2Periph_USART1, ENABLE);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_30MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOD, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOD, &GPIO_InitStructure);

    USART_InitStructure.USART_BaudRate = 1000000;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;

    USART_Init(USART1, &USART_InitStructure);
    USART_Cmd(USART1, ENABLE);
}

uint8_t ID[6];
int16_t Position[6];
uint16_t Speed[6];
uint8_t ACC[6];

void testPickAndPlace() {
    setEnd(0);//SMS_STS舵机为小端存储结构
    ID[0] = 1;//舵机ID1
    ID[1] = 2;//舵机ID2
    ID[2] = 3;//舵机ID2
    ID[3] = 4;//舵机ID2
    ID[4] = 5;//舵机ID2
    ID[5] = 6;//舵机ID2
    Speed[0] = 1000;
    Speed[1] = 1000;
    Speed[2] = 1000;
    Speed[3] = 1000;
    Speed[4] = 1000;
    Speed[5] = 1000;
    ACC[0] = 50;
    ACC[1] = 50;
    ACC[2] = 50;
    ACC[3] = 50;
    ACC[4] = 50;
    ACC[5] = 50;



    Position[0] = 1400;
    Position[1] = 2522;
    Position[2] = 2315;
    Position[3] = 1355;
    Position[4] = 2048;
    Position[5] = 3900;
    Speed[0] = 300;
    Speed[1] = 300;
    Speed[2] = 300;
    Speed[3] = 300;
    Speed[4] = 300;
    SyncWritePosEx(ID, 6, Position, Speed, ACC);
    Delay_Ms(5000);

    Position[5] = 3075;
    Speed[0] = 1000;
    Speed[1] = 1000;
    Speed[2] = 1000;
    Speed[3] = 1000;
    Speed[4] = 1000;
    Speed[5] = 1000;
    SyncWritePosEx(ID, 6, Position, Speed, ACC);
    Delay_Ms(2000);

    Position[3] = 1000;
    SyncWritePosEx(ID, 6, Position, Speed, ACC);
    Delay_Ms(2000);

    Position[1] = 1113;
    Position[2] = 2807;
    Position[3] = 1000;
    SyncWritePosEx(ID, 6, Position, Speed, ACC);
    Delay_Ms(2000);

    Position[0] = 2676;
    Position[3] = 1000;
    SyncWritePosEx(ID, 6, Position, Speed, ACC);
    Delay_Ms(2000);

    Position[1] = 2468;
    Position[2] = 2538;
    Position[3] = 1156;
    SyncWritePosEx(ID, 6, Position, Speed, ACC);
    Delay_Ms(2000);

    Position[5] = 3900;
    Speed[5] = 200;
    SyncWritePosEx(ID, 6, Position, Speed, ACC);
    Delay_Ms(3000);

    Position[3] = 500;
    Speed[5] = 1000;
    SyncWritePosEx(ID, 6, Position, Speed, ACC);
    Delay_Ms(2000);

    Position[0] = 2048;
    Position[1] = 2048;
    Position[2] = 2048;
    Position[4] = 2048;
    Position[5] = 2700;
    Speed[0] = 400;
    Speed[1] = 400;
    Speed[2] = 400;
    Speed[3] = 400;
    Speed[4] = 400;
    SyncWritePosEx(ID, 6, Position, Speed, ACC);
    Delay_Ms(4000);

    Position[5] = 3900;
    SyncWritePosEx(ID, 6, Position, Speed, ACC);
    Delay_Ms(2000);

//   // 位置1
//   Position[0] = 2048;
//   Position[1] = 2048;
//   Position[2] = 2048;
//   Position[3] = 2048;
//   Position[4] = 2048;
//   Position[5] = 2700;
//   SyncWritePosEx(ID, 6, Position, Speed, ACC);
//   Delay_Ms(2000);

//   // 位置2
//   Position[0] = 2048;
//   Position[1] = 2048;
//   Position[2] = 2048;
//   Position[3] = 3072;
//   Position[4] = 2048;
//   Position[5] = 2700;
//   SyncWritePosEx(ID, 6, Position, Speed, ACC);
//   Delay_Ms(2000);

//   // 位置3
//   Position[0] = 2048;
//   Position[1] = 1024;
//   Position[2] = 2048;
//   Position[3] = 3072;
//   Position[4] = 2048;
//   Position[5] = 2700;
//   SyncWritePosEx(ID, 6, Position, Speed, ACC);
//   Delay_Ms(2000);

//   Position[5] = 3900;
//   SyncWritePosEx(ID, 6, Position, Speed, ACC);
//   Delay_Ms(2000);

//   Position[5] = 2700;
//   SyncWritePosEx(ID, 6, Position, Speed, ACC);
//   Delay_Ms(2000);
}

int main(void) {
    SYS_Init();
    DRV_Init();
}
