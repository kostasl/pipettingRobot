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
///AccelStepper(uint8_t interface =AccelStepper::FULL4WIRE, uint8_t pin1 = 2, uint8_t pin2 = 3, uint8_t pin3 = 4, uint8_t pin4 = 5, bool enable = true);


// Font structures for newer Adafruit_GFX (1.1 and later).
// Example fonts are included in 'Fonts' directory.
// To use a font in your Arduino sketch, #include the corresponding .h
// file and pass address of GFXfont struct to setFont().  Pass NULL to
// revert to 'classic' fixed-space bitmap font.

//Note Any imported library is copied to the local sketchbook subfolder

#include <AccelStepper.h>
#include <MultiStepper.h>

//Headers for Screen Adafruit OLED  Monochrome OLEDs based on SSD1306 drivers 
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
//#include <gfxfont.h>
#include <Adafruit_SSD1306.h>
#include <FreeMono9pt7b.h>
#include <FreeSerifItalic9pt7b.h>
    
#if (SSD1306_LCDHEIGHT != 64)
#error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif




//  DEFINITIONS 
//  PINS
//
#define TXT_TITLE          "-AutoPip-"
#define OLED_RESET           31 //NOTE The connected OLED Does not Have A RST PIN

#define PIN_MOTOR_P_EN    2 //Pipette drive
#define PIN_MOTOR_P_STEP  3
#define PIN_MOTOR_P_DIR   4

#define PIN_MOTOR_Z_EN    5
#define PIN_MOTOR_Z_STEP  6
#define PIN_MOTOR_Z_DIR   7

#define PIN_MOTOR_Y_EN    8
#define PIN_MOTOR_Y_STEP  9
#define PIN_MOTOR_Y_DIR   10

#define PIN_MOTOR_X_EN    11
#define PIN_MOTOR_X_STEP  12
#define PIN_MOTOR_X_DIR   13


//Limit Switches - X and Y axes have on both sides
#define PIN_SW_XL           52
#define PIN_SW_XR           51
#define PIN_SW_YF           50
#define PIN_SW_YB           49
#define PIN_SW_ZT           48
#define PIN_SW_PB           47


// EG X-Y position bed driven by 2 steppers
// Alas its not possible to build an array of these with different pins for each :-(
// Up to 10 steppers can be handled as a group by MultiStepper

long positions[4]; // Array of desired stepper positions
  

AccelStepper stepperX(AccelStepper::DRIVER,PIN_MOTOR_X_STEP,PIN_MOTOR_X_DIR); // Defaults to AccelStepper::FULL4WIRE (4 pins) on 2, 3, 4, 5
AccelStepper stepperY(AccelStepper::DRIVER,PIN_MOTOR_Y_STEP,PIN_MOTOR_Y_DIR); // Defaults to AccelStepper::FULL4WIRE (4 pins) on 2, 3, 4, 5
AccelStepper stepperZ(AccelStepper::DRIVER,PIN_MOTOR_Z_STEP,PIN_MOTOR_Z_DIR); // Defaults to AccelStepper::FULL4WIRE (4 pins) on 2, 3, 4, 5
AccelStepper stepperP(AccelStepper::DRIVER,PIN_MOTOR_P_STEP,PIN_MOTOR_P_DIR); // Defaults to AccelStepper::FULL4WIRE (4 pins) on 2, 3, 4, 5

MultiStepper steppers;

///Screen Vars 
Adafruit_SSD1306 display(OLED_RESET);


///STATE Variables
enum ENUM_LH_STATE {IDLE=1,HOMING=2,HOME=3,TEST_RUN=4,MOVING=5,CMD_PRESSURE=6,HOLD_LARVA=7,LAST_STATE=8};

ENUM_LH_STATE systemState = LAST_STATE;
ENUM_LH_STATE nextState = HOMING;

//Limit Switches Global Variables
int stateSW_XL = 1;
int stateSW_XR = 1;
int stateSW_YF = 1;
int stateSW_YB = 1;
int stateSW_ZT = 1;
int stateSW_PB = 1;

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

void dispWelcome();

