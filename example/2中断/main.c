#include "reg52.h" 
void delay50ms(void);   //��� 0us
void Led_Init(void);    //����ʾ
void Int_Init(void);    //�жϳ�ʼ��
  
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
    EX0 = 1; //�ⲿ�ж�0����
    IT0 = 1; //�½��ش���
    EX1 = 1;//�ⲿ�ж�1����
    IT1 = 1; //�½��ش���
    EA = 1;//���ܿ���
}
void Led_Init(void)
{
  P0 = 0xff;
  P2 = P2 & 0x1f | 0x80;//p27-p25�����㣬�ڶ�λy4c
  P2 = P2 & 0x1f;//������
  delay50ms(); 
  P0 = 0x00;
  P2 = P2 & 0x1f | 0x80;//p27-p25�����㣬�ڶ�λy4c
  P2 = P2 & 0x1f;//������
  delay50ms(); 
}
void delay50ms(void)   //��� 0us
{
    unsigned char a,b;
    for(b=165;b>0;b--)
        for(a=150;a>0;a--);
}
void int0(void) interrupt 0
{
    P0 = ~P0;
    P2 = P2 & 0x1f | 0x80;//p27-p25�����㣬�ڶ�λy4c
    P2 = P2 & 0x1f;//������
}
void int1(void) interrupt 2
{
    P0 = ~P0|0x01;
    P2 = P2 & 0x1f | 0x80;//p27-p25�����㣬�ڶ�λy4c
    P2 = P2 & 0x1f;//������
}
