#include "tqbot_webots/TQBotDriver.hpp"

#include <pluginlib/class_list_macros.hpp>

#include <webots/motor.h>
#include <webots/robot.h>
#include <webots/supervisor.h>

#include <cmath>
#include <stdexcept>
#include <algorithm>

#include <geometry_msgs/msg/transform_stamped.hpp>

namespace tqbot_webots
{

TQBotDriver::TQBotDriver()
: node_(nullptr),
  left_motor_(0),
  right_motor_(0),
  self_node_(0),
  wheel_radius_(0.04),
  half_wheel_separation_(0.1),
  max_motor_velocity_(100.0),
  odom_frame_id_("odom"),
  base_frame_id_("base_link")
{
  cmd_vel_msg_.linear.x = 0.0;
  cmd_vel_msg_.angular.z = 0.0;
}

void TQBotDriver::init(
  webots_ros2_driver::WebotsNode * node,
  std::unordered_map<std::string, std::string> & parameters)
{
  node_ = node;

  if (parameters.count("wheel_radius"))
    wheel_radius_ = std::stod(parameters["wheel_radius"]);
  if (parameters.count("half_wheel_separation"))
    half_wheel_separation_ = std::stod(parameters["half_wheel_separation"]);
  if (parameters.count("max_motor_velocity"))
    max_motor_velocity_ = std::stod(parameters["max_motor_velocity"]);
  if (parameters.count("odom_frame_id"))
    odom_frame_id_ = parameters["odom_frame_id"];
  if (parameters.count("base_frame_id"))
    base_frame_id_ = parameters["base_frame_id"];

  left_motor_ = wb_robot_get_device("wheel_front_left");
  right_motor_ = wb_robot_get_device("wheel_front_right");

  if (!left_motor_ || !right_motor_) {
    throw std::runtime_error("Missing Webots motor devices");
  }

  wb_motor_set_position(left_motor_, INFINITY);
  wb_motor_set_velocity(left_motor_, 0.0);
  wb_motor_set_position(right_motor_, INFINITY);
  wb_motor_set_velocity(right_motor_, 0.0);

  self_node_ = wb_supervisor_node_get_self();
  if (!self_node_) {
    throw std::runtime_error(
      "wb_supervisor_node_get_self() failed. Did you set supervisor TRUE on the Robot?");
  }

  cmd_vel_subscription_ = node_->create_subscription<geometry_msgs::msg::Twist>(
    "/cmd_vel",
    rclcpp::QoS(10),
    std::bind(&TQBotDriver::cmdVelCallback, this, std::placeholders::_1));

  odom_publisher_ = node_->create_publisher<nav_msgs::msg::Odometry>("/odom", rclcpp::QoS(10));
  tf_broadcaster_ = std::make_unique<tf2_ros::TransformBroadcaster>(*node_);

  RCLCPP_INFO(node_->get_logger(), "TQBotDriver initialized with ground-truth odom");
}

void TQBotDriver::cmdVelCallback(const geometry_msgs::msg::Twist::SharedPtr msg)
{
  cmd_vel_msg_ = *msg;
}

void TQBotDriver::step()
{
  const double forward_speed = cmd_vel_msg_.linear.x;
  const double angular_speed = cmd_vel_msg_.angular.z;

  double left =
    (forward_speed - angular_speed * half_wheel_separation_) / wheel_radius_;
  double right =
    (forward_speed + angular_speed * half_wheel_separation_) / wheel_radius_;

  left = std::clamp(left, -max_motor_velocity_, max_motor_velocity_);
  right = std::clamp(right, -max_motor_velocity_, max_motor_velocity_);

  wb_motor_set_velocity(left_motor_, left);
  wb_motor_set_velocity(right_motor_, right);

  const double *position = wb_supervisor_node_get_position(self_node_);
  const double *orientation = wb_supervisor_node_get_orientation(self_node_);

  // orientation is a 3x3 rotation matrix in row-major order
  const double yaw = std::atan2(orientation[3], orientation[0]);
  const double half_yaw = 0.5 * yaw;

  const auto stamp = node_->get_clock()->now();

  nav_msgs::msg::Odometry odom;
  odom.header.stamp = stamp;
  odom.header.frame_id = odom_frame_id_;
  odom.child_frame_id = base_frame_id_;

  odom.pose.pose.position.x = position[0];
  odom.pose.pose.position.y = position[1];
  odom.pose.pose.position.z = position[2];

  odom.pose.pose.orientation.x = 0.0;
  odom.pose.pose.orientation.y = 0.0;
  odom.pose.pose.orientation.z = std::sin(half_yaw);
  odom.pose.pose.orientation.w = std::cos(half_yaw);

  // reuse commanded velocities as a simple odom twist
  odom.twist.twist.linear.x = forward_speed;
  odom.twist.twist.angular.z = angular_speed;

  odom_publisher_->publish(odom);

  geometry_msgs::msg::TransformStamped tf;
  tf.header.stamp = stamp;
  tf.header.frame_id = odom_frame_id_;
  tf.child_frame_id = base_frame_id_;
  tf.transform.translation.x = position[0];
  tf.transform.translation.y = position[1];
  tf.transform.translation.z = position[2];
  tf.transform.rotation = odom.pose.pose.orientation;

  tf_broadcaster_->sendTransform(tf);
}

}  // namespace tqbot_webots

PLUGINLIB_EXPORT_CLASS(tqbot_webots::TQBotDriver, webots_ros2_driver::PluginInterface)