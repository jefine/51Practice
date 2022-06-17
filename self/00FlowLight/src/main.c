#include "STC15F2K60S2.H"

typedef unsigned char u8;
typedef unsigned int u16;
u8 light_i=1;

#define LED_SET(x) {EA = 0;P0 = ~(0x01<<(x-1));P2 = (P2 & 0x1f)|0x80;P2 = P2 & 0x1f;EA = 1;}
//key_old 的初始化状态是根据具体的业务逻辑进行的
//初始化为我们想要的初始状态所采用的按键。
u8 key_old=1,key_down,key_up;
u8 read_key()
{
	u16 t;
	u8 key;
	/*这里为什么第一次将4个引脚置高，而之后置改变两个呢？
	因为第一次时候需要把其他引脚置高，来确保P44的准确性，算作初始化。
	而之后因为我们把P44拉低了，所以还需要置高，防止他影响其他的判断，
	而对于P34，P35我们并没有拉低他们，所以不需要改变。
	每次只需要把自己拉低的引脚重新置高就可了
	*/
	P44 =0 ;P42 = 1;P35 = 1;P34 =1;
	t = P3;
	P44 = 1;P42 = 0;
	t = t<<4 | (P3 & 0x0f);
	P42 = 1;P35 = 0;
	t = t<<4 | (P3 & 0x0f);
	P35 = 1; P34 = 0;
	t = t<<4 | (P3 & 0x0f);

	switch (~t)
	{
	case 0x8000:key = 4;break;
	case 0x4000:key = 5;break;
	case 0x2000:key = 6;break;
	case 0x1000:key = 7;break;
	case 0x0800:key = 8;break;
	case 0x0400:key = 9;break;
	case 0x0200:key = 10;break;
	case 0x0100:key = 11;break;
	case 0x0080:key = 12;break;
	case 0x0040:key = 13;break;
	case 0x0020:key = 14;break;
	case 0x0010:key = 15;break;
	case 0x0008:key = 16;break;
	case 0x0004:key = 17;break;
	case 0x0002:key = 18;break;
	case 0x0001:key = 19;break;
	default:key = 0;
	}
	return key;

}
//在里面尽量只改变状态，动作在Main函数或其他函数进行
void key_proc()
{
	u8 key = read_key();
	//Don't try to find out why,just feel and remember them :）
	key_down = key & (key ^ key_old);
	key_up = ~key & (key ^ key_old);
	key_old = key;

	if(key_down == 12) {
		light_i--;
		if(light_i<1)light_i=1;
	}
	if(key_down == 16){
		light_i++;
		if(light_i>8)light_i=8;
	}
	
}
void Delay10ms()		//@12.000MHz
{
	unsigned char i, j;

	i = 117;
	j = 184;
	do
	{
		while (--j);
	} while (--i);
}

int main()
{
	
	while(1)
	{
		key_proc();
		Delay10ms();
		LED_SET(light_i);
	}
	
}