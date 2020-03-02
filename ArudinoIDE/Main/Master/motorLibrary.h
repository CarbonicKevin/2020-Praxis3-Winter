#include <Wire.h>

#define noMot 3

#define minDely 2000

struct vector {
    int deg;
    int mag;
    int rot;
}; typedef vector vector;

struct slave {     // Structure whose contents will be sent to the slave
    int     addr;  // Address of Slave
    int     dely;  // >= 2000 microseconds
    int  noSteps;  // 0:Stop; >0:More; Number of steps to turn; if timeMode = 1; and noSteps = -1, then inf turn
    int timeMode;  // noSteps will be interepreted as time required if true
    int      dir;  // Direction of Motor; 0 or 1. Defining 0 as lturn, and 1 as rturn
    int  stpSize;  // 1, 2, 4, 8

    bool updateSlave(bool waitForReply) {
        char msg[100];
        (String(dely)+','+String(noSteps)+','+String(dir)+','+String(stpSize)).toCharArray(msg, 100);
        Wire.beginTransmission(addr);
        Wire.write(msg);
        Wire.endTransmission(1);
        if (waitForReply) {
            Wire.requestFrom(addr, 1, 1);
            while (1) {
                if (Wire.available() > 0) {Wire.read(); return(1);} // waiting for any response from slave
            }
        } return(1);
    }
}; typedef slave slave;

struct leadMotor {
    // Code for Lead Screws (z-axis and eef)
    // call modeLead with a distance to move a certain length
    // call startup and stop for startup mode and stop motor
    float stepAng;  // angle per step of motor
    float    lead;  // lead of the lead screw  
    int  totSteps;  
    slave motSlave;

    void moveLead(float dist, bool waitForReply) {
        int steps         = (dist*360)/(lead*stepAng);
        motSlave.timeMode = 0;
        motSlave.noSteps  = abs(steps);
        motSlave.dir      = abs(steps)/steps;
        motSlave.updateSlave(waitForReply);
    }

    void startup() {
        // Startup raise motor a little
        motSlave.timeMode =     0;
        motSlave.noSteps  =   100;
        motSlave.dir      =     0;
        motSlave.dely     =  minDely;
        motSlave.updateSlave(1);

        // Then set the motor to move down constantly
        motSlave.timeMode =     1;
        motSlave.noSteps  =    -1;
        motSlave.dir      =     0;
        motSlave.dely     =  minDely;
        motSlave.updateSlave(0);
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
    float motVel;
    float motAngle;
    slave motSlave;
}; typedef driveMotor driveMotor;

struct base {
    vector V;
    driveMotor m1;
    driveMotor m2;
    driveMotor m3;

    void calcMotVelo() {
        // Function to calculate and update motor slave properties based on the set vector V
        // Chagnes m1, m2, m3; .dir and .dely, .noSteps and .timeMode is not changed
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
