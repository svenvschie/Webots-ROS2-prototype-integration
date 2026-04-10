# Swarm Robot

![Swarm Robot](docs/images/header.png)

## Introduction

Robot Swarm is a project focused on developing autonomous mobile robots operating within the TQ campus at Fontys University of Applied Sciences. These robots navigate independently, respond to their environment, and manage their own operation, including returning to charging stations when required.

This repository contains the ROS 2 workspace used to develop and run the robot software stack within a Docker-based environment.

## Prerequisites

Make sure the following is installed on your host machine:

- Docker Desktop  
  https://www.docker.com/products/docker-desktop/

No local ROS 2 installation is required.

## Getting Started

### Clone the repository

~~~bash
git clone git@projects.fhict.nl:pp-ol/robot-swarm.git
~~~

### Build the Docker image

Run the following command **from the root of this repository** (the directory containing the `Dockerfile`):

~~~bash
docker build -t tqbot-jazzy .
~~~

### Start the development container

Run the container while mounting your local copy of this repository into the workspace inside Docker.

Replace `<path-to-repo>` with the absolute path to the cloned repository on your system.

#### Windows (PowerShell)

~~~powershell
docker run -it --rm `
  -v "<path-to-repo>:/root/robot-swarm" `
  tqbot-jazzy
~~~

#### Linux

~~~bash
docker run -it --rm \
  -v <path-to-repo>:/root/robot-swarm \
  tqbot-jazzy
~~~

### Build the workspace

Inside the container:

~~~bash
source /opt/ros/jazzy/setup.bash
cd /root/robot-swarm
colcon build
source install/setup.bash
~~~

## Simulation

For running the system in Webots, refer to:

- `docs/simulation.md`

Simulation is used for testing and validating robot behaviour without requiring physical hardware.

## Conclusion

This repository defines the Docker-based ROS 2 workspace for the Robot Swarm system. Following the steps above results in a consistent development environment with the workspace mounted and ready to build.

From this point, development can proceed directly within the container:

- Modify or extend existing packages  
- Implement new ROS 2 nodes and system components  
- Build and validate changes within the workspace  

This environment serves as the baseline for all further development and integration within the Robot Swarm system.
