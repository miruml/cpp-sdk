## ROS 2
In an effort to making switching to miru as painless as possible, we're using an
interface similar to ros2 parameters where possible. Quick diffs between rolling (the
latest version of ROS in development), jazzy (latest stable version of ROS), and
humble (the latest LTS version of ROS) are trivial. We'll be using the latest tagged
version of rolling as a reference for the ros2 interface:
- ParameterClient:
  https://github.com/ros2/rclcpp/blob/a0a2a067d84fd6a38ab4f71b691d51ca5aa97ba5/rclcpp/include/rclcpp/parameter_client.hpp 
- Parameter:
  https://github.com/ros2/rclcpp/blob/a0a2a067d84fd6a38ab4f71b691d51ca5aa97ba5/rclcpp/include/rclcpp/parameter.hpp 
- ParameterValue:
  https://github.com/ros2/rclcpp/blob/a0a2a067d84fd6a38ab4f71b691d51ca5aa97ba5/rclcpp/include/rclcpp/parameter_value.hpp 

