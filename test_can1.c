#include "test_can1.h"
#include "can.h"
#include "chassis_control.h"
#include "gimbal_control.h"
#include "delay.h"
  
uint8_t can1_rx_data[8];
uint8_t can2_rx_data[8];
unsigned char can2_tx_success_flag = 0;

//can filter must be initialized before use
void CanFilter_Init(CAN_HandleTypeDef* hcan)
{
  CAN_FilterConfTypeDef canfilter;
  
  //create memory to save the message, if not will raise error
  static CanTxMsgTypeDef  Tx1Message;
  static CanRxMsgTypeDef  Rx1Message;
  static CanTxMsgTypeDef  Tx2Message;
  static CanRxMsgTypeDef  Rx2Message;
  
	
  canfilter.FilterMode = CAN_FILTERMODE_IDMASK;
  canfilter.FilterScale = CAN_FILTERSCALE_32BIT;
  
  //filtrate any ID you want here
  canfilter.FilterIdHigh = 0x0000;
  canfilter.FilterIdLow = 0x0000;
  canfilter.FilterMaskIdHigh = 0x0000;
  canfilter.FilterMaskIdLow = 0x0000;
  
  canfilter.FilterFIFOAssignment = CAN_FilterFIFO0;
  canfilter.FilterActivation = ENABLE;
  canfilter.BankNumber = 14;
  
  //use different filter for can1&can2
  if(hcan == &hcan1)
  {
    canfilter.FilterNumber = 0;
    hcan->pTxMsg = &Tx1Message;
    hcan->pRxMsg = &Rx1Message;
  }
  if(hcan == &hcan2)
  {
    canfilter.FilterNumber = 14;
    hcan->pTxMsg = &Tx2Message;
    hcan->pRxMsg = &Rx2Message;
  }
  
  if(HAL_CAN_ConfigFilter(hcan, &canfilter)!=HAL_OK){
	Error_Handler();
	};
  
}

//it will be auto callback when can receive msg completely
void HAL_CAN_RxCpltCallback(CAN_HandleTypeDef* hcan)
{ 
	__HAL_CAN_ENABLE_IT(hcan, CAN_IT_FMP0);
	//printf("%d\n",hcan->pRxMsg->StdId);
	if(hcan==&hcan1){
		switch(hcan->pRxMsg->StdId)
  {
		case 0x201:
            {
                RM3510_1.thisPosition = (hcan->pRxMsg->Data[0]<<8)|(hcan->pRxMsg->Data[1]);
                RM3510_1.thisVelocity = (hcan->pRxMsg->Data[2]<<8)|(hcan->pRxMsg->Data[3]);	

						}break;	
		case 0x202:
            {
                RM3510_2.thisPosition = (hcan->pRxMsg->Data[0]<<8)|(hcan->pRxMsg->Data[1]);
                RM3510_2.thisVelocity = (hcan->pRxMsg->Data[2]<<8)|(hcan->pRxMsg->Data[3]);							
						}break;	
		case 0x203:
            {
                RM3510_3.thisPosition = (hcan->pRxMsg->Data[0]<<8)|(hcan->pRxMsg->Data[1]);
                RM3510_3.thisVelocity = (hcan->pRxMsg->Data[2]<<8)|(hcan->pRxMsg->Data[3]);
						}break;	
		case 0x204:
            {
                RM3510_4.thisPosition = (hcan->pRxMsg->Data[0]<<8)|(hcan->pRxMsg->Data[1]);
                RM3510_4.thisVelocity = (hcan->pRxMsg->Data[2]<<8)|(hcan->pRxMsg->Data[3]);	
						}break;	
	}
	}
	if(hcan==&hcan2){
	switch(hcan->pRxMsg->StdId)
  {		
		case 0x205:
            {
                             //�����̨���0x205������ֵ 
							yaw.thisAngle_Raw = (hcan->pRxMsg->Data[0]<<8)|hcan->pRxMsg->Data[1];     //��е�Ƕ�
							yaw.thisCurrent = (hcan->pRxMsg->Data[2]<<8)|hcan->pRxMsg->Data[3];      //ʵ�ʵ���ֵ
							yaw.targetCurrent = (hcan->pRxMsg->Data[4]<<8)|hcan->pRxMsg->Data[5];     //�����ĵ���ֵ
							yaw.thisAngle=yaw.thisAngle_Raw;
							if((yaw.thisAngle & 0x1000) == 0x1000)
							{
								yaw.thisAngle = yaw.thisAngle_Raw - 8191 ;   //��205��ֵ��Ϊ������������
							}  		
							yaw.thisAngle = 3000-yaw.thisAngle;
						}break;	
		case 0x206:
            {
             //�����̨���0x206������ֵ  
							pitch.thisAngle_Raw = (hcan->pRxMsg->Data[0]<<8)|hcan->pRxMsg->Data[1];     //��е�Ƕ�
							pitch.thisCurrent = (hcan->pRxMsg->Data[2]<<8)|hcan->pRxMsg->Data[3];      //ʵ�ʵ���ֵ
							pitch.targetCurrent = (hcan->pRxMsg->Data[4]<<8)|hcan->pRxMsg->Data[5];     //�����ĵ���ֵ
							pitch.thisAngle=pitch.thisAngle_Raw;
							if((pitch.thisAngle & 0x1000) == 0x1000)
							{
								pitch.thisAngle = pitch.thisAngle_Raw - 8191;   //��205��ֵ��Ϊ������������
							}  
							pitch.thisAngle = 3000-pitch.thisAngle;
						}break;	
  }
	}
	HAL_CAN_Receive_IT(&hcan1, CAN_FIFO0);
  HAL_CAN_Receive_IT(&hcan2, CAN_FIFO0);
}


