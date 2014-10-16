#include <scheduling.h>

/*
* Enable timer and set to interrupt.
*/
void InitializeTimer()
{

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
    
    TIM_TimeBaseInitTypeDef timerInitStructure;
    timerInitStructure.TIM_Prescaler = 7200;
    timerInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
    timerInitStructure.TIM_Period = 1;
    timerInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    timerInitStructure.TIM_RepetitionCounter = 0;
    TIM_TimeBaseInit(TIM2, &timerInitStructure); // configure timer
    TIM_Cmd(TIM2, ENABLE); // ena1le the counter for timer
    TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE); // enable interrupt on timer
}

/*
* Enable and initialize interrupt on timer
*/
void InitializeInterruptGen()
{
    NVIC_InitTypeDef nvic;
    nvic.NVIC_IRQChannel = TIM2_IRQn;
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

	    count ++;
	    // If interrupt set, reset interrupt and write the toggled led state
	    if (count == 10)
	    {
		functions = 00001;	
		isCalled = 1;
	    }
	    if (count == 100)
	    {		
		functions = 00011;
		isCalled = 1;
	    }
	    if (count == 995){
		functions = 10000;
		isCalled = 1;
	    }

	    if (count == 1000)
	    {

		functions = 01111;
		count = 0;
		isCalled = 1;
	    }

    }

}

void schedule()
{
    InitializeTimer();
    InitializeInterruptGen();
}


