#include <stdlib.h>
#include <stdbool.h>
#include "StateOfCharge.h"

/**********************************************************************
  * Function name: updateStateOfCharge
  * Function inputs: float* theStateOfCharge
  * Function outputs: ~
  * Function description: places the current state of charge value in
  *                       the provided parameter, theStateOfCharge.
  *                       Currently just cycles between [0, 50, 100]
  *                       every 3 seconds
  * Author(s): Leonard Shin, Leika Yamada
  **********************************************************************/
void updateStateOfCharge ( float* theStateOfCharge ) {
    extern byte clockTick;
    if(clockTick%3 == 0x0){
        *theStateOfCharge = EMPTY;
      } 
      else if(clockTick%3 == 0x1){
        *theStateOfCharge = HALF;
      }
      else {
        *theStateOfCharge = FULL;
      }
    return;
}

/***********************************************************************
  * Function name: stateOfChargeTask
  * Function inputs: generic pointer to the updated state of charge data
  * Function outputs: ~
  * Function description: This function calls an update to in order to
  *                       update the state of charge. It cycles through
  *                       the values 0, 50, and 100 at a 1 second rate.
  * Author(s): Leonard Shin, Leika Yamada
  **********************************************************************/
void stateOfChargeTask ( void* socData ) {
  
    stateOfChargeData* data = (stateOfChargeData*) socData;
    
                                                                          // Update all sensors
    updateStateOfCharge(data->stateOfCharge);
    
  return;
}
