//
//  posmech.h
//  
//
//  Created by Sam Burton on 03/11/2018.
//

#ifndef __POSMECH__
#define __POSMECH__

#include <stdio.h>
#include <vector>


class PositionMachine {
private:
    double position, targetPosition, speed, scale, upLim, lowLim, range, runSpeed, positionOffset;
    double temperature, power, current;
    bool seeking, manOverride, speedOverride, holding;
    pros::controller_digital_e_t posButton, negButton;
    double posManualSpeed, negManualSpeed, posSeekRate, negSeekRate;
    pros::Controller* controller;
    std::vector<pros::Motor*> motors;
    
public:
    
    // CONSTRUCTOR
    PositionMachine();  // constructor
    ~PositionMachine(); // destructor
    
    // CONFIG
    void setController(pros::Controller*);    // tell machine what the controller is
    void addMotor(pros::Motor*); // add a motor to the machine
    void setPosition(double);   // set position
    void setLimits(double, double); // set location of mechanical limits
    void setRange(double);  // set range where seek is 'close enough'
    void setScale(double);  // set scale of motor ticks per position unit
    void setManualPos(pros::controller_digital_e_t, double);    // set manual button and rate up
    void setManualNeg(pros::controller_digital_e_t, double);    // set manual rate and button down
    void setSeekRate(double);   // set seek rate of auto-seek
    void setSeekRates(double, double);  // set seek rates for seek up/down
    
    // GETTERS
    void calcPosition();    // calculate current pos
    double getPosition();   // return current pos
    double getSpeed();  // return current speed
    double getTemperature();    // return average temp
    double getPower();  // return average power
    double getCurrent();    // return average current
    int getNumMotors(); // returns number of motors
    
    // CONTROL FUNCTIONS
    void disableSeek(); // turn seeking off
    void enableSeek();  // turn seeking on
    void enableManual();    // enable manual overrides
    void disableManual();   // disable manual overrides
    void enableHolding();   // enable holding
    void disableHolding();  // disable holding
    void disableSpeed();    // stop running at a speed
    void enableSpeed(); // enable run at speed
    
    // AUTON FUNCTIONS
    void stop();    // stop all autonomous controlls
    void runAtSpeed(double);    // start running at a speed, disables seeking
    void hold();    // hold at current position
    void hold(double);  // hold at target position
    void seek(double);  // seek to target position (disables holding)
    
    // CALL EVERY LOOP
    void run(); // runs the machine, call every loop
};

#endif
