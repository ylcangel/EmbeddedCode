#include "global.h"

#define SPI_CTR_MASK 0xdf
#define SPI_CLEAR_SPSTAT 0xc0
#define SPI_P345 0x04
sbit SPI_CS = P7^2;

// stc8a has spi bus fabric
// 4 groups spi 
// we use p2.3 2.4 2.5
//P_SW1 = 0x04; //SS_2/P2.2, MOSI_2/P2.3, MISO_2/P2.4, SCLK_2/P2.5
extern void spi_init(u1 speed) {
	speed &= 0x3;
	P_SW1 = SPI_P345; // spi [p2.3 2.4 2.5]
	SPCTL = SPI_CTR_MASK | speed;
	SPSTAT = SPI_CLEAR_SPSTAT; // clear SPIF WCOL flags
}

extern void spi_enable() {
	SPI_CS = 0;
}

extern void spi_disable() {
	SPI_CS = 1;
}

extern u1 spi_rw(u1 val) { // read and write
	SPDAT = val;
	while((SPSTAT & 0x80) == 0);
	SPSTAT = 0x80; // must set
	return SPDAT;
}

