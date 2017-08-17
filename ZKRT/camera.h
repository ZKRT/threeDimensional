#ifndef _CAMERA_H
#define _CAMERA_H

#include "sys.h"

#define _PWM_2000	2000
#define _PWM_1500	1500

#define _PWM_TAKEPHOTO	1100
#define _PWM_CLOSETAKE	1500


#define _TIM_SBUS		 20
#define _TIM_PAIZHAO 1000
#define _TIM_RETURN  500

//PWM1_T3C2，对应PB5  //三维吊舱拍照
//PWM2_T3C1，对应PB4  //IO 吊舱电源控制
//PWM3_T2C1，对应PA15 //IO 吊舱镜头控制
//PWM4_T3C4，对应PB1
//PWM5_T3C3，对应PB0
//PWM6_T2C4，对应PA3
//PWM7_T2C3，对应PA2
//PWM8_T2C2，对应PA1

#define PWM_DONGZUO(value)   TIM_SetCompare2(TIM3, value);
#define GET_DONGZUO           TIM_GetCapture2(TIM3)

void sub_camera_zkrt_recv_decode_and_zkrt_encode_ack(void);
void action_paizhao(uint8_t mode);
void action_vedio_pwr_ctrl(uint8_t mode);
void action_camera_ctrl(uint8_t mode);

extern uint8_t camera_ack_flag;
#endif
