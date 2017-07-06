/**
  ******************************************************************************
  * @file    camera.c 
  * @author  ZKRT
  * @version V0.0.1
  * @date    09-February-2017
  * @brief   ��ά���տ���
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

//����ģ���У�����ֽڵ��жϽ��յ����ݣ���zkrtָ������Ȼ�������õľ��������˷�����Ӧ���Ƕ�ȡ�ľͶ�ȡ���˷���ֵ��������װ��zkrt�����ٷ���
zkrt_packet_t sub_throw_zkrt_packet_can1_rx;
void sub_camera_zkrt_recv_decode_and_zkrt_encode_ack(void)
{
	while (CAN1_rx_check() == 1)
	{
		can_value = CAN1_rx_byte();
		if (zkrt_decode_char(&sub_throw_zkrt_packet_can1_rx,can_value)==1)//ÿ���ַ����ж�һ�£������յ�����������ʱ����1
		{
			if(sub_throw_zkrt_packet_can1_rx.command == DEFAULT_NUM)
			{
				if (sub_throw_zkrt_packet_can1_rx.data[0] == 1) 			//����
				{
					action_paizhao(1);
					camera_ack_flag = 1;															
				}
				else if (sub_throw_zkrt_packet_can1_rx.data[0] == 0) //ֹͣ����
				{
					action_paizhao(0);
					camera_ack_flag = 0;
				}
			}
		}
	}
}
//mode=1�����գ�PWMֵ1100��mode=0��ֹͣ���գ�PWMֵ��1500��
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
