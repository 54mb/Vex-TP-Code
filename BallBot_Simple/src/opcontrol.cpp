//
//
// All code for multi-bot is contained here (minus auton routines)
// Created by Sam Burton
// Contributors:
//
// To Do:
// Test everything
// Calibrate arm/wrist/flipper/flywheel/drive variables & #defines
// Calculate flywheel speed table
// Choose controller mapping & change #defines
// Write vision sensor code
// Write IR / LiDAR code
//
//


#include "main.h"
#include "BallBotAutons.h"

using namespace pros;


#define TURBO E_MOTOR_GEARSET_06
#define SPEED E_MOTOR_GEARSET_18
#define TORQUE E_MOTOR_GEARSET_36
#define RAW E_MOTOR_ENCODER_COUNTS

#define FLYWHEEL 1
#define ARM 2

#define HIGH 1
#define MIDDLE 2

///////////////////////////////////////////////////////////////////////////
// Controller Mapping
// #defines for controller buttons              MAP CONTROLLER
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

#define BTN_ARM_MAN_UP DIGITAL_L1
#define BTN_ARM_MAN_DOWN DIGITAL_L1
#define BTN_WRIST_MAN_UP DIGITAL_L1
#define BTN_WRIST_MAN_DOWN DIGITAL_L1
#define BTN_FLIPPER_MAN_UP DIGITAL_L1
#define BTN_FLIPPER_MAN_DOWN DIGITAL_L1

// #defines for arm positions                   CALCULATE
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

#define armSeekRate 5
#define wristSeekRate 5
#define flipperSeekRate 5

#define flywheelSlowSpeed 60
#define flywheelFastSpeed 100

// Gyro Stuff
#define CSCALE 1    //Clockwise scale adjustments to counteract rotation errors
#define ASCALE 1    //Anti-clockwise scale adjustments to counteract rotation errors
#define GYRO_PORT 1


Controller controller(E_CONTROLLER_MASTER);     // Controller object

///////////////////////////////////////////////////////////////////////////////
// Motors
// Motor name(port, gearing, reversed?, encoder_units);
// Drive Motors
Motor drive_left_1(1, TURBO, 0, RAW);
Motor drive_left_2(2, TURBO, 1, RAW);
Motor drive_left_3(3, TURBO, 0, RAW);
Motor drive_right_1(4, TURBO, 1, RAW);
Motor drive_right_2(5, TURBO, 0, RAW);
Motor drive_right_3(6, TURBO, 1, RAW);
// Flywheel Motors
Motor flywheel_1(7, TURBO, 0, RAW);
Motor flywheel_2(8, TURBO ,1, RAW);
// Intake
Motor intake_in(15, SPEED, 0, RAW);
Motor intake_out(10, TURBO, 0, RAW);
// Arm Motors
Motor arm_1(11, TORQUE, 0, RAW);
Motor arm_2(12, TORQUE, 1, RAW);
// Flipper
Motor wrist(13, TORQUE, 0, RAW);
Motor flip(14, TORQUE, 0, RAW);
// Gyro Sensor
ADIGyro sensor_gyro(1, GYRO_PORT);


///////////////////////////////////////////////////////////////
// Drive tuning variables (CALIBRATE)
// Drive
double deadZone = 10;
double ticksPerTile = 363.2123;
// Turn
double turnAccepted = 1;
double pulsePause = 5;
double pulseTime = 5;
double minSpeed = 2;
double maxTurn = 75;
double turnRate = 30;
double ticksPerDegree = 63.532;
// Tracking
double trackingTicksPerTile = 72.123;
double trackingTicksPerDegree = 12.1234;

// Drive control variables
// Drive
double autoMode = DRIVEMODE_USER;
bool autonComplete = false;
double autoTime = 0;
bool speedOverride = false;
double rightRunSpeed = 0;
double leftRunSpeed = 0;
bool drivingToPos = false;
double autoTimeOut = 0;
double targetDistance = 0;
double autoSpeed = 0;
double minForward = 0;
bool usingGyro = true;
double currentDist = 0;
double recordedTime = 0;
double recordedDistLeft = 0;
double recordedDistRight = 0;
double lastRightEnc = 0;
double lastLeftEnc = 0;
// Turn
double targetDirection = 0;
double turnMode = 0;
double direction = 0;
// Position Tracking
double targetX = 0;
double targetY = 0;
double targetS = 0;
double yPosition = 0;
double xPosition = 0;


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

