#include "key.h"
unsigned char Key_Read(void)
{
  unsigned int Key_New;
  unsigned char Key_Val;

  P44 = 0; P42 = 1; P35 = 1; P34 = 1;     //╣зр╩ап
  Key_New = P3;

  P44 = 1; P42 = 0;
  Key_New = (Key_New<<4) | (P3&0x0f);

  P42 = 1; P35 = 0;
  Key_New = (Key_New<<4) | (P3&0x0f);

  P35 = 1; P34 = 0;
  Key_New = (Key_New<<4) | (P3&0x0f);
  
  //P3 = P3&0xf0;
  //while( ( (P44==1) && (P42 ==1) && (P35 ==1)&&(P34 ==0)) == 0);
  
  switch(~Key_New)
  {
    case 0x8000:Key_Val = 4 ; break;   //s4
    case 0x4000:Key_Val = 5 ; break;   //s5
    case 0x2000:Key_Val = 6 ; break;   //s6
    case 0x1000:Key_Val = 7 ; break;   //s7
    case 0x0800:Key_Val = 8 ; break;   //s8
    case 0x0400:Key_Val = 9; break;   //s9
    case 0x0200:Key_Val = 10; break;   //s10
    case 0x0100:Key_Val = 11; break;   //s11
    case 0x0080:Key_Val = 12; break;   //s12
    case 0x0040:Key_Val = 13; break;   //s13
    case 0x0020:Key_Val = 14; break;   //s14
    case 0x0010:Key_Val = 15; break;   //s15
    case 0x0008:Key_Val = 16; break;   //s16
    case 0x0004:Key_Val = 17; break;   //s17
    case 0x0002:Key_Val = 18; break;   //s18
    case 0x0001:Key_Val = 19 ; break;   //s19
    default: Key_Val = 0;
  }
  return Key_Val;
}
