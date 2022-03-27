// author:AngelToms
#include "global.h"

// 参考： 
// https://zhuanlan.zhihu.com/p/166184285

#define SETMODE  0x38
#define DISOPEN  0x0C
#define DISMODE  0x6
#define DISSHIFT 0x14
#define CLEAR  0x01
#define RET  0x02

#define SETADDR  0x80

sbit LCD_RS = P5^0;
sbit LCD_RW = P5^1;
sbit LCD_E = P5^2;

#define LCD_DB P2

void lcd_write_data(uint8 dat) {
    lcd_checkbusy();
    // LCD_DB = dat; // also you can set LCD_DB firstly
    
    LCD_RS = 1;
    LCD_E = 0;
    LCD_RW = 0;
    time_HoggingDelayUs(2);
    LCD_E = 1;
    time_HoggingDelayUs(2);
    LCD_DB = dat;
    LCD_E = 0;
}

void lcd_write_cmd(uint8 cmd) {
    lcd_checkbusy();
    
    LCD_RS = 0;
    LCD_E = 0;
    LCD_RW = 0;
    time_HoggingDelayUs(2);
    LCD_E = 1;
    time_HoggingDelayUs(2);
    LCD_DB = cmd;
    LCD_E = 0;
}

uint8 lcd_readstatus() {
    uint8 status;
    LCD_DB = 0xff;
    
    LCD_RS = 0;
    LCD_RW = 1;
    LCD_E = 0;
    time_HoggingDelayUs(2);
    LCD_E = 1;
    time_HoggingDelayUs(2);
    status = LCD_DB;
    LCD_E = 0;
    return status;
}

void lcd_checkbusy() {
    while(lcd_readstatus() & 0x80);
}

void lcd_init() {
    LCD_DB = 0xff;
    LCD_E = 0;
    time_HoggingDelayMs(50);
    lcd_write_cmd(SETMODE);
    lcd_write_cmd(DISOPEN);
    lcd_write_cmd(DISMODE);
    // lcd_write_cmd(DISSHIFT);
    lcd_write_cmd(CLEAR);
}

void lcd_write_one_data(uint8 x, uint8 y, uint8 dat) {
    x &= 0xf; // x : line, one line can show 16 characters(2 ^ 4 )
    y &= 0x1; // y : row, y & 0x1 ,only last bit used, 1 or 0, 0 next line disable 
    if(y)
       x |= 0x40; // db6 set 1, over one rows
    x |= 0x80;
    lcd_write_cmd(x); // addr
    lcd_write_data(dat);
}

void lcd_wirte_string(uint8 x, uint8 y, const char* pbuf) {
    uint8_t addr = 0x80;
    x &= 0x0f;
    y &= 0x01;
    
    if(y) 
        addr += 0x40;
        
    addr += x;
    lcd_write_cmd(addr);
    for(; x<16 && *pbuf; pbuf++, x++) {
      lcd_write_data(*pbuf);
    }
}


 