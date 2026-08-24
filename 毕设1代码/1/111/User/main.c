#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "Motor.h"
#include "PWM.h"
#include "Delay.h"
#include "Timer.h"
#include "Serial1.h"
#include "Serial2.h"
#include "Serial3.h"
#include "Serial4.h"
#include "WZJZ.h"
#include "ShiQv.h"
int ARR,PSC , CCR;  //左L 右R
int Data,dd,qq,ww,yy,zz;
uint8_t RxData;
u16 set_speed = 0;
int main(void)
{
	 OLED_Init();
	 ARR = 200;
   PSC = 72;
	 CCR = 100;
	 Timer_Init(120,720);
	 Timer_Init_seven(120,720);
	 Serial1_Init();
	 Serial_Init2();
	 Motor_InitC();
	 Motor_Init();
	 Motor_InitD();
	 PWM_Init3(ARR,PSC,ARR/2);  //右
	 PWM_Init2(ARR,PSC,ARR/2);  //右
	 PWM_Init1(ARR,PSC,ARR/2);   //左
	 PWM_Init(ARR,PSC,ARR/2);    //左   //ARR  PSC  CCR   arr过高会堵转  f越高脉冲越多
	 CountSensor1_Init();
	
	while (1)
	{		
		Serial_Printf2("page0.t1.txt=\"%d\"\xff\xff\xff",350-ARR);
		if(Data == 1)
		{
			Motor_go();
			Data = 0;
			Serial_Printf2("page0.t3.txt=\"go\"\xff\xff\xff");
		}
		if(Data == 2)
		{
			Motor_left();
			Data = 0;
			Serial_Printf2("page0.t3.txt=\"left\"\xff\xff\xff");
		}
		if(Data == 3)
		{
			Motor_right();
			Data = 0;
			Serial_Printf2("page0.t3.txt=\"right\"\xff\xff\xff");
		}
		if(Data == 4)
		{
			Motor_back();
			Data = 0;
			Serial_Printf2("page0.t3.txt=\"back\"\xff\xff\xff");
		}
		if(Data == 5)
		{
			Motor_stop();
			Data = 0;
		}
		if(dd == 1)
		{
			Buzzer_ON();
			qq=1;
			Data = 0;
		}
		if(Data == 6)
		{
			GPIO_SetBits(GPIOD,GPIO_Pin_8);
			Motor_stop();
			Data = 0;
		}
		if(Data == 7)
		{
			GPIO_ResetBits(GPIOD,GPIO_Pin_8);
			Motor_stop();
			Data = 0;
		}
		if(Data == 8)
		{
			set_speed = 1;
			Data = 0;
		}
		if(Data == 9)
		{
			set_speed = 2;
			Data = 0;
		}
	}
}


void USART1_IRQHandler(void)
{
	if (USART_GetITStatus(USART1, USART_IT_RXNE) == SET)
	{
		RxData = USART_ReceiveData(USART1);
		if(RxData == 1)
		{
			Data = 1;
		}
		if(RxData == 2)
		{
			Data = 2;
		}
		if(RxData == 3)
		{
			Data = 3;
		}
		if(RxData == 4)
		{
			Data = 4;
		}
		if(RxData == 5)
		{
			Data = 5;
		}
		if(RxData == 6)
		{
			Data = 6;
		}
		if(RxData == 7)
		{
			Data = 7;
		}
		if(RxData == 8)
		{
			Data = 8;
		}
		if(RxData == 9)
		{
			Data = 9;
		}
		USART_ClearITPendingBit(USART1, USART_IT_RXNE);
	}
}

void EXTI15_10_IRQHandler(void)
{
	if (EXTI_GetITStatus(EXTI_Line14) == SET)
	{ 
		/*如果出现数据乱跳的现象，可再次判断引脚电平，以避免抖动*/
		if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_14) == 0)
		{
			dd=1;
		}
	}
	EXTI_ClearITPendingBit(EXTI_Line14);
}

void TIM6_IRQHandler(void)         //定时器6，处理标志位响应逻辑
{
	if (TIM_GetITStatus(TIM6, TIM_IT_Update) == SET)  //获取中断标志位
	{
		if(set_speed == 1)
		{
			yy++;
			if(yy >= 100)
			{
				ARR = ARR - 30;
				if(ARR < 50)
				{
					ARR = 200;
				}
				PWM_Init3(ARR,PSC,ARR/2);  //右
				PWM_Init2(ARR,PSC,ARR/2);  //右
				PWM_Init1(ARR,PSC,ARR/2);   //左
				PWM_Init(ARR,PSC,ARR/2);    //左   //ARR  PSC  CCR   arr过高会堵转  f越高脉冲越多
				yy = 0;
				set_speed = 0;
			}
		}
		
		if(set_speed == 2)
		{
			zz++;
			if(zz >= 100)
			{
				ARR = ARR + 30;
				if(ARR > 350)
				{
					ARR = 200;
				}
				PWM_Init3(ARR,PSC,ARR/2);  //右
				PWM_Init2(ARR,PSC,ARR/2);  //右
				PWM_Init1(ARR,PSC,ARR/2);   //左
				PWM_Init(ARR,PSC,ARR/2);    //左   //ARR  PSC  CCR   arr过高会堵转  f越高脉冲越多
				zz = 0;
				set_speed = 0;
			}
		}
		
		if(qq == 1)
		{
			ww++;
			if(ww >= 500)
			{
				Buzzer_OFF();
				dd=2;
				ww=0; 
				qq=2;
			}
		}
		TIM_ClearITPendingBit(TIM6, TIM_IT_Update);  //清除中断标志位
	}
}

void TIM7_IRQHandler(void)         //定時器7，处理基本方向逻辑
{
	if (TIM_GetITStatus(TIM7, TIM_IT_Update) == SET)  //获取中断标志位
	{
		
		TIM_ClearITPendingBit(TIM7, TIM_IT_Update);  //清除中断标志位
	}
}
