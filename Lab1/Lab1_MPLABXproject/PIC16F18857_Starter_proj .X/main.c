
#include "mcc_generated_files/mcc.h" //default library 


// ++++++++++++ Helpful Notes ++++++++++++++++


/*
 include or set any library or definition you think you will need
 */

void servoRotate0() //0 Degree
{
  unsigned int i;
  for(i=0;i<50;i++)
  {
    PORTB = 1;
    __delay_ms(1.4);
    PORTB = 0;
    __delay_ms(18.6);
  }
}

void servoRotate90() //90 Degree
{
  unsigned int i;
  for(i=0;i<50;i++)
  {
    PORTB = 1;
    __delay_ms(4);
    PORTB = 0;
    __delay_ms(16);
  }
}

void servoRotate180() //-90 Degree
{
  unsigned int i;
  for(i=0;i<50;i++)
  {
    PORTB = 1;
    __delay_ms(0.5);
    PORTB = 0;
    __delay_ms(19.5);
  }
}
void ADC_Init(void)  {
 //  Configure ADC module  
 //---- Set the Registers below::
 // 1. Set ADC CONTROL REGISTER 1 to 0 
 // 2. Set ADC CONTROL REGISTER 2 to 0 
 // 3. Set ADC THRESHOLD REGISTER to 0
 // 4. Disable ADC auto conversion trigger control register
 // 5. Disable ADACT  
 // 6. Clear ADAOV ACC or ADERR not Overflowed  related register
 // 7. Disable ADC Capacitors
 // 8. Set ADC Precharge time control to 0 
 // 9. Set ADC Clock 
 // 10 Set ADC positive and negative references
 // 11. ADC channel - Analog Input
 // 12. Set ADC result alignment, Enable ADC module, Clock Selection Bit, Disable ADC Continuous Operation, Keep ADC inactive
  
 
    TRISA = 0b11111110;   //set PORTA to input except for pin0
    TRISAbits.TRISA1 = 1;   //set pin A1 to input
    ANSELAbits.ANSA1 = 1;   //set as analog input
    ADCON1 = 0;
    ADCON2 = 0;
    ADCON3 = 0;
    ADACT = 0;
    ADSTAT = 0;
    ADCAP = 0;
    ADPRE = 0;
    ADCON0 = 0b10000100; 
    ADREF = 0;
    ADPCH = 0b00000001;
    // transmit status and control register (UART CABLE)
    TX1STA = 0b00100000;
    RC1STA = 0b10000000;
            
}   

unsigned int ADC_conversion_results() {  
    ADPCH = 1; 
           
    ADCON0 |= 1;           //Initializes A/D conversion
    
    while(ADCON0 & 1);             //Waiting for conversion to complete
    unsigned result = (unsigned)((ADRESH << 8) + ADRESL);
    return result;    
}
 
/*
Develop your Application logic below
*/
#define ADC_THRESHOLD 0x0390
#define COUNT_THRESHOLD 5000
void main(void)
{
    // Initialize PIC device
    SYSTEM_Initialize(); //UART is initialized on portC
    ADC_Init(); //initializes ADC on port A1
    TRISA &= !0x01; //make sure portA0 is ouput for the LED
    TRISB = 00;
    TRISA = OUTPUT;
    
    unsigned results;
    unsigned count = 0;
    bool servo_direction_clockwise = true;
    while (1) // keep your application in a loop
    { 
        // ****** write your code 
        results = ADC_conversion_results();
        // Debug your application code using the following statement
        //printf("ADC says: %d compared to %d\n\r", results, ADC_THRESHOLD);
        
        if(results > ADC_THRESHOLD)
            PORTA |= 0x01; //turn on LEd
        else
            PORTA &= !0x01; //turn off LED
        
        count++;
        
        if( count > COUNT_THRESHOLD && servo_direction_clockwise)
        {
            count = 0;
            servoRotate180();
            servo_direction_clockwise = false;
        }
        else if( count > COUNT_THRESHOLD && !servo_direction_clockwise)
        {
            count = 0;
            servoRotate90();
            servo_direction_clockwise = true;
        }
           
    }
    
    
}

/**
 End of File
*/