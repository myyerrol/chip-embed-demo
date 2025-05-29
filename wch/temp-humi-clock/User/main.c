/********************************** (C) COPYRIGHT *******************************
 * File Name          : main.c
 * Author             : WCH
 * Version            : V1.0.0
 * Date               : 2023/12/25
 * Description        : Main program body.
 *********************************************************************************
 * Copyright (c) 2021 Nanjing Qinheng Microelectronics Co., Ltd.
 * Attention: This software (modified or not) and binary are used for
 * microcontroller manufactured by Nanjing Qinheng Microelectronics.
 *******************************************************************************/

/*
 *@Note
 *Multiprocessor communication mode routine:
 *Master:USART1_Tx(PD5)\USART1_Rx(PD6).
 *This routine demonstrates that USART1 receives the data sent by CH341 and inverts
 *it and sends it (baud rate 115200).
 *
 *Hardware connection:PD5 -- Rx
 *                     PD6 -- Tx
 *
 */

// #include "debug.h"


// /* Global define */


// /* Global Variable */
// vu8 val;

// /*********************************************************************
//  * @fn      USARTx_CFG
//  *
//  * @brief   Initializes the USART2 & USART3 peripheral.
//  *
//  * @return  none
//  */
// void USARTx_CFG(void)
// {
//     GPIO_InitTypeDef  GPIO_InitStructure = {0};
//     USART_InitTypeDef USART_InitStructure = {0};

//     RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD | RCC_APB2Periph_USART1, ENABLE);

//     /* USART1 TX-->D.5   RX-->D.6 */
//     GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
//     GPIO_InitStructure.GPIO_Speed = GPIO_Speed_30MHz;
//     GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
//     GPIO_Init(GPIOD, &GPIO_InitStructure);
//     GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
//     GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
//     GPIO_Init(GPIOD, &GPIO_InitStructure);

//     USART_InitStructure.USART_BaudRate = 115200;
//     USART_InitStructure.USART_WordLength = USART_WordLength_8b;
//     USART_InitStructure.USART_StopBits = USART_StopBits_1;
//     USART_InitStructure.USART_Parity = USART_Parity_No;
//     USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
//     USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;

//     USART_Init(USART1, &USART_InitStructure);
//     USART_Cmd(USART1, ENABLE);
// }

// /*********************************************************************
//  * @fn      main
//  *
//  * @brief   Main program.
//  *
//  * @return  none
//  */
// int main(void)
// {
//     NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
//     SystemCoreClockUpdate();
//     Delay_Init();
// #if (SDI_PRINT == SDI_PR_OPEN)
//     SDI_Printf_Enable();
// #else
//     USART_Printf_Init(115200);
// #endif
//     printf("SystemClk:%d\r\n",SystemCoreClock);
//     printf( "ChipID:%08x\r\n", DBGMCU_GetCHIPID() );

//     USARTx_CFG();

//     while(1)
//     {

//         while(USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == RESET)
//         {
//             /* waiting for receiving finish */
//         }
//         val = (USART_ReceiveData(USART1));
//         USART_SendData(USART1, ~val);
//         while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET)
//         {
//             /* waiting for sending finish */
//         }
//     }
// }






#include <dht22.h>
#include <oled.h>
#include <oled_bmp.h>

