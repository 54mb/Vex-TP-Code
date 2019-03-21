//
//  speedmech.cpp
//  
//
//  Created by Sam Burton on 03/11/2018.
//

#include "speedmech.hpp"


// CONSTRUCTOR

SpeedMachine::SpeedMachine() {
    speed = 0;
    targetSpeed = 0;
    scale = 1;
    highPow = 0;
    lowPow = 0;
    range = 10;
    temperature = 0;
    power = 0;
    current = 0;
    running = false;
    speedOverride = false;
    runSpeed = 0;
    withinRange = false;
}
SpeedMachine::~SpeedMachine() {
    
}


// CONFIG

void SpeedMachine::addMotor(pros::Motor* newMotor) {
    motors.push_back(newMotor);
}
void SpeedMachine::setPowers(double l, double h) {
    lowPow = l;
    highPow = h;
}
void SpeedMachine::setRange(double r) {
    range = r;
}
void SpeedMachine::setScale(double s) {
    scale = s;
}

// GETTERS
void SpeedMachine::calcDifference() {
    calcSpeed();
    difference = targetSpeed - speed;
}
double SpeedMachine::getSpeed() {
    calcSpeed();
    return speed;
}
double SpeedMachine::getTemperature() {
    double tot = 0;
    for (int i = 0; i < motors.size(); i++) {
        tot += motors[i]->get_temperature();
    }
    temperature = tot/motors.size();
    return temperature;
}
double SpeedMachine::getPower() {
    double tot = 0;
    for (int i = 0; i < motors.size(); i++) {
        tot += motors[i]->get_power();
    }
    power = tot/motors.size();
    return power;
}
double SpeedMachine::getCurrent() {
    double tot = 0;
    for (int i = 0; i < motors.size(); i++) {
        tot += motors[i]->get_current_draw();
    }
    current = tot/motors.size();
    return current;
}
int SpeedMachine::getNumMotors() {
    return motors.size();
}


// CONTROL

void SpeedMachine::enable() {
    running = true;
}
void SpeedMachine::disable() {
    running = false;
}
void SpeedMachine::stop() {
    running = false;
    speedOverride = false;
    targetSpeed = 0;
    runSpeed = 0;
}
void SpeedMachine::calcWithinRange() {
    if (difference*difference < range*range) {
        withinRange = true;
    }
    withinRange = false;
}
bool SpeedMachine::isWithinRange() {    // checks if the speed we're at is close enough to the target
    calcWithinRange();
    return withinRange;
}
void SpeedMachine::resetEncoders() {
    for (int i = 0; i < motors.size(); i++) {
        motors[i]->tare_position();
    }
}
void SpeedMachine::calcSpeed() {
    double tot = 0;
    for (int i = 0; i < motors.size(); i++) {
        tot += motors[i]->get_actual_velocity();
    }
    tot *= scale;
    speed = tot;
}


// AUTON FUNCTIONS

void SpeedMachine::runAtSpeed(double s) {
    targetSpeed = s;
    speedOverride = false;
    running = true;
}
void SpeedMachine::runAtPower(double p) {
    running = true;
    runSpeed = p;
    speedOverride = true;
}


// CALL EVERY LOOP

void SpeedMachine::run() {
    
    double power = 0;
    calcDifference();
    
    // Bang-Bang speed control
    if (difference < 0) {
        power = highPow;
    }
    else if (difference > 0) {
        power = lowPow;
    }
    else {
        power = (highPow + lowPow)/2;
    }
    
    // Power override
    if (speedOverride) {
        power = runSpeed;
    }
    
    // Enabled/Disabled
    if (!running) {
        power = 0;
    }
    
    // run the motors
    for (int i = 0; i < motors.size(); i++) {
        motors[i]->move_voltage(power*12000);
    }
    
}
