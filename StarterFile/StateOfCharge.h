#ifdef __cplusplus  
extern "C" { 
#endif 

#ifndef STATEOFCHARGE_H_
#define STATEOFCHARGE_H_

                                            //Define the three different values to cycle through
#define EMPTY 0
#define HALF  50
#define FULL  100

#include <stdlib.h>
#include <stdbool.h>
#include <Arduino.h>


typedef struct stateOfChargeTaskData {      // Defines a data struct to hold the SOC
  
    float* stateOfCharge;
    
} stateOfChargeData;


void stateOfChargeTask (void*);


#endif

#ifdef __cplusplus 
} 
#endif 
