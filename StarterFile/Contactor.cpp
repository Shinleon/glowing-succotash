#include <stdlib.h>
#include <stdbool.h>
#include <Arduino.h>
#include "Contactor.h"

/******************************************************************
  * Function name: updateContactor
  * Function inputs: bool* contactorStatus, int* contactorLED
  * Function outputs: void
  * Function description: updates the conctactor LED's value
  *                       based upon the contactor status (changes
  *                       the contactor signal into an output)
  * Author(s): Leonard Shin, Leika Yamada
  *****************************************************************/
void updateContactor ( bool* contactorStatus, bool* local, bool* ack, int* contactorLED ) {
        // Need to ack change if it was changed
    if(*contactorStatus != *local){
        *local = *contactorStatus;
        *ack = true; 
    }
    if( *contactorStatus == 0 ){
        
        digitalWrite(*contactorLED, LOW);
    }
    else{
        digitalWrite(*contactorLED, HIGH);
    }
      
    return;
}

/*************************************************************************
  * Function name: contactorTask
  * Function inputs: void* contactData
  * Function outputs: void
  * Function description: Makes the contactorTask update all of it's data
  * Author(s): Leonard Shin, Leika Yamada
  ************************************************************************/
void contactorTask ( void* contactData ) {
  
    contactorData* data = (contactorData*) contactData;
    
    updateContactor(data->contactorStatus, data->localContactor, data->acknowledge, data->contactorLED);   // Update all sensors
    
    return;
}
