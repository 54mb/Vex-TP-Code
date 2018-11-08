#define NUMBER_AUTONS
int autonSelect = 1;


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

#define DISTANCE -1
#define WHITELINE -2

#define FIRED -1
#define GOTBALL -2
#define GOTBALLS -3

double[] defaultAuton = {
    0,
    END
}

// Arrays for routines
double[] auton1 = {
    0,
    END
}
