// Test code for the mavlink interface stuff
// Basically stuff from https://discuss.ardupilot.org/t/mavlink-and-arduino-step-by-step/25566
//
// Connect to the serial port via Mission Planner (115200 baud) and it should connect and send parameters

#include <mavlink.h>

// Hardware ports
COMMS_MAVLINK mavlink(Serial);

// Global variables
unsigned long  _1Hz_timer = 0;
unsigned long _10Hz_timer = 0;
uint16_t _counter = 0;
const float DTR = 0.0174533;

void setup() {
  // Initialise the UART ports
  Serial.begin(115200);  // Open the mavlink port

  // LEDs
  pinMode(LED_BUILTIN, OUTPUT);

  // Set up the mavlink connection
  mavlink.init();

  // Set the ID for the vehicle
  // Everything in the mavlink library is set to use the computer IDs
  // as it was developed to be a companion computer (rather than a vehicle)
  // as used here
  mavlink.computer.system_id = 2;
  mavlink.computer.component_id = 1;

}

void loop() {
  // Handle any data we're being sent
  mavlink.update();

  // Send attitude data at 10 Hz
  if (millis() - _10Hz_timer > 100) {
    // Update timer
    _10Hz_timer = millis();
    _counter++;

    // Send VFR Message
    send_VFR();

    // Send Attitude Information
    send_attitude();

  }

  // Send a heartbeat at 1 Hz
  if (millis() - _1Hz_timer > 1UL * 1000) {
    // Update timer
    _1Hz_timer = millis();

    // Update LED
    digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));

    // Send a heartbeat
    mavlink.send_heartbeat();

  }
}


void send_VFR(void) {
  // Allocate msg variable
  mavlink_message_t msg;

  // Collect VFR Information
  float airspeed    = 10.0f + frandf();
  float groundspeed = 10.0f + frandf();
  float heading     = (float) _counter / 100.0f;
  float throttle    = fmodf((float) _counter,100.0f);
  float alt         = 100.0f + frandf()/10.0f;  // Altitude and climb rate for the Mission Planner HUD
  float climb       =   1.5f + frandf()/100.0f; // must come from somewhere else as these don't work...

  // Pack and broadcast
  mavlink_msg_vfr_hud_pack(mavlink.computer.system_id, mavlink.computer.component_id, &msg,
                           airspeed, groundspeed, heading, throttle, alt, climb);
  mavlink.broadcast(msg);

  return;
}

void send_attitude() {
  // Allocate msg variable
  mavlink_message_t msg;

  // Collect attitude information
  uint32_t time_boot_ms = millis();
  float roll            =  0.0f*DTR + 1.0f*DTR * frandf();
  float pitch           = 10.0f*DTR + 1.0f*DTR*frandf();
  float yaw             = (float) _counter/ 100.0f;
  float rollspeed       =  0.0f*DTR + 1.0f*DTR * frandf();
  float pitchspeed      =  0.0f*DTR + 1.0f*DTR * frandf();
  float yawspeed        =  0.0f*DTR + 1.0f*DTR * frandf();

  // Pack and broadcast
  mavlink_msg_attitude_pack(mavlink.computer.system_id, mavlink.computer.component_id, &msg,
                            time_boot_ms, roll, pitch, yaw, rollspeed, pitchspeed, yawspeed);
  mavlink.broadcast(msg);

  return;

}

float frandf() {
  // Generates a random float between -1 and 1
  float num = (float)rand()/(float)2147483647;
  num = num*2.0f-1.0f;
  return (num);
}
