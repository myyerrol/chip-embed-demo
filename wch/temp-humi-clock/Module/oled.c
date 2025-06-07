#include <oled_font.h>
#include <oled.h>

u8 OLED_GRAM[144][8];

void OLED_DrawPoint(u8 x, u8 y) {
    u8 i, m, n;
    i = y / 8;
    m = y % 8;
    n = 1 << m;
    OLED_GRAM[x][i] |= n;
}

void OLED_DrawLine(u8 x1, u8 y1, u8 x2, u8 y2) {
    u8 i, k, k1, k2;
    if ((x1 < 0) || (x2 > 128) || (y1 < 0) || ( y2 > 64) || (x1 > x2) || (y1 > y2)) return;
    if (x1 == x2) {
        for (i = 0; i < (y2 - y1); i++) {
            OLED_DrawPoint(x1, y1 + i);
        }
    }
    else if (y1 == y2) {
        for (i = 0; i < (x2 - x1); i++) {
            OLED_DrawPoint(x1+i,y1);
        }
    }
    else {
        k1 = y2 - y1;
        k2 = x2 - x1;
        k  = k1 * 10 / k2;
        for(i = 0; i < (x2 - x1); i++) {
            OLED_DrawPoint(x1 + i, y1 + i * k / 10);
        }
    }
}

void OLED_DrawCircle(u8 x, u8 y,u8 r) {
    int a, b, num;
    a = 0;
    b = r;
    while(2 * b * b >= r * r) {
        OLED_DrawPoint(x + a, y - b);
        OLED_DrawPoint(x - a, y - b);
        OLED_DrawPoint(x - a, y + b);
        OLED_DrawPoint(x + a, y + b);

        OLED_DrawPoint(x + b, y + a);
        OLED_DrawPoint(x + b, y - a);
        OLED_DrawPoint(x - b, y - a);
        OLED_DrawPoint(x - b, y + a);

        a++;
        num = (a * a + b * b) - r * r;
        if(num > 0) {
            b--;
            a--;
        }
    }
}

