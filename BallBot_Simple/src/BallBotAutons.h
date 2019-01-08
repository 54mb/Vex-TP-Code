
#ifndef __BALLBOTAUTONS__
#define __BALLBOTAUTONS__

#define NUMBER_AUTONS 1
int autonSelect = 1;


// DEFINES FOR AUTON MODES
#define DRIVEMODE_USER 0
#define DRIVEMODE_TIME -1
#define DRIVEMODE_DIST -2
#define DRIVEMODE_CUSTOM -3
#define DRIVEMODE_TURN 1
#define TURNMODE_GYRO 1
#define TURNMODE_ENCODER 2


// #defines for commands & conditions
#define END 0
#define DRIVE -1
#define TURN -2
#define TURN_REL -3
#define TURN_ENC -4
#define FIRE_PRESET -5
#define FIRE_AIM -6
#define INTAKE_ON -7
#define INTAKE_OFF -8
#define ARMSEEK -9
#define WRISTSEEK -10
#define FLIPPERSEEK -11
#define FLIP -12
#define STACK_LOW -13
#define STACK_HIGH -14
#define STACK_LOW_FROM -15
#define STACK_HIGH_FROM -16
#define PAUSE -17

#define DISTANCE -1
#define WHITELINE -2

#define FIRED -1
#define GOTBALL -2
#define GOTBALLS -3



double defaultAuton[] = {
    0,
    END
};

// Arrays for routines
double auton1[] = {
    0,
    END
};

#endif
