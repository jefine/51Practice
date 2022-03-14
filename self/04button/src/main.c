#include "STC15F2K60S2.h" 
#define key_in P3
unsigned int cnt = 0;
unsigned int state = 0;
unsigned char read_key(){
  unsigned char key,key_value;
  key = key_in & 0x0f;
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
          key_value = 1;
        }else if(key == 0x0d){
          key_value = 2;
        }else if(key == 0x0b){
          key_value = 3;
        }else if(key == 0x07){
          key_value = 4;
        }else{
          key_value = 0;
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
}

void time0 interrupt 1
{
  if (cnt++ < 10)
  return;
  cnt = 0;
  
  P2 = P2 & 0x1f | 0x80;
  P2 = P2 & 0x1f;
}

int main()
{
  {
    
  }
}
