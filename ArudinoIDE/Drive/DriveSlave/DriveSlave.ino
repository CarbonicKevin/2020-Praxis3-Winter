#define  dirPin 3
#define stepPin 2

#define MS1 4
#define MS2 5
#define MS3 6

#define dirInput 8
#define stepInput 9

String uInput;

void setup() {
  pinMode(dirPin, OUTPUT);
  pinMode(stepPin, OUTPUT);
  
  pinMode(MS1, OUTPUT);
  pinMode(MS2, OUTPUT);
  pinMode(MS3, OUTPUT);

  pinMode(dirInput, INPUT);
  pinMode(stepInput, INPUT);
  
  Serial.begin(9600);
}
int i;
void loop() {
  digitalWrite(dirPin, digitalRead(dirInput)); // Writing to dir pin
  
  while (1) {
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(500);
    digitalWrite(stepPin, LOW);
    i = int(analogRead(stepInput)*1000/255);
    Serial.println(i);
    delayMicroseconds(i);
  }
}
