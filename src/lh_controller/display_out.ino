/*
 * DISPLAY CODE 
 * 
 * Functions handle display output relative to current state
 */

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

      case RESET:
        display.setCursor(0,11);
        display.print(iposSaveIndex);
        display.println(" RESET ");
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

