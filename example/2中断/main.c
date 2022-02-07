#include "reg52.h" 
void delay50ms(void);   //误差 0us
void Led_Init(void);    //灯显示
void Int_Init(void);    //中断初始化
  
void main()
{
  Int_Init();
  while(1)
	{	
      ;
	}		
}
void Int_Init(void)
{
    EX0 = 1; //外部中断0允许
    IT0 = 1; //下降沿触发
    EX1 = 1;//外部中断1允许
    IT1 = 1; //下降沿触发
    EA = 1;//打开总开关
}
void Led_Init(void)
{
  P0 = 0xff;
  P2 = P2 & 0x1f | 0x80;//p27-p25先清零，在定位y4c
  P2 = P2 & 0x1f;//再清零
  delay50ms(); 
  P0 = 0x00;
  P2 = P2 & 0x1f | 0x80;//p27-p25先清零，在定位y4c
  P2 = P2 & 0x1f;//再清零
  delay50ms(); 
}
void delay50ms(void)   //误差 0us
{
    unsigned char a,b;
    for(b=165;b>0;b--)
        for(a=150;a>0;a--);
}
void int0(void) interrupt 0
{
    P0 = ~P0;
    P2 = P2 & 0x1f | 0x80;//p27-p25先清零，在定位y4c
    P2 = P2 & 0x1f;//再清零
}
void int1(void) interrupt 2
{
    P0 = ~P0|0x01;
    P2 = P2 & 0x1f | 0x80;//p27-p25先清零，在定位y4c
    P2 = P2 & 0x1f;//再清零
}