// For Intake
bool forceIntake = false;
double intakeSpeedOuter = 0;    // speed for outer intake
double intakeSpeedInner = 0;    // speed for inner intake
double flyWheelDefaultSpeed = 0;    // set speed for fixed-dist fireing
int runTillBall = 0;            // 0 = nothing, 1 = run till 1 ball in, 2 = run for two balls

// For cap mechanisms
double armSeek = -1;
double wristSeek = -1;
double flipperSeek = -1;

double armPos = 0;
double flipperPos = 0;
double wristPos = 0;

double armOffset = 0;
double flipperOffset = 0;
double wristOffset = 0;

int stackTarget = -1;
int stackStep = -1;

// Array for flywheel speed lookup'
// Distance (tiles), low flag speed (rpm), high flag speed (rpm)
// For each distance we record flywheel speeds needed for hitting high/low flags

double flyWheelSpeeds[3][3] = {                 // CALIBRATE & add more
    {-100, 0, 0},   // to catch errors
    {0, 25, 50},
    {0.5, 100, 125},
};
int flyWheelSpeedsDefinition = 3;   // number of entries


void setArmPos(double pos) {
    // set all motor encoders to 0
    arm_1.tare_position();
    arm_2.tare_position();
    // set position
    armOffset = pos;
    armPos = pos;
}
void setFlipperPos(double pos) {
    // set all motor encoders to 0
    flip.tare_position();
    // set position
    flipperOffset = pos;
    flipperPos = pos;
}
void setWristPos(double pos) {
    // set all motor encoders to 0
    wrist.tare_position();
    // set position
    wristOffset = pos;
    wristPos = pos;
}


void initAll() {        // called when robot activates & start of auton
    if (!hasInitialised) {
        // First time / manual init...
        // eg. calibrate gyro
        sensor_gyro = ADIGyro(1, GYRO_PORT);
    }
    hasInitialised = true;
    // Every time init...
    // eg. tare arm position
    arm_1.tare_position();
    arm_2.tare_position();
    wrist.tare_position();
    flip.tare_position();
}


double processEntry() {
    autonCommand++;
    return *autonCommand;
}


//////////////////////////////////////////////////////////////////////////////////
// Gyro Stuff (To move into own file)
// gyroDirection will be updated with 'more accurate' gyro value
gyros gyro1,gyro2;

short gyroinit=0;

void resetGyro()
{
    sensor_gyro.reset();
    gyro1.truedir=0;
    gyro2.truedir=0;
    gyro1.last=0;
    gyro2.last=0;
    gyroDirection=0;
}

void setGyro(double dir)
{
    gyro1.truedir=dir;
    gyro2.truedir=dir;
    //    gyro1.last=dir;
    //    gyro2.last=dir;
    gyroDirection=dir;
}

void checkGyro(gyros *gyro)
{
    float currentGyro;                                         //gyro position
    float tempAngle;                                            //temporary angle variable
    
    currentGyro=sensor_gyro.get_value();    //read hardware gyro value
    tempAngle=currentGyro-gyro->last;            //what is the delta change in the gyro this loop?
    tempAngle=-tempAngle;
    gyro->last=currentGyro;                                //store current gyro value for comparison next time
    
    if (abs(tempAngle)>2500)                //huge delta so probably wrapped
    {
        if (tempAngle>0) {tempAngle=tempAngle-3600;}    //get true delta change taking...
        else {tempAngle=tempAngle+3600;}                //...into account wrap
    }
    //tempAngle now holds correct delta change between old and new gyro angles
    
    //        if (abs(ang2)<JITTER) {SensorValue[gyro]=lastgyro;}    //tiny delta change so overwrite hardware gyro with lastgyro (removes jitter)
    //        else
    
    //        if (abs(ang2)>JITTER)
    if (tempAngle>0)    //anti-clockwise rotation
    {
        gyro->truedir=gyro->truedir+(tempAngle*gyro->ascale);                    //update ?tempDir? if anti-clockwise rotation and scale by Anti-Clockwise scale
        if (gyro->truedir<0) {gyro->truedir=gyro->truedir+3600;}        //wrap
    }
    else
    {
        gyro->truedir=gyro->truedir+(tempAngle*gyro->cscale);                    //update ?tempDir? if clockwise rotation and scale by Clockwise scale
        if (gyro->truedir>=3600) {gyro->truedir=gyro->truedir-3600;}    //wrap
    }
    //truedir ends up as positive float >=0 and <3600 to be used in rest of code
}

