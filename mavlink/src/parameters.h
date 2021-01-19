// Data structures for the params
//
// Matt Anderson 2020

#ifndef ARDU_MAVLINK_PARAM_STRUCTURES
#define ARDU_MAVLINK_PARAM_STRUCTURES


// Param enum for all the parameters that we're using.
// Remember to initialise them with their names in mavlink_parameters.cpp

enum params_enum
{
  // Parameter names
  PARAM_1 = 0,     // Test parameter 1
  PARAM_2,         // Test parameter 2
  PARAM_3,         // Test parameter 3
  PARAM_4,         // Test parameter 4
 
  RESET_EERPOM,       // Set to 1 reset the eeprom at next boot
  EEPROM_CHECKSUM,    // Keep this here, tracks if the eeprom values are good
  P_COUNT,            // This sets the number of parameters
  
};

// This struct can be used to hold the names and values of all the parameters we currently have.
// The size is automatically set using the P_COUNT enum value
struct params_struct
{
  char  p_name[P_COUNT][17]; // String names for the parameters that will appear in Mission Planner (max 17 chars long)
  float p_value[P_COUNT];    // Float values for each of the parameters.  I think keeping everything as floats will make our life easier
                             // We could me more intelligent and store some as uint8_t etc, but ain't nobody got time for that
};

#endif
