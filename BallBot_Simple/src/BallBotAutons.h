
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
#define SET_GYRO -18            // Set gyro to some angle

#define DISTANCE -1             // Drive condition for distance based on encoders
#define LIDAR -2                // Drive condition for distance based on lidar

#define FIRED -1                // Pause condition wait till fired
#define GOTBALL -2              // Pause condition wait till one ball
#define GOTBALLS -3             // Pause condition wait till two balls

/*
 Motor drive_left_1(1, SPEED, 0, RAW);
 Motor drive_left_2(2, SPEED, 1, RAW);
 Motor drive_left_3(3, SPEED, 0, RAW);
 Motor drive_right_1(4, SPEED, 1, RAW);
 Motor drive_right_2(5, SPEED, 0, RAW);
 Motor drive_right_3(6, SPEED, 1, RAW);
 // Flywheel Motors
 Motor flywheel_1(17, TURBO, 1, RAW);
 Motor flywheel_2(16, TURBO ,0, RAW);
 // Intake
 Motor intake_in(18, SPEED, 1, RAW);
 Motor intake_out(19, SPEED, 1, RAW);
 // Arm Motors
 Motor arm_1(7, SPEED, 0, RAW);
 Motor arm_2(20, SPEED, 1, RAW);
 // Flipper
 Motor wrist(11, SPEED, 1, RAW);
 Motor flip(14, SPEED, 0, RAW);
 // Gyro Sensor
 ADIGyro sensor_gyro(1, GYRO_PORT);
 */

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
