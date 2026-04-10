cmake_minimum_required(VERSION 3.8)
project(tqbot_webots)

if(NOT CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 17)
endif()

find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(geometry_msgs REQUIRED)
find_package(pluginlib REQUIRED)
find_package(webots_ros2_driver REQUIRED)

pluginlib_export_plugin_description_file(webots_ros2_driver tqbot_webots_driver.xml)

add_library(${PROJECT_NAME} SHARED
  src/TQBotDriver.cpp
)

target_include_directories(${PROJECT_NAME}
  PRIVATE
    include
)

ament_target_dependencies(${PROJECT_NAME}
  rclcpp
  geometry_msgs
  pluginlib
  webots_ros2_driver
)

install(
  TARGETS ${PROJECT_NAME}
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib
  RUNTIME DESTINATION bin
)

install(
  DIRECTORY include/
  DESTINATION include/
)

install(
  FILES tqbot_webots_driver.xml
  DESTINATION share/${PROJECT_NAME}
)

install(
  DIRECTORY launch urdf worlds
  DESTINATION share/${PROJECT_NAME}
)

ament_export_include_directories(include)
ament_export_libraries(${PROJECT_NAME})

ament_package()