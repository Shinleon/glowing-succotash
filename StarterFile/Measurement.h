#ifdef __cplusplus  
extern "C" { 
#endif 

#ifndef MEASUREMENT_H_
#define MEASUREMENT_H_


#define HVIL_OPEN   false
#define HVIL_CLOSED true

#include <stdlib.h>
#include <stdbool.h>
#include <Arduino.h>


typedef struct measurementTaskData {      // Contains Measurement Data
    bool* hvilStatus;
    const int* hvilPin;
    float* temperature;
  	float* hvCurrent;
	  float* hvVoltage;
} measurementData;


void measurementTask (void*);


#endif

#ifdef __cplusplus 
} 
#endif 
