#!/usr/bin/env python
import dynamic_reconfigure.client
import rospy
if __name__=="__main__":
    rospy.init_node("reconfig_py")
    client = dynamic_reconfigure.client.Client( "/camera/driver" )
    params = {'color_mode': '9'}
    config = client.update_configuration(params)
