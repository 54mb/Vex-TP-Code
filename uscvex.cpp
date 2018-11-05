//
//  VEXUSC.cpp
//  
//
//  Created by Sam Burton on 03/11/2018.
//

#include "uscvex.h"

#define DRIVEMODE_USER 0
#define DRIVEMODE_TIME -1
#define DRIVEMODE_DIST -2
#define DRIVEMODE_CUSTOM -3
#define DRIVEMODE_TURN 1

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
int PositionMachine::getNumMotors() {
    return motors.size();
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
bool SpeedMachine::isWithinRange() {
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
    targetDistance = 0';
    autoSpeed = 0;
    autoTime = 0;
    autoMode = DRIVEMODE_USER;
    autoTimeOut = 0;
    turnAccepted = 1;
    turnRate = 1;
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
}
void Drive::setArcadeJoy(vex::controller::axis* p,vex::controller::axis* t) {
    arcadeMode = true;
    powerJoy = p;
    turnJoy = t;
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
void Drive::setDirection(double d) {
    direction = d;
}
void Drive::setTimeOut(double t) {
    autoTimeOut = t;
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
void Drive::setMinSpeed(double m) {
    minSpeed = m;
}
void Drive::setMaxTurn(double m) {
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

void Drive::finishMove() {
    autonComplete = true;
}

void Drive::driveTime(double s, double d, double t) {
    // speed, direction, distance, time
    autoSpeed = s;
    autoMode = DRIVEMODE_TIME;
    autoTime = t;
    targetDirection = d;
}
void Drive::driveDist(double s, double dir, double dist, double t) {
    // speed, direction, distance, timeout
    autoSpeed = s;
    targetDirection = dir;
    autoMode = DRIVEMODE_DIST;
    targetDistance = dist;
    autoTimeOut = t;
}
void Drive::driveCustom(double s, double d, double t) {
    // speed, direction, timeout
    autoSpeed = s;
    autoMode = DRIVEMODE_CUSTOM;
    autoTime = t;
    targetDirection = d;
}
void Drive::turnTo(double a, double t) {
    // angle, timeout
    targetDirection = a;
    autoTimeOut = t;
    autoMode = DRIVEMODE_TURN;
}
void Drive::turnRelative(double a, double t) {
    // angle, timeout
    targetDirection = direction + a;
    autoTimeOut = t;
    autoMode = DRIVEMODE_TURN;
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
    
}







