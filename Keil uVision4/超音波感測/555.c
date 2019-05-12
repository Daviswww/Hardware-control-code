#include "reg51.h"

void delay(unsigned int s);
int s=0;
sbit P27=P2^7;
sbit P32=P3^2;
unsigned char	 DIST;
unsigned char	 DIST_BUF;
//unsigned char	 count;
void main()
{
     EA=1;
	 ET1=1;
	 TMOD=0x01;
	 TH0=(65536-5000)/256;
	 TL0=(65536-5000)%256;
	 TR0=1;

while(1)
 {
      DIST = 0x00; //REST DIST
	  P27 = 1; //Trig for ULTRASONIC
	  delay(1); // 12 us
	  P27 = 0; // Trig for ULTRASONIC
//
//
      while(P32==0) // echo wait
	  delay(45); // 540 us 
	  while(P32 == 1) //echo on
	  {
	        delay(5); // 5*12us = 60us approximately 58.8us
			DIST = DIST + 1; // distance increase 1cm
			if (DIST > 250)
			{
			       DIST = 250;

			}
	  }
 }

	  DIST_BUF = DIST; // echo off
}
//
void T1_int(void)  interrupt 1
{
   TH1=(65536-5000)/256;
   TL1=(65536-5000)%256;
   if(++s==200)
   {
   s=0;
   }
}

void delay(unsigned int s)
{
	 unsigned int i;   
	 for(i=0;i<s;i++);      
}