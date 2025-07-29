/*
 * SCServo.c
 * ���ض��Ӳ���ӿڲ����
 * ����: 2024.12.2
 * ����: txl
 */
#include <stdint.h>
#include <ch32v00x.h>

uint8_t wBuf[128];
uint8_t wLen = 0;
uint32_t IOTimeOut = 5000;//ͨ�ų�ʱ

//UART �������ݽӿ�
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

//UART �������ݽӿ�
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

//���ջ�����ˢ��
void rFlushSCS()
{
	// USART_ClearFlag(USART1, USART_FLAG_RXNE);
	while (USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == SET);
}

//���ͻ�����ˢ��
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
