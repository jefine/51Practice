#include "stc15f2k60s2.h" 
int cnt = 0;
// 数码管 8毫秒扫描一遍
// 每毫秒 点亮一个数码管，这样才能保持视觉延迟。
//（如果放在一起去操作，那么除了最后一个，其他的都会亮度极低）

bit busy;
unsigned char uart_cnt=0;
unsigned char uart_buf[8]={0};

unsigned char  t_display[]={                       //标准字库
//   0    1    2    3    4    5    6    7    8    9    A    B    C    D    E    F
    0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F,0x77,0x7C,0x39,0x5E,0x79,0x71,
//black  -     H    J    K    L    N    o   P    U     t    G    Q    r   M    y
    0x00,0x40,0x76,0x1E,0x70,0x38,0x37,0x5C,0x73,0x3E,0x78,0x3d,0x67,0x50,0x37,0x6e,
    0xBF,0x86,0xDB,0xCF,0xE6,0xED,0xFD,0x87,0xFF,0xEF,0x46};    //0. 1. 2. 3. 4. 5. 6. 7. 8. 9. -1

unsigned char T_COM[]={0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80};      //位码


void Timer0Init(void)		//1毫秒@12.000MHz
{
	AUXR |= 0x80;		//定时器时钟1T模式
	TMOD &= 0xF0;		//设置定时器模式
	TL0 = 0x20;		//设置定时初始值
	TH0 = 0xD1;		//设置定时初始值
	TF0 = 0;		//清除TF0标志
	TR0 = 1;		//定时器0开始计时
  ET0 = 1;
  EA = 1;
}
void Timer0Handle() interrupt 1
{
  P0 = T_COM[cnt];
  P2 = 0xc0;
  P2 = 0;
  P0 = ~t_display[cnt];
  P2 = 0xe0;
  P2 = 0;
  cnt++;
  if(cnt==8)cnt =0;
}
void UartInit(void)		//4800bps@12.000MHz
{
	SCON = 0x50;		//8位数据,可变波特率
	AUXR |= 0x40;		//定时器1时钟为Fosc,即1T
	AUXR &= 0xFE;		//串口1选择定时器1为波特率发生器
	TMOD &= 0x0F;		//设定定时器1为16位自动重装方式
	TL1 = 0x8F;		//设定定时初值
	TH1 = 0xFD;		//设定定时初值
	ET1 = 0;		//禁止定时器1中断
	TR1 = 1;		//启动定时器1
  ES = 1;
  EA = 1;
}
void uart_Rec_handle()
{
  
}
void Uart() interrupt 4
{
    if (RI)
    {
        RI = 0;                 //清除RI位
        uart_buf[uart_cnt++] = SBUF;              //P0显示串口数据
        if(uart_cnt == 8){
          uart_cnt = 0;
          uart_Rec_handle();
        }
    }
    if (TI)
    {
        TI = 0;                 //清除TI位
        busy = 0;               //清忙标志
    }
}
void SendData(unsigned char dat)
{
    while (busy);               //等待前面的数据发送完成
    busy = 1;
    SBUF = dat;                 //写数据到UART数据寄存器
}

void SendString(char *s)
{
    while (*s)                  //检测字符串结束标志
    {
        SendData(*s++);         //发送当前字符
    }
}

void main()
{
  unsigned char msg[]="123\n";
  UartInit();
  Timer0Init();
  while(1)
  {
    SendString(msg);
  }
}
