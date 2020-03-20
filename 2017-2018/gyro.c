///gyro

#define	CSCALE1 			-1.04105556							//Clockwise scale adjustments to counteract rotation errors
#define	ASCALE1 			-1.03416667							//Anti-clockwise scale adjustments to counteract rotation errors

#define	CSCALE2				-1.04105556						//Clockwise scale adjustments to counteract rotation errors
#define	ASCALE2 			-1.03416667							//Anti-clockwise scale adjustments to counteract rotation errors

//		1+(error/(5*3600))
//    1+((error-3600)/(5*3600))

struct	gyros {														//create a structure to be used by the gyro code
	short	port;															//gyro port
	float	truedir;													//corrected direction
	float	last;															//last direction pointing
	float	ascale;														//anti-clockwise scale adjustment
	float	cscale;														//clockwise scale adjustment
};

gyros	gyro1,gyro2;

short gyroinit=0;

float gyroDir;														//average direction of gyros

//short gyro;

void resetGyro()
{
	SensorValue[gyro]=0;
	SensorValue[gyro]=0;					//gyrob if using two
	gyro1.truedir=0;
	gyro2.truedir=0;
	gyro1.last=0;
	gyro2.last=0;
	gyroDir=0;
}

void setGyro(int dir)
{
	gyro1.truedir=dir;
	gyro2.truedir=dir;
	//	gyro1.last=dir;
	//	gyro2.last=dir;
	gyroDir=dir;
}

void checkGyro(gyros *gyro)
{
	float currentGyro; 										//gyro position
	float tempAngle;											//temporary angle variable

	currentGyro=SensorValue[gyro->port];	//read hardware gyro value
	tempAngle=currentGyro-gyro->last;			//what is the delta change in the gyro this loop?
	tempAngle=-tempAngle;
	gyro->last=currentGyro;								//store current gyro value for comparison next time

	if (abs(tempAngle)>2500)				//huge delta so probably wrapped
	{
		if (tempAngle>0) {tempAngle=tempAngle-3600;}	//get true delta change taking...
		else {tempAngle=tempAngle+3600;}				//...into account wrap
	}
	//tempAngle now holds correct delta change between old and new gyro angles

	//		if (abs(ang2)<JITTER) {SensorValue[gyro]=lastgyro;}	//tiny delta change so overwrite hardware gyro with lastgyro (removes jitter)
	//		else

	//		if (abs(ang2)>JITTER)
	if (tempAngle>0)	//anti-clockwise rotation
	{
		gyro->truedir=gyro->truedir+(tempAngle*gyro->ascale);					//update ?tempDir? if anti-clockwise rotation and scale by Anti-Clockwise scale
		if (gyro->truedir<0) {gyro->truedir=gyro->truedir+3600;}		//wrap
	}
	else
	{
		gyro->truedir=gyro->truedir+(tempAngle*gyro->cscale);					//update ?tempDir? if clockwise rotation and scale by Clockwise scale
		if (gyro->truedir>=3600) {gyro->truedir=gyro->truedir-3600;}	//wrap
	}
	//truedir ends up as positive float >=0 and <3600 to be used in rest of code
}

task readGyro()
{
	if (gyroinit==0)
	{
		gyroinit=1;

		gyro1.port=gyro;
		gyro1.truedir=0;
		gyro1.last=SensorValue[gyro1.port];
		gyro1.ascale=ASCALE1;
		gyro1.cscale=CSCALE1;

		gyro2.port=gyro;//////SHOULD BE GYROB
		gyro2.truedir=0;
		gyro2.last=SensorValue[gyro2.port];
		gyro2.ascale=ASCALE2;
		gyro2.cscale=CSCALE2;

		gyroDir=0;
	}

	while(true)
	{
		checkGyro(&gyro1);
		checkGyro(&gyro2);

		//		gyroDir=gyro1.truedir;

		//find average of the two angles
		if (gyro1.truedir>gyro2.truedir)
		{
			float tempAngle=gyro1.truedir;
			gyro1.truedir=gyro2.truedir;
			gyro2.truedir=tempAngle;			//swap order so that gyro2 always larger
		}
		if (gyro2.truedir-gyro1.truedir>1800) gyro2.truedir-=3600;	//big difference so fix wrap
			gyroDir=(gyro2.truedir+gyro1.truedir)/2;		//average them
		if (gyroDir<0) gyroDir+=3600;		//unwrap negative case
			wait1Msec(25);
	}
}