#include "tqbot_webots/TQBotDriver.hpp"

#include <pluginlib/class_list_macros.hpp>

#include <webots/motor.h>
#include <webots/robot.h>

#include <algorithm>
#include <cmath>
#include <stdexcept>

namespace tqbot_webots
{

TQBotDriver::TQBotDriver()
: node_(nullptr),
  left_motor_(0),
  right_motor_(0),
  wheel_radius_(0.04),
  half_wheel_separation_(0.1),
  max_motor_velocity_(100.0)
{
  cmd_vel_msg_.linear.x = 0.0;
  cmd_vel_msg_.angular.z = 0.0;
}

void TQBotDriver::init(
  webots_ros2_driver::WebotsNode * node,
  std::unordered_map<std::string, std::string> & parameters)
{
  node_ = node;

  if (parameters.count("wheel_radius")) {
    wheel_radius_ = std::stod(parameters["wheel_radius"]);
  }
  if (parameters.count("half_wheel_separation")) {
    half_wheel_separation_ = std::stod(parameters["half_wheel_separation"]);
  }
  if (parameters.count("max_motor_velocity")) {
    max_motor_velocity_ = std::stod(parameters["max_motor_velocity"]);
  }

  left_motor_ = wb_robot_get_device("wheel_front_left");
  right_motor_ = wb_robot_get_device("wheel_front_right");

  if (!left_motor_ || !right_motor_) {
    RCLCPP_FATAL(
      node_->get_logger(),
      "Could not find Webots motors 'wheel_front_left' and/or 'wheel_front_right'.");
    throw std::runtime_error("Missing Webots motor devices");
  }

  wb_motor_set_position(left_motor_, INFINITY);
  wb_motor_set_velocity(left_motor_, 0.0);

  wb_motor_set_position(right_motor_, INFINITY);
  wb_motor_set_velocity(right_motor_, 0.0);

  cmd_vel_subscription_ = node_->create_subscription<geometry_msgs::msg::Twist>(
    "/cmd_vel",
    rclcpp::SensorDataQoS().reliable(),
    std::bind(&TQBotDriver::cmdVelCallback, this, std::placeholders::_1));

  RCLCPP_INFO(
    node_->get_logger(),
    "TQBotDriver initialized: wheel_radius=%.3f, half_wheel_separation=%.3f, max_motor_velocity=%.3f",
    wheel_radius_,
    half_wheel_separation_,
    max_motor_velocity_);
}

void TQBotDriver::cmdVelCallback(const geometry_msgs::msg::Twist::SharedPtr msg)
{
  cmd_vel_msg_ = *msg;
}

void TQBotDriver::step()
{
  const double forward_speed = cmd_vel_msg_.linear.x;
  const double angular_speed = cmd_vel_msg_.angular.z;

  double command_motor_left =
    (forward_speed - angular_speed * half_wheel_separation_) / wheel_radius_;
  double command_motor_right =
    (forward_speed + angular_speed * half_wheel_separation_) / wheel_radius_;

  command_motor_left = std::clamp(
    command_motor_left, -max_motor_velocity_, max_motor_velocity_);
  command_motor_right = std::clamp(
    command_motor_right, -max_motor_velocity_, max_motor_velocity_);

  wb_motor_set_velocity(left_motor_, command_motor_left);
  wb_motor_set_velocity(right_motor_, command_motor_right);
}

}  // namespace tqbot_webots

PLUGINLIB_EXPORT_CLASS(tqbot_webots::TQBotDriver, webots_ros2_driver::PluginInterface)