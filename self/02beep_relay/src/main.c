#include "reg52.h" 

sbit relay = P0^4;
sbit buzzer = P0^6;
int main()
{
    // 1 means on, 0 means off
    buzzer = 0;
    relay = 0;
    P2 = P2 & 0x1f|0xa0;
    P2 = P2 & 0x1f;


    while (1)
    {
    }
  
}

