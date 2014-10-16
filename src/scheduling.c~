#include <scheduling.h>

int led = 0;

/*
* Enable timer and set to interrupt.
*/
void InitializeTimer(int tim, int pre, int per)
{

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
    
    TIM_TimeBaseInitTypeDef timerInitStructure;
    timerInitStructure.TIM_Prescaler = 7200;
    timerInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
    timerInitStructure.TIM_Period = 10;
    timerInitStructure.TIM_ClockDivision = TIM_CKD_DIV2;
    timerInitStructure.TIM_RepetitionCounter = 0;
    TIM_TimeBaseInit(TIM2, &timerInitStructure); // configure timer
    TIM_Cmd(TIM2, ENABLE); // enable the counter for timer
    TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE); // enable interrupt on timer
}

/*
* Enable and initialize interrupt on timer
*/
void InitializeInterruptGen(int TIM_IRQn)
{
    NVIC_InitTypeDef nvic;
    nvic.NVIC_IRQChannel = TIM_IRQn;
    nvic.NVIC_IRQChannelPreemptionPriority = 0;
    nvic.NVIC_IRQChannelSubPriority = 1;
    nvic.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&nvic); // configure the timer interrupt

}

/*
* Interrupt Callback for the LEDs, flips green light at 1hz
*/
void TIM2_IRQHandler()
{
    if (TIM_GetITStatus(TIM2, TIM_IT_Update)!= RESET)
    {
        TIM_ClearITPendingBit(TIM2, TIM_IT_Update);

	    count += 1;
	    // If interrupt set, reset interrupt and write the toggled led state
	    if (count == 10)
	    {
		functions = 00001;	
	    }
	    if (count == 100)
	    {
		
		functions = 00011;
	    }
	    if (count == 995){
		functions = 10000;
	    }
	    if (count == 1000)
	    {
		led = 1-led;
		GPIO_WriteBit(GPIOB, GPIO_Pin_5, led);
		functions = 01111;
		count = 0;
	    }

    }

}

void schedule()
{
    InitializeTimer(TIM2_IRQn, 7200,10);
    InitializeInterruptGen(TIM2_IRQn);
}


