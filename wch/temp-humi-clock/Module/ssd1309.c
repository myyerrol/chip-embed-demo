#include <ssd1309_font.h>
#include <ssd1309.h>

u8 SSD1309_GRAM[144][8];

void SSD1309_DrawPoint(u8 x, u8 y) {
    u8 i, m, n;
    i = y / 8;
    m = y % 8;
    n = 1 << m;
    SSD1309_GRAM[x][i] |= n;
}

void SSD1309_DrawLine(u8 x1, u8 y1, u8 x2, u8 y2) {
    u8 i, k, k1, k2;
    if ((x1 < 0) || (x2 > 128) || (y1 < 0) || ( y2 > 64) || (x1 > x2) || (y1 > y2)) return;
    if (x1 == x2) {
        for (i = 0; i < (y2 - y1); i++) {
            SSD1309_DrawPoint(x1, y1 + i);
        }
    }
    else if (y1 == y2) {
        for (i = 0; i < (x2 - x1); i++) {
            SSD1309_DrawPoint(x1+i,y1);
        }
    }
    else {
        k1 = y2 - y1;
        k2 = x2 - x1;
        k  = k1 * 10 / k2;
        for(i = 0; i < (x2 - x1); i++) {
            SSD1309_DrawPoint(x1 + i, y1 + i * k / 10);
        }
    }
}

void SSD1309_DrawCircle(u8 x, u8 y,u8 r) {
    int a, b, num;
    a = 0;
    b = r;
    while(2 * b * b >= r * r) {
        SSD1309_DrawPoint(x + a, y - b);
        SSD1309_DrawPoint(x - a, y - b);
        SSD1309_DrawPoint(x - a, y + b);
        SSD1309_DrawPoint(x + a, y + b);

        SSD1309_DrawPoint(x + b, y + a);
        SSD1309_DrawPoint(x + b, y - a);
        SSD1309_DrawPoint(x - b, y - a);
        SSD1309_DrawPoint(x - b, y + a);

        a++;
        num = (a * a + b * b) - r * r;
        if(num > 0) {
            b--;
            a--;
        }
    }
}

void SSD1309_ShowChar(u8 x, u8 y, u8 chr, u8 size1) {
    u8 i, m, temp, size2, chr1;
    u8 y0 = y;
    size2 = (size1 / 8 + ((size1 % 8) ? 1 : 0)) * (size1 / 2);
    chr1 = chr - ' ';
    for (i = 0; i < size2; i++) {
        if (size1 == 12) {
            temp = ascii_1206[chr1][i];
        }
        else if (size1 == 16) {
            temp = ascii_1608[chr1][i];
        }
        else if (size1 == 24) {
            temp = ascii_2412[chr1][i];
        }
        else {
            return;
        }

        for(m = 0; m < 8; m++) {
            if (temp & 0x80) {
                SSD1309_DrawPoint(x, y);
            }
            else {
                SSD1309_ClearPoint(x, y);
            }
            temp <<= 1;
            y++;
            if((y - y0) == size1) {
                y=y0;
                x++;
                break;
            }
        }
    }
}

void SSD1309_ShowString(u8 x, u8 y , char *chr, u8 size1) {
    while((*chr >= ' ') && (*chr <= '~')) {
        SSD1309_ShowChar(x, y, *chr, size1);
        x += size1 / 2;
        if(x > 128 - size1) {
            x  = 0;
            y += 2;
        }
        chr++;
    }
}

void SSD1309_ShowNum(u8 x, u8 y, u32 num, u8 len, u8 size1) {
    u8 t, temp;
    for(t = 0; t < len; t++) {
        temp = (num / SSD1309_POW(10, len - t - 1)) % 10;
        if(temp == 0) {
            SSD1309_ShowChar(x + (size1 / 2) * t, y, '0', size1);
        }
        else {
            SSD1309_ShowChar(x + (size1 / 2) * t, y, temp + '0', size1);
        }
    }
}

void SSD1309_ShowChinese(u8 x, u8 y, u8 num, u8 size1) {
    u8 i, m, n = 0, temp = 0, chr1;
    u8 x0 = x, y0 = y;
    u8 div = size1 / 2;;
    u8 size3 = size1 / div;
    while (size3--) {
        chr1 = num * size1 / div + n;
        n++;
        for(i = 0; i < size1; i++) {
            if (size1 == 12) {
                temp = zh_1212[chr1][i];
            }
            else if (size1 == 16) {
            }
            else if (size1 == 24) {
            }
            else if (size1 == 32) {
            }
            else if (size1 == 64) {
            }
            else {
                return;
            }

            for (m = 0; m < 8; m++) {
                if (temp & 0x01) {
                    SSD1309_DrawPoint(x, y);
                }
                else {
                    SSD1309_ClearPoint(x, y);
                }
                temp >>= 1;
                y++;
            }
            x++;
            if ((x - x0) == size1) {
                x  = x0;
                y0 = y0 + 8;
            }
            y = y0;
        }
    }
}

void SSD1309_ShowPicture(u8 x0, u8 y0, u8 x1, u8 y1, u8 bmp[]) {
    u32 j = 0;
    u8 x = 0,y = 0;
    if (y % 8 == 0) {
        y = 0;
    }
    else {
        y += 1;
    }
    for (y = y0; y < y1; y++) {
        SSD1309_SetStartLocation(x0, y);
         for(x = x0; x < x1; x++) {
            SSD1309_WriteByte(bmp[j], SSD1309_DATA);
            j++;
        }
    }
}

void SSD1309_Clear(void) {
    u8 i, n;
    for (i = 0; i < 8; i++) {
       for (n = 0; n < 128; n++) {
            SSD1309_GRAM[n][i] = 0;
        }
    }
    SSD1309_Refresh();
}

