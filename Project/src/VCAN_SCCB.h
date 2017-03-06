/*!
 *     COPYRIGHT NOTICE
 *     Copyright (c) 2013,山外科技
 *     All rights reserved.
 *     技术讨论：山外初学论坛 http://www.vcan123.com
 *
 *     除注明出处外，以下所有内容版权均属山外科技所有，未经允许，不得用于商业用途，
 *     修改内容时必须保留山外科技的版权声明。
 *
 * @file       VCAN_SCCB.h
 * @brief      OV摄像头配置总线SCCB函数库
 * @author     山外科技
 * @version    v5.0
 * @date       2013-09-01
 */


#ifndef _VCAN_SCCB_H_
#define _VCAN_SCCB_H_

//SCCB 管脚配置
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

#define DEV_ADR  ADDR_OV7725             /*设备地址定义*/

#define SCCB_DELAY()    SCCB_delay(400)


extern void SCCB_GPIO_init(void);
extern int SCCB_WriteByte( uint16_t WriteAddress , uint8_t SendByte);
extern int SCCB_ReadByte(uint8_t *pBuffer,   uint16_t length,   uint8_t ReadAddress);


#endif      //_VCAN_SCCB_H_
