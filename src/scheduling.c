#include <scheduling.h>



/*
* Enable timer and set to interrupt.
*/
void InitializeTimer(int tim, int pre, int per)
{

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);
    
    TIM_TimeBaseInitTypeDef timerInitStructure;
    timerInitStructure.TIM_Prescaler = 7200;
    timerInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
    timerInitStructure.TIM_Period = 1000;
    timerInitStructure.TIM_ClockDivision = TIM_CKD_DIV2;
    timerInitStructure.TIM_RepetitionCounter = 0;
    TIM_TimeBaseInit(TIM5, &timerInitStructure); // configure timer
    TIM_Cmd(TIM5, ENABLE); // enable the counter for timer
    TIM_ITConfig(TIM5, TIM_IT_Update, ENABLE); // enable interrupt on timer
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
void TIM5_IRQHandler()
{
    count += 1;
    // If interrupt set, reset interrupt and write the toggled led state
    if (count == 10)
    {
	functions = 00001;	
	detectEmergency();
    }
    if (count == 100)
    {
	functions = 00011;
	detectEmergency();
	refreshSensorData();
    }
    if (count == 995){
	functions = 10000;
	logDebugInfo();
    }
    if (count == 1000)
    {
	functions = 01111;
	detectEmergency();
	refreshSensorData();
	calculateOrientation();
	updatePid(motorSpeeds);
    }
}

void schedule()
{
    InitializeTimer(TIM5_IRQn, 7200,10);
    InitializeInterruptGen(TIM5_IRQn);
}


