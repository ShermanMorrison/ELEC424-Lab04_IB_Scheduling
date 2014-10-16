#include <scheduling.h>


/*
* Add a function to the queue
*/
void AddFunction(int *queue, int pos)
{
    if (pos<NUM_FUNCTIONS){
        queue[pos]++;
    }
}

int HandleFunction(int *queue)
{
    int l;
    for(l=0; l<NUM_FUNCTIONS; l++)
    {
        if(queue[l]>0)
        {
            //TODO
        }
    }


}

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
        if (count % 10 == 0)
        {
            AddFunction(scheduled_functions, 1);	
	    isCalled = 1;
	}
	if (count % 100 == 0)
	{
	    AddFunction(scheduled_functions, 2);
            isCalled = 1;
	}
	if (count == 995)
        {
            green_state = 1-green_state;
            if (green_state == 0)
                red_state = 1-red_state;
            GPIO_WriteBit(GPIOB, GPIO_Pin_5, green_state);
            GPIO_WriteBit(GPIOB, GPIO_Pin_4, red_state);
            AddFunction(scheduled_functions, 5);
            isCalled = 1;
	}
	if (count == 1000)
	{
            AddFunction(scheduled_functions, 3);
            AddFunction(scheduled_functions, 4);
            count = 0;
	    isCalled = 1;
	}

    }

}

void schedule()
{
    for(k=0;k<NUM_FUNCTIONS; k++)
        scheduled_functions[k] =0;
    InitializeTimer();
    InitializeInterruptGen();
}