void SSD1309_ClearPoint(u8 x, u8 y) {
    u8 i, m, n;
    i = y / 8;
    m = y % 8;
    n = 1 << m;
    SSD1309_GRAM[x][i] =~ SSD1309_GRAM[x][i];
    SSD1309_GRAM[x][i] |= n;
    SSD1309_GRAM[x][i] =~ SSD1309_GRAM[x][i];
}

void SSD1309_Init(void) {
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_APB2PeriphClockCmd(SSD1309_APB2, ENABLE);
    GPIO_InitStructure.GPIO_Pin   = SSD1309_PIN_SCK  |
                                    SSD1309_PIN_MOSI |
                                    SSD1309_PIN_RES  |
                                    SSD1309_PIN_DC   |
                                    SSD1309_PIN_CS;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = SSD1309_SPD;
    GPIO_Init(SSD1309_PORT, &GPIO_InitStructure);

    SSD1309_WR_RES_0;
    Delay_Ms(200);
    SSD1309_WR_RES_1;

    SSD1309_WriteByte(0xAE, SSD1309_CMDS);

    SSD1309_WriteByte(0x40, SSD1309_CMDS);
    SSD1309_WriteByte(0x81, SSD1309_CMDS);
    SSD1309_WriteByte(0xAF, SSD1309_CMDS);
    SSD1309_WriteByte(0xA1, SSD1309_CMDS);
    SSD1309_WriteByte(0xC8, SSD1309_CMDS);

    SSD1309_WriteByte(0xA8, SSD1309_CMDS);
    SSD1309_WriteByte(0x3f, SSD1309_CMDS);
    SSD1309_WriteByte(0xD3, SSD1309_CMDS);
    SSD1309_WriteByte(0x00, SSD1309_CMDS);
    SSD1309_WriteByte(0xd5, SSD1309_CMDS);
    SSD1309_WriteByte(0x80, SSD1309_CMDS);
    SSD1309_WriteByte(0xD9, SSD1309_CMDS);
    SSD1309_WriteByte(0x25, SSD1309_CMDS);
    SSD1309_WriteByte(0xDA, SSD1309_CMDS);
    SSD1309_WriteByte(0x12, SSD1309_CMDS);
    SSD1309_WriteByte(0xDB, SSD1309_CMDS);
    SSD1309_WriteByte(0x34, SSD1309_CMDS);

    SSD1309_WriteByte(0xA4, SSD1309_CMDS);
    SSD1309_WriteByte(0xA6, SSD1309_CMDS);

    SSD1309_Clear();
    SSD1309_WriteByte(0xAF, SSD1309_CMDS);
}

void SSD1309_Refresh(void) {
    u8 i, n;
    for (i = 0; i < 8; i++) {
       SSD1309_WriteByte(0xb0 + i, SSD1309_CMDS);
       SSD1309_WriteByte(0x00    , SSD1309_CMDS);
       SSD1309_WriteByte(0x10    , SSD1309_CMDS);
       for (n = 0; n < 128; n++) {
            SSD1309_WriteByte(SSD1309_GRAM[n][i], SSD1309_DATA);
       }
    }
}

void SSD1309_SetStartLocation(u8 x, u8 y) {
    SSD1309_WriteByte(    0xb0 + y,         SSD1309_CMDS);
    SSD1309_WriteByte(((x&0xf0)>>4) | 0x10, SSD1309_CMDS);
    SSD1309_WriteByte( (x&0x0f),            SSD1309_CMDS);
}

void SSD1309_TurnOn(void) {
    SSD1309_WriteByte(0x8D, SSD1309_CMDS);
    SSD1309_WriteByte(0x14, SSD1309_CMDS);
    SSD1309_WriteByte(0xAF, SSD1309_CMDS);
}

void SSD1309_TurnOff(void) {
    SSD1309_WriteByte(0x8D, SSD1309_CMDS);
    SSD1309_WriteByte(0x10, SSD1309_CMDS);
    SSD1309_WriteByte(0xAF, SSD1309_CMDS);
}

void SSD1309_UpdateColor(u8 cmd) {
    if (cmd == SSD1309_COLOR_NORMAL) {
        SSD1309_WriteByte(0xA6, SSD1309_CMDS);
    }
    if (cmd == SSD1309_COLOR_INVERT) {
        SSD1309_WriteByte(0xA7, SSD1309_CMDS);
    }
}

void SSD1309_UpdateDisplay(u8 cmd) {
    if (cmd == SSD1309_DISPLAY_NORMAL) {
        SSD1309_WriteByte(0xC8, SSD1309_CMDS);
        SSD1309_WriteByte(0xA1, SSD1309_CMDS);
    }
    if (cmd == SSD1309_DISPLAY_INVERT) {
        SSD1309_WriteByte(0xC0, SSD1309_CMDS);
        SSD1309_WriteByte(0xA0, SSD1309_CMDS);
    }
}

void SSD1309_WriteByte(u8 dat, u8 cmd) {
    u8 i;
    if (cmd) {
      SSD1309_WR_DC_1;
    }
    else {
        SSD1309_WR_DC_0;
    }
    SSD1309_WR_CS_0;
    for (i = 0; i < 8; i++) {
        SSD1309_WR_SCK_0;
        if(dat & 0x80) {
            SSD1309_WR_MOSI_1;
        }
        else {
           SSD1309_WR_MOSI_0;
        }
        SSD1309_WR_SCK_1;
        dat <<= 1;
    }
    SSD1309_WR_CS_1;
    SSD1309_WR_DC_1;
}
