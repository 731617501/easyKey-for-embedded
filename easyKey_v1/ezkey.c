#include "ezkey.h"
#include <stdint.h>


//------------------------------ KEY Enum Def -----------------------------//

typedef enum {
    INIT = 1 ,
    NOT_INIT
} init_status;

typedef enum {
    PRESS ,
    NOT_PRESS
} key_status;

typedef enum {
    TRIGGER ,
    NOT_TRIGGER
} key_trigger_status;

//------------------------------ KEY Struct Def -----------------------------//

struct key
{
    init_status                         is_init ;
    //---------------- 用户自定义部分 ---------------//

    //------------- 有效电平 -----------------//
    uint8_t                        active_level ;
    //------------- 回调函数cb ---------------//
    pf_read                          pf_io_read ;
    pf_key_callback           pf_click_callback ;
    pf_key_callback      pf_long_press_callback ;

    //----------- end of 用户自定义部分 ------------//


    //------------- 由Scan控制 -----------------//
    key_status                          isPress ;
    uint8_t                       confirm_count ;

    //------- 产生于Scan ，使用并恢复于Task ----------//
    key_trigger_status                     isUp ;
    uint32_t                          keep_time ;// [ms]

    

};



//KEY按键数组（包含全部按键）
static struct key KeyArray[numKeys];


//即代表下一个要初始化的按键，代表目前已经初始化的个数
volatile static uint8_t numInitKey = 0 ;



//初始化按键,绑定读取函数和触发后的回调函数
void KEY_Init (  uint8_t              key_active_level , 
                 pf_read                       io_read , 
                 pf_key_callback        click_callback , 
                 pf_key_callback   long_press_callback    )
{
    
    //-----0.用户设置初始化---------//
    KeyArray[numInitKey].active_level             =       key_active_level ;
    KeyArray[numInitKey].pf_io_read               =                io_read ;
    KeyArray[numInitKey].pf_click_callback        =         click_callback ;
    KeyArray[numInitKey].pf_long_press_callback   =    long_press_callback ;
    
    //-----1.按键设置初始化---------//
    KeyArray[numInitKey].is_init      =         INIT ;
    KeyArray[numInitKey].isPress      =    NOT_PRESS ;
    KeyArray[numInitKey].isUp         =  NOT_TRIGGER ;
    KeyArray[numInitKey].keep_time    =            0 ;


    //初始化完成，初始化各数+1
    numInitKey++;

}

void KEY_Task( )
{   
    //按键组遍历
    for( int i = 0 ; i < numInitKey ; i++  )
    {
         //检查是否初始化，以及被触发
        if( INIT    ==  KeyArray[i].is_init  && 
            TRIGGER ==  KeyArray[i].isUp        )
        {
            //触发逻辑部分
            if( LONG_PRESS_TIME < KeyArray[i].keep_time &&
                KeyArray[i].keep_time < MAX_PRESS_TIME       )
            {
                KeyArray[i].pf_long_press_callback();

            }
            else if( 0 <= KeyArray[i].keep_time    &&
                    KeyArray[i].keep_time < LONG_PRESS_TIME  )
            {
                KeyArray[i].pf_click_callback();
            }
            
            //还原isUp和keeptime
            KeyArray[i].keep_time = 0 ;
            KeyArray[i].isUp = NOT_TRIGGER ;
        }
    }
}


void KEY_Scan()
{   
    //按键组遍历
    for( int i = 0 ; i < numInitKey ; i++  )
    {
        //检查是否初始化
        if( INIT == KeyArray[i].is_init )
        {
            //---扫描逻辑实现---//
            if( KeyArray[i].active_level 
                                         == KeyArray[i].pf_io_read() )
            {
                if ( NOT_PRESS == KeyArray[i].isPress ) {

                    KeyArray[i].confirm_count++;
                    if( KeyArray[i].confirm_count >= CONFRIM_COUNT )
                    {
                        KeyArray[i].isPress = PRESS ;
                        KeyArray[i].confirm_count = 0 ;
                    }
                }
                else if ( PRESS == KeyArray[i].isPress) 
                {
                    KeyArray[i].keep_time += READ_DELAY ;
                }
            }
            else 
            {
                if( PRESS == KeyArray[i].isPress )
                {
                    KeyArray[i].isUp = TRIGGER ;
                }
                
                KeyArray[i].isPress = NOT_PRESS ;
            }
            
        }

        
    }
}


void KEY_Tick()
{
    static uint8_t key_count ; 
    key_count++;

    //经过READ_DELAY次进入按键扫描
    if( key_count > READ_DELAY ){
        KEY_Scan();

        key_count = 0 ;
    }
}

