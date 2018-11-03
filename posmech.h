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

// Bear Min Setup:
/*
 PositionMachine pM = PositionMachine();
 pM.addMotor(m);
 // following lines will let manual controlls work
 pM.setManualPos(b,r);
 pM.setManualNeg(b,r);
 // following will let seek/hold work
 pM.setSeekRate(r);
 pM.setScale(s);
 // rest is optional to suit particular setup
 */
