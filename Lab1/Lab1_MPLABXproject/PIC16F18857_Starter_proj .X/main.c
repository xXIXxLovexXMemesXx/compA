
#include "mcc_generated_files/mcc.h" //default library 

#define ADC_CHAN 1
#define ADC_THRESH 0x7F
// ====================  prototype functions: ====================

/* Configure Timer 2 and start it
 */
void Timer2_Init(void)  
{
 //* 1. Set the Basic Configuration Register for Timer 2 
 //* 2. Set the Clock Source in relation with system Oscillator frequency 
    T2CLKCON  = 0x06; //sets CS to SOSC freq (pg 440))
 //* 3. T2PSYNC Not Synchronized; T2MODE Software control; T2CKPOL Rising Edge; T2CKSYNC Not Synchronized; Timer Mode
    T2HLT = 0x00; //sets mode to free running with software gate.  (424))
 //* 4. T2RSEL set reset source to Pin selected by T2INPPS (pg 443_)
    T2RST = 0x00;
 //* 5. Set PR2 255; 
    PR2 = 255; //Timer2 Module Period Register
 //* 6. Set TMR2  Prescale Value to 0 
    T2CON |= 0x00; //Prescale is 0
 //* 7. Clearing IF for timer 2
    PIR4 &= 0x01; //clears only the Timer2 interrupt flag
 //* 8. Start Timer2
    T2CON |= 0x80;  //turn timer 2 on.
}   

void ADC_Init(void)  {
 /*  Configure ADC module  

 ----- Set the Registers below::
 * 1. Set ADC CONTROL REGISTER 1 to 0 
 * 2. Set ADC CONTROL REGISTER 2 to 0 
 * 3. Set ADC THRESHOLD REGISTER to 0
 * 4. Disable ADC auto conversion trigger control register
 * 5. Disable ADACT  
 * 6. Clear ADAOV ACC or ADERR not Overflowed  related register
 * 7. Disable ADC Capacitors
 * 8. Set ADC Precharge time control to 0 
 * 9. Set ADC Clock 
 * 10 Set ADC positive and negative references
 * 11. ADC channel - Analog Input
 * 12. Set ADC result alignment, Enable ADC module, Clock Selection Bit, Disable ADC Continuous Operation, Keep ADC inactive
  
  */
    TRISA = 0xFF;   //set PORTA to input
    TRISAbits.TRISA2 = 1;   //set pin A2 to input
    ANSELAbits.ANSA2 = 1;   //set as analog input
    ADCON1 = 0;
    ADCON2 = 0;
    ADCON3 = 0;
    ADACT = 0;
    ADSTAT = 0;
    ADCAP = 0;
    ADPRE = 0;
    ADCON0 = 0b00001001; 
    ADREF = 0;
    
}   


 /**
 * - set your ADC channel , activate the ADC module , and get the ADC result to a value , then deactivate again the ADC module
 * - Set the appropriate Registers in the right sequence
 */
uint8_t ADC_conversion_results(unsigned ch) {}

void servoRotate0() //0 Degree
{
  unsigned int i;
  for(i=0;i<50;i++)
  {
    PORTB = 1;
    __delay_ms(1.5);
    PORTB = 0;
    __delay_ms(18.5);
  }
}

void servoRotate90() //90 Degree
{
  unsigned int i;
  for(i=0;i<50;i++)
  {
    PORTB = 1;
    __delay_ms(2);
    PORTB = 0;
    __delay_ms(18);
  }
}

void servoRotate180() //-90 Degree
{
  unsigned int i;
  for(i=0;i<50;i++)
  {
    PORTB = 1;
    __delay_ms(1);
    PORTB = 0;
    __delay_ms(19);
  }
}

/**
 * Test whatever we're working on atm
 */
void testMain(void)
{
  TRISB = 0; // PORTB as Ouput Port
  while(1)
  {
    servoRotate0(); //0 Degree
    __delay_ms(200);
    servoRotate90(); //90 Degree
    __delay_ms(200);
    servoRotate180(); //180 Degree
  }
}

void main(void)
{
    // Initialize PIC device
    SYSTEM_Initialize();
    testMain();
    // Initialize the required modules 
    Timer2_Init(); //starts the timer.
    ADC_Init();
    // **** write your code 
    
    bool servo_rotates_up = true;
    while (1) // keep your application in a loop
    {
        //If the photoresistor is above a certain threshold, turn on the LED (or not))
        if(ADC_conversion_results(ADC_CHAN) > ADC_THRESH)
            PORTA = 0x01;
        else
            PORTA = 0x00;
        
        //if the timer is done, rotate the servo the right direction
        if(0 && servo_rotates_up)
        {
            servoRotate180();
            servo_rotates_up = false;
        } 
        else if(0 && !servo_rotates_up)
        {
            servoRotate0();
            servo_rotates_up = true;
        }        
    } 
}

/**
 End of File
*/