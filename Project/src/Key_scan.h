#include "common.h"

#define KEY_D  PCin(13)  //定义PTE端口的13引脚为输入
#define KEY_L  PCin(16)  //定义PTE端口的16引脚为输入
#define KEY_R  PCin(17)  //定义PTE端口的17引脚为输入
#define KEY_O  PCin(18)  //定义PTC端口的18引脚为输入
/////* 按键返回状态 0未按 1按下 */
#define NO_KEY_D          (0x00)
#define NO_KEY_L          (0x00)
#define NO_KEY_R          (0x00)
#define NO_KEY_O          (0x00)
#define KEY_SINGLE_D      (0x01)
#define KEY_SINGLE_L      (0x01)
#define KEY_SINGLE_R      (0x01)
#define KEY_SINGLE_O      (0x01)

/* 状态机表 */
typedef enum
{
    kKEY_Idle_D,          /*空闲态 */
    kKEY_Debounce_D,      /*确认与消抖态 */
    kKEY_Confirm_D,       /*确认按键状态*/
}KEY_Status_D;
typedef enum
{
    kKEY_Idle_L,          /*空闲态 */
    kKEY_Debounce_L,      /*确认与消抖态 */
    kKEY_Confirm_L,       /*确认按键状态*/
}KEY_Status_L;
typedef enum
{
    kKEY_Idle_R,          /*空闲态 */
    kKEY_Debounce_R,      /*确认与消抖态 */
    kKEY_Confirm_R,       /*确认按键状态*/
}KEY_Status_R;
typedef enum
{
    kKEY_Idle_O,          /*空闲态 */
    kKEY_Debounce_O,      /*确认与消抖态 */
    kKEY_Confirm_O,       /*确认按键状态*/
}KEY_Status_O;
void KEY_init();
void KEY_Scan_D(void);
void KEY_Scan_L(void);
void KEY_Scan_R(void);
void KEY_Scan_O(void);
void KEY_Scan();