void Cmd_ESC_820R(int16_t current_201,int16_t current_202,int16_t current_203,int16_t current_204)
{   
	  CAN_HandleTypeDef* hcan=&hcan1;
		hcan->pTxMsg->StdId = TEST_CAN1_ID ;
		hcan->pTxMsg->IDE = CAN_ID_STD;
		hcan->pTxMsg->RTR = CAN_RTR_DATA;
		hcan->pTxMsg->DLC = 0x08;
    hcan->pTxMsg->Data[0] = (unsigned char)(current_201 >> 8);
    hcan->pTxMsg->Data[1] = (unsigned char)current_201;
    hcan->pTxMsg->Data[2] = (unsigned char)(current_202 >> 8);
    hcan->pTxMsg->Data[3] = (unsigned char)current_202;
    hcan->pTxMsg->Data[4] = (unsigned char)(current_203 >> 8);
		hcan->pTxMsg->Data[5] = (unsigned char)current_203;
    hcan->pTxMsg->Data[6] = (unsigned char)(current_204 >> 8);
    hcan->pTxMsg->Data[7] = (unsigned char)current_204;
    HAL_CAN_Transmit(hcan, 10);
}


//RM35�������ROBOMODULE���ͺ���
/****************************************************************************************
                                       ��λָ��
*****************************************************************************************/
void CAN_RoboModule_DRV_Reset(unsigned char Group,unsigned char Number)
{   CAN_HandleTypeDef* hcan=&hcan1;
    unsigned short can_id = 0x000;  
		hcan->pTxMsg->StdId = TEST_CAN1_ID ;
		hcan->pTxMsg->IDE = CAN_ID_STD;
		hcan->pTxMsg->RTR = CAN_RTR_DATA;
    
    switch(Group)
    {
        case 0x0:
        case 0x1:
        case 0x2:
        case 0x3:
        case 0x4:
        case 0x5:
        case 0x6:
        case 0x7: can_id |= Group<<8; break;
        default: return;
    }
    
    switch(Number)
    {
        case 0x0:
        case 0x1:
        case 0x2:
        case 0x3:
        case 0x4:
        case 0x5:
        case 0x6:
        case 0x7:
        case 0x8:
        case 0x9:
        case 0xA:
        case 0xB:
        case 0xC:
        case 0xD:
        case 0xE:
        case 0xF: can_id |= Number<<4; break;
        default: return;
    }
    
    hcan->pTxMsg->StdId = can_id;      //֡IDΪ���������CAN_ID
		
    hcan->pTxMsg->Data[0] = 0x55;
    hcan->pTxMsg->Data[1] = 0x55;
    hcan->pTxMsg->Data[2] = 0x55;
    hcan->pTxMsg->Data[3] = 0x55;
    hcan->pTxMsg->Data[4] = 0x55;
    hcan->pTxMsg->Data[5] = 0x55;
    hcan->pTxMsg->Data[6] = 0x55;
    hcan->pTxMsg->Data[7] = 0x55;
    can2_tx_success_flag = 0;
		HAL_CAN_Transmit(hcan, 10);
    while(can2_tx_success_flag == 0);
}

