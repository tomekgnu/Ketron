/*
 * sounds.h
 *
 * Created: 16.03.2019 23:51:45
 *  Author: Tomek
 */ 


#ifndef SPI_FLASH_H_
#define SPI_FLASH_H_
#include <main.h>

void sFLASH_ReadBuffer(uint8_t *dst,uint32_t addr,uint32_t size);
void sFLASH_WriteBuffer(uint8_t *src,uint32_t addr,uint32_t size);
uint32_t sFLASH_Erase(uint32_t addr, uint32_t size);

#endif /* SOUNDS_H_ */