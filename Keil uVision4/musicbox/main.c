#include "reg51.h"
/*
#define clk 12000000/12/2
sbit BZpin=0x90;
bit timeout=0;
sbit sw=0xa0;
char one;
char zero;
bit KeyData=1;
unsigned char key=0;
unsigned int time;
unsigned int value;
unsigned int temp;

unsigned int table1[16] = {
    262, 294, 330, 349, 392, 440, 494, 523,
    587, 659, 698, 784, 880, 988, 1046, 1175
};
unsigned int table2[20] = {
    253, 238, 212, 190, 179, 159, 142, 126, 119, 
    106, 95, 89, 80, 70, 63, 225, 201, 169, 150, 134
};

unsigned int code beat[] = {
    1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 2,
    1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 2,
    1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 2, 0
};
unsigned char i=0;
unsigned char j=0;
unsigned char k=0;
*/
unsigned int r[] = {
    61, 65, 73, 82, 87, 98, 110, 123, 131, 147, 
    164, 174, 196, 220, 247, 69, 8, 92, 104, 116
};
unsigned int code song[] = {
    1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 0
};
void delay(unsigned char count);
unsigned int i = 0;
unsigned int k = 0;
main()
{

    while(1)
    {
        P1=0xAA;
        delay(r[song[i]]);
        P1=0x55;
        delay(r[song[i]]);
        i++;
        if(song[i]==0)
        {
            i = 0;
        }
        for(k = 0; k < 65; k++)
        {
            P1=0x55;
            delay(60);
        }
    }
}
void delay(unsigned char count)
{
    unsigned int j = 0;
    for(j = 0; j < count; j++);
}
/*
void tone(unsigned int frequency)
{
    if(temp!=frequency)
    {
        temp=frequency;
        value=clk/frequency;
        value=(65536-value)+15;
    }
    TR0=1;
}
void noTone()
{
    TR0=0;
    BZpin=0;
}

void T0_int(void) interrupt 1
{
    TR0=0;
    TH0=value/256;
    TL0=value%256;
    TR0=1;
    BZpin=~BZpin;
}

void T1_int(void) interrupt 3
{
    TR1=0;
    TH1=(65536-1000)/256;
    TL1=(65536-1000)%256;
    TR1=1;
    if(--time==0)
    {
        timeout=1;
    }
}*/