// MultiStepper.pde
// -*- mode: C++ -*-
// Use MultiStepper class to manage multiple steppers and make them all move to 
// the same position at the same time for linear 2d (or 3d) motion.

/// Defaults to AccelStepper::FULL4WIRE (4) pins.
/// \param[in] pin1 Arduino digital pin number for motor pin 1. Defaults
/// to pin 2. For a AccelStepper::DRIVER (interface==1), 
/// this is the Step input to the driver. Low to high transition means to step)
/// \param[in] pin2 Arduino digital pin number for motor pin 2. Defaults
/// to pin 3. For a AccelStepper::DRIVER (interface==1), 
/// this is the Direction input the driver. High means forward.
/// \param[in] pin3 Arduino digital pin number for motor pin 3. Defaults
/// to pin 4.
/// \param[in] pin4 Arduino digital pin number for motor pin 4. Defaults
/// to pin 5.
/// \param[in] enable If this is true (the default), enableOutputs() will be called to enable
/// the output pins at construction time.
///AccelStepper(uint8_t interface = AccelStepper::FULL4WIRE, uint8_t pin1 = 2, uint8_t pin2 = 3, uint8_t pin3 = 4, uint8_t pin4 = 5, bool enable = true);

#include <AccelStepper.h>
#include <MultiStepper.h>

//  DEFINITIONS 
//  PINS
const int PIN_MOTOR_P_EN    = 2; //Pipette drive
const int PIN_MOTOR_P_STEP  = 3;
const int PIN_MOTOR_P_DIR   = 4;

const int PIN_MOTOR_Z_EN    = 5;
const int PIN_MOTOR_Z_STEP  = 6;
const int PIN_MOTOR_Z_DIR   = 7;

const int PIN_MOTOR_Y_EN    = 8;
const int PIN_MOTOR_Y_STEP  = 9;
const int PIN_MOTOR_Y_DIR   = 10;

const int PIN_MOTOR_X_EN    = 11;
const int PIN_MOTOR_X_STEP  = 12;
const int PIN_MOTOR_X_DIR   = 13;


//Limit Switches - X and Y axes have on both sides
const int PIN_SW_XL          = 52;
const int PIN_SW_XR          = 51;
const int PIN_SW_YF          = 50;
const int PIN_SW_YB          = 49;
const int PIN_SW_ZT          = 48;
const int PIN_SW_PB          = 47;


// EG X-Y position bed driven by 2 steppers
// Alas its not possible to build an array of these with different pins for each :-(
// Up to 10 steppers can be handled as a group by MultiStepper

long positions[4]; // Array of desired stepper positions
  

AccelStepper stepperX(AccelStepper::DRIVER,PIN_MOTOR_X_STEP,PIN_MOTOR_X_DIR); // Defaults to AccelStepper::FULL4WIRE (4 pins) on 2, 3, 4, 5
AccelStepper stepperY(AccelStepper::DRIVER,PIN_MOTOR_Y_STEP,PIN_MOTOR_Y_DIR); // Defaults to AccelStepper::FULL4WIRE (4 pins) on 2, 3, 4, 5
AccelStepper stepperZ(AccelStepper::DRIVER,PIN_MOTOR_Z_STEP,PIN_MOTOR_Z_DIR); // Defaults to AccelStepper::FULL4WIRE (4 pins) on 2, 3, 4, 5
AccelStepper stepperP(AccelStepper::DRIVER,PIN_MOTOR_P_STEP,PIN_MOTOR_P_DIR); // Defaults to AccelStepper::FULL4WIRE (4 pins) on 2, 3, 4, 5

MultiStepper steppers;

///STATE Variables
enum ENUM_LH_STATE {IDLE=1,HOMING=2,HOME=3,TEST_RUN=4,MOVING=5,CMD_PRESSURE=6,HOLD_LARVA=7,LAST_STATE=8};

