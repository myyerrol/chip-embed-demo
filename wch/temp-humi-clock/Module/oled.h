#ifndef OLED_H
#define OLED_H

#include <debug.h>

#define OLED_MODE_SPI
#define OLED_DEBUG

// SPI
#ifdef OLED_MODE_SPI
#define OLED_PIN_SCK  GPIO_Pin_5 // PC5
#define OLED_PIN_MOSI GPIO_Pin_6 // PC6
#define OLED_PIN_RES  GPIO_Pin_1 // PC1
#define OLED_PIN_DC   GPIO_Pin_2 // PC2
#define OLED_PIN_CS   GPIO_Pin_3 // PC3
#else
// I2C
#define OLED_PIN_SCL  GPIO_Pin_2 // PC2
#define OLED_PIN_SDA  GPIO_Pin_1 // PC1
#endif

#define OLED_SPD   GPIO_Speed_50MHz
#define OLED_PORT  GPIOC
#define OLED_APB2  RCC_APB2Periph_GPIOC

#ifdef OLED_MODE_SPI
#define OLED_WR_SCK_1  GPIO_SetBits  (OLED_PORT, OLED_PIN_SCK )
#define OLED_WR_SCK_0  GPIO_ResetBits(OLED_PORT, OLED_PIN_SCK )
#define OLED_WR_MOSI_1 GPIO_SetBits  (OLED_PORT, OLED_PIN_MOSI)
#define OLED_WR_MOSI_0 GPIO_ResetBits(OLED_PORT, OLED_PIN_MOSI)
#define OLED_WR_RES_1  GPIO_SetBits  (OLED_PORT, OLED_PIN_RES )
#define OLED_WR_RES_0  GPIO_ResetBits(OLED_PORT, OLED_PIN_RES )
#define OLED_WR_DC_1   GPIO_SetBits  (OLED_PORT, OLED_PIN_DC  )
#define OLED_WR_DC_0   GPIO_ResetBits(OLED_PORT, OLED_PIN_DC  )
#define OLED_WR_CS_1   GPIO_SetBits  (OLED_PORT, OLED_PIN_CS  )
#define OLED_WR_CS_0   GPIO_ResetBits(OLED_PORT, OLED_PIN_CS  )
#else
#endif

#define OLED_CMDS           0
#define OLED_DATA           1
#define OLED_COLOR_NORMAL   0
#define OLED_COLOR_INVERT   1
#define OLED_DISPLAY_NORMAL 0
#define OLED_DISPLAY_INVERT 1

#define OLED_DISPLAY_W 128
#define OLED_DISPLAY_H 64

#define u8  uint8_t
#define u32 uint32_t

#define OLED_POW(m, n) ({        \
    u32 res = 1;                 \
    for (u8 i = n; i > 0; i--) { \
        res *=  m;               \
    }                            \
    res;                         \
})

void OLED_DrawPoint(u8 x, u8 y);
void OLED_DrawLine(u8 x1, u8 y1, u8 x2, u8 y2);
void OLED_DrawCircle(u8 x, u8 y, u8 r);

void OLED_ShowChar(u8 x, u8 y, u8 chr, u8 size1);
void OLED_ShowString(u8 x, u8 y, char *chr, u8 size1);
void OLED_ShowNum(u8 x, u8 y, u32 num, u8 len, u8 size1);
void OLED_ShowChinese(u8 x,u8 y,u8 num,u8 size1);
void OLED_ShowPicture(u8 x0,u8 y0,u8 x1,u8 y1,u8 bmp[]);

void OLED_Clear(void);
void OLED_ClearPoint(u8 x, u8 y);
void OLED_Init(void);
void OLED_Refresh(void);
void OLED_SetStartLocation(u8 x,u8 y);
void OLED_TurnOn(void);
void OLED_TurnOff(void);
void OLED_UpdateColor(u8 cmd);
void OLED_UpdateDisplay(u8 cmd);
void OLED_WriteByte(u8 dat, u8 cmd);

#endif
