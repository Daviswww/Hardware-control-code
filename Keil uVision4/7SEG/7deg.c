#include"reg51.h"

main()
{
    void delay(unsigned int);
    char num;
    unsigned char seg[11] = {0xc0, 0xf9, 0xa4, 0xb0, 0x99, 0x92, 0x82, 0xf8, 0x80, 0x90};

    while(1)
    {
        for(num = 1; num < 10; num++)
        {
            P0 = seg[num];
            delay(100000);
        }
    }
    return 0;
}

void delay(unsigned int count)
{
    unsigned int i;
    for(i = 0; i < count; i++);
}