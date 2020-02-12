#include <AFMotor.h>
#include <SoftwareSerial.h>

#define TX A0
#define RX A1

struct vector {
  int deg; // in rad
  int mag;
  int rot;
}; typedef vector vector;

struct motor {
  int motVel;
  float motAngle;
  AF_DCMotor mot;
}; typedef motor motor;

motor mot1 = {.motVel = 0, .motAngle =  (       0.00)*PI , .mot=AF_DCMotor(1)};
motor mot2 = {.motVel = 0, .motAngle =  (120.0/180.0)*PI , .mot=AF_DCMotor(2)};
motor mot3 = {.motVel = 0, .motAngle =  (240.0/180.0)*PI , .mot=AF_DCMotor(3)};
motor motList[] = {mot1, mot2, mot3};

String uInput;

SoftwareSerial iSerial(RX, TX);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600); // set up Serial library at 9600 bps
  iSerial.begin(9600);
}

void loop() {
  if (iSerial.available()) { // Parse through input
    uInput = iSerial.readString(); // Read Input
    Serial.println(uInput);
    mov(uInput);
  }
}

void mov(String uInput) {
  vector uVector = {.deg=0, .mag=0, .rot=0};
  
  char *token;
  char buf[200]; char split[] = ",";
  String listStr[3];

  int i;
  
  // Function to parse through the string and package input into vector

  // PARSING --------------------------------------------------------------------------------------------
  // Parsing done with help from: https://www.tutorialspoint.com/c_standard_library/c_function_strtok.htm
  // and https://www.arduino.cc/reference/en/language/variables/data-types/string/functions/tochararray/
  // Creates a vector object uVector
  if      (uInput == "x") {uVector.deg =     0; uVector.mag =   0; uVector.rot =  0.0;} 
  else if (uInput == "w") {uVector.deg =   0.0; uVector.mag = 255; uVector.rot =  0.0;} 
  else if (uInput == "s") {uVector.deg = 180.0; uVector.mag = 255; uVector.rot =  0.0;} 
  else if (uInput == "a") {uVector.deg =  90.0; uVector.mag = 255; uVector.rot =  0.0;} 
  else if (uInput == "d") {uVector.deg = 270.0; uVector.mag = 255; uVector.rot =  0.0;} 
  else if (uInput == "q") {uVector.deg =   0.0; uVector.mag =   0; uVector.rot =  1.0;}
  else if (uInput == "e") {uVector.deg =   0.0; uVector.mag =   0; uVector.rot = -1.0;}
  else {
    uInput.toCharArray(buf, 200); // Convert input to char array
    
    token = strtok(buf, split);
    i = 0;
    while (token != NULL) {
      //printf("%s\n", token);
      listStr[i] = String(token);
      token = strtok(NULL, split);
      i++;
    }
    if (i != 3) {
      Serial.println("Failed to parse: " + String(i));
      return;
    }
    
    uVector.deg=listStr[0].toInt(); uVector.mag=listStr[1].toInt(); uVector.rot=listStr[2].toInt(); // creating vector
    Serial.println("Deg: " + listStr[0] + " Mag: " + listStr[1]+ " Rot: " +listStr[2]); // Displaying vector for debug
  }
  runMotVelo(uVector, motList);
} 

    
void runMotVelo(vector V, motor* motP) {
  // Calculates and runs motors
  //Mathmatics from http://robocup.mi.fu-berlin.de/buch/omnidrive.pdf

  float deg = V.deg * PI/180;
  int xVel = V.mag*cos(deg); int yVel = V.mag*sin(deg);
  Serial.println("Velocity: " +  String(xVel) + ", " + String(yVel));
  int i; int tempVel;

  for (i=0; i<3; i++) {
    motP[i].motVel = -sin((motP[i]).motAngle) * xVel + cos((motP[i]).motAngle) * yVel + 255*V.rot; // Calculating motor speed
    Serial.println(motP[i].motAngle);
    motP[i].mot.setSpeed(abs(floor(motP[i].motVel)));  // Setting Motor Speed
    Serial.println("Motor " + String(i) + " Velocity: " + String((motP[i]).motVel)); // Printing calculated motor speeds for debug
  }

  for (i=0; i<3; i++) {
    tempVel = floor(motP[i].motVel);
    if      (tempVel <  0) {motP[i].mot.run(BACKWARD);}
    else if (tempVel >  0) {motP[i].mot.run(FORWARD);}
    else if (tempVel == 0) {motP[i].mot.run(RELEASE);}
    else    {Serial.println("Could not start motor");} // Debug Line
  }
  
}
