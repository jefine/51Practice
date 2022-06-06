#include "reg52.h" 
void delay50ms(void);   //��� 0us
void Led_Init(void);
int times;
unsigned char code t_display[]={                       //��׼�ֿ�
//   0    1    2    3    4    5    6    7    8    9    A    B    C    D    E    F
    0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F,0x77,0x7C,0x39,0x5E,0x79,0x71,
//black  -     H    J    K    L    N    o   P    U     t    G    Q    r   M    y
    0x00,0x40,0x76,0x1E,0x70,0x38,0x37,0x5C,0x73,0x3E,0x78,0x3d,0x67,0x50,0x37,0x6e,
    0xBF,0x86,0xDB,0xCF,0xE6,0xED,0xFD,0x87,0xFF,0xEF,0x46};    //0. 1. 2. 3. 4. 5. 6. 7. 8. 9. -1

unsigned char code T_COM[]={0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80};      //λ��

/*
38��������ͨ��P2.5��P2.6��P2.7�������Ž������룬�����Ͽ��Կ��ư��� ����
���������˲�ͬ�Ĺ��ܣ����ı䲻ͬ�Ĵ������ֵ��
�����ĸ��Ĵ���������˵���Ա����ĸ���ͬ��p0��״̬�������Ʋ�ͬ�Ĺ���

P2 = 0xc0;//�������λ Ȼ�� �Ž�ȥһ��ѡ��λ��ֵ�����ͻᱣ�浽һ���Ĵ�������
Ȼ���ڴ�һ���Ĵ����Ŀ��أ����ͬʱ�������ľͱ��ر��ˣ�
P2 = 0xe0;//������ܶ�ѡ������
�����ͷֱ���ȥ��λ�ͶΣ�������ֵ ��������P0 ֱ�Ӹ��ģ������� ����ԵļĴ��� ���и�ֵ��

*/
// a simple light led example 
int main()
{
  //in old compiler, such as keil.
  //you should put the variable at the beginning of the function.
  unsigned int x;
  x=0;


  // 0xfd = 1111 1101
  // P2 = 0x80;
  // P0 = 0xfd;
  // P2 = 0;

  // for function delay n*50 ms()
  // for (i = 0; i < 20; i++)
  // {
  //   delay50ms();
  // }

  //0xe0 = 1110 0000
  //0xc0 = 1100 0000

  Led_Init();
  //close the led
  while(1)
  {
    P0 = T_COM[x];
    P2 = 0xc0;
    P2 = 0;
    P0 = ~t_display[x];
    P2 = 0xe0;
    P2 = 0;
    x = (x+1)%6;
     delay50ms();
  }
}

void Led_Init(void)
{
  //0x1f = 0001 1111
  //0x80 = 1000 0000 
  P0 = 0xf8;
  P2 = P2 & 0x1f | 0x80;//p27-p25�����㣬�ڶ�λy4c
  P2 = P2 & 0x1f;//������
  //P2 = 0;
  times =10;
  // while (times--)
  // {
  //   delay50ms();
  // }
}
void delay50ms(void)   //��� 0us
{
    unsigned char a,b;
    for(b=165;b>0;b--);
}

