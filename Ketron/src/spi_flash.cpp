#include <spi_flash.h>
#include <DataFlash.h>
#include <SPI.h>

extern DataFlash dataflash;

void sFLASH_ReadBuffer(uint8_t *dst,uint32_t addr,uint32_t size){
	uint16_t page = addr / DF_45DB161_PAGESIZE;
	uint16_t offset = addr % DF_45DB161_PAGESIZE;
	dataflash.arrayRead(page,offset);
	for(uint16_t i = 0; i < size; i++)
		dst[i] = SPI.transfer(0xff);
}
void sFLASH_WriteBuffer(uint8_t *src,uint32_t addr,uint32_t size){
	uint16_t index = 0;
	uint16_t startpage = addr / DF_45DB161_PAGESIZE;
	uint16_t totalpages = (size / DF_45DB161_PAGESIZE) + 1;
		
	for(uint16_t page = 0; page < totalpages; page++,startpage++){
		uint16_t offset = addr % DF_45DB161_PAGESIZE;
		dataflash.pageToBuffer(startpage,0);		
		dataflash.bufferWrite(0,offset);
		for(uint16_t i = offset; i < DF_45DB161_PAGESIZE; i++){
			SPI.transfer(src[index]);
			addr++;
			index++;
		}
		dataflash.bufferToPage(0,startpage);
	}
}

uint32_t sFLASH_Erase(uint32_t addr, uint32_t size){
	uint16_t startpage = addr / DF_45DB161_PAGESIZE;
	uint16_t totalpages = (size / DF_45DB161_PAGESIZE) + 1;
	/* Invalid input */
	if (addr + size > DF_45DB161_SIZE)
		return 1;
	/* Not aligned to page */
	if (addr % DF_45DB161_PAGESIZE)
		return 1;
		
	/* Largest unit is page */
	for(uint16_t page = 0; page < totalpages; page++,startpage++) {
		dataflash.pageErase(startpage);
		startpage++;
	}
	return 0;
}