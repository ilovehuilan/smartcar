#include "gpio.h"
#include "Key_scan.h"
#include "oled.h"
#include "common.h"
/* 按键返回值 */
uint8_t gRetValue_D;
uint8_t gRetValue_L;
uint8_t gRetValue_R;
uint8_t gRetValue_O;
uint8_t mode_D=0;
uint8_t mode_L=0;
uint8_t mode_R=0;
uint8_t mode_O=0;
/*========================================================================
*  函数名称:  KEY_init
*  功能说明：按键初始化
*  创建时间：2015/04/13
*  修改时间：2015/04/14
*  参数说明：
========================================================================*/
void KEY_init()
{
  GPIO_QuickInit(HW_GPIOC, 10, kGPIO_Mode_IPU);
  GPIO_QuickInit(HW_GPIOC, 11, kGPIO_Mode_IPU);
  GPIO_QuickInit(HW_GPIOC, 12, kGPIO_Mode_IPU);
  GPIO_QuickInit(HW_GPIOC, 13, kGPIO_Mode_IPU);
}
/*========================================================================
*  函数名称:  KEY_Scan_D
*  功能说明：按键检测
*  创建时间：2015/04/13
*  修改时间：2015/04/14
*  参数说明：
========================================================================*/

 void KEY_Scan_D(void)
{
 static    KEY_Status_D status_D = kKEY_Idle_D;//////待检测
    switch(status_D)
    {
        case kKEY_Idle_D:
            gRetValue_D = NO_KEY_D;
            if(KEY_D == 0) /* 如果按键被按下 进入确认与消抖态 */
            {
                status_D = kKEY_Debounce_D;
            }
            break;
        case kKEY_Debounce_D: /* 确认与消抖态 */
            if(KEY_D== 0)
            {
                status_D = kKEY_Confirm_D;
            }
            else
            {
                status_D= kKEY_Idle_D;
                gRetValue_D = NO_KEY_D;
            }
            break;
        case kKEY_Confirm_D: /* 确认态 */
            if(KEY_D == 1) /* 按键松开 */
            {
                gRetValue_D = KEY_SINGLE_D;
                status_D = kKEY_Idle_D;
            }
            break;
        default:
            break;
    }
}
void KEY_Scan_L(void)
{
 static    KEY_Status_L status_L = kKEY_Idle_L;//////待检测
    switch(status_L)
    {
        case kKEY_Idle_L:
            gRetValue_L = NO_KEY_L;
            if(KEY_L == 0) /* 如果按键被按下 进入确认与消抖态 */
            {
                status_L = kKEY_Debounce_L;
            }
            break;
        case kKEY_Debounce_L: /* 确认与消抖态 */
            if(KEY_L== 0)
            {
                status_L = kKEY_Confirm_L;
            }
            else
            {
                status_L= kKEY_Idle_L;
                gRetValue_L = NO_KEY_L;
            }
            break;
        case kKEY_Confirm_L: /* 确认态 */
            if(KEY_L == 1) /* 按键松开 */
            {
              gRetValue_L = KEY_SINGLE_L;
                status_L = kKEY_Idle_L;
            }
            break;
        default:
            break;
    }
}
void KEY_Scan_R(void)
{
 static    KEY_Status_R status_R = kKEY_Idle_R;//////待检测
    switch(status_R)
    {
        case kKEY_Idle_R:
            gRetValue_R = NO_KEY_R;
            if(KEY_R == 0) /* 如果按键被按下 进入确认与消抖态 */
            {
                status_R = kKEY_Debounce_R;
            }
            break;
        case kKEY_Debounce_R: /* 确认与消抖态 */
            if(KEY_R== 0)
            {
                status_R = kKEY_Confirm_R;
            }
            else
            {
                status_R= kKEY_Idle_R;
                gRetValue_R = NO_KEY_R;
            }
            break;
        case kKEY_Confirm_R: /* 确认态 */
            if(KEY_R == 1) /* 按键松开 */
            {
                gRetValue_R = KEY_SINGLE_R;
                status_R = kKEY_Idle_R;
            }
            break;
        default:
            break;
    }
}
void KEY_Scan_O(void)
{
  static   KEY_Status_O status_O = kKEY_Idle_O;//////待检测
    switch(status_O)
    {
        case kKEY_Idle_O:
            gRetValue_O = NO_KEY_O;
            if(KEY_O == 0) /* 如果按键被按下 进入确认与消抖态 */
            {
                status_O = kKEY_Debounce_O;
            }
            break;
        case kKEY_Debounce_O: /* 确认与消抖态 */
            if(KEY_O== 0)
            {
                status_O = kKEY_Confirm_O;
            }
            else
            {
                status_O= kKEY_Idle_O;
                gRetValue_O = NO_KEY_O;
            }
            break;
        case kKEY_Confirm_O: /* 确认态 */
            if(KEY_O == 1) /* 按键松开 */
            {
                gRetValue_O = KEY_SINGLE_O;
                status_O = kKEY_Idle_O;
            }
            break;
        default:
            break;
    }
}
//void  KEY_Scan()
//{
//        //按键检测
//    KEY_Scan_D(); //调用按键扫描程序 
//    DelayMs(10);
//    if(gRetValue_D == KEY_SINGLE_D)
//    {
//      LCD_set_XY(0, 2);
//      LCD_write_english("wo");
//      mode_D=1;
//    }
//    KEY_Scan_L(); //调用按键扫描程序
//    DelayMs(10);
//    if(gRetValue_L == KEY_SINGLE_L)
//    {
//      LCD_set_XY(0, 3);
//      LCD_write_english("ai");
//      mode_L=1;
//    }
//    KEY_Scan_R(); //调用按键扫描程序
//    DelayMs(10);
//    if(gRetValue_R == KEY_SINGLE_R)
//    {
//      LCD_set_XY(0, 4);
//      LCD_write_english("ni");
//      mode_R=1;
//    }
//    KEY_Scan_O(); //调用按键扫描程序
//    DelayMs(10);
//    if(gRetValue_O == KEY_SINGLE_O)
//    {
//      LCD_set_XY(0, 5);
//      LCD_write_english("ha");
//      mode_O=1;
//    } 
//}

/*调参数时使用*/
void  KEY_Scan()
{
        //按键检测
    KEY_Scan_D(); 
    DelayMs(10);
    if(gRetValue_D == KEY_SINGLE_D)
    {
      Control=Control+2;
    }
    KEY_Scan_L(); 
    DelayMs(10);
    if(gRetValue_L == KEY_SINGLE_L)
    {
      Control=Control-2;
    }
    KEY_Scan_R(); 
    DelayMs(10);
    if(gRetValue_R == KEY_SINGLE_R)
    {
      Control=Control+10;
    }
    KEY_Scan_O(); 
    DelayMs(10);
    if(gRetValue_O == KEY_SINGLE_O)
    {
      Control=Control-10;
    } 
}