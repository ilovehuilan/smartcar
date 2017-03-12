#include "speed_control.h"
#include "ftm.h"
#include "gpio.h"
#include "math.h"
#include "uart.h"
#include "picture.h"
#include "pit.h"
#include "math.h"
int32_t right_pwm=0,left_pwm=0;
int16_t right_set=250,right_error=0,right_last_error=0,right_last_error2=0;
float left_KP=10,left_KI=10,left_KD=0;
int16_t left_set=250,left_error=0,left_last_error=0,left_last_error2=0;
float right_KP=10,right_KI=10,right_KD=0;
int value1=0; /* 记录正交脉冲个数 */
int value2=0;

////速度控制
uint16_t speed_zhidao=0,speed_wandao=0;
/*========================================================================
*  函数名称:  speed_init
*  功能说明：电机初始化
*  创建时间：2015/04/13
*  修改时间：2015/04/14
*  参数说明：
========================================================================*/
void speed_init()
{
  /////pwm使能
  
  ///电机初始化
  FTM_PWM_QuickInit(FTM0_CH0_PC01 , kPWM_EdgeAligned, 10000);
  FTM_PWM_QuickInit(FTM0_CH1_PC02 , kPWM_EdgeAligned, 10000);
  FTM_PWM_QuickInit(FTM0_CH2_PC03 , kPWM_EdgeAligned, 10000);
  FTM_PWM_QuickInit(FTM0_CH3_PC04 , kPWM_EdgeAligned, 10000);
  
  FTM_PWM_ChangeDuty(HW_FTM0, HW_FTM_CH0, 0); //左电机正
  FTM_PWM_ChangeDuty(HW_FTM0, HW_FTM_CH1, 0);
  FTM_PWM_ChangeDuty(HW_FTM0, HW_FTM_CH2, 0); //右电机正
  FTM_PWM_ChangeDuty(HW_FTM0, HW_FTM_CH3, 0);
  //编码器正交解码
  FTM_QD_QuickInit(FTM1_QD_PHA_PA12_PHB_PA13, kFTM_QD_NormalPolarity, kQD_PHABEncoding);//左电机
  FTM_QD_QuickInit(FTM2_QD_PHA_PB18_PHB_PB19, kFTM_QD_NormalPolarity, kQD_PHABEncoding);//右电机
  PIT_QuickInit(HW_PIT_CH0, 1000*10);
  PIT_CallbackInstall(HW_PIT_CH0, PIT_ISR);
  PIT_ITDMAConfig(HW_PIT_CH0, kPIT_IT_TOF, false);
  
}