// Called Once on startup(after reset) for initialization //
void setup() {
  Serial.begin(9600);
  Serial.print("Liquid Handler Controller Initializing\n");
  inputString.reserve(100);
  
  //DISPLAY/
  // by default, we'll generate the high voltage from the 3.3v line internally! (neat!)
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3D (for the 128x64)
  delay(50);
  display.setFont(&FreeSerifItalic9pt7b);
  dispWelcome();
  delay(1500);
  display.setFont(&FreeMono9pt7b);
  
  // Configure each stepper
  stepperX.setEnablePin(PIN_MOTOR_X_EN);
  stepperX.setPinsInverted(true,true,true);
  stepperX.setCurrentPosition(0);  
  stepperX.setMaxSpeed(1500);
  //stepper2.setMaxSpeed(100);
  // Then give them to MultiStepper to manage
  steppers.addStepper(stepperX);

  stepperY.setEnablePin(PIN_MOTOR_Y_EN);
  stepperY.setPinsInverted(true,true,true);
  stepperY.setCurrentPosition(0);
 	
  stepperY.setMaxSpeed(1500);
  //stepper2.setMaxSpeed(100);
  // Then give them to MultiStepper to manage
  steppers.addStepper(stepperY);

  stepperZ.setEnablePin(PIN_MOTOR_Z_EN);
  stepperZ.setPinsInverted(true,true,true);
  stepperZ.setCurrentPosition(0);
  
  stepperZ.setMaxSpeed(2000);
  //stepper2.setMaxSpeed(100);
  // Then give them to MultiStepper to manage
  steppers.addStepper(stepperZ);

  stepperP.setEnablePin(PIN_MOTOR_P_EN);
  stepperP.setPinsInverted(false,true,true); //Not Inverting Direction - +ve are towards Home Switches
  stepperP.setCurrentPosition(0);
   	
  stepperP.setMaxSpeed(1000);
  //stepper2.setMaxSpeed(100);
  // Then give them to MultiStepper to manage
  steppers.addStepper(stepperP);

  positions[0] = 800; //X
  positions[1] = 800;//Y
  positions[2] = 300; //Z Axis
  positions[3] = -300; //Pipete

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
  

  //Serial.println(stateButton,DEC); 
  //delay(20);
  
  //Handle expiration/end of system state
  //State Expired
  if ((stateTimeOut < millis()) && stateTimeOut != 0)
  {
   stateTimeOut = 0;
   nextState = IDLE;
  }

  //Check if time to report to host
  if (stateReportInterval < millis())
  {//Report Every sec.
    stateReportInterval = millis()+700;
//    printPressureTemp();
    display.display(); //Update display from Buffer
    //Serial.println("PP");
  }

   
// Move to a different coordinate
//  steppers.moveTo(positions);
//  steppers.runSpeedToPosition(); // Blocks until all are in position
//  stepperY.moveTo(positions[1]);

  checkHoming();
  checkOutOfRange();



  if (nextState == systemState)
  {
    //Add State Events Events In Following function
    handleStopStateEvents();
  } 
  else   //Handle command to Switch to Next state
  {
    
    //Report State Change
    Serial.print("New system state set:\t");
    Serial.println(systemState,DEC);
    //Serial.print('\n');
    //Report INfo State on Display  
     dispState();
   }


//First Do Start Event Handling, because on these events some motor States may be set
 handleStartStateEvents();


///RUN STEP -- Need to Be Able to Move after Homed! So SW should be ignored
//  if (stateSW_XL == 1 && stateSW_XR == 1)
    stepperX.run();
  
//  if (stateSW_YF == 1 && stateSW_YB == 1)
    stepperY.run();
    
//  if (stateSW_ZT == 1)
    stepperZ.run();
  
//  if (stateSW_PB == 1)
    stepperP.run();

   
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
  //Save prior Read State
  bool bXFlagged = !(stateSW_XR == 1);
  bool bYFlagged = !(stateSW_YB == 1);
  bool bZFlagged = !(stateSW_ZT == 1);
  bool bPBlagged = !(stateSW_PB == 1);
  
  //Read Switches Home Reached
  
  stateSW_XR = digitalRead(PIN_SW_XR);
  stateSW_YB = digitalRead(PIN_SW_YB);
  stateSW_ZT = digitalRead(PIN_SW_ZT);
  stateSW_PB = digitalRead(PIN_SW_PB);
    
   if (stateSW_XR == 0){
      stepperX.setCurrentPosition(0);
      //stepperX.moveTo(0);
      
      iret++;
      
      if (!bXFlagged){ //If switch just changed then Update Screen - Note Screen has a buffer
        stepperX.stop();
        display.setCursor(0,45);
        display.println("X ON");
        display.display();
      }
    }else
    {
      //Clear Screen On Swithc
        display.setCursor(0,45);
        display.setTextColor (BLACK,WHITE); // 'inverted' text
        display.print("X ON");
        display.setTextColor (WHITE,BLACK); // 'inverted' text
        //display.fillRectangle()
    }
    
    
    if (stateSW_YB == 0)
    {
      stepperY.setCurrentPosition(0); //Set As Ref Point
      //stepperY.moveTo(0);
      //
      iret++;  
      
      if (!bYFlagged){ //If switch just changed then Update Screen
        stepperY.stop();
        display.setCursor(0,57);
        display.println("Y ON");
        display.display();
      } 
    }else
    {
      //Clear Screen On Swithc
        display.setCursor(0,57);
        display.setTextColor (BLACK,WHITE); // 'inverted' text
        display.print("Y ON");
        display.setTextColor (WHITE,BLACK); // 'inverted' text
    }
   

    if (stateSW_ZT == 0)
    {
      stepperZ.setCurrentPosition(0);
      //stepperZ.moveTo(0);
      //
      iret++;
      
      if (!bZFlagged){ //If switch just changed then Update Screen
        stepperZ.stop();
        display.setCursor(50,45);
        display.println("Z ON");
        display.display();
      }
    }else
    {
      //Clear Screen On Swithc
        display.setCursor(50,45);
        display.setTextColor (BLACK,WHITE); // 'inverted' text
        display.print("Z ON");
        display.setTextColor (WHITE,BLACK); // 'inverted' text
    }
  
    if (stateSW_PB == 0)
    {
      stepperP.setCurrentPosition(0);
      //stepperP.moveTo(0);
      //stepperP.stop();
      iret++;

      if (!bPBlagged){ //If switch just changed then Update Screen
        stepperP.stop();
        display.setCursor(50,57);
        display.println("P ON");
        display.display();
      }
    }else
    {
      //Clear Screen On Swithc
        display.setCursor(50,57);
        display.setTextColor (BLACK,WHITE); // 'inverted' text
        display.print("P ON");
        display.setTextColor (WHITE,BLACK); // 'inverted' text
    }
    
//    Serial.print("crcHOme:\t");
//    Serial.println(iret,DEC);

  return iret;
}


