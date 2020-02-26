
//////////////////////////////////
////		WHITE LINE TRIGGER		////
//////////////////////////////////
int rightWhiteLineAccepted = 1000;					// Values for right whiteline sensors to accept, will depend somewhat on ambient conditions
int leftWhiteLineAccepted = 1000;						// Values for left whiteline sensors to accept, will depend somewhat on ambient conditions


int driveSlewRate = 256;

//////////////////////////
////		STACK SIDES		////
//////////////////////////
#define RIGHT -1														// Right stack side
#define LEFT 1															// Left stack side

//////////////////////////
////		SEEK RATES		////
//////////////////////////
int baseSeekRate = 20;											// How fast to seek base
float sweeperSeekRate = 5;									// How fast to move sweeper
float liftSeekRateUp = 0.5;									// How fast to move lift
float liftSeekRateDown = 0.5;								// How fast to move lift


////////////////////////////
////		STACK VALUES		////
////////////////////////////

int stackSide = 1;													// Side to stack on
int sweeperSwingSpeed = 90;									// Speed to move sweeper
int liftLowPos = 755;												// Where lift is too low for sweeper to swap sides
int sweeperStackSpeed = 30;									// Spee to move sweeper when stacking
int sweeperHoldTime1 = 0;										// How long to pause #1
int sweeperHoldTime2 = 1000;									// How long to pause #2
int sweeperStackPos = 1220;									// Position to move sweeper to stack cone
int lowestConeHeight = 1450;								// Height to stack lowest cone
int coneHeight = 200;												// Height each cone adds to lift stacking height
int liftGrabPos = 1;												// Height lift grabs cone
int liftReadyPos = 1030;												// 650 Height to move lift up before swinging to stack
int sweeperMaxSwing = 127;									// Max sweeper speed
int sweeperHomePos = 2500;									// Where to seek sweeper home
int sweeperHomePosLeft = 2500;							// Where to seek sweeper home left
int sweeperHomePosRight = 1;								// Where to seek sweeper home right


//////////////////////////////////
////		CALIBRATION VALUES		////
//////////////////////////////////
int liftOffset = 58;												// Location the lift starts at
int liftTopPos = 2180;											// Highest lift can ever be
int sweeperOffset = 0;											// Location sweeper starts at


////////////////////////////////
////		LERP CLOSE ENOUGH		////
////////////////////////////////
int sweeperCloseEnough = 50;								// When to stop seeking sweeper
int liftCloseEnough=30;											// When to stop seeking lift
int baseCloseEnough = 100;									// When to stop seeking base


//////////////////////////////////
////		BASE MAGIC NUMBERS		////
//////////////////////////////////
int baseDownPos = 1570;//1430;											// Where to move base lift to put goal down
int baseThrowPos = 2500;										// Where to move base lift to 'throw' base
int baseUpPos = 3300;												// Where to move base lift to pick base up
