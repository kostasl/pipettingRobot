/*
 * Contains functions that define standard programs for pipetting
 * 
 * Make structures and position list for Pippetting
 */


/*
 * Generates Program Struct for dispensing to 18 Vials
 */
  
 //pip dispense 2 X:50 Y:50,Z:3500,P:-805
 //pip drop 2 X:50 Y:50,Z:3500,P:0
 //pip take tip pos i: 4 X:4918 Y:4160,Z:29902,P:-2801 
 //pip Above Tip pos i: 5 X:4918 Y:4160,Z:28486,P:-2801 
 //Place Food In Vial pos i: 7 X:4039 Y:5146,Z:27425,P:-2801 
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

const uint16_t upZPos   = 4653;
const uint16_t downZPos = 25425;
const int      dispPPos = -538; //Pip Pressed to Stage 1
const int      deprPPos = -2801; //Pip dePressed 

prog_position* newpos;


/////////Pickup Tip//////////// 
//pos i: 1 X:4895 Y:4247,Z:-24976,P:-2500 
//pos i: 2 X:4895 Y:4247,Z:-24976,P:-2500 
//pos i: 3 X:4895 Y:4247,Z:29209,P:-2500 
//Big pip Above Tip pos i: 5 X:4918 Y:4160,Z:28486,P:-2801 
 
//Move Over Tip//
  newpos = &gposbuffer[prog->posCount];//prog->telosPos+sizeof(prog_position);
  *newpos               = *(prog->telosPos); //Copy Data Over
  newpos->Xpos          = 4945; //1st tip in corner with box stuck on back of machine  
  newpos->Ypos          = 4202; //  
  newpos->Zpos          = 15000; //Mid Way Down  
  newpos->Ppos          = -2800; //DePress  
  newpos->seqID         = prog->posCount;
  newpos->epomPos       = 0; //IMportant to set this to 0 So clear end of list 
  
  prog->telosPos->epomPos = newpos;
  prog->telosPos          = newpos; //Update That Last Pos Is this new pos         
  prog->posCount++;
 

//Move Down INto Tip
//Big pip take tip pos i: 4 X:4918 Y:4160,Z:29902,P:-2801 pos i: 3 X:4945 Y:4202,Z:29688,P:-2470 
  newpos = &gposbuffer[prog->posCount];//prog->telosPos+sizeof(prog_position);
  *newpos               = *(prog->telosPos); //Copy Data Over
  newpos->Xpos          = 4945; //Press  
  newpos->Ypos          = 4202; //Press  
  newpos->Zpos          = 29688; //Press  
  newpos->Ppos          = -2800; //Press  
  newpos->seqID         = prog->posCount;
  newpos->epomPos       = 0; //IMportant to set this to 0 So clear end of list 
  
  prog->telosPos->epomPos = newpos;
  prog->telosPos          = newpos; //Update That Last Pos Is this new pos         
  prog->posCount++;



//Move Up With TIP
  newpos = &gposbuffer[prog->posCount];//prog->telosPos+sizeof(prog_position);
  *newpos               = *(prog->telosPos); //Copy Data Over
  newpos->Xpos          = 4918; //Press  
  newpos->Ypos          = 4160; //Press  
  newpos->Zpos          = 1600; //Press  
  newpos->Ppos          = -2800; //DePress  
  newpos->seqID         = prog->posCount;
  newpos->epomPos       = 0; //IMportant to set this to 0 So clear end of list 
  
  prog->telosPos->epomPos = newpos;
  prog->telosPos          = newpos; //Update That Last Pos Is this new pos         
  prog->posCount++;


