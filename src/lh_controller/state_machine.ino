
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
         nextState = IDLE;

         if (btn_JR_lim.onPressed()){
           if (selectedProgramFile == "NEW PROGRAM"){
              nextState = JOYSTICK;
           }else
           if (selectedProgramFile == "FILL FOOD VIALS"){
              nextState = HOMING;
              prog_DispenseFoodToVialsPos(savedPrograms,4);
           }
           else{ //Load the selected File 
               nextState = LOAD_PROGRAM;
           }
         }

         //Shift Selected File With Joystick Movement
         if (posJRy > LH_MIN_JOY_MOVE && (gi_filelistSelectedIndex > 0))
         {
            gi_filelistSelectedIndex--;
            dispState();
            display.display();

         }
         
         if ((posJRy < -LH_MIN_JOY_MOVE) && (gi_filelistSelectedIndex < (gi_numberOfProgFiles-1)))
         {
            gi_filelistSelectedIndex++;
            //Serial.println(gi_filelistSelectedIndex);
            dispState();
            display.display();
         }


      break;

      case HOMING: //2 //Focuses on lifting the Z axis out of Vials
        //Check if a PUMP Move has been completed
        if (checkHoming() > 3)  //Homing is complete Anyway
        {
          nextState = HOME;
        }else{        
          if (abs(stepperZ.distanceToGo()) < 50) //Carry on With Stage 2 Homing 
          {
            nextState = HOMING_XY;
          }else
            nextState = HOMING;
        }
      break;
      
      case HOMING_XY:
        if (checkHoming() > 3)  //Homing is complete Anyway
        {
          nextState = HOME;
        }else
          nextState = HOMING_XY;
                     
      break;

      
      case HOME: //3 //Reached Home - Stay Here Unclick Switches
            //If  a program has been loaded / Run It
//           char buff[100];
//           sprintf(buff,("Prog Opened  %s has n:%d"),savedPrograms[0]->progname,savedPrograms[0]->posCount);
//           Serial.println(buff); 
            if (savedPrograms[0].posCount > 1)
            {
                nextState = TEST_RUN;
            }else //if not then wait until state timesout to IDLE
             {
                nextState = HOME;
             }
      break;      

      
      case TEST_RUN:
 
          
          nextState = MOVING;
      break;

      case MOVING:
            nextState = MOVING;

         if (stepperX.distanceToGo()==0 && stepperY.distanceToGo()==0 && stepperZ.distanceToGo()==0 && stepperP.distanceToGo()==0)
         {
            //displState();
          //Do not Exceed Last saved Position - Check If Next Is Null
           if (savedPrograms[0].epiPos !=0)
           { 
              //iposCurrentIndex++;
             
              nextState = TEST_RUN; //Do it again
          }else
          { 
            char buff[60];
            sprintf(buff,("End of Program at pos i: %d "), savedPrograms[0].telosPos->seqID);
            Serial.println(buff);
            //DOne the sequence 
            //Reset Program To Beginning 
            //savedPrograms[0].epiPos = savedPrograms[0].protoPos;
            savedPrograms[0].posCount = 1; //Stop From Running Again
            
            //- Go Back HOME after and reset to Unload Program
            //reset();
            nextState = HOMING;
            ///iposCurrentIndex = 0;
          }
         }

            //Interrupt Button/
         if (stateSW_BT3 == 1){ //Click So as to Replay saved POsitions
          nextState = IDLE;
         }


     
      break;
      
      case JOYSTICK:
 
        stepperY.setMaxSpeed(2*abs(posJRy));
        stepperX.setMaxSpeed(2*abs(posJRx));
        stepperZ.setMaxSpeed(2300);
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
           stepperZ.move(-1200); //Up Z Axis
