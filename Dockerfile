FROM osrf/ros:jazzy-desktop

SHELL ["/bin/bash", "-c"]

RUN apt update && apt install -y \
    ros-jazzy-webots-ros2-driver \
    ros-jazzy-xacro \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /root/ros2_ws