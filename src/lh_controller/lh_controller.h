/* Header file
   Pipeting Robot Controller code -running on Arduino Mega 2560  ///////////////


*/
#define __ASSERT_USE_STDERR

#define TXT_TITLE          "-AutoPip-" // Machine Name


//  PIN DEFINITIONS //


#define PIN_MOTOR_P_EN    2 //Pipette drive
#define PIN_MOTOR_P_STEP  3
#define PIN_MOTOR_P_DIR   4

#define PIN_MOTOR_Z_EN    11
#define PIN_MOTOR_Z_STEP  12
#define PIN_MOTOR_Z_DIR   13

#define PIN_MOTOR_Y_DIR   24
#define PIN_MOTOR_Y_STEP  26
#define PIN_MOTOR_Y_EN    28

#define PIN_MOTOR_X_DIR   30
#define PIN_MOTOR_X_STEP  32
#define PIN_MOTOR_X_EN    34


#define PIN_AJR_X         0 //Joystick R
#define PIN_AJR_Y         1
#define PIN_SW_JR         36 //JoyStick R PushSwitch
#define PIN_SW_BT1        38 //JoyStick BTN 1 PushSwitch
#define PIN_SW_BT2        40 //JoyStick BTN 1 PushSwitch
#define PIN_SW_BT3        42 //JoyStick BTN 1 PushSwitch
#define PIN_SW_BT4        44 //JoyStick BTN 1 PushSwitch
#define PIN_SW_BT5        46 //JoyStick BTN 1 PushSwitch

#define PIN_CS_SDCARD     53 //Chipselect for SD Card 

//Limit Switches - X and Y axes have on both sides -Mounted On Header PINS
#define PIN_SW_YB          23
#define PIN_SW_YF          25
#define PIN_SW_XR          27
#define PIN_SW_XL          29
#define PIN_SW_ZT          31
#define PIN_SW_PB          33

#define LH_MIN_JOY_MOVE    20 //Minimum Joystick Reading Before Moving Motor
#define LIM_PIPETTE_UPTIX  -2800 //Upper Allowed position Of Pippete in #steps (stopping upper colission)

#define BTN_DEBOUNCE_TIMEMS 100 //DEbounce of LImit Switches in ms

#define MAX_NUMBER_OF_FILES 25 //Define Size of filename holding array

RBD::Button btn_YB_lim(PIN_SW_YB);
RBD::Button btn_YF_lim(PIN_SW_YF);
RBD::Button btn_XR_lim(PIN_SW_XR);
RBD::Button btn_XL_lim(PIN_SW_XL);
RBD::Button btn_PB_lim(PIN_SW_PB);
RBD::Button btn_ZT_lim(PIN_SW_ZT);

RBD::Button btn_JR_lim(PIN_SW_JR); //Joystic Button Press
bool flag_P_lim = false; //Flag holding state of Soft Limit Switch


// These are neede for both hardware & softare SPI /Modified lib to give SPI_CLOCK_DIV4 
#define OLED_RESET          49
#define OLED_DC             48
#define OLED_CS             47 //Cannot use Blue Cable on which MISO Port is on, when in SPI mode

//For Future SD Card Reader SPI Use 
///N/A The Green MISO Line Is unused -
//ChipSelect >?

#define MAX_POSITIONS 100 //Size Of Position Buffer
#define MAX_PROGS 1

// EG X-Y position bed driven by 2 steppers
// Alas its not possible to build an array of these with different pins for each :-(
// Up to 10 steppers can be handled as a group by MultiStepper

// Defines a target position to which the robot will move into - Coords defined as stepper positions
//Be carefu
struct list_position {
  uint8_t    seqID;//Id In Position Sequence
  int16_t     Ppos;
  uint16_t    Xpos;
  uint16_t    Ypos;
  uint16_t    Zpos;
  struct list_position  *epomPos; //Next Position   - Null Means  this Final Position
}; 

typedef struct list_position prog_position;

typedef struct {
  uint16_t ID;
  uint16_t posCount; //Number of positions
  uint8_t repsRemain; //Defines remaining Number of repetitions during a run
  uint8_t totalReps; //Total  Number of repetitions of a subroutine
  char progname[18]; //The program Name
  char timestamp[10]; //Last Mod Date
  list_position* protoPos;
  list_position* epiPos;
  list_position* telosPos;
  
} t_program;


