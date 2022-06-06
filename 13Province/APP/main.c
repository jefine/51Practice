#include <STC15F2K60S2.H>
#include "misc.h"
#include "onewire.h"
#include "ds1302.h"

#define relay_set {EA = 0;P0 = 0x10;P2 = 0xa0;P2 = 0;jidianqi=1;EA =1;}
#define relay_reset {EA = 0;P0 = 0;P2 = 0xa0;P2 = 0;jidianqi=0;EA =1;}

#define led_set(x) {EA =0;P0 = ~(0x01<<x-1);P2 = 0x80;P2 =0;EA =1;}

#define led_set_all(x) {EA =0;P0 = x;P2 = 0x80;P2 =0;EA =1;}

#define led_all_set(x) {l &= ~(0x01<<x-1);}
u8 code t_display[]={                       //标准字库
//   0    1    2    3    4    5    6    7    8    9    A    B    C    D    E    F
    0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F,0x77,0x7C,0x39,0x5E,0x79,0x71,
//  black  -   H    J    K    L    N    o   P    U     t    G    Q    r   M    y
    0x00,0x40,0x76,0x1E,0x70,0x38,0x37,0x5C,0x73,0x3E,0x78,0x3d,0x67,0x50,0x37,0x6e,
    0xBF,0x86,0xDB,0xCF,0xE6,0xED,0xFD,0x87,0xFF,0xEF,0x46};    //0. 1. 2. 3. 4. 5. 6. 7. 8. 9. -1

u8 code T_COM[]={0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80};      //位码

u8 smg_cnt=0,ui=0,temper_setting = 23;
u16 ms =0,temper=0;

extern unsigned char hour,min,sec;

u8 smg_buf[8] = {0};
u8 mode = 1,key17_flag=0,jidianqi=0;
//1 温度；0时间控制
u8 key_old=1,key_down,key_up;
void Timer0Init(void)		//1毫秒@12.000MHz
{
	AUXR |= 0x80;		//定时器时钟1T模式
	TMOD &= 0xF0;		//设置定时器模式
	TL0 = 0x20;		//设置定时初值
	TH0 = 0xD1;		//设置定时初值
	TF0 = 0;		//清除TF0标志
	TR0 = 1;		//定时器0开始计时
	ET0 = 1;
	EA = 1;
}

void timer0handle()interrupt 1
{
	ms ++;
	if(ms == 10000)ms = 0;
	
		P0 = T_COM[smg_cnt];
		P2 = 0xc0;
		P2 = 0;
		
		P0 = ~t_display[smg_buf[smg_cnt]];
		P2 = 0xe0;
		P2 = 0;
	
		if(++smg_cnt==8)smg_cnt=0;
	
	
}

void ui_flush()
{
	//temper
	if(ui == 0)
	{
		smg_buf[0] = 25;
		smg_buf[1] = ui+1;
		smg_buf[2] = 16;
		smg_buf[3] = 16;
		smg_buf[4] = 16;
		smg_buf[5] = temper / 100;
		smg_buf[6] = temper / 10 % 10 + 32;
		smg_buf[7] = temper % 10;
	}
	//time
	if(ui == 1)
	{
		if( key17_flag )
	{
			smg_buf[0] = 25;
			smg_buf[1] = ui+1;
			smg_buf[2] = 16;
			smg_buf[3] = min/10;
			smg_buf[4] = min%10;
			smg_buf[5] = 17;
			smg_buf[6] = sec/10;
			smg_buf[7] = sec%10;
	}
	else{
			smg_buf[0] = 25;
			smg_buf[1] = ui+1;
			smg_buf[2] = 16;
			smg_buf[3] = hour/10;
			smg_buf[4] = hour%10;
			smg_buf[5] = 17;
			smg_buf[6] = min/10;;
			smg_buf[7] = min%10;
	}
	}
	//setting
	if(ui == 2)
	{
		smg_buf[0] = 25;
		smg_buf[1] = ui+1;
		smg_buf[2] = 16;
		smg_buf[3] = 16;
		smg_buf[4] = 16;
		smg_buf[5] = 16;
		smg_buf[6] = temper_setting/10;;
		smg_buf[7] = temper_setting%10;
	}
}
void key_proc()
{
	u8 key;
	key = read_key();
	key_down = key & (key ^ key_old);
	key_up = ~key & (key ^ key_old);
	key_old = key;
	
	if(key_down!=0)smg_buf[0] = key_down;
	if(key_down == 12) ui = (++ui)%3;
	
	if(key_down == 13) 
	{
		if(mode == 1)mode = 0;
		else mode =1;
	}
	if(ui == 2 && key_down == 16)temper_setting++;
	if(ui == 2 && key_down == 17)temper_setting--;
	
	if(ui ==1 &&key_down ==17)key17_flag =1;
	if(ui ==1 &&key_up ==17)key17_flag =0;
}
void led()
{
	unsigned char l=0xff;
	
	led_set(0);
	
	if(min == 0 && (sec>=0 && sec<5))led_all_set(1);
	
	if(mode)led_all_set(2);
	
	if((jidianqi ==1) && (ms%100==0)){
		led_all_set(3);
	}
	EA =0;P0 = l;P2 = 0x80;P2 =0;EA =1;
}
void relay()
{
	if(mode == 1){
		if(temper/10 >= temper_setting) relay_set;
		if(temper/10 < temper_setting) relay_reset;
	}
	if(mode == 0){
		if(min == 0 && (sec>=0 && sec<5)){relay_set;}
		else relay_reset;
	}
}
void main()
{
	relay_reset;
	temper = read_temperature();
	Timer0Init();
	write_time(12,59,55);
	while(1){
		led();
		ui_flush();
		if(ms % 10 == 0)key_proc();
		if(ms % 500 == 0)temper = read_temperature();
		if(ms % 100 == 0)read_time();  
		if(ms % 100 == 0)relay();
	}
}