void run_gyro(void* params)
{
    if (gyroinit==0)
    {
        gyroinit=1;
        
        gyro1.port=GYRO_PORT;
        gyro1.truedir=0;
        gyro1.last=sensor_gyro.get_value();
        gyro1.ascale=ASCALE;
        gyro1.cscale=CSCALE;
        
        gyro2.port=GYRO_PORT;//////SHOULD BE GYROB
        gyro2.truedir=0;
        gyro2.last=sensor_gyro.get_value();
        gyro2.ascale=ASCALE;
        gyro2.cscale=CSCALE;
        
        gyroDirection=0;
    }
    
    while(true)
    {
        checkGyro(&gyro1);
        checkGyro(&gyro2);
        
        //        gyroDirection=gyro1.truedir;
        
        //find average of the two angles
        if (gyro1.truedir>gyro2.truedir)
        {
            float tempAngle=gyro1.truedir;
            gyro1.truedir=gyro2.truedir;
            gyro2.truedir=tempAngle;            //swap order so that gyro2 always larger
        }
        if (gyro2.truedir-gyro1.truedir>1800) gyro2.truedir-=3600;    //big difference so fix wrap
        gyroDirection=(gyro2.truedir+gyro1.truedir)/2;        //average them
        if (gyroDirection<0) gyroDirection+=3600;        //unwrap negative case
        pros::delay(20);
    }
}
//////////////////////////////////////////////////////////////////////////////////
// End of gyro stuff
//

double getLeftEnc() {
    return ( drive_left_1.get_position() + drive_left_2.get_position() + drive_left_3.get_position() ) / 3;
}

double getRightEnc() {
    return ( drive_right_1.get_position() + drive_right_2.get_position() + drive_right_3.get_position() ) / 3;
}


////////////////////////////////////////////////////////////////////////////
// Drive auton functions
//
void driveStop() {
    autoTime = 0;
    autoMode = DRIVEMODE_USER;
    autoSpeed = 0;
    speedOverride = false;
    drivingToPos = false;
}

void driveTime(double s, double d, double t) {
    // speed, direction, distance, time
    autoSpeed = s;
    autoMode = DRIVEMODE_TIME;
    autoTimeOut = t*1000;
    targetDirection = d;
    recordedTime = pros::millis();
}

void driveDist(double s, double dir, double dist, double t = 10) {
    // speed, direction, distance, timeout
    autoSpeed = s;
    targetDirection = dir;
    autoMode = DRIVEMODE_DIST;
    autoTimeOut = t*1000;
    recordedTime = pros::millis();
    recordedDistLeft = getLeftEnc();
    recordedDistRight = getRightEnc();
    
    targetDistance = dist * ticksPerTile; + (recordedDistRight + recordedDistLeft)/2;
}

void driveCustom(double s, double d, double t = 10) {
    // speed, direction, timeout
    recordedTime = pros::millis();
    autoSpeed = s;
    autoMode = DRIVEMODE_CUSTOM;
    autoTimeOut = t*1000;
    targetDirection = d;
}

void turnTo(double a, double t = -1) {
    // angle, timeout
    recordedTime = pros::millis();
    targetDirection = a;
    autoTimeOut = t*1000;
    autoMode = DRIVEMODE_TURN;
    turnMode = TURNMODE_GYRO;
}

void turnRelative(double a, double t = -1) {
    // angle, timeout
    recordedTime = pros::millis();
    targetDirection = direction + a;
    autoTimeOut = t*1000;
    autoMode = DRIVEMODE_TURN;
    turnMode = TURNMODE_GYRO;
}

