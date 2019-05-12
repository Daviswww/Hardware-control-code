#include<regx51.h>
#include<stdio.h>
#include<string.h> 
int a,b,ans;
char rec[4];
void main()   
{    
    a=80;
    b=40;
    ans=a+b;
    sprintf(rec, "%d", ans);
    TMOD=0x20;                                //Choosing Timer mode    
    TH1=0xFD;                                   //Selecting Baud Rate    
    SCON=0x50;                               //Serial mode selection    
    TR1=1;    
    IE=0x90;                                      //Enabling Serial Interrupt    
    while(1);    
}
void ser_intr(void)interrupt 4        //Subroutine for Interrupt  
{
    IE=0x00;
    unsigned int i;
    for(i=0;i<=2;i++)                      //Transmitting data
    {
        SBUF=rec[i];
        while(TI==0);
        TI=0;
    }
    IE=0x90;
}