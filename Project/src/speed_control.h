////H§ß§Õ¨n§¹
#include "common.h"
void speed_init();
void speed_pid_control();
void speed_feedback_control(int16_t left_feedback,int16_t right_feedback);
void speed_pwm_output(int32_t left_pwm,int32_t right_pwm);
void speed_stop();
void speed_send_information(uint16_t n,uint16_t m );

void PIT_ISR();


extern void speed_test( );
extern int16_t right_set;
extern int16_t left_set;