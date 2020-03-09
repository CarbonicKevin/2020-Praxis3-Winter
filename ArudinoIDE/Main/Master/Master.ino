#include "motorLibrary.h"
#include <math.h>
#include <Wire.h>
#include <SoftwareSerial.h>

#define iRX 11
#define iTX 12

slave dSlave1    = {.addr=1, .dely=1000, .noSteps=-1, .timeMode=1, .dir=0, .stpSize=4};
slave dSlave2    = {.addr=2, .dely=1000, .noSteps=-1, .timeMode=1, .dir=0, .stpSize=4};
slave dSlave3    = {.addr=3, .dely=1000, .noSteps=-1, .timeMode=1, .dir=0, .stpSize=4};
slave eeSlave    = {.addr=4, .dely=1000, .noSteps=-1, .timeMode=1, .dir=0, .stpSize=2};
slave  zSlave    = {.addr=5, .dely=1000, .noSteps=-1, .timeMode=1, .dir=0, .stpSize=1};

leadMotor eeMot  = {.stepAng=1.8, .lead=4, .totSteps=0, .motSlave=eeSlave};
leadMotor  zMot  = {.stepAng=1.8, .lead=4, .totSteps=0, .motSlave=zSlave};

driveMotor d1Mot = {.motVel=0, .motAngle=(       0.00)*PI, .motSlave=dSlave1};
driveMotor d2Mot = {.motVel=0, .motAngle=(120.0/180.0)*PI, .motSlave=dSlave2};
driveMotor d3Mot = {.motVel=0, .motAngle=(240.0/180.0)*PI, .motSlave=dSlave3};

vector V         = {.deg=0, .mag=0, .rot=0};

base kiwi        = {.V=V, .m1=d1Mot, .m2=d2Mot, .m3=d3Mot};

SoftwareSerial iSerial(iRX, iTX);

void setup() {
    Wire.begin();
    Serial.begin(9600);
    iSerial.begin(9600);
}

void loop() {
    char uInput;
    if (iSerial.available()) { // Parse through input
        uInput = iSerial.read(); // Read Input
        Serial.println(uInput);
        uInMov(uInput);
    }
}

void uInMov(char uInput) {
    if      (uInput == 'w') {kiwi.V.deg =   0.0; kiwi.V.mag = 255; kiwi.V.rot =    0.0;}
    else if (uInput == 's') {kiwi.V.deg = 180.0; kiwi.V.mag = 255; kiwi.V.rot =    0.0;}
    else if (uInput == 'a') {kiwi.V.deg =  90.0; kiwi.V.mag = 255; kiwi.V.rot =    0.0;}
    else if (uInput == 'd') {kiwi.V.deg = 270.0; kiwi.V.mag = 255; kiwi.V.rot =    0.0;}
    else if (uInput == 'q') {kiwi.V.deg =   0.0; kiwi.V.mag =   0; kiwi.V.rot =  255.0;}
    else if (uInput == 'e') {kiwi.V.deg =   0.0; kiwi.V.mag =   0; kiwi.V.rot = -255.0;}
    else                    {kiwi.V.deg =     0; kiwi.V.mag =   0; kiwi.V.rot =    0.0;}
    kiwi.calcMotVelo();
    kiwi.updateAll();
}