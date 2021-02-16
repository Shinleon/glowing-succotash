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

}

/***************************************************************************
  * Function name: updateTemperature
  * Function inputs: float* temperatureReading
  * Function outputs: ~
  * Function description:  alters temperatureReading to cycle between
  *                       [-10, 5, 25], switching value every 1 sec
  * Author(s):  Leonard Shin; Leika Yamada
  **************************************************************************/
void updateTemperature ( float* temperatureReading, const byte* pin ) {
    float lowest_temp = -10;
    float highest_temp = 45;
    //float slope = (highest_temp-lowest_temp)/read_range;
    //float scaling = slope*analogRead(*pin)+lowest_temp; 
    *temperatureReading = (((analogRead(*pin) - read_range_low)*(highest_temp-lowest_temp))/(read_range_high - read_range_low))+lowest_temp;
}

/*******************************************************************
  *  Function name: updateHvCurrent
  *  Function inputs: float* currentReading
  *  Function outputs: ~
  *  Function description:  alters currentReading to cycle between
  *                       [-20, 0, 20], switching value every 2 sec
  *  Author(s):  Leonard Shin; Leika Yamada
  ******************************************************************/
void updateHvCurrent ( float* currentReading, const byte* pin  ) {
    float lowest_curr = -25;
    float highest_curr = 25;
    //float slope = (highest_curr-lowest_curr)/read_range;
    //float scaling = slope*analogRead(*pin)+lowest_curr; 
    *currentReading =(((analogRead(*pin) - read_range_low)*(highest_curr-lowest_curr))/(read_range_high - read_range_low))+lowest_curr;
    
    
}
/********************************************************************
  * Function name: updateHvVoltage
  * Function inputs: float* voltageReading
  * Function outputs: ~
  * Function description: alters voltageReading to cycle between
  *                       [10, 150, 450], switching value every 3 sec
  * Author(s): Leonard Shin; Leika Yamada
  *******************************************************************/
void updateHvVoltage ( float* voltageReading, const byte* pin  ) {
    //float lowest_volt = 0;
    float highest_volt = 450;
    //float slope = (highest_volt-lowest_volt)/read_range;
    //float scaling = slope*analogRead(*pin)+lowest_volt; 
    //*voltageReading = scaling;
    
    *voltageReading = abs(analogRead(*pin) - read_range_low)*(highest_volt/read_range_high);
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
  
    // Update all sensors
    updateHVIL(data->hvilStatus, data->hvilPin);
    updateTemperature(data->temperature, data->tempPin);
    updateHvCurrent(data->hvCurrent, data->currPin);
    updateHvVoltage(data->hvVoltage, data->voltPin);
  
    return;
}
