/**********************************************************************************************************
 * @文件     Attitude_control.c
 * @说明     姿态控制函数
 * @作者     YuyingJin
 * @网站     https://yuyingjin0111.github.io/
 * @日期     2018 ~
**********************************************************************************************************/
#include "Attitude_control.h"
FPS_AttitudeControl FPSAttitudeControl;
/**********************************************************************************************************
*函 数 名: Attitude_Control
*功能说明: 飞行内环控制，包括姿态内环和高度内环控制
*形    参: 角速度测量值
*返 回 值: 无
**********************************************************************************************************/
Vector3f_t Attitude_InnerController(Vector3f_t ExpectGyro, Vector3f_t EstimateGyro){
	OS_ERR err;
	Vector3f_t ErrorGyro,Thrust;
	//计算函数运行时间间隔
	FPSAttitudeControl.CurrentTime = (OSTimeGet(&err) - FPSAttitudeControl.LastTime) * 1e-3;
  FPSAttitudeControl.LastTime = OSTimeGet(&err);
	
	//计算角速度环控制误差：目标角速度 - 实际角速度（低通滤波后的陀螺仪测量值）
	ErrorGyro.x = ExpectGyro.x - EstimateGyro.x;
	ErrorGyro.y = ExpectGyro.y - EstimateGyro.y;
	ErrorGyro.z = ExpectGyro.z - EstimateGyro.z;
	//PID算法，计算出角速度环的控制量
	Thrust.x = PID_GetPID(&OriginalWxRate, ErrorGyro.x, FPSAttitudeControl.CurrentTime)
												+ EstimateGyro.y * (Inertia_Wz * EstimateGyro.z) - (Inertia_Wy * EstimateGyro.y) * EstimateGyro.z;//考虑轴间耦合现象
	Thrust.y = PID_GetPID(&OriginalWyRate, ErrorGyro.y, FPSAttitudeControl.CurrentTime)
												+ (-(EstimateGyro.x * (Inertia_Wz * EstimateGyro.z) - (Inertia_Wx * EstimateGyro.x) * EstimateGyro.z));//考虑轴间耦合现象
	Thrust.z = PID_GetPID(&OriginalWzRate, ErrorGyro.z, FPSAttitudeControl.CurrentTime)
												+ EstimateGyro.x * (Inertia_Wy * EstimateGyro.y) - (Inertia_Wx * EstimateGyro.x) * EstimateGyro.y;//考虑轴间耦合现象
	
	return Thrust;
}

/**********************************************************************************************************
*函 数 名: AttitudeOuterControl
*功能说明: 姿态外环控制
*形    参: 无
*返 回 值: 无
**********************************************************************************************************/
Vector3angle_t Attitude_OuterController(Vector3angle_t ExpectAngle){
	static Vector3angle_t AngleLpf;
  Vector3angle_t Angle,VIOAngle,ExpectAnguleRate,ErrorAngle;
	//获取当前飞机的姿态角(mahany filter)
  Angle = GetCopterAngle();
	//获取视觉里程计姿态
	VIOAngle = GetVisualOdometryAngle();
	//对姿态测量值进行低通滤波，减少数据噪声对控制器的影响
	AngleLpf.roll = AngleLpf.roll * 0.92f + Angle.roll * 0.08f;
	AngleLpf.pitch = AngleLpf.pitch * 0.92f + Angle.pitch * 0.08f;
	AngleLpf.yaw = VIOAngle.yaw;
	//计算姿态外环控制误差：目标角度 - 实际角度
	ErrorAngle.roll = ExpectAngle.roll - AngleLpf.roll;
	ErrorAngle.pitch = ExpectAngle.pitch - AngleLpf.pitch;
	ErrorAngle.yaw = ExpectAngle.yaw - AngleLpf.yaw;
	//PID算法，计算出姿态外环的控制量 限幅 2.5rad/s
	ExpectAnguleRate.roll = PID_GetP(&OriginalRoll,  ErrorAngle.roll);
	ExpectAnguleRate.roll = ConstrainFloat(ExpectAnguleRate.roll,-2.5,2.5); 
	ExpectAnguleRate.pitch = PID_GetP(&OriginalPitch, ErrorAngle.pitch);
	ExpectAnguleRate.pitch = ConstrainFloat(ExpectAnguleRate.pitch,-2.5,2.5);
	ExpectAnguleRate.yaw = PID_GetP(&OriginalYaw, ErrorAngle.yaw);
	ExpectAnguleRate.yaw = ConstrainFloat(ExpectAnguleRate.yaw,-2.5,2.5);
	return ExpectAnguleRate;
}

/**********************************************************************************************************
*函 数 名: GetFPSAttitudeControl
*功能说明: 获取姿态控制频率
*形    参: 无
*返 回 值: 无
**********************************************************************************************************/
float GetFPSAttitudeControl(void){
	return FPSAttitudeControl.CurrentTime;
}


