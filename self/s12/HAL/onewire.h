#ifndef __ONEWIRE_H
#define __ONEWIRE_H

#include "STC15F2K60S2.h"



sbit DQ = P1^4;  

unsigned int read_temperature(void);  
void Delay_OneWire(unsigned int t);
unsigned char Read_DS18B20(void);
void Write_DS18B20(unsigned char dat);
bit init_ds18b20(void);
#endif
