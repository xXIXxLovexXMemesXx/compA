
#include "mcc_generated_files/mcc.h" //default library 











 void ADC_Init(void)  {   
    //Configure ADC module  

    //----- Set the Registers below::
    //* 1. Set ADC CONTROL REGISTER 1 to 0 
    //* 2. Set ADC CONTROL REGISTER 2 to 0 
    //* 3. Set ADC THRESHOLD REGISTER to 0
    //* 4. Disable ADC auto conversion trigger control register
    //* 5. Disable ADACT  
    //* 6. Clear ADAOV ACC or ADERR not Overflowed  related register
    //* 7. Disable ADC Capacitors
    //* 8. Set ADC Precharge time control to 0 
    //* 9. Set ADC Clock 
    //* 10 Set ADC positive and negative references
    //* 11. ADC channel - Analog Input
    //2. Set ADC result alignment, Enable ADC module, Clock Selection Bit, Disable ADC Continuous Operation, Keep ADC inactive
 

 }







/*
 ADC_conversion_results() prototype function
 * 
 * - set your ADC channel , activate the ADC module , and get the ADC result to a value , then deactivate again the ADC module
 * - Set the appropriate Registers in the right sequence
 */

//++++++++++++++++++++++++++++++++++++++++++++++++++++++






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

