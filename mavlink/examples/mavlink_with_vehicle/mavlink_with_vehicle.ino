// Test code for the mavlink interface stuff.  Tested on a Teensy
//      Teensy
//          USB Port (Used for Debug)
//          Serial1 (Connection to autopilot)
//         
// Basically stuff from https://discuss.ardupilot.org/t/mavlink-and-arduino-step-by-step/25566
//
// Connect to the vehicle's autopilot via Mission Planner and the packets from the Teensy should
// route through the vehicle's mavlink connection.  Use the debug output (Serial) to view information
// coming from the vehicle through to the Teensy

#include <mavlink.h> 

// Hardware ports
COMMS_MAVLINK mavlink(Serial1);

// Global variables
unsigned long _1Hz_timer = 0;
uint16_t _counter = 0;

void setup()
{
  // Initialise the UART ports
  Serial1.begin(115200);   // Port the autopilot is connected to
  
  Serial.begin(115200);    // Debug port.  To get debug messages from the mavlink library on a different port,
                           // you'll have to modify the library header mavlink.h (defaults to Serial)
  Serial.print("MAVLINK Library Test Program\n");

  // LEDs
  pinMode(LED_BUILTIN,OUTPUT);

  // Set up the vehicles
  mavlink.init();
  mavlink._debug = 0;

  // Request data streams
  mavlink.request_stream(MAV_DATA_STREAM_EXTRA2,      0x0A);
  mavlink.request_stream(MAV_DATA_STREAM_EXTRA3,      0x0A);
  mavlink.request_stream(MAV_DATA_STREAM_RC_CHANNELS, 0x0A);

}

void loop()
{
  // Handle any data we're being sent
  mavlink.update();

  // Send a heartbeat at 1 Hz
  if (millis() - _1Hz_timer > 1UL * 1000)
  {
    _1Hz_timer = millis();
    
    digitalWrite(LED_BUILTIN,!digitalRead(LED_BUILTIN));

    char buf[50];
    sprintf(buf, "Counter is at %3d", _counter++);
     
    mavlink.send_heartbeat();
    mavlink.send_string(buf,sizeof(buf));
        
    if (mavlink.vehicle_connected()) {
           
      sprintf(buf, "        RC1: %4u [ us ]\n", mavlink.RCOU(1)      ); Serial.print(buf);
      sprintf(buf, "        RC2: %4u [ us ]\n", mavlink.RCOU(2)      ); Serial.print(buf);
      sprintf(buf, "        RC3: %4u [ us ]\n", mavlink.RCOU(3)      ); Serial.print(buf);
      sprintf(buf, "        RC4: %4u [ us ]\n", mavlink.RCOU(4)      ); Serial.print(buf);
      sprintf(buf, "        RC5: %4u [ us ]\n", mavlink.RCOU(5)      ); Serial.print(buf);
      sprintf(buf, "        RC6: %4u [ us ]\n", mavlink.RCOU(6)      ); Serial.print(buf);
      sprintf(buf, "        RC7: %4u [ us ]\n", mavlink.RCOU(7)      ); Serial.print(buf);
      sprintf(buf, "        RC8: %4u [ us ]\n", mavlink.RCOU(8)      ); Serial.print(buf);
     
    } else {
      
    }
  }
}
