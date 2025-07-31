#ifndef USART_SIM_H
#define USART_SIM_H

#include <stdint.h>


#define BuadRate2_9600 104

// void     USARTSIM_Init(void);
// void     USARTSIM_SendData(uint16_t data);
// uint16_t USARTSIM_ReceiveData(void);
void IO2_TXD(u8 Data);
void USART2_Send(u8 *buf, u8 len);
void IO2Config(void);
void TIM5_Int_Init(u16 arr,u16 psc);

#endif
