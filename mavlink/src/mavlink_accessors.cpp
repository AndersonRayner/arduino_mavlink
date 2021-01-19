
#include "mavlink.h"

// Connection Health
bool COMMS_MAVLINK::vehicle_connected(void) { return ((millis() < _t_heartbeat_vehicle + 3000) ? (1) : (0)); }
bool COMMS_MAVLINK::gcs_connected(void) { return ((millis() < _t_heartbeat_GCS + 3000) ? (1) : (0)); }

// Rangefinder Range
bool COMMS_MAVLINK::rangefinder_healthy(void) { return ((millis() < _t_rangefinder + 500) ? (1) : (0)); }
float COMMS_MAVLINK::rangefinder(void) { return (_range); }

// Servo Values
bool COMMS_MAVLINK::RCOU_healthy(void) { return ((millis() < _t_RCOU + 500) ? (1) : (0)); }
uint16_t COMMS_MAVLINK::RCOU(uint8_t servo_number) {
  // Check for a valid servo number
  if (servo_number == 0) { return (0); }
  if (servo_number > 16) { return (0); }

  // Get the RC out channel value
  uint16_t rc_out = _RCOU.rc[servo_number-1];
  return (rc_out);
  
}

// Local Position
bool COMMS_MAVLINK::NED_local_healthy(void) { return ((millis() < _t_NED_local + 500) ? (1) : (0)); }

float COMMS_MAVLINK::X(void)  { return (_NED.X);  }
float COMMS_MAVLINK::Y(void)  { return (_NED.Y);  }
float COMMS_MAVLINK::Z(void)  { return (_NED.Z);  }
float COMMS_MAVLINK::Vx(void) { return (_NED.Vx); }
float COMMS_MAVLINK::Vy(void) { return (_NED.Vy); }
float COMMS_MAVLINK::Vz(void) { return (_NED.Vz); }
float COMMS_MAVLINK::V(void)  { return (_NED.V);  }

// VFR_HUD
bool COMMS_MAVLINK::VFR_HUD_healthy(void) { return ((millis() < _t_VFR_HUD + 500) ? (1) : (0)); }

float    COMMS_MAVLINK::airspeed(void)    { return (_VFR_HUD.airspeed);    }
float    COMMS_MAVLINK::groundspeed(void) { return (_VFR_HUD.groundspeed); }
float    COMMS_MAVLINK::alt(void)         { return (_VFR_HUD.alt);         }
float    COMMS_MAVLINK::climb(void)       { return (_VFR_HUD.climb);       }
int16_t  COMMS_MAVLINK::heading(void)     { return (_VFR_HUD.heading);     }
uint16_t COMMS_MAVLINK::throttle(void)    { return (_VFR_HUD.throttle);    }
