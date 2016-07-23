
/*
 * Functions Checking Motor Motion limit switches and events
 */

 
//Return >0 if all positions at home
int checkHoming()
{
  int iret = 0;
    
  //Read Switches Home Reached
  
  //stateSW_XR =  //digitalRead(PIN_SW_XR);
  //stateSW_YB = digitalRead(PIN_SW_YB);
//  stateSW_ZT = digitalRead(PIN_SW_ZT);
//  stateSW_PB = digitalRead(PIN_SW_PB);

  display.setTextColor (WHITE,BLACK); // 'inverted' text
  display.setFont(); //Revert To Standart Font
 
   
   if (btn_XR_lim.onPressed() )
   {//&& stateSW_XR == 1
      stateSW_XR    = 1-(int)digitalRead(PIN_SW_XR);  //Invert So it Behaves like the limit SW
      if (stateSW_XR == 1)  
        {
        stepperX.setCurrentPosition(0); //Set ref Point And Target to 0 Has the side effect of setting the current motor speed to 0. 
        stepperX.stop();
        display.setCursor(0,disp_LOW_LINE);
        display.print("X ON");
        display.display();
        }
     }

    if (btn_XR_lim.isPressed())
    {
      //stateSW_XR

       iret++; //Increment Number of Switches pressed 
       if (stepperX.targetPosition() < 1) //Only stopMotor if next pos is pressing against switch
        {
            stepperX.setCurrentPosition(0); //Set ref Point And Target to 0 Has the side effect of setting the current motor speed to 0. 
           stepperX.moveTo(1);
           stepperX.stop();
       }
    }
    
    
    if (btn_YB_lim.onPressed()) //Triggered Once When State Changes to pressed
    {      
      //&& stateSW_YB == 1 //Verify SW Press
       stateSW_YB    = 1-(int)digitalRead(PIN_SW_YB);  //Invert So it Behaves like the limit SW

       if (stateSW_YB == 1)  
       {
        stepperY.setCurrentPosition(0); //Set As Ref Point Has the side effect of setting the current motor speed to 0. 
        stepperY.stop(); //Just in case future vers change the above
        display.setCursor(disp_LIM_SPACING,disp_LOW_LINE);
        display.print("Y ON");
        display.display();
      }
    }
      
    if (btn_YB_lim.isPressed())
    {
      iret++; //Increment Number of Switches pressed 
      if (stepperY.targetPosition() < 1) //Stop Motor only if Pressing Against LIMIT switch
      {
          stepperY.setCurrentPosition(0); //Set As Ref Point Has the side effect of setting the current motor speed to 0. 

          stepperY.moveTo(1);
          stepperY.stop();
      }
    }

    if (btn_ZT_lim.onPressed())
    {
      ////Verify SW Press
      stateSW_ZT    = 1-(int)digitalRead(PIN_SW_ZT);  //Invert So it Behaves like the limit SW
      if ( stateSW_ZT == 1)
      {
        stepperZ.setCurrentPosition(0);
        stepperZ.stop();
        stepperZ.setSpeed(0);
        display.setCursor(disp_LIM_SPACING*2,disp_LOW_LINE);
        display.print("Z ON");
        display.display();
      }
    }
    
    if (btn_ZT_lim.isPressed())
    {
      iret++; //Increment Number of Switches pressed 

      if (stepperZ.targetPosition() < 1) //Stop Motor only if Pressing Against LIMIT switch
      {
          stepperZ.setCurrentPosition(0); //If not reset again it doesnt seem to stop the pushing against sw

          stepperZ.moveTo(1);
          stepperZ.stop();
  
      }
    }

    
    if (btn_PB_lim.onPressed() )
    {
      //Verify SW Press
        stateSW_PB    = 1-(int)digitalRead(PIN_SW_PB);  //Invert So it Behaves like the limit SW

        if (stateSW_PB == 1)
        {
          stepperP.setCurrentPosition(0);
          stepperP.setSpeed(0);
          stepperP.stop();
          display.setCursor(disp_LIM_SPACING*3,disp_LOW_LINE);
          display.print("P ON");
          display.display();
        }
    }

    if (btn_PB_lim.isPressed())
    {
      
      iret++; //Increment Number of Switches pressed 

      if (stepperP.targetPosition() > 1) //Stop Motor only if Pressing Against LIMIT switch
      {
          stepperP.setCurrentPosition(0);

          stepperP.moveTo(-1);
          stepperP.stop();
  
      }
    }

    

  return iret;
}

// Out Of Range Control
//Check if Carrriege has reached the  Switches at the limit Of Axis opposite of Home - 
// Returns : Number of limit Switches Hit
int checkOutOfRange()
{
  int iret = 0;

  if (btn_XL_lim.onPressed())
  {
        stateSW_XL    = 1-(int)digitalRead(PIN_SW_XL);  //Invert So it Behaves like the limit SW
        if (stateSW_XL == 1)
        {
          stepperX.setSpeed(0);
          display.setCursor(0,disp_LOW_LINE);
          display.print("X ON");
          display.display();
        }
  }
  if (btn_XL_lim.isPressed())
  {
      //Do not allow to push against the switch
        if (stepperX.targetPosition() - stepperX.currentPosition() > 0)
        {
         stepperX.moveTo(stepperX.currentPosition());
         stepperX.stop(); //Just in case future vers change the above
        }
      
    iret++;
  } 
  
  if (btn_YF_lim.onPressed())
  {
        stateSW_YF    = 1-(int)digitalRead(PIN_SW_YF);  //Invert So it Behaves like the limit SW
        if (stateSW_YF == 1)
        {
          stepperY.setSpeed(0);//Just in case future vers change the above
          display.setCursor(disp_LIM_SPACING,disp_LOW_LINE);
          display.print("Y ON");
          display.display(); //These Calls CAuse jitter
        }
  }
  if (btn_YF_lim.isPressed())
  {
        //Do not allow to push against the switch
        if (stepperY.targetPosition() - stepperY.currentPosition() > 0)
        {
           stepperY.moveTo(stepperY.currentPosition());
           stepperY.stop(); //Just in case future vers change the above
        }

  
    iret++;
  }

    //Other Limit Is -LIM_PIPETTE_UPTIX Tix Brings it to the upper lim
      //stepperP.setCurrentPosition(LIM_PIPETTE_UPTIX);
    if (flag_P_lim) //If Limit Hit Already - Then recover if moving back out
    {
      if (stepperP.currentPosition() - stepperP.targetPosition() >= 0)
      {
        stepperP.moveTo(LIM_PIPETTE_UPTIX-1);
        stepperP.setSpeed(0);
        stepperP.stop();
      }else
        flag_P_lim=false; //Unflag
    }
    
    if (stepperP.currentPosition() <= LIM_PIPETTE_UPTIX && flag_P_lim==false)
    {     
          display.setCursor(disp_LIM_SPACING*3,disp_LOW_LINE);
          display.print("P LIM");
          display.display();
          
          flag_P_lim = true;
          
          iret++;
    }


//Return The number of Switches Pressed
return iret;
}