/****************************************************************************************
                                     ģʽѡ��ָ��
mode��ȡֵ��Χ���£�
PWM_MODE
PWM_CURRENT_MODE
PWM_VELOCITY_MODE
PWM_POSITION_MODE
PWM_VELOCITY_POSITION_MODE
CURRENT_VELOCITY_MODE
CURRENT_POSITION_MODE
CURRENT_VELOCITY_POSITION_MODE
*****************************************************************************************/
void CAN_RoboModule_DRV_Mode_Choice(unsigned char Group,unsigned char Number,unsigned char Mode)
{   CAN_HandleTypeDef* hcan=&hcan1;
    unsigned short can_id = 0x001;
    
    hcan->pTxMsg->IDE = CAN_ID_STD;    //��׼֡
    hcan->pTxMsg->RTR = CAN_RTR_DATA;  //����֡
    hcan->pTxMsg->DLC = 0x08;          //֡����Ϊ8
    
    switch(Group)
    {
        case 0x0:
        case 0x1:
        case 0x2:
        case 0x3:
        case 0x4:
        case 0x5:
        case 0x6:
        case 0x7: can_id |= Group<<8; break;
        default: return;
    }
    
    switch(Number)
    {
        case 0x0:
        case 0x1:
        case 0x2:
        case 0x3:
        case 0x4:
        case 0x5:
        case 0x6:
        case 0x7:
        case 0x8:
        case 0x9:
        case 0xA:
        case 0xB:
        case 0xC:
        case 0xD:
        case 0xE:
        case 0xF: can_id |= Number<<4; break;
        default: return;
    }
    
    hcan->pTxMsg->StdId = can_id;      //֡IDΪ���������CAN_ID
    
    hcan->pTxMsg->Data[0] = Mode;
    hcan->pTxMsg->Data[1] = 0x55;
    hcan->pTxMsg->Data[2] = 0x55;
    hcan->pTxMsg->Data[3] = 0x55;
    hcan->pTxMsg->Data[4] = 0x55;
    hcan->pTxMsg->Data[5] = 0x55;
    hcan->pTxMsg->Data[6] = 0x55;
    hcan->pTxMsg->Data[7] = 0x55;
    can2_tx_success_flag = 0;
		HAL_CAN_Transmit(hcan, 10);
    while(can2_tx_success_flag == 0);
}

/****************************************************************************************
                                  PWMģʽ�µ�����ָ��
temp_pwm��ȡֵ��Χ���£�
-5000 ~ +5000
*****************************************************************************************/
void CAN_RoboModule_DRV_PWM_Mode(unsigned char Group,unsigned char Number,short Temp_PWM)
{
    unsigned short can_id = 0x002;
    CAN_HandleTypeDef* hcan=&hcan1;
    
    hcan->pTxMsg->IDE = CAN_ID_STD;    //��׼֡
    hcan->pTxMsg->RTR = CAN_RTR_DATA;  //����֡
    hcan->pTxMsg->DLC = 0x08;          //֡����Ϊ8
    
    switch(Group)
    {
        case 0x0:
        case 0x1:
        case 0x2:
        case 0x3:
        case 0x4:
        case 0x5:
        case 0x6:
        case 0x7: can_id |= Group<<8; break;
        default: return;
    }
    
    switch(Number)
    {
        case 0x0:
        case 0x1:
        case 0x2:
        case 0x3:
        case 0x4:
        case 0x5:
        case 0x6:
        case 0x7:
        case 0x8:
        case 0x9:
        case 0xA:
        case 0xB:
        case 0xC:
        case 0xD:
        case 0xE:
        case 0xF: can_id |= Number<<4; break;
        default: return;
    }
    
    hcan->pTxMsg->StdId = can_id;      //֡IDΪ���������CAN_ID

    if(Temp_PWM > 5000)
    {
        Temp_PWM = 5000;
    }
    else if(Temp_PWM < -5000)
    {
        Temp_PWM = -5000;
    }
    
    hcan->pTxMsg->Data[0] = (unsigned char)((Temp_PWM>>8)&0xff);
    hcan->pTxMsg->Data[1] = (unsigned char)(Temp_PWM&0xff);
    hcan->pTxMsg->Data[2] = 0x55;
    hcan->pTxMsg->Data[3] = 0x55;
    hcan->pTxMsg->Data[4] = 0x55;
    hcan->pTxMsg->Data[5] = 0x55;
    hcan->pTxMsg->Data[6] = 0x55;
    hcan->pTxMsg->Data[7] = 0x55;
    
    can2_tx_success_flag = 0;
    HAL_CAN_Transmit(hcan, 10);
    while(can2_tx_success_flag == 0);
}


