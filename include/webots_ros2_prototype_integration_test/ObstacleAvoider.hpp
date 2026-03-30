#ifndef OBSTACLE_AVOIDER_HPP
#define OBSTACLE_AVOIDER_HPP

#include "rclcpp/rclcpp.hpp"
#include "sensor_msgs/msg/range.hpp"
#include "geometry_msgs/msg/twist.hpp"

namespace webots_ros2_prototype_integration_test {

class ObstacleAvoider : public rclcpp::Node {
public:
  ObstacleAvoider();

private:
  void leftSensorCallback(const sensor_msgs::msg::Range::SharedPtr msg);
  void rightSensorCallback(const sensor_msgs::msg::Range::SharedPtr msg);
  void controlLoop();

  rclcpp::Subscription<sensor_msgs::msg::Range>::SharedPtr left_sensor_subscription_;
  rclcpp::Subscription<sensor_msgs::msg::Range>::SharedPtr right_sensor_subscription_;
  rclcpp::Publisher<geometry_msgs::msg::Twist>::SharedPtr cmd_vel_publisher_;
  rclcpp::TimerBase::SharedPtr timer_;

  double left_range_;
  double right_range_;
  bool left_received_;
  bool right_received_;
};

}  // namespace webots_ros2_prototype_integration_test

#endif