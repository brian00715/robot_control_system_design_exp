#include "../include/pid_s.h"

PID_s::PID_s()
{
    kp = 0.1;
    ki=0;
    kd=0;
    int_max = 99;
    ctrl_max = 99;
    int_time = 1;
    err = 0;
    delta_err = 0;
    last_delta_err = 0;
    last_err = 0;
    int_count = 0;
    
}


PID_s::PID_s(float _kp,float _ki,float _kd,float _int_max,float _int_time,float _ctrl_max)
{
    kp = _kp;
    ki = _ki;
    kd = _kd;
    int_max = _int_max;
    int_time = _int_time;
    ctrl_max=_ctrl_max;
    err = 0;
    delta_err = 0;
    last_delta_err = 0;
    last_err = 0;
    int_count=0;
}

void PID_s::setGain(float _kp,float _ki,float _kd)
{
    kp = _kp;
    ki = _ki;
    kd = _kd;
}

/**
 * @brief 计算当前控制量
 * 
 * @param now_value 当前值
 * @param target_value 期望值
 * @return float 
 */
float PID_s::getOutput(float now_value,float target_value)
{
    err = target_value-now_value;
    int_count  = (int_count+err)*int_time;
    if(int_count>int_max) // 积分限幅
    {
        int_count = int_max;
    }
    else if (int_count<-int_max)
    {
        int_count = -int_max;
    }
    delta_err = err-last_err;
    last_err = err;
    float ctrl_value = err*kp+int_count*ki+delta_err*kd;
    if(ctrl_value>ctrl_max) // 控制量限幅
    {
        ctrl_value = ctrl_max;
    }
    else if (ctrl_value<-ctrl_max)
    {
        ctrl_value = -ctrl_max;
    }
    
    return ctrl_value;
    
}