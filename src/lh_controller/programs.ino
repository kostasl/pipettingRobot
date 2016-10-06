/*
 * Contains functions that define standard programs for pipetting
 * 
 * Make structures and position list for Pippetting
 */


/*
 * Generates Program Struct for dispensing to 18 Vials
 */

 
void prog_DispenseFoodToVialsPos(t_program* prog,uint8_t vialCount)
{

prog->telosPos = prog->protoPos;

Serial.println((int)prog);
Serial.println(vialCount);
Serial.println((int)prog->telosPos);
assert(prog);
assert(vialCount > 0);
assert(prog->telosPos);
assert(vialCount < MAX_POSITIONS/2);

strcpy(prog->progname,"Fill_FdV");

const uint16_t upZPos   = 28653;
const uint16_t downZPos = 32653;

prog_position* newpos;

for (int i=0; i<vialCount;i++)
{

  ///Move OVER To Food
  //pos i: 1 X:1057 Y:3158,Z:629,P:-2801 
  newpos = &gposbuffer[prog->posCount];//prog->telosPos+sizeof(prog_position);
  *newpos               = *(prog->telosPos); //Copy Data Over
  newpos->Xpos          = 1057; //Press  
  newpos->Ypos          = 3158; //Press  
  newpos->Zpos          = 629; //Press  
  newpos->Ppos          = -1000; //Press  
  newpos->seqID         = prog->posCount;
  newpos->epomPos       = 0; //IMportant to set this to 0 So clear end of list 
  
  prog->telosPos->epomPos = newpos;
  prog->telosPos          = newpos; //Update That Last Pos Is this new pos         
  prog->posCount++;
  
  
  //Move Into Food
  //pos i: 2 X:1057 Y:3158,Z:9088,P:-2801 
  newpos = &gposbuffer[prog->posCount];//prog->telosPos+sizeof(prog_position);
  *newpos               = *(prog->telosPos); //Copy Data Over
  newpos->Xpos          = 1057; //Press  
  newpos->Ypos          = 3158; //Press  
  newpos->Zpos          = 9088; //Press  
  newpos->Ppos          = -1000; //Press  
  newpos->seqID         = prog->posCount;
  newpos->epomPos       = 0; //IMportant to set this to 0 So clear end of list 
  
  prog->telosPos->epomPos = newpos;
  prog->telosPos          = newpos; //Update That Last Pos Is this new pos         
  prog->posCount++;

  
  //Move Take Food Use Pipette
  //pos i: 2 X:1057 Y:3158,Z:9088,P:-2801 
  newpos = &gposbuffer[prog->posCount];//prog->telosPos+sizeof(prog_position);
  *newpos               = *(prog->telosPos); //Copy Data Over
  newpos->Xpos          = 1057; //Press  
  newpos->Ypos          = 3158; //Press  
  newpos->Zpos          = 9088; //Press  
  newpos->Ppos          = -2800; //Press  
  newpos->seqID         = prog->posCount;
  newpos->epomPos       = 0; //IMportant to set this to 0 So clear end of list 
  
  prog->telosPos->epomPos = newpos;
  prog->telosPos          = newpos; //Update That Last Pos Is this new pos         
  prog->posCount++;
  

  //Move out from the  Food 
  //pos i: 2 X:1057 Y:3158,Z:9088,P:-2801 
  newpos = &gposbuffer[prog->posCount];//prog->telosPos+sizeof(prog_position);
  *newpos               = *(prog->telosPos); //Copy Data Over
  newpos->Xpos          = 1057; //Press  
  newpos->Ypos          = 3158; //Press  
  newpos->Zpos          = 629; //Press  
  newpos->Ppos          = -2800; //Press  
  newpos->seqID         = prog->posCount;
  newpos->epomPos       = 0; //IMportant to set this to 0 So clear end of list 
  
  prog->telosPos->epomPos = newpos;
  prog->telosPos          = newpos; //Update That Last Pos Is this new pos         
  prog->posCount++;
  
 
  
 ////////////Move Over Vial///////////////
  newpos = &gposbuffer[prog->posCount];//prog->telosPos+sizeof(prog_position);
  newpos->Xpos          = 4034 - 470*(i/6);
  newpos->Ypos          = 5300 + 470*(i%6);
  newpos->Zpos          = upZPos;
  newpos->Ppos          = -2500;
  newpos->seqID         = prog->posCount;
  newpos->epomPos        = 0; //IMportant to set this to 0 So clear end of list 

  prog->telosPos->epomPos = newpos;
  prog->telosPos          = newpos; //Update That Last Pos Is this new pos         

  prog->posCount++;
  showProgPos(newpos);

 //Move DOWN Vial
  newpos = &gposbuffer[prog->posCount];//prog->telosPos+sizeof(prog_position);;
  *newpos               = *prog->telosPos; //Copy Data Over
  newpos->Zpos          = downZPos; //39758; //Move Down  
  newpos->Ppos          = -2500; //39758; //Move Down  
  newpos->seqID         = prog->posCount;
  newpos->epomPos        = 0; //IMportant to set this to 0 So clear end of list 
  
  prog->telosPos->epomPos = newpos;
  prog->telosPos          = newpos; //Update That Last Pos Is this new pos         

  prog->posCount++;
  showProgPos(newpos);

  ////Press Pipette 
  newpos = &gposbuffer[prog->posCount];//prog->telosPos+sizeof(prog_position);;
  *newpos               = *prog->telosPos; //Copy Data Over
  newpos->Ppos          = -1000; //Press 
  newpos->Zpos          = downZPos; //Press   
  newpos->seqID         = prog->posCount;
  newpos->epomPos        = 0; //IMportant to set this to 0 So clear end of list 
  
  prog->telosPos->epomPos = newpos;
  prog->telosPos          = newpos; //Update That Last Pos Is this new pos         

  prog->posCount++;
  showProgPos(newpos);

  ////MOVE UP VIAL  
  newpos = &gposbuffer[prog->posCount];//prog->telosPos+sizeof(prog_position);
  *newpos               = *(prog->telosPos); //Copy Data Over
  newpos->Zpos          = upZPos; //Press  
  newpos->seqID         = prog->posCount;
  newpos->epomPos       = 0; //IMportant to set this to 0 So clear end of list 
  
  prog->telosPos->epomPos = newpos;
  prog->telosPos          = newpos; //Update That Last Pos Is this new pos         

  prog->posCount++;

  showProgPos(newpos);

  
  Serial.print(F("Added Vial: "));
  Serial.println(i);
   
  //Serial.print("telos MPos: ");
  //Serial.println((int)prog->telosPos);


}//Loop For Each  Vial

 //Done
}



