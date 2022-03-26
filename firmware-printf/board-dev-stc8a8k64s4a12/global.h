/*
 * Copyright        : Shendong MCU studio
 * File Description : Globally Included head file.
 * Created          : oliverzhang@shendongmcu.com
 */

#ifndef __GLOBAL_H__
#define __GLOBAL_H__

#include <stdio.h>
#include <intrins.h>
#include "STC8AXX.h"
#include <stdio.h>
#include <string.h>

/*led.c*/


/*uart1.c*/
// extern void uart1_Initialization(void);

/*time.c*/
extern void time_HoggingDelayMs(unsigned int ms);
#define CFG_SYSFREQ 22118400UL


#define uint8     unsigned char
#define uint16   unsigned short int
#define uint32   unsigned long int
#define int8       signed char
#define int16     signed short int
#define int32     signed long int
#define uint64   unsigned long long int
#define int64     signed long long int
	
typedef unsigned char uint8_t;
typedef signed char int8_t;
typedef unsigned char u1;
typedef signed char s1;
typedef unsigned short int u2;
typedef signed short int s2;
typedef unsigned int u4;
typedef signed int s4;

typedef unsigned short uint16_t;
typedef signed short int16_t;

typedef unsigned long uint32_t;
typedef signed long int32_t;
typedef u1 boolean;

#define __disable_irq() do{_nop_();EA = 0;_nop_();}while(0)
#define __enable_irq() do{_nop_();EA = 1;_nop_();}while(0)

/** LCD1602
extern void lcd_init();
extern uint8 lcd_readstatus();
extern void lcd_checkbusy();
extern void lcd_write_data(uint8 dat);
extern void lcd_write_cmd(uint8 cmd);
extern void lcd_write_one_data(uint8 x, uint8 y, uint8 dat);
extern void lcd_wirte_string(uint8 x, uint8 y, const char* pbuf);
**/


extern unsigned long time_GetTicks(void);
extern void tick_Task(void);
extern void time_Initialization(void);
extern void time_HoggingDelayMs(unsigned int ms);
extern void time_HoggingDelayUs(unsigned int us);


/**
extern void enable_xdata();
extern void disable_xdata();
extern void enable_irc24mcr();
extern void check_irc24mcr_status();
extern void enable_nest_clock();
**/

extern void spi_init(u1 speed);
extern void spi_enable();
extern void spi_disable();
extern u1 spi_rw(u1 val);

//extern void page_write(u1* pbuf, u4 len, u4 addr);
//extern void fast_read(u1* pbuf, u4 len, u4 addr);

extern void oled_init();
extern void oled_command(u1 cmd);
extern void oled_data(u1 dat);
extern void oled_data(u1 dat);
extern void oled_write_page(u1* dat, u2 len);

extern void oled_fill_gram();
extern void oled_clear_gram();
extern void oled_show_gram();
extern void oled_set_pixel(u1 x, u1 y);
extern void oled_draw_xline(u1 x, u1 y, u1 w);
extern void oled_draw_yline(u1 x, u1 y, u1 h);
extern void oled_draw_line(u1 x1, u1 y1, u1 x2, u1 y2);
extern void oled_draw_rect(u1 x, u1 y, u1 w, u1 h);
extern void oled_fill_rect(u1 x, u1 y, u1 w, u1 h);
extern void oled_draw_char(u1 x, u1 y, u1 w, u1 h, u1 sz);
extern void oled_draw_from_bitmap(u1 x, u1 y, u1 width, u1 height, u1* dat);
extern void oled_test();

#define VERI_SCAN_LSB_BIT_REVERSE
#endif
