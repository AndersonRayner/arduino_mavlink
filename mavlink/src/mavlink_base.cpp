
#include "mavlink.h"

COMMS_MAVLINK::COMMS_MAVLINK(Stream& _MAVLINK_SERIAL) :
  MAVLINK_SERIAL(_MAVLINK_SERIAL)
{

}

void COMMS_MAVLINK::update(void)
{

  if (_debug >= 3) {
    DEBUG_SERIAL.print("MAVLINK: updating\n");
  }

  mavlink_message_t msg;
  mavlink_status_t status;

  // Decode whatever infomation we have on the mavlink serial stream
  while (MAVLINK_SERIAL.available() > 0)
  {
    uint8_t c = MAVLINK_SERIAL.read();

    if (_debug >= 3) {
      DEBUG_SERIAL.print("MAVLINK: got ");
      DEBUG_SERIAL.print(c, HEX);
      DEBUG_SERIAL.print("\n");
    }

    // Try to get a new message
    if (mavlink_parse_char(MAVLINK_COMM_0, c, &msg, &status))
    {
      switch (msg.msgid)
      {
        case (MAVLINK_MSG_ID_HEARTBEAT) :
            heartbeat_rx(msg);
            break;

        case (MAVLINK_MSG_ID_RANGEFINDER) :
            rangefinder_rx(msg);
            break;

        case (MAVLINK_MSG_ID_SERVO_OUTPUT_RAW) :
            servo_output_raw_rx(msg);
            break;

        case (MAVLINK_MSG_ID_LOCAL_POSITION_NED) :
            local_position_ned_rx(msg);
            break;

        case (MAVLINK_MSG_ID_VFR_HUD) :
            vfr_hud_rx(msg);
            break;
          
          
        case MAVLINK_MSG_ID_PARAM_REQUEST_LIST :
            // Send all parameters to the ground station
            send_parameter_value(-1);
            break;
        
        case MAVLINK_MSG_ID_PARAM_REQUEST_READ :
            // Read packet to work out which parameter it wants
            mavlink_param_request_read_t data;
            mavlink_msg_param_request_read_decode(&msg, &data);

            // Send parameter
             send_parameter_value(data.param_index);
            break;

        case MAVLINK_MSG_ID_PARAM_SET :
            // Set a parameter based on the received value
            set_parameter_from_mavlink(msg);

            break;

        default:
            // Looks like we got a message that we're not really interested in
            if (_debug >= 2) {
                DEBUG_SERIAL.print("MSG_ID: ");
                DEBUG_SERIAL.print(msg.msgid);
                DEBUG_SERIAL.print("\n");
            }
      }
    }
  }

  return;
}

void COMMS_MAVLINK::init()
{
  // Vehicle
  // This will get set by whatever is sending us heartbeats
  vehicle.system_id = 1;
  vehicle.component_id = 1;
  vehicle.type = MAV_TYPE_QUADROTOR;
  vehicle.autopilot = MAV_AUTOPILOT_ARDUPILOTMEGA;
  vehicle.mode = MAV_MODE_PREFLIGHT;
  vehicle.custom_mode = 0;
  vehicle.status = MAV_STATE_STANDBY;

  // Host computer
  computer.system_id = vehicle.system_id;
  computer.component_id = 2;
  computer.type = MAV_TYPE_ONBOARD_CONTROLLER;
  computer.autopilot = MAV_AUTOPILOT_INVALID;
  computer.mode = MAV_MODE_PREFLIGHT;
  computer.custom_mode = 0;
  computer.status = MAV_STATE_STANDBY;

  // GCS
  gcs.system_id = _GCS_SYSID;
  gcs.component_id = 1;
  gcs.type = MAV_TYPE_QUADROTOR;
  gcs.autopilot = MAV_AUTOPILOT_ARDUPILOTMEGA;
  gcs.mode = MAV_MODE_PREFLIGHT;
  gcs.custom_mode = 0;
  gcs.status = MAV_STATE_STANDBY;
  
  // Initialise the parameters
  init_parameters();
  
  // All done
  return;
}

