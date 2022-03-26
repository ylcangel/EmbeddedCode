// author: AngelToms

#include "global.h"

// one master one slave mode
// W25Q32X驱动，该flash是32mbits，即4m字节
// 本驱动 spi flash仅作为一个从设备，而且是一主一丛
// 这里并没有实现所有指令，仅仅用于测试标准spi传输情景

#define SPI_ENABLE spi_enable() // 仅仅是把CS拉低电平
#define SPI_DISABLE spi_disable() // 把CS拉高电平

#define PAGE_MAX_BYTES 256
#define SECTOR_SIZE 0x1000 // 4k
#define BLOCK_SIZE_64K 0x10000 // 64k
#define BLOCK_SIZE_32K 0x8000 // 32k
#define SECTOR_SIZE_PER_BLOCK 16
#define MAX_BLOCK_64K_NR 64
#define MAX_BLOCK_32K_NR (MAX_BLOCK_64K_NR * 2)
#define MAX_SECTOR_SIZE (SECTOR_SIZE_PER_BLOCK * SECTOR_SIZE_PER_BLOCK) // 1024

#define DUMMY_DATA 0x0
// inst, format {inst, data}, every inst 1 byte length
#define WRITE_ENABLE 0x6 //  1
#define WRITE_DISABLE 0x4 // 1
#define READ_STATUS_REGISTER1 0x5 // 2
#define READ_STATUS_REGISTER2 0x35 // 2
#define WRITE_STATUS_REGISTER 0x1 // 3
#define PAGE_PROGARAM 0x2 // 5
#define QUAD_PAGE_PROGRAM 0x32 // 5
#define BLOCK_ERASE_64K 0xd8 // 4
#define BLOCK_ERASE_32K 0x52 // 4
#define SECTOR_ERASE 0x20 // 4
#define CHIP_ERASE 0xc7 // 1
#define CHIP_ERASE1 0x60 // eq CHIP_ERASE
#define ERASE_SUSPEND 0x75 // 1
#define ERASE_RESUME 0x7a // 1
#define POWER_DONW 0xb9 // 1
#define HIGH_PERFORMANCE_MODE 0xa3 // 4
#define MODE_BIT_RESET 0xff // 2, byte2 was always 0xff
#define RELEASE_POWER_DOWN_OR_HPM_DEVICE_ID 0xab // 5
#define MANUFACTURER_DEVICE_ID 0x90 // 6
#define READ_UNIQUE_ID 0x4b // 6
#define JEDEC_ID 0x9f // 4
#define READ_DATA 0x3 //  5
#define FAST_READ 0xb // 6
#define FAST_READ_DUAL_OUTPUT 0x3b // 6
#define FAST_READ_DUAL_IO 0xbb // 4
#define FAST_READ_QUAD_OUTPUT 0x6b // 6
#define FAST_READ_QUAD_IO 0xeb // 4

#define IS_BUSY(S_Register1) (S_Register1 & 0x1)

static void write_enable() {
	SPI_ENABLE;
	spi_rw(WRITE_ENABLE);
	SPI_DISABLE;
}

static void write_disable() {
	SPI_ENABLE;
	spi_rw(WRITE_DISABLE);
	SPI_DISABLE;
}

extern u1 read_SR1() {
	u1 sr;
	SPI_ENABLE;
	spi_rw(READ_STATUS_REGISTER1);
	sr = spi_rw(DUMMY_DATA);
	SPI_DISABLE;
	return sr;
}

extern u1 read_SR2() {
	u1 sr;
	SPI_ENABLE;
	spi_rw(READ_STATUS_REGISTER2);
	sr = spi_rw(DUMMY_DATA);
	SPI_DISABLE;
	return sr;
}


extern void write_SR(u2 val) {
	u1 low = val & 0xf;
	u1 high = (val >> 8) & 0xf;
	write_enable();
	SPI_ENABLE;
	spi_rw(WRITE_STATUS_REGISTER);
	spi_rw(high); // MSB 
	spi_rw(low); //
	SPI_DISABLE;
}

