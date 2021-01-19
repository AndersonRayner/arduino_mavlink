// Data structures for mavlink code thing
// Matt Anderson, 2020

#ifndef ARDU_MAVLINK_DATA_STRUCTURES
#define ARDU_MAVLINK_DATA_STRUCTURES

// Global structs
struct mavlink_ID_struct
{
  uint8_t system_id;
  uint8_t component_id;
  uint8_t type;
  uint8_t autopilot;
  uint8_t mode;
  uint32_t custom_mode;
  uint8_t status;
};

// Servo PWM Struct
struct servo_out_struct 
{
    uint16_t RCOU1;
    uint16_t RCOU2;
    uint16_t RCOU3;
    uint16_t RCOU4;
    uint16_t RCOU5;
    uint16_t RCOU6;
    uint16_t RCOU7;
    uint16_t RCOU8;
    uint16_t RCOU9;
    uint16_t RCOU10;
    uint16_t RCOU11;
    uint16_t RCOU12;
    uint16_t RCOU13;
    uint16_t RCOU14;
    uint16_t RCOU15;
    uint16_t RCOU16;

}__attribute__((packed));

union servo_data
{
    servo_out_struct chan;
    uint16_t rc[16];
};

// Servo NED Struct
struct ned_struct 
{
    float X;
    float Y;
    float Z;
    float Vx;
    float Vy;
    float Vz;
    float V;

}__attribute__((packed));

#endif  // ARDU_MAVLINK_DATA_STRUCTURES
