#include <TimerOne.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "Measurement.h"
#include "TaskControlBlock.h"
#include "StateOfCharge.h"
#include "Contactor.h"
#include "Display.h"
#include "Alarm.h"
#include "RemoteTerminal.h"
#include "DataLog.h"


#include <pin_magic.h>
#include <registers.h>
#include <Elegoo_GFX.h>         // Core graphics library
#include <Elegoo_TFTLCD.h>      // Hardware-specific library


#define NOT_ACTIVE      0
#define ACTIVE_NO_ACK   1
#define ACTIVE_ACK      2

/* The control pins for the LCD can be assigned to any digital or
 * analog pins...but we'll use the analog pins as this allows us to
 * double up the pins with the touch screen (see the TFT paint example).*/

#define LCD_CS A3     // Chip Select goes to Analog 3
#define LCD_CD A2     // Command/Data goes to Analog 2
#define LCD_WR A1     // LCD Write goes to Analog 1
#define LCD_RD A0     // LCD Read goes to Analog 0

#define LCD_RESET A4  // Can alternately just connect to Arduino's reset pin

#define YP A3         // must be an analog pin, use "An" notation!
#define XM A2         // must be an analog pin, use "An" notation!
#define YM 9          // can be a digital pin
#define XP 8          // can be a digital pin


#define MEASURE 0x00  // Used to keep track of which screen is displayed: Measurement screen
#define ALARM 0x01    // Used to keep track of which screen is displayed: Alarm screen
#define BATTERY 0x02  // Used to keep track of which screen is displayed: Battery screen

#define SOC 0                   // Constant SOC value

                                // EEPROM Reset values
#define CURR_RESET 0            // HV current reset value
#define VOLT_RESET -1           // HV Voltage reset value
#define TEMP_RESET 0            // Temperature reset value

                                // Task Control Blocks
TCB measurementTCB;             // Declare measurement TCB
TCB stateOfChargeTCB;           // Declare state of charge TCB
TCB contactorTCB;               // Declare contactor TCB
TCB alarmTCB;                   // Declare alarm TCB
TCB displayTCB;                 // Declare display TCB   [Display should be last task done each cycle]
TCB terminalTCB;                // Declare remote terminal TCB
TCB datalogTCB;                 // Declare data logger TCB

bool EEPROMReset = true;        // Flag to check if the user wants to reset EEPROM
terminalData terminal;          // Remote terminal data struct
logData dataLog;                // DataLog data struct

                                // Measurement Data
measurementData measure;        // Declare measurement data structure - defined in Measurement.h
float temperature   = 0;        // Stores the measured temperature of the system
byte tempPin =  A11;            // <- That's an A-eleven, not an all
float hvCurrent     = 0;        // Stores the measured current in the HVIL
byte currPin = A12;
float hvVoltage     = 0;        // Stores the measured voltage in the HVIL
byte voltPin = A13;
bool hVIL           = 0;        // Stores whether or not the HVIL is closed(0) or open(1) *Switched due to pullup
const int hvilPin   = 21;       // Stores the input pin number for HVIL
float minTemp;                  // Stores the minimum temperature
float maxTemp;                  // Stores the maximum temperature
float minCurrent;               // Stores the minimum HV current
float maxCurrent;               // Stores the maximum HV current
float minVolt;                  // Stores the minimum HV voltage
float maxVolt;                  // Stores the maximum HV voltage
bool tempChange = false;        // Flag to check if the min or max temperature has changed
bool voltChange = false;        // Flag to check if the min or max voltage has changed
bool currChange = false;        // Flag to check if the min or max current has changed
                               
alarmData alarmStatus;                // Declare an Alarm data structure - defined in Alarm.h
volatile byte hVoltInterlock = 0;     // Store the alarm status for the HVIL alarm
byte overCurrent = 0;                 // Store the overcurretn alarm status
byte hVoltOutofRange = 0;             // Store alarm status for HV out of range
bool acknowledgeFlag = 0;

                                      // State Of Charge Data
stateOfChargeData chargeState;        // Declare charge state data structure
float stateOfCharge = SOC;        

                                      // Contactor Data
contactorData contactState;
bool contactorState = 0;        
int contactorLED = 53;                // Store the output pin for the contactor
bool contactorLocal = contactorState; // initialize local to be same as state
bool contactorAck = 0;

