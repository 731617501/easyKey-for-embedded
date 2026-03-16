#ifndef  __KEY_USER_H__
#define __KEY_USER_H__

#include "ezkey_user.h"
#include "stdint.h"
#include "main.h"
#include "FreeRTOS.h"
#include "queue.h"
#include "cmsis_os2.h"

typedef enum {
    LED_TOGGLE,
    LED_BLINK
} led_ops;

extern osMessageQueueId_t KeyQueueHandle;

uint8_t key_read( );
void key_click_callback();
void key_long_press_callback();



#endif