//Initializes a new Program Data structure 
//If pointer not empty then it deletes the existing structure first 
void prog_init(t_program* prog)
{

  if (prog != 0)
  {
    prog_clearPoslist(prog);
    //delete(prog); //avoid this
  }
  
  
  //Init Prog / Create 1st defaultPosition where all programs start from
  prog_position* newpos = gposbuffer;
  newpos->Xpos  = 0;
  newpos->Ypos  = 0;
  newpos->Zpos  = 0;
  newpos->Ppos  = -2500;
  newpos->seqID = 0;

  //prog = new  t_program;
  prog->posCount = 1; //First Default Position Saved
  prog->protoPos = newpos; //First Position
  prog->epiPos   = newpos; //Current Position
  prog->telosPos = newpos; //Last Position
  strcpy(prog->progname,("NEW PROGRAM")); //Name of 1st Program

}

///Delete Clear memory of structs in program list of positions
void prog_clearPoslist(t_program* prog)
{
  int n = 0;
  
      if (prog == 0)
        return;
      if (prog->protoPos == 0 || prog->posCount == 0 )
        return;

      memset(prog->protoPos,0,sizeof(t_program)*MAX_POSITIONS);

/*     ///Need to clear LIst of positions too
      prog_position* cpos    = prog->protoPos;
      prog_position* nxtpos  = 0;
      while (n < prog->posCount)
      {
        nxtpos = cpos->epomPos;
        //delete(cpos);
        cpos = 0;        
        cpos = nxtpos;
        n++;
      }
      */

    prog->posCount = 0;


}


