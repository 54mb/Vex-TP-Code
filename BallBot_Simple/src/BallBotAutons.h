
#ifndef __BALLBOTAUTONS__
#define __BALLBOTAUTONS__

#define NUMBER_AUTONS 3                 // Number of programmed routines
int autonSelect = 0;                    // Routine to start on

                                        // #defines for auton drive modes
#define DRIVEMODE_USER 0                // User mode
#define DRIVEMODE_TIME -1               // Drive for time
#define DRIVEMODE_DIST -2               // Drive for distance
#define DRIVEMODE_CUSTOM -3             // Drive custom
#define DRIVEMODE_TURN 1                // Turn
#define TURNMODE_GYRO 1                 // Do turns with gyro
#define TURNMODE_ENCODER 2              // Do turns with encoders


                                        // #defines for commands & conditions
#define END 0                           // End of auton routine
#define DRIVE -1                        // Drive (until some condition met)
#define TURN -2                         // Turn to some angle at some speed
#define TURN_REL -3                     // Turn for some angle at some speed
#define TURN_ENC -4                     // Turn for some angle using encoder ticks
#define FIRE -5                         // Shoot flywheel at preset speed
#define FIRE_AIM -6                     // Aim and fire ball
#define INTAKE_ON -7                    // Turn intake on
#define INTAKE_OFF -8                   // Turn intake off
#define ARMSEEK -9                      // Move arm to pos
#define WRISTSEEK -10                   // Move wrist to pos
#define FLIPSEEK -11                    // Move flipper to pos
#define FLIP -12                        // Auto flip 180°
#define STACK_LOW -13                   // Auto stack on low
#define STACK_HIGH -14                  // Auto stack on high
#define STACK_LOW_FROM -15              // Auto stack from a step low
#define STACK_HIGH_FROM -16             // Auto stack from a step high
#define PAUSE -17                       // Do nothing for some time
#define SET_GYRO -18                    // Set gyro to some angle
#define STOP_FLYWHEEL -19               // Stop Flywheel running
#define FINISH_LOW_STACK -20            // Finish low stack
#define TURN_AIM -21                    // Turn to aim at _____ (target, location, time)
#define STOP_FIRE -22                   // Cancel Fire

#define BLUE_FLAG 1                     // Blue flag for aiming
#define RED_FLAG 2                      // Red flag for aiming
#define GREEN_FLAG 3                    // Green flag for aiming
#define CENTER 0                        // Aim at center-most obj.
#define LEFT 1                          // Aim at left-most obj.
#define RIGHT 2                         // Aim at right-most obj.

#define CDIR -563                       // Turn Parameter for drive facing current direction
#define DISTANCE -1                     // Drive condition for distance based on encoders
#define LIDAR -2                        // Drive condition for distance based on lidar

                                        // Drive conditions for white lines
#define WHITE_E -3                      // Either white
#define WHITE_B -4                      // Both white
#define WHITE_L -5                      // Left white
#define WHITE_R -6                      // Right white
#define BLACK_E -7                      // Either black
#define BLACK_B -8                      // Both black
#define BLACK_L -9                      // Left black
#define BLACK_R -10                     // Right black

#define FIRED -1                        // Pause condition wait till fired
#define GOTBALL -2                      // Pause condition wait till one ball
#define GOTBALLS -3                     // Pause condition wait till two balls
#define UNTIL -4                        // Pause condition wait till time (not for time)
#define STACKED -5                      // Pause condition wait till stacked

                                        // #defines for arm positions
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

