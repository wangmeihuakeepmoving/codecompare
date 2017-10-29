/**
  ******************************************************************************
  * File Name          : main.h
  * Description        : This file contains the common defines of the application
  ******************************************************************************
  *
  * COPYRIGHT(c) 2017 STMicroelectronics
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H
  /* Includes ------------------------------------------------------------------*/

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private define ------------------------------------------------------------*/

#define IST_INT_Pin GPIO_PIN_3
#define IST_INT_GPIO_Port GPIOE
#define IST_RST_Pin GPIO_PIN_2
#define IST_RST_GPIO_Port GPIOE
#define LASER_Pin GPIO_PIN_13
#define LASER_GPIO_Port GPIOG
#define KEY_Pin GPIO_PIN_10
#define KEY_GPIO_Port GPIOD
#define LED_RED_Pin GPIO_PIN_14
#define LED_RED_GPIO_Port GPIOF
#define LED_GREEN_Pin GPIO_PIN_7
#define LED_GREEN_GPIO_Port GPIOE
/* USER CODE BEGIN Private defines */
//���ú��� ȡ���ֵ�;���ֵ
#define abs(x) ((x)>0? (x):(-(x)))
#define maxs(a,b) (a>b? a:b)
//���峵��,��Ҫ��Ӧ�������̨
#define CAR_B
//�����Զ���
#define AUTO_TRACK
//CAR1 RM35-ROBOMODULE_NEW
//CAR2 RM35-ROBOMODULE_NEW
//CAR3 RM3510

//���幤�������ģʽ
//#define DEBUG

//���ݳ��Ŷ���
#ifdef CAR_HERO   //Ӣ�۳�
	//������̵��
	#define RM3510 

	//��̨�Ƕ�
	#define YAW_LEFT -850
	#define YAW_RIGHT	3150
	#define YAW_MID 2095
	#define PITCH_UP 2400
	#define PITCH_DOWN 1320
	#define PITCH_MID 1550
	//��̨PID
	#define YAW_205_VEL_P 8.0
	#define YAW_205_VEL_D 0.0
	#define YAW_205_POS_P 2.0
	#define YAW_205_POS_I 0.01
	#define YAW_205_POS_D 0.1
	#define PITCH_206_VEL_P 20.0
	#define PITCH_206_VEL_D 0.0
	#define PITCH_206_POS_P 1.0
	#define PITCH_206_POS_I 0.0
	#define PITCH_206_POS_D 1.0
	
	//����RM3510��pid
	#define ESC_820R_VEL_P 10.0//10.0
	#define ESC_820R_VEL_I 0.0
	#define ESC_820R_VEL_D 0.5//0.5
	#define ESC_820R_POS_P 10.0//10.0
	#define ESC_820R_POS_I 0.0
	#define ESC_820R_POS_D 0.0


	#define SHOOT_SPEED  10   //0-10000
#endif

#ifdef CAR_A
	//������̵��
	#define RM3510 

	//��̨�Ƕ�
	#define YAW_LEFT 240//545
	#define YAW_RIGHT	2000//5887
	#define YAW_MID 1050//3250
	#define PITCH_UP 3000//1100
	#define PITCH_DOWN 2150//-350
	#define PITCH_MID 2500//200
	//��̨PID
	#define YAW_205_VEL_P 8.0
	#define YAW_205_VEL_D 0.0
	#define YAW_205_POS_P 2.0
	#define YAW_205_POS_I 0.01
	#define YAW_205_POS_D 0.1
	#define PITCH_206_VEL_P 10.0
	#define PITCH_206_VEL_D 0.0
	#define PITCH_206_POS_P 1.0
	#define PITCH_206_POS_I 0.0
	#define PITCH_206_POS_D 0.5
	
	//����RM3510��pid
	#define ESC_820R_VEL_P 10.0//10.0
	#define ESC_820R_VEL_I 0.0
	#define ESC_820R_VEL_D 0.5//0.5
	#define ESC_820R_POS_P 10.0//10.0
	#define ESC_820R_POS_I 0.0
	#define ESC_820R_POS_D 0.0

	#define SHOOT_SPEED  13   //0-10000
#endif

#ifdef CAR_B
	//������̵��
	#define RM3510 

	//��̨�Ƕ�
	#define YAW_LEFT 1700
	#define YAW_RIGHT	3700
	#define YAW_MID 2830
	#define PITCH_UP 5400
	#define PITCH_DOWN 4350
	#define PITCH_MID 4900
	//��̨PID
	#define YAW_205_VEL_P 8.0
	#define YAW_205_VEL_D 0.0
	#define YAW_205_POS_P 2.0
	#define YAW_205_POS_I 0.01
	#define YAW_205_POS_D 0.1
	#define PITCH_206_VEL_P 10.0
	#define PITCH_206_VEL_D 0.0
	#define PITCH_206_POS_P 1.0
	#define PITCH_206_POS_I 0.0
	#define PITCH_206_POS_D 0.5
	
	//����RM3510��pid
	#define ESC_820R_VEL_P 10.0//10.0
	#define ESC_820R_VEL_I 0.0
	#define ESC_820R_VEL_D 0.5//0.5
	#define ESC_820R_POS_P 10.0//10.0
	#define ESC_820R_POS_I 0.0
	#define ESC_820R_POS_D 0.0

	#define SHOOT_SPEED  10   //0-10000
#endif

#ifdef CAR_C
	//������̵��
	#define RM3510 

	//��̨�Ƕ�
	#define YAW_LEFT 4000
	#define YAW_RIGHT	6150
	#define YAW_MID 4625
	#define PITCH_UP 6600
	#define PITCH_DOWN 5500
	#define PITCH_MID 5680
	//��̨PID
	#define YAW_205_VEL_P 9.0
	#define YAW_205_VEL_D 0.0
	#define YAW_205_POS_P 2.0
	#define YAW_205_POS_I 0.01
	#define YAW_205_POS_D 0.1
	#define PITCH_206_VEL_P 11.0
	#define PITCH_206_VEL_D 0.0
	#define PITCH_206_POS_P 1.0
	#define PITCH_206_POS_I 0.0
	#define PITCH_206_POS_D 0.5
	
	//����RM3510��pid
#define ESC_820R_VEL_P 10.0//10.0
#define ESC_820R_VEL_I 0.0
#define ESC_820R_VEL_D 0.5//0.5
#define ESC_820R_POS_P 10.0//10.0
#define ESC_820R_POS_I 0.0
#define ESC_820R_POS_D 0.0

	#define SHOOT_SPEED  5   //0-10000
#endif

//�����˶��ٶȶ���
 #define NormalSpeed 180
 #define HighSpeed 230
 #define LowSpeed 50
 
 #define ch_up 1684
 #define ch_mid 1024
 #define ch_down 423

//��������������Ϳ���֪����RM35�����
//#ifdef ROBOMODULE_NEW
//#define RM35
//#endif

//#ifdef ROBOMODULE_OLD
//#define RM35
//#endif


//֮�������ϰ�
//����Ħ�����ٶ�
#define RUB_SPEED    1400  //1000-2200

#define PWM1  TIM12->CCR1
#define PWM2  TIM12->CCR2
#define PWM3  TIM2->CCR1

/* USER CODE END Private defines */

/**
  * @}
  */ 

/**
  * @}
*/ 

#endif /* __MAIN_H */
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
