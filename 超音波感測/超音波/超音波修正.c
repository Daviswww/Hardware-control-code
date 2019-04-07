#include "reg51.h"

sbit P27=P2^7;
sbit P32=P3^2;
void delay(unsigned int s)
{
    unsigned int m;
    for(m=0;m<s;m++);
}

void main()
{ 
	unsigned char	 DIST;
	unsigned char	 DIST_BUF;
	while(1)
	{
		DIST =0x00;
		P27=1;
		delay(1);
		P27=0;
		//while(P32 == 0)
		delay(45);
		while(P32 == 1)
		{
			delay(5);
			DIST =DIST + 1;
			if (DIST>250)
			{
				DIST = 250;
			}
		}
		DIST_BUF = DIST;
		P0 = DIST_BUF;
	}
}