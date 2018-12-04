//
//  posmech.cpp
//  
//
//  Created by Sam Burton on 03/11/2018.
//

#include "main.h"
#include "posmech.h"
#include <cmath>

// CONSTRUCTOR
bug

PositionMachine::PositionMachine() {
    position = 0;                   // initial position defaults to 0
    targetPosition = 0;             // initial target position 0
    speed = 0;                      // not moving so 0
    scale = 1;                      // scale 1, 1:1 ratio
    upLim = 0;                      // upLim = lowLim = 0, so no mech. stops
    lowLim = 0;
    range = 0;                      // range of 0 means never good enough
    runSpeed = 0;                   // override speed 0
    positionOffset = 0;              // used to calculate position
    temperature = 0;
    power = 0;
    current = 0;
    seeking = false;                // are we currently seeking
    manOverride = false;            // are we allowing button control
    speedOverride = false;          // are we running at constant speed
    holding = false;                // are we trying to hold
    posManualSpeed = 100;           // manual speeds default to full
    negManualSpeed = -100;
    posSeekRate = 1;                // default seek rates of 1
    negSeekRate = -1;
}
PositionMachine::~PositionMachine() {
    // nothing to do
}


// CONFIG

void PositionMachine::setController(pros::Controller* c) {
    controller = c;
}
void PositionMachine::addMotor(pros::Motor* newMotor) {
    // add a motor to our vector
    motors.push_back(newMotor);
}
void PositionMachine::setPosition(double p) {
    // set all motor encoders to 0
    for (int i = 0; i < motors.size(); i++) {
        motors[i]->tare_position();
    }
    // set position
    positionOffset = p;
    position = p;
}
void PositionMachine::setLimits(double l, double u) {   // setters for physical conditions
    lowLim = l;
    upLim = u;
}
void PositionMachine::setRange(double s) {
    range = s;
}
void PositionMachine::setScale(double s) {
    scale = s;
}
void PositionMachine::setManualPos(pros::controller_digital_e_t b, double s) {  // setters for manual controls
    posButton = b;
    posManualSpeed = s;
    manOverride = true;
}
void PositionMachine::setManualNeg(pros::controller_digital_e_t b, double s) {
    negButton = b;
    negManualSpeed = s;
    manOverride = true;
}
void PositionMachine::setSeekRate(double r) {   // setters for tuning seek
    negSeekRate = r;
    posSeekRate = r;
}
void PositionMachine::setSeekRates(double dr, double ur) {
    negSeekRate = dr;
    posSeekRate = ur;
}


// GETTERS

void PositionMachine::calcPosition() {  // calculates position from motor encoders
    double tot = 0;
    for (int i = 0; i < motors.size(); i++) {   // add up all motor positions
        tot += motors[i]->get_position();
    }
    tot /= motors.size();                       // average them
    tot *= scale;                               // multiply by scale factor
    tot += positionOffset;                      // add position offset
    if (lowLim != upLim) {                      // if mechanical stops active
        if (tot < lowLim) {                     // if pos is < lower stop
            tot -= positionOffset;              // take position offset out
            positionOffset = lowLim - tot;      // change position offset
            tot += positionOffset;              // add it back in
        }
        if (tot < upLim) {                      // do same for if gone too high
            tot -= positionOffset;
            positionOffset = upLim - tot;
            tot += positionOffset;
        }
    }
    position = tot;                             // set position
}
double PositionMachine::getPosition() {             // getter functions
    calcPosition();
    return position;
}
double PositionMachine::getSpeed() {
    return speed;
}
double PositionMachine::getTemperature() {      // returns average from all motors
    double tot = 0;
    for (int i = 0; i < motors.size(); i++) {
        tot += motors[i]->get_temperature();
    }
    temperature = tot/motors.size();
    return temperature;
}
double PositionMachine::getPower() {
    double tot = 0;
    for (int i = 0; i < motors.size(); i++) {
        tot += motors[i]->get_power();
    }
    power = tot/motors.size();
    return power;
}
double PositionMachine::getCurrent() {
    double tot = 0;
    for (int i = 0; i < motors.size(); i++) {
        tot += motors[i]->get_current_draw();
    }
    current = tot/motors.size();
    return current;
}
int PositionMachine::getNumMotors() {
    return motors.size();
}


// CONTROL FUNCTIONS

void PositionMachine::disableSeek() {   // stops seeking
    seeking = false;
}
void PositionMachine::enableSeek() {    // resumes last seek
    seeking = true;
}
void PositionMachine::enableManual() {  // lets user manually override
    manOverride = true;
}
void PositionMachine::disableManual() { // stops manual buttons
    manOverride = false;
}
void PositionMachine::enableHolding() { // sets holding flag
    holding = true;
}
void PositionMachine::disableHolding() {    // disables holding flag
    holding = false;
}
void PositionMachine::disableSpeed() {  // disables speed override
    speedOverride = false;
}
void PositionMachine::enableSpeed() {   // enables speed override
    speedOverride = true;
}


// AUTON FUNCTIONS

void PositionMachine::stop() {  // disable any auto funtion
    seeking = false;
    holding = false;
    speedOverride = false;
    speed = 0;
}
void PositionMachine::runAtSpeed(double s) {    // moves at constant speed
    speedOverride = true;
    seeking = false;
    runSpeed = s;
}
void PositionMachine::hold() {  //  holds at current position
    seeking = true;
    holding = true;
    speedOverride = false;
    targetPosition = position;
}
void PositionMachine::hold(double p) {  // seeks and holds at a target position
    seeking = true;
    holding = true;
    speedOverride = false;
    targetPosition = p;
}
void PositionMachine::seek(double p) {  // seeks to target
    targetPosition = p;
    seeking = true;
    holding = false;
}


// CALL EVERY LOOP

void PositionMachine::run() {   // call every loop to run the machine
    
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
        if (controller->get_digital(negButton)) {
            stop();
            speed = negManualSpeed;
        }
        if (controller->get_digital(posButton)) {
            stop();
            speed = posManualSpeed;
        }
    }
    
    // run the motors
    for (int i = 0; i < motors.size(); i++) {
        motors[i]->move_voltage(speed * 12000);
    }
    
}