/****************************************************************************************
                                  PWM����ģʽ�µ�����ָ��
temp_pwm��ȡֵ��Χ���£�
0 ~ +5000

temp_current��ȡֵ��Χ���£�
-1600 ~ +1600
*****************************************************************************************/
void CAN_RoboModule_DRV_PWM_Current_Mode(unsigned char Group,unsigned char Number,short Temp_PWM,short Temp_Current)
{
    unsigned short can_id = 0x003;
    CAN_HandleTypeDef* hcan=&hcan1;
    
    hcan->pTxMsg->IDE = CAN_ID_STD;    //��׼֡
    hcan->pTxMsg->RTR = CAN_RTR_DATA;  //����֡
    hcan->pTxMsg->DLC = 0x08;          //֡����Ϊ8
    
    switch(Group)
    {
        case 0x0:
        case 0x1:
        case 0x2:
        case 0x3:
        case 0x4:
        case 0x5:
        case 0x6:
        case 0x7: can_id |= Group<<8; break;
        default: return;
    }
    
    switch(Number)
    {
        case 0x0:
        case 0x1:
        case 0x2:
        case 0x3:
        case 0x4:
        case 0x5:
        case 0x6:
        case 0x7:
        case 0x8:
        case 0x9:
        case 0xA:
        case 0xB:
        case 0xC:
        case 0xD:
        case 0xE:
        case 0xF: can_id |= Number<<4; break;
        default: return;
    }
    
    hcan->pTxMsg->StdId = can_id;      //֡IDΪ���������CAN_ID

    if(Temp_PWM > 5000)
    {
        Temp_PWM = 5000;
    }
    else if(Temp_PWM < -5000)
    {
        Temp_PWM = -5000;
    }
    
    if(Temp_PWM < 0)
    {
        Temp_PWM = abs(Temp_PWM);
    }
    
    if(Temp_Current > 2000)
    {
        Temp_Current = 2000;
    }
    else if(Temp_Current < -2000)
    {
        Temp_Current = -2000;
    }
    
    hcan->pTxMsg->Data[0] = (unsigned char)((Temp_PWM>>8)&0xff);
    hcan->pTxMsg->Data[1] = (unsigned char)(Temp_PWM&0xff);
    hcan->pTxMsg->Data[2] = (unsigned char)((Temp_Current>>8)&0xff);
    hcan->pTxMsg->Data[3] = (unsigned char)(Temp_Current&0xff);
    hcan->pTxMsg->Data[4] = 0x55;
    hcan->pTxMsg->Data[5] = 0x55;
    hcan->pTxMsg->Data[6] = 0x55;
    hcan->pTxMsg->Data[7] = 0x55;
    
    can2_tx_success_flag = 0;
    HAL_CAN_Transmit(hcan, 10);
    while(can2_tx_success_flag == 0);
}

/****************************************************************************************
                                  PWM�ٶ�ģʽ�µ�����ָ��
temp_pwm��ȡֵ��Χ���£�
0 ~ +5000

temp_velocity��ȡֵ��Χ���£�
-32768 ~ +32767
*****************************************************************************************/
void CAN_RoboModule_DRV_PWM_Velocity_Mode(unsigned char Group,unsigned char Number,short Temp_PWM,short Temp_Velocity)
{   
    unsigned short can_id = 0x004;
    CAN_HandleTypeDef* hcan=&hcan1;
    
    hcan->pTxMsg->IDE = CAN_ID_STD;    //��׼֡
    hcan->pTxMsg->RTR = CAN_RTR_DATA;  //����֡
    hcan->pTxMsg->DLC = 0x08;          //֡����Ϊ8
    
    switch(Group)
    {
        case 0x0:
        case 0x1:
        case 0x2:
        case 0x3:
        case 0x4:
        case 0x5:
        case 0x6:
        case 0x7: can_id |= Group<<8; break;
        default: return;
    }
    
    switch(Number)
    {
        case 0x0:
        case 0x1:
        case 0x2:
        case 0x3:
        case 0x4:
        case 0x5:
        case 0x6:
        case 0x7:
        case 0x8:
        case 0x9:
        case 0xA:
        case 0xB:
        case 0xC:
        case 0xD:
        case 0xE:
        case 0xF: can_id |= Number<<4; break;
        default: return;
    }
    
    hcan->pTxMsg->StdId = can_id;      //֡IDΪ���������CAN_ID

    if(Temp_PWM > 5000)
    {
        Temp_PWM = 5000;
    }
    else if(Temp_PWM < -5000)
    {
        Temp_PWM = -5000;
    }
    
    if(Temp_PWM < 0)
    {
        Temp_PWM = abs(Temp_PWM);
    }
    
    hcan->pTxMsg->Data[0] = (unsigned char)((Temp_PWM>>8)&0xff);
    hcan->pTxMsg->Data[1] = (unsigned char)(Temp_PWM&0xff);
    hcan->pTxMsg->Data[2] = (unsigned char)((Temp_Velocity>>8)&0xff);
    hcan->pTxMsg->Data[3] = (unsigned char)(Temp_Velocity&0xff);
    hcan->pTxMsg->Data[4] = 0x55;
    hcan->pTxMsg->Data[5] = 0x55;
    hcan->pTxMsg->Data[6] = 0x55;
    hcan->pTxMsg->Data[7] = 0x55;
    
    can2_tx_success_flag = 0;
    HAL_CAN_Transmit(hcan, 10);
    while(can2_tx_success_flag == 0);

}

