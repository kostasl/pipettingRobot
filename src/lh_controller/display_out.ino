/*
 * DISPLAY CODE 
 * 
 * Functions handle display output relative to current state
 */

 
 const int disp_LIM_SPACING = 33; //Spacing required for Displaying X ON switches Text - Can use display.getTextBounds for this .getTextBounds(string, x, y, &y1, &y1, &w, &h);
 const int disp_LOW_LINE = 57;
 

void dispWelcome()
{
    display.setFont(&FreeMono9pt7b);
    //display.setTextSize(1);

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


/*
 * 
 */
//Show Text Relating to the Current System System STATE
void dispState()
{
    // Clear the buffer.
  // miniature bitmap display
  display.clearDisplay();
  //display.stopscroll();
  display.setFont(&FreeMono9pt7b);
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
        sprintf(buff," %d Saved", iposSaveIndex);
        display.println(buff);
        display.setFont();
        sprintf(buff,"X:%ld \t Y:%ld \nZ:%ld  \t P:%ld",savedPositions[iposSaveIndex].Xpos, savedPositions[iposSaveIndex].Ypos, savedPositions[iposSaveIndex].Zpos,savedPositions[iposSaveIndex].Ppos );

        display.println(buff);
        
      break;

      case RESET:
        display.setCursor(0,11);
        display.print(iposSaveIndex);
        display.println(" RESET ");
      break;

      default:
      break;
    }
  
      ////Clear The Switch Status - Clear Screen On Swithc
        //display.setFont(); //Revert To Standart Font
      if (!(btn_XR_lim.isPressed() || btn_XL_lim.isPressed()))
          display.fillRect(0,disp_LOW_LINE,disp_LIM_SPACING,19,BLACK);

      if (!(btn_YB_lim.isPressed() || btn_YB_lim.isPressed()) )
          display.fillRect(disp_LIM_SPACING,disp_LOW_LINE,disp_LIM_SPACING,19,BLACK);

      if (!(btn_ZT_lim.isPressed()) )
          display.fillRect(disp_LIM_SPACING*2,disp_LOW_LINE,disp_LIM_SPACING,19,BLACK);
      
      if (!(btn_PB_lim.isPressed()) )
          display.fillRect(disp_LIM_SPACING*3,disp_LOW_LINE,disp_LIM_SPACING,19,BLACK);
      

  
  //display.display();

}

/////////////////END DISPLAY CODE ////////////////

