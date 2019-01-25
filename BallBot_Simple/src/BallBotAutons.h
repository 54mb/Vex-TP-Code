
#ifndef __BALLBOTAUTONS__
#define __BALLBOTAUTONS__

#define NUMBER_AUTONS 3
int autonSelect = 0;

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
#define FIRE -5                 // Shoot flywheel at preset speed
#define FIRE_AIM -6             // Aim and fire ball
#define INTAKE_ON -7            // Turn intake on
#define INTAKE_OFF -8           // Turn intake off
#define ARMSEEK -9              // Move arm to pos
#define WRISTSEEK -10           // Move wrist to pos
#define FLIPSEEK -11            // Move flipper to pos
#define FLIP -12                // Auto flip 180°
#define STACK_LOW -13           // Auto stack on low
#define STACK_HIGH -14          // Auto stack on high
#define STACK_LOW_FROM -15      // Auto stack from a step low
#define STACK_HIGH_FROM -16     // Auto stack from a step high
#define PAUSE -17               // Do nothing for some time
#define SET_GYRO -18            // Set gyro to some angle
#define STOP_FLYWHEEL -19       // Stop Flywheel running
#define FINISH_LOW_STACK -20    // Finish low stack
#define TURN_AIM -21            // Turn to aim at _____ (target, location, time)
#define STOP_FIRE -22           // Cancel Fire

#define BLUE_FLAG 1             // Blue flag for aiming
#define RED_FLAG 2              // Red flag for aiming
#define GREEN_FLAG 3            // Green flag for aiming
#define CENTER 0                // Aim at center-most obj.
#define LEFT 1                  // Aim at left-most obj.
#define RIGHT 2                 // Aim at right-most obj.

#define DISTANCE -1             // Drive condition for distance based on encoders
#define LIDAR -2                // Drive condition for distance based on lidar

#define WHITE_E -3              // Drive conditions for white lines
#define WHITE_B -4
#define WHITE_L -5
#define WHITE_R -6
#define BLACK_E -7
#define BLACK_B -8
#define BLACK_L -9
#define BLACK_R -10

#define FIRED -1                // Pause condition wait till fired
#define GOTBALL -2              // Pause condition wait till one ball
#define GOTBALLS -3             // Pause condition wait till two balls
#define UNTIL -4                // Pause condition wait till time (not for time)
#define STACKED -5              // Pause condition wait till stacked

// #defines for arm positions           // CALCULATE
#define FLIP_POS1 1                     // 1:1 Ratio, 0°
#define FLIP_POS2 180                   // 1:1 Ratio, 180°
#define WRIST_BACK_POS (200*3)          // 1:3 Ratio, 200°
#define WRIST_BACKWARD_DROP_POS (-70*3) // 1:3 Ratio, -70°
#define WRIST_FORWARD_POS (80*3)        // 1:3 Ratio, 80°
#define WRIST_FORWARD_DROP_POS (70*3)   // 1:3 Ratio, 65°
#define WRIST_VERTICAL_POS 1            // 1:3 Ratio, 0°
#define ARM_POS_HIGH (120*5)            // 1:5 Ratio, 120°
#define ARM_POS_LOW (88*5)              // 1:5 Ratio, 90°
#define ARM_POS_DOWN 1                  // 1:5 Ratio, 0°

#define TOP 2                           // Top Flag
#define MIDDLE 1                        // Middle Flag

double defaultAuton[] = {
    0,
    END
};

// Arrays for routines
double blueAuton[] = {
    0,
    END
};

