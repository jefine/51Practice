#include "tim.h"
#include "seg.h" 
#include "uart.h"
#include "stdio.h"
void delay50ms(void);   //误差 0us
void Led_Init(void);
 
unsigned char ucSec=0,ucSec1,ucLed,miao;
unsigned char pucSeg_Buf[11],pucSeg_Code[8],ucSeg_Pos; //改为11了
unsigned char pucUart_Buf[2],ucUart_Num;
unsigned long ulms; 

void Uart_Proc(void);

void main()
{
  Led_Init();
  Time_init();
  UartInit();
  while(1)
	{	
    sprintf(pucSeg_Buf, "%08u\r\n",(unsigned int)ucSec);
    Seg_Tran(pucSeg_Buf,pucSeg_Code);
    Uart_Proc();
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
void Uart_Proc(void)
{
  if(ucSec1 != ucSec)
  {
    ucSec1 = ucSec;
    Uart_SendString(pucSeg_Buf);
  }
  if(ucUart_Num == 2)
  {
    ucSec = ((pucUart_Buf[0]-'0')*10) + pucUart_Buf[1]-'0';
    ucUart_Num = 0;
  } 
}
void uart_0(void) interrupt 4
{
  if(RI)
  {
    pucUart_Buf[ucUart_Num++] = SBUF;
    RI = 0;
  }
}
void time_1(void) interrupt 3
{
  //注意变量的最大值，最开始用unsigned char 最大值到不了1000，哭死
  ulms++;
  if(ulms == 1000)
  {
    ulms = 0;
    ucSec++;
    miao++;
    P0 = miao;
    P2 = P2 & 0x1f | 0x80;//p27-p25先清零，在定位y4c
    P2 = P2 & 0x1f;//再清零
  }
  Seg_Disp(pucSeg_Code, ucSeg_Pos);
  if(++ucSeg_Pos == 8) 
    ucSeg_Pos = 0;
}
