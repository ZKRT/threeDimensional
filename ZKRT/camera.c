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

#define PHOTO_FLAG    1
#define PWR_FLAG      2
#define CAMERA_FLAG   3

#define OPEN_VALUE    1
#define CLOSE_VALUE   0

uint8_t camera_ack_flag;
uint8_t can_value;
uint8_t ctrl_flag;
uint8_t ctrl_vlaue;

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
//mode=1�Ǵ򿪵�Դ��mode=0�ǹرյ�Դ
void action_vedio_pwr_ctrl(uint8_t mode)
{
	GPIO_SetBits(GPIOB, GPIO_Pin_4);
	delay_ms(100);
	GPIO_ResetBits(GPIOB, GPIO_Pin_4);	
}
//mode=1�Ǵ򿪾�ͷ��mode=0�ǹرվ�ͷ
void action_camera_ctrl(uint8_t mode)
{
	GPIO_SetBits(GPIOA, GPIO_Pin_15);
	delay_ms(100);	
	GPIO_ResetBits(GPIOA, GPIO_Pin_15);
}
