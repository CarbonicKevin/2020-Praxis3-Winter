#include "motorLibrary.h"
#include <math.h>
#include <Wire.h>
//#include <SoftwareSerial.h>
// #include "ISR_LmtSwitch.h" //keeping all in this file for now.

#define iRX 8
#define iTX 9
//SoftwareSerial iSerial(iRX, iTX);

/* Set up slave motor objects */

slave dSlave1    = {.addr=1, .dely=1000, .noSteps=-1, .timeMode=1, .dir=0, .stpSize=4};
slave dSlave2    = {.addr=2, .dely=1000, .noSteps=-1, .timeMode=1, .dir=0, .stpSize=4};
slave dSlave3    = {.addr=3, .dely=1000, .noSteps=-1, .timeMode=1, .dir=0, .stpSize=4};
slave eeSlave    = {.addr=4, .dely=1000, .noSteps=-1, .timeMode=1, .dir=0, .stpSize=2};
slave  zSlave    = {.addr=5, .dely=1000, .noSteps=-1, .timeMode=1, .dir=0, .stpSize=1};

leadMotor eeMot  = {.stepAng=1.8, .lead=4, .totSteps=0, .motSlave=eeSlave};
leadMotor  zMot  = {.stepAng=1.8, .lead=4, .totSteps=0,  .motSlave=zSlave};

driveMotor d1Mot = {.motVel=0, .motAngle=(       0.00)*PI, .motSlave=dSlave1};
driveMotor d2Mot = {.motVel=0, .motAngle=(120.0/180.0)*PI, .motSlave=dSlave2};
driveMotor d3Mot = {.motVel=0, .motAngle=(240.0/180.0)*PI, .motSlave=dSlave3};

vector V         = {.deg=0, .mag=0, .rot=0};
base kiwi        = {.V=V, .m1=d1Mot, .m2=d2Mot, .m3=d3Mot};
String uInput;

/* Set up interrupt objects */

/*
#define PCINT_Z_FCN InterruptZaxis
#define PCINT_EF1_FCN InterruptEffBd
#define PCINT_EF2_FCN InterruptEffFd
#define PCINT_EFX_FCN InterruptX
#define PCINT_EFY_FCN InterruptY
#define PCINT_EFZ_FCN InterruptZ
*/

// when Interrupt_Flag set, there was an interrupt on D2 through D13
volatile byte Interrupt_Flag;

//for timing
unsigned long timeMillis;
boolean timingFlag = false;

byte interrupt_pin;
int prev_state[6] = {0,0,0,0,0,0};

// for testing
byte pin = 2;

void setup() {
    Wire.begin();
    Serial.begin(9600);
    //iSerial.begin(9600);
    Serial.println("hello.");

    // setup for interrupts
    // replace with pins 2-7 later
    pinMode(pin, INPUT_PULLUP); // 

    //PCMSK2 |= bit (PCINT16); // Pin D0
    //PCMSK2 |= bit (PCINT17); // Pin D1
    PCMSK2 |= bit (PCINT18); // Pin D02
    PCMSK2 |= bit (PCINT19); // Pin D03
    PCMSK2 |= bit (PCINT20); // Pin D04
    PCMSK2 |= bit (PCINT21); // Pin D05
    PCMSK2 |= bit (PCINT22); // Pin D06
    PCMSK2 |= bit (PCINT23); // Pin D07
    PCIFR  |= bit (PCIF2);   // clear any outstanding interrupts
    PCICR  |= bit (PCIE2);   // enable pin change interrupts for D00 to D07

    //pciSetup(pin); //
}

void loop() {
    zMot.moveLead(20,0);
    zMot.moveLead(-20,0);
    Serial.println("pin: " + String(digitalRead(pin)));

    if (Interrupt_Flag == 1) {
            Interrupt_Flag = 0;
            
            //check for the bit that caused the interrupt
            interrupt_pin = checkBit();
            
            if (interrupt_pin <=7 && interrupt_pin >= 2) {
              Serial.print("Interrupt called = ");
              Serial.println(interrupt_pin);
              
              //enable timing
              timingFlag = true;
              //time the LED turned ON
              timeMillis = millis();
    
               //are we timing and has the interval finished?
              if (timingFlag == true && millis() - timeMillis >= 1000 ){
                //disable timing
                timingFlag = false;
              }
            } else { 
              Serial.println("error: pin not set up for interrupts"); 
            }
    }
    
    /* 
    if (iSerial.available()) { // Parse through input
        uInput = iSerial.readString(); // Read Input
        Serial.println(uInput);
        uInMov(uInput);
    }
    */

} // loop

void uInMov(String uInput) {
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

/*
void pciSetup(byte pin) {
    *digitalPinToPCMSK(pin) |= bit (digitalPinToPCMSKbit(pin));  // enable pin
    PCIFR |= bit (digitalPinToPCICRbit(pin)); // clear any outstanding interrupt
    PCICR |= bit (digitalPinToPCICRbit(pin)); // enable interrupt for the group
    Serial.println("enabled interrupts on pin " + String(pin));
}
*/

/* Functions for Interrupt */

//TODO: complete interrupt functions for the FSM

void InterruptZaxis() {
    // stop z-axis motion
    zMot.stop();
    // wait, decide next step, proceed with that.
}

void InterruptEffBd() {
    // stop eff backwards motion
    eeMot.stop();
    // ...wait, decide next step, proceed with that.

}

void InterruptEffFd() {
    // stop eff forwards motion
    eeMot.stop();
    // ...wait, decide next step, proceed with that.
    
}
void InterruptX() {
    // stop eff x adjustment - controlled by drive.
    kiwi.V.deg =  0; kiwi.V.mag = 0; kiwi.V.rot = 0;
    kiwi.calcMotVelo();
    kiwi.updateAll();
    // ...wait, decide next step, proceed with that.
    
}
void InterruptY() {
    // stop eff y adjustment
    kiwi.V.deg =  0; kiwi.V.mag = 0; kiwi.V.rot = 0;
    kiwi.calcMotVelo();
    kiwi.updateAll();
    // ...wait, decide next step, proceed with that.
}
void InterruptZ() {
    // stop eff z adjustment
    kiwi.V.deg =  0; kiwi.V.mag = 0; kiwi.V.rot = 0;
    kiwi.calcMotVelo();
    kiwi.updateAll();
    // ...wait, decide next step, proceed with that.
}

byte checkBit() {
    //check PIND D02-D07
    for (byte x = 2; x <= 7; x++) {
        //check for LOW pin
        byte value = PIND;
        if (bitRead(value, x) == 0) {
            return x;
        }
    }
}

ISR (PCINT2_vect) { //D02-07
  Serial.println("--------------------ISR!");
  if      (interrupt_pin == 2) { InterruptZaxis(); }
  else if (interrupt_pin == 3) { InterruptEffBd(); }
  else if (interrupt_pin == 4) { InterruptEffFd(); }
  else if (interrupt_pin == 5) {     InterruptX(); }
  else if (interrupt_pin == 6) {     InterruptY(); }
  else if (interrupt_pin == 7) {     InterruptZ(); }
  Interrupt_Flag = 1;
}
