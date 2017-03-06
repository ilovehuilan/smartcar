#include "gpio.h"
#include "common.h"
#include "i2c.h"
#include "dma.h"
#include "uart.h"
#include "i2c.h"
#include "VCAN_camera.h"
#include "VCAN_OV7725_Eagle.h"
#include "feature.h"
#include "ftm.h"
#include "speed_control.h"

uint8_t imgbuff[CAMERA_SIZE];  //ͼ��Դ��ַ
uint8_t img[CAMERA_W*CAMERA_H];  //��ѹ��Ŀ���ַ
int stop_flag = 0;

int centerline[CAMERA_H]=
{
CAMERA_W/2,CAMERA_W/2,CAMERA_W/2,CAMERA_W/2,CAMERA_W/2,CAMERA_W/2,CAMERA_W/2,CAMERA_W/2,
CAMERA_W/2,CAMERA_W/2,CAMERA_W/2,CAMERA_W/2,CAMERA_W/2,CAMERA_W/2,CAMERA_W/2,CAMERA_W/2,
CAMERA_W/2,CAMERA_W/2,CAMERA_W/2,CAMERA_W/2,CAMERA_W/2,CAMERA_W/2,CAMERA_W/2,CAMERA_W/2,
CAMERA_W/2,CAMERA_W/2,CAMERA_W/2,CAMERA_W/2,CAMERA_W/2,CAMERA_W/2,CAMERA_W/2,CAMERA_W/2,
CAMERA_W/2,CAMERA_W/2,CAMERA_W/2,CAMERA_W/2,CAMERA_W/2,CAMERA_W/2,CAMERA_W/2,CAMERA_W/2,
CAMERA_W/2,CAMERA_W/2,CAMERA_W/2,CAMERA_W/2,CAMERA_W/2,CAMERA_W/2,CAMERA_W/2,CAMERA_W/2,
CAMERA_W/2,CAMERA_W/2,CAMERA_W/2,CAMERA_W/2,CAMERA_W/2,CAMERA_W/2,CAMERA_W/2,CAMERA_W/2,
CAMERA_W/2,CAMERA_W/2,CAMERA_W/2,CAMERA_W/2
};  //��������


/****************�����ֽں���****************
* �������ƣ�uart_putbuff
* ����˵���������ֽ�
* ��ע��
********************************************/

void uart_putbuff (uint32_t instance, uint8_t *buff, uint32_t len)
{
    while(len--)
    {
        UART_WriteByte(instance, *buff);
        buff++;
    }
}

/****************************************************************
 *  @brief      ����ͼ��eSmartCameraCar��λ����ʾ
 *  @param      imgaddr         ͼ���ַ
 *  @param      imgsize         ͼ��ռ�ÿռ��С
 *  @since      v5.0
 *  @note       ��ͬ����λ������ͬ���������ʹ�� eSmartCameraCar�����
                ���ʹ��������λ��������Ҫ�޸Ĵ��롣
 *  Sample usage:   sendimg(imgbuff, CAMERA_W * CAMERA_H);                    //���͵���λ��
*******************************************************************/

void sendimg(uint8_t *imgaddr, uint32_t imgsize)
{
  #define CMD_IMG  1
  
    uint8_t cmd[4] = {0, 255, 1, 0 };    //yy_����ͷ���ڵ��� ʹ�õ�����
    //uart_putbuff(HW_UART0, cmd, sizeof(cmd));    //�ȷ�������
    UART_WriteByte(HW_UART4,0x01);
    UART_WriteByte(HW_UART4,0xfe);
    uart_putbuff(HW_UART4, (uint8_t *)imgaddr, imgsize); //�ٷ���ͼ��
    UART_WriteByte(HW_UART4,0xfe);
    UART_WriteByte(HW_UART4,0x01);
}

/********************��ѹ����*******************
* �������ƣ�imgextract
* ����˵������ֵ��ͼ���ѹ
* ��ע��

*  @brief      ��ֵ��ͼ���ѹ���ռ� �� ʱ�� ��ѹ��
*  @param      dst             ͼ���ѹĿ�ĵ�ַ
*  @param      src             ͼ���ѹԴ��ַ
*  @param      srclen          ��ֵ��ͼ���ռ�ÿռ��С
*  @since      v5.0            img_extract(img, imgbuff,CAMERA_SIZE);
*  Sample usage:
************************************************/