displayData displayUpdates;                                     // Display Data structure
Elegoo_TFTLCD tft(LCD_CS, LCD_CD, LCD_WR, LCD_RD, LCD_RESET);   // LCD touchscreen
TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);              // Touch screen input object
                                                                         

Elegoo_GFX_Button buttons[3];                                 // Create an array of button objects for the display
char buttonlabels[3][9]   = {"Measures", "Alarms", "Battery"};  
uint16_t buttoncolors[3]  = {CYAN, CYAN, CYAN};
bool measureButton = 0;                                      // Flag is true when the measuremnt screen button is pushed 
bool batteryButton = 0;                                      // Flag is true when the battery screen button is pushed
bool alarmButton = 0;                                        // Flag is true when the alarm screen button is pushed
byte currentScreen = 0;                                      // Stores which screen the user is on, 0 for measurement, 1 for alarm, 2 for battery

/*Battery Screen buttons*/
Elegoo_GFX_Button batteryButtons[2];                         // Creates an array of buttons for the battery ON, OFF buttons
char batteryButtonLabels[2][4] = {"OFF", "ON"};

/*Alarm Screen button*/
Elegoo_GFX_Button alarmButtons[1];                           // Creates an alarm button array
char alarmButtonLabels[1][12] = {"Ack. All"};

/*Timer Initialization*/
volatile bool timeBaseFlag = 0;                              // Global time base flag
volatile bool myHvilStat = 0;                                // Hvil status
TCB* head = null;
byte tenths = 0;
/************************************************************************************
  * Function name: loop
  * Function inputs: void
  * Function outputs: void
  * Function description: Display data and lights indicating alarm status, contactor 
  *                       status, sensor data, & state of charge
  *                       This is a doubly linked list scheduler to run a series of 
  *                       tasks that create a user interface to a battery management system
  * Author(s): Leonard Shin, Leika Yamada
  **********************************************************************************/
void loop() {
    while( 1 ){
        if( 1 == timeBaseFlag)  // check if timeBaseFlag has been set by the timer
        {                       // interrupt (interrupt runs once per global time
                                // time base period)
            timeBaseFlag = 0;
            tenths  = (tenths + 1) % 50;
            /*serialMonitor();*/// uncomment for debug code
            scheduler();    
            
        }                                                         
    }
    return;
}
/******************************************************************************
  * Function name:    scheduler
  * Function inputs:  void
  * Function outputs: void
  * Function description: This runs all the tasks calling the doubly linked list.
  * Author(s): Leonard Shin, Leika Yamada
  ******************************************************************************/
void scheduler() {
    //TCB* curr = &measurementTCB;   // &measurementTCB is the start point
    // {&measurementTCB, &stateOfChargeTCB, &alarmTCB, &contactorTCB, &EEPROM_shit, &terminalTCB, &displayTCB}
    TCB* curr = head;

    while(curr != NULL)
    {
        temp = curr->next;         // make temp variable for next
        temp->prev = null;         // delete back arrow from next
        curr->next = null;         // delete forard connection on current
        curr->task(curr->taskDataPtr);
        curr = temp;               // step forward
    }
    // reconstruct 
    head = &measurementTCB;   //always start with measurement task
    curr = head;
    
    curr->next = &stateofChargeTCB;
    curr->next->prev = curr;
    curr = curr->next;
    
    curr->next = &alarmTCB;
    curr->next->prev = curr;
    curr = curr->next;
    
    curr->next = &contactorTCB;
    curr->next->prev = curr;
    curr = curr->next;

    if(tenths % 50 == 0 )
    {
       // Do the EEPROM read/write given that the EEPROM needs to be chaged
       //   aka. there is a new min or max for any of the measures
    }

    if(tenths % 10 == 0 )
    {
       // Do the EEPROM read/write given that the EEPROM needs to be chaged
       //   aka. there is a new min or max for any of the measures
    }

    
    return;
}

/******************************************************************************
  * Function name:    timerISR
  * Function inputs:  void
  * Function outputs: void
  * Function description: Set the flag to run the interrupt service routine. Timer 
  *                       will be set to run once every 100 milliseconds.
  * Author(s): Leonard Shin, Leika Yamada
  ******************************************************************************/
void timerISR() {   // interrupt service routine
  
    timeBaseFlag = 1;    // set timerISR flag
    return;
    
}
/******************************************************************************
  * Function name:    hvilISR
  * Function inputs:  void
  * Function outputs: void
  * Function description: Set hvil alarm status to active not acknowledged. 
  *                       Turn off contactor. Set contactor state to 0.
  * Author(s): Leonard Shin, Leika Yamada
  ******************************************************************************/
