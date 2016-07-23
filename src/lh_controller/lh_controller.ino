
/*      Pipeting Robot Controller code -running on Arduino Mega 2560  ///////////////
// 
// Author: Kostas Lagogiannis costaslag@gmail.com 2016
// Synopsis:
    This code is to be used to run on the controller of the associated open hardware design pipetting machine.
    It drives the following peripherals:
    *Step. MotorDriver X,Step MotorDriver Y, Step MotorDriver  Z (Ballscrew), Step MotorDriver P (Non Captive ballscrew) -
    these are controller via 3 wire interface wired to the arduino board at pin number defined in the header file.
    We use a 3 wire interface to each MotorDriver (EN, DIR, STEP - HY-DIV268N-5A with active Low setup on pins)
    
    Using the AccelStepper Library V 1.51 that contains the Multistepper class too (not currently used)
    ///AccelStepper(uint8_t interface =AccelStepper::FULL4WIRE, uint8_t pin1 = 2, uint8_t pin2 = 3, uint8_t pin3 = 4, uint8_t pin4 = 5, bool enable = true);
    \param[in] is the Step input to the driver. Low to high transition means to step)
    \param[in] this is the Direction input the driver. High means forward
    \param[in] enable If this is true (the default), enableOutputs() will be called to enable the output pins at construction time.


    Display: 2.7inch OLED 128x64: Using Adafruit GFX and Adafruit1325 Driver 
    Font structures for newer Adafruit_GFX (1.1 and later).
    Example fonts are included in 'Fonts' directory.
    To use a font in your Arduino sketch, #include the corresponding .h
    file and pass address of GFXfont struct to setFont().  Pass NULL to
    revert to 'classic' fixed-space bitmap font.

// Note Any imported library is copied to the local sketchbook subfolder
//// Revision and Notes
// 26/5/16: -Bug : I changed something and the accelleration to Homing Has failed
// 4/6/16 : Changed pin wiring to use Digitalpins header, Moved definitions to dedicated header file  
// 10/6/16 Added Learning pos sequence via joystic - and replay- Note sometimes sequence run stops and needs to issue SST 4 again
// 12/06/12 Added Switch debouncing - Handling of 5 extra controller/joystick buttons for learning pos. 
   Problems To FIX:Jitter/drift on Y axis backwards,Z axis sometimes does not home completely thus zero position misplaced causes crash through tip holder!
  27/06/16 Added Support for SD Card
  03/07/16 There are issues with replay positionioning especially for P, Need to stop it from exceeding upper limit (where there is no switch - limit to steps 3500 / or add physical Sw)
  10/07/16 Made Program Saving/Loading from SD Card, with replay - But problems after 1st replay - loading again fails/readis from random mem
  11/07/16 Fixed bug above, was due to using free instead of delete/new combination for dyn memory alloc/dealloc
  23/07/16 Added Conservative error handling to avoid colission with Limit SW glitch //Was not enough-> Added manual dig pin read of SW in checkHoming to verify what the RDBButton Reads -
            This seems fix the glitch! prob bug in RDBLibrary!
*/

#include <AccelStepper.h>
#include <MultiStepper.h>

//Headers for Screen Adafruit OLED  Monochrome OLEDs based on SSD1306 drivers 
#include <Wire.h>
#include <SPI.h>
#include <SD.h>

#include <Adafruit_GFX.h>
//#include <gfxfont.h>
#include <Adafruit_SSD1325.h>
//#include <Fonts/FreeMono7pt7b.h>
//#include <Fonts/FreeMono8pt7b.h>
#include <Fonts/FreeMono9pt7b.h>

//#include <FreeSerifItalic9pt7b.h>

//For Switch Debouncing
#include <RBD_Timer.h>
#include <RBD_Button.h>

#include "lh_controller.h"

    
#if (SSD1325_LCDHEIGHT != 64)
#error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif

//
File fileroot;


