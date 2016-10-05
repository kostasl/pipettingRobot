/*
  Function handle input from Joystick or serials commands - this is called when the string is completed -  
      --serialEvent called between loop calls- once string completed then handleSerialCmd is called 
      that can be used to change the output of an arbitrary port  using the command SOP -
     or using SST you can set the state of the robot. 
*/




///READ JOYStick //

void readJoystick()
{
  posJRx       = analogRead(PIN_AJR_X)-531; //Substrract 0 pos, 
  posJRy       = analogRead(PIN_AJR_Y)-531;
 
  //stateSW_JR    = 1-(int)digitalRead(PIN_SW_JR);  //Invert So it Behaves like the limit SW
  stateSW_BT1    = 1-(int)digitalRead(PIN_SW_BT1);  //Invert So it Behaves like the limit SW
  stateSW_BT2    = 1-(int)digitalRead(PIN_SW_BT2);  //Invert So it Behaves like the limit SW
  stateSW_BT3    = 1-(int)digitalRead(PIN_SW_BT3);  //Invert So it Behaves like the limit SW
  stateSW_BT4    = 1-(int)digitalRead(PIN_SW_BT4);  //Invert So it Behaves like the limit SW
  stateSW_BT5    = 1-(int)digitalRead(PIN_SW_BT5);  //Invert So it Behaves like the limit SW

  //LIMIT SW - Update States
  stateSW_XL    = 1-(int)digitalRead(PIN_SW_XL);  //Invert So it Behaves like the limit SW
  stateSW_XR    = 1-(int)digitalRead(PIN_SW_XR);  //Invert So it Behaves like the limit SW
  stateSW_YF    = 1-(int)digitalRead(PIN_SW_YF);  //Invert So it Behaves like the limit SW
  stateSW_ZT    = 1-(int)digitalRead(PIN_SW_ZT);  //Invert So it Behaves like the limit SW
  stateSW_PB    = 1-(int)digitalRead(PIN_SW_PB);  //Invert So it Behaves like the limit SW




}


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

