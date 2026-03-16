# EasyKey

就像这个仓库的名字一样，这是一个非常好上手，无论是实现还是使用都很简单的库。但是请不用担心EasyKey是否会不稳定，不好用，你只要用简单的一点配置，就可以拥有一个/一群非常好用的按键。

### Quick Start
1. `KEY_Tick()`
EasyKey需要一个被定时触发的函数，一般情况下是你提供一个定时器，并把KEY_Tick()放入其重装载的回调函数中，从而固定时间间隔的按键扫描,以STM的HAL库为例就像下面一样：
``` c
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if( htim == &htim2 )
    {
        KEY_Tick();
    }
}
```
2.  `KEY_Init()`使用KEY_Init来初始化你的按键，他需要四个参数
> active_level : 激活电平 
> io_read      : 按键电平读取函数
> click_cb     : 点击按键时会触发的函数
> long_press_cb: 长按按键的收会触发的函数

``` c
KEY key1 = KEY_Init( active_level , io_read, click_cb , long_press_cb );
```

3. `KEY_Task`这是最后一步，你只需要把KEY_Task()放在裸机的主循环中，或者是RTOS的任务中,然后一切你的按键就会生效，你就可以正常使用了

```c
void Task()
{
    for(;;)
    {
        KEY_Task();
    }
}
```

### 配置参数
```c
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

```
注意如果如果并没有使用那么多按键，请不要把最大按键数填的太大，因为为了自动管理所有按键，会提前声明一个最大数量按键的数组

### 说明
#### 对于使用者
EasyKey本质上是为了成为一个最快速，最简单的按键模块，你只需要查看`ezkey.h`就可以知道这个库可配置可使用的，而且没有任何多余的东西在其中，为的就是让EasyKey成为一个不要有那么多配置，那么多可调用函数的库，这只会让第一眼看到的人觉得眼花。你只需要用Init函数告诉我们你按键的信息和在被按下的时候需要调用什么函数，剩下的一切都交给EasyKey。
如果你还是不会使用可以看一看DEMO文件夹中的例程
#### 对于想要自定义这个库的开发者
EasyKey本身的实现逻辑也非常直观，而且简单，并不使用过于复杂的抽象逻辑，并且在库中都有写好相应的注释

#### verilog版本
verilog版本由WuZhenhuan428提供

### 写在最后
有什么问题，或者想讨论的都可以发在issue中，非常乐意接受批评和更改，EasyKey的目标就是做一个使用最简单，兼容性也很好，非常利于小白上手的仓库，同时我们也希望EasyKey可以有非常好的性能。目前的easyKey的实现确实特别简单，所以也没有引入错误系统，如果还有后续开发，可能会考虑引入。
