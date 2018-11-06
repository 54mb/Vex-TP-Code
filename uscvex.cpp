//
//  uscvex.cpp
//  
//
//  Created by Sam Burton on 03/11/2018.
//

#include "uscvex.h"
#include <cmath>

#define DRIVEMODE_USER 0
#define DRIVEMODE_TIME -1
#define DRIVEMODE_DIST -2
#define DRIVEMODE_CUSTOM -3
#define DRIVEMODE_TURN 1
#define TURNMODE_GYRO 1
#define TURNMODE_ENCODER 2

PositionMachine::PositionMachine() {
    position = 0;                   // initial position defaults to 0
    targetPosition = 0;             // initial target position 0
    speed = 0;                      // not moving so 0
    scale = 1;                      // scale 1, 1:1 ratio
    upLim = 0;                      // upLim = lowLim = 0, so no mech. stops
    lowLim = 0;
    range = 0;                      // range of 0 means never good enough
    runSpeed = 0;                   // override speed 0
    positionOffset = 0              // used to calculate position
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

void PositionMachine::addMotor(vex::motor* newMotor) {
    // add a motor to our vector
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
void PositionMachine::setManualPos(vex::controller::button* b, double s) {  // setters for manual controls
    posButton = b;
    posManualSpeed = s;
    manOverride = true;
}
void PositionMachine::setManualNeg(vex::controller::button* b, double s) {
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
void PositionMachine::calcPosition() {  // calculates position from motor encoders
    double tot = 0;
    for (int i = 0; i < motors.size(); i++) {   // add up all motor positions
        tot += motors[i]->rotation(vex::rotationUnits::deg);
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
int PositionMachine::getNumMotors() {
    return motors.size();
}

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
void PositionMachine::enableSpeed() {   // enables speed override
    speedOverride = true;
}
void PositionMachine::disableSpeed() {  // disables speed override
    speedOverride = false;
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
void PositionMachine::runAtSpeed(double s) {    // moves at constant speed
    speedOverride = true;
    seeking = false;
    runSpeed = s;
}
void PositionMachine::stop() {  // disable any auto funtion
    seeking = false;
    holding = false;
    speedOverride = false;
    speed = 0;
}

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


Drive::Drive() {
    arcadeMode = false;
    deadZone = 0;
    ticksPerTile = 1;
    ticksPerDegree = 1;
    temperature = 0;
    power = 0;
    current = 0;
    direction = 0;
    targetDirection = 0;
    distance = 0;
    targetDistance = 0;
    autoSpeed = 0;
    autoTime = 0;
    autoMode = DRIVEMODE_USER;
    autoTimeOut = 0;
    turnAccepted = 1;
    turnRate = 1;
    turnPulse = 0;
    pulseTime = 1;
    pulsePause = 0;
    minSpeed = 0;
    maxTurn = 100;
    autonComplete = false;
    currentTime = 0;
    recordedTime = 0;
    leftSpeed = 0;
    rightSpeed = 0;
    leftRunSpeed = 0;
    rightRunSpeed = 0;
    controlsSet = false;
    recordedDistLeft = 0;
    recordedDistRight = 0;
    minForward = 0;
    turnMode = TURNMODE_GYRO;
    lastAngle = 0;
}
~Drive::Drive() {
    
}

void Drive::setBrain(vex::brain* b) {
    driveBrain = b;
}

void Drive::addMotorLeft(vex::motor* m) {
    motorsLeft.push_back(m);
}
void Drive::addMotorRight(vex::motor*) {
    motorsRight.push_back(m);
}

void Drive::setTankJoy(vex::controller::axis* l,vex::controller::axis* r) {
    arcadeMode = false;
    leftSideJoy = l;
    rightSideJoy = r;
    controlsSet = true;
}
void Drive::setArcadeJoy(vex::controller::axis* p,vex::controller::axis* t) {
    arcadeMode = true;
    powerJoy = p;
    turnJoy = t;
    controlsSet = true;
}

void Drive::setTicksPerTile(double t) {
    ticksPerTile = t;
}
void Drive::setTicksPerDegree(double t) {
    ticksPerDegree = t;
}
void Drive::setDeadZone(double d) {
    deadZone = d;
}
void Drive::setMinForwardSpeed(double d) {
    minForward = d;
}
void Drive::setDirection(double d) {
    direction = d;
}
void Drive::setTimeOut(double t) {
    autoTimeOut = t;
}
void Drive::setTurnMode(int m) {
    turnMode = m;
}

void Drive::setTurnAccepted(double t) {
    turnAccepted = t;
}
void Drive::setTurnRate(double t) {
    turnRate = t;
}
void Drive::setPulseTime(double p) {
    pulseTime = p;
}
void Drive::setPulsePause(double p) {
    pulsePause = p
}
void Drive::setMinTurnSpeed(double m) {
    minSpeed = m;
}
void Drive::setMaxTurnSpeed(double m) {
    maxTurn = m;
}

double Drive::getTemperature() {
    double tot = 0;
    for (int i = 0; i < motorsLeft.size(); i++) {
        tot += motors[i]->temperature(vex::percentUnits::pct);
    }
    for (int i = 0; i < motorsRight.size(); i++) {
        tot += motors[i]->temperature(vex::percentUnits::pct);
    }
    temperature = tot/(motorsLeft.size()+motorsRight.size());
    return temperature;
}
double Drive::getPower() {
    double tot = 0;
    for (int i = 0; i < motorsLeft.size(); i++) {
        tot += motors[i]->power(vex::powerUnits::watt);
    }
    for (int i = 0; i < motorsRight.size(); i++) {
        tot += motors[i]->power(vex::powerUnits::watt);
    }
    power = tot/(motorsLeft.size()+motorsRight.size());
    return power;
}
double Drive::getCurrent() {
    double tot = 0;
    for (int i = 0; i < motorsLeft.size(); i++) {
        tot += motors[i]->current(vex::currentUnits::amp);
    }
    for (int i = 0; i < motorsRight.size(); i++) {
        tot += motors[i]->current(vex::currentUnits::amp);
    }
    current = tot/(motorsLeft.size()+motorsRight.size());
    return current;
}
double Drive::getDirection() {
    return direction;
}
void Drive::finishMove() {
    autonComplete = true;
}

void Drive::driveTime(double s, double d, double t) {
    // speed, direction, distance, time
    autoSpeed = s;
    autoMode = DRIVEMODE_TIME;
    autoTimeOut = t;
    targetDirection = d;
    recordedTime = driveBrain->timer(vex::timeUnits::msec);
}
void Drive::driveDist(double s, double dir, double dist, double t = 10) {
    // speed, direction, distance, timeout
    autoSpeed = s;
    targetDirection = dir;
    autoMode = DRIVEMODE_DIST;
    autoTimeOut = t;
    recordedTime = driveBrain->timer(vex::timeUnits::msec);
    recordedDistLeft = 0;
    for (int i = 0; i < motorsLeft.size(); i++) {
        recordedDistLeft += motorsLeft[i]->rotation(vex::rotationUnits::deg);
    }
    recordedDistLeft /= motorsLeft.size();
    recordedDistRight = 0;
    for (int i = 0; i < motorsRight.size(); i++) {
        recordedDistRight += motorsRight[i]->rotation(vex::rotationUnits::deg);
    }
    recordedDistRight /= motorsRight.size();
    
    targetDistance = dist * ticksPerTile; + (recordedDistRight + recordedDistLeft)/2
    
}
void Drive::driveCustom(double s, double d, double t = 10) {
    // speed, direction, timeout
    recordedTime = driveBrain->timer(vex::timeUnits::msec);
    autoSpeed = s;
    autoMode = DRIVEMODE_CUSTOM;
    autoTimeOut = t*1000;
    targetDirection = d;
}
void Drive::turnTo(double a, double t = -1) {
    // angle, timeout
    recordedTime = driveBrain->timer(vex::timeUnits::msec);
    targetDirection = a;
    autoTimeOut = t*1000;
    autoMode = DRIVEMODE_TURN;
    turnMode = TURNMODE_GYRO;
}
void Drive::turnRelative(double a, double t = -1) {
    // angle, timeout
    recordedTime = driveBrain->timer(vex::timeUnits::msec);
    targetDirection = direction + a;
    autoTimeOut = t*1000;
    autoMode = DRIVEMODE_TURN;
    turnMode = TURNMODE_GYRO;
}
void Drive::turnRelativeEncoder(double a, double t = -1) {
    // angle, timeout
    recordedTime = driveBrain->timer(vex::timeUnits::msec);
    targetDirection = direction + a;
    autoTimeOut = t*1000;
    autoMode = DRIVEMODE_TURN;
    turnMode = TURNMODE_ENCODER;
    recordedDistLeft = 0;
    for (int i = 0; i < motorsLeft.size(); i++) {
        recordedDistLeft += motorsLeft[i]->rotation(vex::rotationUnits::deg);
    }
    recordedDistLeft /= motorsLeft.size();
    recordedDistRight = 0;
    for (int i = 0; i < motorsRight.size(); i++) {
        recordedDistRight += motorsRight[i]->rotation(vex::rotationUnits::deg);
    }
    recordedDistRight /= motorsRight.size();
    targetDistance = (a * ticksPerDegree) + (recordedDistRight - recordedDistLeft)/2
}

void Drive::runAtSpeed(double s) {
    leftRunSpeed = s;
    rightRunSpeed = s;
    speedOverride = true;
}
void Drive::runAtSpeeds(double l, double r) {
    leftRunSpeed = l;
    rightRunSpeed = r;
    speedOverride = true;
}

void Drive::stop() {
    autoTime = 0;
    autoMode = DRIVEMODE_USER;
    autoSpeed = 0;
    speedOverride = false;
}

void Drive::run() {
    // This is where the fun begins
    
    double forward = 0;
    double turn = 0;
    
    // Calculate useful information
    currentTime = driveBrain->timer(vex::timeUnits::msec);
    
    double currentDistLeft = 0;
    for (int i = 0; i < motorsLeft.size(); i++) {
        currentDistLeft += motorsLeft[i]->rotation(vex::rotationUnits::deg);
    }
    currentDistLeft /= motorsLeft.size();
    double currentDistRight = 0;
    for (int i = 0; i < motorsRight.size(); i++) {
        currentDistRight += motorsRight[i]->rotation(vex::rotationUnits::deg);
    }
    currentDistRight /= motorsRight.size();
    currentDist = (currentDistRight + currentDistLeft)/2;
    
    // auto functions
    if (autoMode != DRIVEMODE_USER) {
        
        forward = autoSpeed;
        
        if (autoMode == DRIVEMODE_TURN)  {
            forward = 0;
            autoSpeed = 0;
        }
        
        if (autoMode == DRIVEMODE_DIST) {
            forward *= abs(((targetDistance - currentDist)/(1*ticksPerTile) + minForward));
                // slow down when within 1 tile
            if (forward > 100) forward = 100;
            if (forward < -100) forward = -100;
            
            // Terminate contition for distance
            if (autoSpeed > 0) {
                if (currentDist > targetDistance) autonComplete = true;
            }
            else {
                if (currentDist < targetDistance) autonComplete = true;
            }
        }
        
        if (currentTime > autoTimeOut && autoTimeOut > 0) {
            autonComplete = true;
        }
        
        // Turn code
        double driveMag = autoSpeed;
        double seek = targetDirection;
        double angle = 0;
        
        if (turnMode == TURNMODE_GYRO) {
            angle = seek - direction;
        }
        else if (turnMode == TURNMODE_ENCODER) {
            angle = (recordedDistRight - recordedDistLeft)/2;
            angle -= (currentDistRight - currentDistLeft)/2;
            angle /= ticksPerDegree;
        }
        
        if (angle < 0) angle += 360;
        if (angle > 180) angle -= 360;
        
        angle /= (2 * turnRate);
        angle *= 127;
        if (driveMag < minSpeed) {
            angle *= 2;
        }
        
        if (angle < -maxTurn) angle = maxTurn;
        if (angle > maxTurn) angle = maxTurn;
        
        if (driveMag > minSpeed) {
            if (angle < 0) {
                if (angle > -2) {
                    angle = 0;
                }
                else if (angle > -4) {
                    angle = -4
                }
            }
            else {
                if (angle < 2) {
                    angle = 0;
                }
                else if (angle < 4) {
                    angle = 4
                }
            }
        }
        else {
            turn = angle;
            angle = abs(angle);
            if (angle < minSpeed) {
                if (((lastAngle > 0) && (turn < 0)) || ((lastAngle < 0) && (turn > 0))) {
                    angle = 0;
                }
                else {
                    if (angle > minSpeed/5) {
                        angle = minSpeed;
                    }
                    else {
                        turnPulse++;
                        if (turnPulse < pulseTime) {
                            angle = minSpeed;
                        }
                        else {
                            angle = 1;
                            if (turnPulse > pulsePause) {
                                turnPulse = 0;
                            }
                        }
                    }
                }
            }
            if (turn < 0) angle *= -1;
        }
        
        turn = angle;
        
        if (autoSpeed == 0 || autoMode == DRIVEMODE_TURN) {
            if (abs(angle) < turnAccepted && abs(lastAngle) < turnAccepted) {
                autonComplete = true;
            }
        }
        
        lastAngle = angle;
        
        
        // Auto-move is complete, so stop moving
        if (autonComplete) {
            autoMode = DRIVEMODE_USER;
            forward = 0;
            turn = 0;
            autoSpeed = 0;
        }
    }
    
    // User controls
    if (autoMode == DRIVEMODE_USER && controlsSet) {
        if (arcadeMode) {
            // Forward-backward, left-right
            leftSpeed = powerJoy->position(vex::percentUnits::pct) - turnJoy->position(vex::percentUnits::pct);
            rightSpeed = powerJoy->position(vex::percentUnits::pct) + turnJoy->position(vex::percentUnits::pct);
        }
        else {
            // Tank controls
            leftSpeed = leftSideJoy->position(vex::percentUnits::pct);
            rightSpeed = rightSideJoy->position(vex::percentUnits::pct);
        }
        if (leftSpeed*leftSpeed < deadZone*deadZone) leftSpeed = 0;
        if (rightSpeed*rightSpeed < deadZone*deadZone) rightSpeed = 0;
    }
    else {
        leftSpeed = forward - turn;
        rightSpeed = forward + turn;
    }
    
    // Constant-speed override
    if (speedOverride) {
        leftSpeed = leftRunSpeed;
        rightSpeed = rightRunSpeed;
    }
    
    // Set motors
    for (int i = 0; i < motorsLeft.size(); i++) {
        motorsLeft[i]->spin(vex::directionType::fwd, leftSpeed, vex::velocityUnits::pct);
    }
    for (int i = 0; i < motorsRight.size(); i++) {
        motorsRight[i]->spin(vex::directionType::fwd, rightSpeed, vex::velocityUnits::pct);
    }
}







