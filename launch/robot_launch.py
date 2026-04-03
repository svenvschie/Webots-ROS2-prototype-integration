import os
import launch
from launch import LaunchDescription
from launch.actions import DeclareLaunchArgument, ExecuteProcess
from launch.conditions import IfCondition
from launch.substitutions import LaunchConfiguration
from launch_ros.actions import Node
from ament_index_python.packages import get_package_share_directory
from webots_ros2_driver.webots_launcher import WebotsLauncher


def generate_launch_description():
    package_dir = get_package_share_directory('webots_ros2_prototype_integration_test')
    robot_description_path = os.path.join(
        package_dir, 'urdf', 'my_robot.urdf'
    )

    use_obstacle_avoider = LaunchConfiguration('use_obstacle_avoider')
    webots_ip = LaunchConfiguration('webots_ip')

    webots = WebotsLauncher(
        world=os.path.join(package_dir, 'worlds', '4_wheeled_robot.wbt')
    )

    robot_driver = ExecuteProcess(
        output='screen',
        cmd=[
            os.path.join(
                get_package_share_directory('webots_ros2_driver'),
                'scripts',
                'webots-controller'
            ),
            '--robot-name=my_robot',
            '--protocol=tcp',
            ['--ip-address=', webots_ip],
            '--port=1234',
            'ros2',
            '--ros-args',
            '-p',
            ['robot_description:=', robot_description_path],
        ],
        name='webots_controller_my_robot',
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
        DeclareLaunchArgument(
            'webots_ip',
            default_value='127.0.0.1',
            description='IP address of the machine running Webots'
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