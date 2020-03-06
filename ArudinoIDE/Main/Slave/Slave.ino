#include "motorLibrary.h"

#define MS1 4 //
#define MS1 5 //
#define MS1 6 //

#define stepPin 2
#define dirPin  3

#define addr 1

// info to be received from master
int      dely;
int   noSteps;
bool timeMode;
int       dir;
int   stpSize;

void setup() {
    pinMode(stepPin, OUTPUT);
    pinMode(dirPin,  OUTPUT);
    pinMode(MS1,  OUTPUT);
    pinMode(MS2,  OUTPUT);
    pinMode(MS3,  OUTPUT);

    Wire.begin(addr);                   // join i2c bus with Slave address
    Wire.OnReceive(rec_handler);        // register receive event

    Serial.begin(9600);
}

void loop() {
    digitalWrite(dirPin, dir);

    // set step size pins on the A4988
    if      (stpSize==1) { MS1=0; MS2=0; MS3=0; }
    else if (stpSize==2) { MS1=1; MS2=0; MS3=0; }
    else if (stpSize==4) { MS1=0; MS2=1; MS3=0; }
    else if (stpSize==8) { MS1=1; MS2=1; MS3=0; }

    //
    if (dely==0 || noSteps==0) { // non-moving state
        digitalWrite(stepPin, HIGH);
    } else if (!timeMode) {
        for (int i=0; i<noSteps; i++) {
            digitalWrite(stepPin, HIGH);
            delay(dely);
            digitalWrite(stepPin, LOW);
            delay(dely);
        }
    } else { // noSteps interpreted as time in ms
        unsigned long startTime =  millis();
        bool inf; // if infinite spin
        if (noSteps == -1) { inf = 1; } else { inf = 0; }
        while (inf || (millis() <= startTime + (unsigned long)noSteps)) {
            digitalWrite(stepPin, HIGH);
            delay(dely);
            digitalWrite(stepPin, LOW);
            delay(dely);
        }
    }
}

void rec_handler(int numBytes) {
    // executes for each slave whenever data is received from master.
    char msg[100];
    int i=0;
    while (0 < Wire.available()) {
        msg[i] = Wire.read();
        i++;
    }

    // split and extract data from msg
    char * split_msg = strtok(msg, ",");
    dely = atoi(split_msg);
    split_msg = strtok(NULL, ",");
    noSteps = atoi(split_msg);
    split_msg = strtok(NULL, ",");
    timeMode = atoi(split_msg);
    split_msg = strtok(NULL, ",");
    dir = atoi(split_msg);
    split_msg = strtok(NULL, ",");
    stpSize = atoi(split_msg);

    // error-checking the values received from master.
    // simple checks
    if (dely < 2000 && dely != 0) { return 0; }
    if (noSteps < -1) { return 0; }
    if (dir != 0 && dir != 1) { return 0; }
    if (stpSize != 1 && stpSize != 2 && stpSize != 4 && stpSize != 8) { return 0; }

    // checking valid timeMode and noSteps combinations
    if (timeMode == 0 && noSteps < 0) { return 0; }
    if (timeMode == 1 && noSteps < -1) { return 0; } // actually unneeded but ok.
}
