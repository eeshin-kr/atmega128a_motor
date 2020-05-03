#include <mega128a.h>  
#include <delay.h>

#define mfrontleft 0 //timer 0  driverfront B
#define mfrontright 2 //timer 2 driverfront A
#define mbackleft 1 //timer 3   driverback A
#define mbackright 3 //timer 3 with mbackleft driverback B
#define mbackleftright 4 //to move back motors simulataneously
#define mbackrightleft 4 //to move back motors simulataneously

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
   TIMER1B PB6  non used port
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
//        speedchange(90, mfrontright);
//        speedchange(50, mbackleft);
//        speedchange(50, mbackright);
    }
} 



void timerinit()
{
    TCCR0 =  0b01101010; //fast_PWM_non_reverse_ouput 8 division
//    TCCR1A = 0b00100011;    //COM1A1_non_reverse_output WGM: 고속PWM OCR1A가 최대값, OCR1B 사용
//    TCCR1B = 0b00011010;    //노이즈제어 회로 사용X, 인풋 캡처 사용 X, WGM: 고속PWM, CS: 8분주 
    TCCR2 =  0b01101010; //fast_PWM_non_reverse_ouput 8 division                              
    TCCR3A = 0b00100011;    //COM1A1_non_reverse_output WGM: 고속PWM OCR1A가 최대값, OCR1B 사용
    TCCR3B = 0b00011010;    //노이즈제어 회로 사용X, 인풋 캡처 사용 X, WGM: 고속PWM, CS: 8분주 
//    OCR1A=0xFFFF;
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
 float absnum = number > 0 ? num : num * (-1); 
 
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
        case mbackrightleft : 
            ocrmax = ocr3max;        
            break;    
            
        }

 ocrnum = ocrmax * absnum / 100;
 
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
            if (percentage > 0){
                PORTE = PORTE & 0b11111100
                PORTE = PORTE | 0b00000001
                }
            else{
                PORTE = PORTE & 0b11111100
                PORTE = PORTE | 0b00000010
                }
            break;
        case mbackleft :
            OCR3BH = (int)num2per(percentage, motornumber) >> 8;
            OCR3BL = (int)num2per(percentage, motornumber) & 0xFF;
            if (percentage > 0){
                PORTE = PORTE & 0b00001111
                PORTE = PORTE | 0b00010000
                }
            else{
                PORTE = PORTE & 0b11001111
                PORTE = PORTE | 0b00100000
                }             
            break;
        case mfrontright :
            OCR2 = num2per(percentage, motornumber);
            if (percentage > 0){
                PORTE = PORTE & 0b11110011
                PORTE = PORTE | 0b00000100
                }
            else{
                PORTE = PORTE & 0b11110011
                PORTE = PORTE | 0b00001000
                }                         
			break;  
        case mbackright : 
            OCR3BH = (int)num2per(percentage, motornumber) >> 8;
            OCR3BL = (int)num2per(percentage, motornumber) & 0xFF;
            if (percentage > 0){
                PORTE = PORTE & 0b00001111
                PORTE = PORTE | 0b01000000
                }
            else{
                PORTE = PORTE & 0b00111111
                PORTE = PORTE | 0b10000000
                }                                               
            break;
        case mbackrightleft :
            OCR3BH = (int)num2per(percentage, motornumber) >> 8;
            OCR3BL = (int)num2per(percentage, motornumber) & 0xFF;
            if (percentage > 0){
                PORTE = PORTE & 0b00001111
                PORTE = PORTE | 0b01010000
                }
            else{
                PORTE = PORTE & 0b00001111
                PORTE = PORTE | 0b10100000
                }                                               
            break;            
                       
        }
        
        
 
 
}
