#include <stdlib.h>
#include <stdbool.h>
#include <Arduino.h>
#include "Accelerometer.h"

float inVolt = 5;
float analogHigh = 1023;
float xadj = -0.07;             // Calibration Values for x
float yadj = -0.21;             // Calibration Values for y
float zadj = -0.17;             // Calibration Values for z
float oneG = 1.65;              // According to Accelerometer spec sheet a voltage of 1.65 is read at 1g.
float sensitivity = 0.800;      // According to spec sheet typical sensitivity = 0.800V/g
float avgXAcc;
float avgYAcc;
float avgZAcc;
/*************************************************************************
  * Function name: acceleration
  * Function inputs: byte* pin, float* adjust
  * Function outputs: float accel
  * Function description: Calculates the accerlation using the accelerometer
  *                       measurement.
  * Author(s): Leonard Shin, Leika Yamada
  ************************************************************************/
float acceleration(byte* pin, float* adjust){
     float myVolt = (((float)analogRead(*pin) * inVolt)/analogHigh)+ *adjust;       // Calculate the voltage input from analog input
     float accel = (myVolt - oneG)/sensitivity;                                     // Calculate acceleration from change in voltage and sensitivity
     return accel;
}
/*************************************************************************
  * Function name: angle
  * Function inputs: float* target
  * Function outputs: void
  * Function description: Calculates static angle of the target axis.
  * Author(s): Leonard Shin, Leika Yamada
  ************************************************************************/
float angle(float* target){
    double accMag = sqrt(pow(avgXAcc,2) + pow(avgYAcc,2) + pow(avgZAcc,2));         // Calculate the magnatude of the acceleration
    float targetVal = *target; 
    double myAngle = acos(((targetVal)/accMag))* 180.0/PI;                          // Calculate static angle from axis acceleration vector, and acceleration magnatude
    return (float) myAngle;
}
/*************************************************************************
  * Function name: accelerometerTask
  * Function inputs: void* accelerometerData
  * Function outputs: void
  * Function description: Calculates static angle, total distance traveled
  *                       and relative position.
  * Author(s): Leonard Shin, Leika Yamada
  ************************************************************************/
void accelerometerTask ( void* accelData ) {  

    accelerometerData* data = (accelerometerData*) accelData;
  
    float xArr[200];                                              // Initialize buffer for x axis
    float yArr[200];                                              // Initialize buffer for y axis
    float zArr[200];                                              // Initialize buffer for z axis
    for(int i = 0; i < 200; i++){
      xArr[i] = acceleration(data->xPin, &xadj);                  // Take 200 samples to calculate static angle
      yArr[i] = acceleration(data->yPin, &yadj);
      zArr[i] = acceleration(data->zPin, &zadj);  
    }
    avgXAcc = 0;
    avgYAcc = 0;
    avgZAcc = 0;
    
    for(int i = 0; i < 200; i++){                                 // Find average accerations
      avgXAcc = xArr[i] + avgXAcc;
      avgYAcc = yArr[i] + avgYAcc;
      avgZAcc = zArr[i] + avgZAcc;
    }
    avgXAcc = avgXAcc/200;
    avgYAcc = avgYAcc/200;
    avgZAcc = avgZAcc/200;
    
    *data->angleX = angle(&avgXAcc);                               // Calculate static angle and store in global var
    *data->angleY = angle(&avgYAcc);
    *data->angleZ = angle(&avgZAcc);
    
    
    return;
}
