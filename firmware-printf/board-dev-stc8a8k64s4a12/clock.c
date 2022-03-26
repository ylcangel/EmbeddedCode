#include "global.h"

#define PSW2_MASK 0x80

#define CLK_RESET 0x0
#define CLKDIV_RESET 0x4

#define CLK_MASK_P54 0x10
#define CLK_MASK_P16 0x18
#define CLK_DIV_MASK 0x0

#define ENABLE_IRC24MCR_MASK 0x80
#define TEST_IRC24MCR_STATUS(n) \
	(n & 0x1)

void enable_xdata() {
	P_SW2 |= PSW2_MASK; // document shows P_SW2 = 80
}
void disable_xdata() {
	P_SW2 &= ~PSW2_MASK; // document shows P_SW2 = 0
}

void enable_irc24mcr() {
	CKSEL = CLK_RESET; // document shows the regs(CKSEL, CLKDIV) didn't reset
	CLKDIV = CLKDIV_RESET;
	
	CKSEL |= CLK_MASK_P54;
	CLKDIV = CLK_DIV_MASK;
	
	IRC32KCR &= 0;
	IRC32KCR = ENABLE_IRC24MCR_MASK;
}

void check_irc24mcr_status() {
	unsigned char volatile xdata status = IRC32KCR;
	while (!TEST_IRC24MCR_STATUS(status));
	return;
}

void enable_nest_clock() {
	enable_xdata();
	enable_irc24mcr();
	check_irc24mcr_status();
	
}