extern void read(u1* pbuf, u4 len, u4 addr) {
	SPI_ENABLE;
	spi_rw(READ_DATA);
	spi_rw((u1)(addr >> 16));
	spi_rw((u1)(addr >> 8));
	spi_rw((u1)(addr));
	while(len--) {
		*pbuf = spi_rw(DUMMY_DATA);
		pbuf++;
	}
	SPI_DISABLE;
}

extern void fast_read(u1* pbuf, u4 len, u4 addr) {
	SPI_ENABLE;
	spi_rw(FAST_READ);
	spi_rw((u1)(addr >> 16));
	spi_rw((u1)(addr >> 8));
	spi_rw((u1)(addr));
	spi_rw(DUMMY_DATA);
	while(len--) {
		*pbuf = spi_rw(DUMMY_DATA);
		pbuf++;
	}
	SPI_DISABLE;
}

extern void page_write(u1* pbuf, u4 len, u4 addr) {
	if (len > PAGE_MAX_BYTES) {
		printf("write size over the limit");
		return;
	}
	
	if ( len == PAGE_MAX_BYTES) {
		addr &= 0xffff00;
	}
	
	write_enable();
	
	SPI_ENABLE;
	spi_rw(PAGE_PROGARAM);
	spi_rw((u1)(addr >> 16));
	spi_rw((u1)(addr >> 8));
	spi_rw((u1)(addr));

	while(len--) {
		spi_rw(*pbuf);
		pbuf++;
	}
	SPI_DISABLE;
	
	while(IS_BUSY(read_SR1()));
}

extern void sector_earse(u4 sectorNr) {
	u4 addr = 0;
	if (sectorNr == 0) {
		sectorNr = 1;
	}
	
	sectorNr = sectorNr -1;
	
	if (sectorNr > MAX_SECTOR_SIZE) {
		printf("bad sector nr");
		return;
	}
	
	addr = (sectorNr * SECTOR_SIZE) & 0x3ff000;
	
	write_enable();
	while(IS_BUSY(read_SR1()));
		
	SPI_ENABLE;
	spi_rw(SECTOR_ERASE);
	spi_rw((u1)(addr >> 16));
	spi_rw((u1)(addr >> 8));
	spi_rw((u1)(addr));
	
	SPI_DISABLE;
	while(IS_BUSY(read_SR1()));
	
}

extern void block_earse(boolean is32k, int blockNr) {
	u4 addr = 0;
	if (blockNr == 0) {
		blockNr =1;
	}
	blockNr = blockNr -1;
	
	if (!is32k) {
		if (blockNr > MAX_BLOCK_64K_NR) {
			printf("bad block nr with 64k");
			return;
		}
		
		addr = (blockNr * BLOCK_SIZE_64K) & 0x3f0000;
	}
	
	if (blockNr > BLOCK_ERASE_32K) {
			printf("bad block nr with 32k");
			return;
	}
	
	if (is32k) {
		addr = (blockNr * BLOCK_SIZE_32K) & 0x7f8000;
	}
	
	write_enable();
	while(IS_BUSY(read_SR1()));
	
	SPI_ENABLE;
	if (is32k) {
		spi_rw(BLOCK_ERASE_32K);
	} else {
		spi_rw(BLOCK_ERASE_64K);
	}
	
	spi_rw((u1)(addr >> 16));
	spi_rw((u1)(addr >> 8));
	spi_rw((u1)(addr));
	
	SPI_DISABLE;
	while(IS_BUSY(read_SR1()));
}

extern void chip_earse() {
	write_enable();
	while(IS_BUSY(read_SR1()));
	SPI_ENABLE;
	spi_rw(CHIP_ERASE);
	SPI_DISABLE;
	while(IS_BUSY(read_SR1()));
}

extern void read_device_id() {
	SPI_ENABLE;
	spi_rw(MANUFACTURER_DEVICE_ID);
	spi_rw(DUMMY_DATA);
	spi_rw(DUMMY_DATA);
	spi_rw(0);
	
	printf("MANUFACTURER ID: %x\r\n", spi_rw(DUMMY_DATA));
	printf("DEVICE ID: %x\r\n", spi_rw(DUMMY_DATA));
	SPI_DISABLE;
}

