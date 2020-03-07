#include "motorLibrary.h"
#include <math.h>

void setup() {
    slave dSlave1 = {.addr=1, .dely=2000, .noSteps=-1, .timeMode=1, .dir=0, .stpSize=1};
    slave dSlave2 = {.addr=2, .dely=2000, .noSteps=-1, .timeMode=1, .dir=0, .stpSize=1};
    slave dSlave3 = {.addr=3, .dely=2000, .noSteps=-1, .timeMode=1, .dir=0, .stpSize=1};
    slave eeSlave = {.addr=4, .dely=1000, .noSteps=-1, .timeMode=1, .dir=0, .stpSize=2};
    slave  zSlave = {.addr=5, .dely=2000, .noSteps=-1, .timeMode=1, .dir=0, .stpSize=1};

    leadMotor eeMot = {.stepAng=1.8, .lead=4, .totSteps=0, .motSlave=eeSlave};
    leadMotor  zMot = {.stepAng=1.8, .lead=4, .totSteps=0, .motSlave=zSlave};

    driveMotor d1Mot = {.};

void loop() {
}
