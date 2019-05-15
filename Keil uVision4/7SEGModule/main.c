#include "reg51.h"
#include "intrins.h"

unsigned char seg[10] =
{
    0xC0, 0xf9, 0xa4, 0xb0, 0x99,
    0x92, 0x82, 0xf8, 0x80, 0x90
};
unsigned char buf[4] = {0, 0, 0, 0};

void delay(unsigned int count)
{
    unsigned int i;
    for(i = 0; i < count; i++);
}

main(void)
{
    int i;
    unsigned char j;
    while(1)
    {
        for(i = 0; i < 9999; i++)
        {
            for(j = 0; j < 20; j++)
            {
                P2 = 0x0e;
                P0 = seg[(i/100)/10];
                delay(200);
                P2 = 0x0d;
                P0 = seg[(i/100)%10];
                delay(200);
                P2 = 0x0b;
                P0 = seg[(i%100)/10];
                delay(200);
                P2 = 0x07;
                P0 = seg[(i%100)%10];
                delay(200);
            }
        }
    }
}