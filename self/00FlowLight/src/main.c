#include "STC15F2K60S2.H"

typedef unsigned char u8;
typedef unsigned int u16;
u8 buzzer_state=0;
u8 relay_state=0;
#define BUZZER_ON {EA = 0;P06 = 1;P2 = 0xa0;P2 = 0;buzzer_state=1;EA = 1;}
#define BUZZER_OFF {EA = 0;P06= 0;P2 = 0xa0;P2 = 0;buzzer_state=0;EA = 1;}

#define RELAY_ON {EA = 0;P04=1;P2 = 0xa0;P2 = 0;relay_state=1;EA = 1;}
#define RELAY_OFF {EA = 0;P04=0;P2 = 0xa0;P2 = 0;relay_state=0;EA = 1;}

void Delay1ms()		//@12.000MHz
{
	unsigned char i, j;
	i = 12;
	j = 169;
	do
	{
		while (--j);
	} while (--i);
}
int main()
{
	int i=0;
	BUZZER_OFF;
	RELAY_OFF;
	while(1)
	{
		Delay1ms();
		i++;
		if(i==1000)
		{
			if(buzzer_state == 0)BUZZER_ON
			else BUZZER_OFF;
			if(relay_state == 0)RELAY_ON
			else RELAY_OFF
			i=0;
		}
	}
}