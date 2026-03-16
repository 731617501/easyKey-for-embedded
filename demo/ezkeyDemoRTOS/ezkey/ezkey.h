#ifndef __EZ_KEY_H__
#define __EZ_KEY_H__

#include "stdint.h"

//------------------------------- 自定义设置 --------------------------------//
//------------------------------ USER SETTING -------------------------------//


//长按时间设置，当按下时间达到这个时间的时候判定为长按
#define LONG_PRESS_TIME      1000
//按键按下最大时间限制，当大于这个时间的时候这次按键被判定为无效
#define MAX_PRESS_TIME       6000
//意思按键按下后要成功读取到多少次按下状态后才算一次按下而不是抖动
#define CONFRIM_COUNT           1

//若定时器回调函数时间周期为1ms 则读取间隔为10ms  
//也就是进入十次定时器的回调函数才会读取一次按键状态
#define READ_DELAY       10    

//定义你所需要的最大按键的数量
//define the maximum number of keys you need to use.
#define numKeys 4


//-------------------------------- 接口设计 ---------------------------------//
//-------------------------------- ezKey API --------------------------------//

typedef uint8_t           ( *pf_read ) ( );
typedef void      ( *pf_key_callback ) ( );


//初始化按键,绑定读取函数和触发后的回调函数
void KEY_Init (  uint8_t                KeyActiveLevel , 
                 pf_read                       io_read , 
                 pf_key_callback        click_callback , 
                 pf_key_callback   long_press_callback    );

//按键读取的主要实现部分，需要把这个函数放在一个定时器的中断回调中
void KEY_Tick( );

//KeyCallback的调用部分，需要把这个函数放主循环，或者RTOS任务线程中
void KEY_Task( );


#endif