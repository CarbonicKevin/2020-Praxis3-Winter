#include <pins_arduino.h>
/* Set up interrupt objects */

#define PCINT_PIN7 7
#define PCINT_MODE FALLING
#define PCINT_Z_FCN InterruptZaxis
#define PCINT_EF1_FCN InterruptEffBd
#define PCINT_EF2_FCN InterruptEffFd
#define PCINT_EFX_FCN InterruptX
#define PCINT_EFY_FCN InterruptY
#define PCINT_EFZ_FCN InterruptZ

// when Flag set, there was an interrupt on D2 through D13
volatile byte Flag;

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

ISR (PCINT2_vect) //D02-07
{
  //get out quick
  Serial.println("ISR!");
  
  Flag = 1;
}