/****************************************************************************************
                                  PWMλ��ģʽ�µ�����ָ��
temp_pwm��ȡֵ��Χ���£�
0 ~ +5000

temp_position��ȡֵ��Χ���£�
32λ�з�������
*****************************************************************************************/
void CAN_RoboModule_DRV_PWM_Position_Mode(unsigned char Group,unsigned char Number,short Temp_PWM,long Temp_Position)
{
    unsigned short can_id = 0x005;
    CAN_HandleTypeDef* hcan=&hcan1;
    
    hcan->pTxMsg->IDE = CAN_ID_STD;    //��׼֡
    hcan->pTxMsg->RTR = CAN_RTR_DATA;  //����֡
    hcan->pTxMsg->DLC = 0x08;          //֡����Ϊ8
    
    switch(Group)
    {
        case 0x0:
        case 0x1:
        case 0x2:
        case 0x3:
        case 0x4:
        case 0x5:
        case 0x6:
        case 0x7: can_id |= Group<<8; break;
        default: return;
    }
    
    switch(Number)
    {
        case 0x0:
        case 0x1:
        case 0x2:
        case 0x3:
        case 0x4:
        case 0x5:
        case 0x6:
        case 0x7:
        case 0x8:
        case 0x9:
        case 0xA:
        case 0xB:
        case 0xC:
        case 0xD:
        case 0xE:
        case 0xF: can_id |= Number<<4; break;
        default: return;
    }
    
    hcan->pTxMsg->StdId = can_id;      //֡IDΪ���������CAN_ID

    if(Temp_PWM > 5000)
    {
        Temp_PWM = 5000;
    }
    else if(Temp_PWM < -5000)
    {
        Temp_PWM = -5000;
    }
    
    if(Temp_PWM < 0)
    {
        Temp_PWM = abs(Temp_PWM);
    }
    
    hcan->pTxMsg->Data[0] = (unsigned char)((Temp_PWM>>8)&0xff);
    hcan->pTxMsg->Data[1] = (unsigned char)(Temp_PWM&0xff);
    hcan->pTxMsg->Data[2] = 0x55;
    hcan->pTxMsg->Data[3] = 0x55;
    hcan->pTxMsg->Data[4] = (unsigned char)((Temp_Position>>24)&0xff);
    hcan->pTxMsg->Data[5] = (unsigned char)((Temp_Position>>16)&0xff);
    hcan->pTxMsg->Data[6] = (unsigned char)((Temp_Position>>8)&0xff);
    hcan->pTxMsg->Data[7] = (unsigned char)(Temp_Position&0xff);
    
    can2_tx_success_flag = 0;
    HAL_CAN_Transmit(hcan, 10);
    while(can2_tx_success_flag == 0);
}

/****************************************************************************************
                                  PWM�ٶ�λ��ģʽ�µ�����ָ��
temp_pwm��ȡֵ��Χ���£�
0 ~ +5000

temp_velocity��ȡֵ��Χ���£�
0 ~ +32767

temp_position��ȡֵ��Χ���£�
32λ�з�������
*****************************************************************************************/
void CAN_RoboModule_DRV_PWM_Velocity_Position_Mode(unsigned char Group,unsigned char Number,short Temp_PWM,short Temp_Velocity,long Temp_Position)
{
    unsigned short can_id = 0x006;
    CAN_HandleTypeDef* hcan=&hcan1;
    
    hcan->pTxMsg->IDE = CAN_ID_STD;    //��׼֡
    hcan->pTxMsg->RTR = CAN_RTR_DATA;  //����֡
    hcan->pTxMsg->DLC = 0x08;          //֡����Ϊ8
    
    switch(Group)
    {
        case 0x0:
        case 0x1:
        case 0x2:
        case 0x3:
        case 0x4:
        case 0x5:
        case 0x6:
        case 0x7: can_id |= Group<<8; break;
        default: return;
    }
    
    switch(Number)
    {
        case 0x0:
        case 0x1:
        case 0x2:
        case 0x3:
        case 0x4:
        case 0x5:
        case 0x6:
        case 0x7:
        case 0x8:
        case 0x9:
        case 0xA:
        case 0xB:
        case 0xC:
        case 0xD:
        case 0xE:
        case 0xF: can_id |= Number<<4; break;
        default: return;
    }
    
    hcan->pTxMsg->StdId = can_id;      //֡IDΪ���������CAN_ID

    if(Temp_PWM > 5000)
    {
        Temp_PWM = 5000;
    }
    else if(Temp_PWM < -5000)
    {
        Temp_PWM = -5000;
    }
    
    if(Temp_PWM < 0)
    {
        Temp_PWM = abs(Temp_PWM);
    }
    
    if(Temp_Velocity < 0)
    {
        Temp_Velocity = abs(Temp_Velocity);
    }
    
    hcan->pTxMsg->Data[0] = (unsigned char)((Temp_PWM>>8)&0xff);
    hcan->pTxMsg->Data[1] = (unsigned char)(Temp_PWM&0xff);
    hcan->pTxMsg->Data[2] = (unsigned char)((Temp_Velocity>>8)&0xff);
    hcan->pTxMsg->Data[3] = (unsigned char)(Temp_Velocity&0xff);
    hcan->pTxMsg->Data[4] = (unsigned char)((Temp_Position>>24)&0xff);
    hcan->pTxMsg->Data[5] = (unsigned char)((Temp_Position>>16)&0xff);
    hcan->pTxMsg->Data[6] = (unsigned char)((Temp_Position>>8)&0xff);
    hcan->pTxMsg->Data[7] = (unsigned char)(Temp_Position&0xff);
    
    can2_tx_success_flag = 0;
    HAL_CAN_Transmit(hcan, 10);
    while(can2_tx_success_flag == 0);
}

