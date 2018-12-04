//
//  BallBot.h
//  
//
//  Created by Sam Burton on 04/12/2018.
//

#ifndef __BallBot__
#define __BallBot__

#include "main.h"

#define FLYWHEEL 1
#define ARM 2

#define HIGH 1
#define MIDDLE 2

// #defines for controller buttons
#define BTN_FIRE_LOW DIGITAL_L1
#define BTN_FIRE_HIGH DIGITAL_L1
#define BTN_FIRE_BOTH DIGITAL_L1
#define BTN_FIRE_PRESET DIGITAL_L1
#define BTN_INTAKE_IN DIGITAL_L1
#define BTN_INTAKE_OUT DIGITAL_L1
#define BTN_INTAKE_TOGGLE DIGITAL_L1
#define BTN_ABORT DIGITAL_L1

#define BTN_TOGGLE_MODE DIGITAL_L1

#define BTN_ARM_HIGH DIGITAL_L1
#define BTN_ARM_LOW DIGITAL_L1
#define BTN_ARM_DOWN DIGITAL_L1
#define BTN_WRIST_VERTICAL DIGITAL_L1
#define BTN_WRIST_FORWARD DIGITAL_L1
#define BTN_WRIST_BACK DIGITAL_L1
#define BTN_FLIP DIGITAL_L1

// #defines for arm positions
#define FLIP_POS1 0
#define FLIP_POS2 0
#define WRIST_BACK_POS 0
#define WRIST_FORWARD_POS 0
#define WRIST_VERTICAL_POS 0
#define ARM_POS_HIGH 0
#define ARM_POS_LOW 0
#define ARM_POS_DOWN 0

// #defines for tuning
#define FLYWHEEL_AIM_RANGE 5    // fire ball when within x degrees of flag

// Shared Function Prototypes
double processEntry();
void setGyro(double);
void runArm(void*);
void runFlywheel(void*);
void initAll();


#endif
