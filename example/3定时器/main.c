#include "tim.h" 
void delay50ms(void);   //误差 0us
void Led_Init(void);
int a=0;
int b=0;  
 
void main()
{
  
  Led_Init();
  Time_init();
  while(1)
	{	
    ;
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
    
    P0 = b;
    P2 = P2 & 0x1f | 0x80;//p27-p25先清零，在定位y4c
    P2 = P2 & 0x1f;//再清零
    
    b++;
    if(b ==0xff)
      b=0;
  }
}
