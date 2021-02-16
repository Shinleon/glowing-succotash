#ifdef __cplusplus  
extern "C" { 
#endif 

#ifndef ALARM_H_
#define ALARM_H_

#include <stdlib.h>
#include <stdbool.h>
#include <Arduino.h>


#define NOT_ACTIVE      0
#define ACTIVE_NO_ACK   1
#define ACTIVE_ACK      2


typedef struct alarmTaskData {
    volatile byte* hVoltInterlock;           // Store HVIL Status, over current, HV out of range
    byte* overCurrent;              // 0 for alarm not active, 1, for active not acknowledged          
    byte* hVoltOutofRange;          // and 2 for active acknowledged
    bool* acknowledgeFlag;
    bool* hVIL;
} alarmData;


void alarmTask (void*);             // Updates alarm task, function header

#endif

#ifdef __cplusplus 
} 
#endif 
