#include "reg52.h" 
void delay50ms(void);   //误差 0us
void Led_Init(void);
int times;
unsigned char code t_display[]={                       //标准字库
//   0    1    2    3    4    5    6    7    8    9    A    B    C    D    E    F
    0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F,0x77,0x7C,0x39,0x5E,0x79,0x71,
//black  -     H    J    K    L    N    o   P    U     t    G    Q    r   M    y
    0x00,0x40,0x76,0x1E,0x70,0x38,0x37,0x5C,0x73,0x3E,0x78,0x3d,0x67,0x50,0x37,0x6e,
    0xBF,0x86,0xDB,0xCF,0xE6,0xED,0xFD,0x87,0xFF,0xEF,0x46};    //0. 1. 2. 3. 4. 5. 6. 7. 8. 9. -1

unsigned char code T_COM[]={0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80};      //位码

/*
38译码器，通过P2.5，P2.6，P2.7三个引脚进行译码，理论上可以控制八种 功能
译码器打开了不同的功能，来改变不同寄存器里的值。
他有四个寄存器，可以说可以保留四个不同的p0的状态，来控制不同的功能

P2 = 0xc0;//开数码管位 然后 放进去一个选管位的值，他就会保存到一个寄存器里面
然后在打开一个寄存器的开关（与此同时，其他的就被关闭了）
P2 = 0xe0;//开数码管段选锁存器
这样就分别存进去了位和段，他俩的值 并不是由P0 直接给的，而是由 其各自的寄存器 进行赋值的

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
  P2 = P2 & 0x1f | 0x80;//p27-p25先清零，在定位y4c
  P2 = P2 & 0x1f;//再清零
  //P2 = 0;
  times =10;
  // while (times--)
  // {
  //   delay50ms();
  // }
}
void delay50ms(void)   //误差 0us
{
    unsigned char a,b;
    for(b=165;b>0;b--);
}