void imgextract(uint8_t *dst, uint8_t *src, uint32_t srclen)
{
  uint8_t colour[2] = {255, 0}; //0 �� 1 �ֱ��Ӧ����ɫ
  //ע��ɽ�������ͷ 0 ��ʾ ��ɫ��1��ʾ ��ɫ
  uint8_t tmpsrc;
  while(srclen --)
  {
    tmpsrc = *src++;
    *dst++ = colour[ (tmpsrc >> 7 ) & 0x01 ];
    *dst++ = colour[ (tmpsrc >> 6 ) & 0x01 ];
    *dst++ = colour[ (tmpsrc >> 5 ) & 0x01 ];
    *dst++ = colour[ (tmpsrc >> 4 ) & 0x01 ];
    *dst++ = colour[ (tmpsrc >> 3 ) & 0x01 ];
    *dst++ = colour[ (tmpsrc >> 2 ) & 0x01 ];
    *dst++ = colour[ (tmpsrc >> 1 ) & 0x01 ];
    *dst++ = colour[ (tmpsrc >> 0 ) & 0x01 ];
  }
}


/****************ѹ������****************
* �������ƣ�img_reduce
* ����˵����ͼ��ѹ��
* ��ע��
********************************************/
void  img_reduce(uint8_t *aim,uint8_t *get,uint32_t length)
{
  while(length>0)
  {
    *get = 0;
    *get += ((*aim++) & 0x80) >> 0;
    *get += ((*aim++) & 0x80) >> 1;
    *get += ((*aim++) & 0x80) >> 2;
    *get += ((*aim++) & 0x80) >> 3;
    *get += ((*aim++) & 0x80) >> 4;
    *get += ((*aim++) & 0x80) >> 5;
    *get += ((*aim++) & 0x80) >> 6;
    *get += ((*aim++) & 0x80) >> 7;
    *get =~ *get;
    get++;
    length -= 8;
  }
  
}

/**************************����****************************
* �������ƣ�ShowLine
* ����˵������ʾ�߽������
* ��ע��
**************************************************************************/
void ShowLine()
{  
  uint8_t *pic_show = NULL;
  int count = 0;
  if(effective_line_temp > 59)
    return;
  pic_show = &img[0];
  /*for(count=0;count<W;count++)                            //��ʾ��Ч�� ͼ������Ϊ��ɫ
    picture[effective_line_temp*W+count]=0;*/
  for(count = H-1;count >= effective_line_temp;count--)                        //��ʾ
  {
    //��ʾ���� ���� ����
    *(pic_show + count*W + centerline[count]) = 0;//(*(pic_show+count*W+centerline[count])-100)<0?0:(*(pic_show+count*W+centerline[count])-100);
    if(centerline[count] > 0)
      *(pic_show + count*W + centerline[count] - 1) = 255;
    if(centerline[count] < (W-1))
      *(pic_show + count*W + centerline[count] + 1) = 255;
    
    *(pic_show + count*W + right_line_temp[count]) = 0;//(*(pic_show+count*W+right_line_temp[count])-100)<0?0:(*(pic_show+count*W+right_line_temp[count])-100);
    if(right_line_temp[count] < (W-1))
      *(pic_show + count*W + centerline[count] + 1) = 255;
    
    *(pic_show + count*W + left_line_temp[count]) = 0;//(*(pic_show+count*W+left_line_temp[count])-100)<0?0:(*(pic_show+count*W+left_line_temp[count])-100);
    if(left_line_temp[count] > 0)
      *(pic_show + count*W + centerline[count] - 1) = 255;
  }
}

/****************��ʾ���߳���****************
* �������ƣ�show_line
* ����˵����ֻ��ʾ����
* ��ע��
********************************************/
void show_line(void)
{
  int x=0;
  for(x=0;x<H;x++)
  {
    //        img[W*x+left_line[x]]=0;
    //        img[W*x+right_line[x]]=0;
    img[W*x+centerline[x]]=0;
  }
}

/***************��ȫģʽ****************
* �������ƣ�away_road_stop
* ����˵�����������������
* ��ע��
********************************************/

void away_road_stop(void)
{
  
  if(img[W*59+centerline[59]]==black&&img[W*58+centerline[58]]==black&&img[W*57+centerline[57]]==black&&img[W*56+centerline[56]]==black&&img[W*55+centerline[55]]==black&&img[W*54+centerline[54]]==black&&img[W*53+centerline[53]]==black)
  {
    stop_flag=1;
  }
  if (stop_flag==1) 
  {
      right_set=0;
      left_set=0;
      FTM_PWM_ChangeDuty(HW_FTM0, HW_FTM_CH0,0); //������
      FTM_PWM_ChangeDuty(HW_FTM0, HW_FTM_CH1, 0);
      FTM_PWM_ChangeDuty(HW_FTM0, HW_FTM_CH2, 0); //�ҵ����
      FTM_PWM_ChangeDuty(HW_FTM0, HW_FTM_CH3, 0);
      DelayMs(300);   
    
  }
}