int main(void) {
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
    SystemCoreClockUpdate();
    Delay_Init();
    USART_Printf_Init(115200);

    Delay_Ms(2000);

    // uint8_t temp;
    // uint8_t humi;
    // while (1) {
    //     if (DHT22_GetDataAll(&temp, &humi)) {
    //         printf("Temp: %d°C   Humi: %d%%RH\n", temp, humi);
    //     }
    //     Delay_Ms(2000);
    // }




// u8 t;
// 	OLED_SPI_Init();
// 	OLED_Init();
// 	OLED_ColorTurn(0);         //0正常显示，1 反色显示
//   OLED_DisplayTurn(0);       //0正常显示 1 屏幕翻转显示
// 	// OLED_DrawPoint(0,0);
// 	OLED_DrawLine(20,0,50,60);
// 	// OLED_DrawCircle(64,32,20);
// 	OLED_Refresh();
// 	t=' ';
// 	while(1)
// 	{
// 		// OLED_ShowPicture(0,0,128,8,BMP1);
// 		// Delay_Ms(500);
// 		// OLED_Clear();
// 	// 	OLED_ShowChinese(0,0,0,16);//中
// 	// 	OLED_ShowChinese(18,0,1,16);//景
// 	// 	OLED_ShowChinese(36,0,2,16);//园
// 	// 	OLED_ShowChinese(54,0,3,16);//电
// 	// 	OLED_ShowChinese(72,0,4,16);//子
// 	// 	OLED_ShowChinese(90,0,5,16);//科
// 	// 	OLED_ShowChinese(108,0,6,16);//技
// 	// 	OLED_ShowString(8,16,"ZHONGJINGYUAN",16);
// 	// 	OLED_ShowString(20,32,"2014/05/01",16);
// 	// 	OLED_ShowString(0,48,"ASCII:",16);
// 	// 	OLED_ShowString(63,48,"CODE:",16);
// 		// OLED_ShowChar(48,48,t,16);//显示ASCII字符
// 	// 	t++;
// 	// 	if(t>'~')t=' ';
// 	// 	OLED_ShowNum(103,48,t,3,16);
// 		// OLED_Refresh();
// 		// Delay_Ms(500);
// 	// 	OLED_Clear();
// 	// 	OLED_ShowChinese(0,0,0,16);  //16*16 中
// 	//   OLED_ShowChinese(16,0,0,24); //24*24 中
// 	// 	OLED_ShowChinese(24,20,0,32);//32*32 中
// 	//   OLED_ShowChinese(64,0,0,64); //64*64 中
// 	// 	OLED_Refresh();
// 	//   Delay_Ms(500);
//   	// OLED_Clear();
// 	// 	OLED_ShowString(0,0,"ABC",12);//6*12 “ABC”
// 	//   OLED_ShowString(0,12,"ABC",16);//8*16 “ABC”
// 	// 	OLED_ShowString(0,28,"ABC",24);//12*24 “ABC”
// 	//   OLED_Refresh();
// 	// 	Delay_Ms(500);
// 	// 	OLED_ScrollDisplay(11,4);
// 	}





	u8 t;
	OLED_Init();
	// LED_Init();
	OLED_ColorTurn(0);//0 normal display 1 negative display
  OLED_DisplayTurn(0);//0normal display 1 Flip Display
//	OLED_DrawPoint(0,0);
//	OLED_DrawLine(20,0,50,60);
//	OLED_DrawCircle(64,32,20);
	OLED_Refresh();
	t=' ';
	// LED0=0;

    uint8_t temp;
    uint8_t humi;
	while(1)
	{

    if (DHT22_GetDataAll(&temp, &humi)) {
        printf("Temp: %d°C   Humi: %d%%RH\n", temp, humi);
    }

    OLED_ShowString(0,0,"Temp:",16);
    OLED_ShowString(0,24,"Humi:",16);
    OLED_ShowNum(50, 0, temp, 2, 16);
    OLED_ShowNum(50, 24, humi, 2, 16);
    OLED_ShowString(72, 0, "C", 16);
    OLED_ShowString(72, 24, "%RH", 16);


		// OLED_ShowPicture(0,0,128,8,BMP1);
		// Delay_Ms(500);
		// OLED_Clear();

		// OLED_ShowChinese(0,0,0,16);//深
		// OLED_ShowChinese(16,0,1,16);//圳
		// OLED_ShowChinese(32,0,2,16);//旭
		// OLED_ShowChinese(48,0,3,16);//日
		// OLED_ShowChinese(64,0,4,16);//东
		// OLED_ShowChinese(80,0,5,16);//方
		// OLED_ShowChinese(96,0,6,16);//科
		// OLED_ShowChinese(112,0,7,16);//技



		// OLED_ShowString(8,16,"BUYDISPLAY.COM",16);
		// OLED_ShowString(20,32,"EASTRISING",16);
		// OLED_ShowString(0,48,"ASCII:",16);
		// OLED_ShowString(63,48,"CODE:",16);
		// OLED_ShowChar(48,48,t,16);//
	// 	t++;
	// 	if(t>'~')t=' ';
	// 	OLED_ShowNum(103,48,t,3,16);
		OLED_Refresh();
		// Delay_Ms(500);
            Delay_Ms(2000);
	// 	OLED_Clear();
	// 	OLED_ShowChinese(0,0,0,16);  //16*16 中
	//   OLED_ShowChinese(16,0,0,24); //24*24 中
	// 	OLED_ShowChinese(24,20,0,32);//32*32 中
	//   OLED_ShowChinese(64,0,0,64); //64*64 中
	// 	OLED_Refresh();
	//   Delay_Ms(500);
  	// OLED_Clear();
	// 	OLED_ShowString(0,0,"ABC",12);//6*12 “ABC”
	//   OLED_ShowString(0,12,"ABC",16);//8*16 “ABC”
	// 	OLED_ShowString(0,28,"ABC",24);//12*24 “ABC”
	//   OLED_Refresh();
	// 	Delay_Ms(500);
	// 	OLED_ScrollDisplay(11,4);
	}




    return 0;
}