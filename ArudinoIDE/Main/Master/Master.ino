#include "motorLibrary.h"
#include <math.h>
#include <Wire.h>

slave dSlave1    = {.addr=1, .dely=2000, .noSteps=-1, .timeMode=1, .dir=0, .stpSize=8};
slave dSlave2    = {.addr=2, .dely=2000, .noSteps=-1, .timeMode=1, .dir=0, .stpSize=8};
slave dSlave3    = {.addr=3, .dely=2000, .noSteps=-1, .timeMode=1, .dir=0, .stpSize=8};
slave eeSlave    = {.addr=4, .dely=1000, .noSteps=-1, .timeMode=1, .dir=0, .stpSize=2};
slave  zSlave    = {.addr=5, .dely=1000, .noSteps=-1, .timeMode=1, .dir=0, .stpSize=1};

leadMotor eeMot  = {.stepAng=1.8, .lead=4, .totSteps=0, .motSlave=eeSlave};
leadMotor  zMot  = {.stepAng=1.8, .lead=4, .totSteps=0, .motSlave=zSlave};

driveMotor d1Mot = {.motVel=0, .motAngle=(       0.00)*PI, .motSlave=dSlave1};
driveMotor d2Mot = {.motVel=0, .motAngle=(120.0/180.0)*PI, .motSlave=dSlave2};
driveMotor d3Mot = {.motVel=0, .motAngle=(240.0/180.0)*PI, .motSlave=dSlave3};

vector V         = {.deg=0, .mag=0, .rot=0};

base kiwi        = {.V=V, .m1=d1Mot, .m2=d2Mot, .m3=d3Mot};

void setup() {
    Wire.begin();
    Serial.begin(9600);
}

void loop() {
    /*
    dSlave1.noSteps=-1; dSlave1.timeMode=1;dSlave1.dir=1;
    dSlave1.updateSlave(1);
    */

    kiwi.V.deg =   0; kiwi.V.mag = 255; kiwi.V.rot = 0;
    kiwi.calcMotVelo();
    kiwi.updateAll();
    Serial.println(kiwi.m1.motSlave.dely);
    Serial.println(kiwi.m2.motSlave.dely);
    Serial.println(kiwi.m3.motSlave.dely);
    delay(5000);

    kiwi.V.deg =  90; kiwi.V.mag = 255; kiwi.V.rot = 0;
    kiwi.calcMotVelo();
    kiwi.updateAll();
    Serial.println(kiwi.m1.motSlave.dely);
    Serial.println(kiwi.m2.motSlave.dely);
    Serial.println(kiwi.m3.motSlave.dely);
    delay(5000);

    kiwi.V.deg = 180; kiwi.V.mag = 255; kiwi.V.rot = 0;
    kiwi.calcMotVelo();
    kiwi.updateAll();
    Serial.println(kiwi.m1.motSlave.dely);
    Serial.println(kiwi.m2.motSlave.dely);
    Serial.println(kiwi.m3.motSlave.dely);
    delay(5000);

    kiwi.V.deg = 270; kiwi.V.mag = 255; kiwi.V.rot = 0;
    kiwi.calcMotVelo();
    kiwi.updateAll();
    Serial.println(kiwi.m1.motSlave.dely);
    Serial.println(kiwi.m2.motSlave.dely);
    Serial.println(kiwi.m3.motSlave.dely);
    delay(5000);
}
