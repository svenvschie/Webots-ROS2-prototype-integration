import os
import launch
from launch import LaunchDescription
from launch.actions import DeclareLaunchArgument
from launch.conditions import IfCondition
from launch.substitutions import LaunchConfiguration
from launch_ros.actions import Node
from ament_index_python.packages import get_package_share_directory
from webots_ros2_driver.webots_launcher import WebotsLauncher
from webots_ros2_driver.webots_controller import WebotsController


def generate_launch_description():
    package_dir = get_package_share_directory('webots_ros2_prototype_integration_test')
    robot_description_path = os.path.join(
        package_dir, 'urdf', 'my_robot.urdf'
    )

    use_obstacle_avoider = LaunchConfiguration('use_obstacle_avoider')

    webots = WebotsLauncher(
        world=os.path.join(package_dir, 'worlds', '4_wheeled_robot.wbt')
    )

    robot_driver = WebotsController(
        robot_name='my_robot',
        parameters=[
            {'robot_description': robot_description_path},
        ]
    )

    obstacle_avoider = Node(
        package='webots_ros2_prototype_integration_test',
        executable='obstacle_avoider',
        output='screen',
        condition=IfCondition(use_obstacle_avoider)
    )

    return LaunchDescription([
        DeclareLaunchArgument(
            'use_obstacle_avoider',
            default_value='false',
            description='Launch the obstacle avoider node'
        ),
        webots,
        robot_driver,
        obstacle_avoider,
        launch.actions.RegisterEventHandler(
            event_handler=launch.event_handlers.OnProcessExit(
                target_action=webots,
                on_exit=[launch.actions.EmitEvent(event=launch.events.Shutdown())],
            )
        )
    ])