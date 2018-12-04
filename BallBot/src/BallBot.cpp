//
//  BallBot.cpp
//  
//
//  Created by Sam Burton on 04/12/2018.
//

#include "BallBot.h"

// Auton Routines
extern int autonSelect;
extern double defaultAuton[];
extern double auton1[];

// Declare objects as globals
Drive drive = Drive();
SpeedMachine flyWheel = SpeedMachine();
PositionMachine arm = PositionMachine();
PositionMachine wrist = PositionMachine();
PositionMachine flipper = PositionMachine();

pros::Controller controller(pros::E_CONTROLLER_MASTER);

pros::Motor intakeInner(1);
pros::Motor intakeOuter(2);

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


void initAll() {        // called when robot activates & start of auton
    if (!hasInitialised) {
        // First time / manual init...
        // eg. calibrate gyro
        
    }
    hasInitialised = true;
    // Every time init...
    // eg. tare arm position
    
}


double processEntry() {
    autonCommand++;
    return *autonCommand;
}



void setGyro(double dir) {
    // Set sensor value for gyro
    gyroDirection = dir;
    
}


void pre_auton( void ) {
    // Set-up SpeedMachine for flywheel
    // Set-up Drive for drive
    // Set-up 3 speed machines for cap mechanisms
    
}


void runArm(void* p) {
    bool justFlipped = false;
    
    while (true) {
        
        // If we want to stack something, follow the steps
        switch (stackStep) {
            case 1:
                
                break;
            case 2:
                
                break;
            default:
                stackStep = -1;
                break;
        }
        
        // Check controller inputs
        if (controlMode == ARM) {
            if (BTN_FLIP) {
                stackStep = -1;
                if (!justFlipped) {
                    if (flipper.getPosition() > (FLIP_POS1 + FLIP_POS2)/2) {
                        flipperSeek = FLIP_POS1;
                    }
                    else {
                        flipperSeek = FLIP_POS2;
                    }
                }
                justFlipped = true;
            }
            else {
                justFlipped = false;
            }
            if (controller.get_digital(BTN_WRIST_BACK)) {
                stackStep = -1;
                wristSeek = WRIST_BACK_POS;
            }
            if (controller.get_digital(BTN_WRIST_FORWARD)) {
                stackStep = -1;
                wristSeek = WRIST_FORWARD_POS;
            }
            if (controller.get_digital(BTN_WRIST_VERTICAL)) {
                stackStep = -1;
                wristSeek = WRIST_VERTICAL_POS;
            }
            if (controller.get_digital(BTN_ARM_HIGH)) {
                stackStep = -1;
                armSeek = ARM_POS_HIGH;
            }
            if (controller.get_digital(BTN_ARM_HIGH)) {
                stackStep = -1;
                armSeek = ARM_POS_LOW;
            }
            if (controller.get_digital(BTN_ARM_DOWN)) {
                stackStep = -1;
                armSeek = ARM_POS_DOWN;
            }
        }
        if (BTN_ABORT) {
            wristSeek = -1;
            armSeek = -1;
            flipperSeek = -1;
            stackStep = -1;
        }
        
        // If we need to seek, then tell the arm, wrist, and flipper
        if (armSeek > 0) {
            arm.hold(armSeek);
        }
        else {
            arm.stop();
        }
        if (wristSeek > 0) {
            wrist.hold(wristSeek);
        }
        else {
            wrist.stop();
        }
        if (flipperSeek > 0) {
            flipper.hold(flipperSeek);
        }
        else {
            flipper.stop();
        }
        
        arm.run();
        wrist.run();
        flipper.run();
        
        pros::delay(20);   // don't hog cpu
    }
}


