# Webots Simulation

## Introduction

This document describes how to run the TQBot system in simulation using Webots.

Simulation is optional and intended for development, testing, and validation of robot behaviour without requiring physical hardware.

## Prerequisites

The following must be installed on the host machine:

- Webots R2025a  
  https://github.com/cyberbotics/webots/releases/tag/R2025a

## Running the Simulation

ROS 2 runs inside Docker, while Webots runs on the host system. Communication between the two is established over a network interface.

To enable this, the Webots installation must be mounted into the container.

### Start the container with Webots access

Replace `<path-to-repo>` and `<path-to-webots>` with the appropriate locations on your system.

#### Windows (PowerShell)

~~~powershell
docker run -it --rm `
  -v "<path-to-repo>:/root/robot-swarm" `
  -v "<path-to-webots>:/opt/webots" `
  -e WEBOTS_HOME=/opt/webots `
  tqbot-jazzy
~~~

#### Linux

~~~bash
docker run -it --rm \
  -v <path-to-repo>:/root/robot-swarm \
  -v <path-to-webots>:/opt/webots \
  -e WEBOTS_HOME=/opt/webots \
  tqbot-jazzy
~~~

## Build the workspace

Inside the container:

~~~bash
source /opt/ros/jazzy/setup.bash
cd /root/robot-swarm
colcon build
source install/setup.bash
~~~

## Start Webots

On the host system:

1. Open Webots  
2. Open a world file from:  
   `src/tqbot_webots/worlds`
3. Start the simulation  

Webots must be running before ROS 2 attempts to connect.

## Launch the simulation

Inside the container:

~~~bash
ros2 launch tqbot_webots sim_launch.py webots_ip:=host.docker.internal
~~~

## Expected Behaviour

On successful connection:

- The Webots controller initializes without errors  
- ROS 2 topics become available, including:

  - `/scan`  
  - `/cam_right/image_raw`  
  - `/ds_left/range`  
  - `/tf`  
  - `/clock`  

## Networking

If the connection cannot be established using `host.docker.internal`, the host gateway IP can be used instead.

Determine the gateway IP inside the container:

~~~bash
ip route | grep default
~~~

Example output:

~~~text
default via 172.17.0.1 dev eth0
~~~

Use the detected IP when launching:

~~~bash
ros2 launch tqbot_webots sim_launch.py webots_ip:=172.17.0.1
~~~

## Conclusion

This setup enables integration between the ROS 2 workspace and the Webots simulation environment. It allows development and validation of robot behaviour without requiring access to physical hardware.
