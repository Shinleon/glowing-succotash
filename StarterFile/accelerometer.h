#ifdef __cplusplus                    // Tells compiler the file is .c
extern "C" { 
#endif 

#ifndef ACCELEROMETER_H_
#define ACCELEROMETER_H_


#include <stdlib.h>
#include <stdbool.h>
#include <Arduino.h>


typedef struct accelerometerTaskData {  // Structure that holds contactor data
    float* relX;                 // Relative positon, X-axis
    float* relY;                 // Relative positon, Y-axis
    float* relZ;                 // Relative positon, Z-axis
    float* totalDist;            // Total distance traveled
    float* angleX;               // Static angle of X
    float* angleY;               // Static angle of Y
    float* angleZ;               // Static angle of Z
} accelerometerData;


void accelerometerTask (void*);         // Update contactor status


#endif

#ifdef __cplusplus 
} 
#endif 
