///////////////////////////////////////////////////////////////
/////			HACKY McHACKY HACK HACK	CO.											/////
/////				"BUTCHERING PERFECTLY GOOD CODE SINCE 1999"		/////
///////////////////////////////////////////////////////////////



// SMART MOTOR HACKS / ADDITIONAL FAILSAFE
// Idea is:
// Force smartmotors to run normally at full power, typically it likes '89'
// Smartmotors works for short periods of time...
// So keep track of how long S.M. is limiting for, and stop telling it to run after some time



// motor failsafe values, should by in SMHacks.c
int STRAINBEFOREFAIL = 2000;		// 5 seconds
int STRAINRATIO = 2;						// we are straining if actual motor power is 2X what we asked for
int SAFEVALUE = 0;							// when failsafe kicks in

// flags for if auton wants to override stuff
bool limitingLeftDrive = false;
bool limitingRightDrive = false;
bool overrideDriveLimit = false;


float driveAskFor[] = { 0,0,0,0 };

void MakeMotor(tMotor index, float power) {
	// find which motor we want, and set corresponding table value with power
	switch (index) {
	case rightDrive1:
		driveAskFor[0] = power;
		break;
	case rightDrive2:
		driveAskFor[1] = power;
		break;
	case leftDrive1:
		driveAskFor[2] = power;
		break;
	case leftDrive2:
		driveAskFor[3] = power;
		break;
	default:
		return;
	}
}

void RegMotor(tMotor index, float power) {
	// only do smartmotors on the drive
	if (!driveStraight&&(index == rightDrive1 || index == rightDrive2 || index == leftDrive1 || index == leftDrive2)) {
		MakeMotor(index, power);
	}
	else {
		motor[index] = power;			// just force set non-S.M-motors
	}
}

task trackMotorStrain() {
	int leftStrainTime = 0;
	int rightStrainTime = 0;
	float lastLeftPower = 0;
	float lastRightPower = 0;
	while (1) {
		for (int i = 0; i < 4; i++) {		// do for each of the 4 drive motor ports
			tMotor index;
			char side;
			switch (i) {				// figure out which side and index we want
			case 0:
				index = rightDrive1;
				side = 'r';
				break;
			case 1:
				index = rightDrive2;
				side = 'r';
				break;
			case 2:
				index = leftDrive1;
				side = 'l';
				break;
			case 3:
				index = leftDrive2;
				side = 'l';
				break;
			default:
				return;
			}

			float polarity = 0;
			if (abs(driveAskFor[i]) != 0) {											// divide by 0 error
				polarity = driveAskFor[i]/(abs(driveAskFor[i])+0.00001);		// for setting safe value later
			}

			float trying = abs(driveAskFor[i]);			// value user asking for
			float actual = abs(motor[index]);				// value SmartMotors is setting

			if (trying > (actual*STRAINRATIO)) {		// figure out if SmartMotors is limiting us
				if (side == 'r') {										// keep track of time each side of drive is limited for
					rightStrainTime += time1(T4);
				}
				else {
					leftStrainTime += time1(T4);
				}
			}

			if (abs(lastRightPower - driveAskFor[0]) > 10) {			// if we change speeds, restart timer, per side
				rightStrainTime = 0;
			}
			if (abs(lastLeftPower - driveAskFor[2]) > 10) {
				leftStrainTime = 0;
			}

			bool manualLimit = false;
			limitingLeftDrive = false;
			limitingRightDrive = false;

			if (rightStrainTime > STRAINBEFOREFAIL * 2 && side == 'r') {			// we haven't changed directions, and have been straining for at least 1.5 seconds (double because we're adding twice)
				manualLimit = true;
				limitingRightDrive = true;																			// flag so auton can 'see' if the drive has 'failsafed'... this way can give up on driving/whole routine
			}
			if (leftStrainTime > STRAINBEFOREFAIL * 2 && side == 'l') {
				manualLimit = true;
				limitingLeftDrive = true;
			}

			if (manualLimit && !overrideDriveLimit) {
				SetMotor(index, SAFEVALUE*polarity);
			}
			else {
				SetMotor(index, driveAskFor[i] * 2);
			}
		}

		lastRightPower = driveAskFor[0];		// remember what we were last asking for
		lastLeftPower = driveAskFor[2];

		clearTimer(T4);											// clear timer
		wait1Msec(10);											// wait
	}
}
