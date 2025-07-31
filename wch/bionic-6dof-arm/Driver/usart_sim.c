#include <debug.h>
#include <usart_sim.h>


#define Recive2_Byte 19
u8 len2 = 0;
u8 USART2_buf[Recive2_Byte];

enum{
    COM_START_BIT,
    COM_D0_BIT,
    COM_D1_BIT,
    COM_D2_BIT,
    COM_D3_BIT,
    COM_D4_BIT,
    COM_D5_BIT,
    COM_D6_BIT,
    COM_D7_BIT,
    COM_STOP_BIT,
};

u8 recvStat2 = COM_STOP_BIT;
u8 recvData2 = 0;

void IO2_TXD(u8 Data)
{
    u8 i = 0;
    GPIO_WriteBit(GPIOD, GPIO_Pin_0, Bit_RESET);
    Delay_Us(BuadRate2_9600);
    for(i = 0; i < 8; i++)
    {
        if(Data&0x01)
            GPIO_WriteBit(GPIOD, GPIO_Pin_0, Bit_SET);
        else
            GPIO_WriteBit(GPIOD, GPIO_Pin_0, Bit_RESET);

        Delay_Us(BuadRate2_9600);
        Data = Data>>1;
    }
    GPIO_WriteBit(GPIOD, GPIO_Pin_0, Bit_SET);
    Delay_Us(BuadRate2_9600);
}

void USART2_Send(u8 *buf, u8 len2)
{
    u8 t;
    for(t = 0; t < len2; t++)
    {
        IO2_TXD(buf[t]);
    }
}

 void IO2Config(void)
 {
    GPIO_InitTypeDef  GPIO_InitStructure = {0};
    NVIC_InitTypeDef NVIC_InitStructure = {0};
     EXTI_InitTypeDef EXTI_InitStruct = {0};
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO | RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOD, ENABLE);

     //SoftWare Serial TXD
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOD, &GPIO_InitStructure);
    // GPIO_SetBits(GPIOD,GPIO_Pin_0);

    //SoftWare Serial RXD
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOC, &GPIO_InitStructure);

    GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource0);
    EXTI_InitStruct.EXTI_Line = EXTI_Line0;
    EXTI_InitStruct.EXTI_Mode=EXTI_Mode_Interrupt;
    EXTI_InitStruct.EXTI_Trigger=EXTI_Trigger_Falling;
    EXTI_InitStruct.EXTI_LineCmd=ENABLE;
    EXTI_Init(&EXTI_InitStruct);

    NVIC_InitStructure.NVIC_IRQChannel = EXTI7_0_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority =2;
    NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

void TIM5_Int_Init(u16 arr,u16 psc)
{
    NVIC_InitTypeDef NVIC_InitStructure={0};
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure={0};

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE );

    TIM_TimeBaseInitStructure.TIM_Period = arr;
    TIM_TimeBaseInitStructure.TIM_Prescaler = psc;
    TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit( TIM1, &TIM_TimeBaseInitStructure);

    TIM_ClearITPendingBit( TIM1, TIM_IT_Update );

    NVIC_InitStructure.NVIC_IRQChannel =TIM1_UP_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority =0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority =1;
    NVIC_InitStructure.NVIC_IRQChannelCmd =ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    TIM_ITConfig(TIM1, TIM_IT_Update, ENABLE);
}

void EXTI7_0_IRQHandler(void) __attribute__((interrupt("WCH-Interrupt-fast")));
void EXTI7_0_IRQHandler(void)
{
    if(EXTI_GetFlagStatus(EXTI_Line0) != RESET)
    {
        if(GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_0) == 0)
        {
            if(recvStat2 == COM_STOP_BIT)
            {
                recvStat2 = COM_START_BIT;
                TIM_Cmd(TIM1, ENABLE);
            }
        }
        EXTI_ClearITPendingBit(EXTI_Line0);
    }
}

void TIM1_UP_IRQHandler(void) __attribute__((interrupt("WCH-Interrupt-fast")));
void TIM1_UP_IRQHandler(void)
{
    if(TIM_GetFlagStatus(TIM1, TIM_IT_Update) != RESET)
    {
        TIM_ClearITPendingBit(TIM1, TIM_IT_Update);
        recvStat2++;
        if(recvStat2 == COM_STOP_BIT)
        {
            TIM_Cmd(TIM1, DISABLE);
            USART2_buf[len2++] = recvData2;
            printf("len2: %d\n", len2);
            if(len2 > Recive2_Byte-1)
            {
                len2 = 0;
                USART2_Send(USART2_buf,Recive2_Byte);
            }
            return;
        }
        if(GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_0) == 1)
        {
            printf("1\n");
            recvData2 |= (1 << (recvStat2 - 1));
        }else{
            printf("0\n");
            recvData2 &= ~(1 << (recvStat2 - 1));
        }
  }
}
