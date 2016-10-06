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

Serial.println((int)prog);
Serial.println(vialCount);
Serial.println((int)prog->telosPos);
assert(prog);
assert(vialCount > 0);
assert(prog->telosPos);
assert(vialCount < MAX_POSITIONS/2);

strcpy(prog->progname,"Fill_FdV");

for (int i=1; i<=vialCount;i++)
{

  
 //Move Over Vial
  prog_position* newpos = prog->telosPos+sizeof(prog_position);;
  newpos->Xpos          = 4034 + 470*(i/6);
  newpos->Ypos          = 5300 + 470*i;
  newpos->Zpos          = 1000;
  newpos->Ppos          = -2500;
  newpos->seqID         = prog->posCount;
  newpos->epomPos        = 0; //IMportant to set this to 0 So clear end of list 

  prog->telosPos->epomPos = newpos;
  prog->telosPos          = newpos; //Update That Last Pos Is this new pos         

  prog->posCount++;

 //Move DOWN Vial
  newpos = prog->telosPos+sizeof(prog_position);;
  *newpos               = *prog->telosPos; //Copy Data Over
  newpos->Zpos          = 31653; //39758; //Move Down  
  newpos->seqID         = prog->posCount;
  newpos->epomPos        = 0; //IMportant to set this to 0 So clear end of list 
  
  prog->telosPos->epomPos = newpos;
  prog->telosPos          = newpos; //Update That Last Pos Is this new pos         

  prog->posCount++;

  ////Press Pipette 
  newpos = prog->telosPos+sizeof(prog_position);;
  *newpos               = *prog->telosPos; //Copy Data Over
  newpos->Ppos          = -1000; //Press 
  newpos->Zpos          = 31653; //Press   
  newpos->seqID         = prog->posCount;
  newpos->epomPos        = 0; //IMportant to set this to 0 So clear end of list 
  
  prog->telosPos->epomPos = newpos;
  prog->telosPos          = newpos; //Update That Last Pos Is this new pos         

  prog->posCount++;

  ////MOVE UP VIAL  
  newpos = prog->telosPos+sizeof(prog_position);
  *newpos               = *prog->telosPos; //Copy Data Over
  newpos->Zpos          = 30653; //Press  
  newpos->seqID         = prog->posCount;
  newpos->epomPos       = 0; //IMportant to set this to 0 So clear end of list 
  
  prog->telosPos->epomPos = newpos;
  prog->telosPos          = newpos; //Update That Last Pos Is this new pos         

  prog->posCount++;

   
  Serial.print("Added Pos: ");
  Serial.println(prog->posCount);
}//Loop For Each  Vial

 //Done
}



//Initializes a new Program Data structure 
//If pointer not empty then it deletes the existing structure first 
void prog_init(t_program* prog)
{

  if (prog != 0)
  {
    //prog_clearPoslist(prog);
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

      memset(prog->protoPos,0,sizeof(t_program)*prog->posCount);

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