void turnRelativeEncoder(double a, double t = -1) {
    // angle, timeout
    recordedTime = pros::millis();
    targetDirection = direction + a;
    autoTimeOut = t*1000;
    autoMode = DRIVEMODE_TURN;
    turnMode = TURNMODE_ENCODER;
    recordedDistLeft = getLeftEnc();
    recordedDistRight = getRightEnc();
    targetDistance = (a * ticksPerDegree) + (recordedDistRight - recordedDistLeft)/2;
}


//////////////////////////////////////////////////////////////////////////////////
// Position Tracking stuff
//

void setPosition(double x, double y, double d) {
    xPosition = x;
    // lastRightEnc = getRightEnc();
    yPosition = y;
    // lastLeftEnc = getLeftEnc();
    direction = d;
}

void trackPosition() {
    double leftEnc = getLeftEnc();      // get encoder values from motors
    double rightEnc = getRightEnc();
    double leftDiff = leftEnc - lastLeftEnc;   // Find encoder changes
    double rightDiff = rightEnc- lastRightEnc;
    
    double angleChange = (rightDiff - leftDiff)/2;  // Find angle change
    angleChange *= trackingTicksPerDegree;
    
    double distChange = (leftDiff + rightDiff)/2;   // Find lin. dist change
    distChange *= trackingTicksPerTile;
    
    direction += angleChange;   // Find cumulative direction
    xPosition += distChange * cos(direction * M_PI / 180);  // find cumulative xPos
    yPosition += distChange * sin(direction * M_PI / 180);  // find cumulative yPoS
    
    lastLeftEnc = leftEnc;  // remember last values for next comparison
    lastRightEnc = rightEnc;
}

 void turnToPoint(double x, double y, double t = -1) {
    double dx = x - xPosition;
    double dy = y - yPosition;
    double dir = atan(dy/dx);
    turnTo(dir);
}
void driveTo(double s, double x, double y, double t = 10) {
    targetX = x;
    targetY = y;
    targetS = s;
    double dx = x - xPosition;
    double dy = y - yPosition;
    double dir = atan(dy/dx);
    double dist = hypot(x,y);
    driveDist(s, dir, dist, t);
    drivingToPos = true;
}

//////////////////////////////////////////////////////////////////////////////////
// Drive task
// Interprets user input & auton commands and sends to drive motors

