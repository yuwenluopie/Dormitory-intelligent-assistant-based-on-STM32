#include "stm32f10x.h"
#include "delay.h"
#include "Reg_RW.h"
#include "LDChip.h"
#include "usart.h"
#include "syn6288.h"
#include "busy.h"
#include "relay.h"
#include "timer.h"
#include "iwdg.h"
#include "pwm.h"

void User_Modification(u8 dat);

u8 nAsrStatus = 0;
u8 nAsrRes = 0;
u8 h_flag = 0;

int main(void)
{
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置中断优先级分组为组2：2位抢占优先级，2位响应优先级
  USART1_Init(9600);
  TIM3_Int_Init(499, 7199);
  LD3320_Init();
  BUSY_Init();
  RELAY_Init();
  EXTIX_Init();
  LD_Reset();
  IWDG_Config(IWDG_Prescaler_64, 625);
  nAsrStatus = LD_ASR_NONE;		//	初始状态：没有在作ASR
  SCS = 0;
  //选择背景音乐2。(0：无背景音乐  1-15：背景音乐可选)
  //v[0~16]:0背景音乐为静音，16背景音乐音量最大
  //m[0~16]:0朗读音量为静音，16朗读音量最大
  //t[0~5]:0朗读语速最慢，5朗读语速最快
  if (RCC_GetFlagStatus(RCC_FLAG_IWDGRST) != RESET)
  {
    RCC_ClearFlag();
  }
  else
  {
    delay_ms(500);
    SYN_FrameInfo(0, "[m1][o0][t5]主人，你好");
  }
  delay_ms(10);
  while(BUSY == 1) {};
  while(1)
  {
    if(overtime >= 300 && h_flag == 1)
    {
      LED1 = 1;
      overtime = 0;
      h_flag = 0;
      SYN_FrameInfo(0, "[o0][t5]主人，我先退下了。");
      delay_ms(10);
      while(BUSY == 1);
    }
    switch(nAsrStatus)
    {
    case LD_ASR_RUNING:
    case LD_ASR_ERROR:
      break;
    case LD_ASR_NONE:
    {
      nAsrStatus = LD_ASR_RUNING;
      if (RunASR() == 0)	/*	启动一次ASR识别流程：ASR初始化，ASR添加关键词语，启动ASR运算*/
      {
        nAsrStatus = LD_ASR_ERROR;
      }
      break;
    }

    case LD_ASR_FOUNDOK: /*	一次ASR识别流程结束，去取ASR识别结果*/
    {
      EXTI->IMR &= ~(EXTI_Line9); //屏蔽外部中断9
      nAsrRes = LD_GetResult();		/*获取结果*/
      User_Modification(nAsrRes);
      nAsrStatus = LD_ASR_NONE;
      EXTI->IMR |= EXTI_Line9; //使能外部中断9
      break;
    }
    case LD_ASR_FOUNDZERO:
    default:
    {
      nAsrStatus = LD_ASR_NONE;
      break;
    }
    }
  }
}


