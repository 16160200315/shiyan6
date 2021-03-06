#include"LPC11xx.h"  //LPC1114头文件
#define LED  (1ul<<0)
#define LEDOFF() LPC_GPIO2->DATA|=LED //熄灭LED
#define LEDON() LPC_GPIO2->DATA&=~LED //点亮LED
/***************************************************
*FunctionName : LedInit()
*Description :LED初始化函数
*EntryParameter :None
*ReturnValue :None
***************************************************/
void LedInit()
{
	LPC_SYSCON->SYSAHBCLKCTRL |=(1<<16);    //使能IOCON时钟
	LPC_IOCON->PIO2_0&=~0X07;
	LPC_IOCON->PIO2_0|=~0X00;    //把P2.0脚设置为GPIO
	LPC_SYSCON->SYSAHBCLKCTRL &=~(1<<16);    //禁能IOCON时钟
	LPC_SYSCON->SYSAHBCLKCTRL |=(1<<6);      //使能GPIO时钟
	LPC_GPIO2->DIR |=LED;  //把P2.0设置为输出引脚
  LPC_GPIO2->DATA|=LED; //把P2.0设置为高电平
}
/***************************************************
*FunctionName : TIMER32_0_IRQHandler()
*Description :32位定时计数器0中断服务函数
*EntryParameter :None
*ReturnValue :None
***************************************************/
void TIMER32_0_IRQHandler(void)
{
	LPC_TMR32B0->IR=0X01; //向匹配通道0写1清除中断
	if(LPC_GPIO2->DATA&LED)
	{
		LEDON();  //点亮LED
	}
	else
	{
		LEDOFF(); //熄灭LED
	}
}
/***************************************************
*FunctionName : T32B1_Init()
*Description :32位定时计数器0初始化函数
*EntryParameter :None
*ReturnValue :None
***************************************************/
void Timer0Init(void)
{
	LPC_SYSCON->SYSAHBCLKCTRL|=(1<<9); //使能TIM32B0时钟
	LPC_TMR32B0->IR=0X01; //MR0中断复位，即清中断（bit0:MR0,bit1:MR1,bit2:MR2,bit3:MR3,bit4:CP0)
	LPC_TMR32B0->PR=0;   //每一个PCLK,TC加1
	LPC_TMR32B0->MCR=0X03; //MR0匹配时复位TC,并产生中断
	LPC_TMR32B0->MR0=SystemCoreClock; //匹配时间，也就是定时时间
	LPC_TMR32B0->TCR=0X01; //启动定时器：TCR[0]=1;
	NVIC_EnableIRQ(TIMER_32_0_IRQn); //使能中断
}
/***************************************************
*FunctionName : main()
*Description :主函数
*EntryParameter :None
*ReturnValue :None
***************************************************/
int main(void)
{
	LedInit();
	Timer0Init();
	while(1);
}