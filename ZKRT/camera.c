/**
  ******************************************************************************
  * @file    camera.c 
  * @author  ZKRT
  * @version V0.0.1
  * @date    09-February-2017
  * @brief   三维吊舱控制
  *          + (1) init
  ******************************************************************************
  * @attention
  *
  * ...
  *
  ******************************************************************************
  */
	
/* Includes ------------------------------------------------------------------*/
#include "camera.h"
#include "zkrt.h"
#include "can.h"
#include "pwm.h"

#define PHOTO_FLAG    1
#define PWR_FLAG      2
#define CAMERA_FLAG   3

#define OPEN_VALUE    1
#define CLOSE_VALUE   0

uint8_t camera_ack_flag;
uint8_t can_value;
uint8_t ctrl_flag;
uint8_t ctrl_vlaue;

//在子模块中，逐个字节的判断接收的数据，对zkrt指令解包，然后是设置的就设置完了返回响应，是读取的就读取完了返回值，将它封装成zkrt包，再返回
zkrt_packet_t sub_throw_zkrt_packet_can1_rx;
void sub_camera_zkrt_recv_decode_and_zkrt_encode_ack(void)
{
	while (CAN1_rx_check() == 1)
	{
		can_value = CAN1_rx_byte();
		if (zkrt_decode_char(&sub_throw_zkrt_packet_can1_rx,can_value)==1)//每个字符都判断一下，当接收到完整的数据时返回1
		{
			if(sub_throw_zkrt_packet_can1_rx.command == DEFAULT_NUM)
			{
				ctrl_flag = sub_throw_zkrt_packet_can1_rx.data[0];
				ctrl_vlaue = sub_throw_zkrt_packet_can1_rx.data[1];
				switch(ctrl_flag)
				{
					case PHOTO_FLAG:
						action_paizhao(ctrl_vlaue);
						camera_ack_flag = ctrl_vlaue;
						break;
					case PWR_FLAG:
						action_vedio_pwr_ctrl(ctrl_vlaue);
						break;
					case CAMERA_FLAG:
						action_camera_ctrl(ctrl_vlaue);
						break;
					default:break;
				}	
			}
		}
	}
}
//mode=1是拍照，PWM值1100；mode=0是停止拍照，PWM值是1500。
void action_paizhao(uint8_t mode)
{
	if (mode == 1)
	{
		PWM_DONGZUO(_PWM_TAKEPHOTO);
	}
	else
	{
		PWM_DONGZUO(_PWM_CLOSETAKE);
	}
}
//mode=1是打开电源；mode=0是关闭电源
void action_vedio_pwr_ctrl(uint8_t mode)
{
	GPIO_SetBits(GPIOB, GPIO_Pin_4);
	delay_ms(100);
	GPIO_ResetBits(GPIOB, GPIO_Pin_4);	
}
//mode=1是打开镜头；mode=0是关闭镜头
void action_camera_ctrl(uint8_t mode)
{
	GPIO_SetBits(GPIOA, GPIO_Pin_15);
	delay_ms(100);	
	GPIO_ResetBits(GPIOA, GPIO_Pin_15);
}
