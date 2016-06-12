
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
// 16/6/16 Added Learning pos sequence via joystic - and replay- Note sometimes sequence run stops and needs to issue SST 4 again
*/
#include <AccelStepper.h>
#include <MultiStepper.h>

//Headers for Screen Adafruit OLED  Monochrome OLEDs based on SSD1306 drivers 
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
//#include <gfxfont.h>
#include <Adafruit_SSD1325.h>
#include <Fonts/FreeMono9pt7b.h>
//#include <FreeSerifItalic9pt7b.h>

#include "lh_controller.h"

    
#if (SSD1325_LCDHEIGHT != 64)
#error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif


// Called Once on startup(after reset) for initialization //
void setup() {
  Serial.begin(9600);
  Serial.print("* Liquid Handler Controller Initializing \n");
  inputString.reserve(100);
  
  //DISPLAY/
  // by default, we'll generate the high voltage from the 3.3v line internally! (neat!)
  //display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3D (for the 128x64)
  display.begin();
  delay(50);
  //display.setFont(&FreeSerifItalic9pt7b);
  display.setFont(&FreeMono9pt7b);

  Serial.print("*Set Welcome Screen * \n");
  dispWelcome();

  delay(1500);
  display.clearDisplay();
  display.display();
  Serial.print("*Done * \n");

  
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
  
  
  stepperX.setAcceleration(1500); 
  stepperY.setAcceleration(1500); 	
  stepperZ.setAcceleration(1500); 	
  stepperP.setAcceleration(1500);

  
  
  //stepper2.setMaxSpeed(100);
  // Then give them to MultiStepper to manage
  steppers.addStepper(stepperP);

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
 pinMode(PIN_SW_JR,INPUT_PULLUP); //Joystic Sw Setup
 pinMode(PIN_SW_BT1,INPUT_PULLUP); //Joystic Sw Setup
 pinMode(PIN_SW_BT2,INPUT_PULLUP); //Joystic Sw Setup
 pinMode(PIN_SW_BT3,INPUT_PULLUP); //Joystic Sw Setup
 pinMode(PIN_SW_BT4,INPUT_PULLUP); //Joystic Sw Setup
 pinMode(PIN_SW_BT5,INPUT_PULLUP); //Joystic Sw Setup

 //digitalWrite(stateSW_JR,HIGH);

  //steppers.addStepper(stepper2);

  //Load Saved Positions
  //Empty Target Array
  memset(savedPositions,0,sizeof(savedPositions));

  //savedPositions[0] = (t_position)800,800,200,100};
  iposSaveIndex = 0; //pos to save next Pos;
  iposCurrentIndex=0;
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
 
   
  //Check Limit Switch sensors and Stop Motion If needed Stop
  checkHoming();
  checkOutOfRange();
  readJoystick();




///RUN STEP -- Need to Be Able to Move after Homed! So SW should be ignored
//if (stateSW_XL == 1)
    stepperX.run();  
// if (stateSW_YF == 1)
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

   display.setTextColor (WHITE,BLACK); // 'inverted' text
    
   if (stateSW_XR == 0){
      //stepperX.moveTo(0);
      
      iret++;
      
      if (!bXFlagged){ //If switch just changed then Update Screen - Note Screen has a buffer
        stepperX.setCurrentPosition(0); //Set ref Point And Target to 0 Has the side effect of setting the current motor speed to 0. 
        stepperX.stop();
        display.setCursor(0,45);
        display.println("X ON");
        display.display();
      }else
      {
       if (stepperX.targetPosition() < 1){ //Only stopMotor if next pos is pressing against switch
           stepperX.moveTo(1);
           stepperX.stop();
         }
      }
    }
    
    
    if (stateSW_YB == 0)
    {
      //stepperY.moveTo(0);
      //
      iret++;  
      
      if (!bYFlagged){ //If switch just changed then Update Screen
        stepperY.setCurrentPosition(0); //Set As Ref Point Has the side effect of setting the current motor speed to 0. 
        stepperY.stop(); //Just in case future vers change the above
        display.setCursor(0,57);
        display.println("Y ON");
        display.display();
      }else
      {

        if (stepperY.targetPosition() < 1)
        {
         stepperY.moveTo(1);
         stepperY.stop();
        }
      }
    }
   

    if (stateSW_ZT == 0)
    {
      //
      iret++;
      
      if (!bZFlagged){ //If switch just changed then Update Screen
        stepperZ.setCurrentPosition(0);
        stepperZ.stop();
        stepperZ.setSpeed(0);
        display.setCursor(50,45);
        display.println("Z ON");
        display.display();
      }
    }else
    {
    }
  
    if (stateSW_PB == 0)
    {
      //stepperP.moveTo(0);
      //stepperP.stop();
      iret++;

      if (!bPBlagged){ //If switch just changed then Update Screen
        stepperP.setCurrentPosition(0);
        stepperP.setSpeed(0);
        stepperP.stop();
        display.setCursor(50,57);
        display.println("P ON");
        display.display();
      }
    }else
    {
    }
    
//    Serial.print("crcHOme:\t");
//    Serial.println(iret,DEC);

  return iret;
}