//list_position savedPositions[MAX_POSITIONS];
t_program savedPrograms[MAX_PROGS]; //Array Of pointers to Saved Programs
prog_position gposbuffer[MAX_POSITIONS]; //Pointer To statically allocated Memory Buffer to save positions


int iposSaveIndex     = 0; // Index Of last position saved on this program  
int iposCurrentIndex  = 0; //Index Of currently running position of program


AccelStepper stepperX(AccelStepper::DRIVER,PIN_MOTOR_X_STEP,PIN_MOTOR_X_DIR); // Defaults to AccelStepper::FULL4WIRE (4 pins) on 2, 3, 4, 5
AccelStepper stepperY(AccelStepper::DRIVER,PIN_MOTOR_Y_STEP,PIN_MOTOR_Y_DIR); // Defaults to AccelStepper::FULL4WIRE (4 pins) on 2, 3, 4, 5
AccelStepper stepperZ(AccelStepper::DRIVER,PIN_MOTOR_Z_STEP,PIN_MOTOR_Z_DIR); // Defaults to AccelStepper::FULL4WIRE (4 pins) on 2, 3, 4, 5
AccelStepper stepperP(AccelStepper::DRIVER,PIN_MOTOR_P_STEP,PIN_MOTOR_P_DIR); // Defaults to AccelStepper::FULL4WIRE (4 pins) on 2, 3, 4, 5

//MultiStepper steppers; -Not Currently Used - Needs to be customized

///Screen Vars   
//Adafruit_SSD1306 display(OLED_RESET);
// this is for hardware SPI, fast! but fixed oubs
Adafruit_SSD1325 display(OLED_DC, OLED_RESET, OLED_CS);

///STATE Variables
enum ENUM_LH_STATE {IDLE=1, HOMING=2,HOMING_XY=20, HOME=3, TEST_RUN=4, MOVING=5, JOYSTICK=6, SAVE_POSITION=7, LOAD_PROGRAM=8, SAVE_PROGRAM=9, RESET=10, POS_ERROR =11 ,LAST_STATE=12};

ENUM_LH_STATE systemState = LAST_STATE;
ENUM_LH_STATE nextState = HOMING;

//Limit Switches Global Variables
int stateSW_XL = 1;
int stateSW_XR = 1;
int stateSW_YF = 1;
int stateSW_YB = 1;
int stateSW_ZT = 1;
int stateSW_PB = 1;
int stateSW_JR = 1; //Joystick
int stateSW_BT1 = 1; //Joystick
int stateSW_BT2 = 1; //Joystick
int stateSW_BT3 = 1; //Joystick
int stateSW_BT4 = 1; //Joystick
int stateSW_BT5 = 1; //Joystick


int posJRx     = 0; //Joystick Analog position X
int posJRy     = 0; //Joystick Analog position X

/// Serial COMMAND interface ///
boolean stringComplete             = false;  // whether the string is complete
String inputString                 = "";         // a string to hold incoming data
unsigned long stateTimeOut         = 0; //An Auxiliary var to set when a state expires and system returns to IDLE
unsigned long stateReportInterval  =  0; //Used to Time a frequent serial output to host. so port does not freeze

//File Loading Variables
String gstr_progfilenames[MAX_NUMBER_OF_FILES]; //Pointer to Array of String Filenames
int gi_filelistSelectedIndex  = 0; //For Listing Programs, start 
int gi_numberOfProgFiles      = 0; //For Listing Programs, start 
int gi_startindexFileList     = 0; //Used For Scrolling Down List indicates First Element
String selectedProgramFile;


 char buff[130]; //Generic Alloc Memory For Messages / Serial
 
// Function Prototypes
int checkHoming(); //Check homing conditions and set pos to 0 when reached
void handleStartStateEvents(); //Handles actions linked to moving to new state
void handleStopStateEvents(); //Checks conditions for when current state end and transits to next one
int handleSerialCmd();

void dispWelcome();
void resetVars();
void setMotorSpeeds();



