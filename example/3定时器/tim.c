#include "tim.h" 
void Time_init(void)  //12M下1ms
{
  AUXR &= 0xBF; //定时器1时钟12t
  TMOD &= 0x0F; //设置定时器模式使用模式0，16为重装
  //TL1 = 0x18;
  //TH1 = 0xfc;
  TH1 = (65535-1000)/256;
  TL1 = (65535-1000)%256;
  TF1 = 0;  //清除tf1标志
  TR1 = 1;  //定时器1开始计时

  //下面为自己加的代码
  ET1 =1;     
  EA = 1;
}