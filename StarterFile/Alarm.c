#include <stdlib.h>
#include <stdbool.h>
#include "Alarm.h"
#include "Arduino.h"

/*****************************************************************
  * Function name: updateHVoltInterlockAlrm
  * Function inputs: byte* hVoltInterlock
  * Function outputs: void
  * Function description: changes the HVInterlock pointer's value
  *                       to cycle between [0, 1, 2] every second
  * Author(s): Leonard Shin; Leika Yamada
  *****************************************************************/
void updateHVoltInterlockAlarm ( volatile byte* hVoltInterlock, bool* acknowledgeFlag ) {
    // 
    // 
    // extern byte clockTick;
    //if( clockTick % 3 == 0 ){
      //  *hVoltInterlock = NOT_ACTIVE;
   // }
   // else if ( clockTick %3 == 1 ){
    //    *hVoltInterlock = ACTIVE_NO_ACK;
    //}
    //else{
    //    *hVoltInterlock = ACTIVE_ACK;
    //}
    if(*acknowledgeFlag == 1 && *hVoltInterlock == ACTIVE_NO_ACK ){
          *hVoltInterlock = ACTIVE_ACK;
    }
}

/**********************************************************************
  * Function name: updateHVoltInterlockAlrm
  * Function inputs: byte* OverCurrent
  * Function outputs: void
  * Function description: changes the OverCurrent's pointer's value
  *                       to cycle between [0, 1, 2] every two seconds
  * Author(s): Leonard Shin; Leika Yamada
  *********************************************************************/
void updateOverCurrent (float* curr, byte* overCurrent, bool* acknowledgeFlag ) {

    float curr_limit_l = -5;
    float curr_limit_r = 20;
    if((*curr < curr_limit_l || *curr > curr_limit_r) && *acknowledgeFlag == 0)
    {
        *overCurrent = ACTIVE_NO_ACK;
    }
    if(*acknowledgeFlag == 1 && *overCurrent == ACTIVE_NO_ACK ){
          *overCurrent = ACTIVE_ACK;
    }
}

/************************************************************************
  * Function name: updateHVoltOutofRange
  * Function inputs: byte* HVoltOutofRange
  * Function outputs: void
  * Function description: Changes the HVoltOutofRange pointer's value
  *                       to cycle between [0, 1, 2] every three seconds
  * Author(s): Leonard Shin; Leika Yamada
  **********************************************************************/
void updateHVoltOutofRange (float* volt, byte* hVoltOutofRange, bool* acknowledgeFlag ) {
    
    float volt_limit_l = 280;
    float volt_limit_r = 405;
    if((*volt < volt_limit_l || *volt > volt_limit_r) && *acknowledgeFlag == 0)
    {
        *hVoltOutofRange = ACTIVE_NO_ACK;
    }
    if(*acknowledgeFlag == 1 && *hVoltOutofRange == ACTIVE_NO_ACK ){
          *hVoltOutofRange = ACTIVE_ACK;
    }
}

/*****************************************************************
  * Function name: measurementTask
  * Function inputs: void* mData
  * Function outputs: void
  * Function description: Modifies mData to represent
  *                       the Alarm data at the current time point
  * Author(s): Leonard Shin; Leika Yamada
  ****************************************************************/
void alarmTask ( void* mData ) {
    
    alarmData* data = (alarmData*) mData;
    
    /* Update all sensors */
    updateHVoltInterlockAlarm(data->hVoltInterlock, data->acknowledgeFlag);
    updateOverCurrent(data->current, data->overCurrent, data->acknowledgeFlag);
    updateHVoltOutofRange(data->voltage, data->hVoltOutofRange, data->acknowledgeFlag);
    
    return;
}
