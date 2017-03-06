#include "gpio.h"
#include "common.h"
#include "i2c.h"
#include "dma.h"
#include "uart.h"
#include "i2c.h"
#include "VCAN_camera.h"
#include "VCAN_OV7725_Eagle.h"

extern uint8_t imgbuff[CAMERA_SIZE];  
extern uint8_t img[CAMERA_W*CAMERA_H];  
extern int centerline[CAMERA_H];
extern int stop_flag;

extern void uart_putbuff (uint32_t instance, uint8_t *buff, uint32_t len);
extern void sendimg(uint8_t *imgaddr, uint32_t imgsize);
extern void imgextract(uint8_t *dst, uint8_t *src, uint32_t srclen);
extern void  img_reduce(uint8_t *aim,uint8_t *get,uint32_t length);
extern void ShowLine();
extern void show_line();
extern void away_road_stop();
