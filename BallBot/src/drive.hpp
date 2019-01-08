//
//  drive.h
//  
//
//  Created by Sam Burton on 03/11/2018.
//

#ifndef __DRIVE__
#define __DRIVE__


#include <stdio.h>
#include <vector>
#include <cmath>

class Drive {
private:
    pros::Controller* controller;
    std::vector<pros::Motor*> motorsLeft, motorsRight;
    pros::controller_analog_e_t leftSideJoy, rightSideJoy, powerJoy, turnJoy;
    bool arcadeMode;
    double deadZone, ticksPerTile, ticksPerDegree, slewRate, rightPower, leftPower;
    double temperature, power, current;
    double direction, targetDirection;
    double distance, targetDistance;
    double autoSpeed, autoTime, autoMode, autoTimeOut;
    double turnAccepted, turnRate, turnPulse, pulseTime, pulsePause, minSpeed, maxTurn;
    bool autonComplete;
    double currentTime, recordedTime;
    double leftSpeed, rightSpeed, leftRunSpeed, rightRunSpeed, recordedDistLeft, recordedDistRight, minForward, lastAngle, currentDist;
    bool speedOverride, controlsSet;
    int turnMode;
    
    double xPosition, yPosition, lastDirection, lastRightEnc, lastLeftEnc, trackingTicksPerTile, trackingTicksPerDegree;
    double targetX, targetY, targetS;
    bool drivingToPos;
    
public:
    // CONSTRUCTOR
    Drive();    // constructor
    ~Drive();   // destructor
    
    
    // CONFIG
    void setController(pros::Controller*);  // sets controller
    void addMotorLeft(pros::Motor*); // adds motor to left side
    void addMotorRight(pros::Motor*);    // adds motor to right side
    
    
    // DRIVER PREFERENCES
    void setSlewRate(double);       // sets slew rate of drive motors
    void setDeadZone(double);   // enables and sets deadzone on joysticks
    void setTankJoy(pros::controller_analog_e_t,pros::controller_analog_e_t); // sets tank controls
    void setArcadeJoy(pros::controller_analog_e_t,pros::controller_analog_e_t);   // sets arcade controls
    
    
    // POSITION TRACKING
    void setPosition(double, double);   // sets the position of the drive
    void setPosition(double, double, double);   // sets the position & direction of the drive
    void setDirection(double);  // 'tells' robot its current direction
    void setTrackingTicksPerTile(double);       // set converts for tracking
    void setTrackingTicksPerDegree(double);
    void trackPosition();    // tracks position of robot

    
    // STATE GETTERS
    double getDirection();  // gets direction of the robot
    double getRightEnc();   // gets average right encoder values
    double getLeftEnc();    // gets average left encoder values
    double getTemperature();    // gets avg. temp of motors
    double getPower();  // gets avg. power draw
    double getCurrent();    // gets avg. current draw
    
    
    // SETTERS FOR AUTON TUNING
    void setTicksPerTile(double);   // sets encoder ticks per field tile
    void setTicksPerDegree(double); // sets encoder ticks per degree
    void setMinForwardSpeed(double);    // sets min motor power for robot to move
    void setTimeOut(double);    // sets auton timeout
    void setTurnMode(int);  // sets turning mode, gyro or encoders
    void setTurnAccepted(double);   // value where turn is 'close-enough'
    void setTurnRate(double);   // sets turn rate
    void setPulseTime(double);  // sets turn pulse time
    void setPulsePause(double); // sets turn pause time
    void setMinTurnSpeed(double);   // sets turn min-speed
    void setMaxTurnSpeed(double);   // sets turn max-speed
    

    // AUTON FUNCTIONS
    void driveTime(double, double, double); // auto drive for time : speed, direction, distance, time
    void driveDist(double, double, double, double); // auto drive for distance : speed, direction, distance, timeout
    void driveCustom(double, double, double);   // auto drive until terminated : speed, direction, distance, timeout
    void turnTo(double, double);    // auto turn to angle : angle, timeout
    void turnRelative(double, double);  // auto turn relative : angle, timeout
    void turnRelativeEncoder(double, double); // auto turn relative with encoders
    void runAtSpeed(double);    // constant speed override straight line
    void runAtSpeeds(double, double);   // constant speed override turn/curve
    void turnToPoint(double, double, double);            // turn to face point on field
    void driveTo(double, double, double, double);   // drive to point on field
    void stop();    // stop any auto movement
    
    
    // CALL EVERY LOOP
    void run(); // runs the drive
    
};



#endif
