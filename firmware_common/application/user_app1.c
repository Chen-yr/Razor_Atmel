/**********************************************************************************************************************
File: user_app1.c                                                                

----------------------------------------------------------------------------------------------------------------------
To start a new task using this user_app1 as a template:
 1. Copy both user_app1.c and user_app1.h to the Application directory
 2. Rename the files yournewtaskname.c and yournewtaskname.h
 3. Add yournewtaskname.c and yournewtaskname.h to the Application Include and Source groups in the IAR project
 4. Use ctrl-h (make sure "Match Case" is checked) to find and replace all instances of "user_app1" with "yournewtaskname"
 5. Use ctrl-h to find and replace all instances of "UserApp1" with "YourNewTaskName"
 6. Use ctrl-h to find and replace all instances of "USER_APP1" with "YOUR_NEW_TASK_NAME"
 7. Add a call to YourNewTaskNameInitialize() in the init section of main
 8. Add a call to YourNewTaskNameRunActiveState() in the Super Loop section of main
 9. Update yournewtaskname.h per the instructions at the top of yournewtaskname.h
10. Delete this text (between the dashed lines) and update the Description below to describe your task
----------------------------------------------------------------------------------------------------------------------

Description:
This is a user_app1.c file template 

------------------------------------------------------------------------------------------------------------------------
API:

Public functions:


Protected System functions:
void UserApp1Initialize(void)
Runs required initialzation for the task.  Should only be called once in main init section.

void UserApp1RunActiveState(void)
Runs current task state.  Should only be called once in main loop.


**********************************************************************************************************************/

#include "configuration.h"

/***********************************************************************************************************************
Global variable definitions with scope across entire project.
All Global variable names shall start with "G_UserApp1"
***********************************************************************************************************************/
/* New variables */
volatile u32 G_u32UserApp1Flags;                       /* Global state flags */
static u8 UserApp_au8MyName[] = "DESIGNED BY";         /*LCD显示初始内容   */
static u8 UserApp_au8second[] = "ARE YOU READY"; 
static u8 UserApp_au8third[] = "GAME NOW(^-^)";       /*游戏环节内容*/
static u8 u8timecontrol = 1500 ;                      /*开场动画跳转时间设置 */
static u8 u8startcontrol = 0 ;                        /*开始控制*/ 
static u8 u8gamecontrol = 0 ;                         /*游戏进度控制*/
static u8 u8difcontrol = 0  ;                         /*游戏关卡控制*/
static u16 u16BlinkCount = 0 ;                        /*LED时间控制*/
static u8 u8endcontrol = 0  ;                         /*LED闪烁结束控制*/
static u8 u8rember = 0 ;


/* Existing variables (defined in other files -- should all contain the "extern" keyword) */
extern volatile u32 G_u32SystemFlags;                  /* From main.c */
extern volatile u32 G_u32ApplicationFlags;             /* From main.c */

extern volatile u32 G_u32SystemTime1ms;                /* From board-specific source file */
extern volatile u32 G_u32SystemTime1s;                 /* From board-specific source file */


/***********************************************************************************************************************
Global variable definitions with scope limited to this local application.
Variable names shall start with "UserApp1_" and be declared as static.
***********************************************************************************************************************/
static fnCode_type UserApp1_StateMachine;            /* The state machine function pointer */
//static u32 UserApp1_u32Timeout;                      /* Timeout counter used across states */


/**********************************************************************************************************************
Function Definitions
**********************************************************************************************************************/

/*--------------------------------------------------------------------------------------------------------------------*/
/* Public functions                                                                                                   */
/*--------------------------------------------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------------------------------------------*/
/* Protected functions                                                                                                */
/*--------------------------------------------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------------------------------------------
Function: UserApp1Initialize

Description:
Initializes the State Machine and its variables.

Requires:
  -

Promises:
  - 
*/
void UserApp1Initialize(void)
{
/*******************蜂鸣器BUZZER*************************************/  
  PWMAudioSetFrequency(BUZZER1, 500);
 /******************LED初始化****************************************/
  LedOff(CYAN);
  LedOff(GREEN);
  LedOff(YELLOW);
  LedOff(BLUE);
  LedOff(PURPLE);
  LedOff(ORANGE);
 /********************LCD初始显示************************************/
  LCDCommand(LCD_HOME_CMD);  
  //UserApp_CursorPosition = LINE1_START_ADDR;
  LCDClearChars(LINE1_START_ADDR + 13, 3);
  LCDCommand(LCD_CLEAR_CMD);
  PWMAudioOn(BUZZER1);//开启蜂鸣器
  LCDMessage(LINE1_START_ADDR, UserApp_au8MyName);
  LCDMessage(LINE2_START_ADDR, "t");
  LCDMessage(LINE2_START_ADDR + 6, "t");
  LCDMessage(LINE2_START_ADDR + 13, "x");
  LCDMessage(LINE2_END_ADDR, "s");
  
  /* If good initialization, set state to Idle */
  if( 1 )
  {
    UserApp1_StateMachine = UserApp1SM_Idle;
  }
  else
  {
    /* The task isn't properly initialized, so shut it down and don't run */
    UserApp1_StateMachine = UserApp1SM_Error;
  }

} /* end UserApp1Initialize() */

  
/*----------------------------------------------------------------------------------------------------------------------
Function UserApp1RunActiveState()

Description:
Selects and runs one iteration of the current state in the state machine.
All state machines have a TOTAL of 1ms to execute, so on average n state machines
may take 1ms / n to execute.

Requires:
  - State machine function pointer points at current state

Promises:
  - Calls the function to pointed by the state machine function pointer
*/
void UserApp1RunActiveState(void)
{
  UserApp1_StateMachine();

} /* end UserApp1RunActiveState */


