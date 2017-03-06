#include "common.h"

#define KEY_D  PCin(13)  //����PTE�˿ڵ�13����Ϊ����
#define KEY_L  PCin(16)  //����PTE�˿ڵ�16����Ϊ����
#define KEY_R  PCin(17)  //����PTE�˿ڵ�17����Ϊ����
#define KEY_O  PCin(18)  //����PTC�˿ڵ�18����Ϊ����
/////* ��������״̬ 0δ�� 1���� */
#define NO_KEY_D          (0x00)
#define NO_KEY_L          (0x00)
#define NO_KEY_R          (0x00)
#define NO_KEY_O          (0x00)
#define KEY_SINGLE_D      (0x01)
#define KEY_SINGLE_L      (0x01)
#define KEY_SINGLE_R      (0x01)
#define KEY_SINGLE_O      (0x01)

/* ״̬���� */
typedef enum
{
    kKEY_Idle_D,          /*����̬ */
    kKEY_Debounce_D,      /*ȷ��������̬ */
    kKEY_Confirm_D,       /*ȷ�ϰ���״̬*/
}KEY_Status_D;
typedef enum
{
    kKEY_Idle_L,          /*����̬ */
    kKEY_Debounce_L,      /*ȷ��������̬ */
    kKEY_Confirm_L,       /*ȷ�ϰ���״̬*/
}KEY_Status_L;
typedef enum
{
    kKEY_Idle_R,          /*����̬ */
    kKEY_Debounce_R,      /*ȷ��������̬ */
    kKEY_Confirm_R,       /*ȷ�ϰ���״̬*/
}KEY_Status_R;
typedef enum
{
    kKEY_Idle_O,          /*����̬ */
    kKEY_Debounce_O,      /*ȷ��������̬ */
    kKEY_Confirm_O,       /*ȷ�ϰ���״̬*/
}KEY_Status_O;
void KEY_init();
void KEY_Scan_D(void);
void KEY_Scan_L(void);
void KEY_Scan_R(void);
void KEY_Scan_O(void);
void KEY_Scan();