double redAuton[] = {
    135,
    
    WRISTSEEK,-1000,
    PAUSE,0.5,
    FLIP,
    WRISTSEEK,WRIST_FORWARD_POS,
    PAUSE,0.5,
    
    DRIVE,-70,135,DISTANCE,0.75,2,
    WRISTSEEK,WRIST_VERTICAL_POS,
    PAUSE,0.5,
    DRIVE,70,135,DISTANCE,1.4,3,
    TURN,90,1,
    DRIVE,70,90,0.125,
    
    STACK_HIGH,
    PAUSE,STACKED,10,
    
    DRIVE,-70,90,DISTANCE,0.25,2,
    TURN,135,1,
    DRIVE,-70,135,DISTANCE,0.75,2,
    TURN,270,2,
    
    INTAKE_ON,
    DRIVE,127,270,DISTANCE,1.25,3,
    PAUSE,0.25,
    DRIVE,-127,270,DISTANCE,2,2,
    
    TURN,0,1,
    TURN_AIM,BLUE_FLAG,LEFT,1,
    FIRE_AIM,TOP,
    PAUSE,FIRED,10,
    STOP_FIRE,
    
    DRIVE,70,0,DISTANCE,1,2,
    FIRE_AIM,MIDDLE,
    PAUSE,FIRED,10,
    STOP_FIRE,
    
    DRIVE,127,0,DISTANCE,1,2,
    DRIVE,-127,0,DISTANCE,1,2,
    
    END
};

double skills[] = {
    0,
    END
};

#endif

/*
 
 Old Red Run
 
 180,
 
 PAUSE,3,
 INTAKE_ON,
 PAUSE,GOTBALL,5,                  // WAIT TILL GOT BALL
 
 INTAKE_OFF,
 
 TURN,270,2,                     // TURN TO FACE FLAG
 TURN,355,3,
 DRIVE,100,0,DISTANCE,0.1,1,      // DRIVE CLOSE ENOUGH
 
 FIRE,                           // SHOOT HIGH FLAG
 PAUSE,0.5,
 PAUSE,FIRED,10,
 PAUSE,0.5,
 INTAKE_ON,
 
 STOP_FLYWHEEL,
 
 //SET_GYRO,0,                      // CRASHES ON SET_GYRO (setGyro();)
 
 DRIVE,100,0,DISTANCE,1.35,1,      // DRIVE TO LOW FLAG RANGE
 
 WRISTSEEK,WRIST_FORWARD_POS,    // HIT LOW FLAG
 
 DRIVE,-127,0,DISTANCE,0.5,1,
 
 FIRE,                           // SHOOT
 PAUSE,0.5,
 PAUSE,FIRED,10,
 INTAKE_OFF,
 PAUSE,0.5,
 
 STOP_FLYWHEEL,
 
 WRISTSEEK,WRIST_VERTICAL_POS,    // PUT WRIST BACK UP
 
 PAUSE,1,
 
 DRIVE,-100,0,DISTANCE,0.65,2,     // DRIVE AWAY FROM FLAG
 TURN,270,2,                      // TURN TO FACE
 
 DRIVE,-70,270,0.5,                 // RESET GYRO AGAINST WALL
 // SET_GYRO,270,
 
 WRISTSEEK,(WRIST_FORWARD_POS),    // PUT FLIPPER DOWN FOR CAP
 
 INTAKE_ON,
 
 PAUSE,2,
 
 //TURN,90,2,
 
 DRIVE,50,270,DISTANCE,1,4,       // DRIVE TO CAP
 
 FLIP,
 
 PAUSE,1,
 
 DRIVE,-70,270,DISTANCE,1,2,      // LINE UP FOR NEXT CAP
 TURN,225,2,
 DRIVE,70,225,DISTANCE,1,2,
 DRIVE,70,270,DISTANCE,1,2,
 
 TURN,315,1,
 
 DRIVE,-70,270,DISTANCE,1,2,      // MOVE AWAY FROM CAP
 
 TURN,325,3,
 
 WRISTSEEK,WRIST_VERTICAL_POS,
 
 DRIVE,127,325,DISTANCE,1,2,
 
 FIRE,                           // LONG SHOT - SHOOT
 PAUSE,0.5,
 PAUSE,FIRED,10,
 INTAKE_OFF,
 PAUSE,0.5,
 
 STOP_FLYWHEEL,
 
 DRIVE,100,325,DISTANCE,1,2,
 
 WRISTSEEK,WRIST_FORWARD_POS,
 PAUSE,1,
 WRISTSEEK,WRIST_VERTICAL_POS,
 
 DRIVE,-100,325,DISTANCE,1.75,2,
 
 END,
 
 */