/****************************************************************************************
                                  �����ٶ�ģʽ�µ�����ָ��
temp_current��ȡֵ��Χ���£�
0 ~ +2000

temp_velocity��ȡֵ��Χ���£�
-32768 ~ +32767
*****************************************************************************************/
void CAN_RoboModule_DRV_Current_Velocity_Mode(unsigned char Group,unsigned char Number,short Temp_Current,short Temp_Velocity)
{
    unsigned short can_id = 0x007;
    CAN_HandleTypeDef* hcan=&hcan1;
    
    hcan->pTxMsg->IDE = CAN_ID_STD;    //��׼֡
    hcan->pTxMsg->RTR = CAN_RTR_DATA;  //����֡
    hcan->pTxMsg->DLC = 0x08;          //֡����Ϊ8
    
    switch(Group)
    {
        case 0x0:
        case 0x1:
        case 0x2:
        case 0x3:
        case 0x4:
        case 0x5:
        case 0x6:
        case 0x7: can_id |= Group<<8; break;
        default: return;
    }
    
    switch(Number)
    {
        case 0x0:
        case 0x1:
        case 0x2:
        case 0x3:
        case 0x4:
        case 0x5:
        case 0x6:
        case 0x7:
        case 0x8:
        case 0x9:
        case 0xA:
        case 0xB:
        case 0xC:
        case 0xD:
        case 0xE:
        case 0xF: can_id |= Number<<4; break;
        default: return;
    }
    
    hcan->pTxMsg->StdId = can_id;      //֡IDΪ���������CAN_ID

    if(Temp_Current > 2000)
    {
        Temp_Current = 2000;
    }
    else if(Temp_Current < -2000)
    {
        Temp_Current = -2000;
    }
    
    if(Temp_Current < 0)
    {
        Temp_Current = abs(Temp_Current);
    }
    
    hcan->pTxMsg->Data[0] = (unsigned char)((Temp_Current>>8)&0xff);
    hcan->pTxMsg->Data[1] = (unsigned char)(Temp_Current&0xff);
    hcan->pTxMsg->Data[2] = (unsigned char)((Temp_Velocity>>8)&0xff);
    hcan->pTxMsg->Data[3] = (unsigned char)(Temp_Velocity&0xff);
    hcan->pTxMsg->Data[4] = 0x55;
    hcan->pTxMsg->Data[5] = 0x55;
    hcan->pTxMsg->Data[6] = 0x55;
    hcan->pTxMsg->Data[7] = 0x55;
    
    can2_tx_success_flag = 0;
    HAL_CAN_Transmit(hcan, 10);
    while(can2_tx_success_flag == 0);
}