//           display.setCursor(0,0);
//           display.println("Z Up");
//           display.display();
        }
         //else
           
        if (stateSW_BT4 == 1) //Down Z Axis
        {
           stepperZ.move(1200);

        }
         //else
           //stepperZ.move(0);

        if(stateSW_BT1==0 && stateSW_BT4 == 0)
           stepperZ.move(0);

        if (stateSW_BT2 == 1) //Up Pippete
           stepperP.move(-400);

        if (stateSW_BT5 == 1) //Down Pippete
           stepperP.move(400);

         if(stateSW_BT2==0 && stateSW_BT5 == 0)
           stepperP.move(0);

        if (btn_JR_lim.onPressed()){ //Click So Save New Position
            nextState = SAVE_POSITION;
         }

        if (stateSW_BT3 == 1){ //Click So as to save POsitions to file
            nextState = SAVE_PROGRAM;
         }

           
      break;

      case SAVE_POSITION:

        //Only Move Back to Joystick Once Button Has been released          
        if (btn_JR_lim.onReleased()) //Click So Save New Position
          nextState = JOYSTICK;
        else
          nextState = SAVE_POSITION;

      break;
      case SAVE_PROGRAM:
          nextState = HOMING;
      break;

      case LOAD_PROGRAM:
        nextState = HOMING; //First Do Homing before Running Program
        //nextState = TEST_RUN;
      break;
      
      case RESET:
        nextState = IDLE;
      break;
      
      case POS_ERROR:
        
        nextState = HOMING;
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
        reset(); //Reset Motor Speeds / Accell

        stepperX.stop();
        stepperY.stop();
        stepperZ.stop();
        stepperP.stop();

        gi_numberOfProgFiles = loadProgramFileNames(); //Load File names from SD card

        systemState = IDLE;
      break;
      
      case HOMING: //DO Partial Z and P axis First, Then Follow up with XY
        setMotorSpeeds(); //replaced Reset with Just Setting Motors

        stepperZ.moveTo(-14000);
        stepperP.moveTo(6000); //It will hit Limit Switch So Distance Doesnt matter

        stateTimeOut =  millis()+85000; //With timeout
        systemState = HOMING;
      break;

      case HOMING_XY: //Used so Z axis Is lifted 1st before the others such that we avoid homing while in a vial
        setMotorSpeeds(); //replaced Reset with Just Setting Motors
        stepperZ.moveTo(-35000);
        stepperX.moveTo(-26000); 
        stepperY.moveTo(-26000);

        stateTimeOut =  millis()+85000; //With timeout
        
        systemState = HOMING_XY;
      break;
      
      case HOME: //nOW sYTEM REACHED hOME / Release SWitches    
      //For Some Reason Using the Main Loop With Move To Does not Work / Need todo a blocking call here
       stepperX.setCurrentPosition(0);
       stepperY.setCurrentPosition(0);
       stepperZ.setCurrentPosition(0);
       stepperP.setCurrentPosition(0);

       stepperX.runToNewPosition(50);
       stepperY.runToNewPosition(50);
       stepperZ.runToNewPosition(3500);
       stepperP.runToNewPosition(-2500);

        
        systemState = HOME;
        
        stateTimeOut =  millis() + 3000; 
      break;
      

      case TEST_RUN:
      {
        setMotorSpeeds(); //replaced Reset with Just Setting Motors

        prog_position* nxtpos;
        nxtpos = savedPrograms[0].epiPos;
                  
        stepperX.moveTo(nxtpos->Xpos); 
        stepperY.moveTo(nxtpos->Ypos);
        stepperZ.moveTo(nxtpos->Zpos);
        stepperP.moveTo(nxtpos->Ppos);

        sprintf(buff,("Run to Pos i: %d X:%ld Y:%ld,Z:%ld,P:%ld "), nxtpos->seqID, nxtpos->Xpos, nxtpos->Ypos, nxtpos->Zpos, nxtpos->Ppos );
        ////INcrement tonext Position  if not at end
        if (savedPrograms[0].epiPos != savedPrograms[0].telosPos)   
        {
          savedPrograms[0].epiPos = nxtpos->epomPos; //Change pointer to next Pos
        }else
        //If Moved to Last One, then Make Sure Next Pos Is null
//        if (savedPrograms[0]->epiPos == savedPrograms[0]->telosPos)  
        {
          savedPrograms[0].epiPos = 0; //Set To Null
        }
        Serial.println(buff);

        stateTimeOut =  millis()  + 35000; //Give 30sec timeout until move executes
        systemState = TEST_RUN;
      }
      break;
      
      case MOVING:
      //Nothing Here
          systemState = MOVING;
      break;

      case JOYSTICK:
          systemState = JOYSTICK;
          
          stateTimeOut =  millis()+75000; //With timeout
      
      break;

      case SAVE_POSITION: //Add Current Position TO list Of Saved positions
      { //Need the Brackets To bypass C restriction on newpos initialization
            iposSaveIndex++;
            assert(iposSaveIndex < MAX_POSITIONS);
            //Create New Position Struct
            prog_position* newpos = savedPrograms[0].telosPos+sizeof(prog_position); //Move Pointer To next Memory Position
            //savedPositions[iposSaveIndex] 
            newpos->Xpos = stepperX.currentPosition();
            newpos->Ypos = stepperY.currentPosition();
            newpos->Zpos = stepperZ.currentPosition();
            newpos->Ppos = stepperP.currentPosition();

            //savedPositions[iposSaveIndex-1].epomPos = newpos;
            //savedPositions[iposSaveIndex] = *newpos;
            savedPrograms[0].telosPos->epomPos = newpos;
            savedPrograms[0].telosPos          = newpos; //Update That Last Pos Is this new pos
            
            newpos->epomPos                   = 0; //IMportant to set this to 0 So clear end of list 
            newpos->seqID                     = savedPrograms[0].posCount;
            
            savedPrograms[0].posCount++;
            
            //char buff[130];
            sprintf(buff,("Saved Pos i: %d X:%ld Y:%ld,Z:%ld,P:%ld "), savedPrograms[0].telosPos->seqID, savedPrograms[0].telosPos->Xpos, newpos->Ypos, newpos->Zpos,newpos->Ppos );
            Serial.println(buff);

            //free(newpos);
            dispState();
            display.display();
            
            systemState = SAVE_POSITION;
      }
      break;
      
      case SAVE_PROGRAM:
      {
        //Set Prog Name To Incremented file
        String filename = "EOS_";
        filename += String(gi_numberOfProgFiles+1) + String(".PRG");
        strcpy(savedPrograms[0].progname,filename.c_str());
        
        saveProgram(&savedPrograms[0]); //Save the 1st Program

//        fileroot = SD.open("/");
//        printDirectory(fileroot, 0);
//        fileroot.close();

        gi_numberOfProgFiles = loadProgramFileNames();

        systemState = SAVE_PROGRAM;
      }
      break;
      
      case LOAD_PROGRAM:
      {
  
        //prog_clearPoslist(savedPrograms[0]);
        savedPrograms[0] = *loadProgram((selectedProgramFile.c_str()));      

        if (savedPrograms[0].posCount > 0)
            systemState = LOAD_PROGRAM;
        else
          {
           display.println("Error Loading file");
           display.display();
          }
      }
      break;

      case RESET:
          reset();
          Serial.println("-RESET-");
          dispState();
          display.display();

          systemState = RESET;

      break;

      case POS_ERROR:
         stateTimeOut =  millis()  + 3000; //Give 10sec timeout until move executes
         if (checkHoming() > 0) //Recheck
         {         
           Serial.println(F("ERR101 - lim sw hit while replay"));
           dispState();
           display.display();
           systemState = POS_ERROR;
         }
         
      break;

      default: //Uknown option
        nextState = systemState; //Reset Next State to current
    }
  
}


