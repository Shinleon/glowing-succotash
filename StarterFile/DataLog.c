#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <pin_magic.h>
#include <registers.h>
#include <Arduino.h>
#include "DataLog.h"

/********************************************************************
  * Function name: terminalPrompt
  * Function inputs: void
  * Function outputs: int The users choice (1 - 4)
  * Function description: Prompts user to choose between 4 options the 
  *                       chosen option is returned by the funciton.
  * Author(s): Leonard Shin; Leika Yamada
  *******************************************************************/
void eepromTask () {
    
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
    
    return;
}
