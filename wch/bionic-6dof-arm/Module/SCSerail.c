/*
 * SCServo.c
 * 飞特舵机硬件接口层程序
 * 日期: 2024.12.2
 * 作者: txl
 */
#include <stdint.h>
#include <ch32v00x.h>

uint8_t wBuf[128];
uint8_t wLen = 0;
uint32_t IOTimeOut = 5000;//通信超时

//UART 接收数据接口
int readSCS(unsigned char *nDat, int nLen)
{
	int Size = 0;
	int ComData;
	uint32_t t_user = 0;
	while (1) {
		while(USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == RESET);
		ComData = USART_ReceiveData(USART1);
		if(ComData!=-1){
			if(nDat){
				nDat[Size] = ComData;
			}
			Size++;
			t_user = 0;
		}
		if(Size>=nLen){
			break;
		}
		t_user++;
		if(t_user>IOTimeOut){
			break;
		}
	}
	return Size;
}

//UART 发送数据接口
int writeSCS(unsigned char *nDat, int nLen)
{
	while(nLen--){
		if(wLen<sizeof(wBuf)){
			wBuf[wLen] = *nDat;
			wLen++;
			nDat++;
		}
	}
	return wLen;
}

int writeByteSCS(unsigned char bDat)
{
	if(wLen<sizeof(wBuf)){
		wBuf[wLen] = bDat;
		wLen++;
	}
	return wLen;
}

//接收缓冲区刷新
void rFlushSCS()
{
	// USART_ClearFlag(USART1, USART_FLAG_RXNE);
	while (USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == SET);
}

//发送缓冲区刷新
void wFlushSCS()
{
	if (wLen) {
		for (uint8_t i = 0; i < wLen; i++) {
			USART_SendData(USART1, wBuf[i]);
			while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
		}
		while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);
		wLen = 0;
	}
}