void run_drive(void* params) {
    
    double currentTime = 0;
    double leftPower = 0;
    double rightPower = 0;
    double leftSpeed = 0;
    double rightSpeed = 0;
    
    double lastAngle = 0;
    double turnPulse = 0;
    
    double slewRate = 2;
    
    
    while (true) {
        
        if (usingGyro) {
            direction = gyroDirection;  // gyroDirection is updated by gyro code, direction is used by drive code
        }
        else {
            // maybe using compass/encoders?
            // direction = compassDirection
        }
        
        // This is where the fun begins
        
        trackPosition();        // keep track of where we are on the field        // CHANGE
        
        double forward = 0;
        double turn = 0;
        
        // Calculate useful information
        currentTime = pros::millis();           // current time to determine if timed out
        
        // find where encoders are right now
        double currentDistLeft = getLeftEnc();
        double currentDistRight = getRightEnc();
        currentDist = (currentDistRight + currentDistLeft)/2;
        
        if (controller.get_digital(BTN_ABORT)) {    // if user wants to abort, stop auton move
            autoMode = DRIVEMODE_USER;
        }
        
        // auto functions
        if (autoMode != DRIVEMODE_USER) {   // If auton is asking for drive to move
            
            if (drivingToPos) {         // keep calculating new angle & distance to stay on-target
                // Must write position tracking algorythm first
                driveTo(targetS, targetX, targetY);
            }
            
            forward = autoSpeed;        // autoSpeed is speed asked for, forward will be sent to drive motors
            
            if (autoMode == DRIVEMODE_TURN)  {  // if we are only turning, make translational speed 0
                forward = 0;
                autoSpeed = 0;
            }
            
            if (autoMode == DRIVEMODE_DIST) {   // If auto move should end with a distance
                forward *= abs(((targetDistance - currentDist)/(1*ticksPerTile) + minForward)); // slow down when within 1 tile
                if (forward > 100) forward = 100;   // Cap max and min speed
                if (forward < -100) forward = -100;
                
                // Terminate contition for distance
                if (autoSpeed > 0) {
                    if (currentDist > targetDistance) autonComplete = true;
                }
                else {
                    if (currentDist < targetDistance) autonComplete = true;
                }
            }
            
            if (currentTime > autoTimeOut && autoTimeOut > 0) {     // If auton move has timed out, stop driving
                autonComplete = true;
            }
            
            // Turn code
            double driveMag = autoSpeed;
            double seek = targetDirection;
            double angle = 0;
            
            if (turnMode == TURNMODE_GYRO) {
                angle = seek - direction;
            }
            else if (turnMode == TURNMODE_ENCODER) {
                angle = (recordedDistRight - recordedDistLeft)/2;
                angle -= (currentDistRight - currentDistLeft)/2;
                angle /= ticksPerDegree;
            }
            
            if (angle < 0) angle += 360;
            if (angle > 180) angle -= 360;
            
            angle /= (2 * turnRate);
            angle *= 127;
            if (driveMag < minSpeed) {
                angle *= 2;
            }
            
            if (angle < -maxTurn) angle = maxTurn;
            if (angle > maxTurn) angle = maxTurn;
            
            if (driveMag > minSpeed) {
                if (angle < 0) {
                    if (angle > -2) {
                        angle = 0;
                    }
                    else if (angle > -4) {
                        angle = -4;
                    }
                }
                else {
                    if (angle < 2) {
                        angle = 0;
                    }
                    else if (angle < 4) {
                        angle = 4;
                    }
                }
            }
            else {
                turn = angle;
                angle = abs(angle);
                if (angle < minSpeed) {
                    if (((lastAngle > 0) && (turn < 0)) || ((lastAngle < 0) && (turn > 0))) {
                        angle = 0;
                    }
                    else {
                        if (angle > minSpeed/5) {
                            angle = minSpeed;
                        }
                        else {
                            turnPulse++;
                            if (turnPulse < pulseTime) {
                                angle = minSpeed;
                            }
                            else {
                                angle = 1;
                                if (turnPulse > pulsePause) {
                                    turnPulse = 0;
                                }
                            }
                        }
                    }
                }
                if (turn < 0) angle *= -1;
            }
            
            turn = angle;
            
            if (autoSpeed == 0 || autoMode == DRIVEMODE_TURN) {
                if (abs(angle) < turnAccepted && abs(lastAngle) < turnAccepted) {
                    autonComplete = true;
                }
            }
            
            lastAngle = angle;
            
        }
        // Auto-move is complete, so stop moving
        if (autonComplete) {
            autoMode = DRIVEMODE_USER;
            forward = 0;
            turn = 0;
            autoSpeed = 0;
            drivingToPos = false;
        }
        
        // User controls
        if (autoMode == DRIVEMODE_USER) {
            
            // Tank controls
            leftSpeed = controller.get_analog(ANALOG_LEFT_Y);
            rightSpeed = controller.get_analog(ANALOG_RIGHT_Y);
            
            if (leftSpeed*leftSpeed < deadZone*deadZone) leftSpeed = 0;
            if (rightSpeed*rightSpeed < deadZone*deadZone) rightSpeed = 0;
        }
        else {
            leftSpeed = forward - turn;
            rightSpeed = forward + turn;
        }
        
        // Constant-speed override
        if (speedOverride) {
            leftSpeed = leftRunSpeed;
            rightSpeed = rightRunSpeed;
        }
        
        // dampen motors so they don't spike current
        rightPower = rightPower + ( (rightSpeed - rightPower) / slewRate );
        leftPower = leftPower + ( (leftSpeed - leftPower) / slewRate );
        
        
        // Send speeds to drive motors
        drive_left_1.move_voltage(leftPower*12000);
        drive_left_2.move_voltage(leftPower*12000);
        drive_left_3.move_voltage(leftPower*12000);
        drive_right_1.move_voltage(rightPower*12000);
        drive_right_2.move_voltage(rightPower*12000);
        drive_right_3.move_voltage(rightPower*12000);
        
        pros::delay(20);   // don't hog cpu
    }
}

