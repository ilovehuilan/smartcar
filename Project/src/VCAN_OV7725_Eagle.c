/*!
 *     COPYRIGHT NOTICE
 *     Copyright (c) 2013,ɽ��Ƽ�
 *     All rights reserved.
 *     �������ۣ�ɽ���ѧ��̳ http://www.vcan123.com
 *
 *     ��ע�������⣬�����������ݰ�Ȩ����ɽ��Ƽ����У�δ����������������ҵ��;��
 *     �޸�����ʱ���뱣��ɽ��Ƽ��İ�Ȩ������
 *
 * @file       VCAN_OV7725_Eagle.c
 * @brief      ӥ��ov7725��������
 * @author     ɽ��Ƽ�
 * @version    v5.0
 * @date       2013-09-07
 */

#include "common.h"
//#include "MK60_gpio.h"
//#include "MK60_port.h"
//#include "MK60_dma.h"
#include "VCAN_camera.h"
#include "VCAN_SCCB.h"
#include "dma.h"
#include "gpio.h"
#include "stdio.h"

#if (!defined(GPIO_BASES))

#ifdef PTF
#define GPIO_BASES {PTA, PTB, PTC, PTD, PTE, PTF}
#define PORT_BASES {PORTA, PORTB, PORTC, PORTD, PORTE, PORTF}
#else
#define GPIO_BASES {PTA, PTB, PTC, PTD, PTE}
#define PORT_BASES {PORTA, PORTB, PORTC, PORTD, PORTE}
#endif

#endif

/* gloabl vars */
static GPIO_Type * const GPIO_InstanceTable[] = GPIO_BASES;
static PORT_Type * const PORT_InstanceTable[] = PORT_BASES;
static GPIO_CallBackType GPIO_CallBackTable[ARRAY_SIZE(PORT_InstanceTable)] = {NULL};


#define OV7725_EAGLE_Delay_ms(time)  DelayMs(time)

uint8_t   *ov7725_eagle_img_buff;

volatile IMG_STATUS_e      ov7725_eagle_img_flag = IMG_FINISH;   //ͼ��״̬

//�ڲ���������
static uint8_t ov7725_eagle_reg_init(void);
static void ov7725_eagle_port_init();
void ov7725_eagle_vsync(uint32_t index);

/*!
 *  @brief      ӥ��ov7725��ʼ��
 *  @since      v5.0
 */
uint8_t ov7725_eagle_init(uint8_t *imgaddr)
{
    ov7725_eagle_img_buff = imgaddr;
    while(ov7725_eagle_reg_init() == 0);
    ov7725_eagle_port_init();
    return 0;
}

/*!
 *  @brief      ӥ��ov7725�ܽų�ʼ�����ڲ����ã�
 *  @since      v5.0
 */