void User_Modification(u8 dat)
{
  if(dat == 0 && h_flag == 0)
  {
    LED1 = 0;
    h_flag = 1;
    overtime = 0;
    SYN_FrameInfo(0, "[o0][t5]我在");
    delay_ms(10);
    while(BUSY == 1);
  }
  else if(h_flag == 1)
  {
    switch(nAsrRes)		   /*对结果执行相关操作,客户修改*/
    {
    case CODE_1KL1:			/*命令*/
      overtime = 0;
      SYN_FrameInfo(0, "灯已打开"); /*语音返回.....*/
      delay_ms(10);
      RELAY8 = 0;
      while(BUSY == 1) {};
      break;
    case CODE_1KL2:			/*命令*/
      overtime = 0;
      SYN_FrameInfo(0, "灯已关闭"); /*语音返回.....*/
      delay_ms(10);
      RELAY8 = 1;
      while(BUSY == 1) {};
      break;

    case CODE_1KL3:	 /*命令*/
      overtime = 0;
      SYN_FrameInfo(0, "风扇已打开"); /*语音返回.....*/
      delay_ms(10);
      RELAY7 = 0;
      while(BUSY == 1) {};
      break;
    case CODE_1KL4:		/*命令*/
      overtime = 0;
      SYN_FrameInfo(0, "风扇已关闭"); /*语音返回.....*/
      delay_ms(10);
      RELAY7 = 1;
      while(BUSY == 1) {};
      break;
    case CODE_1KL5:	 /*命令*/
      overtime = 0;
      SYN_FrameInfo(0, "摄像头已打开"); /*语音返回.....*/
      delay_ms(10);
      RELAY6 = 0;
      while(BUSY == 1) {};
      break;
    case CODE_1KL6:		/*命令*/
      overtime = 0;
      SYN_FrameInfo(0, "摄像头已关闭"); /*语音返回.....*/
      delay_ms(10);
      RELAY6 = 1;
      while(BUSY == 1) {};
      break;

    case CODE_1KL7:	 /*命令*/
      overtime = 0;
      SYN_FrameInfo(0, "窗帘已打开"); /*语音返回.....*/
      delay_ms(10);
      RELAY4 = 0;
		  delay_ms(8000);
		  RELAY4 = 1;
      while(BUSY == 1) {};
      break;
    case CODE_2KL1:	 /*命令*/
      overtime = 0;
      SYN_FrameInfo(0, "窗帘已关闭"); /*语音返回.....*/
      delay_ms(10);
      RELAY3 = 0;
		  delay_ms(10000);
		  RELAY3 = 1;
      while(BUSY == 1) {};
      break;
    case CODE_2KL2:	 /*命令*/
      overtime = 0;
      SYN_FrameInfo(0, "防御模式已打开"); /*语音返回.....*/
      delay_ms(10);
      RELAY6 = RELAY5 = 0;
      while(BUSY == 1) {};
      break;
    case CODE_2KL3:	 /*命令*/
      overtime = 0;
      SYN_FrameInfo(0, "防御模式已关闭"); /*语音返回.....*/
      delay_ms(10);
      RELAY6 = RELAY5 = 1;
      while(BUSY == 1) {};
      break;

    case CODE_2KL4:	 /*命令*/
      overtime = 0;
      SYN_FrameInfo(0, "音乐已打开"); /*语音返回.....*/
      delay_ms(10);
      RELAY2 = 0;
      while(BUSY == 1) {};
      break;
    case CODE_2KL5:	 /*命令*/
      overtime = 0;
      SYN_FrameInfo(0, "音乐已关闭"); /*语音返回.....*/
      delay_ms(10);
      RELAY2 = 1;
      while(BUSY == 1) {};
      break;
    case CODE_2KL6:	 /*命令*/
      overtime = 0;
      SYN_FrameInfo(0, "收音机已打开"); /*语音返回.....*/
      delay_ms(10);
      RELAY2 = 0;
      while(BUSY == 1) {};
      break;
    case CODE_2KL7:	 /*命令*/
      overtime = 0;
      SYN_FrameInfo(0, "收音机已关闭"); /*语音返回.....*/
      delay_ms(10);
      RELAY2 = 1;
      while(BUSY == 1) {};
      break;
    case CODE_3KL1:	 /*命令*/
      overtime = 0;
      SYN_FrameInfo(0, "水瓶已打开"); /*语音返回.....*/
      delay_ms(10);
      RELAY1 = 0;
      while(BUSY == 1) {};
      break;
    case CODE_3KL2:	 /*命令*/
      overtime = 0;
      SYN_FrameInfo(0, "水瓶已关闭"); /*语音返回.....*/
      delay_ms(10);
      RELAY1 = 1;
      while(BUSY == 1) {};
      break;
    case CODE_3KL3:	 /*命令*/
      overtime = 0;
      SYN_FrameInfo(0, "已全部打开"); /*语音返回.....*/
      delay_ms(10);
      RELAY8 = RELAY7 = RELAY6 = RELAY5 = RELAY4 = 0;
		  delay_ms(8000);
		  RELAY4 = 1;
      while(BUSY == 1) {};
      break;
    case CODE_3KL4:	 /*命令*/
      overtime = 0;
      SYN_FrameInfo(0, "已全部关闭"); /*语音返回.....*/
      delay_ms(10);
      RELAY8 = RELAY7 = RELAY6 = RELAY5 = 1;
		  RELAY3 = 0;
		  delay_ms(10000);
		  RELAY3 = 1;
      while(BUSY == 1) {};
      break;
    case CODE_3KL5:	 /*命令*/
      LED1 = 1;
      overtime = 0;
	h_flag = 0;
      SYN_FrameInfo(0, "好的，我先退下了。"); /*语音返回.....*/
      delay_ms(10);
      while(BUSY == 1) {};
      break;
    default:
      overtime = 0;
      h_flag = 0;
      SYN_FrameInfo(0, "没听清楚"); /*语音返回.....*/
      delay_ms(10);
      while(BUSY == 1) {};
      break;
    }
  }
  else
  {
    //printf("请说出一级口令\r\n"); /*text.....*/
  }

}

