#include "pid_algorithm.h"
#include "main.h"
#include "gimbal_control.h"

#define GAP 0.0f
/********************************************************************************
                         pitch��������ٶȻ�����
                    ���� pitch�ᵱǰ�ٶ� pitch��Ŀ���ٶ�
//	#define YAW_205_VEL_P 8.0
//	#define YAW_205_VEL_D 0.0
//	#define YAW_205_POS_P 2.0
//	#define YAW_205_POS_I 0.01
//	#define YAW_205_POS_D 0.1
//	#define PITCH_206_VEL_P 10.0
//	#define PITCH_206_VEL_D 0.0
//	#define PITCH_206_POS_P 1.0
//	#define PITCH_206_POS_I 0.0
//	#define PITCH_206_POS_D 3.0
*********************************************************************************/
float Velocity_Control_206(float current_velocity_206,float target_velocity_206)
{
    const float v_p =PITCH_206_VEL_P;
    const float v_d =PITCH_206_VEL_D;
    static float velocity[2]={0.0,0.0};
    static float error_v[2] = {0.0,0.0};
    static float output = 0;
    
    velocity[0]=velocity[1];
		velocity[1]=pitch.thisAngle;
		//current_velocity_206=(velocity[1]-velocity[0])*vt;
		current_velocity_206=(float)((current_velocity_206*0.01f));
		//current_velocity_206=0;
		if(abs(current_velocity_206) < GAP)
    {
        current_velocity_206 = 0.0;
    }
    error_v[0] = error_v[1];
    error_v[1] = target_velocity_206 - current_velocity_206;
    
    output = error_v[1] * v_p             
             + (error_v[1] - error_v[0]) * v_d;
     
    if(output > ESC_MAX)
    {
        output = ESC_MAX;
    }
    
    if(output < -ESC_MAX)
    {
        output = -ESC_MAX;
    }
    
    return -output;//cyq:for6015 ����
}


/********************************************************************************
                         pitch�������λ�û����� 
                    ���� pitch�ᵱǰλ�� pitch��Ŀ��λ��
*********************************************************************************/
float Position_Control_206(float current_position_206,float target_position_206)
{
	
    const float l_p = PITCH_206_POS_P;
    const float l_i = PITCH_206_POS_I;
    const float l_d = PITCH_206_POS_D;

    static float error_l[2] = {0.0,0.0};
    static float output = 0;
    static float inte = 0;
    
    error_l[0] = error_l[1];
    error_l[1] = target_position_206 - current_position_206;
    inte += error_l[1]; 
    output = error_l[1] * l_p 
            + inte * l_i 
            + (error_l[1] - error_l[0]) * l_d;
    if(output > ESC_MAX)
    {
        output = ESC_MAX;
    }
    
    if(output < -ESC_MAX)
    {
        output = -ESC_MAX;
    }
    		
    return -output;
}
/********************************************************************************
                           yaw��������ٶȻ�����
                      ���� yaw�ᵱǰ�ٶ� yaw��Ŀ���ٶ�
*********************************************************************************/
float Velocity_Control_205(float current_velocity_205,float target_velocity_205)
{   
//	#define YAW_205_VEL_P 20.0
//	#define YAW_205_VEL_D 0.0
//	#define YAW_205_POS_P 2.0
//	#define YAW_205_POS_I 0.0
//	#define YAW_205_POS_D 0.1
    const float v_p = YAW_205_VEL_P;
    const float v_d = YAW_205_VEL_D;
	  static float velocity[2]={0.0,0.0};
    static float error_v[2] = {0.0,0.0};
    static float output = 0;
		velocity[0]=velocity[1];
		velocity[1]=yaw.thisAngle;
		current_velocity_205=current_velocity_205*0.01f;
    if(abs(current_velocity_205) < GAP)
    {
        current_velocity_205 = 0.0;
    }
    
    error_v[0] = error_v[1];
    error_v[1] = target_velocity_205 - current_velocity_205;
    
    output = error_v[1] * v_p
             + (error_v[1] - error_v[0]) * v_d;
     
    if(output > ESC_MAX)
    {
        output = ESC_MAX;
    }
    
    if(output < -ESC_MAX)
    {
        output = -ESC_MAX;
    }
    
    return -output;//cyq:for6015 ����
}

