#include "STC15F2K60S2.h" 
unsigned int cnt = 0;
unsigned int state = 0;

unsigned char trg = 0;
unsigned char cont = 0;
unsigned char key_value;
//判断trg的值，4个按键值分别为0x01 0x02 0x04 0x08
void read_key_simple(){
	unsigned char read_data = P3 ^ 0xff;
	trg = read_data & (read_data ^ cont);
	cont = read_data;
}
//通过按键控制状态机
unsigned char read_key(){
  unsigned char key,key_value=0;
  key = P3 & 0x0f;//P3端口与0x0f相与，屏蔽掉高四位对其影响，同时获得按键值
  //0x0f=00001111
  switch(state){
    case 0: //判断按键是否按下
      if(key != 0x0f){
        state = 1;
      }
      break;
    case 1: //通过定时器消抖，并判断键值
      if(key != 0x0f){ //再次判断，防止按键抖动。如果key_in不等于0x0f，则说明按键确实按下
        if(key == 0x0e){
          //0x0e=00001110
          key_value = 1;
        }else if(key == 0x0d){
          //0x0d=00001101
          key_value = 2;
        }else if(key == 0x0b){
          key_value = 3;
        }else if(key == 0x07){
          key_value = 4;
        }
        state = 2;
      }
      else {//如果key_in等于0x0f，则说明是按键的抖动
        state = 0;
      }
      break;
    case 2: //按键抖动消除，判断按键是否松开
      if(key == 0x0f){
        state = 0; //按键松开，返回初始状态
      }
      break;
  }
  return key_value;
}

void Timer0Init(void)		//1毫秒@11.0592MHz
{
	AUXR |= 0x80;		//定时器时钟1T模式
	TMOD &= 0xF0;		//设置定时器模式
	TL0 = 0xCD;		//设置定时初值
	TH0 = 0xD4;		//设置定时初值
	TF0 = 0;		//清除TF0标志
	TR0 = 1;		//定时器0开始计时
  //add
  ET0 = 1;		//开定时器0中断
  EA = 1;		//开总中断
}

//定时器0中断服务程序
void Time0() interrupt 1
{
  if (cnt++ < 10)return;
  //cnt 用于计数，使得每10ms执行一次真正的程序，用于按键消抖
  cnt = 0;
  key_value = read_key();
  if(key_value == 1){
    P0 = 0x0e;
  }else if(key_value == 2){
    P0 = 0x0d;
  }else if(key_value == 3){
    P0 = 0x0b;
  }else if(key_value == 4){
    P0 = 0x07;
  }
}

int main()
{
  Timer0Init();
  P2 = P2 & 0x1f | 0x80;
  P0 = 0x00;
 

  while(1){
   
  }
}
