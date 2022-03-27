#include "global.h"

uint8 code line0[] = "I love MCU!     ";

uint8 code line1[] = "LCD1602 Test!   ";


static void InitSTC8A(void)
{
    P0M1 = 0x00;
    P0M0 = 0x00;
    P1M1 = 0x00;
    P1M0 = 0x00;
    P2M1 = 0x00;
    P2M0 = 0x00;
    P3M1 = 0x00;
    P3M0 = 0x00;
    P4M1 = 0x00;
    P4M0 = 0x00;
    P5M1 = 0x00;
    P5M0 = 0x00;
    P6M1 = 0x00;
    P6M0 = 0x00;
    P7M1 = 0x00;
    P7M0 = 0x00;
}

#define ESPI            0x02

void main(void)
{
    int i, j;
    //u1* pbuf = "hello world spi flash write test!";
    //u1* pbuf = "AAAAAAABBBBBBBBBBBCCCCCCCCCCAAAA!";
    //int len = 33;
    //u1 rpbuf[34];
    /**
    unsigned int ix = 0;
    int idata x;
    int xdata y;
    int data z;
    int pdata o;**/
    InitSTC8A();
    //time_Initialization();
    uart1_Initialization();
  //printf("Hello world.\r\n");
    spi_init(0);
    //oled_ligth();
    oled_init();
    oled_test();
    printf("------------------------\r\n");
    
    
    time_HoggingDelayMs(100000000000);
    
    /**
    read_device_id();
    page_write(pbuf, len, 256);
    fast_read(rpbuf, len, 0);
    rpbuf[len] = '\0';
    printf("buf[0] = %s\r\n", rpbuf);
    fast_read(rpbuf, len, 256);
    rpbuf[len] = '\0';
    printf("buf[1] = %s\r\n", rpbuf);
 **/

    
    //time_HoggingDelayMs(100000);
    /**
    led_Initialization();
    time_HoggingDelayMs(1000);
    led_TurnOn();
    time_Initialization();
    time_HoggingDelayMs(1000);
    led_unint();
    lcd_init();
    lcd_wirte_string(0, 0, "Hello World!    ");
  lcd_wirte_string(0, 1, "LCD1602 Test!   ");

    enable_nest_clock();
    **/
/*
    led_Initialization();
    uart1_Initialization();
    printf("Hello world.\r\n");
        
    for (;ix < 1000;ix++)

    {

       led_TurnOn();

       printf("Turn on LED. count = %u\r\n",ix);

       time_HoggingDelayMs(500);

       led_TurnOff();

       printf("Turn off LED. count = %u\r\n",ix);

       time_HoggingDelayMs(500);

    }*/
        
}
