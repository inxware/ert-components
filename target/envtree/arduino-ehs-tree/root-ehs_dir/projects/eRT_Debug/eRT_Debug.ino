#include <Arduino.h>
#include <eRT.h>

void setup() {
  // start serial for debugging
  Serial.begin(9600);
  //while (!Serial);    // wait for the serial to connect before starting ehs
  eRT_wifi(NULL, NULL); // read baked-in credentials if exist 
  eRT_setup();
}

void loop() {
  eRT_loop();
}
