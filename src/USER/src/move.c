#include "headfile.h"
#include "move.h"

int _speed = 4000;
int errorL = 0;
int errorR = 0;
void car_move(int8_t f){

    int left_speed = _speed - f * 20 + errorL * 0.02;
    int right_speed = _speed + f * 20 - errorR * 0.02;
    errorL = left_speed;
    errorR = right_speed;
    if(left_speed < 0){
        left_speed = 0;
    }
    if(right_speed < 0){
        right_speed = 0;
    }
    motor_forward(left, left_speed);
    motor_forward(right, right_speed);
}
