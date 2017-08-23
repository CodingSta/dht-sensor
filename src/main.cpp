#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dht_read.h"

void printUsage(char* progName) {
    printf("Usage:\n");
    printf("\t%s <gpio> <DHT11 || DHT22> [  || json]\n", progName);
    printf("\tOptional third argument specifies json output");
    printf("\n");
}

int main(int argc, char* argv[])
{
  int   gpioNum       = -1;
  int   sensorType    = -1;
  int   bJsonOutput   = 0;

  int   maxRetry      = 3;
  int   result        = 0;

  float humidity      = 0.0f;
  float temperature   = 0.0f;


  // check for the correct number of arguments
  if ( argc < 3 )
  {
    printUsage(argv[0]);
    return EXIT_FAILURE;
  }

  //// parse arguments
  // gpio number
  gpioNum = atoi(argv[1]);
  // sensor type
  if(strcmp(argv[2], "DHT11") == 0)
  {
    sensorType = DHT11;
  }
  else if(strcmp(argv[2], "DHT22") == 0)
  {
    sensorType = DHT22;
  }
  // output type
  if (argc > 3 && strcmp(argv[3], "json")) {
    bJsonOutput = 1;
  }
  // check for correct arguments
  if (gpioNum == -1) {
    printf("ERROR: Invalid GPIO specified\n");
    printUsage(argv[0]);
    return EXIT_FAILURE;
  }
  if (sensorType == -1) {
    printf("ERROR: Invalid DHT sensor, 'DHT11' or 'DHT22' are valid\n");
    printUsage(argv[0]);
    return EXIT_FAILURE;
  }

  //// perform the measurement
  do
  {
    result = dht_read(sensorType, gpioNum, &humidity, &temperature);
    maxRetry--;
  } while(result != 0 && maxRetry > 0);

  //// output the reading
  if (bJsonOutput) {
      printf("{\"humidity\": %f, \"temperature\": %f}\n", humidity, temperature);
  }
  else {
      printf("humidity: %f\ntemperature: %f\n", humidity, temperature);
  }

  // return based on if the read was successful or not
  if(result == 0)
  {
    return EXIT_SUCCESS;
  }
  else
  {
    return EXIT_FAILURE;
  }
}
