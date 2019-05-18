#include "reg51.h"
#define max() {X > Y ? X : Y}
sbit LED = 0x90;
unsigned int j;
unsigned int ALED[3][3] = 
{{0x78, 0x74, 0x72},
{0xB8, 0xB4, 0xB2},
{0xD8, 0xD4, 0xD2}};
main()
{
   
    void delay(unsigned int);
    for(j = 0; j < 3; j++)
    {
        LED = 0x00;
        delay(100000);
        LED = 0xff;
        delay(100000);
    }
    while(1)
    {
        P2 = 0xff;
        delay(100000);
        P2 = 0xB8;
        delay(100000);

    }
}

void delay(unsigned int count)
{
    unsigned int i;
    for(i = 0; i < count; i++);
}