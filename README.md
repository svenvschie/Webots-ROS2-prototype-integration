# Webots ROS 2 Integration Test

This repository contains files for testing integration between **Webots** and **ROS 2 Jazzy** (current LTS).

It provides a small prototype package for verifying a basic Webots and ROS 2 integration workflow:

* launching a robot simulation in Webots
* connecting ROS 2 nodes to the simulation

## Supported and documented setups

This README documents the following setups:

* **Ubuntu 24.04**
* **Windows + WSL2 + Ubuntu 24.04**

This README does **not** document native Windows ROS 2 installation.

> Although ROS 2 Jazzy officially documents native Windows 10 support, that setup requires a different installation process which is not covered in this repository.
> Additionally, the Webots ROS 2 documentation recommends using WSL2 with Ubuntu on Windows, regardless of Windows version.

## Prerequisites

Before using this package, make sure you have the following installed.

### WSL2 (Windows only)

If you are using Windows, install WSL2 (With default option: Ubuntu) first:

[https://learn.microsoft.com/en-us/windows/wsl/install](https://learn.microsoft.com/en-us/windows/wsl/install)

After installation, a reboot is sometimes required.
In some cases, you may need to run wsl --install again after reboot to complete the Ubuntu installation.

### ROS 2 Jazzy

Install ROS 2 Jazzy in Ubuntu (using WSL2 in case of Windows):

[https://docs.ros.org/en/jazzy/Installation/Ubuntu-Install-Debs.html](https://docs.ros.org/en/jazzy/Installation/Ubuntu-Install-Debs.html)

### Webots

* Install **Webots on Windows** if you are using **Windows + WSL2**
* Install **Webots on Ubuntu** if you are using a full Ubuntu installation

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

Install the required `webots_ros2` packages and `colcon` for building packages (if not already installed):

```bash
sudo apt-get install ros-jazzy-webots-ros2
sudo apt install python3-colcon-common-extensions
```

### ROS 2 workspace

You need a working ROS 2 workspace, for example `~/ros2_ws`:

```bash
mkdir -p ~/ros2_ws/src 
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

### Launch the package

```bash
cd ~/ros2_ws
source /opt/ros/jazzy/setup.bash
source install/setup.bash
ros2 launch webots_ros2_prototype_integration_test robot_launch.py
```

In case of a persistent "Cannot connect to Webots instance" error, see [WSL networking fix](#wsl-networking-fix).

### Publish velocity commands

Open a second terminal and run:

```bash
source /opt/ros/jazzy/setup.bash
source ~/ros2_ws/install/setup.bash
ros2 topic pub /cmd_vel geometry_msgs/msg/Twist "{linear: {x: 2.0}, angular: {z: 0.0}}"
```

### Launch with obstacle avoidance

```bash
cd ~/ros2_ws
source /opt/ros/jazzy/setup.bash
source ~/ros2_ws/install/setup.bash
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

If Webots runs on Windows and ROS 2 runs in WSL2, the default address (`127.0.0.1`) may not work.

#### Get the Windows host IP

From WSL, run:

```bash
ip route | grep default
```

Example output:

default via 172.20.160.1 dev eth0 proto kernel

#### Launch with the correct IP

Use the `default via` address as `webots_ip`:

```bash
ros2 launch webots_ros2_prototype_integration_test robot_launch.py webots_ip:=172.20.160.1
```

Launch with obstacle avoidance

```bash
ros2 launch webots_ros2_prototype_integration_test robot_launch.py webots_ip:=172.20.160.1 use_obstacle_avoider:=true
```