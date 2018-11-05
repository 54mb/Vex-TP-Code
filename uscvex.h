//
//  VEXUSC.h
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
    PositionMachine();
        // constructor
    ~PositionMachine();
        // destructor
    
    void addMotor(vex::motor*);
        // add a motor to the machine
    
    void setPosition(double);
        // set position
    void setLimits(double, double);
        // set location of mechanical limits
    void setRange(double);
        // set range where seek is 'close enough'
    void setScale(double);
        // set scale of motor ticks per position unit
    void setManualPos(vex::controller::button*, double);
        // set manual button and rate up
    void setManualNeg(vex::controller::button*, double);
        // set manual rate and button down
    void setSeekRate(double);
        // set seek rate of auto-seek
    void setSeekRates(double, double);
        // set seek rates for seek up/down
    
    void calcPosition();
        // calculate current pos
    double getPosition();
        // return current pos
    double getSpeed();
        // return current speed
    double getTemperature();
        // return average temp
    double getPower();
        // return average power
    double getCurrent();
        // return average current
    int getNumMotors();
        // returns number of motors
    
    void disableSeek();
        // turn seeking off
    void enableSeek();
        // turn seeking on
    void enableManual();
        // enable manual overrides
    void disableManual();
        // disable manual overrides
    void enableHolding();
        // enable holding
    void disableHolding();
        // disable holding
    void disableSpeed();
        // stop running at a speed
    void enableSpeed();
        // enable run at speed
    
    void stop();
        // stop all autonomous controlls
    void runAtSpeed(double);
        // start running at a speed, disables seeking
    void hold();
        // hold at current position
    void hold(double);
        // hold at target position
    void seek(double);
        // seek to target position (disables holding)
    
    void run();
        // runs the machine, call every loop
};


class SpeedMachine {
private:
    double speed, targetSpeed, difference, scale, highPow, lowPow, range, runSpeed;
    double temperature, power, current;
    bool running, speedOverride, withinRange;
    std::vector<vex::motor*> motors;
    
public:
    SpeedMachine();
        // constructor
    ~SpeedMachine();
        // destructor
    
    void addMotor(vex::motor*);
        // add a motor to the machine
    
    void setPowers(double, double);
        // set power levels for 'Bang-Bang'
    void setRange(double);
        // set range
    void setScale(double);
        // set scale
    void calcDifference();
        // find difference between speed and target
    
    double getSpeed();
        // return current speed
    double getTemperature();
        // return average temp
    double getPower();
        // return average power
    double getCurrent();
        // return average current
    int getNumMotors();
        // returns number of motors
    
    void enable();
        // turn on
    void disable();
        // turn off
    void stop();
        // stop all autonomous controlls
    bool isWithinRange();
        // is the machine within the range of the target speed
    
    void resetEncoders();
        // resets the motor encoders to zero
    void calcSpeed();
        // calculates speed of motors
    void runAtSpeed(double);
        // sets target speed
    void runAtPower(double);
        // runs at specific power
    
    void run();
        // runs the machine
};



class Drive {
private:
    vex::brain* driveBrain;
    vector<vex::motor*> leftMotors, rightMotors;
    vex::controller::axis *leftSideJoy, *rightSideJoy, *powerJoy, *turnJoy;
    bool arcadeMode;
    double deadZone, ticksPerTile, ticksPerDegree;
    double temperature, power, current;
    double direction, targetDirection;
    double distance, targetDistance;
    double autoSpeed, autoTime, autoMode, autoTimeOut;
    double turnAccepted, turnRate, pulseTime, pulsePause, minSpeed, maxTurn;
    bool autonComplete;
    double currentTime, recordedTime;
    double leftSpeed, rightSpeed, leftRunSpeed, rightRunSpeed;
    bool speedOverride;
public:
    Drive();
    ~Drive();
    
    void setBrain(vex::brain*);
    
    void addMotorLeft(vex::motor*);
    void addMotorRight(vex::motor*);
    
    void setTankJoy(vex::controller::axis*,vex::controller::axis*);
    void setArcadeJoy(vex::controller::axis*,vex::controller::axis*);
    
    void setTicksPerTile(double);
    void setTicksPerDegree(double);
    void setDeadZone(double);
    void setDirection(double);
    void setTimeOut(double);
    
    void setTurnAccepted(double);
    void setTurnRate(double);
    void setPulseTime(double);
    void setPulsePause(double);
    void setMinSpeed(double);
    void setMaxTurn(double);
    
    double getTemperature();
    double getPower();
    double getCurrent();
    
    void finishMove();
    
    void driveTime(double, double, double);
        // speed, direction, distance, time
    void driveDist(double, double, double, double);
        // speed, direction, distance, timeout
    void driveCustom(double, double, double);
        // speed, direction, distance, timeout
    void turnTo(double, double);
        // angle, timeout
    void turnRelative(double, double);
        // angle, timeout
    void runAtSpeed(double);
    void runAtSpeeds(double, double);
    
    void stop();
    void run();
};