int checkOutOfRange()
{
  int iret = 0;
  bool bXFlagged = !(stateSW_XL == 1);
  bool bYFlagged = !(stateSW_YF == 1);

    //EXtreme Switches
  stateSW_XL = digitalRead(PIN_SW_XL);
  stateSW_YF = digitalRead(PIN_SW_YF);

  if (stateSW_XL == 0)
  {
      if (!bXFlagged){ //If switch just changed then Update Screen - Note Screen has a buffer
        stepperX.setSpeed(0);
        display.setCursor(0,45);
        display.println("X ON");
        display.display();
      }else
      {
      //Do not allow to push against the switch
        if (stepperX.targetPosition() - stepperX.currentPosition() > 0)
        {
         stepperX.setSpeed(0);
         stepperX.moveTo(stepperX.currentPosition());
         stepperX.stop(); //Just in case future vers change the above
        }
      }

    iret++;
  } 
  if (stateSW_YF == 0) {

      if (!bYFlagged){ //If switch just changed then Update Screen
        stepperY.setSpeed(0);//Just in case future vers change the above
        display.setCursor(0,57);
        display.println("Y ON");
        display.display();
      }else
      {

        //Do not allow to push against the switch
        if (stepperY.targetPosition() - stepperY.currentPosition() > 0)
        {
           stepperY.moveTo(stepperY.currentPosition());
           stepperY.stop(); //Just in case future vers change the above
        }
      }

    

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
        
         if (stateSW_JR == 1){
           nextState = JOYSTICK;
           display.println("BUTTON");
           display.display();
         }else
           nextState = IDLE;
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
            nextState = MOVING;

         if (stepperX.distanceToGo()==0 && stepperY.distanceToGo()==0 && stepperZ.distanceToGo()==0 && stepperP.distanceToGo()==0)
         {
            //displState();
          //Do not Exceed Last saved Position
           if (iposCurrentIndex < iposSaveIndex)
           { 

              
              iposCurrentIndex++;
              nextState = TEST_RUN; //Do it again
          }else
          { //DOnethe sequence - Go Back to Idle
            nextState = IDLE;
            iposCurrentIndex = 0;
          }
         }

     
      break;
      
      case JOYSTICK:
 
        stepperY.setMaxSpeed(2*abs(posJRy));
        stepperX.setMaxSpeed(2*abs(posJRx));
        stepperZ.setMaxSpeed(1000);
        stepperP.setMaxSpeed(1000);

        
        if (abs(posJRy) > 10)
        {
         stepperY.move(posJRy);
        }
        else
          stepperY.move(0);
          
        if (abs(posJRx) > 10)
           stepperX.move(posJRx);
         else
           stepperX.move(0);

        if (stateSW_BT1 == 1)
           stepperZ.move(-400);
         //else
           
        if (stateSW_BT4 == 1)
           stepperZ.move(400);
         //else
           //stepperZ.move(0);

        if(stateSW_BT1==0 && stateSW_BT4 == 0)
           stepperZ.move(0);

        if (stateSW_BT2 == 1)
           stepperP.move(-300);

        if (stateSW_BT5 == 1)
           stepperP.move(300);

         if(stateSW_BT2==0 && stateSW_BT5 == 0)
           stepperP.move(0);

        if (stateSW_JR == 1){ //Click So Save New Position
            nextState = SAVE_POSITION;
         }

        if (stateSW_BT3 == 1){ //Click So as to Replay saved POsitions
            nextState = TEST_RUN;
         }

           
      break;

      case SAVE_POSITION:

        //Only Move Back to Joystick Once Button Has been released          
        if (stateSW_JR == 1){ //Click So Save New Position
            nextState = SAVE_POSITION;
         }else
          nextState = JOYSTICK;
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
      char buff[130];

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

        stepperX.moveTo(-8000); 
        stepperY.moveTo(-8000);
        stepperZ.moveTo(-12000);
        stepperP.moveTo(-8000);
        

        stateTimeOut =  millis()+25000; //With timeout
        systemState = HOMING;
      break;

      case HOME: //nOW sYTEM rEACHED hOME / Release SWitches    
      //For Some Reason Using the Main Loop With Move To Does not Work / Need todo a blocking call here
       stepperX.runToNewPosition(50);
       stepperY.runToNewPosition(50);
       stepperZ.runToNewPosition(100);
       stepperP.runToNewPosition(4500);
//
//        stepperX.stop();
//        stepperY.stop();
//        stepperZ.stop();
//        stepperP.stop();
//
//        stepperX.moveTo(50); 
//        stepperY.moveTo(50);
//        stepperZ.moveTo(100);
//        stepperP.moveTo(100);


        
        systemState = HOME;
        
        stateTimeOut =  millis() + 3000; //No timeout
      break;
      

      case TEST_RUN:
        stepperX.setAcceleration(1500); 
        stepperY.setAcceleration(1500); 	
        stepperZ.setAcceleration(1500); 	
        stepperP.setAcceleration(1500);

        stepperX.setMaxSpeed(2000);
        stepperY.setMaxSpeed(2000);
        stepperZ.setMaxSpeed(2000);
        stepperP.setMaxSpeed(2000);

        
        stepperX.moveTo(savedPositions[iposCurrentIndex].Xpos); 
        stepperY.moveTo(savedPositions[iposCurrentIndex].Ypos);
        stepperZ.moveTo(savedPositions[iposCurrentIndex].Zpos);
        stepperP.moveTo(savedPositions[iposCurrentIndex].Ppos);


        sprintf(buff,"Run to Pos i:%d, X:%ld, Y:%ld,Z:%ld,P:%ld ",iposCurrentIndex, savedPositions[iposCurrentIndex].Xpos,savedPositions[iposCurrentIndex].Ypos,savedPositions[iposCurrentIndex].Zpos,savedPositions[iposCurrentIndex].Ppos);
        Serial.println(buff);
        
        systemState = TEST_RUN;
      break;
      
      case MOVING:
      //Nothing Here
          systemState = MOVING;
      break;

      case JOYSTICK:
          systemState = JOYSTICK;
          
          stateTimeOut =  millis()+65000; //With timeout
      
      break;

      case SAVE_POSITION: //Add Current Position TO list Of Saved positions
            //stepperX.stop();
            //stepperY.stop();   
            //stepperZ.stop();
            //stepperP.stop();
               
            savedPositions[iposSaveIndex].Xpos = stepperX.currentPosition();
            savedPositions[iposSaveIndex].Ypos = stepperY.currentPosition();
            savedPositions[iposSaveIndex].Zpos = stepperZ.currentPosition();
            savedPositions[iposSaveIndex].Ppos = stepperP.currentPosition();
          
            //char buff[130];
            sprintf(buff,"Saved Pos i: %d X:%ld Y:%ld,Z:%ld,P:%ld ", iposSaveIndex,savedPositions[iposSaveIndex].Xpos, savedPositions[iposSaveIndex].Ypos, savedPositions[iposSaveIndex].Zpos,savedPositions[iposSaveIndex].Ppos );
            Serial.println(buff);

            iposSaveIndex++;
            
            dispState();
            display.display();

            
            systemState = SAVE_POSITION;
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



void dispWelcome()
{
    // Clear the buffer.
  // miniature bitmap display
  display.clearDisplay();
  //display.stopscroll();
  display.setTextColor (WHITE,BLACK); // 'inverted' text
  display.setTextSize(1);
  display.setCursor(0,11);
  display.println(TXT_TITLE);
  display.setTextSize(1);
  display.setCursor(0,28);

  display.print(".Breathing.");
  display.display();

}

//Show Text Relating to the Current System System STATE
void dispState()
{
    // Clear the buffer.
  // miniature bitmap display
  display.clearDisplay();
  //display.stopscroll();
  display.setTextColor( WHITE,BLACK); // 'inverted' text
  display.setTextSize(1);

    //State finished  
  switch (systemState)
    {
      char buff[130];  

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
        //display.startscrollright(0x00, 0x0F);
      break;
      
      case MOVING:
        display.setCursor(0,11);
        display.println("Keep clear");
        display.setTextColor( WHITE,BLACK); // 'inverted' text
        display.setCursor(0,28);
     
        display.println("-Moving-");

        //display.startscrollright(0x00, 0x0F);
      break;

      case JOYSTICK:
        sprintf(buff,"%d - %d",posJRx,posJRy);
        display.setCursor(0,11);
        display.println("Joystick");
        display.setTextColor( WHITE,BLACK); // 'inverted' text
        display.setCursor(5,30);
        display.print(buff);
        //display.display();

  
      break;

      case SAVE_POSITION:
        display.setCursor(0,11);
        display.print(iposSaveIndex);
        display.println(" POSITION \n \t SAVED");
        
      break;
      
      default:
      break;
    }
  
       //Clear Screen On Swithc
        display.setCursor(0,45);
        display.setTextColor (BLACK,WHITE); // 'inverted' text
        display.print("X ON");
        display.setTextColor (WHITE,BLACK); // 'inverted' text
 
      //Clear Screen On Swithc
        display.setCursor(0,57);
        display.setTextColor (BLACK,WHITE); // 'inverted' text
        display.print("Y ON");
        display.setTextColor (WHITE,BLACK); // 'inverted' text
 
      //Clear Screen On Swithc
        display.setCursor(50,45);
        display.setTextColor (BLACK,WHITE); // 'inverted' text
        display.print("Z ON");
        display.setTextColor (WHITE,BLACK); // 'inverted' text
 
      //Clear Screen On Swithc
        display.setCursor(50,57);
        display.setTextColor (BLACK,WHITE); // 'inverted' text
        display.print("P ON");
        display.setTextColor (WHITE,BLACK); // 'inverted' text
 
  
  //display.display();

}

/////////////////END DISPLAY CODE ////////////////


///READ JOY Stick //

void readJoystick()
{
  posJRx       = analogRead(PIN_AJR_X)-531; //Substrract 0 pos, 
  posJRy       = analogRead(PIN_AJR_Y)-531;
  stateSW_JR    = 1-(int)digitalRead(PIN_SW_JR);  //Invert So it Behaves like the limit SW
  stateSW_BT1    = 1-(int)digitalRead(PIN_SW_BT1);  //Invert So it Behaves like the limit SW
  stateSW_BT2    = 1-(int)digitalRead(PIN_SW_BT2);  //Invert So it Behaves like the limit SW
  stateSW_BT3    = 1-(int)digitalRead(PIN_SW_BT3);  //Invert So it Behaves like the limit SW
  stateSW_BT4    = 1-(int)digitalRead(PIN_SW_BT4);  //Invert So it Behaves like the limit SW
  stateSW_BT5    = 1-(int)digitalRead(PIN_SW_BT5);  //Invert So it Behaves like the limit SW

}

