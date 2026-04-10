import os

from ament_index_python.packages import get_package_share_directory

from launch import LaunchDescription
from launch.actions import DeclareLaunchArgument, ExecuteProcess
from launch.substitutions import LaunchConfiguration
from launch_ros.actions import Node

import xacro


def generate_launch_description():
    tqbot_webots_dir = get_package_share_directory('tqbot_webots')
    webots_driver_dir = get_package_share_directory('webots_ros2_driver')

    use_sim_time = LaunchConfiguration('use_sim_time')
    webots_ip = LaunchConfiguration('webots_ip')
    webots_port = LaunchConfiguration('webots_port')

    xacro_file = os.path.join(
        tqbot_webots_dir,
        'urdf',
        'tqbot_webots.urdf.xacro'
    )

    robot_description = xacro.process_file(xacro_file).toxml()

    robot_state_publisher = Node(
        package='robot_state_publisher',
        executable='robot_state_publisher',
        output='screen',
        parameters=[{
            'robot_description': robot_description,
            'use_sim_time': use_sim_time
        }]
    )

    tqbot_driver = ExecuteProcess(
        output='screen',
        cmd=[
            os.path.join(webots_driver_dir, 'scripts', 'webots-controller'),
            '--robot-name=tqbot',
            '--protocol=tcp',
            ['--ip-address=', webots_ip],
            ['--port=', webots_port],
            'ros2',
            '--ros-args',
            '-p',
            ['robot_description:=', robot_description],
            '-p',
            ['use_sim_time:=', use_sim_time],
            '-p',
            'set_robot_state_publisher:=False',
        ],
        name='webots_controller_tqbot',
        respawn=True,
    )

    return LaunchDescription([
        DeclareLaunchArgument(
            'use_sim_time',
            default_value='true',
            description='Use simulation time'
        ),
        DeclareLaunchArgument(
            'webots_ip',
            default_value='host.docker.internal',
            description='IP or hostname of the machine running Webots'
        ),
        DeclareLaunchArgument(
            'webots_port',
            default_value='1234',
            description='TCP port used by Webots controller'
        ),
        robot_state_publisher,
        tqbot_driver,
    ])