#ifndef TQBOT_WEBOTS__TQBOT_DRIVER_HPP_
#define TQBOT_WEBOTS__TQBOT_DRIVER_HPP_

#include <geometry_msgs/msg/twist.hpp>
#include <rclcpp/rclcpp.hpp>

#include <unordered_map>
#include <string>

#include <webots_ros2_driver/PluginInterface.hpp>
#include <webots_ros2_driver/WebotsNode.hpp>
#include <webots/types.h>

namespace tqbot_webots
{

class TQBotDriver : public webots_ros2_driver::PluginInterface
{
public:
  TQBotDriver();

  void init(
    webots_ros2_driver::WebotsNode * node,
    std::unordered_map<std::string, std::string> & parameters) override;

  void step() override;

private:
  void cmdVelCallback(const geometry_msgs::msg::Twist::SharedPtr msg);

  webots_ros2_driver::WebotsNode * node_;

  WbDeviceTag left_motor_;
  WbDeviceTag right_motor_;

  geometry_msgs::msg::Twist cmd_vel_msg_;

  rclcpp::Subscription<geometry_msgs::msg::Twist>::SharedPtr cmd_vel_subscription_;

  double wheel_radius_;
  double half_wheel_separation_;
  double max_motor_velocity_;
};

}  // namespace tqbot_webots

#endif  // TQBOT_WEBOTS__TQBOT_DRIVER_HPP_