void hvilISR() {   // interrupt service routine for the hvil
  
    hVoltInterlock = ACTIVE_NO_ACK;
    digitalWrite(contactorLED, LOW);
    contactorState = 0;
    return;
    
}
/******************************************************************************
  * Function name:    serialMonitor
  * Function inputs:  void
  * Function outputs: void
  * Function description: This displays all of our processes to the serial
  *                       display. Used for purposes of debugging the system.
  * Author(s): Leonard Shin, Leika Yamada
  ******************************************************************************/
void serialMonitor() {
  
      Serial.print("My State of Charge is: ");
      Serial.println(SOC, DEC);
      Serial.print("\n");

      Serial.print("My HVIL_alarm is: ");
      Serial.println(hVoltInterlock, DEC);
      Serial.print("My Overcurrent_alarm is: ");
      Serial.println(overCurrent, DEC);
      Serial.print("My HVOutofRange_alarm is: ");
      Serial.println(hVoltOutofRange, DEC);

      Serial.print("My Temperature is: ");
      Serial.println(temperature, DEC);
      Serial.print("My Current is: ");
      Serial.println(hvCurrent, DEC);
      Serial.print("My Voltage is: ");
      Serial.println(hvVoltage, DEC);
      Serial.print("My HVIL input is: ");
      Serial.println(hVIL, DEC);
}


/********************************************************************
  * Function name: setup
  * Function inputs: void
  * Function outputs: void
  * Function description: Initializes all  TCBs and the scheduler.
  *                       Sets up the TFT LCD screen to display and 
  *                       receive real time data. 
  * Author(s): Leonard Shin, Leika Yamada
  *******************************************************************/
