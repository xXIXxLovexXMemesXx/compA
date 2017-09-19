
#include "mcc_generated_files/mcc.h" //default library 


// ++++++++++++ Helpful Notes ++++++++++++++++


/*
 include or set any library or definition you think you will need
 */

// ====================  prototype functions: ====================

void Timer2_Init(void)  {}     
 /*  Configure Timer 2 and start it
    
 USE Timer 2
 * 
 * ---- Set the Registers below:-----------
 * 
 * 1. Set the Basic Configuration Register for Timer 2 
 * 2. Set the Clock Source in relation with system Oscillator frequency 
 * 3. T2PSYNC Not Synchronized; T2MODE Software control; T2CKPOL Rising Edge; T2CKSYNC Not Synchronized; Timer Mode
 * 4. T2RSEL T2CKIPPS pin; Reset Source
 * 5. Set PR2 255; 
 * 6. Set TMR2  Prescale Value to 0 
 * 7. Clearing IF flag.
 * 8. Start Timer2
 * 
 * ----------------------
 */  



void PWM_Init(void)  {}  
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



void ADC_Init(void)  {}   
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



void PWM_signal_out(unsigned duty_cycle) 
{
    
}
 /*
 *- you set 10bits value for the duty cycle being careful with the MSB/LSB alignment 
 *- Set the appropriate Registers in the right sequence
 */



uint8_t ADC_conversion_results(unsigned ch) {}
 /* 
 * - set your ADC channel , activate the ADC module , and get the ADC result to a value , then deactivate again the ADC module
 * - Set the appropriate Registers in the right sequence
 */

//++++++++++++++++++++++++++++++++++++++++++++++++++++++




/*
Develop your Application logic below
*/

void main(void)
{
    // Initialize PIC device
    SYSTEM_Initialize();

    // Initialize the required modules 
   
    // **** write your code 
    
    

    while (1) // keep your application in a loop
    {
        
        
        // ****** write your code 
        
        
        
        // Debug your application code using the following statement
        printf("Hello Lab 1 \n\r");
        
        
        
    }
    
    
}

/**
 End of File
*/