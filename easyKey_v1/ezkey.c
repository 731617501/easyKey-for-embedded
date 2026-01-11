#include "ezkey.h"


//KEY结构体定义
struct key
{
  uint8_t       ACTIVE_LEVEL;  //按键的有效电平
  uint8_t       pre_isPress;  
  uint8_t       isPress;
  uint8_t       keepTime;
  uint8_t       isUp;
  pfun          io_read;
};

//KEY按键数组（包含全部按键）
struct key KeyArray[numKeys];

//即代表下一个要初始化的按键，代表目前已经初始化的个数
volatile static uint8_t numInitKey = 0 ;


KEY KEY_Init ( uint8_t KeyActiveLevel , pfun io_read )
{

 
  KeyArray[numInitKey].ACTIVE_LEVEL = KeyActiveLevel;
  KeyArray[numInitKey].pre_isPress = 0 ;
  KeyArray[numInitKey].isPress = 0 ;
  KeyArray[numInitKey].keepTime = 0;
  KeyArray[numInitKey].io_read = io_read ;
  
  //指向下一个初始化按键
  numInitKey++;
  
  return &KeyArray[numInitKey-1];

}

void KEY_Scan()
{
  for( int i = 0 ; i < numInitKey ; i++  )
  {
    KeyArray[i].pre_isPress = KeyArray[i].isPress ;
    if( KeyArray[i].io_read() == KeyArray[i].ACTIVE_LEVEL )  
         (&KeyArray[i])->isPress = 1 ;
    else (&KeyArray[i])->isPress = 0 ;

  }
}


void KEY_UP_Compare()
{
  for( int i = 0 ;  i < numInitKey ; i++ )
  {
    if( !KeyArray[i].isPress && KeyArray[i].pre_isPress )  
         (&KeyArray[i])->isUp = 1 ;
  }
}


void KEY_Tick()
{
  static uint8_t key_count ; 
  key_count++;

  //经过READ_DELAY次进入按键扫描
  if( key_count > READ_DELAY ){
    KEY_Scan();
    KEY_UP_Compare();

    key_count = 0 ;
  }
}


uint8_t KEY_UP( KEY key )
{
  if( key->isUp )
  {
    key->isUp = 0;
    return 1;
  }
  return 0; 
}

