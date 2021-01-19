// Test code for the mavlink interface stuff
// Basically stuff from https://discuss.ardupilot.org/t/mavlink-and-arduino-step-by-step/25566
//
// Connect to the serial port via Mission Planner (115200 baud) and it should connect and send parameters

#include <mavlink.h> 

// Hardware ports
COMMS_MAVLINK mavlink(Serial);

// Global variables
unsigned long _1Hz_timer = 0;
uint16_t _counter = 0;

void setup()
{
  // Initialise the UART ports
  Serial.begin(115200);  // Open the mavlink port

  // LEDs
  pinMode(LED_BUILTIN,OUTPUT);

  // Set up the mavlink connection
  mavlink.init();

}

void loop()
{
  // Handle any data we're being sent
  mavlink.update();

  // Send a heartbeat at 1 Hz
  if (millis() - _1Hz_timer > 1UL * 1000) {
    // Update timer
    _1Hz_timer = millis();

    // Update LED
    digitalWrite(LED_BUILTIN,!digitalRead(LED_BUILTIN));

    // Send some data and a heartbeat
    char buf[50];
    sprintf(buf, "Counter is at %3d", _counter++);
     
    mavlink.send_heartbeat();
    mavlink.send_string(buf,sizeof(buf));
  
  }
}
