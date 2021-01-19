# arduino_mavlink
An example library for interfacing mavlink with Arduino.
This library has been tested for interfacing with ArduPilot but should be easy enough to modify for use with Pixhawk or your own flight stack by changing the mavlink dialect.

Created using the great instructions at ```https://discuss.ardupilot.org/t/mavlink-and-arduino-step-by-step/```

```
git clone --recursive git@github.com:AndersonRayner/arduino_mavlink.git
```

# Installation
Install the ```mavlink``` folder directly into your Arduino libraries folder.
Make sure to also pull the submodules for this to work (or clone with the --recursive flag).

# Example Files
## mavlink_minimum
Minimal example that sends both a heartbeat and an incrementing counter message over mavlink.
Connect via Mission Planner (or other mavlink GCS) to see the messages.

## mavlink_send_attitude
Example of how to send messages via mavlink.  
This example fakes attitude data for the HUD in Mission Planner.

## mavlink_with_vehicle
Example file of how to use mavlink to get data from another flight controller (such as a Pixhawk running Ardupilot or PX4) and display it through the Arduino.