double redAuton[] = {                   // RED SIDE, WE WANT 19 PT SWING
    135,                                // START FACING 315
    
    WRISTSEEK,-1000,                    // DEPLOY FLIPPER
    PAUSE,0.5,
    FLIP,
    WRISTSEEK,WRIST_FORWARD_POS,
    PAUSE,0.5,                          // DEPLOY DONE
    
    
    DRIVE,-70,135,DISTANCE,0.75,2,      // DRIVE TO CAP 1
    WRISTSEEK,WRIST_VERTICAL_POS,       // PICK IT UP
    PAUSE,0.5,                          // TIME TO PICK UP
    
    DRIVE,70,135,WHITE_R,2,             // DRIVE TILL RIGHT SENSOR IS OVER START TILE
    TURN,180,2,                         // TURN FOR NEXT DRIVE
    DRIVE,70,180,BLACK_B,2,             // DRIVE UNTIL OFF START TILE
    DRIVE,-70,180,DISTANCE,0.125,1,     // DRIVE TO LINE UP
    TURN,90,2,                          // TURN TO LINE UP
    DRIVE,70,90,DISTANCE,0.25,1,        // DRIVE TO WALL
    
    STACK_HIGH,                         // STACK CAP 1
    PAUSE,STACKED,10,                   // WAIT TILL STACKED
    
    DRIVE,-70,90,DISTANCE,0.5,2,        // DRIVE AWAY FROM WALL
    TURN,180,2,                         // TURN TO INTAKE BALL
    INTAKE_ON,                          // TURN INTAKE ON
    DRIVE,70,180,DISTANCE,0.5,2,        // DRIVE WITHIN RANGE OF OTHER BOT
    
    PAUSE,GOTBALLS,5,                   // WAIT UNTIL BALL ARRIVED
    
    DRIVE,-70,180,DISTANCE,0.5,2,       // DRIVE AWAY FROM OTHER BOT
    
    TURN,0,2,                           // TURN TOWARDS FLAGS
    TURN_AIM,BLUE_FLAG,LEFT,2,          // AIM AT LEFT-MOST BLUE FLAG
    DRIVE,70,CDIR,DISTANCE,0.5,2,       // DRIVE TO TOP FLAG DIST
    FIRE_AIM,TOP,                       // SHOOT TOP FLAG
    PAUSE,FIRED,5,                      // WAIT TILL SHOT
    STOP_FIRE,                          // STOP FLYWHEEL
    
    DRIVE,70,CDIR,DISTANCE,1,2,         // DRIVE TO MIDDLE FLAG DIST
    FIRE_AIM,MIDDLE,                    // SHOOT MIDDLE FLAG
    PAUSE,FIRED,5,                      // WAIT TILL SHOT
    STOP_FIRE,                          // STOP FLYWHEEL
    
    DRIVE,70,0,DISTANCE,1,2,            // DRIVE TO TOGGLE BOTTOM FLAG
    PAUSE,0.25,                         // SHORT PAUSE
    
    DRIVE,-127,0,WHITE_L,4,             // DRIVE TO START TILE
    DRIVE,-100,0,BLACK_L,2,             // DRIVE OFF OF START TILE
    
    TURN,315,2,                         // TURN FOR PLATFORM BALL
    INTAKE_ON,                          // TURN INTAKE ON
    
    DRIVE,90,315,DISTANCE,1,2,          // DRIVE W/ GLANCING BLOW ON PLATFORM
    TURN,270,0.5,                       // TURN FOR NEXT MOVE
    DRIVE,100,270,DISTANCE,1,2,         // DRIVE TO GET BALL FROM UNDER CAP
    
    PAUSE,0.5,                          // SHORT PAUSE
    DRIVE,-70,270,DISTANCE,0.5,2,       // DRIVE AWAY FROM CAP
    
    TURN,315,2,                         // TURN TOWARDS CENTER FLAGS
    TURN_AIM,BLUE_FLAG,CENTER,2,        // AIM AT CENTER-MOST BLUE FLAG
    DRIVE,70,CDIR,DISTANCE,0.5,2,       // DRIVE TO TOP FLAG DIST
    
    PAUSE,UNTIL,35,                     // WAIT TILL LAST 10 SECONDS
    
    FIRE_AIM,TOP,                       // SHOOT TOP FLAG
    PAUSE,FIRED,5,                      // WAIT TILL SHOT
    STOP_FIRE,                          // STOP FLYWHEEL
    
    DRIVE,70,CDIR,DISTANCE,1,2,         // DRIVE TO MIDDLE FLAG DIST
    FIRE_AIM,MIDDLE,                    // SHOOT MIDDLE FLAG
    PAUSE,FIRED,5,                      // WAIT TILL SHOT
    STOP_FIRE,                          // STOP FLYWHEEL
    
    DRIVE,127,CDIR,DISTANCE,1,2,        // DRIVE TO TOGGLE BOTTOM FLAG
    PAUSE,UNTIL,44,                     // WAIT UNTIL LAST SECOND
    DRIVE,-127,315,DISTANCE,1,2,        // DRIVE AWAY FROM FLAGS
    
    END                                 // END OF ROUTINE
};

double redAuton_1_25[] = {
    135,
    
    WRISTSEEK,-1000,                    // Deploy flipper
    PAUSE,0.5,
    FLIP,
    WRISTSEEK,WRIST_FORWARD_POS,
    PAUSE,0.5,
    
    DRIVE,-70,135,DISTANCE,0.75,2,      // Drive to Cap 1
    WRISTSEEK,WRIST_VERTICAL_POS,       // Pick it up
    PAUSE,0.5,
    DRIVE,70,135,DISTANCE,1.4,3,        // Drive to pole
    TURN,90,1,                          // Line up
    DRIVE,70,90,0.125,
    
    STACK_HIGH,                         // Stack Cap 1
    PAUSE,STACKED,10,
    
    DRIVE,-70,90,DISTANCE,0.25,2,       // Drive away from wall
    TURN,135,1,
    DRIVE,-70,135,DISTANCE,0.75,2,      // Line up for ball under Cap 2
    TURN,270,2,
    
    INTAKE_ON,                          // Activate Intake
    DRIVE,127,270,DISTANCE,1.25,3,      // Drive to Cap 2
    PAUSE,0.25,                         // Get ball
    DRIVE,-127,270,DISTANCE,2,2,        // Drive back to tile
    
    TURN,0,1,                           // Turn towards flags
    TURN_AIM,BLUE_FLAG,LEFT,1,          // Use camera to aim
    FIRE_AIM,TOP,                       // Shoot at top flag
    PAUSE,FIRED,10,
    STOP_FIRE,
    
    DRIVE,70,0,DISTANCE,1,2,            // Drive to range for Middle Flag
    FIRE_AIM,MIDDLE,                    // Aim and fire at middle flag
    PAUSE,FIRED,10,
    STOP_FIRE,
    
    DRIVE,127,0,DISTANCE,1,2,           // Drive to toggle bottom flag
    DRIVE,-127,0,DISTANCE,1,2,          // Drive away after
    
    END
};

double skills[] = {
    0,
    END
};

#endif