void PIT_ISR(void)
{
  
  
  uint8_t dir1; /* 记录编码器旋转方向1 */
  uint8_t dir2 ;
  
  /* 获取正交解码数据 */
  FTM_QD_GetData(HW_FTM1, &value1, &dir1);
  FTM_QD_GetData(HW_FTM2, &value2, &dir2);
  value2=65535-value2;
  speed_feedback_control(value1,value2);
  FTM_QD_ClearCount(HW_FTM1); /* 如测量频率则需要定时清除Count值  */
  FTM_QD_ClearCount(HW_FTM2); /* 如测量频率则需要定时清除Count值  */
}
/*========================================================================
*  函数名称:  speed_pid_control
*  功能说明：电机控制函数(直道，进弯，出弯，进出弯快速加减速)
*  创建时间：2015/04/13
*  修改时间：2015/04/14
*  参数说明：
========================================================================*/
//void speed_pid_control()
//{ 
//       left_set=speed_zhidao;
//       right_set=speed_zhidao;
//    ///////进出弯快速加减速
//     if(在直道和小S)//根据有效行
//     {
//         left_set=speed_zhidao;
//         right_set=speed_zhidao;
//         //bangbang
//         if((speed_zhidao-left_feedback)>0&&(speed_zhidao-left_feedback)>=)
//         {
//            left_set+=100;
//         }
//         if((speed_zhidao-left_feedback)<0&&(speed_zhidao-left_feedback)<=)
//         {
//            left_set-=100;
//         }
//        if((speed_zhidao-right_feedback)>0&&(speed_zhidao-right_feedback)>=)
//         {
//            left_set+=100;
//         }
//         if((speed_zhidao-right_feedback)<0&&(speed_zhidao-right_feedback)<=)
//         {
//            left_set-=100;
//         }
//     }
//     ///根据有效行，进入弯道和处在弯道中
//     if(进入弯道)//根据有效行 不用在处于弯道中
//     {
//       left_set=speed_wandao;
//       right_set=speed_wandao;
//       ////用棒棒
//       if((speed_zhidao-left_feedback)>0&&(speed_zhidao-left_feedback)>=)
//       {
//         left_set+=100;
//       }
//       if((speed_zhidao-left_feedback)<0&&(speed_zhidao-left_feedback)<=)
//       {
//         left_set-=100;
//       }
//       if((speed_zhidao-right_feedback)>0&&(speed_zhidao-right_feedback)>=)
//       {
//         left_set+=100;
//       }
//       if((speed_zhidao-right_feedback)<0&&(speed_zhidao-right_feedback)<=)
//       {
//         left_set-=100;
//       }
//     }
//     //////////阿克曼差速（只在弯道中使用）
//     if(偏差>0)
//     {
//          ////根据舵机打角
//       left_set*=(1-(B*())/(2*L+B*()));
//       right_set*=(1+(B*())/(2*L+B*()));
//     }
//     if()
//     {
//       left_set*=(1+(B*())/(2*L+B*());
//       right_set*=(1-(B*())/(2*L+B*()));
//     }
//     speed_feedback_control();
//}
/*========================================================================
*  函数名称:  speed_feedback_control
*  功能说明：电机闭环函数
*  创建时间：2015/04/13
*  修改时间：2015/04/14
*  参数说明：
========================================================================*/
void speed_feedback_control(int16_t left_feedback,int16_t right_feedback)
{
  
  uint16_t send_information[4]={0};
  left_error=left_set-left_feedback; 
  left_pwm+=(left_KP*(float)(left_error-left_last_error)+left_KI*(float)left_error+left_KD*(float)(left_error-2*left_last_error+left_last_error2));
  left_last_error2=left_last_error;
  left_last_error=left_error;     
  send_information[0]=left_set;
  send_information[1]=left_feedback; 
  
  right_error=right_set-right_feedback; 
  right_pwm+=(right_KP*(float)(right_error-right_last_error)+right_KI*(float)right_error+right_KD*(float)(right_error-2*right_last_error+right_last_error2));
  right_last_error2=right_last_error;
  right_last_error=right_error; 
  send_information[2]=right_set;
  send_information[3]=right_feedback; 
  
  speed_pwm_output(left_pwm,right_pwm);
  //speed_send_information(send_information[0],send_information[1]);
}
/*========================================================================
*  函数名称:  speed_pwm_output
*  功能说明：电机输出函数
*  创建时间：2015/04/13
*  修改时间：2015/04/14
*  参数说明：
========================================================================*/
void speed_pwm_output(int32_t left_pwm,int32_t right_pwm)
{
  if(left_pwm<=-9900)
  {
    left_pwm=-9900;
  }
  if(left_pwm>=9900)
  {
    left_pwm=9900;
  }
  if(right_pwm<=-9900)
  {
    right_pwm=-9900;
  }
  if(right_pwm>=9900)
  {
    right_pwm=9900;
  }
  if(left_pwm<=0)
  {
    FTM_PWM_ChangeDuty(HW_FTM0, HW_FTM_CH0, 0); //左电机
    FTM_PWM_ChangeDuty(HW_FTM0, HW_FTM_CH1, fabs(left_pwm));
  }
  else
  {
    FTM_PWM_ChangeDuty(HW_FTM0, HW_FTM_CH0, left_pwm); //左电机
    FTM_PWM_ChangeDuty(HW_FTM0, HW_FTM_CH1, 0);
  }
  if(right_pwm<=0)
  {
    FTM_PWM_ChangeDuty(HW_FTM0, HW_FTM_CH2, fabs(right_pwm)); //右电机
    FTM_PWM_ChangeDuty(HW_FTM0, HW_FTM_CH3, 0);
  }
  else
  {
    FTM_PWM_ChangeDuty(HW_FTM0, HW_FTM_CH2, 0); //右电机
    FTM_PWM_ChangeDuty(HW_FTM0, HW_FTM_CH3, right_pwm);
  }
}
/*========================================================================
*  函数名称:  speed_stop
*  功能说明：电机停止运转函数
*  创建时间：2015/04/13
*  修改时间：2015/04/14
*  参数说明：
========================================================================*/
void speed_stop()
{
  int valueleft;
  uint8_t dirleft;
  // int valueright;
  while(1)
  {
    /////缺少对舵机控制
    //    FTM_QD_GetData(HW_FTM1, &valueright, &dirright);//右电机
    FTM_QD_GetData(HW_FTM2, &valueleft,&dirleft);//左电机
    if(valueleft>2)/////反转
    {
      FTM_PWM_ChangeDuty(HW_FTM0, HW_FTM_CH0, 0); //左电机
      FTM_PWM_ChangeDuty(HW_FTM0, HW_FTM_CH1, 2000);
      FTM_PWM_ChangeDuty(HW_FTM0, HW_FTM_CH2, 0); //右电机
      FTM_PWM_ChangeDuty(HW_FTM0, HW_FTM_CH3, 2000);
    }
    if(valueleft<-2)///反转
    {
      FTM_PWM_ChangeDuty(HW_FTM0, HW_FTM_CH0, 2000); //左电机
      FTM_PWM_ChangeDuty(HW_FTM0, HW_FTM_CH1, 0);
      FTM_PWM_ChangeDuty(HW_FTM0, HW_FTM_CH2, 2000); //右电机
      FTM_PWM_ChangeDuty(HW_FTM0, HW_FTM_CH3, 0);
    }
    else
    {
      FTM_PWM_ChangeDuty(HW_FTM0, HW_FTM_CH0, 0); //左电机
      FTM_PWM_ChangeDuty(HW_FTM0, HW_FTM_CH1, 0);
      FTM_PWM_ChangeDuty(HW_FTM0, HW_FTM_CH2, 0); //右电机
      FTM_PWM_ChangeDuty(HW_FTM0, HW_FTM_CH3, 0);
    }
  }
}
/*========================================================================
*  函数名称:  speed_send_information
*  功能说明：电机参数发送函数////虚拟示波器

*  创建时间：2015/04/13
*  修改时间：2015/04/14
*  参数说明：
========================================================================*/
void speed_send_information(uint16_t n,uint16_t m )
{
#define xuni     3
  uint8_t a,b,c,d;
  uint8_t cmdf[2] = {xuni, ~xuni};    //开头命令
  uint8_t cmdr[2] = {~xuni, xuni};    //结尾命令
  
  UART_WriteByte(HW_UART4, cmdf[0]);    //先发送命令
  UART_WriteByte(HW_UART4, cmdf[1]);
  
  a=n>>8;
  b=n&0x0ff;
  UART_WriteByte(HW_UART4,b); 
  UART_WriteByte(HW_UART4,a); 
  c=m>>8;
  d=m&0x0ff;
  UART_WriteByte(HW_UART4,d); 
  UART_WriteByte(HW_UART4,c); 
  //   UART_WriteByte(HW_UART4,shi[0]);   
  //UART_WriteByte(HW_UART4, 255);//另一种上位机
  
  UART_WriteByte(HW_UART4, cmdr[0]);   //再发送命令 
  UART_WriteByte(HW_UART4, cmdr[1]);   
}
/*========================================================================
*  函数名称:  speed_test
*  功能说明：电机测试
*  创建时间：2015/04/13
*  修改时间：2015/04/14
*  参数说明：
========================================================================*/
void speed_test()
{
  FTM_PWM_ChangeDuty(HW_FTM0, HW_FTM_CH0, 2700); //左电机正
  FTM_PWM_ChangeDuty(HW_FTM0, HW_FTM_CH1, 0);
  FTM_PWM_ChangeDuty(HW_FTM0, HW_FTM_CH2, 0); //右电机正
  FTM_PWM_ChangeDuty(HW_FTM0, HW_FTM_CH3, 2700);
}