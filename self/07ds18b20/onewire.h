#ifndef __ONEWIRE_H
#define __ONEWIRE_H
#include "STC15F2K60S2.h"
#include "INTRINS.H"
unsigned char rd_temperature(void);  //; ;
bit init_ds18b20();
unsigned char Read_DS18B20();
void Delay_OneWire(unsigned int t);
void Write_DS18B20(unsigned char dat);
#endif
