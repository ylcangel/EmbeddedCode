// author: AngelToms

#include "global.h"
// sh1106 oled 1.3 inch

sbit OLED_RESET = P5^2;
sbit OLED_DC = P5^3;

void oled_command(u1 cmd) {
	OLED_DC = 0; // write command
	spi_enable();
	spi_rw(cmd);
	spi_disable();
	time_HoggingDelayUs(5);
}

void oled_data(u1 dat) {
	OLED_DC = 1; // write data
	spi_enable();
	spi_rw(dat);
	spi_disable();
	time_HoggingDelayUs(5);
}

static void oled_datas(u1* dat, int len) {
	int i = 0;
	OLED_DC = 1; // write data
	spi_enable();
	for (; i < len; i++) {
		spi_rw(*dat++);
	}
	spi_disable();
	time_HoggingDelayUs(5);
}

void oled_init() {
	OLED_RESET = 0;
	time_HoggingDelayMs(100);
	OLED_RESET = 1;
	time_HoggingDelayMs(100);
	#if 0
	oled_command(0xAE);//--turn off oled panel
	oled_command(0x02);//---set low column address
	oled_command(0x10);//---set high column address
	oled_command(0x40);//--set start line address  Set Mapping RAM Display Start Line (0x00~0x3F)
	oled_command(0x81);//--set contrast control register
	oled_command(0xCF); // Set SEG Output Current Brightness
	oled_command(0xA1);//--Set SEG/Column Mapping     0xa0???? 0xa1??
	oled_command(0xC8);//Set COM/Row Scan Direction   0xc0???? 0xc8??
	oled_command(0xA6);//--set normal display
	oled_command(0xA8);//--set multiplex ratio(1 to 64)
	oled_command(0x3f);//--1/64 duty
	oled_command(0xD3);//-set display offset	Shift Mapping RAM Counter (0x00~0x3F)
	oled_command(0x00);//-not offset
	oled_command(0xd5);//--set display clock divide ratio/oscillator frequency
	oled_command(0x80);//--set divide ratio, Set Clock as 100 Frames/Sec
	oled_command(0xD9);//--set pre-charge period
	oled_command(0xF1);//Set Pre-Charge as 15 Clocks & Discharge as 1 Clock
	oled_command(0xDA);//--set com pins hardware configuration
	oled_command(0x12);
	oled_command(0xDB);//--set vcomh
	oled_command(0x40);//Set VCOM Deselect Level
	oled_command(0x20);//-Set Page Addressing Mode (0x00/0x01/0x02)
	oled_command(0x02);//
	oled_command(0x8D);//--set Charge Pump enable/disable
	oled_command(0x14);//--set(0x10) disable
	oled_command(0xA4);// Disable Entire Display On (0xa4/0xa5)
	oled_command(0xA6);// Disable Inverse Display On (0xa6/a7) 
	oled_command(0X8D);  //SET DCDC??
	oled_command(0X14);  //DCDC ON
	#endif
	//oled_command(0xAD);
	//oled_command(0x8A);

	oled_command(0XAF);  //DISPLAY ON
	oled_command(0xA1); // from left to right
	oled_command(0xC8); // up to down
	oled_command(0x81);//--set contrast control register
	oled_command(0x60); // Set SEG Output Current Brightness
	
	#if 0
	oled_command(0x20);//-Set Page Addressing Mode (0x00/0x01/0x02)
	oled_command(0x0);//
	
	oled_command(0xA1); // from left to right
	oled_command(0xC8); // up to down
	
	oled_command(0xA6); // normal dis
	oled_command(0x40); // start display line
	oled_command(0xB0); //page
	oled_command(0x01); // column low
	oled_command(0x10); // column high
	
	oled_command(0xDA);
	oled_command(0x02);
	#endif
	
}

static void oled_display_all() {
	oled_command(0xA5);
}

void oled_write_page(u1* dat, u2 len) {
	oled_datas(dat, len);
}