ENUM_LH_STATE systemState = LAST_STATE;
ENUM_LH_STATE nextState = HOMING;

//Limit Switches Global Variables
int stateSW_XL;
int stateSW_XR;
int stateSW_YF;
int stateSW_YB;
int stateSW_ZT;
int stateSW_PB;

/// Serial COMMAND interface ///
boolean stringComplete             = false;  // whether the string is complete
String inputString                 = "";         // a string to hold incoming data
unsigned long stateTimeOut         = 0; //An Auxiliary var to set when a state expires and system returns to IDLE
unsigned long stateReportInterval  =0; //Used to Time a frequent serial output to host. so port does not freeze


// Function Prototypes
int checkHoming(); //Check homing conditions and set pos to 0 when reached
void handleStartStateEvents(); //Handles actions linked to moving to new state
void handleStopStateEvents(); //Checks conditions for when current state end and transits to next one
int handleSerialCmd();


// Called Once on startup(after reset) for initialization //
void setup() {
  Serial.begin(9600);
  Serial.print("Liquid Handler Controller Initializing\n");
  inputString.reserve(100);
  
  // Configure each stepper
  stepperX.setEnablePin(PIN_MOTOR_X_EN);
  stepperX.setPinsInverted(true,true,true);
  stepperX.setCurrentPosition(0);
  stepperX.setAcceleration(1500); 	
  stepperX.setMaxSpeed(1500);
  //stepper2.setMaxSpeed(100);
  // Then give them to MultiStepper to manage
  steppers.addStepper(stepperX);

  stepperY.setEnablePin(PIN_MOTOR_Y_EN);
  stepperY.setPinsInverted(true,true,true);
  stepperY.setCurrentPosition(0);
  stepperY.setAcceleration(1500); 	
  stepperY.setMaxSpeed(1500);
  //stepper2.setMaxSpeed(100);
  // Then give them to MultiStepper to manage
  steppers.addStepper(stepperY);

  stepperZ.setEnablePin(PIN_MOTOR_Z_EN);
  stepperZ.setPinsInverted(true,true,true);
  stepperZ.setCurrentPosition(0);
  stepperZ.setAcceleration(1500); 	
  stepperZ.setMaxSpeed(1000);
  //stepper2.setMaxSpeed(100);
  // Then give them to MultiStepper to manage
  steppers.addStepper(stepperZ);

  stepperP.setEnablePin(PIN_MOTOR_P_EN);
  stepperP.setPinsInverted(true,true,true);
  stepperP.setCurrentPosition(0);
  stepperP.setAcceleration(1500); 	
  stepperP.setMaxSpeed(1000);
  //stepper2.setMaxSpeed(100);
  // Then give them to MultiStepper to manage
  steppers.addStepper(stepperP);

  positions[0] = -800;
  positions[1] = -800;
  positions[2] = 300;
  positions[3] = -800;

 // steppers.moveTo(positions);
  //steppers.runSpeedToPosition(); // Blocks until all are in position
  //stepperX.moveTo(positions[0]);
  //stepperY.moveTo(positions[1]);
  //stepperZ.moveTo(positions[2]);
  //stepperP.moveTo(positions[3]);

 
 //Setup Buttons
 pinMode(PIN_SW_XL,INPUT_PULLUP);
 pinMode(PIN_SW_XR,INPUT_PULLUP);
 pinMode(PIN_SW_YF,INPUT_PULLUP);
 pinMode(PIN_SW_YB,INPUT_PULLUP);
 pinMode(PIN_SW_ZT,INPUT_PULLUP);
 pinMode(PIN_SW_PB,INPUT_PULLUP);
 

  //steppers.addStepper(stepper2);
}
void loop() {
  
  //delay(3000);
  stateSW_XL = digitalRead(PIN_SW_XL);
  stateSW_XR = digitalRead(PIN_SW_XR);
  stateSW_YF = digitalRead(PIN_SW_YF);
  stateSW_YB = digitalRead(PIN_SW_YB);
  stateSW_ZT = digitalRead(PIN_SW_ZT);
  stateSW_PB = digitalRead(PIN_SW_PB);
  //Serial.println(stateButton,DEC); 
  //delay(20);
  
  //Handle expiration/end of system state
  //State Expired
  if ((stateTimeOut < millis()) && stateTimeOut != 0)
  {
   stateTimeOut = 0;
   nextState = IDLE;
  }

  if (nextState == systemState)
  {
    //Add State Events Events In Following function
    handleStopStateEvents();
  } 
  else   //Handle command to Switch to Next state
  {

    handleStartStateEvents();
    //Report State Change
    Serial.print("New system state set:\t");
    Serial.println(systemState,DEC);
    //Serial.print('\n');
   }

   
// Move to a different coordinate
//  steppers.moveTo(positions);
//  steppers.runSpeedToPosition(); // Blocks until all are in position
//  stepperY.moveTo(positions[1]);

 
///RUN STEP -- Need to Be Able to Move after Homed! So SW should be ignored
  if (stateSW_XL == 1 && stateSW_XR == 1)
  {
    stepperX.run();
  }
  
  if (stateSW_YF == 1 && stateSW_YB == 1)
  {
    stepperY.run();
  }
    
  if (stateSW_ZT == 1)
  {
    stepperZ.run();
  }
  if (stateSW_PB == 1)
  {
    stepperP.run();
  }
////-------------------////

   //Handle Serial Commands
  // print the string when a newline arrives:
  if (stringComplete) {
    
    handleSerialCmd();
     // clear the string:
    inputString = "";
    stringComplete = false;
    }



}