void run_flywheel(void* params) {
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
        double targetSpeed = flyWheelDefaultSpeed;
        
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
            targetSpeed = flyWheelDefaultSpeed;
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
                    if (targetFlag == 1) targetSpeed = flyWheelSpeeds[flyWheelSpeedsDefinition-1][1];
                    else targetSpeed = flyWheelSpeeds[flyWheelSpeedsDefinition-1][2];
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
            //flyWheel.runAtSpeed(targetSpeed);                             CHANGE
            
            // Read vision sensor & ask drive to turn appropriately
            //drive.turnRelative(relativeAngle,-1);                         CHANGE
            
            bool fireBall = false;
            // Check current speed of flywheel & if aimed
            if ((/*flyWheel.isWithinRange()*/1 && abs(relativeAngle) < FLYWHEEL_AIM_RANGE)) {
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
        
        
        // Math for the flywheel
        double flywheelCurrSpeed = 0;
        double flywheelSpeed = 0;
        flywheelCurrSpeed = ( flywheel_1.get_actual_velocity() + flywheel_2.get_actual_velocity() ) / 2;
        
        if (flywheelCurrSpeed > targetSpeed) {  // Too fast
            flywheelSpeed = flywheelSlowSpeed;  // So run slow
        }
        if (flywheelCurrSpeed <= targetSpeed) { // Too slow
            flywheelSpeed = flywheelFastSpeed;  // So run fast
        }
        
        // Set motors on flywheel
        flywheel_1.move_voltage(flywheelSpeed * 12000);
        flywheel_2.move_voltage(flywheelSpeed * 12000);
        
        // Send speeds to intake motors
        intake_in.move_voltage(intakeSpeedInner*12000);
        intake_out.move_voltage(intakeSpeedOuter*12000);
        
        // Remember ball info for fireing
        ballWasIn = ballIsIn;
        
        
        pros::delay(20);   // don't hog cpu
    }
}

