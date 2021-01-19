
#include "mavlink.h"


void COMMS_MAVLINK::heartbeat_rx(mavlink_message_t msg) {

  // Decode message
  mavlink_heartbeat_t data;
  mavlink_msg_heartbeat_decode(&msg, &data);

  switch (msg.sysid)
  {
    case (_GCS_SYSID) :
      {
        gcs.system_id    = msg.sysid;
        gcs.component_id = msg.compid;
        gcs.type         = data.type;
        gcs.autopilot    = data.autopilot;
        gcs.mode         = data.base_mode;
        gcs.custom_mode  = data.custom_mode;
        gcs.status       = data.system_status;

        // We have a connection to the GCS so we can update that things are alive
        _t_heartbeat_GCS = millis();

        break;
      }

    default :  // Probably from the vehicle
      {
        vehicle.system_id    = msg.sysid;
        vehicle.component_id = msg.compid;
        vehicle.type         = data.type;
        vehicle.autopilot    = data.autopilot;
        vehicle.mode         = data.base_mode;
        vehicle.custom_mode  = data.custom_mode;
        vehicle.status       = data.system_status;

        // We want the companion computer ot match the vechile system ID so update that
        computer.system_id   = vehicle.system_id;

        // We have a connection to the vehicle so we can update that things are alive
        _t_heartbeat_vehicle = millis();

        break;
      }
  }

  // Print what we just got
  if (_debug >= 2)
  {
    DEBUG_SERIAL.print("Got a HEARTBEAT message\n");
    DEBUG_SERIAL.print("\tID: ");           DEBUG_SERIAL.print(msg.sysid);
    DEBUG_SERIAL.print(", Component: ");   DEBUG_SERIAL.print(msg.compid);
    DEBUG_SERIAL.print(", Type: ");        DEBUG_SERIAL.print(data.type);
    DEBUG_SERIAL.print(", Autopilot: ");   DEBUG_SERIAL.print(data.autopilot);
    DEBUG_SERIAL.print(", Mode: ");        DEBUG_SERIAL.print(data.base_mode);
    DEBUG_SERIAL.print(", Custom Mode: "); DEBUG_SERIAL.print(data.custom_mode);
    DEBUG_SERIAL.print(", Status: ");      DEBUG_SERIAL.print(data.system_status);
    DEBUG_SERIAL.print("\n");
  }

  // All done
  return;

}

void COMMS_MAVLINK::rangefinder_rx(mavlink_message_t msg) {
  // Decode message
  mavlink_rangefinder_t data;
  mavlink_msg_rangefinder_decode(&msg, &data);

  // Store data
  _range = data.distance;

  _t_rangefinder = millis();

  // Print data
  if (_debug >= 2)
  {
    DEBUG_SERIAL.print("Got a RANGEFINDER message\n");
    DEBUG_SERIAL.print("\tRange: "); DEBUG_SERIAL.print(_range); DEBUG_SERIAL.print(" [ m ]\n");
  }
}

void COMMS_MAVLINK::local_position_ned_rx(mavlink_message_t msg) {
  // Decode message
  mavlink_local_position_ned_t data;
  mavlink_msg_local_position_ned_decode(&msg, &data);

  // Store data
  _NED.X  = data.x;
  _NED.Y  = data.y;
  _NED.Z  = data.z;
  _NED.Vx = data.vx;
  _NED.Vy = data.vy;
  _NED.Vz = data.vz;

  _NED.V  = sqrtf(data.vx * data.vx + data.vy * data.vy + data.vz * data.vz);

  _t_NED_local = millis();

  // Debugging
  if (_debug)
  {
    DEBUG_SERIAL.print("Got a Local Position (NED) message\n");
    DEBUG_SERIAL.print("\tX: ");  DEBUG_SERIAL.print(_NED.X );
    DEBUG_SERIAL.print(", Y: ");  DEBUG_SERIAL.print(_NED.Y );
    DEBUG_SERIAL.print(", Z: ");  DEBUG_SERIAL.print(_NED.Z );
    DEBUG_SERIAL.print(", Vx: "); DEBUG_SERIAL.print(_NED.Vx);
    DEBUG_SERIAL.print(", Vy: "); DEBUG_SERIAL.print(_NED.Vy);
    DEBUG_SERIAL.print(", Vz: "); DEBUG_SERIAL.print(_NED.Vz); DEBUG_SERIAL.print("\n");
  }

  // All done
  return;
}

