#include <Wire.h>

void setup() {
    Wire.begin(2);
    Wire.onRequest(req_handle);
}

void loop() {
    delay(100);
}

void req_handle() {
    Wire.write("hello:");
}
