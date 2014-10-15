#include <blinky.h>

/*
* Initialize GPIOB's  GPIO_Pin_5 as a push-pull GPIOB pin.
*/
void InitializeLEDs()
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE); 
    GPIO_InitTypeDef gpioStructure;
    gpioStructure.GPIO_Pin = GPIO_Pin_5;
    gpioStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    gpioStructure.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_Init(GPIOB, &gpioStructure); // configure gpio pin 5
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
    timerInitStructure.TIM_Period = 1000;
    timerInitStructure.TIM_ClockDivision = TIM_CKD_DIV2;
    timerInitStructure.TIM_RepetitionCounter = 0;
    TIM_TimeBaseInit(TIM2, &timerInitStructure); // configure timer 4
    TIM_Cmd(TIM2, ENABLE); // enable the counter for timer 4
    TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE); // enable interrupt on timer 4
}

/*
* Enable timer and set to interrupt.
*/
void InitializeMotorsTimer()
{

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
    TIM_TimeBaseInitTypeDef timerInitStructure;
    timerInitStructure.TIM_Prescaler = 7200;
    timerInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
    timerInitStructure.TIM_Period = 20;
    timerInitStructure.TIM_ClockDivision = TIM_CKD_DIV2;
    timerInitStructure.TIM_RepetitionCounter = 0;
    TIM_TimeBaseInit(TIM3, &timerInitStructure); // configure timer 4
    TIM_Cmd(TIM3, ENABLE); // enable the counter for timer 4
    TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE); // enable interrupt on timer 4

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
    TIM_TimeBaseInitTypeDef timerInitStructure2;
    timerInitStructure2.TIM_Prescaler = 7200;
    timerInitStructure2.TIM_CounterMode = TIM_CounterMode_Up;
    timerInitStructure2.TIM_Period = 20;
    timerInitStructure2.TIM_ClockDivision = TIM_CKD_DIV2;
    timerInitStructure2.TIM_RepetitionCounter = 0;
    TIM_TimeBaseInit(TIM4, &timerInitStructure2); // configure timer 4
    TIM_Cmd(TIM4, ENABLE); // enable the counter for timer 4
    TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE); // enable interrupt on timer 4

}


/*
* Enable and initialize interrupt on timer 4
*/
void InitializeInterrupt()
{
    NVIC_InitTypeDef nvic;
    nvic.NVIC_IRQChannel = TIM2_IRQn;
    nvic.NVIC_IRQChannelPreemptionPriority = 0;
    nvic.NVIC_IRQChannelSubPriority = 1;
    nvic.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&nvic); // configure the timer 4 interrupt
}

/*
* Interrupt Callback
*/
void TIM2_IRQHandler()
{
	// If interrupt set, reset interrupt and write the toggled led state
    if (TIM_GetITStatus(TIM2, TIM_IT_Update)!= RESET)
    {
        TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
        led_state = 1-led_state;
        GPIO_WriteBit(GPIOB, GPIO_Pin_5, led_state);
    }
}

/*
* Main Function
*/
int main()
{
	SystemInit();
	//motorsInit();
	//motorsTest();
	InitializeLEDs();
    InitializeTimer();
    InitializeInterrupt();
    while(1);
    return(0);
}