/*--------------------------------------------------------------------------------------------------------------------*/
/* Private functions                                                                                                  */
/*--------------------------------------------------------------------------------------------------------------------*/


/************************************************************************************************************************
State Machine Function Definitions
**********************************************************************************************************************/

/*-------------------------------------------------------------------------------------------------------------------*/
/* Wait for ??? */
static void UserApp1SM_Idle(void)
{
 
  u8timecontrol-- ;
  if(u8timecontrol == 1)
  {
    LCDCommand(LCD_CLEAR_CMD);    //清除第一行屏幕
    LCDMessage(LINE1_START_ADDR, UserApp_au8second);  //跳转显示
    u8timecontrol = 0 ;
  }
  if(IsButtonPressed(BUTTON0)) //按下开始键BUTTON0
  {
    u8startcontrol = 1 ;  
  }
  if(u8startcontrol == 1 )
  {
  /****************LCD变色******************************/
    LedOff(LCD_RED);
    LedOn(LCD_GREEN);
    LedOn(LCD_BLUE);
  /*****************************************************/
    PWMAudioOff(BUZZER1);  //关闭蜂鸣器
    LCDCommand(LCD_CLEAR_CMD);
    LCDMessage(LINE1_START_ADDR, UserApp_au8third); //显示游戏画面
    u8gamecontrol++;
  }
  if(u8gamecontrol == 200 )
  {
    u8startcontrol = 0 ;
    u8difcontrol = 1 ; //游戏第一关转换
  }
  if( u8difcontrol == 1) //游戏第一关
  {
    u16BlinkCount++ ;
    static u8 rem[]={4,5,6};
 
   /**********LED闪烁***********************************************/
    if(u16BlinkCount == 600)
    {
      LedOn(RED);
    }
    if(u16BlinkCount == 1000)
    {
      LedOn(YELLOW);
      LedOff(RED);
    }
    if(u16BlinkCount == 1200)
    {
      LedOn(ORANGE);
      LedOff(YELLOW);
    }
    if(u16BlinkCount == 1400)
    {
      LedOn(BLUE);
      LedOff(ORANGE);
      u8endcontrol = 1;
    }
    
  /*******************************************************************/
  /************按键判断**********************************************/  
    if( IsButtonPressed(BUTTON3) )
    {
      rem[0] = 1;
    }
    if(IsButtonPressed(BUTTON1)&WasButtonPressed(BUTTON3))    
    {
      rem[1] = 2;
    }
    if(IsButtonPressed(BUTTON2)&WasButtonPressed(BUTTON3)&WasButtonPressed(BUTTON1))
    {
      rem[2] = 3;
    }
    if(rem[0]==1&rem[1]==2&rem[2]==3)
    {
      u8rember = 2;
    }
    else if(WasButtonPressed(BUTTON3)&WasButtonPressed(BUTTON2)&WasButtonPressed(BUTTON1))
    {
      u8rember = 3;
    }
  }
 /************************************************************************/
 /************************结果判断**************************************/
   if(u8endcontrol == 1)
   {
    if(u8rember == 2 )
    {
      LedOn(GREEN);
      LedOn(PURPLE);
      LedOn(RED);
      LedOn(YELLOW);
      LedOn(ORANGE);
      LedOn(WHITE);
      LedOn(CYAN);
    }
    else if(u8rember == 3)
    {
       LedOff(BLUE);
       LedOn(YELLOW);
       LedOn(CYAN);
       LedOn(PURPLE);
       LedOn(RED);
  
    }
   }     
    
} /* end UserApp1SM_Idle() */
    

/*-------------------------------------------------------------------------------------------------------------------*/
/* Handle an error */
static void UserApp1SM_Error(void)          
{
  
} /* end UserApp1SM_Error() */



/*--------------------------------------------------------------------------------------------------------------------*/
/* End of File                                                                                                        */
/*--------------------------------------------------------------------------------------------------------------------*/
