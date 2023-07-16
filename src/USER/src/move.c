#include "headfile.h"
#include "move.h"

int _speed = 4000;
int errorL = 0;
int errorR = 0;


//PID结构体
typedef struct{
	int16 Current_Speed;//—————————————————————————————————————————————————当前速度
	int16 Target_Speed;//——————————————————————————————————————————————————目标速度
	int16 Encoder;//———————————————————————————————————————————————————————编码器值
	int16 E;//—————————————————————————————————————————————————————————————本次的偏差
	int16 E_L;//———————————————————————————————————————————————————————————上一次的偏差
	int16 E_L_L;//—————————————————————————————————————————————————————————上上次的偏差
	int16 KP;//————————————————————————————————————————————————————————————比例系数
	int16 KI;//————————————————————————————————————————————————————————————积分系数
	int16 KD;//————————————————————————————————————————————————————————————微分系数
	int16 PIDOUT;//————————————————————————————————————————————————————————PID输出
	int16 Target_Speed_old;
  int16 Current_Speed_last;	
  int16 Current_Speed_llast;		
}PID_struct;
 
PID_struct Motor1;
PID_struct Motor2;
 
/*==================================================================
* 函数名: Motor_Parameters_Init(PID_struct* Motor)
* 功能: 速度控制PID参数初始化
* 输入: void
* 输出: void
* 用法: Motor_Parameters_Init(Motor);
* 修改记录:
===================================================================*/
void Motor_Parameters_Init(PID_struct *Motor)
{
	
	Motor->Current_Speed   = 0;
	Motor->Target_Speed   = 0;
	Motor->Encoder        = 0;
	Motor->E            = 0;
	Motor->E_L          = 0;
	Motor->E_L_L        = 0;
	Motor->KP           = 400;      //350   300
	Motor->KI           = 80;       //60    45
	Motor->KD           = 60;        //60    65
	Motor->PIDOUT       = 0;
	Motor->Target_Speed_old = 0;
  Motor->Current_Speed_last = 0;	
  Motor->Current_Speed_llast = 0;	
}
 
void Motor_Parameters_Init2(PID_struct *Motor)
{
	
	Motor->Current_Speed   = 0;
	Motor->Target_Speed   = 0;
	Motor->Encoder        = 0;
	Motor->E            = 0;
	Motor->E_L          = 0;
	Motor->E_L_L        = 0;
	Motor->KP           = 350;      //350   300
	Motor->KI           = 60;       //60    45
	Motor->KD           = 60;        //60    65
	Motor->PIDOUT       = 0;
	Motor->Target_Speed_old = 0;
  Motor->Current_Speed_last = 0;	
  Motor->Current_Speed_llast = 0;	
}

/************************************************************     
【函数名称】Motor_Parameters_Init(PID_struct* Motor)
【功    能】增量式PID控制
【参    数】PID结构体
【返 回 值】输出占空比
【实    例】Motro_PID_Control(&Motor)
【注意事项】无
************************************************************/
short Motro_PID_Control(PID_struct* Motor)
{
  int duty;
  //占空比计算
  duty = Motor->KP*(Motor->E-Motor->E_L)+Motor->KI*Motor->E+Motor->KD*(Motor->E+Motor->E_L_L-2*Motor->E_L);
  //更新偏差
  Motor->E_L_L = Motor->E_L;
  Motor->E_L = Motor->E;
  Motor->PIDOUT+=duty;
  //限幅
 
  if(Motor->PIDOUT>10000)
    Motor->PIDOUT=10000;
  else if(Motor->PIDOUT<-10000)
    Motor->PIDOUT=-10000;
  return Motor->PIDOUT;
}

/*-----------------------------------------------------------                            
【函数名称】：Motor_Ctrl
【功    能】：动力PWM输出
【传入参数】：num 电机编号 1-2    duty占空比  0-10000对应0%-100%
【返回参数】：无
【实    例】：Motor_Ctrl(1, 2000);
【注意事项】：无
-----------------------------------------------------------*/
void Motor_Ctrl(uint8 num, int16 duty)
{
 if(num==1)
 {
  if(duty>0)
  {
   pwm_duty(L_PWM_F, 0);
   pwm_duty(L_PWM_B, duty*5);
  }
  else
  {
   pwm_duty(L_PWM_F, -duty*5);
   pwm_duty(L_PWM_F, 0);
  }
 }
 if(num==2)
 {
  if(duty>0)
  {
   pwm_duty(R_PWM_F, 0);
   pwm_duty(R_PWM_B, duty*5);
  }
  else
  {
   pwm_duty(R_PWM_F, -duty*5);
   pwm_duty(R_PWM_B, 0);
  }
 }
}

// void car_move(int8_t f){

//     int left_speed = _speed - f * 20 + errorL * 0.02;
//     int right_speed = _speed + f * 20 - errorR * 0.02;
//     errorL = left_speed;
//     errorR = right_speed;
//     if(left_speed < 0){
//         left_speed = 0;
//     }
//     if(right_speed < 0){
//         right_speed = 0;
//     }
//     motor_forward(left, left_speed);
//     motor_forward(right, right_speed);
// }