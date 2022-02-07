#include "tim.h" 
void Time0_init(void)		//1毫秒@12.000MHz
{
	AUXR &= 0x7F;		//定时器时钟12T模式
	TMOD |= 0x05;		//注意！！设置计算器模式
	TL0 = 0x00;		//设置定时初值
	TH0 = 0x00;		//设置定时初值
	TF0 = 0;		//清除TF0标志
	TR0 = 1;		//定时器0开始计时
}

void Time1_init(void)  //12M下1ms
{
  AUXR &= 0xBF; //定时器1时钟12t
  TMOD &= 0x0F; //设置定时器模式使用模式0，16为重装
  //TL1 = 0x18;
  //TH1 = 0xfc;
  TH1 = (65535-1000)/256;
  TL1 = (65535-1000)%256;
  TF1 = 0;  //清除tf1标志
  TR1 = 1;  //定时器1开始计时
  ET1 =1;
  EA = 1;
}