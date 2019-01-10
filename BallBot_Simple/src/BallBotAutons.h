
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
#define END 0                   // End of auton routine
#define DRIVE -1                // Drive (until some condition met)
#define TURN -2                 // Turn to some angle at some speed
#define TURN_REL -3             // Turn for some angle at some speed
#define TURN_ENC -4             // Turn for some angle using encoder ticks
#define FIRE_PRESET -5          // Shoot flywheel at preset speed
#define FIRE_AIM -6             // Aim and fire ball
#define INTAKE_ON -7            // Turn intake on
#define INTAKE_OFF -8           // Turn intake off
#define ARMSEEK -9              // Move arm to pos
#define WRISTSEEK -10           // Move wrist to pos
#define FLIPPERSEEK -11         // Move flipper to pos
#define FLIP -12                // Auto flip 180°
#define STACK_LOW -13           // Auto stack on low
#define STACK_HIGH -14          // Auto stack on high
#define STACK_LOW_FROM -15      // Auto stack from a step low
#define STACK_HIGH_FROM -16     // Auto stack from a step high
#define PAUSE -17               // Do nothing for some time

#define DISTANCE -1             // Drive condition for distance based on encoders
#define WHITELINE -2            // Drive condition looking for white line

#define FIRED -1                // Pause condition wait till fired
#define GOTBALL -2              // Pause condition wait till one ball
#define GOTBALLS -3             // Pause condition wait till two balls



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