/********************************************************************************
                           yaw�������λ�û�����
                      ���� yaw�ᵱǰλ�� yaw��Ŀ��λ��
*********************************************************************************/
float Position_Control_205(float current_position_205,float target_position_205)
{   
    const float l_p = 2;
		const float l_i = 0.01;
    const float l_d = 0.1;
    static float error_l[3] = {0.0,0.0,0.0};
    static float output = 0;
    
    error_l[0] = error_l[1];
    error_l[1] = error_l[2];    
    error_l[2] = target_position_205 - current_position_205;
 
    output = error_l[2] * l_p 
							+ error_l[2] * l_i 
							+ (error_l[2] - error_l[1]) * l_d;
    
    if(output > ESC_MAX)
    {
        output = ESC_MAX;
    }
    
    if(output < -ESC_MAX)
    {
        output = -ESC_MAX;
    }
    
    return -output;
}
/********************************************************************************
                           820R��������ٶȻ�����
                      ���� 820R�ᵱǰ�ٶ� 820R��Ŀ���ٶ�
*********************************************************************************/
float Velocity_Control_820R(float current_velocity_820R,float target_velocity_820R)
{
    const float l_p = ESC_820R_VEL_P;//7.0
    const float l_i = ESC_820R_VEL_I;//0.5
    const float l_d = ESC_820R_VEL_D;

    static float error_l[2] = {0.0,0.0};
    static float output = 0;
    static float inte = 0;
    
    error_l[0] = error_l[1];
    error_l[1] = target_velocity_820R - current_velocity_820R;
    inte += error_l[1]; 
    
    output = error_l[1] * l_p 
            + inte * l_i 
            + (error_l[1] - error_l[0]) * l_d;
    
    if(output > ESC_MAX)
    {
        output = ESC_MAX;
    }
    
    if(output < -ESC_MAX)
    {
        output = -ESC_MAX;
    }
    		
    return output;
}
/********************************************************************************
                           820R������λ�û�����
                      ���� 820R�ᵱǰλ�� 820R��Ŀ��λ��
*********************************************************************************/
float Position_Control_820R(float current_position_820R,float target_position_820R)
{
    const float l_p = ESC_820R_POS_P;
    const float l_i = ESC_820R_POS_I;
    const float l_d = ESC_820R_POS_D;

    static float error_l[2] = {0.0,0.0};
    static float output = 0;
    static float inte = 0;
    
    error_l[0] = error_l[1];
    error_l[1] = target_position_820R - current_position_820R;
    inte += error_l[1]; 
    
    output = error_l[1] * l_p 
            + inte * l_i 
            + (error_l[1] - error_l[0]) * l_d;
    
    if(output > ESC_MAX)
    {
        output = ESC_MAX;
    }
    
    if(output < -ESC_MAX)
    {
        output = -ESC_MAX;
    }
    		
    return output;
}
/********************************************************************************
                           ����������ٶȻ�����
                      ���� ���������ǰ�ٶ� �������Ŀ���ٶ�
*********************************************************************************/
float Velocity_Control_Shoot(float current_velocity_Shoot,float target_velocity_Shoot)
{
    //const float l_p = ESC_SHOOT_VEL_P;//7.0
    //const float l_i = ESC_SHOOT_VEL_I;//0.5
    //const float l_d = ESC_SHOOT_VEL_D;
		const float l_p = 300;//7.0
    const float l_i = 0;//0.5
    const float l_d = 10;
    static float error_l[2] = {0.0,0.0};
    static float output = 0;
    static float inte = 0;
    
    error_l[0] = error_l[1];
    error_l[1] = target_velocity_Shoot - current_velocity_Shoot;
    inte += error_l[1]; 
    
    output = (error_l[1] * l_p 
            + inte * l_i 
            + (error_l[1] - error_l[0]) * l_d);
		
    
    if(output > ESC_MAX)
    {
        output = ESC_MAX;
    }
    
    if(output < 0)
    {
        output = 0;
    }
    		
    return output;
}