for (int i=0; i<vialCount;i++)
{

  ///Move OVER To Food
  //pos i: 2 X:827 Y:3245,Z:1,P:-2500 1 
  /// X:905 Y:3548,Z:1,P:-580 
  newpos = &gposbuffer[prog->posCount];//prog->telosPos+sizeof(prog_position);
  *newpos               = *(prog->telosPos); //Copy Data Over
  newpos->Xpos          = 905; //Press  
  newpos->Ypos          = 3548; //Press  
  newpos->Zpos          = 1; //Press  
  newpos->Ppos          = -580; //Press  
  newpos->seqID         = prog->posCount;
  newpos->epomPos       = 0; //IMportant to set this to 0 So clear end of list 
  
  prog->telosPos->epomPos = newpos;
  prog->telosPos          = newpos; //Update That Last Pos Is this new pos         
  prog->posCount++;
  
  
  //Move Into Food
  //Big Tip X:830 Y:3371,Z:9340,P:-2500  
  newpos = &gposbuffer[prog->posCount];//prog->telosPos+sizeof(prog_position);
  *newpos               = *(prog->telosPos); //Copy Data Over
  newpos->Xpos          = 830; //Press  
  newpos->Ypos          = 3371; //Press  
  newpos->Zpos          = 9340; //Press  
  newpos->Ppos          = dispPPos; //Press  
  newpos->seqID         = prog->posCount;
  newpos->epomPos       = 0; //IMportant to set this to 0 So clear end of list 
  
  prog->telosPos->epomPos = newpos;
  prog->telosPos          = newpos; //Update That Last Pos Is this new pos         
  prog->posCount++;


  // Take Food Use Pipette
  //pos i: 2 X:1057 Y:3158,Z:9088,P:-2801 
  newpos = &gposbuffer[prog->posCount];//prog->telosPos+sizeof(prog_position);
  *newpos               = *(prog->telosPos); //Copy Data Over
  newpos->Xpos          = 830; //Press  
  newpos->Ypos          = 3371; //Press  
  newpos->Zpos          = 9340; //Press  
  newpos->Ppos          = deprPPos; //Press  
  newpos->seqID         = prog->posCount;
  newpos->epomPos       = 0; //IMportant to set this to 0 So clear end of list 
  
  prog->telosPos->epomPos = newpos;
  prog->telosPos          = newpos; //Update That Last Pos Is this new pos         
  prog->posCount++;
//
  //Move Out of Food 
  //pos i: 2 X:1057 Y:3158,Z:9088,P:-2801 
  newpos = &gposbuffer[prog->posCount];//prog->telosPos+sizeof(prog_position);
  *newpos               = *(prog->telosPos); //Copy Data Over
  newpos->Xpos          = 830; //Press  
  newpos->Ypos          = 3371; //Press  
  newpos->Zpos          = 1; //All way Up  
  newpos->Ppos          = deprPPos; //Press  
  newpos->seqID         = prog->posCount;
  newpos->epomPos       = 0; //IMportant to set this to 0 So clear end of list 
  
  prog->telosPos->epomPos = newpos;
  prog->telosPos          = newpos; //Update That Last Pos Is this new pos         
  prog->posCount++;

  
 ////////////Move Over Vial///////////////
 //Big Tip Place Food In Vial pos i: 7 X:4039 Y:5146,Z:27425,P:-2801 
///Distance Betwee Vials Apos i: 4 X:4038 Y:5142,Z:10608,P:-2470   B pos i: 5 X:4071 Y:5630,Z:10608,P:-2470 ~ DY :490

  newpos = &gposbuffer[prog->posCount];//prog->telosPos+sizeof(prog_position);
  newpos->Xpos          = 4039 - 490*(i/6);
  newpos->Ypos          = 5146 + 490*(i%6);
  newpos->Zpos          = upZPos;
  newpos->Ppos          = deprPPos;
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
  newpos->Ppos          = deprPPos; 
  newpos->seqID         = prog->posCount;
  newpos->epomPos        = 0; //IMportant to set this to 0 So clear end of list 
  
  prog->telosPos->epomPos = newpos;
  prog->telosPos          = newpos; //Update That Last Pos Is this new pos         

  prog->posCount++;
  showProgPos(newpos);

  ////Press Pipette 
  newpos = &gposbuffer[prog->posCount];//prog->telosPos+sizeof(prog_position);;
  *newpos               = *prog->telosPos; //Copy Data Over
  newpos->Ppos          = dispPPos; //Press 
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


