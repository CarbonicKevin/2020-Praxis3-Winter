#include <Wire.h>
#define MS1  8
#define MS2  9
#define MS3 10

#define stepPin 11
#define dirPin  12

#define addr 2

// info to be received from master
int      dely = 0;
int   noSteps = 0;
bool timeMode = 0;
int       dir = 0;
int   stpSize = 0;
bool      req = 1;
int newInstruct = 0;

void setup() {
    pinMode(stepPin, OUTPUT);
    pinMode(dirPin,  OUTPUT);
    pinMode(MS1,  OUTPUT);
    pinMode(MS2,  OUTPUT);
    pinMode(MS3,  OUTPUT);

    Wire.begin(addr);                   // join i2c bus with Slave address
    Wire.onReceive(rec_handler);        // register receive event
    Wire.onRequest(req_handler);        // register request event

    Serial.begin(9600);
}

void loop() {
    if (newInstruct == 1) {drive(); newInstruct=0;}
    req = 1;
}
void drive() {
    digitalWrite(dirPin, dir);
    // set step size pins on the A4988
    if      (stpSize==1) {digitalWrite(MS1,  LOW); digitalWrite(MS2,  LOW); digitalWrite(MS3,  LOW);}
    else if (stpSize==2) {digitalWrite(MS1, HIGH); digitalWrite(MS2,  LOW); digitalWrite(MS3,  LOW);}
    else if (stpSize==4) {digitalWrite(MS1,  LOW); digitalWrite(MS2, HIGH); digitalWrite(MS3,  LOW);}
    else if (stpSize==8) {digitalWrite(MS1, HIGH); digitalWrite(MS2, HIGH); digitalWrite(MS3,  LOW);}
    req = 0;
    if (dely==0 || noSteps==0) { // non-moving state
        digitalWrite(stepPin, LOW);
    } else if (!timeMode) {
        for (int i=0; i<noSteps; i++) {
            digitalWrite(stepPin, HIGH);
            delayMicroseconds(dely);
            digitalWrite(stepPin, LOW);
            delayMicroseconds(dely);
        }
    } else { // noSteps interpreted as time in ms
        unsigned long startTime =  millis();
        bool inf; // if infinite spin
        if (noSteps == -1) { inf = 1; } else { inf = 0; }
        while (inf || (millis() <= startTime + (unsigned long)noSteps)) {
            digitalWrite(stepPin, HIGH);
            delayMicroseconds(dely);
            digitalWrite(stepPin, LOW);
            delayMicroseconds(dely);
            if (newInstruct==1) {return;}
        }
    } req=1;
}

void rec_handler(int numBytes) {
    // executes for each slave whenever data is received from master.
    char msg[100]; String listStr[5];
    int i=0; char *token;
    while (0 < Wire.available()) {
        msg[i] = Wire.read();
        i++;
    }

    // split and extract data from msg
    token = strtok(msg, ",");
    i=0;
    while (token != NULL) {
      //printf("%s\n", token);
      if (i==4) {listStr[i] = String(int(token)/100);}
      
      listStr[i] = String(token);
      token = strtok(NULL, ",");
      i++;
    }
    dely=listStr[0].toInt(); noSteps=listStr[1].toInt(); timeMode=listStr[2].toInt(); dir=listStr[3].toInt(); stpSize=listStr[4].toInt();

    //Serial.println(String(dely)+","+String(noSteps)+","+String(timeMode)+","+String(dir)+","+String(stpSize));

    /*
    // error-checking the values received from master.
    // simple checks
    if (dely < 2000 && dely != 0) { return; }
    if (noSteps < -1) { return; }
    if (dir != 0 && dir != 1) { return; }
    if (stpSize != 1 && stpSize != 2 && stpSize != 4 && stpSize != 8) { return ; }

    // checking valid timeMode and noSteps combinations
    if (timeMode == 0 && noSteps < 0) { return; }
    if (timeMode == 1 && noSteps < -1) { return; } // actually unneeded but ok.
    */
    newInstruct = 1;
}

void req_handler(){
    if (req == 0) {Wire.write('0');}
    else          {Wire.write('1');}
}
