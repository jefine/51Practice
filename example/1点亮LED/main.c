#include "reg52.h" 
void delay50ms(void);   //��� 0us
void Led_Init(void);
  
void main()
{
  
  while(1)
	{	
    Led_Init();
	}		
}
void Led_Init(void)
{
  P0 = 0xff;
  P2 = P2 & 0x1f | 0x80;//p27-p25�����㣬�ڶ�λy4c
  P2 = P2 & 0x1f;//������
  delay50ms();
  delay50ms();
  delay50ms();
  
  delay50ms();
  P0 = 0x00;
  P2 = P2 & 0x1f | 0x80;//p27-p25�����㣬�ڶ�λy4c
  P2 = P2 & 0x1f;//������
  delay50ms(); 
}
void delay50ms(void)   //��� 0us
{
    unsigned char a,b;
    for(b=165;b>0;b--);
}