//Return >0 if all positions at home
int checkHoming()
{
  int iret = 0;
    //Home Reached
  if (stateSW_XR == 0)
  {
    stepperX.setCurrentPosition(0);
    stepperX.moveTo(0);
    stepperX.stop();
    iret++;
  }
  if (stateSW_YB == 0)
  {
    stepperY.setCurrentPosition(0);
    stepperY.moveTo(0);
    stepperY.stop();
    iret++;

  }
  if (stateSW_ZT == 0)
  {
    stepperZ.setCurrentPosition(0);
    stepperZ.moveTo(0);
    stepperZ.stop();

    iret++;
  }
  if (stateSW_PB == 0)
  {
    stepperP.setCurrentPosition(0);
    stepperP.moveTo(0);
    iret++;
  }  
  
//    Serial.print("crcHOme:\t");
//    Serial.println(iret,DEC);


  return iret;
}



///////////////
//Run from the main loop on every iteration to check and handle the conditions that may end a state
///////////
void handleStopStateEvents()
{
   //State finished  
  switch (systemState)
    {
      case IDLE: //1
      //ENDS when New Command is issued
        //stepperZ.setMaxSpeed(1);
        //stepperY.setMaxSpeed(1);
        //stepperX.setMaxSpeed(1);
        //stepperP.setMaxSpeed(1);
        stepperX.stop();
        stepperY.stop();
        stepperZ.stop();
        stepperP.stop();
      break;

      case HOMING: //2
        //Check if a PUMP Move has been completed
        if (checkHoming() >= 2) 
        {
          nextState = HOME;
        }else
        {
          nextState = HOMING;
        }
      break;
      

      
      case HOME: //3 //Reached Home - Stay Here

           nextState = IDLE;
      break;      

      
      case TEST_RUN:
          
          
          nextState = MOVING;
      break;

      case MOVING:
         if (stepperX.distanceToGo()==0 && stepperY.distanceToGo()==0 && stepperZ.distanceToGo()==0)
         {
           nextState = TEST_RUN; //Do it again
         }
         
      break;
      
      default: //Uknown option
        nextState = IDLE; //Reset Next State to current
    }
}


