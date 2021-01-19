
#include "mavlink.h"

void COMMS_MAVLINK::send_heartbeat() {                                   // Send heartbeat
  // MAVLink config
  // Initialize the required buffers
  mavlink_message_t msg;

  // Pack the message
  mavlink_msg_heartbeat_pack(computer.system_id, computer.component_id, &msg, computer.type, computer.autopilot, computer.mode, computer.custom_mode, computer.status);

  // Send message
  broadcast(msg);

  // All done
  return;
}

void COMMS_MAVLINK::send_string(char buf[], uint8_t len) {          // Send a string
    // Sends a string of text.  Can do up to 50 characters
    mavlink_message_t msg;
    mavlink_statustext_t tx;
 
    if ( len > sizeof(tx.text) )
    {
        // The buffer is overflowed so send an error message instead
        tx.severity = MAV_SEVERITY_ALERT;
        sprintf(tx.text, "Text buffer overflowing...");
        
        if (_debug) { DEBUG_SERIAL.print("mavlink: Text buffer overflow, only allowed "); DEBUG_SERIAL.print(sizeof(tx.text)); DEBUG_SERIAL.print(" bytes\n"); }
        
    } else {       
        // Fill out the message
        tx.severity = MAV_SEVERITY_INFO;
        memcpy(tx.text,buf,len);
    }
    
    if (_debug) { DEBUG_SERIAL.print("mavlink: Sending string << "); DEBUG_SERIAL.print(tx.text); DEBUG_SERIAL.print(" >>\n"); }

    // Pack the message
    mavlink_msg_statustext_pack(computer.system_id, computer.component_id, &msg, tx.severity, tx.text, 0, 0);
  
    // Send Message
    broadcast(msg);

    // All done
    return;
  
}

void COMMS_MAVLINK::request_stream(uint8_t stream, uint16_t rate) { // Request Stream
  if (_debug) { DEBUG_SERIAL.print("Requesting Stream "); DEBUG_SERIAL.print(stream); DEBUG_SERIAL.print(" at "); DEBUG_SERIAL.print(rate); DEBUG_SERIAL.print(" Hz\n"); } 
    
  // Set up STREAMs to request from Pixhawk.  Get these from << typedef enum MAV_DATA_STREAM >> in common.h
  mavlink_message_t msg;

  // Assemble Request
  mavlink_msg_request_data_stream_pack( computer.system_id, computer.component_id, &msg, vehicle.system_id, 0, stream, rate, 1);
 
  // Send the message
  broadcast(msg);

  // All done
  return;
}

void COMMS_MAVLINK::broadcast(mavlink_message_t msg) {              // Broadcast a mavlink packet
    uint8_t buf[MAVLINK_MAX_PACKET_LEN];
      
    // Copy the message to the send buffer
    uint16_t len = mavlink_msg_to_send_buffer(buf, &msg);

    // Send the message
    MAVLINK_SERIAL.write(buf, len);

    // All done
    return;
    
}
