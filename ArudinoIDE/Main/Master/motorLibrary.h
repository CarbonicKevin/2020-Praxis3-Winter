#include <Wire.h>

#define noMot 3

#define minDely 2000

struct vector {
    int deg;
    int mag;
    int rot;
}; typedef vector vector;

struct slave {     // Structure whose contents will be sent to the slave from master
    int      addr;  // Address of Slave
    int      dely;  // >= 2000 microseconds
    int   noSteps;  // 0:Stop; >0:More; Number of steps to turn if timeMode=0, Time to turn (in ms) if timeMode=1; if timeMode = 1 and noSteps = -1, then inf turn
    bool timeMode;  // noSteps will be interepreted as time required if true
    int       dir;  // Direction of Motor; 0 or 1. Defining 0 as lturn, and 1 as rturn
    int   stpSize;  // 1, 2, 4, 8

    bool updateSlave(bool waitForReply) { // master sends information to slave.
        
        Wire.beginTransmission(addr);
        Wire.write((String(dely)+','+String(noSteps)+','+String(timeMode)+','+String(dir)+','+String(stpSize)+"NONE").c_str());
        Wire.endTransmission(1); // send stop msg after transmission
        delay(10);
        if (waitForReply) {
          char buf = '0';
          while (buf=='0'){
            if (Wire.requestFrom(addr, 1)) {
              // requests 1 byte from slave, and stops message after.
              buf = Wire.read();
            }
         }   
       } // TODO: add a thing (time limit, etc.) to prevent master from getting stuck here.
    }
}; typedef slave slave;

struct leadMotor {
    // Code for motors operating Lead Screws (z-axis and eff)
    // calling modeLead and providing distance -> moves a certain length
    // calling startup and stop -> startup mode and stop motor
    float  stepAng;  // angle per step of motor.
    float     lead;  // lead of the lead screw.
    int   totSteps;  // count number of steps taken in total.
    slave motSlave;  // slave struct, info to be sent from master to slave.

    void moveLead(float dist, bool waitForReply) {
        // fcn to rotate the lead screw.
        // if waitForReply is true, wait for totSteps to update before exit.
        int steps         = (dist*360*motSlave.stpSize)/(lead*stepAng);
        motSlave.timeMode = 0;
        motSlave.noSteps  = abs(steps);
        if (dist<0) {motSlave.dir=1;} else {motSlave.dir=0;}
        motSlave.updateSlave(waitForReply);
        totSteps+=abs(steps);
    }

    void startup() {
        // Startup raise motor a little
        motSlave.timeMode =     0;
        motSlave.noSteps  =   400;
        motSlave.dir      =     0;
        motSlave.dely     =  minDely;
        motSlave.updateSlave(1);

        // Then set the motor to move down constantly
        motSlave.timeMode =     1;
        motSlave.noSteps  =    -1;
        motSlave.dir      =     0;
        motSlave.dely     =  minDely;
        motSlave.updateSlave(0);

        totSteps=0;
    }
    void stop() {
        motSlave.timeMode =    0;
        motSlave.noSteps  =    0;
        motSlave.dir      =    0;
        motSlave.dely     =    0;
        motSlave.updateSlave(0);
    }

}; typedef leadMotor leadMotor;

struct driveMotor {
    float motVel; //
    float motAngle; // can be 0, 60, 120. used to specify which motor.
    slave motSlave; //
}; typedef driveMotor driveMotor;

struct base {
    vector V;
    driveMotor m1;
    driveMotor m2;
    driveMotor m3;

    void calcMotVelo() {
        // Function to calculate and update motor slave properties based on the set vector V
        // Changes m1, m2, m3; .dir and .dely, .noSteps and .timeMode is not changed
        driveMotor motP[noMot] = {m1, m2, m3};
        float deg = V.deg * PI/180;
        float xVel = V.mag*cos(deg); float yVel = V.mag*sin(deg);
        int i; float maxVel; float normal;

        // Calculate velocity, save the max abs velocity and a list of velocities to work on
        maxVel = 0.0;
        for (i=0; i<noMot; i++) {
            motP[i].motVel = -sin((motP[i]).motAngle) * xVel + cos((motP[i]).motAngle) * yVel + V.rot; // Calculating motor speed
            maxVel = max(abs(motP[i].motVel), abs(maxVel));
        }
        // Calculate 'delta' the multiple of the delay required, update slave dir and dely vars
        if (maxVel == 0) {for (i=0; i<noMot; i++) {motP[i].motSlave.dely = 0.0; motP[i].motSlave.dir =0;}}
        else {
            for (i=0; i<noMot; i++) {
                normal = (abs(motP[i].motVel)/maxVel);
                if (normal < 0.0001) {motP[i].motSlave.dely = 0;}
                else {motP[i].motSlave.dely = abs((maxVel/abs(motP[i].motVel))*minDely*(255/V.mag)*(255/V.rot));} // 1/(speed/maximum motor speed) * delay * (max capable speed/desired speed)
                if (motP[i].motVel >= 0) {motP[i].motSlave.dir = 0;}
                else                     {motP[i].motSlave.dir = 1;}
            }
        }
    }

}; typedef base base;
