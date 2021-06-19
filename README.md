## 机器人控制系统设计”代码简介

author: 司马宽宽 张萌

date:2021/03~2021/06

---



#### 目录结构

|-- build //编译空间

|-- devel // 开发空间

|-- src // 源代码空间

​	|-- ad_sensor_package // ad传感器实验ROS包

​	|-- mecanum_control_package // 麦克纳姆轮控制ROS包

​	|-- motor_control_package // 电机控制ROS包

​	|-- omni_control_package // 横辊子全向轮底盘控制ROS包

​	|-- race_package // 结课比赛ROS包

​	|-- rf2o_laser_odometry // 激光里程计ROS包

​	|-- ros_astra_camera // 深度相机驱动包

​	|-- ros_astra_launch // 深度相机运行包

​   |-- slam_package // SLAM功能包

​	|-- teleop // 键盘控制ROS包

​	|-- velocity_control_package // 速度控制ROS包

​	|-- wpb_cv // 核心驱动包

​		|-- wpb_cv_bringup // 核心功能包，包括pc和底盘控制器的通信实现

​		|-- wpb_cv_local_planner // 自定义ROS导航局部规划器

​		|-- wpb_cv_tutorials // 例程