void ov7725_eagle_port_init()
{
//    //DMAͨ��0��ʼ����PTA27����Դ(Ĭ��������)��Դ��ַΪPTB_B0_IN��Ŀ�ĵ�ַΪ��IMG_BUFF��ÿ�δ���1Byte
//    dma_portx2buff_init(CAMERA_DMA_CH, (void *)&PTB_B0_IN, (void *)ov7725_eagle_img_buff, PTA27, DMA_BYTE1, CAMERA_DMA_NUM, DADDR_KEEPON);
//
//    DMA_DIS(CAMERA_DMA_CH);
//    disable_irq(PORTA_IRQn);                        //�ر�PTA���ж�
//    DMA_IRQ_CLEAN(CAMERA_DMA_CH);                   //���ͨ�������жϱ�־λ
//    DMA_IRQ_EN(CAMERA_DMA_CH);
//
//    port_init(PTA27, ALT1 | DMA_FALLING | PULLUP );         //PCLK
//
//    port_init(PTA29, ALT1 | IRQ_RISING | PULLUP | PF);     //���жϣ��������Ͻ��ش����жϣ����˲�
    
    
    DMA_InitTypeDef DMA_InitStruct1 = {0};
    
    /* HREF VSYNC PCLK */
    GPIO_QuickInit(BOARD_OV7725_PCLK_PORT, BOARD_OV7725_PCLK_PIN, kGPIO_Mode_IPD);
    GPIO_QuickInit(BOARD_OV7725_VSYNC_PORT, BOARD_OV7725_VSYNC_PIN, kGPIO_Mode_IPU);
    //GPIO_QuickInit(BOARD_OV7725_HREF_PORT, BOARD_OV7725_HREF_PIN, kGPIO_Mode_IPD);
    
    /* install callback */
    GPIO_CallbackInstall(BOARD_OV7725_VSYNC_PORT, ov7725_eagle_vsync);
   
    //GPIO_ITDMAConfig(BOARD_OV7725_HREF_PORT, BOARD_OV7725_HREF_PIN, kGPIO_IT_FallingEdge, true);
    GPIO_ITDMAConfig(BOARD_OV7725_VSYNC_PORT, BOARD_OV7725_VSYNC_PIN, kGPIO_IT_FallingEdge, false);
          
    /* DATA PORT */
    for(int i=0;i<8;i++)
    {
        GPIO_QuickInit(BOARD_OV7725_DATA_PORT, i, kGPIO_Mode_IFT);
    }
    
    //DMA
    GPIO_ITDMAConfig(BOARD_OV7725_PCLK_PORT, BOARD_OV7725_PCLK_PIN, kGPIO_DMA_RisingEdge, true);
    DMA_InitStruct1.chl = CAMERA_DMA_CH;
    DMA_InitStruct1.chlTriggerSource = PORTC_DMAREQ;
    DMA_InitStruct1.triggerSourceMode = kDMA_TriggerSource_Normal;
    DMA_InitStruct1.minorLoopByteCnt = 1;
    DMA_InitStruct1.majorLoopCnt = CAMERA_DMA_NUM+1;//��֪��Ϊʲô��λ��ǿ�м�һ��������
    
    DMA_InitStruct1.sAddr = (uint32_t)&PTD->PDIR;
    DMA_InitStruct1.sLastAddrAdj = 0;
    DMA_InitStruct1.sAddrOffset = 0;
    DMA_InitStruct1.sDataWidth = kDMA_DataWidthBit_8;
    DMA_InitStruct1.sMod = kDMA_ModuloDisable;
    
    DMA_InitStruct1.dAddr = (uint32_t)(ov7725_eagle_img_buff);
    DMA_InitStruct1.dLastAddrAdj = 0;
    DMA_InitStruct1.dAddrOffset = 1;
    DMA_InitStruct1.dDataWidth = kDMA_DataWidthBit_8;
    DMA_InitStruct1.dMod = kDMA_ModuloDisable;
    
    /* initialize DMA moudle */
    DMA_Init(&DMA_InitStruct1);
    DMA_CallbackInstall(CAMERA_DMA_CH, ov7725_eagle_dma);
    DMA_ITConfig(CAMERA_DMA_CH, kDMA_IT_Major, true);  //����
    //DMA_EnableRequest(CAMERA_DMA_CH);/* ����DMA���� */

}

/*!
 *  @brief      ӥ��ov7725���жϷ�����
 *  @since      v5.0
 */
void ov7725_eagle_vsync(uint32_t index)
{

//  if(index & (1 << BOARD_OV7725_VSYNC_PIN))
//  {
//    GPIO_ITDMAConfig(BOARD_OV7725_VSYNC_PORT, BOARD_OV7725_VSYNC_PIN, kGPIO_IT_FallingEdge, false);
    //���ж���Ҫ�ж��ǳ��������ǳ���ʼ
    if(ov7725_eagle_img_flag == IMG_START)                   //��Ҫ��ʼ�ɼ�ͼ��
    {
      ov7725_eagle_img_flag = IMG_GATHER;                  //���ͼ��ɼ���
      GPIO_ITDMAConfig(BOARD_OV7725_VSYNC_PORT, BOARD_OV7725_VSYNC_PIN, kGPIO_IT_FallingEdge, false);
//      disable_irq(PORTA_IRQn);
//      
//      PORTA_ISFR = 1 <<  PT27;            //���PCLK��־λ
//      DMA_DADDR(CAMERA_DMA_CH) = (uin32_t)ov7725_eagle_img_buff;    //�ָ���ַ
//      DMA_EN(CAMERA_DMA_CH);                  //ʹ��ͨ��CHn Ӳ������
      
     // DMA0->CINT = DMA_CINT_CINT(HW_DMA_CH0);
      
              PORT_InstanceTable[HW_GPIOC]->ISFR = 0xFFFFFFFF;////���pclk

      DMA_SetDestAddress(CAMERA_DMA_CH,(uint32_t)(ov7725_eagle_img_buff-0x01));//ǿ�м�һ������
      
      DMA_EnableRequest(CAMERA_DMA_CH);
//      GPIO_ITDMAConfig(BOARD_OV7725_VSYNC_PORT, BOARD_OV7725_VSYNC_PIN, kGPIO_IT_FallingEdge, true);
    }
    else                                        //ͼ��ɼ�����
    {
//      disable_irq(PORTA_IRQn);                        //�ر�PTA���ж�
      //GPIO_ITDMAConfig(BOARD_OV7725_VSYNC_PORT, BOARD_OV7725_VSYNC_PIN, kGPIO_IT_RisingEdge, false);
      GPIO_ITDMAConfig(BOARD_OV7725_VSYNC_PORT, BOARD_OV7725_VSYNC_PIN, kGPIO_IT_FallingEdge, false);
      ov7725_eagle_img_flag = IMG_FAIL;                    //���ͼ��ɼ�ʧ��
    }
    
    //PORTB->ISFR = 0xFFFFFFFF;
//  }
}

