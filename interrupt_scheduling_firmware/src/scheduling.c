#include <scheduling.h>


// this references
extern int calcOrientFlag;
/*
* Enable timer and set to interrupt.
*/
void InitializeTimer()
{
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
 
    TIM_TimeBaseInitTypeDef timerInitStructure;
    timerInitStructure.TIM_Prescaler = 80; 
    timerInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
    timerInitStructure.TIM_Period = 1000;
    timerInitStructure.TIM_ClockDivision = 0;
    timerInitStructure.TIM_RepetitionCounter = 0;
    TIM_TimeBaseInit(TIM2, &timerInitStructure); // configure timer
    TIM_Cmd(TIM2, ENABLE); // enable the counter for timer
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
* Interrupt Callback for the LEDs, calcOrientation,updatePid
*/
void TIM2_IRQHandler()
{

        detectEmergency();
        count1++;
        if (count1 % 10 == 0)
        {
            // run refresh sensor every time
            refreshSensorData();
        }
        
        if (count1 % 100 == 0)
        {
            // now control the led toggle at 1hz
            if(green_state == 0)
                red_state = 1-red_state;
            green_state = 1-green_state;
            GPIO_WriteBit(GPIOB, GPIO_Pin_5, green_state);
            GPIO_WriteBit(GPIOB, GPIO_Pin_4, red_state);
        }
        
        if (count1 == 100)
        {
            // run required scheduler functions
            MotorSpeeds motorSpeeds = {0,0,0,0};
            calcOrientFlag = 1;
            
            // runs updatePid to get motors to turn on and then set as required
            updatePid(&motorSpeeds);
            if (motorSpeeds.m1 == 1)
                motorsSetRatio(0, 20000);
            else
                motorsSetRatio(0,0);
             if (motorSpeeds.m2 == 1)           
                motorsSetRatio(1, 20000);
             else
                motorsSetRatio(1,0);
             if (motorSpeeds.m3 == 1)           
                motorsSetRatio(2, 20000);
             else
                motorsSetRatio(2,0);
            if (motorSpeeds.m4 == 1)
                motorsSetRatio(3, 20000);
            else 
                motorsSetRatio(3,0); 
            count1 = 0;
       
        }
        TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
}

/*
* Enable Schedule Timer and interrupts
*/
void schedule()
{
    InitializeTimer();
    InitializeInterruptGen();
}


