#include <STC15F2K60S2.H>
#include "key_board.h"
#include "onewire.h"
#include "iic.h"

#define set_led(x) {P0 = ~(0x01<<(x));P2 = 0x80;P2 = 0;}

u8  smg_buf[8]={0};        //显示缓冲
u8  smg_cnt=0;  //显示位索引

u8 key_val,key_old;
u16 temper,dac_output,ms;

u8 slowsmg = 0;
u8 temper_setting = 25,dac_mode=1,ui=1,temper_setting_new;

u8 code t_display[]={                       //标准字库
//   0    1    2    3    4    5    6    7    8    9    A    B    C    D    E    F
    0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F,0x77,0x7C,0x39,0x5E,0x79,0x71,
//  black  -   H    J    K    L    N    o   P    U     t    G    Q    r   M    y
    0x00,0x40,0x76,0x1E,0x70,0x38,0x37,0x5C,0x73,0x3E,0x78,0x3d,0x67,0x50,0x37,0x6e,
    0xBF,0x86,0xDB,0xCF,0xE6,0xED,0xFD,0x87,0xFF,0xEF,0x46,0x77};    //0. 1. 2. 3. 4. 5. 6. 7. 8. 9. -1 R

u8 code T_COM[]={0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80};      //位码

void Timer1Init(void)		//1毫秒@12.000MHz 数码管
{
	AUXR &= 0xBF;		//定时器时钟12T模式
	TMOD &= 0x0F;		//设置定时器模式
	TL1 = 0x18;		//设置定时初值
	TH1 = 0xFC;		//设置定时初值
	TF1 = 0;		//清除TF1标志
	TR1 = 1;		//定时器1开始计时
	ET1 = 1;
	EA =1;
}

void timer1handle() interrupt 3
{
	if(++ms==10000)ms=0;
	P0 = T_COM[smg_cnt];
	P2 = 0xc0;
	P2 = 0;
	
	P0 = ~t_display[smg_buf[smg_cnt]];
	P2 = 0xe0;
	P2 = 0;
	if(++smg_cnt==8)smg_cnt = 0;
}
void key_proc()
{
	u8 key_down,key_up,key;
	key = read_key();
	key_down = key & (key ^ key_old);
	key_up = ~key & (key ^ key_old);
	key_old = key;
	if(key_up == 4)
	{
		ui = (ui+1)%4;
		if(!ui)ui++;
		set_led(ui);
		if(ui == 3 && dac_mode)
		{
			P0 = 0xf6;P2 = 0x80;P2 = 0;
		}
		//if(ui == 3)temper_setting = temper_setting_new;
		
		
		
	}
	if(ui == 2)
	{
//		if(key_down == 8)temper_setting_new = temper_setting-1;
//		if(key_down == 9)temper_setting_new = temper_setting+1;
		if(key_down == 8)temper_setting--;
		if(key_down == 9)temper_setting++;
	}	
	if(key_down == 5){
		if(dac_mode==1)dac_mode =2;
		else dac_mode = 1;
	}
}
void smg_proc()
{
	if(ui == 1)
	{
		smg_buf[0] = 12;
		smg_buf[1] = 16;
		smg_buf[2] = 16;
		smg_buf[3] = 16;
		smg_buf[4] = temper/1000;
		smg_buf[5] = temper/100%10+32;
		smg_buf[6] = temper/10%10;
		smg_buf[7] = temper%10;
	}
	if(ui == 2)
	{
		smg_buf[0] = 24;
		smg_buf[1] = 16;
		smg_buf[2] = 16;
		smg_buf[3] = 16;
		smg_buf[4] = 16;
		smg_buf[5] = 16;
		smg_buf[6] = temper_setting/10;
		smg_buf[7] = temper_setting%10;
	}
	if(ui == 3)
	{
		smg_buf[0] = 43;
		smg_buf[1] = 16;
		smg_buf[2] = 16;
		smg_buf[3] = 16;
		smg_buf[4] = 16;
		smg_buf[5] = dac_output/100+32;
		smg_buf[6] = dac_output/10%10;
		smg_buf[7] = dac_output%10;
	}
}
void main()
{
	
	Timer1Init();
	set_led(ui);	
	while(1)
	{
		if(ms%10==0)key_proc();
		if(ms%100==0)temper = read_temperature();
		if(ms%100==0)
		if(ui ==3)
		{
			if(dac_mode==1)
			{
				if(temper < temper_setting*100)dac_output = 0;
				else dac_output = 500;
			}
			if(dac_mode == 2)
			{
				if(temper/100<20)dac_output=100;
				else if(temper/100>40)dac_output=400;
				else dac_output = (3/20.0*temper/100.0-2)*100;
			}
			write_dac(dac_output/500.0*255);
		}
		
		smg_proc();
		
	}
}