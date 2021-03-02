#ifdef __cplusplus  
extern "C" { 
#endif 

#ifndef DATALOG_H_
#define DATALOG_H_


#include <stdlib.h>
#include <stdbool.h>
#include <Arduino.h>



typedef struct dataTaskData {      // Contains Terminal Data
    float* minTemp;
    float* maxTemp;
    float* minCurrent;
    float* maxCurrent;
    float* minVolt;
    float* maxVolt;
    bool* EEPROMReset;
    bool* tempChange;        // Flag to check if the min or max temperature has changed
    bool* voltChange;        // Flag to check if the min or max voltage has changed
    bool* currChange;        // Flag to check if the min or max current has changed
} logData;


void dataLogTask (void*);


#endif

#ifdef __cplusplus 
} 
#endif 
