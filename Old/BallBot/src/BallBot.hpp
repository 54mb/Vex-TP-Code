//
//  BallBot.hpp
//  
//
//  Created by Sam Burton on 04/12/2018.
//

#ifndef __BallBot__
#define __BallBot__

#include "drive.hpp"
#include "main.h"
#include "speedmech.hpp"
#include "posmech.hpp"

// Motor Setup
#define TURBO E_MOTOR_GEARSET_06
#define SPEED E_MOTOR_GEARSET_18
#define TORQUE E_MOTOR_GEARSET_36
#define RAW E_MOTOR_ENCODER_COUNTS

#define FLYWHEEL 1
#define ARM 2

#define HIGH 1
#define MIDDLE 2

// #defines for controller buttons
#define BTN_FIRE_LOW DIGITAL_L1
#define BTN_FIRE_HIGH DIGITAL_L1
#define BTN_FIRE_BOTH DIGITAL_L1
#define BTN_FIRE_PRESET DIGITAL_L1
#define BTN_INTAKE_IN DIGITAL_L1
#define BTN_INTAKE_OUT DIGITAL_L1
#define BTN_INTAKE_TOGGLE DIGITAL_L1
#define BTN_ABORT DIGITAL_L1

#define BTN_TOGGLE_MODE DIGITAL_L1

#define BTN_ARM_HIGH DIGITAL_L1
#define BTN_ARM_LOW DIGITAL_L1
#define BTN_ARM_DOWN DIGITAL_L1
#define BTN_WRIST_VERTICAL DIGITAL_L1
#define BTN_WRIST_FORWARD DIGITAL_L1
#define BTN_WRIST_BACK DIGITAL_L1
#define BTN_FLIP DIGITAL_L1

// #defines for arm positions
#define FLIP_POS1 0
#define FLIP_POS2 0
#define WRIST_BACK_POS 0
#define WRIST_FORWARD_POS 0
#define WRIST_VERTICAL_POS 0
#define ARM_POS_HIGH 0
#define ARM_POS_LOW 0
#define ARM_POS_DOWN 0

// #defines for tuning
#define FLYWHEEL_AIM_RANGE 5    // fire ball when within x degrees of flag

using namespace pros;


Controller controller(E_CONTROLLER_MASTER);

// Motors
// Motor name(port, gearing, reversed, encoder_units);

// Drive Motors
Motor mDriveLeft_1(1, TURBO, 0, RAW);
Motor mDriveLeft_2(2, TURBO, 1, RAW);
Motor mDriveLeft_3(3, TURBO, 0, RAW);
Motor mDriveRight_1(4, TURBO, 1, RAW);
Motor mDriveRight_2(5, TURBO, 0, RAW);
Motor mDriveRight_3(6, TURBO, 1, RAW);

// Flywheel Motors
Motor mFlywheel_1(7, TURBO, 0, RAW);
Motor mFlywheel_2(8, TURBO ,1, RAW);
// Intake
Motor mIntakeIn(9, SPEED, 0, RAW);
Motor mIntakeOut(10, TURBO, 0, RAW);

// Arm Motors
Motor mArm_1(11, TORQUE, 0, RAW);
Motor mArm_2(12, TORQUE, 1, RAW);
// Flipper
Motor mWrist(13, TORQUE, 0, RAW);
Motor mFlip(14, TORQUE, 0, RAW);

// Declare objects as globals
SpeedMachine flyWheel = SpeedMachine();
PositionMachine arm = PositionMachine();
PositionMachine wrist = PositionMachine();
PositionMachine flipper = PositionMachine();
DriveMachine drive = DriveMachine();

// Auton Routines
extern int autonSelect;
extern double defaultAuton[];
extern double auton1[];

double gyroDirection = 0;
bool hasInitialised = false;

// Declare and initialize any global flags we need:
// Control mode
int controlMode = FLYWHEEL;

// Auton Control
double* autonCommand = &defaultAuton[0];    // default auto routine

// For Flywheel
int autoFireState = -1;         // -1 for neutral, 1 for 'aim&spin&fire', 2 for 'spin & fire', 3 for 'fire!'
int targetFlag = 1;             // 1 for low, 2 for high, 3 for high then low

//For Intake
bool forceIntake = false;
double intakeSpeedOuter = 0;    // speed for outer intake
double intakeSpeedInner = 0;    // speed for inner intake
double flyWheelDefaultSpeed = 0;    // set speed for fixed-dist fireing
int runTillBall = 0;            // 0 = nothing, 1 = run till 1 ball in, 2 = run for two balls

// For cap mechanisms
double armSeek = -1;
double wristSeek = -1;
double flipperSeek = -1;
int stackTarget = -1;
int stackStep = -1;

// Array for flywheel speed lookup'
// Distance (tiles), low flag speed (rpm), high flag speed (rpm)
// For each distance we record flywheel speeds needed for hitting high/low flags

double flyWheelSpeeds[3][3] = {
    {-100, 0, 0},   // to catch errors
    {0, 25, 50},
    {0.5, 100, 125},
};
int flyWheelSpeedsDefinition = 3;   // number of entries

// Shared Function Prototypes
double processEntry();
void setGyro(double);
void runArm(void*);
void runFlywheel(void*);
void initAll();


#endif
