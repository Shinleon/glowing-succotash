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
    const byte* tempPin;
  	float* hvCurrent;
    const byte* currPin;
	  float* hvVoltage;
    const byte* voltPin;
    bool* EEPROMReset;
    float* minTemp;
    float* maxTemp;
    float* minCurrent;
    float* maxCurrent;
    float* minVolt;
    float* maxVolt;
    bool* tempChange;        // Flag to check if the min or max temperature has changed
    bool* voltChange;        // Flag to check if the min or max voltage has changed
    bool* currChange;        // Flag to check if the min or max current has changed
} measurementData;


void measurementTask (void*);


#endif

#ifdef __cplusplus 
} 
#endif 