// Called Once on startup(after reset) for initialization //
void setup() {
  Serial.begin(9600);
  Serial.print(F("* Liquid Handler Controller Initializing \n"));
  inputString.reserve(100);
  
  //DISPLAY/
  // by default, we'll generate the high voltage from the 3.3v line internally! (neat!)
  //display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3D (for the 128x64)
  display.begin();
  delay(50);
  
  Serial.print(F("*Set Welcome Screen * \n"));
  dispWelcome();

  display.clearDisplay();
  display.display();
  Serial.print(F("*Done * \n"));

  
  // Configure each stepper
  stepperX.setEnablePin(PIN_MOTOR_X_EN);
  stepperX.setPinsInverted(true,true,true);
  stepperX.setCurrentPosition(0);  
  //stepperX.setMaxSpeed(1500);
  //stepper2.setMaxSpeed(100);
  // Then give them to MultiStepper to manage
  //steppers.addStepper(stepperX);

  stepperY.setEnablePin(PIN_MOTOR_Y_EN);
  stepperY.setPinsInverted(true,true,true);
  stepperY.setCurrentPosition(0);
 	
  //stepperY.setMaxSpeed(1500);
  //stepper2.setMaxSpeed(100);
  // Then give them to MultiStepper to manage
  //steppers.addStepper(stepperY);

  stepperZ.setEnablePin(PIN_MOTOR_Z_EN);
  stepperZ.setPinsInverted(true,true,true);
  stepperZ.setCurrentPosition(0);
  
  //stepperZ.setMaxSpeed(2000);
  //stepper2.setMaxSpeed(100);
  // Then give them to MultiStepper to manage
  //steppers.addStepper(stepperZ);

  stepperP.setEnablePin(PIN_MOTOR_P_EN);
  stepperP.setPinsInverted(true,true,true); //Not Inverting Direction - +ve are towards Home Switches
  stepperP.setCurrentPosition(0);
     
  


  
  
  //stepper2.setMaxSpeed(100);
  // Then give them to MultiStepper to manage
  //steppers.addStepper(stepperP);


//CAn Use Debounce RDB Buttons Here TOo
 pinMode(PIN_SW_BT1,INPUT_PULLUP); //Joystic Sw Setup
 pinMode(PIN_SW_BT2,INPUT_PULLUP); //Joystic Sw Setup
 pinMode(PIN_SW_BT3,INPUT_PULLUP); //Joystic Sw Setup
 pinMode(PIN_SW_BT4,INPUT_PULLUP); //Joystic Sw Setup
 pinMode(PIN_SW_BT5,INPUT_PULLUP); //Joystic Sw Setup
 pinMode(PIN_SW_XL,INPUT_PULLUP);
 pinMode(PIN_SW_YF,INPUT_PULLUP);
 pinMode(PIN_SW_YB,INPUT_PULLUP);
 pinMode(PIN_SW_ZT,INPUT_PULLUP);
 pinMode(PIN_SW_PB,INPUT_PULLUP);
 
 pinMode(PIN_CS_SDCARD,OUTPUT); //Set To it must be left as an output or the SD library won't work.

//Set SW  Debounce To longer - Avoid Noisy Signals.
btn_PB_lim.setDebounceTimeout(BTN_DEBOUNCE_TIMEMS);
btn_ZT_lim.setDebounceTimeout(BTN_DEBOUNCE_TIMEMS);
btn_YB_lim.setDebounceTimeout(BTN_DEBOUNCE_TIMEMS);
btn_YF_lim.setDebounceTimeout(BTN_DEBOUNCE_TIMEMS);
btn_XR_lim.setDebounceTimeout(BTN_DEBOUNCE_TIMEMS);
btn_XL_lim.setDebounceTimeout(BTN_DEBOUNCE_TIMEMS);

  //Load Saved Positions

  //savedPositions[0] = (t_position)800,800,200,100};
  iposSaveIndex = 0; //pos to save next Pos;
  iposCurrentIndex=0;

///SD Card Init /cHIP Select is on 53 (which is also the SPI default)

  Serial.print("Initializing SD card...");

  if (!SD.begin(53)) {
    Serial.println("initialization failed!");
    return;
  }
  Serial.println("initialization done.");

  fileroot = SD.open("/");
  printDirectory(fileroot, 0);
  fileroot.close();

  //Empty Pos Mem Array /Set Motor MaxSpeed v& Accell
  memset(savedPrograms,0,sizeof(t_program*)*MAX_POSITIONS); //Set to 0 For non-inited
  //reset(); //Homing Will Call Reset
  
} //End Of Setup



