//
//  BallBot.cpp
//  
//
//  Created by Sam Burton on 04/12/2018.
//

#include "BallBot.h"

// Globals

Drive drive = Drive();
PositionMachine arm = PositionMachine();
SpeedMachine flyWheel = SpeedMachine();

pros::Controller master(pros::E_CONTROLLER_MASTER);
pros::Motor left_mtr(1);
pros::Motor right_mtr(2);

void setUp() {
    // Eg. PosMech setup
    arm.addMotor(&left_mtr);
    arm.addMotor(&right_mtr);
    arm.setController(&master);
    
    // Eg. SpeedMech setup
    SpeedMachine flyWheel = SpeedMachine();
    flyWheel.addMotor(&right_mtr);
    
    // Eg. Drive setup
    drive.addMotorLeft(&left_mtr);
    drive.addMotorRight(&right_mtr);
}
