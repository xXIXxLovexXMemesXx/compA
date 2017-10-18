/*
 * File:   PIC and Galileo communication               
 * 
 * simple Galileo program example for main function
 * for UMass Lowell 16.480/552
 * 
 * Author: Jose Velis, Andy MacGregor,Grayson Colwell
 * Lab 2 main function Rev_1
 *
 * Created on 10/17/2017
 */


#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>


//Linux GPIO number // Aruino Shield Pin Name

#define Strobe     (40) // 8
#define GP_4       (48) // A0
#define GP_5	   (50) // A1
#define GP_6	   (52) // A2
#define GP_7	   (54) // A3
#define GPIO_DIRECTION_IN      (1)//Go HIGH (acoording to handout timing diagram))
#define GPIO_DIRECTION_OUT     (0)//Go LOW (acoording to handout timing diagram))
#define ERROR                  (-1)

/*User Commands*/
#define MSG_RESET   0X1
#define MSG_PING    0X2
#define MSG_GET     0X3
#define MSG_TURN30  0X4
#define MSG_TURN90  0X5
#define MSG_TURN120 0X6
#define MSG_ACK
/************************/



//open GPIO and set the direction
int openGPIO(int gpio, int direction )
{
     //   1.set the GPIO
    //    2.set the direction
    //    3.set the voltage
}

//write value
int writeGPIO(int f_Handle, int status)
{
        if (status == GPIO_DIRECTION_OUT) //?
        {
                // Set GPIO to low status
        }
        else
        {
                // Set GPIO to high status
        }

        return(0);

}



//Functions definitions - for commands
void reset();
void ping();
void adc_value();
void servo_30();
void servo_90();
void servo_120();





//main
int main(void)
{
 	  int fileHandleGPIO_4;
      int fileHandleGPIO_5;
      int fileHandleGPIO_6;
      int fileHandleGPIO_7;
	  int fileHandleGPIO_S;  //Should these 5 variables be used globally?

      fileHandleGPIO_S = openGPIO(Strobe, GPIO_DIRECTION_OUT);
    /*Line 92 sets the strobe to LOW as shown in figure 1 of the lab manual. 
        The data lines are also set LOW (lines 99-102) to assure that
        they aren't sending any data */ 
      
        while(1)
        {
            
            fileHandleGPIO_4 = openGPIO(GP_4, GPIO_DIRECTION_OUT);
            fileHandleGPIO_5 = openGPIO(GP_5, GPIO_DIRECTION_OUT);
            fileHandleGPIO_6 = openGPIO(GP_6, GPIO_DIRECTION_OUT);
            fileHandleGPIO_7 = openGPIO(GP_7, GPIO_DIRECTION_OUT);
            

           
            writeGPIO(fileHandleGPIO_S, GPIO_DIRECTION_IN); 
    /*For line 106, I'm not sure what the best way to handle 
        the write to of the GPIO is since a #define directive was 
        used (lines 28 & 29 ) to pick high or low. Can you send a
        #define variable and compare it to a normal variable? Even 
        if you can do that, how to physically change direction?
            
            
     2.write data... (where the menu selection is used? and a bunch of 
       other stuff?))*/
            
            int input;
            do{
            printf("Select a number for desired action: \n\n");
            printf("1. Reset\n");
            printf("2. Ping\n");
            printf("3. Get ADC value")
            printf("4. Turn Servo 30 degrees\n");
            printf("5. Turn Servo 90 degrees\n");
            printf("6. Turn Servo 120 degrees\n");
            printf("7. Exit\n");
            
            scanf("%s", input);
            
            switch (input)
            {
                case '1' :
                    reset();
                    break;
                case '2'  :
                    ping();
                    break;
                case '3'  :
                    adc_value();
                    break;
                case '4'  :
                    servo_30();
                    break;
                case '5'  :
                    servo_90();
                    break;
                case '6'  :
                    servo_120();
                default :
                    printf("Please enter a valid number (1 - 6)\n");
                    break;
            }
            }while(1); /// probably incorrect
            
           
           
           
           
           
           
           
           //3.Strobe low because Galileo is done writing to the bus
           writeGPIO(fileHandleGPIO_S, GPIO_DIRECTION_OUT);
	       //4.Strobe high
           writeGPIO(fileHandleGPIO_S, GPIO_DIRECTION_IN);
        }
}