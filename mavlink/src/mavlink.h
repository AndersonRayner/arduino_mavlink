// Library for interfacing mavlink stream with Volcano PCB coe

#ifndef ARDU_MAVLINK_MAIN_HEADER
#define ARDU_MAVLINK_MAIN_HEADER

#include <Arduino.h>
#include <stdint.h>
#include <math.h>

#include <EEPROM.h>

#include "c_library_v2/ardupilotmega/mavlink.h"

#include "data_structures.h"
#include "parameters.h"

#define _GCS_SYSID 255

class COMMS_MAVLINK {
  public :
    COMMS_MAVLINK(Stream& _MAVLINK_SERIAL);

    // === Functions
    void init();
    void update();

    void request_stream(uint8_t stream, uint16_t rate);

    void send_heartbeat();
    void send_string(char buf[], uint8_t len);

    void broadcast(mavlink_message_t msg);

    // === Accessor Functions
    // Connection Health
    bool vehicle_connected();
    bool gcs_connected();

    // Range
    bool rangefinder_healthy();
    float rangefinder();

    // Servo
    bool RCOU_healthy();
    uint16_t RCOU(uint8_t servo_number);

    // Local Position
    bool NED_local_healthy();
    float X();
    float Y();
    float Z();
    float Vx();
    float Vy();
    float Vz();
    float V();

    // VFR_HUD
    bool VFR_HUD_healthy();
    float airspeed();
    float groundspeed();
    float alt();
    float climb();
    int16_t heading();
    uint16_t throttle();
    
    // Parameter handling
    float get_param(uint8_t PARAM_NAME);
    void  set_param(uint8_t PARAM_NAME, float value);
        
    // === Variables
    mavlink_ID_struct computer, vehicle, gcs;
    uint8_t _debug = 0;

  private :
    Stream& MAVLINK_SERIAL;
    Stream& DEBUG_SERIAL = Serial;

    // === Functions
    // Parameter handling
    params_struct _params;

    float eeprom_checksum(void);
    bool  eeprom_valid(void);
    void  init_parameters(void);
    void  send_parameter_value(int8_t param_ID);
    void  set_name_and_default(uint8_t param_name, char* display_name, float default_value);
    void  set_parameter_from_mavlink(mavlink_message_t msg);
    
    bool  _load_param_defaults = 1;

    // Received mavlink message handling
    void heartbeat_rx(mavlink_message_t msg);
    void rangefinder_rx(mavlink_message_t msg);
    void servo_output_raw_rx(mavlink_message_t msg);
    void local_position_ned_rx(mavlink_message_t msg);
    void vfr_hud_rx(mavlink_message_t msg);


    // === Variables
    bool _vehicle_alive = 0;
    bool _gcs_alive = 0;

    servo_data        _RCOU;
    ned_struct        _NED;
    mavlink_vfr_hud_t _VFR_HUD;
    float _range = 0.0f;

    unsigned long _t_RCOU              = -500;
    unsigned long _t_rangefinder       = -500;
    unsigned long _t_NED_local         = -500;
    unsigned long _t_VFR_HUD           = -500;
    unsigned long _t_heartbeat_vehicle = -3000;
    unsigned long _t_heartbeat_GCS     = -3000;

    uint8_t _counter = 0;
    
    char _buf[50];

};

#endif