//Run When a new state has been set so the switching between states occurs
void handleStartStateEvents()
{  
    switch (nextState)
    {
      case IDLE:
        //tipStopAll();
        //Read Pressure Sensor - C        
        systemState = IDLE;
      break;
      
      case HOMING:
        stepperX.moveTo(-8000); 
        stepperY.moveTo(-8000);
        stepperZ.moveTo(-3800);
        stepperP.moveTo(-100);

        stateTimeOut =  millis()+25000; //With timeout
        systemState = HOMING;
      break;

      case HOME: //nOW sYTEM rEACHED hOME
        
          systemState = HOME;
          stateTimeOut =  0; //No timeout
      break;
      

      case TEST_RUN:
  
          positions[0] = positions[0]*(-1);
          stepperX.moveTo(positions[0]); 
          positions[1] = positions[1]*(-1);
          stepperY.moveTo(positions[1]);
          positions[2] = positions[2]*(-1);
          stepperZ.moveTo(positions[2]);
          positions[3] = positions[3]*(-1);
          stepperP.moveTo(positions[3]);
    
          systemState = MOVING;
      break;
      
      default: //Uknown option
        nextState = systemState; //Reset Next State to current
    }
  
}



/*
  Function handle serials commands - this is called when the string is completed -  
      --serialEvent called between loop calls- once string completed then handleSerialCmd is called 
      that can be used to change the output of an arbitrary port  using the command SOP -
     or using SST you can set the state of the robot. 
*/

int handleSerialCmd()
{

  String strbuff;
  boolean bCommandHandled = false;
  int readPin   = 0;
  int readState = 0;

  //If CMD Starts WITH SOP - Set Output port
    if (inputString.startsWith("SOP"))
    {
      Serial.print("Set Output Port State received\n");
        //Take Port Number -Remove newline character
       strbuff = inputString.substring(3,inputString.length()-2);
//       Serial.print("\n");
//       Serial.print(strbuff);

       readPin = strbuff.toInt();
       strbuff = "";
       strbuff =  inputString.substring(inputString.length()-2,inputString.length()-1);
       readState = strbuff.toInt();
       
       
       // Set the Pin
       pinMode(readPin, OUTPUT);          
       digitalWrite(readPin, readState == 0? LOW:HIGH);

      //Respond with Input Params
       Serial.print("\t");
       Serial.print(readPin,DEC);
       Serial.print("\t");
       Serial.print(readState,DEC);
       
       bCommandHandled= true;
      }
      
      //SET SYSTEM STATE received
    if (inputString.startsWith("SST"))
    {
       strbuff = inputString.substring(inputString.length()-2,inputString.length()-1);
       nextState = (ENUM_LH_STATE)strbuff.toInt();
     
  
       //Check if Value within range
       if (nextState > LAST_STATE || nextState < IDLE)
       {
         nextState = systemState; //No Change
       }
       
       bCommandHandled= true;
     }
  
      if (!bCommandHandled) 
      {
        Serial.print("ERR_CMD\n");
        return -1;
      }

  
  return 0;
}


/*
  SerialEvent occurs whenever a new data comes in the
 hardware serial RX.  This routine is run between each
 time loop() runs, so using delay inside loop can delay
 response.  Multiple bytes of data may be available.
 */
void serialEvent() {


  while (Serial.available()) {
    
     //If In Press Command Mode then Pass Command To SSC Sensor Library directly 
/*     if (systemState == CMD_PRESSURE)
     {
       ssc.commandRequest(Serial);
     }
     else
     {
       */
       //Store In command Buffer which is handled by handleSerialCmd function
      // get the new byte:
      char inChar = (char)Serial.read();
      // add it to the inputString:
      inputString += inChar;
      // if the incoming character is a newline, set a flag
      // so the main loop can do something about it:
      if (inChar == '\n') 
      {
        stringComplete = true;
      }
 //    }
    
    //Make State Machine Here - With Input Codes
  }
 
}


