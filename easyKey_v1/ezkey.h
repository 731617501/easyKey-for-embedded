#ifndef __KEY_H
#define __KEY_H

#include "stdint.h"

#define DOUBLECLICK_TIME 1000
#define KEEPON_TIME      1000

/*
若定时器回调函数时间周期为1ms 则读取间隔为20ms  
If the timer callback function has a period of 1 ms, then the reading interval is 20 ms.
*/ 
#define READ_DELAY       20    

/*
定义你所需要的最大按键的数量
define the maximum number of keys you need to use.
*/
#define numKeys 4


typedef void* pGPIO;
typedef struct key* KEY ;
typedef uint8_t (*pfun)();


KEY KEY_Init ( uint8_t KeyActiveLevel , pfun io_read );
uint8_t KEY_UP( KEY key );
void KEY_Tick();



#endif