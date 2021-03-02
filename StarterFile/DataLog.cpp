#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <pin_magic.h>
#include <registers.h>
#include <Arduino.h>
#include <EEPROM.h>
#include "DataLog.h"

/********************************************************************
  * Function name: eepromReset
  * Function inputs: void
  * Function outputs:void
  * Function description: Set EEPROM values to default. Voltage to -1,
  *                       current to 0, and temperature to 0.
  *                       voltage from location 
  * Author(s): Leonard Shin; Leika Yamada
  *******************************************************************/
void eepromReset () {
    //EEPROM.write(0, i);           // EEPROM write, address, int(0-255)
}
/********************************************************************
  * Function name: eepromCurrReset
  * Function inputs: void
  * Function outputs:void
  * Function description: Set EEPROM values to default. Voltage to -1,
  *                       current to 0, and temperature to 0.
  *                       voltage from location 
  * Author(s): Leonard Shin; Leika Yamada
  *******************************************************************/
void eepromCurrReset (float* minCurr, float* maxCurr) {
    //EEPROM.write(0, i);           // EEPROM write, address, int(0-255)
}
/********************************************************************
  * Function name: eepromVoltReset
  * Function inputs: void
  * Function outputs:void
  * Function description: Set EEPROM values to default. Voltage to -1,
  *                       current to 0, and temperature to 0.
  *                       voltage from location 
  * Author(s): Leonard Shin; Leika Yamada
  *******************************************************************/
void eepromVoltReset (float* minVolt, float* maxVolt) {
    //EEPROM.write(0, i);           // EEPROM write, address, int(0-255)
}
/********************************************************************
  * Function name: eepromTempReset
  * Function inputs: void
  * Function outputs:void
  * Function description: Set EEPROM values to default. Voltage to -1,
  *                       current to 0, and temperature to 0.
  *                       voltage from location 
  * Author(s): Leonard Shin; Leika Yamada
  *******************************************************************/
void eepromTempReset (float* minTemp, float* maxTemp) {
    //EEPROM.write(0, i);           // EEPROM write, address, int(0-255)
}
/**********************************************************************
  *  Function name: terminalTask
  *  Function inputs: void* tData
  *  Function outputs: void
  *  Function description: Prompts user to choose between 4 options, 
  *                        if option 1 is selected the EEPROM reset 
  *                        flag is raised. If option 2 is selected 
  *                        the historical min/max current is displayed, 
  *                        if 3 is slected min/max voltage, if 4 is 
  *                        selected min/max temperature.
  *  Author(s): Leonard Shin; Leika Yamada
  *********************************************************************/
void dataLogTask ( void* dData ) {
     logData* data = ( logData* ) dData;
     if(*(data->EEPROMReset) == true){
          *(data->EEPROMReset) = false;
          eepromReset();
      }else{
          if (*(data->tempChange) == true){
              *(data->tempChange) = false;
                eepromTempReset(data->minTemp, data->maxTemp);
            }
          if (*(data->voltChange) == true){
              *(data->voltChange) = false;
                eepromVoltReset(data->minVolt, data->maxVolt);
            }
          if (*(data->currChange) == true){
              *(data->currChange) = false;
                eepromCurrReset(data->minCurrent, data->maxCurrent);
            }   
      }
    return;
}
