/*
  Listfiles

 This example shows how print out the files in a
 directory on a SD card

 The circuit:
 * SD card attached to SPI bus as follows:
 ** MOSI - pin 11
 ** MISO - pin 12
 ** CLK - pin 13
 ** CS - pin 4

 created   Nov 2010
 by David A. Mellis
 modified 9 Apr 2012
 by Tom Igoe
 modified 2 Feb 2014
 by Scott Fitzgerald

 This example code is in the public domain.

 */


void printDirectory(File dir, int numTabs) {
  while (true) {

    File entry =  dir.openNextFile();
    if (! entry) {
      // no more files
      break;
    }
    for (uint8_t i = 0; i < numTabs; i++) {
      Serial.print('\t');
    }
    Serial.print(entry.name());
    if (entry.isDirectory()) {
      Serial.println("/");
      printDirectory(entry, numTabs + 1);
    } else {
      // files have sizes, directories do not
      Serial.print("\t\t");
      Serial.println(entry.size(), DEC);
    }
    entry.close();
  }
}

void testReadWrite()
{
  // open the file. note that only one file can be open at a time,
    // so you have to close this one before opening another.
    File myFile = SD.open("test.txt", FILE_WRITE);
  
    // if the file opened okay, write to it:
    if (myFile) {
      Serial.print("Writing to test.txt...");
      myFile.println("testing 1, 2, 3.");
      // close the file:
      myFile.close();
      Serial.println("done.");
    } else {
      // if the file didn't open, print an error:
      Serial.println("error opening test.txt");
    }
  
    // re-open the file for reading:
    myFile = SD.open("test.txt");
    if (myFile) {
      Serial.println("test.txt:");
  
      // read from the file until there's nothing else in it:
      while (myFile.available()) {
        Serial.write(myFile.read());
      }
      // close the file:
      myFile.close();
    } else {
      // if the file didn't open, print an error:
      Serial.println("error opening test.txt");
    }
}



// Defines a target position to which the robot will move into - Coords defined as stepper positions
//struct list_position {
//  int     seqID;//Id In Position Sequence
//  long    Xpos;
//  long    Ypos;
//  long    Zpos;
//  long    Ppos;
//  struct list_position  *epomPos; //Next Position   - Null Means  this Final Position
//}; 
//
//typedef struct list_position prog_position;
//
//typedef struct {
//  uint16_t ID;
//  uint8_t posCount; //Number of positions
//  uint8_t repsRemain; //Defines remaining Number of repetitions during a run
//  char progname[15]; //The program Name
//  char timestamp[10]; //Last Mod Date
//  list_position* protoPos;
//  list_position* epiPos;
//  list_position* telosPos;
//  
//} t_program;
//Function Loads Program header and list of positions - Memory is not loaded as a block, pointers need to be reinstated
//So assigns the right pointers as it loads
t_program* loadProgram(char* progname)
{
  t_program* prog = new t_program;
  int cnt = 0;
  char buff[100];
  sprintf(buff,("--SD - Load program: %s"),progname);
  Serial.println(buff); 
  
  //Open File with respective progname
  File progFile = SD.open(progname, FILE_READ);
  
  //Load Header
  if (progFile.available())
    progFile.read(prog,sizeof(t_program));
  else
  {
    Serial.println(F("Loading failted"));
    return 0; 
  } 

    sprintf(buff,("Prog Opened  %s has n:"),prog->progname,prog->posCount);
    Serial.println(buff); 
  
  //Load Each Position
     while (progFile.available()) {
      prog_position* loadPos = new prog_position;

       //Read Bytes into structure of program_Position
       //read(void *buf, uint16_t nbyte);
       progFile.read(loadPos,sizeof(prog_position));     


      //Attach Position
      if (cnt == 0)        //Set pointer to 1st position and epiPos (current position to start of program)
      {
        prog->protoPos  = loadPos;
        prog->epiPos    = loadPos;

        prog->telosPos->epomPos = loadPos; //LInk The list , before replacing the pointer to the last Pos
        prog->telosPos = loadPos; //Set last Pos to be the last one loaded
      }
      else
       prog->telosPos = loadPos; //Set last Pos to be the last one loaded

      
       sprintf(buff,"SD. Loaded Pos i: %d X:%ld Y:%ld,Z:%ld,P:%ld ", loadPos->seqID, loadPos->Xpos, loadPos->Ypos, loadPos->Zpos,loadPos->Ppos );
       Serial.println(buff);
     
      cnt++;
    } //End While loop

  //Close File
  progFile.close();

  Serial.println("Done Saving");
  
  return prog;
}

int saveProgram(t_program* prog)
{
  int cnt = 0;
  char buff[100];
  
  sprintf(buff,"--SD - Saving program: %s",prog->progname);
  Serial.println(buff); 

  //Delete File if it exists
  if (SD.exists(prog->progname))
     SD.remove(prog->progname); 
  
  //Open new File for writing program
  File progFile = SD.open(prog->progname, FILE_WRITE);

  //Program has position pointers to save - get the 1st one 
  prog_position* savePos = prog->protoPos;

  //Save Header
  progFile.write((uint8_t*)prog,sizeof(t_program));
 
  //Iterate and save positions - Check Count Just in case next position is not null
  while (savePos != 0 && cnt < prog->posCount) //Stop when 
  {

   //sprintf(buff,"SD Saving pos. i: %d",savePos->seqID);
   sprintf(buff,"SD. Saved Pos i: %d X:%ld Y:%ld,Z:%ld,P:%ld ", savePos->seqID, savePos->Xpos, savePos->Ypos, savePos->Zpos,savePos->Ppos );
   Serial.println(buff);
   
   progFile.write((uint8_t*)savePos,sizeof(prog_position));
   //Switch to Next position in List

   savePos = savePos->epomPos; //Move Pointer To next
   
   cnt++;
  }

  Serial.println("Done Saving");
  //Close File
  progFile.close();
  return 1; //Success

}
