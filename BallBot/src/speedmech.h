//
//  speedmech.h
//  
//
//  Created by Sam Burton on 03/11/2018.
//

#ifndef __SPEEDMECH__
#define __SPEEDMECH__

#include "main.h"
#include <stdio.h>
#include <vector>


class SpeedMachine {
private:
    double speed, targetSpeed, difference, scale, highPow, lowPow, range, runSpeed;
    double temperature, power, current;
    bool running, speedOverride, withinRange;
    std::vector<pros::Motor* > motors;
    
public:
    
    // CONSTRUCTOR
    SpeedMachine();     // constructor
    ~SpeedMachine();    // destructor
    
    // CONFIG
    void addMotor(pros::Motor*); // add a motor to the machine
    void setPowers(double, double); // set power levels for 'Bang-Bang'
    void setRange(double);  // set range
    void setScale(double);  // set scale
    
    // GETTERS
    void calcDifference();  // find difference between speed and target
    double getSpeed();  // return current speed
    double getTemperature();    // return average temp
    double getPower();  // return average power
    double getCurrent();    // return average current
    int getNumMotors(); // returns number of motors
    
    // CONTROL
    void enable();  // turn on
    void disable(); // turn off
    void stop();    // stop all autonomous controlls
    void calcWithinRange(); // calculates if within range
    bool isWithinRange();   // is the machine within the range of the target speed
    void resetEncoders();   // resets the motor encoders to zero
    void calcSpeed();   // calculates speed of motors
    
    // AUTON FUNCTIONS
    void runAtSpeed(double);    // sets target speed
    void runAtPower(double);    // runs at specific power
    
    // CALL EVERY LOOP
    void run(); // runs the machine
};

#endif