void loop() {
  
  //Serial.println(stateButton,DEC); 
  //delay(20);
  
  //Handle expiration/end of system state
  //State Expired
  if ((stateTimeOut < millis()) && stateTimeOut != 0)
  {
   stateTimeOut = 0;
   display.println("-TIMEOUT-");
   display.display();
   delay(1000);
   nextState = IDLE;
  }

//  if (nextState != systemState)
     //First Do Start Event Handling, because on these events some motor States may be set

if (nextState != systemState)
{
     handleStartStateEvents();
     dispState();
     display.display();
}
else
    //Add State Events Events In Following function
    handleStopStateEvents();

  int limCnt = 0;
  //Check Limit Switch sensors and Stop Motion If needed Stop
  limCnt = checkHoming();
  //delay(1);
  limCnt += checkHoming();
  limCnt += checkHoming();

  //if (systemState != HOMING) //If LimitOut SW are pressed Speed is set to 0/ So dont do this when homing
  limCnt += checkOutOfRange();

  ////---CONSERVATIVE ERROR HANDLE - Raise error if SW is pressed while robot is moving/replay position
  //If Switched pressed while replay, then Hault Robot- Report Error
  if (limCnt > 3  && (systemState != HOMING && systemState != HOME && systemState != IDLE && systemState != JOYSTICK))
  {
   //nextState = POS_ERROR;
  }
  
  //Read Buttons And Limit Switches
  readJoystick();


///RUN STEP -- Need to Be Able to Move after Homed! So SW should be ignored
    stepperX.run();  
    stepperY.run();
    stepperZ.run();
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


  //update display Check if time to report to host
  if (stateReportInterval < millis())
  {//Report Every sec.
    stateReportInterval = millis()+500;

       
    if (stepperX.distanceToGo()==0 && stepperY.distanceToGo()==0 && stepperZ.distanceToGo()==0 && stepperP.distanceToGo()==0) //Display Updates disrupt the  Motion
    {
      dispState();
      display.display(); //Update display from Buffer
    }
  }


}


void reset()
{
  
  //Initially savedPrograms[0] = 0;
  //memset(savedPrograms,0,sizeof(t_program*)*MAX_POSITIONS); //Set to 0 For non-inited
  
  Serial.println("Prog Pointer:");
  Serial.print((unsigned int)savedPrograms[0]);
  
  prog_init(savedPrograms[0]);
  
  stepperX.setMaxSpeed(1500);
  stepperY.setMaxSpeed(1500);
  stepperZ.setMaxSpeed(2000);
  stepperP.setMaxSpeed(1000);

  stepperX.setAcceleration(1500); 
  stepperY.setAcceleration(1500);   
  stepperZ.setAcceleration(2500);   
  stepperP.setAcceleration(1500);


  //savedPositions[0] = (t_position)800,800,200,100};
  iposSaveIndex = 0; //pos to save next Pos;
  iposCurrentIndex=0;
}


//Initializes a new Program Data structure 
//If pointer not empty then it deletes the existing structure first 
void prog_init(t_program*& prog)
{

  if (prog != 0)
  {
    prog_clearPoslist(prog);
    delete(prog);
  }

  
  
  //Init Prog / Create 1st defaultPosition where all programs start from
  prog_position* newpos = new prog_position;
  newpos->Xpos  = 0;
  newpos->Ypos  = 0;
  newpos->Zpos  = 0;
  newpos->Ppos  = -2500;
  newpos->seqID = 0;

  prog = new  t_program;
  prog->posCount = 1; //First Default Position Saved
  prog->protoPos = newpos; //First Position
  prog->epiPos   = newpos; //Current Position
  prog->telosPos = newpos; //Last Position
  strcpy(prog->progname,"EOS.PRG"); //Name of 1st Program

}

///Delete Clear memory of structs in program list of positions
void prog_clearPoslist(t_program* prog)
{
      if (prog == 0)
        return;
      if (prog->protoPos == 0 || prog->posCount == 0 )
        return;
        
      ///Need to clear LIst of positions too
      prog_position* cpos    = prog->protoPos;
      prog_position* nxtpos = 0;
      
      while (cpos)
      {
        nxtpos = cpos->epomPos;
        delete(cpos);        
        cpos = nxtpos;
      }

    prog->posCount = 0;
      
}

