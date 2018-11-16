//
//  uscvex.h
//  
//
//  Created by Sam Burton on 03/11/2018.
//

#include <stdio.h>
#include <vex>
#include <vector>


class PositionMachine {
private:
    double position, targetPosition, speed, scale, upLim, lowLim, range, runSpeed, positionOffset;
    double temperature, power, current;
    bool seeking, manOverride, speedOverride, holding;
    vex::controller::button *posButton, *negButton;
    double posManualSpeed, negManualSpeed, posSeekRate, negSeekRate;
    std::vector<vex::motor*> motors;
    
public:
    PositionMachine();  // constructor
    ~PositionMachine(); // destructor
    
    void addMotor(vex::motor*); // add a motor to the machine
    
    void setPosition(double);   // set position
    void setLimits(double, double); // set location of mechanical limits
    void setRange(double);  // set range where seek is 'close enough'
    void setScale(double);  // set scale of motor ticks per position unit
    void setManualPos(vex::controller::button*, double);    // set manual button and rate up
    void setManualNeg(vex::controller::button*, double);    // set manual rate and button down
    void setSeekRate(double);   // set seek rate of auto-seek
    void setSeekRates(double, double);  // set seek rates for seek up/down
    
    void calcPosition();    // calculate current pos
    double getPosition();   // return current pos
    double getSpeed();  // return current speed
    double getTemperature();    // return average temp
    double getPower();  // return average power
    double getCurrent();    // return average current
    int getNumMotors(); // returns number of motors
    
    void disableSeek(); // turn seeking off
    void enableSeek();  // turn seeking on
    void enableManual();    // enable manual overrides
    void disableManual();   // disable manual overrides
    void enableHolding();   // enable holding
    void disableHolding();  // disable holding
    void disableSpeed();    // stop running at a speed
    void enableSpeed(); // enable run at speed
    
    void stop();    // stop all autonomous controlls
    void runAtSpeed(double);    // start running at a speed, disables seeking
    void hold();    // hold at current position
    void hold(double);  // hold at target position
    void seek(double);  // seek to target position (disables holding)
    
    void run(); // runs the machine, call every loop
};


class SpeedMachine {
private:
    double speed, targetSpeed, difference, scale, highPow, lowPow, range, runSpeed;
    double temperature, power, current;
    bool running, speedOverride, withinRange;
    std::vector<vex::motor*> motors;
    
public:
    SpeedMachine();     // constructor
    ~SpeedMachine();    // destructor
    
    void addMotor(vex::motor*); // add a motor to the machine
    
    void setPowers(double, double); // set power levels for 'Bang-Bang'
    void setRange(double);  // set range
    void setScale(double);  // set scale
    void calcDifference();  // find difference between speed and target
    
    double getSpeed();  // return current speed
    double getTemperature();    // return average temp
    double getPower();  // return average power
    double getCurrent();    // return average current
    int getNumMotors(); // returns number of motors
    
    void enable();  // turn on
    void disable(); // turn off
    void stop();    // stop all autonomous controlls
    bool isWithinRange();   // is the machine within the range of the target speed
    
    void resetEncoders();   // resets the motor encoders to zero
    void calcSpeed();   // calculates speed of motors
    void runAtSpeed(double);    // sets target speed
    void runAtPower(double);    // runs at specific power
    
    void run(); // runs the machine
};



class Drive {
private:
    vex::brain* driveBrain;
    vector<vex::motor*> leftMotors, rightMotors;
    vex::controller::axis *leftSideJoy, *rightSideJoy, *powerJoy, *turnJoy;
    bool arcadeMode;
    double deadZone, ticksPerTile, ticksPerDegree, slewRate, rightPower, leftPower;
    double temperature, power, current;
    double direction, targetDirection;
    double distance, targetDistance;
    double autoSpeed, autoTime, autoMode, autoTimeOut;
    double turnAccepted, turnRate, turnPulse, pulseTime, pulsePause, minSpeed, maxTurn;
    bool autonComplete;
    double currentTime, recordedTime;
    double leftSpeed, rightSpeed, leftRunSpeed, rightRunSpeed, recordedDistLeft, recordedDistRight, minForward, lastAngle;
    bool speedOverride, controlsSet;
    int turnMode;
public:
    Drive();    // constructor
    ~Drive();   // destructor
    
    void setBrain(vex::brain*); // sets brain (for timers)
    
    void addMotorLeft(vex::motor*); // adds motor to left side
    void addMotorRight(vex::motor*);    // adds motor to right side
    
    void setTankJoy(vex::controller::axis*,vex::controller::axis*); // sets tank controls
    void setArcadeJoy(vex::controller::axis*,vex::controller::axis*);   // sets arcade controls
    
    void setSlewRate(double);
    
    void setTicksPerTile(double);   // sets encoder ticks per field tile
    void setTicksPerDegree(double); // sets encoder ticks per degree
    void setDeadZone(double);   // enables and sets deadzone on joysticks
    void setMinForwardSpeed(double);    // sets min motor power for robot to move
    void setDirection(double);  // 'tells' robot its current direction
    void setTimeOut(double);    // sets auton timeout
    void setTurnMode(int);  // sets turning mode
    
    void setTurnAccepted(double);   // value where turn is 'close-enough'
    void setTurnRate(double);   // sets turn rate
    void setPulseTime(double);  // sets turn pulse time
    void setPulsePause(double); // sets turn pause time
    void setMinTurnSpeed(double);   // sets turn min-speed
    void setMaxTurnSpeed(double);   // sets turn max-speed
    
    double getTemperature();    // gets avg. temp of motors
    double getPower();  // gets avg. power draw
    double getCurrent();    // gets avg. current draw
    double getDirection();  // gets direction of the robot
    
    void finishMove();  // stops any auton function
    
    void driveTime(double, double, double); // auto drive for time : speed, direction, distance, time
    void driveDist(double, double, double, double); // auto drive for distance : speed, direction, distance, timeout
    void driveCustom(double, double, double);   // auto drive until terminated : speed, direction, distance, timeout
    void turnTo(double, double);    // auto turn to angle : angle, timeout
    void turnRelative(double, double);  // auto turn relative : angle, timeout
    void turnRelativeEncoder(double, double) // auto turn relative with encoders
    void runAtSpeed(double);    // constant speed override straight line
    void runAtSpeeds(double, double);   // constant speed override turn/curve
    
    void stop();    // stop any auto movement
    void run(); // runs the drive
};



