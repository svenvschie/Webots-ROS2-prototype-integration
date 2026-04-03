# Webots ROS 2 Integration Test
## Overview

This repository demonstrates a hybrid Webots–ROS 2 setup.

* **Webots** runs natively on the host system (Windows or Ubuntu) and handles the robot simulation
* **ROS 2** runs inside Ubuntu (WSL2 on Windows) and runs the control nodes that interact with the simulation
* Communication between Webots and ROS 2 occurs over a network interface

This setup balances compatibility and graphical performance on Windows systems.

## Supported Setups

This README supports the following environments:

* **Ubuntu 24.04 (native)**
* **Windows with WSL2 (Ubuntu 24.04)**

Native Windows ROS 2 installation is **not** supported.

## Prerequisites

Before using this package, ensure the following components are installed:

### WSL2 (Windows only)

If you are using Windows, install WSL2 (with Ubuntu) first:

[https://learn.microsoft.com/en-us/windows/wsl/install](https://learn.microsoft.com/en-us/windows/wsl/install)

After installation, a reboot may be required.
In some cases, you may need to run `wsl --install` again after reboot to complete the Ubuntu installation.

### ROS 2 Jazzy

Install ROS 2 Jazzy in Ubuntu (using WSL2 if on Windows):

[https://docs.ros.org/en/jazzy/Installation/Ubuntu-Install-Debs.html](https://docs.ros.org/en/jazzy/Installation/Ubuntu-Install-Debs.html)

### Webots

Install Webots:

* On **Windows** if using **Windows + WSL2**
* On **Ubuntu** if using a native Ubuntu system

#### Windows + WSL2 only: set Webots path in WSL

If Webots is installed on Windows, make sure WSL can find it before installing the Webots ROS 2 integration:

```bash
export WEBOTS_HOME="/mnt/c/Program Files/Webots"
```

To make this persistent:

```bash
echo 'export WEBOTS_HOME="/mnt/c/Program Files/Webots"' >> ~/.bashrc
source ~/.bashrc
```

### Install Webots ROS 2 integration & colcon

```bash
sudo apt install ros-jazzy-webots-ros2 python3-colcon-common-extensions
```

### ROS 2 workspace

You need a working ROS 2 workspace, for example `~/ros2_ws`:

```bash
mkdir -p ~/ros2_ws/src
```

### Environment setup (every new terminal)

In every new terminal, remember to source ROS 2 and your workspace:

```bash
cd ~/ros2_ws
source /opt/ros/jazzy/setup.bash
source ~/ros2_ws/install/setup.bash
```

## Installation

```bash
cd ~/ros2_ws/src
git clone <REPO_URL>
cd ~/ros2_ws
source /opt/ros/jazzy/setup.bash
colcon build --packages-select webots_ros2_prototype_integration_test
```

## Usage

In a new terminal (after running the environment setup step):

### Launch the package

```bash
ros2 launch webots_ros2_prototype_integration_test robot_launch.py
```

In case of a persistent "Cannot connect to Webots instance" error, see [WSL networking fix](#wsl-networking-fix).

### Publish velocity commands

Open a second terminal and run:

```bash
ros2 topic pub /cmd_vel geometry_msgs/msg/Twist "{linear: {x: 2.0}, angular: {z: 0.0}}"
```

### Launch with obstacle avoidance

```bash
ros2 launch webots_ros2_prototype_integration_test robot_launch.py use_obstacle_avoider:=true
```

## Troubleshooting

### Webots cannot be found from WSL

Check that `WEBOTS_HOME` is set correctly:

```bash
echo $WEBOTS_HOME
```

Expected value:

```text
/mnt/c/Program Files/Webots
```

### `ros2` command not found

Make sure ROS 2 has been sourced:

```bash
source /opt/ros/jazzy/setup.bash
```

### Package not found

Make sure the workspace has been sourced after building:

```bash
source ~/ros2_ws/install/setup.bash
```

### WSL networking fix

If Webots runs on Windows and ROS 2 runs in WSL2, `127.0.0.1` may not work because they are on different network interfaces.

#### Get the Windows host IP

From WSL, run:

```bash
ip route | grep default
```

Example output:

```text
default via 172.20.160.1 dev eth0 proto kernel
```

#### Launch with the correct IP

Use the `default via` address as `webots_ip`:

```bash
ros2 launch webots_ros2_prototype_integration_test robot_launch.py webots_ip:=172.20.160.1
```

Launch with obstacle avoidance:

```bash
ros2 launch webots_ros2_prototype_integration_test robot_launch.py webots_ip:=172.20.160.1 use_obstacle_avoider:=true
```
