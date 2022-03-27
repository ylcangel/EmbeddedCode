// author: AngelToms

#include "global.h"
#include "font.h"

#define OLED_WIDTH 132
#define OLED_HEIGHT 64
#define OLED_PAGE_SIZE 8
#define OLED_PAGE_ALIGN
#define OLED_ROW_BIT_SZ 8
#define OLED_MAX_LINE_SZ 128

static u1 xdata OLED_GRAM[OLED_PAGE_SIZE][OLED_WIDTH];

int ABS(int n) {
        int tmp = n >>31;   
    return (n ^ tmp) - tmp;
}

void oled_fill_gram() {
    u1 i,j;
    for (i = 0; i < OLED_PAGE_SIZE; i++) {
        for (j = 0; j < OLED_WIDTH; j++) {
            OLED_GRAM[i][j] = 0xff;
        }
    }
}

void oled_clear_gram() {
    u1 i,j;
    for (i = 0; i < OLED_PAGE_SIZE; i++) {
        for (j = 0; j < OLED_WIDTH; j++) {
            OLED_GRAM[i][j] = 0;
        }
    }
}

void oled_show_gram() {
    u1 i,j;
#ifdef CONSOLE
    for (i = 0; i < OLED_PAGE_SIZE; i++) {
        for (j = 0; j < OLED_WIDTH; j++) {
            printf("%x ", OLED_GRAM[i][j]);
        }
        printf("\n");
    }
#else
    for (i = 0; i < OLED_PAGE_SIZE; i++) {
        oled_command(0xb0 + i);
        oled_command(0x00);
        oled_command(0x10);
        oled_write_page(OLED_GRAM[i], OLED_WIDTH);
    }
#endif
}

void oled_set_pixel(u1 x, u1 y) {
    if (x > OLED_WIDTH || y > OLED_HEIGHT) {
        printf("oled_set_pixel bad params");
        return;
    }

    // D0 - D8(one page 8 column)
    OLED_GRAM[y/8][x] |= (1 << (y & 7)); // 1 << (y % 8)
}

void oled_draw_point(u1 x, u1 y) {
    oled_set_pixel(x, y);
}

void oled_draw_xline(u1 x, u1 y, u1 w) {
    u1 i;
    if (x > OLED_WIDTH || w > OLED_WIDTH || y > OLED_HEIGHT) {
        printf("oled_draw_xline bad params");
        return;
    }

    if (x + w > OLED_WIDTH) {
        w = OLED_WIDTH - x;
    }

    for (i = 0; i < w; i++) {
        oled_draw_point(x+i, y);
    }
}

void oled_draw_yline(u1 x, u1 y, u1 h) {
    u1 i;
        printf("x = %u, y = %u, h = %u\r\n", x, y, h);
    if (x > OLED_WIDTH || h > OLED_HEIGHT || y > OLED_HEIGHT) {
        printf("oled_draw_yline bad params");
        return;
    }

        printf("x = %u, y = %u, h = %u\r\n", x, y, h);
    if (y + h > OLED_HEIGHT) {
        h = OLED_HEIGHT - y;
    }
        printf("x = %u, y = %u, h = %u\r\n", x, y, h);
    for (i = 0; i < h; i++) {
        oled_draw_point(x, y+i);
    }
}

void oled_draw_line(u1 x1, u1 y1, u1 x2, u1 y2) {
    u1 dx, dy, n, k;
    float xinc, yinc, x, y;
    char dotten_len;
    if (x1 > OLED_WIDTH || x2 > OLED_WIDTH || y1 > OLED_HEIGHT || y2 > OLED_HEIGHT) {
        printf("oled_draw_line bad params");
        return;
    }
        
    dx = x2 - x1;
    dy = y2 - y1;
    
    if (ABS((int)dx) > ABS((int)dy))
        n = ABS((int)dx);
    else
        n = ABS((int)dy);
    
    xinc = (float)dx/n;
    yinc = (float)dy/n;

    x = (float)x1;
    y = (float)y1;
    for (k=1; k <= n; k++){
        oled_draw_point((u1)(x+0.5), (u1)(y+0.5));
        x += xinc;
        y += yinc;
    }
}

void oled_draw_rect(u1 x, u1 y, u1 w, u1 h) {
    if (x > OLED_WIDTH || w > OLED_WIDTH || 
        (x + w) > OLED_WIDTH || y > OLED_HEIGHT || h > OLED_HEIGHT || 
        (y + h) > OLED_HEIGHT) {
        printf("oled_draw_rect bad params");
        return;
    }

    oled_draw_xline(x, y, w);
    oled_draw_xline(x, y + h, w);
    oled_draw_yline(x, y, h);
    oled_draw_yline(x + w, y, h);
}

void oled_fill_rect(u1 x, u1 y, u1 w, u1 h) {
    u1 i = y;
    if (x > OLED_WIDTH || (x + w) > OLED_WIDTH || y > OLED_HEIGHT || (y + h) > OLED_HEIGHT) {
        printf("oled_draw_rect bad params");
        return;
    }

    for (; i < y + h; i++) {
        oled_draw_xline(x, y+i, w);
    }
}