void run_arm(void* params) {
    bool justFlipped = false;
    
    while (true) {
        
        double armSpeed = 0;            // Start with zero speeds
        double wristSpeed = 0;
        double flipperSpeed = 0;
        
        flipperPos = flip.get_position();       // Find current positions
        wristPos = wrist.get_position();
        armPos = (arm_1.get_position() + arm_2.get_position()) / 2;
        
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
            if (controller.get_digital(BTN_FLIP)) {     // Auto flip (180°)
                stackStep = -1;
                if (!justFlipped) {
                    if (flipperPos > (FLIP_POS1 + FLIP_POS2)/2) {
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
            if (controller.get_digital(BTN_WRIST_BACK)) {       // Pos for backwards reach
                stackStep = -1;
                wristSeek = WRIST_BACK_POS;
            }
            if (controller.get_digital(BTN_WRIST_FORWARD)) {    // Pos for collecting
                stackStep = -1;
                wristSeek = WRIST_FORWARD_POS;
            }
            if (controller.get_digital(BTN_WRIST_VERTICAL)) {   // Pos for holding
                stackStep = -1;
                wristSeek = WRIST_VERTICAL_POS;
            }
            if (controller.get_digital(BTN_ARM_HIGH)) {         // Pos for high stack
                stackStep = -1;
                armSeek = ARM_POS_HIGH;
            }
            if (controller.get_digital(BTN_ARM_LOW)) {          // Pos for low stack
                stackStep = -1;
                armSeek = ARM_POS_LOW;
            }
            if (controller.get_digital(BTN_ARM_DOWN)) {         // Pos for collecting
                stackStep = -1;
                armSeek = ARM_POS_DOWN;
            }
        }
        if (controller.get_digital(BTN_ABORT)) {                // Stop all auton functions!
            wristSeek = -1;
            armSeek = -1;
            flipperSeek = -1;
            stackStep = -1;
        }
        
        
        // If we need to seek, then tell the arm, wrist, and flipper (lerp code)
        if (armSeek > 0) {
            armSpeed = (armSeek - armPos) / armSeekRate;
            if (armSpeed > 100) armSpeed = 100;
            if (armSpeed < -100) armSpeed = -100;
        }
        if (wristSeek > 0) {
            wristSpeed = (wristSeek - wristPos) / wristSeekRate;
            if (wristSpeed > 100) wristSpeed = 100;
            if (wristSpeed < -100) wristSpeed = -100;
        }
        if (flipperSeek > 0) {
            flipperSpeed = (flipperSeek - flipperPos) / flipperSeekRate;
            if (flipperSpeed > 100) flipperSpeed = 100;
            if (flipperSpeed < -100) flipperSpeed = -100;
        }
        
        
        // Manual overrides
        if (controller.get_digital(BTN_ARM_MAN_DOWN)) {
            armSpeed = -100;
            armSeek = -1;
            stackStep = -1;
        }
        if (controller.get_digital(BTN_ARM_MAN_UP)) {
            armSpeed = 100;
            armSeek = -1;
            stackStep = -1;
        }
        if (controller.get_digital(BTN_WRIST_MAN_DOWN)) {
            wristSpeed = -100;
            wristSeek = -1;
            stackStep = -1;
        }
        if (controller.get_digital(BTN_WRIST_MAN_UP)) {
            wristSpeed = 100;
            armSeek = -1;
            stackStep = -1;
        }
        if (controller.get_digital(BTN_FLIPPER_MAN_DOWN)) {
            flipperSpeed = -100;
            flipperSeek = -1;
            stackStep = -1;
        }
        if (controller.get_digital(BTN_FLIPPER_MAN_UP)) {
            flipperSpeed = 100;
            flipperSeek = -1;
            stackStep = -1;
        }
        
        // Finally, send values to motors
        flip.move_voltage(flipperSpeed * 12000);
        wrist.move_voltage(wristSpeed * 12000);
        arm_1.move_voltage(armSpeed * 12000);
        arm_2.move_voltage(armSpeed * 12000);
        
        pros::delay(20);   // don't hog cpu
    }
}


void run_auton() {
    
    initAll();
    
    // Start task
    pros::Task flywheelTask (run_flywheel);
    pros::Task armTask (run_arm);
    pros::Task driveTask (run_drive);
    pros::Task gyroTask (run_gyro);
    
    bool nextCommand = true;
    int driveMode = 0;
    double pauseTime = 0;
    // Set pointer to chosen auton routine
    if (autonSelect == 1) autonCommand = &auton1[0];
    
    // First entry is always starting direction,
    setGyro(*autonCommand);
    //drive.setDirection(*autonCommand);
    direction = *autonCommand;
    
    
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
                            driveDist(ds,dd,processEntry(),processEntry());
                        }
                        else {
                            driveMode = dt;
                            driveCustom(ds,dd,processEntry());
                        }
                    }
                    else {
                        driveMode = 0;
                        driveTime(ds,dd,dt);
                    }
                    break;
                case TURN:
                    turnTo(processEntry(),processEntry());
                    break;
                case TURN_REL:
                    turnRelative(processEntry(),processEntry());
                    break;
                case TURN_ENC:
                    turnRelativeEncoder(processEntry(),processEntry());
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
                    if (flipperPos > (FLIP_POS1 + FLIP_POS2)/2) {
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
            driveStop();
            nextCommand = true;
        }
        
        
        pros::delay(20);   // let other tasks use cpu
    }
}



/**
 * Runs the operator control code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the operator
 * control mode.
 *
 * If no competition control is connected, this function will run immediately
 * following initialize().
 *
 * If the robot is disabled or communications is lost, the
 * operator control task will be stopped. Re-enabling the robot will restart the
 * task, not resume it from where it left off.
 */
void opcontrol() {
    
    // Start task
    pros::Task flywheelTask (run_flywheel);
    pros::Task armTask (run_arm);
    pros::Task driveTask (run_drive);
    pros::Task gyroTask (run_gyro);
    
    // Start runArm, runDrive, runFlywheel as tasks
    
    while (true) {
        pros::lcd::print(0, "%d %d %d", (pros::lcd::read_buttons() & LCD_BTN_LEFT) >> 2,
                         (pros::lcd::read_buttons() & LCD_BTN_CENTER) >> 1,
                         (pros::lcd::read_buttons() & LCD_BTN_RIGHT) >> 0);
        
        pros::delay(20);
    }
}