/*!
 *  @brief      ӥ��ov7725 DMA�жϷ�����
 *  @since      v5.0
 */
void ov7725_eagle_dma()
{
    ov7725_eagle_img_flag = IMG_FINISH ;
    DMA0->CINT = DMA_CINT_CINT(HW_DMA_CH0);
    //DMA_IRQ_CLEAN(CAMERA_DMA_CH);           //���ͨ�������жϱ�־λ
    //DMA_ITConfig(CAMERA_DMA_CH, kDMA_IT_Major, 1);
    //DMA_EnableRequest(CAMERA_DMA_CH);
}

/*!
 *  @brief      ӥ��ov7725�ɼ�ͼ�񣨲ɼ��������ݴ洢�� ��ʼ��ʱ���õĵ�ַ�ϣ�
 *  @since      v5.0
 */
void ov7725_eagle_get_img()
{
    ov7725_eagle_img_flag = IMG_START;                   //��ʼ�ɼ�ͼ��
    PORTC_ISFR = ~0;                        //д1���жϱ�־λ(����ģ���Ȼ�ص���һ���жϾ����ϴ����ж�)
//    enable_irq(PORTA_IRQn);                         //����PTA���ж�
        PORT_InstanceTable[HW_GPIOC]->ISFR = 0xFFFFFFFF;////������ж�
        
    GPIO_ITDMAConfig(BOARD_OV7725_VSYNC_PORT, BOARD_OV7725_VSYNC_PIN, kGPIO_IT_FallingEdge, true);
    while(ov7725_eagle_img_flag != IMG_FINISH)           //�ȴ�ͼ��ɼ����
    {
        if(ov7725_eagle_img_flag == IMG_FAIL)            //����ͼ��ɼ����������¿�ʼ�ɼ�
        {
            ov7725_eagle_img_flag = IMG_START;           //��ʼ�ɼ�ͼ��
            PORTC_ISFR = ~0;                //д1���жϱ�־λ(����ģ���Ȼ�ص���һ���жϾ����ϴ����ж�)
//            enable_irq(PORTA_IRQn);                 //����PTA���ж�
            GPIO_ITDMAConfig(BOARD_OV7725_VSYNC_PORT, BOARD_OV7725_VSYNC_PIN, kGPIO_IT_FallingEdge, true);
        }
    }
}


