#include <mega128a.h>  
#include <delay.h>

#define mfrontleft 0 //timer 0  driverfront B
#define mfrontright 2 //timer 2 driverfront A
#define mbackleft 1 //timer 1   driverback A
#define mbackright 3 //timer 3  driverback B

#define ocr0max 0xFF
#define ocr1max 0xFFFF
#define ocr2max 0xFF
#define ocr3max 0xFFFF

void timerinit();
void portinit();
float num2per(char number, char motornumber);
void speedchange(char percentage, char motornumber);

/*
   PORT LIST
   TIMER0  PB4    
   TIMER1B PB6
   TIMER2  PB7  
   TIMER3B PE4
*/

void main(void)
{
    timerinit();
    portinit();   
    while (1)
    {
        speedchange(90, mfrontleft);
        speedchange(90, mfrontright);
        speedchange(50, mbackleft);
        speedchange(50, mbackright);
    }
} 



void timerinit()
{
    TCCR0 =  0b01101010; //fast_PWM_non_reverse_ouput 8 division
    TCCR1A = 0b00100011;    //COM1A1_non_reverse_output WGM: 고속PWM OCR1A가 최대값, OCR1B 사용
    TCCR1B = 0b00011010;    //노이즈제어 회로 사용X, 인풋 캡처 사용 X, WGM: 고속PWM, CS: 8분주 
    TCCR2 =  0b01101010; //fast_PWM_non_reverse_ouput 8 division                              
    TCCR3A = 0b00100011;    //COM1A1_non_reverse_output WGM: 고속PWM OCR1A가 최대값, OCR1B 사용
    TCCR3B = 0b00011010;    //노이즈제어 회로 사용X, 인풋 캡처 사용 X, WGM: 고속PWM, CS: 8분주 
    OCR1A=0xFFFF;
    OCR3AH=0xFF;
    OCR3AL=0xFF;
    
}

void portinit() //declare all timer ocr port opened 
{
    DDRA=0xFF; //for test purpose
    PORTA=0xFF;
    DDRB=0xFF;
    DDRE=0xFF;
}

float num2per(char number, char motornumber)   //if int is used, OCR1 doesn't work well.
{
 float ocrnum = 1;
 float ocrmax = 255;
 
 switch (motornumber)
        {
        case mfrontleft : 
            ocrmax = ocr0max;
            break;
		case mbackleft :
            ocrmax = ocr1max;
            break; 	
        case mfrontright:
            ocrmax = ocr2max; 
            break;        
        case mbackright : 
            ocrmax = ocr3max;        
            break;
        }

 ocrnum = ocrmax * number / 100;
 
 if( ocrnum >= 0)
    return ocrnum;
 else
    return 0;
}

void speedchange(char percentage, char motornumber)
{
 switch (motornumber)
        {
        case mfrontleft :
            OCR0 = num2per(percentage, motornumber);
            break;
        case mbackleft :
            OCR1BH = (int)num2per(percentage, motornumber) >> 8;
            OCR1BL = (int)num2per(percentage, motornumber) & 0xFF;      
            break;
        case mfrontright :
            OCR2 = num2per(percentage, motornumber);
			break;  
        case mbackright : 
            OCR3BH = (int)num2per(percentage, motornumber) >> 8;
            OCR3BL = (int)num2per(percentage, motornumber) & 0xFF;        
            break;
        }
}
