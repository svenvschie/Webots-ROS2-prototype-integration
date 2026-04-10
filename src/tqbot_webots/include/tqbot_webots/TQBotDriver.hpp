#ifndef TQBOT_WEBOTS__TQBOT_DRIVER_HPP_
#define TQBOT_WEBOTS__TQBOT_DRIVER_HPP_

#include <geometry_msgs/msg/twist.hpp>
#include <nav_msgs/msg/odometry.hpp>
#include <rclcpp/rclcpp.hpp>
#include <tf2_ros/transform_broadcaster.h>

#include <string>
#include <unordered_map>

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
  WbNodeRef self_node_;

  geometry_msgs::msg::Twist cmd_vel_msg_;
  rclcpp::Subscription<geometry_msgs::msg::Twist>::SharedPtr cmd_vel_subscription_;

  rclcpp::Publisher<nav_msgs::msg::Odometry>::SharedPtr odom_publisher_;
  std::unique_ptr<tf2_ros::TransformBroadcaster> tf_broadcaster_;

  double wheel_radius_;
  double half_wheel_separation_;
  double max_motor_velocity_;

  std::string odom_frame_id_;
  std::string base_frame_id_;
};

}  // namespace tqbot_webots

#endif