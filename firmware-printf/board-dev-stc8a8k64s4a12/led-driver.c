// author: AngelToms

#include "global.h"

sbit LED_RUN = P4^2;
sbit LED_RUN1 = P4^3;

void led_Initialization(void)
{
	LED_RUN = 0;
	LED_RUN1 = 0;
}

void led_TurnOn(void)
{
	LED_RUN = 0;
	LED_RUN1 = 1;
}

void led_TurnOff(void)
{
	LED_RUN = 1;
	LED_RUN1 = 0;
}

void led_unint() {
	
	LED_RUN = 1;
	LED_RUN1 = 1;
	
}