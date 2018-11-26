//
//  speedmech.cpp
//  
//
//  Created by Sam Burton on 03/11/2018.
//

#include "uscvex.h"
#include <cmath>



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

void SpeedMachine::addMotor(vex::motor*) {
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
        tot += motors[i]->temperature(vex::percentUnits::pct);
    }
    temperature = tot/motors.size();
    return temperature;
}
double SpeedMachine::getPower() {
    double tot = 0;
    for (int i = 0; i < motors.size(); i++) {
        tot += motors[i]->power(vex::powerUnits::watt);
    }
    power = tot/motors.size();
    return power;
}
double SpeedMachine::getCurrent() {
    double tot = 0;
    for (int i = 0; i < motors.size(); i++) {
        tot += motors[i]->current(vex::currentUnits::amp);
    }
    current = tot/motors.size();
    return current;
}
int SpeedMachine::getNumMotors() {
    return motors.size();
}

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
        motors[i]->resetRotation();
    }
}
void SpeedMachine::calcSpeed() {
    double tot = 0;
    for (int i = 0; i < motors.size(); i++) {
        tot += motors[i]->velocity(vex::velocityUnits::pct);
    }
    tot *= scale;
    speed = tot;
}
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

void SpeedMachine::run() {
    
    double power = 0;
    calcSpeed();
    calcDifference();
    
    // Bang-Bang speed control
    if (speed < targetSpeed) {
        power = highPow;
    }
    if (speed > targetSpeed) {
        power = lowPow;
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
        motors[i]->spin(vex::directionType::fwd, power, vex::velocityUnits::pct);
    }
    
}
