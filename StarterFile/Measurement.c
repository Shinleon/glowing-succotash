#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <pin_magic.h>
#include <registers.h>
#include "Measurement.h"
#include "Arduino.h"


float read_range_high = 970;
float read_range_low = 13;
/**************************************************************************
  * Function name: updateHVIL
  * Function inputs: bool* hvilReading, const byte* pin
  * Function outputs: void
  * Function description: changes hvilReading to represent the
  *                      voltage (by expressing 0 or 1) on the provided pin
  * Author(s): Leonard Shin; Leika Yamada
  *************************************************************************/
void updateHVIL ( bool* hvilReading, const int* pin ) {
  
    *hvilReading = digitalRead(*pin);
    return;
}

/***************************************************************************
  * Function name: updateTemperature
  * Function inputs: float* temperatureReading
  * Function outputs: ~
  * Function description:  alters temperatureReading to cycle between
  *                       [-10, 5, 25], switching value every 1 sec.
  *                       Also check current readings against historical 
  *                       min and max sets the eeprom flag if a historical min
  *                       or max is acheived.
  * Author(s):  Leonard Shin; Leika Yamada
  **************************************************************************/
void updateTemperature ( float* temperatureReading, const byte* pin, float* minTemp, float* maxTemp, bool* tempChange ) {
    float lowest_temp = -10;
    float highest_temp = 45;
    //float slope = (highest_temp-lowest_temp)/read_range;
    //float scaling = slope*analogRead(*pin)+lowest_temp; 
    *temperatureReading = (((analogRead(*pin) - read_range_low)*(highest_temp-lowest_temp))/(read_range_high - read_range_low))+lowest_temp;

    if(*minTemp > *temperatureReading){
        *minTemp = *temperatureReading;
        *tempChange = true;
    }else if(*maxTemp < *temperatureReading){
        *maxTemp = *temperatureReading;
        *tempChange = true;  
    }
    return;
}

/*******************************************************************
  *  Function name: updateHvCurrent
  *  Function inputs: float* currentReading
  *  Function outputs: ~
  *  Function description:  alters currentReading to cycle between
  *                       [-20, 0, 20], switching value every 2 sec
  *                       Also check current readings against historical 
  *                       min and max sets the eeprom flag if a historical min
  *                       or max is acheived.
  *  Author(s):  Leonard Shin; Leika Yamada
  ******************************************************************/
void updateHvCurrent ( float* currentReading, const byte* pin, float* minCurrent, float* maxCurrent, bool* currChange  ) {
    float lowest_curr = -25;
    float highest_curr = 25;
    //float slope = (highest_curr-lowest_curr)/read_range;
    //float scaling = slope*analogRead(*pin)+lowest_curr; 
    *currentReading =(((analogRead(*pin) - read_range_low)*(highest_curr-lowest_curr))/(read_range_high - read_range_low))+lowest_curr;

    if(*minCurrent > *currentReading){
        *minCurrent = *currentReading;
        *currChange = true;
    }else if(*maxCurrent < *currentReading){
        *maxCurrent = *currentReading;
        *currChange = true;  
    }
    
    return;
}
/********************************************************************
  * Function name: updateHvVoltage
  * Function inputs: float* voltageReading
  * Function outputs: ~
  * Function description: alters voltageReading to cycle between
  *                       [10, 150, 450], switching value every 3 sec
  *                       Also check current readings against historical 
  *                       min and max sets the eeprom flag if a historical min
  *                       or max is acheived.
  * Author(s): Leonard Shin; Leika Yamada
  *******************************************************************/
void updateHvVoltage ( float* voltageReading, const byte* pin, float* minVolt, float* maxVolt, bool* voltChange  ) {
    //float lowest_volt = 0;
    float highest_volt = 450;
    //float slope = (highest_volt-lowest_volt)/read_range;
    //float scaling = slope*analogRead(*pin)+lowest_volt; 
    //*voltageReading = scaling;
    
    *voltageReading = abs(analogRead(*pin) - read_range_low)*(highest_volt/read_range_high);

    if(*minVolt > *voltageReading || -1 == *minVolt){
        *minVolt = *voltageReading;
        *voltChange = true;
    }else if(*maxVolt < *voltageReading || -1 == *maxVolt){
        *maxVolt = *voltageReading;
        *voltChange = true;  
    }
    
    return;
}

/********************************************************************
  * Function name: resetHistory
  * Function inputs: float* minTemp, float* maxTemp, float* minCurrent, 
  *                  float* maxCurrent, float* minVolt, float* maxVolt
  * Function outputs: void
  * Function description: Resets the historical min and max of
  *                       current, voltage, and temperature to the
  *                       default values: Zero, -1, Zero.
  * Author(s): Leonard Shin; Leika Yamada
  *******************************************************************/
void resetHistory ( float* minTemp, float* maxTemp, float* minCurrent, float* maxCurrent, float* minVolt, float* maxVolt ) {
    *minTemp = 0;
    *maxTemp = 0;
    *minCurrent = 0;
    *maxCurrent = 0;
    *minVolt = -1;
    *maxVolt = -1;
    return;
}

/**********************************************************************
  *  Function name: measurementTasks
  *  Function inputs: void* mData
  *  Function outputs: ~
  *  Function description: modifies mData to represent
  *                        the data at the current time point
  *  Author(s): Leonard Shin; Leika Yamada
  *********************************************************************/
void measurementTask ( void* mData ) {
    measurementData* data = (measurementData*) mData;

    if(*(data->EEPROMReset) == true){
        resetHistory(data->minTemp, data->maxTemp, data->minCurrent, data->maxCurrent, data->minVolt, data->maxVolt );
    }

    // Update all sensors
    updateHVIL(data->hvilStatus, data->hvilPin);
    updateTemperature(data->temperature, data->tempPin, data->minTemp, data->maxTemp, data->tempChange);
    updateHvCurrent(data->hvCurrent, data->currPin, data->minCurrent, data->maxCurrent, data->currChange);
    updateHvVoltage(data->hvVoltage, data->voltPin, data->minVolt, data->maxVolt, data->voltChange);
    
    return;
}
