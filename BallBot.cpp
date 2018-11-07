#include "BallBotConfig.h"
#include "uscvex.h"
#include "BallBotAutons.cpp"

vex::competition Competition;

// Declare objects as globals

// Declare and initialize any global flags we need:
// autoAimEnabled, autoFireEnabled, flywheelTargetSpeed, etc.

void pre_auton( void ) {
    // Set-up SpeedMachine for flywheel
    // Set-up Drive for drive
    
}

int runFlywheel() {
    
    // Declare any local variables
    
    while (true) {
        
        // Set intake motor speeds to 0
        


        if (/*  auto-aim flag is set    */) {
            
            // Check vision sensor to determine necessary turn
            
            
            // Check lidar / ultrasonic for target flywheel speed
            
            
            // Read flags for & set flywheel object target speed
            
            
            // Read vision sensor & ask drive to turn appropriately
            

            // Check current speed of flywheel
            
            
            if (/*  within accepted aim error & flywheel at correct speed   */) {
                // Set flag for fireing ball
            }
            
        }
        
        
        if (/*  flag set for fireing ball   */) {
            // Set intake motor speed
            
            
            if (/*  ball launched   */) {
                // Clear flags
            }
            
            
        }
        
        // Check controller buttons...
        // Set flags for preset flywheel speeds & auto-aim-fire
        // If manual intake buttons pressed, set intake speeds
        
        // Buttons to run intake (For/Rev)
        // Buttons for auto aim & fire (High/Low)
        // Buttons for runing flywheel at preset speeds (2)
        // Button to clear all flags, i.e. stop auto functions
        
        if (/*  preset speed wanted */) {
            // Send target speed to flywheel
            // flywheel.runAtSpeed(speed);
            
        }
        
        // Send speeds to intake motors
        
        
        // flywheel.run();
        
        vex::task::sleep(20);   // don't hog cpu
    }
    return 0;
}


void autonomous( void ) {
    // Start flywheel task
    
    
    // Set pointer to chosen auton routine
    
    
    while (true) {
        
        // Auton table decipherer - switch statement
        // Commands will set flags / call object funtions
        // Need commands for:
        // DRIVE (Time, distance, whiteline)
        // TURN (Abs & relative)
        // PAUSE (Time, till ball shot)
        // SETGYRO
        // FIRE (Auto aim, high & low)
        // INTAKE (Time / Until ball enters)
        
        
        
        
        // Read any sensors and set global variables
        // eg. gyroDirection, etc.
        
        
        
        // Auton command termination code
        // Decide if we should move to the next command
        // eg. checking timers for pause, flags for shooting balls, etc.
        
        
        
        // drive.setDirection(gyroDirection);
        // drive.run();
        
        vex::task::sleep(20);   // let other tasks use cpu
    }
}

void usercontrol( void ) {
    // Start flywheel task
    while (true) {
        
        // Not much to do here
        
        // Read any sensors and set global variables
        // eg. gyroDirection, etc.
        
        
        // drive.setDirection(gyroDirection);
        // drive.run();
        vex::task::sleep(20); //Sleep the task for a short amount of time to prevent wasted resources.
    }
}

//
// Main will set up the competition functions and callbacks.
//
int main() {
    
    //Run the pre-autonomous function.
    pre_auton();
    
    //Set up callbacks for autonomous and driver control periods.
    Competition.autonomous( autonomous );
    Competition.drivercontrol( usercontrol );
    
    //Prevent main from exiting with an infinite loop.
    while (true) {
        vex::task::sleep(100);//Sleep the task for a short amount of time to prevent wasted resources.
    }
    
}