/*OV7725��ʼ�����ñ�*/
reg_s ov7725_eagle_reg[] =
{
    //�Ĵ������Ĵ���ֵ��
    {OV7725_COM4         , 0x81},
    {OV7725_CLKRC        , 0x00},
    {OV7725_COM2         , 0x03},
    {OV7725_COM3         , 0xD0},
    {OV7725_COM7         , 0x40},
    {OV7725_HSTART       , 0x3F},
    {OV7725_HSIZE        , 0x50},
    {OV7725_VSTRT        , 0x03},
    {OV7725_VSIZE        , 0x78},
    {OV7725_HREF         , 0x00},
    {OV7725_SCAL0        , 0x0A},
    {OV7725_AWB_Ctrl0    , 0xE0},
    {OV7725_DSPAuto      , 0xff},
    {OV7725_DSP_Ctrl2    , 0x0C},
    {OV7725_DSP_Ctrl3    , 0x00},
    {OV7725_DSP_Ctrl4    , 0x00},

#if (CAMERA_W == 80)
    {OV7725_HOutSize     , 0x14},
#elif (CAMERA_W == 160)
    {OV7725_HOutSize     , 0x28},
#elif (CAMERA_W == 240)
    {OV7725_HOutSize     , 0x3c},
#elif (CAMERA_W == 320)
    {OV7725_HOutSize     , 0x50},
#else

#endif

#if (CAMERA_H == 60 )
    {OV7725_VOutSize     , 0x1E},
#elif (CAMERA_H == 120 )
    {OV7725_VOutSize     , 0x3c},
#elif (CAMERA_H == 180 )
    {OV7725_VOutSize     , 0x5a},
#elif (CAMERA_H == 240 )
    {OV7725_VOutSize     , 0x78},
#else

#endif

    {OV7725_EXHCH        , 0x00},
    {OV7725_GAM1         , 0x0c},
    {OV7725_GAM2         , 0x16},
    {OV7725_GAM3         , 0x2a},
    {OV7725_GAM4         , 0x4e},
    {OV7725_GAM5         , 0x61},
    {OV7725_GAM6         , 0x6f},
    {OV7725_GAM7         , 0x7b},
    {OV7725_GAM8         , 0x86},
    {OV7725_GAM9         , 0x8e},
    {OV7725_GAM10        , 0x97},
    {OV7725_GAM11        , 0xa4},
    {OV7725_GAM12        , 0xaf},
    {OV7725_GAM13        , 0xc5},
    {OV7725_GAM14        , 0xd7},
    {OV7725_GAM15        , 0xe8},
    {OV7725_SLOP         , 0x20},
    {OV7725_LC_RADI      , 0x00},
    {OV7725_LC_COEF      , 0x13},
    {OV7725_LC_XC        , 0x08},
    {OV7725_LC_COEFB     , 0x14},
    {OV7725_LC_COEFR     , 0x17},
    {OV7725_LC_CTR       , 0x05},
    {OV7725_BDBase       , 0x99},
    {OV7725_BDMStep      , 0x03},
    {OV7725_SDE          , 0x04},
    {OV7725_BRIGHT       , 0x00},
    {OV7725_CNST         , 0x45},
    {OV7725_SIGN         , 0x06},
    {OV7725_UVADJ0       , 0x11},
    {OV7725_UVADJ1       , 0x02},

};

uint8_t ov7725_eagle_cfgnum = ARR_SIZE( ov7725_eagle_reg ) ; /*�ṹ�������Ա��Ŀ*/


/*!
 *  @brief      ӥ��ov7725�Ĵ��� ��ʼ��
 *  @return     ��ʼ�������0��ʾʧ�ܣ�1��ʾ�ɹ���
 *  @since      v5.0
 */
uint8_t ov7725_eagle_reg_init(void)
{
    uint16_t i = 0;
    uint8_t Sensor_IDCode = 0;
    SCCB_GPIO_init();

    //OV7725_Delay_ms(50);
    if( 0 == SCCB_WriteByte ( OV7725_COM7, 0x80 ) ) /*��λsensor */
    {
        DEBUG_PRINTF("\n����:SCCBд���ݴ���");
        return 0 ;
    }

    OV7725_EAGLE_Delay_ms(50);

    if( 0 == SCCB_ReadByte( &Sensor_IDCode, 1, OV7725_VER ) )    /* ��ȡsensor ID��*/
    {
        DEBUG_PRINTF("\n����:��ȡIDʧ��");
        return 0;
    }
    DEBUG_PRINTF("\nGet ID success��SENSOR ID is 0x%x", Sensor_IDCode);
    DEBUG_PRINTF("\nConfig Register Number is %d ", ov7725_eagle_cfgnum);
    if(Sensor_IDCode == OV7725_ID)
    {
        for( i = 0 ; i < ov7725_eagle_cfgnum ; i++ )
        {
            if( 0 == SCCB_WriteByte(ov7725_eagle_reg[i].addr, ov7725_eagle_reg[i].val) )
            {
                DEBUG_PRINTF("\n����:д�Ĵ���0x%xʧ��", ov7725_eagle_reg[i].addr);
                return 0;
            }
        }
    }
    else
    {
        return 0;
    }
    DEBUG_PRINTF("\nOV7725 Register Config Success!");
    return 1;
}


