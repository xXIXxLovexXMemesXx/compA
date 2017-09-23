
#include "mcc_generated_files/mcc.h" //default library 


#define ADC_THRESH 0x3190

#define COUNT_THRESH 10000

// ====================  prototype functions: ====================


void timer0Init(void)
{
    //Timer0 enabled with 16 bit timer and a 1:16 postscaler
    T0CON0 = 0b10011111;
    //set source to FOSC/4 and 1:1 prescaler
    T0CON1 = 0b01000000; 
    //enable Timer0 interupts
    PIE0 = 0xFF;
}
/* Configure Timer 2 and start it
 */
void Timer2_Init(void)  
{
 //* 1. Set the Basic Configuration Register for Timer 2 
 //* 2. Set the Clock Source in relation with system Oscillator frequency 
    T2CLKCON  = 0x06; //sets CS to SOSC freq (pg 440))
 //* 3. T2PSYNC Not Synchronized; T2MODE Software control; T2CKPOL Rising Edge; T2CKSYNC Not Synchronized; Timer Mode
    T2HLT = 0x08; //sets mode to One shot timer.  (424))
 //* 5. Set PR2 255; 
    T2PR = 0xFF; //Timer2 Module Period Register
 //* 6. Set TMR2  Prescale Value to 0 
    T2CON = 0b01100000 | 0b00001111; //1:64 prescaler and 1:16 postscaler
 //* 7. Clearing IF for timer 2
    PIR4 &= 0x01; //clears only the Timer2 interrupt flag
 //* 8. Start Timer2
    T2CON |= 0x80;  //turn timer 2 on.
}   

void PWM_Init(void)  {
    
}  
 /*  Configure CCP1 module in PWM mode (PWM channel = RC3 by default, Timer selection)
  Registers:
 * 
 * --- Set the Registers below: ---
 * 
 * 1. Set up CCP1 module register in PWM mode and bits alignment
 * 2. Set RH to 0; 
 * 3. Set RL to 0; 
 * 4. Selecting Timer 2 - for PWM in CCP1 module
 * 
 * ---------------
  */ 

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
    ADCON0 = 0b10000101; 
    ADREF = 0;
    ADPCH = 0b00000001;
}   


 /**
 * - set your ADC channel , activate the ADC module , and get the ADC result to a value , then deactivate again the ADC module
 * - Set the appropriate Registers in the right sequence
 */
unsigned int ADC_conversion_results() {
    GO_nDONE = 1; 
    
    if(ADCON0 & GO_nDONE)
        return;
  //ADCON0 &= 0xC5;              //Clearing channel selection bits
  //ADCON0 |= ADC_CHAN<<3;        //Setting channel selection bits
    ADPCH = 1; 
           
    ADCON0 |= GO_nDONE;           //Initializes A/D conversion
    // __delay_ms(2);               //Acquisition time to charge hold capacitor
    
    //while(!ADCON0 & GO_nDONE);             //Waiting for conversion to complete
    return ((ADRESH << 8) + ADRESL);     //Return result
//<<8)
}

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
    // Initialize the required modules 
    ADC_Init();
    TRISA = 0b11111110; //make sure pin A0 is output
    TRISB = 0;
    // **** write your code 
    bool servo_rotates_up = true;
    unsigned count = 0;
    while (1) // keep your application in a loop
    {
        count++;

        //If the photoresistor is above a certain threshold, turn on the LED (or not))
        if(ADC_conversion_results() > ADC_THRESH)
        {
            PORTA = 0x01;
        }
        else
            PORTA = 0x00;
        
        
        //if the timer is done, rotate the servo the right direction
        if(servo_rotates_up && (count >= COUNT_THRESH) )
        {
            servoRotate180();
            servo_rotates_up = false;
            //PORTA = 0x00;
            count = 0;
        } 
        else if(!servo_rotates_up && (count >= COUNT_THRESH) )
        {
            servoRotate90();
            servo_rotates_up = true;
            //PORTA = 0x01;
            count = 0;
        }        
    } 
}

/**
 End of File
*/