/****************************************************************************************
                                  ����λ��ģʽ�µ�����ָ��
temp_current��ȡֵ��Χ���£�
0 ~ +2000

temp_position��ȡֵ��Χ���£�
32λ�з�������
*****************************************************************************************/
void CAN_RoboModule_DRV_Current_Position_Mode(unsigned char Group,unsigned char Number,short Temp_Current,long Temp_Position)
{
    unsigned short can_id = 0x008;
    CAN_HandleTypeDef* hcan=&hcan1;
    
    hcan->pTxMsg->IDE = CAN_ID_STD;    //��׼֡
    hcan->pTxMsg->RTR = CAN_RTR_DATA;  //����֡
    hcan->pTxMsg->DLC = 0x08;          //֡����Ϊ8
    
    switch(Group)
    {
        case 0x0:
        case 0x1:
        case 0x2:
        case 0x3:
        case 0x4:
        case 0x5:
        case 0x6:
        case 0x7: can_id |= Group<<8; break;
        default: return;
    }
    
    switch(Number)
    {
        case 0x0:
        case 0x1:
        case 0x2:
        case 0x3:
        case 0x4:
        case 0x5:
        case 0x6:
        case 0x7:
        case 0x8:
        case 0x9:
        case 0xA:
        case 0xB:
        case 0xC:
        case 0xD:
        case 0xE:
        case 0xF: can_id |= Number<<4; break;
        default: return;
    }
    
    hcan->pTxMsg->StdId = can_id;      //֡IDΪ���������CAN_ID

    if(Temp_Current > 2000)
    {
        Temp_Current = 2000;
    }
    else if(Temp_Current < -2000)
    {
        Temp_Current = -2000;
    }
    
    if(Temp_Current < 0)
    {
        Temp_Current = abs(Temp_Current);
    }

    hcan->pTxMsg->Data[0] = (unsigned char)((Temp_Current>>8)&0xff);
    hcan->pTxMsg->Data[1] = (unsigned char)(Temp_Current&0xff);
    hcan->pTxMsg->Data[2] = 0x55;
    hcan->pTxMsg->Data[3] = 0x55;
    hcan->pTxMsg->Data[4] = (unsigned char)((Temp_Position>>24)&0xff);
    hcan->pTxMsg->Data[5] = (unsigned char)((Temp_Position>>16)&0xff);
    hcan->pTxMsg->Data[6] = (unsigned char)((Temp_Position>>8)&0xff);
    hcan->pTxMsg->Data[7] = (unsigned char)(Temp_Position&0xff);
    
    can2_tx_success_flag = 0;
    HAL_CAN_Transmit(hcan, 10);
    while(can2_tx_success_flag == 0);
}

/****************************************************************************************
                                  �����ٶ�λ��ģʽ�µ�����ָ��
temp_current��ȡֵ��Χ���£�
0 ~ +2000

temp_velocity��ȡֵ��Χ���£�
0 ~ +32767

temp_position��ȡֵ��Χ���£�
32λ�з�������
*****************************************************************************************/
void CAN_RoboModule_DRV_Current_Velocity_Position_Mode(unsigned char Group,unsigned char Number,short Temp_Current,short Temp_Velocity,long Temp_Position)
{
    unsigned short can_id = 0x009;
    CAN_HandleTypeDef* hcan=&hcan1;
    
    hcan->pTxMsg->IDE = CAN_ID_STD;    //��׼֡
    hcan->pTxMsg->RTR = CAN_RTR_DATA;  //����֡
    hcan->pTxMsg->DLC = 0x08;          //֡����Ϊ8
    
    switch(Group)
    {
        case 0x0:
        case 0x1:
        case 0x2:
        case 0x3:
        case 0x4:
        case 0x5:
        case 0x6:
        case 0x7: can_id |= Group<<8; break;
        default: return;
    }
    
    switch(Number)
    {
        case 0x0:
        case 0x1:
        case 0x2:
        case 0x3:
        case 0x4:
        case 0x5:
        case 0x6:
        case 0x7:
        case 0x8:
        case 0x9:
        case 0xA:
        case 0xB:
        case 0xC:
        case 0xD:
        case 0xE:
        case 0xF: can_id |= Number<<4; break;
        default: return;
    }
    
    hcan->pTxMsg->StdId = can_id;      //֡IDΪ���������CAN_ID

    if(Temp_Current > 2000)
    {
        Temp_Current = 2000;
    }
    else if(Temp_Current < -2000)
    {
        Temp_Current = -2000;
    }
    
    if(Temp_Current < 0)
    {
        Temp_Current = abs(Temp_Current);
    }

    hcan->pTxMsg->Data[0] = (unsigned char)((Temp_Current>>8)&0xff);
    hcan->pTxMsg->Data[1] = (unsigned char)(Temp_Current&0xff);
    hcan->pTxMsg->Data[2] = (unsigned char)((Temp_Velocity>>8)&0xff);
    hcan->pTxMsg->Data[3] = (unsigned char)(Temp_Velocity&0xff);
    hcan->pTxMsg->Data[4] = (unsigned char)((Temp_Position>>24)&0xff);
    hcan->pTxMsg->Data[5] = (unsigned char)((Temp_Position>>16)&0xff);
    hcan->pTxMsg->Data[6] = (unsigned char)((Temp_Position>>8)&0xff);
    hcan->pTxMsg->Data[7] = (unsigned char)(Temp_Position&0xff);
    
    can2_tx_success_flag = 0;
    HAL_CAN_Transmit(hcan, 10);
    while(can2_tx_success_flag == 0);
}

