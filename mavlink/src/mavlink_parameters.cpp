
#include "mavlink.h"

// Initialise all the parameter names and values
void COMMS_MAVLINK::init_parameters(void) {
       
    // Read parameter values from eeprom.
    // Trigger a reset to defaults if either the eeprom is invalid or we've been asked to reset it
    EEPROM.get(0x00, _params.p_value);
    if (!eeprom_valid() || get_param(RESET_EERPOM)) {
        _load_param_defaults = 1;
    } else { 
        _load_param_defaults = 0;
    }     
    
    // Set everything to zero and default names
    // This will prevent Mission Planner from hanging if we missed something
    for (uint8_t ii=0; ii<P_COUNT; ii++)
    {
       sprintf(_params.p_name[ii],"IDX%03u_NOT_SET",ii);
       if (_load_param_defaults) { _params.p_value[ii] = 0.0f; }
    }
    
    // Initialise the names for the parameters.
    // Reset them to the default value if the eeprom checksum failed
    char p_name[16];
    
    sprintf(p_name, "TEST_PARAM_1"); set_name_and_default(PARAM_1, p_name, 10.0f );
    sprintf(p_name, "TEST_PARAM_2"); set_name_and_default(PARAM_2, p_name,  8.0f );
    sprintf(p_name, "TEST_PARAM_3"); set_name_and_default(PARAM_3, p_name, 20.0f );
    sprintf(p_name, "TEST_PARAM_4"); set_name_and_default(PARAM_4, p_name,  2.0f );
    
    sprintf(p_name, "RESET_EERPOM");    set_name_and_default(RESET_EERPOM,    p_name,  0.0f );
    sprintf(p_name, "EEPROM_CHECKSUM"); set_name_and_default(EEPROM_CHECKSUM, p_name,  0.0f );
    
    // Save parameters to the eeprom
    _params.p_value[EEPROM_CHECKSUM] = eeprom_checksum();
    EEPROM.put(0x00, _params.p_value);
    _load_param_defaults = 0;
              
    // All done
    return;
}

// Set the display name default value for a variable
void COMMS_MAVLINK::set_name_and_default(uint8_t param_name, char* display_name, float default_value) {
    
    // Set the display name
    strcpy(_params.p_name[param_name], display_name);
    
    // Override the exisitng value if we're loading the defaults
    if (_load_param_defaults) {
        set_param(param_name, default_value);
    }
    
    return;
    
}

// Extract the param value from the params struct
float COMMS_MAVLINK::get_param(uint8_t param_name) {
  return (_params.p_value[param_name]);
}

// Set a param value
void COMMS_MAVLINK::set_param(uint8_t param_name, float value) {
    // Set the value
    _params.p_value[param_name] = value;
  
    // Update parameter table checksum
    _params.p_value[EEPROM_CHECKSUM] = eeprom_checksum();
    
    // Send the updated parameter value to the GCS
    send_parameter_value(param_name);
    
    return;
}

// Checksums the eeprom to make sure it has valid values
float COMMS_MAVLINK::eeprom_checksum(void) { 
    
    float checksum = 0;
    
    for (uint8_t ii=0; ii<(P_COUNT-1); ii++) {
        checksum = checksum + _params.p_value[ii];
    }
    
    return (100.0f - checksum);

}

// Check if the values in the eeprom are valid
bool COMMS_MAVLINK::eeprom_valid(void) {
    
    float checksum_stored = get_param(EEPROM_CHECKSUM);
    float checksum_calced = eeprom_checksum();
   
    if ((checksum_stored != checksum_calced) || isnan(checksum_calced) ) {
        return (0);
    } else { 
        return (1);
    }
    
    // Shouldn't be able to get here
    return (0);
}

// Transmit parameter value on the mavlink network
void COMMS_MAVLINK::send_parameter_value(int8_t param_ID) {

  // Initialize the required buffers
  mavlink_message_t msg;

  // Send all if (param_ID == -1)
  // We don't want to send the eeprom checksum, hence send all except the last parameter
  // Not sending the last parameter isn't working so who knows...
  if (param_ID == -1) {
    for (uint8_t ii = 0; ii < P_COUNT; ii++) {
      send_parameter_value(ii);
    }

    return;
  }

  // Assemble the packet
  mavlink_msg_param_value_pack(computer.system_id, computer.component_id, &msg,
                               _params.p_name[param_ID], _params.p_value[param_ID],
                               MAVLINK_TYPE_FLOAT, P_COUNT, param_ID);

  // Send the requested parameter on the link
  broadcast(msg);

  // All done
  return;

}

// Handle incoming parameter set message
void COMMS_MAVLINK::set_parameter_from_mavlink(mavlink_message_t msg) {
  // Decode incoming message
  mavlink_param_set_t data;
  mavlink_msg_param_set_decode(&msg, &data);

  int ii = 0;

  // Find ID of matching parameter by name (exit if reached end of param struct)
  while ((strcmp(_params.p_name[ii], data.param_id) != 0) && (ii < P_COUNT)) {
    ii++;
  }

  // if parameter name is valid
  if (ii < P_COUNT) {

    // Set the parameter in the param struct
    set_param(ii,data.param_value);

    // Send parameter back to confirm
    send_parameter_value(ii);
    
    // Save parameters to the eeprom
    EEPROM.put(0x00, _params.p_value);

  } else {
    // Parameter wasn't found, send mavlink message to GCS
    char buf[50];
    sprintf(buf,"Param %s not found",data.param_id);
    send_string(buf, sizeof(buf));

  }

  // All done
  return;

}

