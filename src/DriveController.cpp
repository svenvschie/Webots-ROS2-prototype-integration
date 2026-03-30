#include "webots_ros2_prototype_integration_test/DriveController.hpp"

#include <cmath>

#include "webots/motor.h"
#include "webots/robot.h"

namespace webots_ros2_prototype_integration_test {

void DriveController::init(
  webots_ros2_driver::WebotsNode *node,
  std::unordered_map<std::string, std::string> &parameters) {

  (void)parameters;

  wheel1 = wb_robot_get_device("wheel1");
  wheel2 = wb_robot_get_device("wheel2");
  wheel3 = wb_robot_get_device("wheel3");
  wheel4 = wb_robot_get_device("wheel4");

  wb_motor_set_position(wheel1, INFINITY);
  wb_motor_set_position(wheel2, INFINITY);
  wb_motor_set_position(wheel3, INFINITY);
  wb_motor_set_position(wheel4, INFINITY);

  wb_motor_set_velocity(wheel1, 0.0);
  wb_motor_set_velocity(wheel2, 0.0);
  wb_motor_set_velocity(wheel3, 0.0);
  wb_motor_set_velocity(wheel4, 0.0);

  cmd_vel_msg.linear.x = 0.0;
  cmd_vel_msg.angular.z = 0.0;

  cmd_vel_subscription_ = node->create_subscription<geometry_msgs::msg::Twist>(
    "cmd_vel", 1,
    [this](const geometry_msgs::msg::Twist::SharedPtr msg) {
      cmd_vel_msg = *msg;
    });
}

void DriveController::step() {
  const double forward_speed = cmd_vel_msg.linear.x;
  const double turn_speed = cmd_vel_msg.angular.z;

  const double left_speed = forward_speed - turn_speed;
  const double right_speed = forward_speed + turn_speed;

  wb_motor_set_velocity(wheel1, left_speed);
  wb_motor_set_velocity(wheel2, right_speed);
  wb_motor_set_velocity(wheel3, left_speed);
  wb_motor_set_velocity(wheel4, right_speed);
}

}  // namespace webots_ros2_prototype_integration_test

#include "pluginlib/class_list_macros.hpp"
PLUGINLIB_EXPORT_CLASS(
  webots_ros2_prototype_integration_test::DriveController,
  webots_ros2_driver::PluginInterface)