void setup() {  

    /*Initailize input and output pins*/
    pinMode(hvilPin, INPUT_PULLUP);
    pinMode(contactorLED, OUTPUT);
    pinMode(tempPin, INPUT_PULLUP);
    pinMode(currPin, INPUT_PULLUP);
    pinMode(voltPin, INPUT_PULLUP);
    
    /*Initialize the Global time base Timer*/
    Timer1.initialize(100000);
    Timer1.attachInterrupt(timerISR);
    Timer1.start();
    
    /*Initailize HVIL Timer*/
    attachInterrupt(digitalPinToInterrupt(hvilPin), hvilISR , RISING);
    interrupts();

       
    /* Initialize Measurement & Sensors*/
    measure = {&hVIL, &hvilPin, &temperature, &tempPin, &hvCurrent, &currPin, &hvVoltage, &voltPin};  // Initailize measure data struct with data
    measurementTCB.task = &measurementTask;                             // Store a pointer to the measurementTask update function in the TCB
    measurementTCB.taskDataPtr = &measure;                                            

   
    /*Initialize Display*/
    displayUpdates = {&hvilPin, &contactorState, &contactorLED, &hvCurrent, &hvVoltage, &temperature, &hVIL, &hVoltInterlock, &overCurrent, &hVoltOutofRange, &stateOfCharge, &contactorAck, &acknowledgeFlag};        // Initialize display data struct with data    
    displayTCB.task = &displayTask;                                     // Store a pointer to the displayTask update function in the TCB
    displayTCB.taskDataPtr = &displayUpdates;
 
    /*Initialize Touch Input*/
    measureButton = 1;                                                  // Initalize the measure button as pressed to start display with measure screen
    batteryButton = 0;                                                  // Battery button initialized as not pressed
    alarmButton = 0;                                                    // Alarm screen button initialized as not pressed
    currentScreen = MEASURE;                                            // Initialize start screen as measurement screen

    
    /*Initialize Contactor*/
    contactState = {&contactorState, &contactorLocal,                   // Initialize contactor data struct with contactor data
                    &contactorAck, &contactorLED, &hVoltInterlock};                    
    contactorTCB.task = &contactorTask;                                 // Store a pointer to the contactor task update function in the TCB                             
    contactorTCB.taskDataPtr = &contactState;


    /*Initialize Alarm */
    alarmStatus = {&hVoltInterlock, &hvCurrent, &overCurrent, &hvVoltage, &hVoltOutofRange, &acknowledgeFlag, &hVIL};    // Initialize alarm data struct with alarm data
    alarmTCB.task = &alarmTask;                                         // Store a pointer to the alarm task update function in the TCB
    alarmTCB.taskDataPtr = &alarmStatus;

    
    /*Initialize SOC*/
    chargeState = {};                                                   // Initialize state of charge data struct with state of charge boolean
    stateOfChargeTCB.task = &stateOfChargeTask;                         // Store a pointer to the soc task update function in the TCBz
    stateOfChargeTCB.taskDataPtr = &chargeState;

     /*Initialize Remote Terminal Task*/
    terminal = {&minTemp, &maxTemp, &minCurrent, &maxCurrent, &minVolt, &maxVolt, &EEPROMReset};  // Initailize terminal data struct with data
    terminalTCB.task = &terminalTask;                                                             // Store a pointer to the measurementTask update function in the TCB
    terminalTCB.taskDataPtr = &terminal;

     /*Initialize DataLog Task*/
    dataLog = {};  // Initailize terminal data struct with data
    datalogTCB.task = &dataLogTask;                                                             // Store a pointer to the measurementTask update function in the TCB
    datalogTCB.taskDataPtr = &dataLog;

    /*Link each task to the next to make the task queue*/
    measurementTCB.prev = NULL;
    measurementTCB.next = &stateOfChargeTCB;
    
    stateOfChargeTCB.prev = &measurementTCB;
    stateOfChargeTCB.next = &alarmTCB;
    
    alarmTCB.prev = &stateOfChargeTCB;
    alarmTCB.next = &contactorTCB;
    
    contactorTCB.prev = &alarmTCB;
    //contactorTCB.next = &displayTCB;
    contactorTCB.next = &terminalTCB;

    terminalTCB.prev = &contactorTCB;
    terminalTCB.next = &datalogTCB;

    datalogTCB.prev = &terminalTCB;
    datalogTCB.next = &displayTCB;

    displayTCB.prev = &terminalTCB;
    //displayTCB.prev = &contactorTCB;
    displayTCB.next = NULL;




    /*Initialize serial communication*/
    Serial.begin(9600);
    Serial1.begin(9600);
    Serial1.setTimeout(1000);

    /*Initialize the TFT LCD screen and prepare it for display*/
    /*Identifier finder from project 1d, given in class*/
    tft.reset();                                                                                             
    
    uint16_t identifier = tft.readID();
    if(identifier == 0x9325) {
        //Serial.println(F("Found ILI9325 LCD driver"));
    } else if(identifier == 0x9328) {
        //Serial.println(F("Found ILI9328 LCD driver"));
    } else if(identifier == 0x4535) {
        //Serial.println(F("Found LGDP4535 LCD driver"));
    }else if(identifier == 0x7575) {
        //Serial.println(F("Found HX8347G LCD driver"));
    } else if(identifier == 0x9341) {
        //Serial.println(F("Found ILI9341 LCD driver"));
    } else if(identifier == 0x8357) {
        //Serial.println(F("Found HX8357D LCD driver"));
    } else if(identifier==0x0101)
    {     
        identifier=0x9341;
        //Serial.println(F("Found 0x9341 LCD driver"));
    }else {
    //Serial.print(F("Unknown LCD driver chip: "));
    //Serial.println(identifier, HEX);
    //Serial.println(F("If using the Elegoo 2.8\" TFT Arduino shield, the line:"));
    //Serial.println(F("  #define USE_Elegoo_SHIELD_PINOUT"));
    //Serial.println(F("should appear in the library header (Elegoo_TFT.h)."));
    //Serial.println(F("If using the breakout board, it should NOT be #defined!"));
    //Serial.println(F("Also if using the breakout, double-check that all wiring"));
    //Serial.println(F("matches the tutorial."));
    identifier=0x9341; 
    }
    
    tft.begin(identifier);
    tft.setRotation(2); 
    tft.fillScreen(BLACK);         
    
    //unsigned long time_1 = millis();                                                                             

   /*Create scroll buttons for measurement, alarm, and battery screens*/
  for (uint8_t row=0; row<3; row++) {                                                         // Measures Screen Button button coordinates start from the center of the button
      buttons[row].initButton(&tft, BUTTON1_SPACING_X + row*BUTTON2_SPACING_X, BUTTON_Y,
                 BUTTON_W, BUTTON_H, WHITE, buttoncolors[row], BLACK,
                 buttonlabels[row], BUTTON_TEXTSIZE); 
      buttons[row].drawButton();
  }
}