/****************************************************************************************
                                      ����ָ��
Temp_Time��ȡֵ��Χ: 0 ~ 255��Ϊ0ʱ��Ϊ�رյ����ٶ�λ�÷�������
Ctl1_Ctl2��ȡֵ��Χ��0 or 1 ������Ϊ0 or 1������Ϊ��0��Ϊ�ر�������λ��⹦��
�ر���ʾ��Ctl1��Ctl2�Ĺ��ܽ�������RMDS-102������汾��������Ctl1_Ctl2 = 0 ����
*****************************************************************************************/
void CAN_RoboModule_DRV_Config(unsigned char Group,unsigned char Number,unsigned char Temp_Time,unsigned char Ctl1_Ctl2)
{
    unsigned short can_id = 0x00A;
    CAN_HandleTypeDef* hcan=&hcan1;
    
    hcan->pTxMsg->IDE = CAN_ID_STD;    //��׼֡
    hcan->pTxMsg->RTR = CAN_RTR_DATA;  //����֡
    hcan->pTxMsg->DLC = 0x08;          //֡����Ϊ8
    
    switch(Group)
    {
        case 0x0:
        case 0x1:
        case 0x2:
        case 0x3:
        case 0x4:
        case 0x5:
        case 0x6:
        case 0x7: can_id |= Group<<8; break;
        default: return;
    }
    
    switch(Number)
    {
        case 0x1: //��ָ�֧�ֹ㲥�����Դ˴�û��0��������
        case 0x2:
        case 0x3:
        case 0x4:
        case 0x5:
        case 0x6:
        case 0x7:
        case 0x8:
        case 0x9:
        case 0xA:
        case 0xB:
        case 0xC:
        case 0xD:
        case 0xE:
        case 0xF: can_id |= Number<<4; break;
        default: return;
    }
    
    if((Ctl1_Ctl2 != 0x00)&&(Ctl1_Ctl2 != 0x01))
    {
        Ctl1_Ctl2 = 0x00;
    }
    
    hcan->pTxMsg->StdId = can_id;      //֡IDΪ���������CAN_ID
    
    hcan->pTxMsg->Data[0] = Temp_Time;
    hcan->pTxMsg->Data[1] = Ctl1_Ctl2;
    hcan->pTxMsg->Data[2] = 0x55;
    hcan->pTxMsg->Data[3] = 0x55;
    hcan->pTxMsg->Data[4] = 0x55;
    hcan->pTxMsg->Data[5] = 0x55;
    hcan->pTxMsg->Data[6] = 0x55;
    hcan->pTxMsg->Data[7] = 0x55;
    
    can2_tx_success_flag = 0;
    HAL_CAN_Transmit(hcan, 10);
    while(can2_tx_success_flag == 0);
}

/****************************************************************************************
                                      ���߼��
*****************************************************************************************/
void CAN_RoboModule_DRV_Online_Check(unsigned char Group,unsigned char Number)
{
    unsigned short can_id = 0x00F;
    CAN_HandleTypeDef* hcan=&hcan1;
    
    hcan->pTxMsg->IDE = CAN_ID_STD;    //��׼֡
    hcan->pTxMsg->RTR = CAN_RTR_DATA;  //����֡
    hcan->pTxMsg->DLC = 0x08;          //֡����Ϊ8
    
    switch(Group)
    {
        case 0x0:
        case 0x1:
        case 0x2:
        case 0x3:
        case 0x4:
        case 0x5:
        case 0x6:
        case 0x7: can_id |= Group<<8; break;
        default: return;
    }
    
    switch(Number)
    {
        case 0x1: //��ָ�֧�ֹ㲥�����Դ˴�û��0��������
        case 0x2:
        case 0x3:
        case 0x4:
        case 0x5:
        case 0x6:
        case 0x7:
        case 0x8:
        case 0x9:
        case 0xA:
        case 0xB:
        case 0xC:
        case 0xD:
        case 0xE:
        case 0xF: can_id |= Number<<4; break;
        default: return;
    }
    
    hcan->pTxMsg->StdId = can_id;      //֡IDΪ���������CAN_ID
    
    hcan->pTxMsg->Data[0] = 0x55;
    hcan->pTxMsg->Data[1] = 0x55;
    hcan->pTxMsg->Data[2] = 0x55;
    hcan->pTxMsg->Data[3] = 0x55;
    hcan->pTxMsg->Data[4] = 0x55;
    hcan->pTxMsg->Data[5] = 0x55;
    hcan->pTxMsg->Data[6] = 0x55;
    hcan->pTxMsg->Data[7] = 0x55;
    
    can2_tx_success_flag = 0;
    HAL_CAN_Transmit(hcan, 10);
    while(can2_tx_success_flag == 0);
}


