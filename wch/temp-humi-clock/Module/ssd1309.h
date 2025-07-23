#ifndef SSD1309_H
#define SSD1309_H

#include <debug.h>

#define SSD1309_MODE_SPI
#define SSD1309_DEBUG

// SPI
#ifdef SSD1309_MODE_SPI
#define SSD1309_PIN_SCK  GPIO_Pin_5 // PC5
#define SSD1309_PIN_MOSI GPIO_Pin_6 // PC6
#define SSD1309_PIN_RES  GPIO_Pin_1 // PC1
#define SSD1309_PIN_DC   GPIO_Pin_2 // PC2
#define SSD1309_PIN_CS   GPIO_Pin_3 // PC3
#else
// I2C
#define SSD1309_PIN_SCL  GPIO_Pin_2 // PC2
#define SSD1309_PIN_SDA  GPIO_Pin_1 // PC1
#endif

#define SSD1309_SPD   GPIO_Speed_50MHz
#define SSD1309_PORT  GPIOC
#define SSD1309_APB2  RCC_APB2Periph_GPIOC

#ifdef SSD1309_MODE_SPI
#define SSD1309_WR_SCK_1  GPIO_SetBits  (SSD1309_PORT, SSD1309_PIN_SCK )
#define SSD1309_WR_SCK_0  GPIO_ResetBits(SSD1309_PORT, SSD1309_PIN_SCK )
#define SSD1309_WR_MOSI_1 GPIO_SetBits  (SSD1309_PORT, SSD1309_PIN_MOSI)
#define SSD1309_WR_MOSI_0 GPIO_ResetBits(SSD1309_PORT, SSD1309_PIN_MOSI)
#define SSD1309_WR_RES_1  GPIO_SetBits  (SSD1309_PORT, SSD1309_PIN_RES )
#define SSD1309_WR_RES_0  GPIO_ResetBits(SSD1309_PORT, SSD1309_PIN_RES )
#define SSD1309_WR_DC_1   GPIO_SetBits  (SSD1309_PORT, SSD1309_PIN_DC  )
#define SSD1309_WR_DC_0   GPIO_ResetBits(SSD1309_PORT, SSD1309_PIN_DC  )
#define SSD1309_WR_CS_1   GPIO_SetBits  (SSD1309_PORT, SSD1309_PIN_CS  )
#define SSD1309_WR_CS_0   GPIO_ResetBits(SSD1309_PORT, SSD1309_PIN_CS  )
#else
#endif

#define SSD1309_CMDS           0
#define SSD1309_DATA           1
#define SSD1309_COLOR_NORMAL   0
#define SSD1309_COLOR_INVERT   1
#define SSD1309_DISPLAY_NORMAL 0
#define SSD1309_DISPLAY_INVERT 1

#define SSD1309_DISPLAY_W 128
#define SSD1309_DISPLAY_H 64

#define u8  uint8_t
#define u32 uint32_t

#define SSD1309_POW(m, n) ({        \
    u32 res = 1;                 \
    for (u8 i = n; i > 0; i--) { \
        res *=  m;               \
    }                            \
    res;                         \
})

void SSD1309_DrawPoint(u8 x, u8 y);
void SSD1309_DrawLine(u8 x1, u8 y1, u8 x2, u8 y2);
void SSD1309_DrawCircle(u8 x, u8 y, u8 r);

void SSD1309_ShowChar(u8 x, u8 y, u8 chr, u8 size1);
void SSD1309_ShowString(u8 x, u8 y, char *chr, u8 size1);
void SSD1309_ShowNum(u8 x, u8 y, u32 num, u8 len, u8 size1);
void SSD1309_ShowChinese(u8 x,u8 y,u8 num,u8 size1);
void SSD1309_ShowPicture(u8 x0,u8 y0,u8 x1,u8 y1,u8 bmp[]);

void SSD1309_Clear(void);
void SSD1309_ClearPoint(u8 x, u8 y);
void SSD1309_Init(void);
void SSD1309_Refresh(void);
void SSD1309_SetStartLocation(u8 x,u8 y);
void SSD1309_TurnOn(void);
void SSD1309_TurnOff(void);
void SSD1309_UpdateColor(u8 cmd);
void SSD1309_UpdateDisplay(u8 cmd);
void SSD1309_WriteByte(u8 dat, u8 cmd);

#endif
