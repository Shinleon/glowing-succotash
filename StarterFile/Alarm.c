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
void updateOverCurrent ( byte* overCurrent, bool* acknowledgeFlag ) {

    //extern byte clockTick;
    //if( clockTick/2 % 3 == 0 ){
    //    *overCurrent = NOT_ACTIVE;
    //}
    //else if ( clockTick/2 %3 == 1 ){
    //    *overCurrent = ACTIVE_NO_ACK;
    //}
    //else{
    //    *overCurrent = ACTIVE_ACK;
    //}
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
void updateHVoltOutofRange ( byte* hVoltOutofRange, bool* acknowledgeFlag ) {
    
    //extern byte clockTick;
    //if( clockTick/3 % 3 == 0 ){
    //    *hVoltOutofRange = NOT_ACTIVE;
    //}
    //else if ( clockTick/3 %3 == 1 ){
    //    *hVoltOutofRange = ACTIVE_NO_ACK;
    //}
    //else{
    //    *hVoltOutofRange = ACTIVE_ACK;
    //}
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
    updateOverCurrent(data->overCurrent, data->acknowledgeFlag);
    updateHVoltOutofRange(data->hVoltOutofRange, data->acknowledgeFlag);
    
    return;
}
