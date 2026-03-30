cmake_minimum_required(VERSION 3.8)
project(webots_ros2_prototype_integration_test)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(webots_ros2_driver REQUIRED)
find_package(geometry_msgs REQUIRED)
find_package(sensor_msgs REQUIRED)
find_package(std_msgs REQUIRED)
find_package(pluginlib REQUIRED)

add_library(
  drive_controller SHARED
  src/DriveController.cpp
)

target_include_directories(
  drive_controller
  PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/include
)

ament_target_dependencies(
  drive_controller
  rclcpp
  geometry_msgs
  sensor_msgs
  std_msgs
  pluginlib
  webots_ros2_driver
)

pluginlib_export_plugin_description_file(
  webots_ros2_driver
  resource/webots_ros2_prototype_integration_test_plugins.xml
)

add_executable(
  obstacle_avoider
  src/ObstacleAvoider.cpp
)

target_include_directories(
  obstacle_avoider
  PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/include
)

ament_target_dependencies(
  obstacle_avoider
  rclcpp
  geometry_msgs
  sensor_msgs
)

install(
  TARGETS
    drive_controller
  LIBRARY DESTINATION lib
)

install(
  TARGETS
    obstacle_avoider
  DESTINATION lib/${PROJECT_NAME}
)

install(
  DIRECTORY
    include
    resource
    urdf
    worlds
    launch
  DESTINATION share/${PROJECT_NAME}
)

if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  set(ament_cmake_copyright_FOUND TRUE)
  set(ament_cmake_cpplint_FOUND TRUE)
  ament_lint_auto_find_test_dependencies()
endif()

ament_package()