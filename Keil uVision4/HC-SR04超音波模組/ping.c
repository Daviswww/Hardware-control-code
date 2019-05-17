#include <reg51.h>

sbit trig=P3^5;
sbit LED=P2^4;

void convert(unsigned int);
void display(unsigned char );
void delay(unsigned int);
void delayz(unsigned int);
void send_pulse(void);
unsigned int get_range(void);
void updata(void);
unsigned char seg[10] =
{
    0xC0, 0xf9, 0xa4, 0xb0, 0x99,
    0x92, 0x82, 0xf8, 0x80, 0x90
};
unsigned char buf[4] = {0, 0, 0, 0};
unsigned int cms;
unsigned char j;
void main()
{
    for(j = 0; j < 3; j++)
    {
        LED=0x00;
        delayz(20);
        LED=0xFF;
        delayz(10);
    }

    TMOD=0x09;//timer0 in 16 bit mode with gate enable
    TR0=1;//timer run enabled
    TH0=0x00;
    TL0=0x00;
    P3|=0x04;//setting pin P3.2
    while(1)
    {
        get_range();
        delayz(2);
        if (cms > 15)
        {
            P1 = 0x0A; // Forward run #00001010
        }
        else
        {
            P1 = 0x00; // motor stop
            delay(8333); // 8333*12us = 0.1 sec
            P1 = 0x06; // motor turn left #00000110
            delay(15000); // 41667*12us = 0.5sec
            P1 = 0x00; // motor stop
        }
        
    }
}
void send_pulse(void) 
{
    TH0=0x00;TL0=0x00;
    trig=1; //Sending trigger pulse P3^^5
    delayz(10); //Wait for about 10milliseconds 
    trig=0; //Turn off trigger
}
unsigned int get_range(void)
{
    long int timer_val,i=0;
    send_pulse();
    while(!INT0); //Waiting until echo pulse is detected
    while(INT0); //Waiting until echo pulse changes its state
    timer_val=(TH0<<8)+TL0;

    if(timer_val<38000)
    {
        cms=timer_val/59;
        if ((cms!=0)&&(cms<100))
        {
            convert(cms);
            updata();
        }
        else
        {
            cms=99;
            updata();
        }
    }
    else
    {
        cms=99;
        updata();
    }
    
    return cms;
}
void convert(unsigned int distance)
{
    buf[3]=distance/1000;
    buf[2]=(distance%1000)/100;
    buf[1]=(distance%100)/10;
    buf[0]=distance%10;
}
void updata(void)
{
    P2 = 0x0e;
    P0 = seg[buf[3]];
    delayz(4);
    P2 = 0x0d;
    P0 = seg[buf[2]];
    delayz(4);
    P2 = 0x0b;
    P0 = seg[buf[1]];
    delayz(4);
    P2 = 0x07;
    P0 = seg[buf[0]];
    delayz(4);
}
void delayz(unsigned int n)
{
    unsigned char i;
    for(n;n>0;n--)
    {
        for(i=250;i>0;i--);
        for(i=247;i>0;i--);
    }
}
void delay(unsigned int count)
{
	unsigned int i;
	for(i = 0; i < count; i++);
}