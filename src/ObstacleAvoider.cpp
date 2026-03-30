#include "webots_ros2_prototype_integration_test/ObstacleAvoider.hpp"

#include <chrono>
#include <memory>
#include <functional>

using namespace std::chrono_literals;

namespace webots_ros2_prototype_integration_test {

ObstacleAvoider::ObstacleAvoider()
: Node("obstacle_avoider"),
  left_range_(0.0),
  right_range_(0.0),
  left_received_(false),
  right_received_(false) {

  left_sensor_subscription_ = create_subscription<sensor_msgs::msg::Range>(
    "/left_sensor", 10,
    std::bind(&ObstacleAvoider::leftSensorCallback, this, std::placeholders::_1));

  right_sensor_subscription_ = create_subscription<sensor_msgs::msg::Range>(
    "/right_sensor", 10,
    std::bind(&ObstacleAvoider::rightSensorCallback, this, std::placeholders::_1));

  cmd_vel_publisher_ = create_publisher<geometry_msgs::msg::Twist>("/cmd_vel", 10);

  timer_ = create_wall_timer(
    100ms,
    std::bind(&ObstacleAvoider::controlLoop, this));
}

void ObstacleAvoider::leftSensorCallback(const sensor_msgs::msg::Range::SharedPtr msg) {
  left_range_ = msg->range;
  left_received_ = true;
}

void ObstacleAvoider::rightSensorCallback(const sensor_msgs::msg::Range::SharedPtr msg) {
  right_range_ = msg->range;
  right_received_ = true;
}

void ObstacleAvoider::controlLoop() {
  if (!left_received_ || !right_received_) {
    return;
  }

  geometry_msgs::msg::Twist cmd;
  const double obstacle_threshold = 0.08;

  if (left_range_ < obstacle_threshold || right_range_ < obstacle_threshold) {
    cmd.linear.x = 0.0;
    cmd.angular.z = 1.0;
  } else {
    cmd.linear.x = 2.0;
    cmd.angular.z = 0.0;
  }

  cmd_vel_publisher_->publish(cmd);
}

}  // namespace webots_ros2_prototype_integration_test

int main(int argc, char **argv) {
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<webots_ros2_prototype_integration_test::ObstacleAvoider>());
  rclcpp::shutdown();
  return 0;
}