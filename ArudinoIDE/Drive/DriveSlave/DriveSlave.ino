#include <SoftwareSerial.h>
#define  dirPin 3
#define stepPin 2

#define MS1 4
#define MS2 5
#define MS3 6

#define RX 9
#define TX 8

String uInput;

void setup() {
  pinMode(dirPin, OUTPUT);
  pinMode(stepPin, OUTPUT);
  
  pinMode(MS1, OUTPUT);
  pinMode(MS2, OUTPUT);
  pinMode(MS3, OUTPUT);
  
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
    String listStr[2]; int dely;
    
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
    dely = listStr[0].toInt();
    if (dely == 0) {return;}
    while (1) {
      digitalWrite(stepPin, HIGH);
      delayMicroseconds(1000);
      digitalWrite(stepPin, LOW);
      delayMicroseconds(dely);
      if (Serial.available()) {break;}
    }
    
}
