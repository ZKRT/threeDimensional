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

uint8_t camera_ack_flag;
uint8_t can_value;

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
				if (sub_throw_zkrt_packet_can1_rx.data[0] == 1) 			//拍照
				{
					action_paizhao(1);
					camera_ack_flag = 1;															
				}
				else if (sub_throw_zkrt_packet_can1_rx.data[0] == 0) //停止拍照
				{
					action_paizhao(0);
					camera_ack_flag = 0;
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
