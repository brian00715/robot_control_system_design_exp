#ifndef PID_H_
#define PID_H_

class PID_s
{
private:
    float kp;
    float ki;
    float kd;
    float int_max;
    float ctrl_max;
    float last_delta_err;
    float err;
    float int_time;
    float delta_err;
    float last_err;
    float int_count;
public:
    PID_s();
    PID_s(float _kp,float _ki,float _kd,float _int_max,float _int_time,float _ctrl_max);
    float getOutput(float now_value,float target_value);
    void setGain(float _kp,float _ki,float _kd);
};

#endif