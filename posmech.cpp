//
//  VEXUSC.cpp
//  
//
//  Created by Sam Burton on 03/11/2018.
//

#include "posmech.h"

PositionMachine::PositionMachine() {
    position = 0;
    targetPosition = 0;
    speed = 0;
    scale = 1;
    upLim = 0;
    lowLim = 0;
    range = 0;
    runSpeed = 0;
    positionOffset = 0
    temperature = 0;
    power = 0;
    current = 0;
    seeking = false;
    manOverride = true;
    speedOverride = false;
    holding = false;
    posManualSpeed = 100;
    negManualSpeed = -100;
    posSeekRate = 1;
    negSeekRate = -1;
}
PositionMachine::~PositionMachine() {
    
}

void PositionMachine::addMotor(vex::motor* newMotor) {
    motors.push_back(newMotor);
}

void PositionMachine::setPosition(double p) {
    // set all motor encoders to 0
    for (int i = 0; i < motors.size(); i++) {
        motors[i]->resetRotation();
    }
    // set position
    positionOffset = p;
    position = p;
}
void PositionMachine::setLimits(double l, double u) {
    lowLim = l;
    upLim = u;
}
void PositionMachine::setRange(double s) {
    range = s;
}
void PositionMachine::setScale(double s) {
    scale = s;
}
void PositionMachine::setManualPos(vex::controller::button* b, double s) {
    posButton = b;
    posManualSpeed = s;
}
void PositionMachine::setManualNeg(vex::controller::button* b, double s) {
    negButton = b;
    negManualSpeed = s;
}
void PositionMachine::setSeekRate(double r) {
    negSeekRate = r;
    posSeekRate = r;
}
void PositionMachine::setSeekRates(double dr, double ur) {
    negSeekRate = dr;
    posSeekRate = ur;
}
void PositionMachine::calcPosition() {
    double tot = 0;
    for (int i = 0; i < motors.size(); i++) {
        tot += motors[i]->rotation(vex::rotationUnits::deg);
    }
    tot /= motors.size();
    tot *= scale;
    tot += positionOffset;
    if (lowLim != upLim) {
        if (tot < lowLim) {
            positionOffset += lowLim - tot;
            tot = lowLim;
        }
        if (tot < upLim) {
            positionOffset -= tot - upLim;
            tot = upLim;
        }
    }
    position = tot;
}
double PositionMachine::getPosition() {
    calcPosition();
    return position;
}
double PositionMachine::getSpeed() {
    return speed;
}
double PositionMachine::getTemperature() {
    double tot = 0;
    for (int i = 0; i < motors.size(); i++) {
        tot += motors[i]->temperature(vex::percentUnits::pct);
    }
    temperature = tot/motors.size();
    return temperature;
}
double PositionMachine::getPower() {
    double tot = 0;
    for (int i = 0; i < motors.size(); i++) {
        tot += motors[i]->power(vex::powerUnits::watt);
    }
    power = tot/motors.size();
    return power;
}
double PositionMachine::getCurrent() {
    double tot = 0;
    for (int i = 0; i < motors.size(); i++) {
        tot += motors[i]->current(vex::currentUnits::amp);
    }
    current = tot/motors.size();
    return current;
}


void PositionMachine::disableSeek() {
    seeking = false;
}
void PositionMachine::enableSeek() {
    seeking = true;
}
void PositionMachine::enableManual() {
    manOverride = true;
}
void PositionMachine::disableManual() {
    manOverride = false;
}
void PositionMachine::enableHolding() {
    holding = true;
}
void PositionMachine::disableHolding() {
    holding = false;
}
void PositionMachine::enableSpeed() {
    speedOverride = true;
}
void PositionMachine::disableSpeed() {
    speedOverride = false;
}

void PositionMachine::hold() {
    seeking = true;
    holding = true;
    speedOverride = false;
    targetPosition = position;
}
void PositionMachine::hold(double p) {
    seeking = true;
    holding = true;
    speedOverride = false;
    targetPosition = p;
}
void PositionMachine::seek(double p) {
    targetPosition = p;
    seeking = true;
    holding = false;
}
void PositionMachine::runAtSpeed(double s) {
    speedOverride = true;
    seeking = false;
    runSpeed = s;
}
void PositionMachine::stop() {
    seeking = false;
    holding = false;
    speedOverride = false;
    speed = 0;
}



void PositionMachine::run() {
    
    speed = 0;
    calcPosition();
    
    // seek to position
    if (seeking) {
        double difference;
        // calc difference between curr and tar
        difference = targetPosition - position;
        // if we are not holding, then stop seeking once within range
        if (!holding && (difference*difference) < (range*range)) seeking = false;
        // calculate speed to run motors at
        if (difference > 0)
            speed = difference * posSeekRate;
        else
            speed = difference * negSeekRate;
    }
    
    // run at speed
    if (speedOverride) {
        speed = runSpeed;
    }
    
    // manual ovverides
    if (manOverride) {
        if (negButton->pressing()) {
            stop();
            speed = negManualSpeed;
        }
        if (posButton->pressing()) {
            stop();
            speed = posManualSpeed;
        }
    }
    
    // run the motors
    for (int i = 0; i < motors.size(); i++) {
        motors[i]->spin(vex::directionType::fwd, speed, vex::velocityUnits::pct);
    }
    
}
