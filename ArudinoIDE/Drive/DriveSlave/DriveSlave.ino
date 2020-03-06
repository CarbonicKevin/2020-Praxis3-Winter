#include <SoftwareSerial.h>
#define  dirPin 12
#define stepPin 11

#define MS1  8
#define MS2  9
#define MS3 10

String uInput;

void setup() {
  pinMode(dirPin, OUTPUT);
  pinMode(stepPin, OUTPUT);
  
  pinMode(MS1, OUTPUT);
  pinMode(MS2, OUTPUT);
  pinMode(MS3, OUTPUT);

  digitalWrite(MS1, HIGH);
  digitalWrite(MS2, HIGH);
  digitalWrite(MS3,  LOW);
  
  Serial.begin(9600);
}

void loop() {
  if (Serial.available()) { // Parse through input
    uInput = Serial.readString(); // Read Input
    Serial.println(uInput);
    mov(uInput);
  }
}

void mov(String uInput) {
    char *token; int i;
    char buf[200]; char split[] = ",";
    String listStr[2]; float dely;
    
    uInput.toCharArray(buf, 200); // Convert input to char array
    
    token = strtok(buf, split);
    i = 0;
    while (token != NULL) {
      //printf("%s\n", token);
      listStr[i] = String(token);
      token = strtok(NULL, split);
      i++;
    }
    if (i != 2) {
      Serial.println("Failed to parse: " + String(i));
      return;
    }

    digitalWrite(dirPin, listStr[1].toInt()); // Writing to dir pin
    dely = listStr[0].toFloat();
    if (dely == 0) {return;}
    while (1) {
      digitalWrite(stepPin, HIGH);
      delayMicroseconds(round(dely/2));
      digitalWrite(stepPin, LOW);
      delayMicroseconds(round(dely/2));
      if (Serial.available()) {break;}
    }
    
}