int checkOutOfRange()
{
  int iret = 0;
    //EXtreme Switches
  stateSW_XL = digitalRead(PIN_SW_XL);
  stateSW_YF = digitalRead(PIN_SW_YF);

  if (stateSW_XL == 0)
  {
    stepperX.stop();
    iret++;
  } 
  if (stateSW_YB == 0) {
    stepperY.stop();
    iret++;
  }

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
      break;

      case HOMING: //2
        //Check if a PUMP Move has been completed
        if (checkHoming() > 3) 
        {
          nextState = HOME;
        }else
        {
          nextState = HOMING;
        }
      break;
      

      
      case HOME: //3 //Reached Home - Stay Here Unclick Switches

           nextState = HOME;
      break;      

      
      case TEST_RUN:
 
          
          nextState = MOVING;
      break;

      case MOVING:
         if (stepperX.distanceToGo()==0 && stepperY.distanceToGo()==0 && stepperZ.distanceToGo()==0 && stepperP.distanceToGo()==0)
         {
           //Invert Positions For next Run
          positions[0] = positions[0]*(-1);
          positions[1] = positions[1]*(-1);
          positions[2] = positions[2]*(-1);
          positions[3] = positions[3]*(-1);
          
         
           nextState = TEST_RUN; //Do it again
         }
         else
         {
           nextState = MOVING;
         }
      break;
      
      default: //Uknown option
        nextState = IDLE; //Reset Next State to current
    }
}


