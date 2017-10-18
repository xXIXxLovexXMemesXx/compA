/*
 * File:   PIC and Galileo communication               
 * 
 * simple Galileo program example
 * for UMass Lowell 16.480/552
 * 
 * Author: Ioannis
 *
 * Created on 2017/9/21
 */


#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>

//Linux GPIO number // Shield Pin Name
#define STROBE                  (40) // pin 8 (no PWM)
#define GP_4                    (48) // pA0
#define GP_5                    (50) // A1
#define GP_6                    (52) // A2
#define GP_7                    (54) // A3

#define GPIO_DIRECTION_IN       (1)
#define GPIO_DIRECTION_OUT      (0)
#define HIGH                    (1)
#define LOW                     (0)
#define ERROR                   (-1)

//Constants
#define BUFFER_SIZE             (256)
#define EXPORT_FILE             "/sys/class/gpio/export"

//open GPIO and set the direction
//returns pointer to string containing the gpio pin directory if successful
//  this needs to get freed after you're done with it
//returns null ptr on error
char* openGPIO(int gpio_handle, int direction )
{
  int n;
  //   1. export GPIO
  FILE* exportFh = fopen(EXPORT_FILE, "w");
  if(exportFh== NULL)
  {
    printf("Couldn't not open export file\n");
    return NULL;
  }
  char numBuffer[5];
  snprintf(numBuffer, 5, "%d", gpio_handle);
  n = fputs(numBuffer, exportFh);
  if(n < 0)
  {
    printf("error writing to export file\n");
    return NULL;
  }
  fclose(exportFh);

  //form the file name of the newly created gpio directory
  char *gpioDirectory = malloc(BUFFER_SIZE);

  n = snprintf(gpioDirectory, BUFFER_SIZE, "/sys/class/gpio/gpio%d/", gpio_handle);
  if(n >= BUFFER_SIZE)
  {
    printf("Buffer overflow when creating directory name\n");
    free(gpioDirectory);
    return NULL;
  }

  //    2.set the direction
  char gpioDirection[BUFFER_SIZE];
  strcpy(gpioDirection, gpioDirectory);
  strcat(gpioDirection, "direction");
  FILE* directionFh = fopen(gpioDirection, "w");

  if(directionFh == NULL)
  {
    printf("gpio direction file is null %s \n", gpioDirection);
    free(gpioDirectory);
    return NULL;
  }

  if(direction == GPIO_DIRECTION_OUT)
  {
    n = fputs("out", directionFh);
  }
  else if(direction == GPIO_DIRECTION_IN)
  {
    n = fputs("in", directionFh);
  }
  if(n < 0)
  {
    printf("Error writing to gpio direction file\n");
    free(gpioDirectory);
    return NULL;
  }
  fclose(directionFh);

  //    3.set the votage
  char gpioValue[BUFFER_SIZE];
  strcpy(gpioValue, gpioDirectory);
  strcat(gpioValue, "value");
  FILE* valueFh = fopen(gpioValue, "w");
  n = fputs("1", valueFh);
  if(n < 0)
  {
    printf("Error writing to gpio value file\n");
    free(gpioDirectory);
    return NULL;
  }

  //return the new gpio directory
  return gpioDirectory;
}

//write value (HIGH or LOW) to port specified
//returns value written on success
//returns ERROR (negative) on failure
int writeGPIO(char* gpioDirectory, int value)
{
  char gpioValue[BUFFER_SIZE];
  strcpy(gpioValue, gpioDirectory);
  strcat(gpioValue, "value");
  FILE* valueFh = fopen(gpioValue, "w");

  char numBuffer[5];
  snprintf(numBuffer, 5, "%d", value);

  int n = fputs(numBuffer, valueFh);
  if(n < 0)
  {
    printf("Error writin to gpio value file in %s", gpioDirectory);
    return ERROR;
  }

  return value;
}

//read value (HIGH or LOW) from port specified
//port direction must be set to input.
//returns port value on success
//returns ERROR (negative) on failure
int readGPIO(char* gpioDirectory)
{
  char gpioValue[BUFFER_SIZE];
  strcpy(gpioValue, gpioDirectory);
  strcat(gpioValue, "value");
  FILE* valueFh = fopen(gpioValue, "r");

  char numBuffer[5];
  char* test = fgets(numBuffer, 5, valueFh);
  if(test == NULL)
  {
    printf("Error reading from gpio value %s", gpioDirectory);
    return ERROR;
  }

  return atoi(numBuffer);
}

//tests the GPIO write functions and exits
void testGPIO(char * fh)
{
  //test read and write.
  int w = writeGPIO(fh, HIGH);
  assert(w == HIGH && "Write high");
  w = writeGPIO(fh, LOW);
  assert(w == LOW && "Write Low");
  exit(0);
}

//main
int main(void)
{
  char* fileHandleGPIO_4;
  char* fileHandleGPIO_5;
  char* fileHandleGPIO_6;
  char* fileHandleGPIO_7;
  char* fileHandleGPIO_S; 

  fileHandleGPIO_4 = openGPIO(GP_4, GPIO_DIRECTION_OUT);
  fileHandleGPIO_5 = openGPIO(GP_5, GPIO_DIRECTION_OUT);
  fileHandleGPIO_6 = openGPIO(GP_6, GPIO_DIRECTION_OUT);
  fileHandleGPIO_7 = openGPIO(GP_7, GPIO_DIRECTION_OUT);
  fileHandleGPIO_S = openGPIO(STROBE, GPIO_DIRECTION_OUT);

  testGPIO(fileHandleGPIO_5);
  while(1)
  {
    //1.Strobe high
    //2.write data
    //3.Strobe low
    //4.Strobe high
  }
}


