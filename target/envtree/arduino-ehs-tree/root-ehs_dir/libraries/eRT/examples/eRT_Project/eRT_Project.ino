
#include <eRT.h>

// Specify you WiFi credentials.
char ssid[] = "YourSSID";        // your network SSID (name)
char pass[] = "YourPassword";    // your network password (use for WPA, or use as key for WEP)

void setup() {
  // Start serial for debugging.
  Serial.begin(9600);
  
  // Wait for the serial to connect before starting eRT, so you can check your device IP.
  // Make sure to open 'Serial Monitor' after uploading, or comment this out.
  while (!Serial);

  eRT_wifi(ssid, pass);
  eRT_setup();
}

void loop() {
  eRT_loop();
}
