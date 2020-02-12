#define noMot 3

struct stepper {
  const int dirPin;
  const int stepPin;
  int delta;
  int curDelta;
}; typedef stepper stepper;

stepper mot1 = {.dirPin=2, .stepPin=3, .delta=0, .curDelta=0,};
stepper mot2 = {.dirPin=4, .stepPin=5, .delta=0, .curDelta=0,};
stepper mot3 = {.dirPin=6, .stepPin=7, .delta=0, .curDelta=0,};

stepper motList[noMot] = {mot1, mot2, mot3};

void setup() {
  Serial.begin(9600);
  
  int i; // Initialising pins
  for (i=0; i<noMot; i++) {pinMode(motList[i].dirPin, OUTPUT); pinMode(motList[i].stepPin, OUTPUT);}

  digitalWrite(mot1.dirPin, HIGH);
}

void loop() {
  int i;

  for (i=20; i< 255; i+=200) {
    analogWrite(mot1.stepPin, i);
    Serial.println(i);
    delay(5000);
  }  
  /*
  // Pulse Setup Phase
  updateDelta()
  for (i=0)
  */
  /*
  for (i=0; i<200; i++) {
    digitalWrite(mot1.dirPin, LOW);
    
    digitalWrite(mot1.stepPin, HIGH);
    delayMicroseconds(500);
    digitalWrite(mot1.stepPin, LOW);
    delayMicroseconds(1000);
  }
  */
}