void oled_draw_from_bitmap(u1 x, u1 y, u1 width, u1 height, u1* dat) {
    u1 i, j, w, h, p;
    if (x > OLED_WIDTH || y > OLED_HEIGHT) {
        printf("oled_draw_from_bitcode bad params");
        return;
    }

    if (width > OLED_WIDTH || height > OLED_HEIGHT) {
        printf("oled_draw_from_bitcode bad params");
        return;
    }

    w = width;
    h = height;
    
    if (x + width > OLED_WIDTH) {
        width = OLED_WIDTH - x;
    }

    if (y + height > OLED_WIDTH) {
        height = OLED_WIDTH - height;
    }

        p = (width+7)/8;

#if defined(VERI_SCAN_LSB_BIT_REVERSE)
    for (i = 0; i < width; i++) {
                for (j = 0; j < height; j++) { // up to down 8 bit as 1 byte
            if (((*(dat + (j >> 3) + i*8)) >> (j&7)) & 1) {
                oled_draw_point(x+i, y+j);
            }
        }
    }
#elif defined(HORI_SCAN_LSB_BIT_REVERSE)
    for (j = 0; j < height; j++) {
            for (i = 0; i < width; i++) {
                    if ((*(dat + i/8 + j*p) >> (i&7)) & 1) {
                            oled_draw_point(x+i, y+j);
                    }
            }
    }
#else
    // left to right LSB
    for (j = 0; j < height; j++) {
        for (i = 0; i < width; i++) {
            if (*(dat + + i/8 + j*p) & (132 >> (i&7))) {
                oled_draw_point(x+i, y+j);
            }
        }
    }
#endif
}

void oled_draw_char(u1 x, u1 y, u1 w, u1 h, u1 sz) {
    int i, j, px, py, k;
    u1* p;
    int fc, l;
    if (x > OLED_WIDTH || y > OLED_HEIGHT || w > OLED_WIDTH || h > OLED_HEIGHT
        || x + w > OLED_WIDTH || y + h > OLED_HEIGHT) {
            printf("oled_draw_char bad params\r\n");
            return;
        }

    if (sz == 32) {
        fc = sizeof(Cn32_Table) / (w * h / OLED_ROW_BIT_SZ);
    } else if (sz == 16) {
        fc = sizeof(Font16_Table) / (w * h / OLED_ROW_BIT_SZ);
    } else {
        fc = sizeof(Font64_Table) / (w * h / OLED_ROW_BIT_SZ);
    }
    
    for (l = 0; l < fc; l++) {
        if (sz == 32) {
            p = Cn32_Table[l];
        } else if (sz == 16) {
            p = Font16_Table[l];
        } else {
            p = Font64_Table[l];
        }
        
        for ( i = 0; i < w; i++) {
            px = x + ((l * w + i) & (OLED_MAX_LINE_SZ-1));
            
            for (j = 0; j < h; j++) {
                py = y + j + ((l * w) / OLED_MAX_LINE_SZ) * h;
                if (((*(p + (j >> 3) * w + i)) >> (j & 7)) & 1) {
                    oled_draw_point(px, py);
                }
            }
        }
    }
}


void oled_test() {
    time_HoggingDelayMs(1000);
    oled_clear_gram();
    time_HoggingDelayMs(1000);
    oled_fill_gram();
    oled_show_gram();
    time_HoggingDelayMs(1000);
    oled_clear_gram();
    oled_draw_xline(0, 0, 60);
    oled_draw_xline(0, 8, 80);
    oled_draw_xline(0, 16, 90);
    oled_draw_xline(0, 24, 100);
    oled_draw_xline(0, 32, 110);
    oled_draw_xline(0, 40, 120);
    oled_draw_xline(0, 48, 130);
    oled_draw_xline(0, 56, 132);
    oled_show_gram();
    time_HoggingDelayMs(1000);
    oled_clear_gram();
    oled_draw_yline(2, 0, 64);
    oled_draw_yline(12, 0, 64);
    oled_draw_yline(22, 8, 56);
    oled_draw_yline(32, 16, 48);
    oled_draw_yline(42, 24, 40);
    oled_draw_yline(52, 32, 32);
    oled_draw_yline(62, 40, 24);
    oled_draw_yline(72, 48, 16);
    oled_draw_yline(82, 56, 8);
    oled_draw_yline(92, 56, 8);
    oled_draw_yline(102, 56, 8);
    oled_draw_yline(112, 56, 8);
    oled_draw_yline(122, 56, 8);
    oled_show_gram();
    time_HoggingDelayMs(1000);
    oled_clear_gram();  
    oled_draw_line(2, 0, 66, 64);
    oled_draw_line(66, 0, 2, 64);
    oled_show_gram();
    time_HoggingDelayMs(1000);
    oled_clear_gram();  
    oled_draw_rect(2, 0, 60, 50);
    oled_show_gram();
    time_HoggingDelayMs(1000);
    oled_clear_gram();  
    oled_fill_rect(40, 0, 60, 50);
    oled_show_gram();
    time_HoggingDelayMs(1000);
    oled_clear_gram();
    oled_draw_from_bitmap(0, 0, 128, 64, gImage_a);
    oled_show_gram();
    time_HoggingDelayMs(1000);
    oled_clear_gram();
    oled_draw_from_bitmap(0, 0, 112, 64, gImage_b);
    oled_show_gram();
    time_HoggingDelayMs(1000);
    oled_clear_gram();
    oled_draw_from_bitmap(0, 0, 126, 64, gImage_c);
    oled_show_gram();
    time_HoggingDelayMs(1000);
    oled_clear_gram();
    oled_draw_char(0, 0, 64, 64, 64);
    oled_show_gram();
    time_HoggingDelayMs(1000);
    oled_clear_gram();
    oled_draw_char(0, 0, 32, 32, 32);
    oled_show_gram();
    time_HoggingDelayMs(1000);
    oled_clear_gram();
    oled_draw_char(2, 0, 8, 16, 16);
    oled_show_gram();
    time_HoggingDelayMs(20000);
}