void runFlywheel(void* p) {
    
    // Declare any local variables
    bool ballIsIn = false;
    bool ballWasIn = false;
    bool justToggledAutoBall = false;
    
    while (true) {
        
        // Set intake motor speeds to 0
        if (!forceIntake) {
            intakeSpeedInner = 0;
            intakeSpeedOuter = 0;
        }
        
        // keep flywheel at default speed
        flyWheel.runAtSpeed(flyWheelDefaultSpeed);
        
        if (autoFireState != -1) {  // Auto fire
            
            // Check vision sensor to determine necessary turn
            double relativeAngle = 0;
            if (autoFireState <= 1) {
                // Read sensor and find relative angle
                
                
            }
            
            // Check lidar / ultrasonic for distance
            double distance = 0;
            if (autoFireState <= 2) {
                // Read sensor and find distance
                
                
            }
            
            // Lookup distance in flywheelSpeeds table, & interpolate to find speed
            double targetSpeed = flyWheelDefaultSpeed;
            if (autoFireState <= 2) {
                int index = -1;
                for (int i = 1; i < flyWheelSpeedsDefinition; i++) {
                    // Look for speed too high
                    if (flyWheelSpeeds[i][0] >= distance) {
                        index = i;
                        break;
                    }
                }
                if (index == -1) {
                    // Further than furthest in table
                    if (targetFlag == 1) targetSpeed = flyWheelSpeeds[flyWheelSpeedsDefinition][1];
                    else targetSpeed = flyWheelSpeeds[flyWheelSpeedsDefinition][2];
                }
                else {
                    // Interpolate for correct speed
                    // find how similar distance is to each value
                    double distDiff = (distance - flyWheelSpeeds[index-1][0]);
                    distDiff /= (flyWheelSpeeds[index][0] - flyWheelSpeeds[index-1][0]);
                    double speedDiff;
                    // Find how similar speed should be to each value & set target speed
                    if (targetFlag == 1) {
                        speedDiff = (flyWheelSpeeds[index][1] - flyWheelSpeeds[index-1][1]);
                        targetSpeed = speedDiff*distDiff + flyWheelSpeeds[index-1][1];
                    }
                    else {
                        speedDiff = (flyWheelSpeeds[index][2] - flyWheelSpeeds[index-1][2]);
                        targetSpeed = speedDiff*distDiff + flyWheelSpeeds[index-1][2];
                    }
                }
            }
            
            // Read flags for & set flywheel object target speed
            flyWheel.runAtSpeed(targetSpeed);
            
            // Read vision sensor & ask drive to turn appropriately
            drive.turnRelative(relativeAngle,-1);
            
            bool fireBall = false;
            // Check current speed of flywheel & if aimed
            if ((flyWheel.isWithinRange() && abs(relativeAngle) < FLYWHEEL_AIM_RANGE)) {
                // Set flag for fireing ball
                fireBall = true;
            }
            
            // Check if ball is in ready position
            ballIsIn = false;
            // read sensors to check if ball is in
            
            
            
            if (!ballIsIn && !ballWasIn) {    // move ball into position if not already
                intakeSpeedOuter = 100;
                intakeSpeedInner = 25;
            }
            
            if (fireBall) {         // aimed and running correct speed
                // Run intake motor
                intakeSpeedInner = 100;
                
                if (ballWasIn && !ballIsIn) {   // ball has left
                    // Clear flags
                    fireBall = false;
                    if (targetFlag == 3) {  // wanted to shoot high then low
                        targetFlag = 1;
                    }
                    else {
                        autoFireState = -1;
                    }
                }
            }
            
            
        }   // end of auto-fire
        
        
        
        // Check controller buttons...
        // Set flags for preset flywheel speeds & auto-aim-fire
        // If manual intake buttons pressed, override intake speeds
        if (controlMode == FLYWHEEL) {
            if (controller.get_digital(BTN_FIRE_LOW)) { // auto fire low
                autoFireState = 1;
                targetFlag = 1;
            }
            if (controller.get_digital(BTN_FIRE_HIGH)) { // auto fire high
                autoFireState = 1;
                targetFlag = 2;
            }
            if (controller.get_digital(BTN_FIRE_BOTH)) { // auto fire both
                autoFireState = 1;
                targetFlag = 3;
            }
            if (controller.get_digital(BTN_FIRE_PRESET)) { // auto fire preset
                autoFireState = 3;
            }
            
            if (controller.get_digital(BTN_INTAKE_IN)) { // manual run intake in
                intakeSpeedInner = 25;
                intakeSpeedOuter = 100;
                runTillBall = 0;
                forceIntake = false;
            }
            if (controller.get_digital(BTN_INTAKE_OUT)) { // manual run intake out
                intakeSpeedInner = -25;
                intakeSpeedOuter = -100;
                runTillBall = 0;
                forceIntake = false;
            }
            if (controller.get_digital(BTN_INTAKE_TOGGLE)) { // toggle auto ball intake
                if (!justToggledAutoBall) {
                    if (runTillBall) runTillBall = 0; else runTillBall = 2;
                }
                justToggledAutoBall = true;
            }
            else {
                justToggledAutoBall = false;
            }
        }
        if (controller.get_digital(BTN_ABORT)) {     // cancel auto functions
            autoFireState = -1;
            runTillBall = 0;
            forceIntake = false;
        }
        
        if (runTillBall) {
            if (/*read inner sensor*/false) {     // ball is not all the way in
                intakeSpeedOuter = 100;
                intakeSpeedInner = 25;
            }
            else if (/*read outer sensor*/false && (runTillBall == 2)) {   // 1 ball is in, but not 2
                intakeSpeedOuter = 100;
            }
        }
        
        
        // Send speeds to intake motors
        intakeInner.move_voltage(intakeSpeedInner*12000);
        intakeOuter.move_voltage(intakeSpeedOuter*12000);
        
        // Run the flywheel
        flyWheel.run();
        
        // Remember ball info for fireing
        ballWasIn = ballIsIn;
        
        
        pros::delay(20);   // don't hog cpu
    }
}
