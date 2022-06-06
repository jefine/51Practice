#include "STC15F2K60S2.h"
#include "INTRINS.H"
#include <onewire.h>
int temp =0;
int timer0_cnt =0;
bit flag = 0;
int temp_smg[8]=0;
int x=0;
unsigned char  t_display[]={                       //标准字库
//   0    1    2    3    4    5    6    7    8    9    A    B    C    D    E    F
    0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F,0x77,0x7C,0x39,0x5E,0x79,0x71,
//black  -     H    J    K    L    N    o   P    U     t    G    Q    r   M    y
    0x00,0x40,0x76,0x1E,0x70,0x38,0x37,0x5C,0x73,0x3E,0x78,0x3d,0x67,0x50,0x37,0x6e,
    0xBF,0x86,0xDB,0xCF,0xE6,0xED,0xFD,0x87,0xFF,0xEF,0x46};    //0. 1. 2. 3. 4. 5. 6. 7. 8. 9. -1

unsigned char T_COM[]={0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80};      //位码

void Delay1000ms()		//@12.000MHz
{
	unsigned char i, j, k;

	_nop_();
	_nop_();
	i = 46;
	j = 153;
	k = 245;
	do
	{
		do
		{
			while (--k);
		} while (--j);
	} while (--i);
}
void Timer0Init(void)		//1毫秒@12.000MHz
{
	AUXR |= 0x80;		//定时器时钟1T模式
	TMOD &= 0xF0;		//设置定时器模式
	TL0 = 0x20;		//设置定时初始值
	TH0 = 0xD1;		//设置定时初始值
	TF0 = 0;		//清除TF0标志
	TR0 = 1;		//定时器0开始计时
	ET0 = 1;
	EA = 1;
}

unsigned int read_temperature(){
	// float temperature;
	// unsigned int temp;
	// unsigned char low,high;
	
	// init_ds18b20();    //初始化
	// Write_DS18B20(0xcc);    //写0xcc
	// Write_DS18B20(0x44);    //写0x44
	// Delay_OneWire(200);    //延时
	
	// init_ds18b20();    //初始化
	// Write_DS18B20(0xcc);    //写0xcc
	// Write_DS18B20(0xbe);    //写0xbe
	
	// low = Read_DS18B20();    //读低位
	// high = Read_DS18B20();    //读高位
	// temp = high << 8 | low;    //高低位连接在一起
	// //temp = high *256 + low;
	// temperature = temp * 0.0625;
	// temp = temperature * 100;
	// return temp;

	float temper;
	unsigned char high,low;
	unsigned int temp;
	
	init_ds18b20();
	Write_DS18B20(0xcc);
	Write_DS18B20(0x44);
	Delay_OneWire(200);

	init_ds18b20();
	Write_DS18B20(0xcc);
	Write_DS18B20(0xbe);
	

	low = Read_DS18B20();
	high = Read_DS18B20();
	temp = high<<8 | low;
	temper =temp * 0.0625;
	return temper * 100;
}

void display_temp_smg(){
    //2541->25.41
    P0 = T_COM[x];
    P2 = 0xc0;
    P2 = 0;
    if(x==1)
    P0 = ~(t_display[temp_smg[x]+32])  ;
    else
    P0 = ~t_display[temp_smg[x]];
    P2 = 0xe0;
    P2 = 0;
    x ++;
    if(x == 8){
      x = 0;
    }
}
void Timer0Handle() interrupt 1
{
  //  if(timer0_cnt++<10)return;
  //  timer0_cnt = 0;
  display_temp_smg();
}
void solve_temp_smg(int temper)
{
  temp_smg[0] = temper/1000;
  temp_smg[1] = temper/100%10;
  temp_smg[2] = temper/10%10;
  temp_smg[3] = temper%10;
}
int main()
{
  Timer0Init();
  while(1)
  {
    temp = read_temperature();
    solve_temp_smg(temp);
	Delay1000ms();
  }
}
