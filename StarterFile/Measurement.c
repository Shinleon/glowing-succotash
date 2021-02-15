#include <stdlib.h>
#include <stdbool.h>
#include "Measurement.h"
#include "Arduino.h"


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
void updateTemperature ( float* temperatureReading ) {
  
    extern byte clockTick;
    if ( clockTick % 3 == 0 ) {
        *temperatureReading = -10;
    }
    else if ( clockTick % 3 == 1 ) {
        *temperatureReading = 5;
    }
    else {
        *temperatureReading = 25;
    }
}

/*******************************************************************
  *  Function name: updateHvCurrent
  *  Function inputs: float* currentReading
  *  Function outputs: ~
  *  Function description:  alters currentReading to cycle between
  *                       [-20, 0, 20], switching value every 2 sec
  *  Author(s):  Leonard Shin; Leika Yamada
  ******************************************************************/
void updateHvCurrent ( float* currentReading ) {
  
    extern byte clockTick;
    if (clockTick / 2 % 3 == 0) {
        *currentReading = -20;
    }
    else if (clockTick / 2 % 3 == 1) {
        *currentReading = 0;
    }
    else {
        *currentReading = 20;
    }
}
/********************************************************************
  * Function name: updateHvVoltage
  * Function inputs: float* voltageReading
  * Function outputs: ~
  * Function description: alters voltageReading to cycle between
  *                       [10, 150, 450], switching value every 3 sec
  * Author(s): Leonard Shin; Leika Yamada
  *******************************************************************/
void updateHvVoltage ( float* voltageReading ) {

    extern byte clockTick;
    if (clockTick / 3 % 3 == 0) {
        *voltageReading = 10;
    }
    else if (clockTick / 3 % 3 == 1) {
        *voltageReading = 150;
    }
    else {
        *voltageReading = 450;
    }
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
    updateTemperature(data->temperature);
    updateHvCurrent(data->hvCurrent);
    updateHvVoltage(data->hvVoltage);
  
    return;
}
