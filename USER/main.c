#include "sys.h"
#include "led.h"
#include "zkrt.h"
#include "adc.h"
#include "can.h"
#include "pwm.h"
#include "camera.h"

void bsp_init(void)
{
	SystemInit ();		/*ϵͳ��ʼ��*/
	RCC_Configuration();
	SysTick_Init();
	LED_Init();
	ADC1_Init();
	CAN_Mode_Init(CAN_Mode_Normal);//CAN��ʼ������ģʽ,������1Mbps    	
	TIM_Init();
}

uint8_t status_camera[8] = {0XAA, 0XBB, 0XCC, 0XDD, 0XEE, DEVICE_TYPE_3DMODELING, 0X00, 0X00};

int main()
{
  bsp_init();
	action_paizhao(0); //��ʼ��ֹͣ����
	
	while (1)
	{		
		sub_camera_zkrt_recv_decode_and_zkrt_encode_ack();
		if (_10ms_count - TimingDelay >= 10)								
		{
			_10ms_count = TimingDelay;
			ADC_StartOfConversion(ADC1);											
			
			if ((_10ms_flag%10) == 0)												
			{				
				if (MAVLINK_TX_INIT_VAL - TimingDelay > 2000)	
				{
					bat_read();
					bat_check();
				}
			}
			
			if ((_10ms_flag%100) == 0)												
			{
				if (MAVLINK_TX_INIT_VAL - TimingDelay > 5000)	 //��ʼ����5S�ڲ�ִ�з����������Ժ�ÿ�ζ���������
				{
					if (GET_DONGZUO == _PWM_TAKEPHOTO)
					{
						camera_ack_flag = 1;
					}
					else
					{
						camera_ack_flag = 0;
						action_paizhao(0);
					}
					
					status_camera[6] = camera_ack_flag;
					status_camera[7]++;
					if (status_camera[7] == 0XFF)
					{
						status_camera[7] = 0;
					}
					Can_Send_Msg(status_camera, 8);							
				}
			}
			_10ms_flag++;
		}
		
		if (led_rx_count - TimingDelay > 50)
		{
			GPIO_SetBits(GPIOB, GPIO_Pin_7);
		}
		
		if (led_tx_count - TimingDelay > 50)
		{
			GPIO_SetBits(GPIOB, GPIO_Pin_6);
		}
	}
}



