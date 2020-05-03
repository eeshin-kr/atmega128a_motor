#include <mega128a.h>  
#include <delay.h>

char num2per(char number)
{
 char ocrnum = 1;
 ocrnum = 255 * number / 100;
 
 if( ocrnum > 0) 
    return ocrnum;
 else
    return 1;
}

void speedchange(char percentage)
{
 OCR0 = num2per(percentage);
}

void main(void)
{
    DDRB=0xFF;    
    TCCR0 = 0b01101010; //non_reverse_ouput 8 division 
    while (1)
    {
    speedchange(70); delay_ms(3000);
    speedchange(80); delay_ms(3000);
    }
}
