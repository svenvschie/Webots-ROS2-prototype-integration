# Webots ROS 2 Integration Test

This repository contains files for testing integration between **Webots** and **ROS 2 Jazzy** (current LTS).

It provides a small prototype package for verifying a basic Webots and ROS 2 integration workflow:

* launching a robot simulation in Webots
* connecting ROS 2 nodes to the simulation

## Supported and documented setups

This README documents the following setups:

* **Ubuntu 24.04**
* **Windows 11 + WSL2 + Ubuntu 24.04**

This README does **not** document native Windows ROS 2 installation.

> Although ROS 2 Jazzy officially documents Windows 10 support, that setup requires a different installation process which is not covered in this repository.
> Additionally, the Webots ROS 2 documentation recommends using WSL2 with Ubuntu on Windows, regardless of version.

## Prerequisites

Before using this package, make sure you have the following installed.

### WSL2 (Windows 11 only)

If you are using Windows 11, install WSL2 and Ubuntu first:

[https://learn.microsoft.com/en-us/windows/wsl/install](https://learn.microsoft.com/en-us/windows/wsl/install)

### ROS 2 Jazzy

Install ROS 2 Jazzy in Ubuntu:

[https://docs.ros.org/en/jazzy/Installation/Ubuntu-Install-Debs.html](https://docs.ros.org/en/jazzy/Installation/Ubuntu-Install-Debs.html)

### Webots

* Install **Webots on Windows** if you are using **Windows 11 + WSL2**
* Install **Webots on Ubuntu** if you are using a full Ubuntu installation

#### Install Webots ROS 2 integration

Follow the official guide to install the required `webots_ros2` packages:

[https://docs.ros.org/en/jazzy/Tutorials/Advanced/Simulators/Webots/Installation-Ubuntu.html](https://docs.ros.org/en/jazzy/Tutorials/Advanced/Simulators/Webots/Installation-Ubuntu.html)

### colcon

You need `colcon` to build the package:

[https://docs.ros.org/en/jazzy/Tutorials/Beginner-Client-Libraries/Colcon-Tutorial.html](https://docs.ros.org/en/jazzy/Tutorials/Beginner-Client-Libraries/Colcon-Tutorial.html)

### ROS 2 workspace

You need a working ROS 2 workspace, for example `~/ros2_ws`:

[https://docs.ros.org/en/jazzy/Tutorials/Beginner-Client-Libraries/Creating-A-Workspace/Creating-A-Workspace.html](https://docs.ros.org/en/jazzy/Tutorials/Beginner-Client-Libraries/Creating-A-Workspace/Creating-A-Workspace.html)

## Installation

### Ubuntu 24.04

If you are using a full Ubuntu 24.04 installation:

```bash
cd ~/ros2_ws/src
git clone <REPO_URL>
cd ~/ros2_ws
source /opt/ros/jazzy/setup.bash
colcon build --packages-select webots_ros2_prototype_integration_test
```

### Windows 11 + WSL2

This setup uses:

* ROS 2 Jazzy inside WSL2 Ubuntu 24.04
* Webots installed on Windows

#### 1. Set Webots path in WSL

```bash
export WEBOTS_HOME="/mnt/c/Program Files/Webots"
```

To make this persistent:

```bash
echo 'export WEBOTS_HOME="/mnt/c/Program Files/Webots"' >> ~/.bashrc
source ~/.bashrc
```

#### 2. Clone and build

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

### Webots and ROS 2 cannot connect

Apply the WSL networking fix described below.

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

## WSL networking fix

On WSL, `webots_ros2_driver` may fail to connect correctly to Webots running on Windows because of the auto-generated `/etc/resolv.conf`.

### 1. Disable auto-generation of `/etc/resolv.conf`

```bash
sudo tee /etc/wsl.conf > /dev/null <<'EOF'
[network]
generateResolvConf = false
EOF
```

### 2. Restart WSL

From Windows PowerShell or Command Prompt:

```powershell
wsl --shutdown
```

Then reopen Ubuntu.

### 3. Replace `/etc/resolv.conf`

```bash
sudo rm -f /etc/resolv.conf
echo -e "nameserver 127.0.0.1\nnameserver 8.8.8.8" | sudo tee /etc/resolv.conf
```

> Using only `nameserver 127.0.0.1` may break internet access inside WSL.

### 4. Verify

```bash
cat /etc/resolv.conf
```

Expected output:

```text
nameserver 127.0.0.1
nameserver 8.8.8.8
```
