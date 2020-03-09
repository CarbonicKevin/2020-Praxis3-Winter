
#include <pins_arduino.h>

// maybe look at this later 
// https://github.com/NicoHood/PinChangeInterrupt/blob/master/examples/PinChangeInterrupt_HowItWorks/PinChangeInterrupt_HowItWorks.ino

// notes:
//  - may need to change pin nums - currently using 0-5
//  

/*
struct limsw {
    int  pin;
    bool isr;
}; typedef limsw limsw;

limsw z     = {.pin = 0, .isr = ISR_z };
limsw eff_b = {.pin = 1, .isr =  };
limsw eff_f = {.pin = 2, .isr =  };
limsw eff_x = {.pin = 3, .isr =  };
limsw eff_y = {.pin = 4, .isr =  };
limsw eff_z = {.pin = 5, .isr =  };
*/ 
// lol keep that mapping tho
// but let's instead use the ISR_list to figure out which to execute.

/*
int prev_state[6];
bool ISR_list[] = {ISR_z};

void pciSetup(byte pin) {
    *digitalPinToPCMSK(pin) |= bit (digitalPinToPCMSKbit(pin));  // enable pin
    PCIFR  |= bit (digitalPinToPCICRbit(pin)); // clear any outstanding interrupt
    PCICR  |= bit (digitalPinToPCICRbit(pin)); // enable interrupt for the group
}

ISR (PCINT2_vect) { // handle pin change interrupt for D0 to D7 here
    for (int i=0; i<=5; i++) {
        if (digitalRead(i) != prev_state[i]) {
            Serial.println(digitalRead(i));     //
            Serial.println(prev_state[i]);      //
            if (prev_state[i]==0){ //trigger on rising edge 
                ISR_list[i]; // call correct ISR                
            }
        }
    }
}  

bool ISR_z() {
    zMot.stop();
}

void setup() {
    prev_state[] = {0,0,0,0,0,0};
    
    Serial.begin(9600);
    Serial.println("PinChangeTest");
    for (int i=0; i<=5; i++){
        pinMode(i, INPUT);
        digitalWrite(i, HIGH); //set pullups
    }
    
    // enable interrupts
    for (int i=0; i<=5; i++){
        pciSetup(i);
    }
}
*/