//Run When a new state has been set so the switching between states occurs
//For State To Switch the Handling needs to Save nextState INto Current state
void handleStartStateEvents()
{  
    switch (nextState)
    {
      case IDLE:

        stepperX.stop();
        stepperY.stop();
        stepperZ.stop();
        stepperP.stop();

        systemState = IDLE;
      break;
      
      case HOMING:
        stepperX.setAcceleration(1500); 
        stepperY.setAcceleration(1500); 	
        stepperZ.setAcceleration(1500); 	
        stepperP.setAcceleration(1500);

        stepperX.moveTo(-5000); 
        stepperY.moveTo(-5000);
        stepperZ.moveTo(-12000);
        stepperP.moveTo(-8000);
        

        stateTimeOut =  millis()+25000; //With timeout
        systemState = HOMING;
      break;

      case HOME: //nOW sYTEM rEACHED hOME / Release SWitches    

//        stepperX.runToNewPosition(50);
//        stepperY.runToNewPosition(50);
//        stepperZ.runToNewPosition(80);
//        stepperP.runToNewPosition(80);
//
//        stepperX.stop();
//        stepperY.stop();
//        stepperZ.stop();
//        stepperP.stop();
//        stepperX.setAcceleration(10);
//        stepperY.setAcceleration(10);
//
        stepperX.moveTo(50); 
        stepperY.moveTo(50);
        stepperZ.moveTo(100);
        stepperP.moveTo(100);

        systemState = HOME;
        
        stateTimeOut =  millis() + 7000; //No timeout
      break;
      

      case TEST_RUN:
        stepperX.setAcceleration(1500); 
        stepperY.setAcceleration(1500); 	
        stepperZ.setAcceleration(1500); 	
        stepperP.setAcceleration(1500);

        stepperX.moveTo(positions[0]); 
        stepperY.moveTo(positions[1]);
        stepperZ.moveTo(positions[2]);
        stepperP.moveTo(positions[3]);

    
          systemState = TEST_RUN;
      break;
      
      case MOVING:
      //Nothing Here
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
//////////////  END SERIAL CODE //////////////////////

////////////////////// DISPLAY CODE ////////////////

void testscrolltext(void) {
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(10,0);
  display.clearDisplay();
  display.println("scroll");
  display.display();

  display.startscrollright(0x00, 0x0F);
  delay(2000);
  display.stopscroll();
  delay(1000);
  display.startscrollleft(0x00, 0x0F);
  delay(2000);
  display.stopscroll();
  delay(1000);    
  display.startscrolldiagright(0x00, 0x07);
  delay(2000);
  display.startscrolldiagleft(0x00, 0x07);
  delay(2000);
  display.stopscroll();
}



void dispWelcome()
{
    // Clear the buffer.
  // miniature bitmap display
  display.clearDisplay();
  display.stopscroll();
  display.setTextColor (WHITE,BLACK); // 'inverted' text
  display.setTextSize(1);
  display.setCursor(0,11);
  display.println(TXT_TITLE);
  display.setTextSize(1);
  display.setCursor(0,28);

  display.print(" Initializing...");
  display.display();

}

//Show Text Relating to the Current System System STATE
void dispState()
{
    // Clear the buffer.
  // miniature bitmap display
  display.clearDisplay();
  display.stopscroll();
  //display.setTextColor(BLACK, WHITE); // 'inverted' text
  display.setTextSize(1);
  

    //State finished  
  switch (systemState)
    {
      case IDLE: //1
        display.setCursor(0,11);
        display.println(TXT_TITLE); 
        display.setTextColor( WHITE,BLACK); // 'inverted' text     
        //display.setCursor(0,20);
        display.setCursor(0,28);

        display.println(" -Ready-");
       
       break;
       
      case HOME: 
        display.setCursor(0,11);
        display.println(TXT_TITLE);
        display.setTextColor( WHITE,BLACK); // 'inverted' text     
        //display.setCursor(0,20);
        display.setCursor(0,28);

        display.println(" -Home-");
       
      break;

      case HOMING:
        display.setTextColor( WHITE,BLACK); // 'inverted' text     
        //display.setCursor(0,20);
        display.setCursor(0,11);
        display.println("-Homing->");
        display.startscrollright(0x00, 0x0F);
      break;
      
      case MOVING:
        display.setCursor(0,11);
        display.println("Keep clear");
        display.setTextColor( WHITE,BLACK); // 'inverted' text
        display.setCursor(0,28);
     
        display.println("-Active-");

        display.startscrollright(0x00, 0x0F);
      break;
      
      
      default:
      break;
    }
  
  display.display();

}

/////////////////END DISPLAY CODE ////////////////

