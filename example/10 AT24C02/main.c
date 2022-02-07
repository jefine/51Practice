#include "tim.h"
#include "seg.h" 
#include "stdio.h"
#include "iic.h"
void delay50ms(void);   //误差 0us
void Led_Init(void);
int a=0;
int b=0;  
unsigned char ucSec,ucLed;
unsigned char pucSeg_Buf[9],pucSeg_Code[8],ucSeg_Pos;
unsigned long ulms; 
void main()
{
  unsigned char a[]="456";
  unsigned char b[4]= "000";
  Led_Init();
  Time_init();  
  EEPROM_Write(a,0,3);
  delay50ms();
   EEPROM_Read(b,0,3); 
  while(1)
	{	
      
       
    sprintf(pucSeg_Buf,"%s",b);
    //Seg_Tran(b,pucSeg_Code);
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
void time_1(void) interrupt 3
{
  //注意变量的最大值，最开始用unsigned char 最大值到不了1000，哭死
  a++;
  if(a == 1000)
  {
    a = 0;
    ucSec++;
    P0 = b;
    P2 = P2 & 0x1f | 0x80;//p27-p25先清零，在定位y4c
    P2 = P2 & 0x1f;//再清零
    b++;
    if(b ==0xff)
      b=0;
  }
  Seg_Disp(pucSeg_Code, ucSeg_Pos);
  if(++ucSeg_Pos == 8) 
    ucSeg_Pos = 0;
}
