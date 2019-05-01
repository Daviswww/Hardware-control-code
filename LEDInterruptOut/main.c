#include "reg51.h"

unsigned char LED=0x80;

int main()
{
    void delay(unsigned int);

    SP=0x60;
    EA=1;
    //ET0=1;
    IT0=1;
    EX0=1;
    P0 =~ LED;
    while(1)
    {
        
        delay(10000);
        LED >>= 1;
        if(LED==0)
        {
            LED=0x80;
        }
    }
}

void delay(unsigned int t)
{
    int i;
    for(i=0; i < t; i++);
}

void EX0_int(void) interrupt 0
{
    unsigned int i;
    unsigned char temp;
    temp=LED;
    P0=0xff;
    for(i=0; i<10;i++)
    {
        delay(10000);
        P0^=0xff;
    }
    LED=temp;
}