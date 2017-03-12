#include "gpio.h"
#include "common.h"
#include "i2c.h"
#include "dma.h"
#include "uart.h"
#include "i2c.h"
#include "VCAN_camera.h"
#include "picture.h"
#include "feature.h"
#include "ftm.h"
#include "adc.h"
#include "steer_control.h"
#include "speed_control.h"
#include "pit.h"
#include "oled.h"


/*���ƴ�������*/
#define uart_open 0 


int main(void)
{
  DelayInit();
  
  //#if uart_open
  UART_QuickInit(UART4_RX_PC14_TX_PC15, 115200);     
  //#endif            
  
  //��ʼ������ͷ
  camera_init(imgbuff);
  S3010_init();
  speed_init();
  OLED_init();
  KEY_init();
  NVIC_SetPriorityGrouping(NVIC_PriorityGroup_3);  //�ж����ȼ��ֳ�3��
  NVIC_SetPriority(PORTC_IRQn, NVIC_EncodePriority(NVIC_PriorityGroup_3, 0, 2)); //����PTC�˿ڵ���ռ���ȼ��������ȼ�
  NVIC_SetPriority(DMA0_IRQn, NVIC_EncodePriority(NVIC_PriorityGroup_3, 1, 2));
  NVIC_SetPriority(PIT0_IRQn, NVIC_EncodePriority(NVIC_PriorityGroup_3, 2, 2));

  DelayMs(3000);
  PIT_ITDMAConfig(HW_PIT_CH0, kPIT_IT_TOF, true);
  while(1)
  {    
    camera_get_img();  //��ȡͼ��         
    imgextract(img,imgbuff,CAMERA_SIZE);//��ѹͼ��
    Center_line();
   
    steer_control();
//    speed_test();
    
#if uart_open        
    ShowLine();  //��λ����ʾ�߽�and����                
    img_reduce(img,imgbuff,CAMERA_W*CAMERA_H);
    sendimg(imgbuff, CAMERA_SIZE);    //���͵���λ��
#endif     
    away_road_stop();
    
  }
}

