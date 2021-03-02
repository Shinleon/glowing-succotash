#ifdef __cplusplus  
extern "C" { 
#endif 

#ifndef DATALOG_H_
#define DATALOG_H_


#include <stdlib.h>
#include <stdbool.h>
#include <Arduino.h>


typedef struct dataTaskData {      // Contains Terminal Data
    //float* minTemp;
    //float* maxTemp;
    //float* minCurrent;
    //float* maxCurrent;
    //float* minVolt;
    //float* maxVolt;
    //bool* EEPROMReset;
} logData;


void dataLogTask (void*);


#endif

#ifdef __cplusplus 
} 
#endif 
