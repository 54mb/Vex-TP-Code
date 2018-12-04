#include "main.h"
#include "BallBotAutons.h"
#include "BallBot.h"


/**
 * Runs the user autonomous code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the autonomous
 * mode. Alternatively, this function may be called in initialize or opcontrol
 * for non-competition testing purposes.
 *
 * If the robot is disabled or communications is lost, the autonomous task
 * will be stopped. Re-enabling the robot will restart the task, not re-start it
 * from where it left off.
 */

extern Drive drive;
extern PositionMachine flipper;
extern PositionMachine arm;
extern PositionMachine wrist;
extern int stackTarget;
extern int stackStep;
extern int autoFireState;
extern double flipperSeek;
extern double wristSeek;
extern int runTillBall;
extern int targetFlag;
extern double armSeek;
extern double gyroDirection;

extern double* autonCommand;

void autonomous() {
    
    initAll();
    
    // Start task
    pros::Task flywheelTask (runFlywheel);
    pros::Task armTask (runArm);
    
    bool nextCommand = true;
    int driveMode = 0;
    double pauseTime = 0;
    // Set pointer to chosen auton routine
    if (autonSelect == 1) autonCommand = &auton1[0];
    
    
    // First entry is always starting direction,
    setGyro(*autonCommand);
    drive.setDirection(*autonCommand);
    
    
    while (true) {
        
        // Auton table decipherer - switch statement
        // Commands will set flags / call object funtions
        // Need commands for:
        // DRIVE (Time, distance, whiteline)
        // TURN (Abs & relative)
        // PAUSE (Time, till ball shot)
        // SETGYRO
        // FIRE (Auto aim, high & low)
        // INTAKE (Time / Until ball enters)
        double ds,dd,dt;
        if (nextCommand) {
            switch ((int)processEntry()) {
                case END:
                    // Do nothing
                    break;
                case PAUSE:
                    pauseTime = processEntry();
                    if (pauseTime > 0) pauseTime = pauseTime * 1000 + pros::millis();
                    break;
                case DRIVE:
                    ds = processEntry();
                    dd = processEntry();
                    dt = processEntry();
                    if (dt < 0) {
                        if (dt == DISTANCE) {
                            driveMode = dt;
                            drive.driveDist(ds,dd,processEntry(),processEntry());
                        }
                        else {
                            driveMode = dt;
                            drive.driveCustom(ds,dd,processEntry());
                        }
                    }
                    else {
                        driveMode = 0;
                        drive.driveTime(ds,dd,dt);
                    }
                    break;
                case TURN:
                    drive.turnTo(processEntry(),processEntry());
                    break;
                case TURN_REL:
                    drive.turnRelative(processEntry(),processEntry());
                    break;
                case TURN_ENC:
                    drive.turnRelativeEncoder(processEntry(),processEntry());
                    break;
                case FIRE_PRESET:
                    autoFireState = 3;
                    nextCommand = true;
                    break;
                case FIRE_AIM:
                    autoFireState = 1;
                    targetFlag = processEntry();
                    nextCommand = true;
                    break;
                case INTAKE_ON:
                    runTillBall = 2;
                    nextCommand = true;
                    break;
                case INTAKE_OFF:
                    runTillBall = 0;
                    nextCommand = true;
                    break;
                case ARMSEEK:
                    armSeek = processEntry();
                    nextCommand = true;
                    break;
                case WRISTSEEK:
                    wristSeek = processEntry();
                    nextCommand = true;
                    break;
                case FLIPPERSEEK:
                    flipperSeek = processEntry();
                    nextCommand = true;
                    break;
                case FLIP:
                    if (flipper.getPosition() > (FLIP_POS1 + FLIP_POS2)/2) {
                        flipperSeek = FLIP_POS1;
                    }
                    else {
                        flipperSeek = FLIP_POS2;
                    }
                    nextCommand = true;
                    break;
                case STACK_LOW:
                    stackTarget = LOW;
                    stackStep = 1;
                    break;
                case STACK_HIGH:
                    stackTarget = HIGH;
                    stackStep = 1;
                    break;
                case STACK_LOW_FROM:
                    stackTarget = LOW;
                    stackStep = processEntry();
                    break;
                case STACK_HIGH_FROM:
                    stackTarget = HIGH;
                    stackStep = processEntry();
                    break;
                default:
                    break;
            }
        }
        
        
        
        // Read any sensors and set global variables
        // eg. gyroDirection, etc.
        
        
        
        // Auton command termination code
        // Decide if we should move to the next command
        // eg. checking timers for pause, flags for shooting balls, etc.
        bool terminateDrive = false;
        if (driveMode == WHITELINE) {
            if (/* read white line sensor */false) {
                terminateDrive = true;
            }
        }
        
        if (stackTarget != 0 && stackStep == -1) {
            stackTarget = 0;
            nextCommand = true;
        }
        
        if (pauseTime > 0) {
            if (pros::millis() > pauseTime) {
                pauseTime = 0;
                nextCommand = true;
            }
        }
        else {
            if (pauseTime == FIRED && autoFireState == -1) {
                nextCommand = true;
            }
            if (pauseTime == GOTBALL) {}
        }
        
        if (terminateDrive) {
            driveMode = 0;
            drive.stop();
            nextCommand = true;
        }
        
        // Maybe set the direction of the drive using the gyro here
        // drive.setDirection(gyroDirection);
        drive.run();
        
        pros::delay(20);   // let other tasks use cpu
    }
    
}