void OLED_ShowChar(u8 x, u8 y, u8 chr, u8 size1) {
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
                OLED_DrawPoint(x, y);
            }
            else {
                OLED_ClearPoint(x, y);
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

void OLED_ShowString(u8 x, u8 y , char *chr, u8 size1) {
    while((*chr >= ' ') && (*chr <= '~')) {
        OLED_ShowChar(x, y, *chr, size1);
        x += size1 / 2;
        if(x > 128 - size1) {
            x  = 0;
            y += 2;
        }
        chr++;
    }
}

void OLED_ShowNum(u8 x, u8 y, u32 num, u8 len, u8 size1) {
    u8 t, temp;
    for(t = 0; t < len; t++) {
        temp = (num / OLED_POW(10, len - t - 1)) % 10;
        if(temp == 0) {
            OLED_ShowChar(x + (size1 / 2) * t, y, '0', size1);
        }
        else {
            OLED_ShowChar(x + (size1 / 2) * t, y, temp + '0', size1);
        }
    }
}

void OLED_ShowChinese(u8 x, u8 y, u8 num, u8 size1) {
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
                    OLED_DrawPoint(x, y);
                }
                else {
                    OLED_ClearPoint(x, y);
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

void OLED_ShowPicture(u8 x0, u8 y0, u8 x1, u8 y1, u8 bmp[]) {
    u32 j = 0;
    u8 x = 0,y = 0;
    if (y % 8 == 0) {
        y = 0;
    }
    else {
        y += 1;
    }
    for (y = y0; y < y1; y++) {
        OLED_SetStartLocation(x0, y);
         for(x = x0; x < x1; x++) {
            OLED_WriteByte(bmp[j], OLED_DATA);
            j++;
        }
    }
}

void OLED_Clear(void) {
    u8 i, n;
    for (i = 0; i < 8; i++) {
       for (n = 0; n < 128; n++) {
            OLED_GRAM[n][i] = 0;
        }
    }
    OLED_Refresh();
}

void OLED_ClearPoint(u8 x, u8 y) {
    u8 i, m, n;
    i = y / 8;
    m = y % 8;
    n = 1 << m;
    OLED_GRAM[x][i] =~ OLED_GRAM[x][i];
    OLED_GRAM[x][i] |= n;
    OLED_GRAM[x][i] =~ OLED_GRAM[x][i];
}

void OLED_Init(void) {
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_APB2PeriphClockCmd(OLED_APB2, ENABLE);
    GPIO_InitStructure.GPIO_Pin   = OLED_PIN_SCK  |
                                    OLED_PIN_MOSI |
                                    OLED_PIN_RES  |
                                    OLED_PIN_DC   |
                                    OLED_PIN_CS;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = OLED_SPD;
    GPIO_Init(OLED_PORT, &GPIO_InitStructure);

    OLED_WR_RES_0;
    Delay_Ms(200);
    OLED_WR_RES_1;

    OLED_WriteByte(0xAE, OLED_CMDS);

    OLED_WriteByte(0x40, OLED_CMDS);
    OLED_WriteByte(0x81, OLED_CMDS);
    OLED_WriteByte(0xAF, OLED_CMDS);
    OLED_WriteByte(0xA1, OLED_CMDS);
    OLED_WriteByte(0xC8, OLED_CMDS);

    OLED_WriteByte(0xA8, OLED_CMDS);
    OLED_WriteByte(0x3f, OLED_CMDS);
    OLED_WriteByte(0xD3, OLED_CMDS);
    OLED_WriteByte(0x00, OLED_CMDS);
    OLED_WriteByte(0xd5, OLED_CMDS);
    OLED_WriteByte(0x80, OLED_CMDS);
    OLED_WriteByte(0xD9, OLED_CMDS);
    OLED_WriteByte(0x25, OLED_CMDS);
    OLED_WriteByte(0xDA, OLED_CMDS);
    OLED_WriteByte(0x12, OLED_CMDS);
    OLED_WriteByte(0xDB, OLED_CMDS);
    OLED_WriteByte(0x34, OLED_CMDS);

    OLED_WriteByte(0xA4, OLED_CMDS);
    OLED_WriteByte(0xA6, OLED_CMDS);

    OLED_Clear();
    OLED_WriteByte(0xAF, OLED_CMDS);
}

void OLED_Refresh(void) {
    u8 i, n;
    for (i = 0; i < 8; i++) {
       OLED_WriteByte(0xb0 + i, OLED_CMDS);
       OLED_WriteByte(0x00    , OLED_CMDS);
       OLED_WriteByte(0x10    , OLED_CMDS);
       for (n = 0; n < 128; n++) {
            OLED_WriteByte(OLED_GRAM[n][i], OLED_DATA);
       }
    }
}

void OLED_SetStartLocation(u8 x, u8 y) {
    OLED_WriteByte(    0xb0 + y,         OLED_CMDS);
    OLED_WriteByte(((x&0xf0)>>4) | 0x10, OLED_CMDS);
    OLED_WriteByte( (x&0x0f),            OLED_CMDS);
}

void OLED_TurnOn(void) {
    OLED_WriteByte(0x8D, OLED_CMDS);
    OLED_WriteByte(0x14, OLED_CMDS);
    OLED_WriteByte(0xAF, OLED_CMDS);
}

void OLED_TurnOff(void) {
    OLED_WriteByte(0x8D, OLED_CMDS);
    OLED_WriteByte(0x10, OLED_CMDS);
    OLED_WriteByte(0xAF, OLED_CMDS);
}

void OLED_UpdateColor(u8 cmd) {
    if (cmd == OLED_COLOR_NORMAL) {
        OLED_WriteByte(0xA6, OLED_CMDS);
    }
    if (cmd == OLED_COLOR_INVERT) {
        OLED_WriteByte(0xA7, OLED_CMDS);
    }
}

void OLED_UpdateDisplay(u8 cmd) {
    if (cmd == OLED_DISPLAY_NORMAL) {
        OLED_WriteByte(0xC8, OLED_CMDS);
        OLED_WriteByte(0xA1, OLED_CMDS);
    }
    if (cmd == OLED_DISPLAY_INVERT) {
        OLED_WriteByte(0xC0, OLED_CMDS);
        OLED_WriteByte(0xA0, OLED_CMDS);
    }
}

void OLED_WriteByte(u8 dat, u8 cmd) {
    u8 i;
    if (cmd) {
      OLED_WR_DC_1;
    }
    else {
        OLED_WR_DC_0;
    }
    OLED_WR_CS_0;
    for (i = 0; i < 8; i++) {
        OLED_WR_SCK_0;
        if(dat & 0x80) {
            OLED_WR_MOSI_1;
        }
        else {
           OLED_WR_MOSI_0;
        }
        OLED_WR_SCK_1;
        dat <<= 1;
    }
    OLED_WR_CS_1;
    OLED_WR_DC_1;
}
