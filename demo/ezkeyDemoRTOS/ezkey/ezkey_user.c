#include "ezkey_user.h"


//写下你按键的读取函数，返回值为0或1
uint8_t key_read( )
{
   return HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0 );
}

void key_click_callback()
{
    led_ops led1_ops = LED_TOGGLE;
    osMessageQueuePut( KeyQueueHandle, &led1_ops , 0, 0);
}

void key_long_press_callback()
{
    led_ops led1_ops = LED_BLINK;
    osMessageQueuePut( KeyQueueHandle , &led1_ops , 0, 0);
}
