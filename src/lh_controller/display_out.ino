/*
 * DISPLAY CODE 
 * 
 * Functions handle display output relative to current state
 */

//#include <stdint.h>
#include <avr/pgmspace.h>

#define BWD_Dithergimp_width 128
#define BWD_Dithergimp_height 64
//--Brainwave discovery Logo----------------------------------------------------
// File generated by LCD Assistant
// http://en.radzio.dxp.pl/bitmap_converter/
//------------------------------------------------------------------------------
//Remember PROGMEM Is necessary
const unsigned char BWD_Dither_mono[] PROGMEM = {
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xF7, 0xFF, 0xF7, 0xFF, 0xF7, 0xFF,
0xF7, 0xFF, 0xC3, 0xFF, 0xE3, 0xFF, 0xC3, 0xFF, 0xC3, 0xFF, 0xC3, 0xFF, 0xC3, 0xFF, 0xC1, 0xFF,
0xF1, 0xFF, 0x81, 0xFF, 0x89, 0xFF, 0x81, 0xFF, 0xC1, 0xFF, 0xC1, 0xFF, 0xC1, 0xFF, 0xC1, 0xFF,
0xF9, 0xFF, 0x9D, 0xFF, 0x99, 0xFF, 0x98, 0xFF, 0xB8, 0xFF, 0x9D, 0xFF, 0x9C, 0xFF, 0x88, 0xFF,
0xFC, 0xFF, 0x9C, 0xFF, 0x3C, 0xFF, 0xBC, 0xFF, 0x9C, 0xFF, 0x1C, 0xFF, 0x9E, 0xFF, 0x9C, 0xFF,
0xFC, 0xFF, 0x3C, 0xFF, 0x3C, 0xFF, 0x3C, 0x7F, 0x7C, 0xFF, 0x7C, 0xFF, 0x3C, 0xFF, 0x3E, 0xFF,
0xFE, 0x7E, 0x3E, 0x7E, 0x3E, 0x7F, 0x3E, 0x7F, 0x3E, 0x7E, 0x3E, 0x7F, 0x3E, 0x7F, 0x3E, 0x7F,
0xFE, 0x7E, 0x7E, 0x7E, 0xFE, 0x7E, 0x3E, 0x3E, 0x7E, 0x7E, 0x7E, 0x7E, 0x7E, 0x7E, 0x7E, 0x7F,
0xFF, 0x7C, 0x7F, 0x7C, 0x7F, 0x7E, 0x7F, 0x3E, 0x7F, 0x7E, 0x7F, 0x7E, 0x7F, 0x7E, 0x7F, 0x7F,
0xFF, 0x7C, 0xFF, 0x3C, 0xFF, 0x3E, 0x7F, 0x3E, 0xFF, 0x7E, 0xFF, 0x3E, 0xFF, 0x7E, 0xFF, 0x3F,
0xFF, 0x3C, 0xFF, 0xBC, 0xFF, 0xBE, 0x7F, 0xBC, 0xFF, 0x3C, 0xFF, 0xBC, 0xFF, 0x3C, 0xFF, 0xBF,
0xFF, 0xB9, 0xFF, 0xB9, 0xFF, 0xBC, 0xFF, 0xBD, 0xFF, 0xBD, 0xFF, 0x9D, 0xFF, 0xB9, 0xFF, 0x9F,
0xFF, 0x81, 0xFF, 0x81, 0xFF, 0x8C, 0xFF, 0x9D, 0xFF, 0x99, 0xFF, 0x8D, 0xFF, 0x89, 0xFF, 0xDF,
0xFF, 0xC3, 0xFF, 0xC3, 0xFF, 0xC9, 0xFF, 0xC3, 0xFF, 0xC3, 0xFF, 0xC1, 0xFF, 0xC3, 0xFF, 0xCF,
0xFF, 0xE7, 0xFF, 0xE7, 0xFF, 0xE3, 0xFF, 0xE7, 0xFF, 0xC3, 0xFF, 0xE3, 0xFF, 0xE7, 0xFF, 0xEF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xF7, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xF7, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0xF3, 0xFF, 0xFF, 0x3F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xF2, 0x7F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0xF3, 0xFF, 0xFF, 0xBF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xF3, 0x7F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0xF3, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xF3, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0xF0, 0x30, 0x83, 0x30, 0x66, 0x64, 0x33, 0x30, 0xFF, 0x02, 0x61, 0x86, 0x19, 0x98, 0xE2, 0x67,
0xF3, 0x10, 0x91, 0x32, 0x24, 0x48, 0x93, 0x26, 0x7F, 0x22, 0x4F, 0x34, 0xC9, 0xB2, 0x62, 0x67,
0xF3, 0x93, 0x99, 0x33, 0x25, 0x49, 0xD9, 0x22, 0x62, 0x72, 0x63, 0x3D, 0xED, 0xA0, 0x6F, 0x0F,
0xF3, 0x93, 0x39, 0x33, 0x31, 0x19, 0xD8, 0x67, 0xC3, 0x72, 0x71, 0x3D, 0xED, 0xA3, 0xEF, 0x0F,
0xF3, 0xB3, 0x81, 0x33, 0x31, 0x99, 0x1C, 0x67, 0xFF, 0x22, 0x79, 0xBC, 0xCC, 0x73, 0xEF, 0x8F,
0xF0, 0x33, 0x85, 0x33, 0x39, 0x9C, 0x5C, 0xF0, 0xFF, 0x06, 0x41, 0x86, 0x1E, 0x78, 0x6F, 0x9F,
0xFF, 0xFF, 0xFF, 0xFB, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x7F, 0xFF, 0xFF, 0xFF, 0xFF, 0x9F,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xBF, 0x3F,
0xFF, 0xEA, 0xBF, 0xFD, 0x05, 0x57, 0xEF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFD, 0x7F, 0x3F,
0xFF, 0xFE, 0xA2, 0xBE, 0xFB, 0x68, 0x95, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFA, 0xA2, 0xBA, 0xC9,
0x00, 0x05, 0x54, 0x05, 0x09, 0x56, 0x90, 0x00, 0x00, 0x00, 0x00, 0x00, 0x09, 0x08, 0x84, 0x54,
0xAA, 0xD5, 0x55, 0x40, 0xAA, 0x54, 0x55, 0x40, 0x02, 0xAA, 0xAA, 0xD6, 0xD5, 0x77, 0x75, 0x2B,
0xAA, 0xAA, 0xAA, 0xBA, 0x01, 0x2B, 0x55, 0x7F, 0xFA, 0x4A, 0xAA, 0xAA, 0xAA, 0x95, 0x55, 0xAA,
0x55, 0x55, 0x55, 0x45, 0x50, 0x04, 0xAA, 0xAA, 0xAA, 0xA8, 0x49, 0x55, 0x44, 0xA2, 0x10, 0x00,
0xDA, 0xAA, 0xAA, 0xB5, 0x55, 0x00, 0x02, 0x00, 0x02, 0x4A, 0x40, 0x00, 0x02, 0x12, 0x00, 0x00,
0x55, 0xB6, 0xDD, 0xAA, 0xAA, 0xE0, 0x09, 0x55, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0x55, 0x5B, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0xEE, 0xED, 0x2A, 0xAD, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0x6A, 0xAB, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xD4, 0x57, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x50, 0x17, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF
};

 const int disp_LIM_SPACING = 33; //Spacing required for Displaying X ON switches Text - Can use display.getTextBounds for this .getTextBounds(string, x, y, &y1, &y1, &w, &h);
 const int disp_LOW_LINE = 57;
 

void dispWelcome()
{
    display.setFont(&FreeMono9pt7b);
    //display.setTextSize(1);

    // Clear the buffer.
  // miniature bitmap display
  display.clearDisplay();
  // void drawBitmap(int16_t x, int16_t y, uint8_t *bitmap, int16_t w, int16_t h, uint16_t color);
 display.drawBitmap(0, 0,  BWD_Dither_mono,  BWD_Dithergimp_width , BWD_Dithergimp_height , 1);
  display.invertDisplay(true);
  
  //display.stopscroll();
  //display.setTextColor (WHITE,BLACK); // 'inverted' text
  //display.setTextSize(1);
  //display.setCursor(0,11);
  //display.println(TXT_TITLE);
  //display.setTextSize(1);
  //display.setCursor(0,28);

  //display.print(".Breathing.");
  display.display();
  //display.invertDisplay(false);

  delay(1500);
  display.invertDisplay(false);

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
        sprintf(buff,"X:%ld \t Y:%ld \nZ:%ld  \t P:%ld",savedPrograms[0]->telosPos->Xpos, savedPrograms[0]->telosPos->Ypos, savedPrograms[0]->telosPos->Zpos,savedPrograms[0]->telosPos->Ppos );

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

