
/*

State Machine code

Note to convert this to separate compiled file, add the .ccp file extension and then , #include "Arduino.h",
*/
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
        
         if (btn_JR_lim.onPressed()){
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
        {//Refresh Move command Until Limit Switch is hit
          stepperZ.move(-5000);
          stepperP.move(-5000);
          //stepperX.move(-5000);
          //stepperY.move(-5000);

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
           if (iposCurrentIndex <= iposSaveIndex)
           { 

              iposCurrentIndex++;
              nextState = TEST_RUN; //Do it again
          }else
          { //DOne the sequence - Go Back HOME
            nextState = HOMING;
            iposCurrentIndex = 0;
          }
         }

     
      break;
      
      case JOYSTICK:
 
        stepperY.setMaxSpeed(2*abs(posJRy));
        stepperX.setMaxSpeed(2*abs(posJRx));
        stepperZ.setMaxSpeed(2000);
        stepperP.setMaxSpeed(1500);
        stepperZ.setAcceleration(2500);   


        
        if (abs(posJRy) > LH_MIN_JOY_MOVE)
        {
         stepperY.move(posJRy);
        }
        else
          stepperY.move(0);
          
        if (abs(posJRx) > LH_MIN_JOY_MOVE)
           stepperX.move(posJRx);
         else
           stepperX.move(0);

        if (stateSW_BT1 == 1)
        {
           stepperZ.move(-1000); //Up Z Axis
//           display.setCursor(0,0);
//           display.println("Z Up");
//           display.display();
        }
         //else
           
        if (stateSW_BT4 == 1) //Down Z Axis
        {
           stepperZ.move(1000);

        }
         //else
           //stepperZ.move(0);

        if(stateSW_BT1==0 && stateSW_BT4 == 0)
           stepperZ.move(0);

        if (stateSW_BT2 == 1) //Up Pippete
           stepperP.move(400);

        if (stateSW_BT5 == 1) //Down Pippete
           stepperP.move(-400);

         if(stateSW_BT2==0 && stateSW_BT5 == 0)
           stepperP.move(0);

        if (btn_JR_lim.onPressed()){ //Click So Save New Position
            nextState = SAVE_POSITION;
         }

        if (stateSW_BT3 == 1){ //Click So as to Replay saved POsitions
            nextState = TEST_RUN;
         }

           
      break;

      case SAVE_POSITION:

        //Only Move Back to Joystick Once Button Has been released          
        if (btn_JR_lim.onReleased()) //Click So Save New Position
          nextState = JOYSTICK;
        else
          nextState = SAVE_POSITION;

      break;

      case RESET:
        nextState = IDLE;
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

        reset(); //Reset Motor Speeds / Accell
        stepperX.moveTo(-8000); 
        stepperY.moveTo(-8000);
        stepperZ.moveTo(-20000);
        stepperP.moveTo(-8000);
        

        stateTimeOut =  millis()+65000; //With timeout
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
        stateTimeOut =  0; //No timeout
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
      { //Need the Brackets To bypass C restriction on newpos initialization
            iposSaveIndex++;
            //Create New Position Struct
            prog_position* newpos = new prog_position;
            //savedPositions[iposSaveIndex]   
            newpos->Xpos = stepperX.currentPosition();
            newpos->Ypos = stepperY.currentPosition();
            newpos->Zpos = stepperZ.currentPosition();
            newpos->Ppos = stepperP.currentPosition();

            //savedPositions[iposSaveIndex-1].epomPos = newpos;
            savedPositions[iposSaveIndex] = *newpos;
            //char buff[130];
            sprintf(buff,"Saved Pos i: %d X:%ld Y:%ld,Z:%ld,P:%ld ", iposSaveIndex,savedPositions[iposSaveIndex].Xpos, savedPositions[iposSaveIndex].Ypos, savedPositions[iposSaveIndex].Zpos,savedPositions[iposSaveIndex].Ppos );
            Serial.println(buff);

            free(newpos);
            dispState();
            display.display();

            

            
            systemState = SAVE_POSITION;
      }
      break;

      case RESET:
          reset();
          Serial.println("-RESET-");
          dispState();
          display.display();

          systemState = RESET;

      break;
      

      default: //Uknown option
        nextState = systemState; //Reset Next State to current
    }
  
}


