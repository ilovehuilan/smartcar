/*!
 *     COPYRIGHT NOTICE
 *     Copyright (c) 2013,ɽ��Ƽ�
 *     All rights reserved.
 *     �������ۣ�ɽ���ѧ��̳ http://www.vcan123.com
 *
 *     ��ע�������⣬�����������ݰ�Ȩ����ɽ��Ƽ����У�δ����������������ҵ��;��
 *     �޸�����ʱ���뱣��ɽ��Ƽ��İ�Ȩ������
 *
 * @file       VCAN_SCCB.h
 * @brief      OV����ͷ��������SCCB������
 * @author     ɽ��Ƽ�
 * @version    v5.0
 * @date       2013-09-01
 */


#ifndef _VCAN_SCCB_H_
#define _VCAN_SCCB_H_

//SCCB �ܽ�����
#define SCCB_SCL_PIN    7
#define SCCB_SDA_PIN    8
#define SCCB_PORT       HW_GPIOC

#define SCL_H()         GPIO_WriteBit(SCCB_PORT, SCCB_SCL_PIN, 1)//PTXn_T(SCCB_SCL,OUT) = 1
#define SCL_L()         GPIO_WriteBit(SCCB_PORT, SCCB_SCL_PIN, 0)//PTXn_T(SCCB_SCL,OUT) = 0
#define SCL_DDR_OUT()   GPIO_PinConfig(SCCB_PORT, SCCB_SCL_PIN, kOutput)//PTXn_T(SCCB_SCL,DDR) = 1
#define SCL_DDR_IN()    GPIO_PinConfig(SCCB_PORT, SCCB_SCL_PIN, kInput)//PTXn_T(SCCB_SCL,DDR) = 0

#define SDA_H()         GPIO_WriteBit(SCCB_PORT, SCCB_SDA_PIN, 1)//PTXn_T(SCCB_SDA,OUT) = 1
#define SDA_L()         GPIO_WriteBit(SCCB_PORT, SCCB_SDA_PIN, 0)//PTXn_T(SCCB_SDA,OUT) = 0
#define SDA_IN()        GPIO_ReadBit(SCCB_PORT, SCCB_SDA_PIN)
#define SDA_DDR_OUT()   GPIO_PinConfig(SCCB_PORT, SCCB_SDA_PIN, kOutput)//PTXn_T(SCCB_SDA,DDR) = 1
#define SDA_DDR_IN()    GPIO_PinConfig(SCCB_PORT, SCCB_SDA_PIN, kInput)//PTXn_T(SCCB_SDA,DDR) = 0



#define ADDR_OV7725   0x42
#define ADDR_OV7620   0x42

#define DEV_ADR  ADDR_OV7725             /*�豸��ַ����*/

#define SCCB_DELAY()    SCCB_delay(400)


extern void SCCB_GPIO_init(void);
extern int SCCB_WriteByte( uint16_t WriteAddress , uint8_t SendByte);
extern int SCCB_ReadByte(uint8_t *pBuffer,   uint16_t length,   uint8_t ReadAddress);


#endif      //_VCAN_SCCB_H_
