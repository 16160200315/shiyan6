#include"LPC11xx.h"  //LPC1114ͷ�ļ�
#define LED  (1ul<<0)
#define LEDOFF() LPC_GPIO2->DATA|=LED //Ϩ��LED
#define LEDON() LPC_GPIO2->DATA&=~LED //����LED
/***************************************************
*FunctionName : LedInit()
*Description :LED��ʼ������
*EntryParameter :None
*ReturnValue :None
***************************************************/
void LedInit()
{
	LPC_SYSCON->SYSAHBCLKCTRL |=(1<<16);    //ʹ��IOCONʱ��
	LPC_IOCON->PIO2_0&=~0X07;
	LPC_IOCON->PIO2_0|=~0X00;    //��P2.0������ΪGPIO
	LPC_SYSCON->SYSAHBCLKCTRL &=~(1<<16);    //����IOCONʱ��
	LPC_SYSCON->SYSAHBCLKCTRL |=(1<<6);      //ʹ��GPIOʱ��
	LPC_GPIO2->DIR |=LED;  //��P2.0����Ϊ�������
  LPC_GPIO2->DATA|=LED; //��P2.0����Ϊ�ߵ�ƽ
}
/***************************************************
*FunctionName : TIMER32_0_IRQHandler()
*Description :32λ��ʱ������0�жϷ�����
*EntryParameter :None
*ReturnValue :None
***************************************************/
void TIMER32_0_IRQHandler(void)
{
	LPC_TMR32B0->IR=0X01; //��ƥ��ͨ��0д1����ж�
	if(LPC_GPIO2->DATA&LED)
	{
		LEDON();  //����LED
	}
	else
	{
		LEDOFF(); //Ϩ��LED
	}
}
/***************************************************
*FunctionName : T32B1_Init()
*Description :32λ��ʱ������0��ʼ������
*EntryParameter :None
*ReturnValue :None
***************************************************/
void Timer0Init(void)
{
	LPC_SYSCON->SYSAHBCLKCTRL|=(1<<9); //ʹ��TIM32B0ʱ��
	LPC_TMR32B0->IR=0X01; //MR0�жϸ�λ�������жϣ�bit0:MR0,bit1:MR1,bit2:MR2,bit3:MR3,bit4:CP0)
	LPC_TMR32B0->PR=0;   //ÿһ��PCLK,TC��1
	LPC_TMR32B0->MCR=0X03; //MR0ƥ��ʱ��λTC,�������ж�
	LPC_TMR32B0->MR0=SystemCoreClock; //ƥ��ʱ�䣬Ҳ���Ƕ�ʱʱ��
	LPC_TMR32B0->TCR=0X01; //������ʱ����TCR[0]=1;
	NVIC_EnableIRQ(TIMER_32_0_IRQn); //ʹ���ж�
}
/***************************************************
*FunctionName : main()
*Description :������
*EntryParameter :None
*ReturnValue :None
***************************************************/
int main(void)
{
	LedInit();
	Timer0Init();
	while(1);
}