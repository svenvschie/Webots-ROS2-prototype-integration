# Run cmd_vel in existing container

Keep your Webots + ROS launch running.

## Open a new terminal

~~~bash
docker ps
~~~

Find your container name or ID.

## Attach to it

~~~bash
docker exec -it <container_name_or_id> bash
~~~

## Source ROS

~~~bash
source /opt/ros/jazzy/setup.bash
source ~/robot-swarm/install/setup.bash
~~~

## Publish cmd_vel

~~~bash
ros2 topic pub -r 10 /cmd_vel geometry_msgs/msg/Twist "
linear:
  x: 0.2
angular:
  z: 0.0
"
~~~