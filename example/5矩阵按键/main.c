#include "tim.h"
#include "seg.h" 
#include "key.h"
#include "stdio.h"
//还有点bug，按下和抬起都会显示一下
void delay50ms(void);   //误差 0us
void Led_Init(void);  
unsigned char ucSec,ucLed; //秒，灯变量
unsigned char ucKey_Dly,ucKey_Old;
unsigned char pucSeg_Buf[9],pucSeg_Code[8],ucSeg_Pos;
unsigned long ulms=0,ulKey_Time; 

void Key_Proc(void);
void SEG_Proc(unsigned char ucSeg_Val);
void main(void)
{
  
  Led_Init();
  Time_init();
  while(1)
	{	
    Key_Proc();
    Seg_Tran(pucSeg_Buf,pucSeg_Code);
	}		
}
void Led_Init(void)
{
  P0 = 0xff;
  P2 = P2 & 0x1f | 0x80;//p27-p25先清零，在定位y4c
  P2 = P2 & 0x1f;//再清零
  delay50ms(); 
  P0 = 0xf0;
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

void Key_Proc(void)
{
  unsigned char ucKey_Val, ucKey_Down, ucKey_Up;
  if(ucKey_Dly) return;
  ucKey_Dly=1;

  ucKey_Val = Key_Read();

  ucKey_Down = ucKey_Val&(ucKey_Old ^ ucKey_Val);
  ucKey_Up = ~ucKey_Val&(ucKey_Old ^ ucKey_Val);
  ucKey_Old = ucKey_Val;

  if(ucKey_Down)
  {
    ulKey_Time = ulms;
    SEG_Proc(ucKey_Down);
  }
  if(ucKey_Up)
    SEG_Proc(ucKey_Up);

  if(ucKey_Old&&(ulms - ulKey_Time > 1000))
  {
    ulKey_Time = ulms;
    SEG_Proc(ucKey_Old) ;
  }

}
void SEG_Proc(unsigned char ucSeg_Val)
{
  unsigned char i;
  for(i=0; i<7; i++)
  {
    pucSeg_Buf[i] = pucSeg_Buf[i+1];
  }

  if(ucSeg_Val < 14)
    pucSeg_Buf[i] = ucSeg_Val-4+'0';
  else
    pucSeg_Buf[i] = ucSeg_Val-14+'A';
}
void time_1(void) interrupt 3
{
  ulms++;
  if(++ucKey_Dly == 10)
  {
    ucKey_Dly = 0;
  }
  if(ulms == 1000)
  {
    ulms = 0;
    ucSec++;
    P0 = ucSec;
    P2 = P2 & 0x1f | 0x80;//p27-p25先清零，在定位y4c
    P2 = P2 & 0x1f;//再清零
  }
  Seg_Disp(pucSeg_Code, ucSeg_Pos);
  if(++ucSeg_Pos == 8) ucSeg_Pos = 0;
}
