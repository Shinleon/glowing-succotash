#ifdef __cplusplus  
extern "C" { 
#endif 

#ifndef STATEOFCHARGE_H_
#define STATEOFCHARGE_H_


#include <stdlib.h>
#include <stdbool.h>
#include <Arduino.h>


int volt_length = 5;
int voltages_soc[] = {200, 250, 300, 350, 400};   // columns
int temp_length = 4;
int temperatures_soc[] = {-10, 0, 25, 45};        // rows
int percent_soc[][] = {
                   {  0,  0,  35, 100, 100},
                   {  0,  0,  20,  80, 100},
                   {  0,  0,  10,  60, 100},
                   {  0,  0,   0,  50, 100}
                  };

typedef struct stateOfChargeTaskData {      // Defines a data struct to hold the SOC
  
    float* vTerminal;
    float* iTerminal;
    float* temp;
    float* soc;
    
} stateOfChargeData;


void stateOfChargeTask (void*);


#endif

#ifdef __cplusplus 
} 
#endif 
