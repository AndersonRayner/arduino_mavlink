[Back to Index](https://github.com/AndersonRayner/uas_tools_index)

# arduino_mavlink
An example library for interfacing mavlink with Arduino.
This library has been tested for interfacing with ArduPilot but should be easy enough to modify for use with Pixhawk or your own flight stack by changing the mavlink dialect.

Created using the great instructions at ```https://discuss.ardupilot.org/t/mavlink-and-arduino-step-by-step/```

```
git clone --recursive git@github.com:AndersonRayner/arduino_mavlink.git
```

This repo contains some extra functionality including adding Parameters that can be modified in real-time through your favourite mavlink GCS such as Mission Planner or qgroundcontrol.
Just have a look at ```mavlink/src/mavlink_parameters.cpp``` and ```mavlink/src/parameters.h``` to see how it is done.
One day I'll get to writing a blog post to properly explain everything...

# Installation
Install the ```mavlink``` folder directly into your Arduino libraries folder.
Make sure to also pull the submodules for this to work (or clone with the --recursive flag).
The submodule is for the official auto-generated C/C++ headers for mavlink available at ```https://github.com/mavlink/c_library_v2```.

# Example Files
## mavlink_minimum
Minimal example that sends both a heartbeat and an incrementing counter message over mavlink.
Connect via Mission Planner (or other mavlink GCS) to see the messages.

## mavlink_send_attitude
Example of how to send messages via mavlink.
This example fakes attitude data for the HUD in Mission Planner.

## mavlink_with_vehicle
Example file of how to use mavlink to get data from another flight controller (such as a Pixhawk running Ardupilot or PX4) and display it through the Arduino.

# Contributions
Submit a PR if there's any silly mistakes I've made with the code (or potential improvements).
As the messages to send and receive are very much dependent on the project at hand, I suggest you fork this repo to get yourself started and develop in your own repo, rather than submitting a PR here adding a particular message.
The idea is to keep this repo simple so it can be used as a stepping stone for other's future projects :).
