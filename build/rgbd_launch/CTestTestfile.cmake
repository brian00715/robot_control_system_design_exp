# CMake generated Testfile for 
# Source directory: /home/simon/catkin_ws/src/rgbd_launch
# Build directory: /home/simon/catkin_ws/build/rgbd_launch
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test(_ctest_rgbd_launch_roslaunch-check_launch "/home/simon/catkin_ws/build/catkin_generated/env_cached.sh" "/usr/bin/python2" "/opt/ros/melodic/share/catkin/cmake/test/run_tests.py" "/home/simon/catkin_ws/build/test_results/rgbd_launch/roslaunch-check_launch.xml" "--return-code" "/usr/bin/cmake -E make_directory /home/simon/catkin_ws/build/test_results/rgbd_launch" "/opt/ros/melodic/share/roslaunch/cmake/../scripts/roslaunch-check -o \"/home/simon/catkin_ws/build/test_results/rgbd_launch/roslaunch-check_launch.xml\" \"/home/simon/catkin_ws/src/rgbd_launch/launch\" ")
add_test(_ctest_rgbd_launch_roslaunch-check_test "/home/simon/catkin_ws/build/catkin_generated/env_cached.sh" "/usr/bin/python2" "/opt/ros/melodic/share/catkin/cmake/test/run_tests.py" "/home/simon/catkin_ws/build/test_results/rgbd_launch/roslaunch-check_test.xml" "--return-code" "/usr/bin/cmake -E make_directory /home/simon/catkin_ws/build/test_results/rgbd_launch" "/opt/ros/melodic/share/roslaunch/cmake/../scripts/roslaunch-check -o \"/home/simon/catkin_ws/build/test_results/rgbd_launch/roslaunch-check_test.xml\" \"/home/simon/catkin_ws/src/rgbd_launch/test\" ")
add_test(_ctest_rgbd_launch_rostest_test_camera.test "/home/simon/catkin_ws/build/catkin_generated/env_cached.sh" "/usr/bin/python2" "/opt/ros/melodic/share/catkin/cmake/test/run_tests.py" "/home/simon/catkin_ws/build/test_results/rgbd_launch/rostest-test_camera.xml" "--return-code" "/usr/bin/python2 /opt/ros/melodic/share/rostest/cmake/../../../bin/rostest --pkgdir=/home/simon/catkin_ws/src/rgbd_launch --package=rgbd_launch --results-filename test_camera.xml --results-base-dir \"/home/simon/catkin_ws/build/test_results\" /home/simon/catkin_ws/src/rgbd_launch/test/camera.test ")
