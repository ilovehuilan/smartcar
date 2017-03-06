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

uint8_t imgbuff[CAMERA_SIZE];  //图像源地址
uint8_t img[CAMERA_W*CAMERA_H];  //解压后目标地址
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
};  //中线数组


/****************发送字节函数****************
* 函数名称：uart_putbuff
* 功能说明：发送字节
* 备注：
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
 *  @brief      发送图像到eSmartCameraCar上位机显示
 *  @param      imgaddr         图像地址
 *  @param      imgsize         图像占用空间大小
 *  @since      v5.0
 *  @note       不同的上位机，不同的命令，这里使用 eSmartCameraCar软件，
                如果使用其他上位机，则需要修改代码。
 *  Sample usage:   sendimg(imgbuff, CAMERA_W * CAMERA_H);                    //发送到上位机
*******************************************************************/

void sendimg(uint8_t *imgaddr, uint32_t imgsize)
{
  #define CMD_IMG  1
  
    uint8_t cmd[4] = {0, 255, 1, 0 };    //yy_摄像头串口调试 使用的命令
    //uart_putbuff(HW_UART0, cmd, sizeof(cmd));    //先发送命令
    UART_WriteByte(HW_UART4,0x01);
    UART_WriteByte(HW_UART4,0xfe);
    uart_putbuff(HW_UART4, (uint8_t *)imgaddr, imgsize); //再发送图像
    UART_WriteByte(HW_UART4,0xfe);
    UART_WriteByte(HW_UART4,0x01);
}

/********************解压程序*******************
* 函数名称：imgextract
* 功能说明：二值化图像解压
* 备注：

*  @brief      二值化图像解压（空间 换 时间 解压）
*  @param      dst             图像解压目的地址
*  @param      src             图像解压源地址
*  @param      srclen          二值化图像的占用空间大小
*  @since      v5.0            img_extract(img, imgbuff,CAMERA_SIZE);
*  Sample usage:
************************************************/

void imgextract(uint8_t *dst, uint8_t *src, uint32_t srclen)
{
  uint8_t colour[2] = {255, 0}; //0 和 1 分别对应的颜色
  //注：山外的摄像头 0 表示 白色，1表示 黑色
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


/****************压缩程序****************
* 函数名称：img_reduce
* 功能说明：图像压缩
* 备注：
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

/**************************函数****************************
* 函数名称：ShowLine
* 功能说明：显示边界和中线
* 备注：
**************************************************************************/
void ShowLine()
{  
  uint8_t *pic_show = NULL;
  int count = 0;
  if(effective_line_temp > 59)
    return;
  pic_show = &img[0];
  /*for(count=0;count<W;count++)                            //显示有效行 图像整行为黑色
    picture[effective_line_temp*W+count]=0;*/
  for(count = H-1;count >= effective_line_temp;count--)                        //显示
  {
    //显示中线 左线 右线
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

/****************显示中线程序****************
* 函数名称：show_line
* 功能说明：只显示中线
* 备注：
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

/***************安全模式****************
* 函数名称：away_road_stop
* 功能说明：车冲出赛道处理
* 备注：
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
      FTM_PWM_ChangeDuty(HW_FTM0, HW_FTM_CH0,0); //左电机正
      FTM_PWM_ChangeDuty(HW_FTM0, HW_FTM_CH1, 0);
      FTM_PWM_ChangeDuty(HW_FTM0, HW_FTM_CH2, 0); //右电机正
      FTM_PWM_ChangeDuty(HW_FTM0, HW_FTM_CH3, 0);
      DelayMs(300);   
    
  }
}