void COMMS_MAVLINK::vfr_hud_rx(mavlink_message_t msg) {
  // Decode message
  //mavlink_vfr_hud_t data;
  mavlink_msg_vfr_hud_decode(&msg, &_VFR_HUD);


  _t_VFR_HUD = millis();

  // Debugging
  if (_debug >= 2)
  {
    DEBUG_SERIAL.print("Got a VFR HUD message\n");
    DEBUG_SERIAL.print("\tAirspeed: ");    DEBUG_SERIAL.print(_VFR_HUD.airspeed    );
    DEBUG_SERIAL.print(", Groundspeed: "); DEBUG_SERIAL.print(_VFR_HUD.groundspeed );
    DEBUG_SERIAL.print(", Alt: ");         DEBUG_SERIAL.print(_VFR_HUD.alt         );
    DEBUG_SERIAL.print(", Climb Rate: ");  DEBUG_SERIAL.print(_VFR_HUD.climb       );
    DEBUG_SERIAL.print(", Heading: ");     DEBUG_SERIAL.print(_VFR_HUD.heading     );
    DEBUG_SERIAL.print(", Throttle: ");    DEBUG_SERIAL.print(_VFR_HUD.throttle    ); DEBUG_SERIAL.print("\n");
  }

  // All done
  return;
  
}

void COMMS_MAVLINK::servo_output_raw_rx(mavlink_message_t msg) {
  // Decode message
  mavlink_servo_output_raw_t data;
  mavlink_msg_servo_output_raw_decode(&msg, &data);

  // Store data
  _RCOU.chan.RCOU1 = data.servo1_raw;
  _RCOU.chan.RCOU2 = data.servo2_raw;
  _RCOU.chan.RCOU3 = data.servo3_raw;
  _RCOU.chan.RCOU4 = data.servo4_raw;
  _RCOU.chan.RCOU5 = data.servo5_raw;
  _RCOU.chan.RCOU6 = data.servo6_raw;
  _RCOU.chan.RCOU7 = data.servo7_raw;
  _RCOU.chan.RCOU8 = data.servo8_raw;
  _RCOU.chan.RCOU9 = data.servo9_raw;
  _RCOU.chan.RCOU10 = data.servo10_raw;
  _RCOU.chan.RCOU11 = data.servo11_raw;
  _RCOU.chan.RCOU12 = data.servo12_raw;
  _RCOU.chan.RCOU13 = data.servo13_raw;
  _RCOU.chan.RCOU14 = data.servo14_raw;
  _RCOU.chan.RCOU15 = data.servo15_raw;
  _RCOU.chan.RCOU16 = data.servo16_raw;

  _t_RCOU = millis();

  // Debugging
  if (_debug >= 2)
  {
    DEBUG_SERIAL.print("Got a SERVO_OUTPUT_RAW message\n");
    DEBUG_SERIAL.print("\tServo1: "); DEBUG_SERIAL.print(RCOU(1));
    DEBUG_SERIAL.print(", Servo2: "); DEBUG_SERIAL.print(RCOU(2));
    DEBUG_SERIAL.print(", Servo3: "); DEBUG_SERIAL.print(RCOU(3));
    DEBUG_SERIAL.print(", Servo4: "); DEBUG_SERIAL.print(RCOU(4));
    DEBUG_SERIAL.print(", Servo5: "); DEBUG_SERIAL.print(RCOU(5));
    DEBUG_SERIAL.print(", Servo6: "); DEBUG_SERIAL.print(RCOU(6));
    DEBUG_SERIAL.print(", Servo7: "); DEBUG_SERIAL.print(RCOU(7));
    DEBUG_SERIAL.print(", Servo8: "); DEBUG_SERIAL.print(RCOU(8)); DEBUG_SERIAL.print("\n");
  }

  // All done
  return;
}

