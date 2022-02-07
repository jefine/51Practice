#include "tim.h"
#include "seg.h" 
#include "key.h"
#include "ds1302.h"
#include "stdio.h"
//还有点bug，按下和抬起都会显示一下
void delay50ms(void);   //误差 0us
void Led_Init(void); 
unsigned int uiSeg_Dly; 
unsigned char ucSec,ucLed,ucState = 1;
unsigned char ucKey_Dly,ucKey_Old;
unsigned char pucSeg_Buf[9],pucSeg_Code[8],ucSeg_Pos;
unsigned char pucRtc[] = {23,59,50};
unsigned long ulms=0,ulKey_Time; 

void Key_Proc(void);
void Seg_Proc(void);
void main(void)
{
  
  Led_Init();
  Time_init();
  Set_RTC(pucRtc);
  while(1)
	{	
    Key_Proc();
    Seg_Proc();
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
  unsigned char ucKey_Val, ucKey_Down;
  if(ucKey_Dly) return;
  ucKey_Dly=1;

  ucKey_Val = Key_Read();
  ucKey_Down = ucKey_Val&(ucKey_Old ^ ucKey_Val);
  ucKey_Old = ucKey_Val;

  if(ucKey_Down == 4)
  {
    if(++ucState == 2)
      ucState = 0;
  }
  P0 = ucState+1;
  P2 = P2 & 0x1f | 0x80;//p27-p25先清零，在定位y4c
  P2 = P2 & 0x1f;//再清零

}
void Seg_Proc(void)
{
  if(ucKey_Dly) return;
  ucKey_Dly = 1;
  switch(ucState)
  {
    case 0:
          sprintf(pucSeg_Buf,"1 %06u",(unsigned int)ucSec);
          break;
    case 1:
          Read_RTC(pucRtc);
          sprintf(pucSeg_Buf,"2 %02d%02d%02d",(unsigned int)pucRtc[0],
          (unsigned int)pucRtc[1],(unsigned int)pucRtc[2]);
  }
  Seg_Tran(pucSeg_Buf,pucSeg_Code);
  
}
void time_1(void) interrupt 3
{
  ulms++;
  if(++ucKey_Dly == 10)
  {
    ucKey_Dly = 0;
  }
  if(++uiSeg_Dly == 500)
      uiSeg_Dly = 0;
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
