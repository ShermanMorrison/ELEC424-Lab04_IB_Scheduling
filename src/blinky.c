#include <blinky.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>


/*
* Initialize GPIOB's  GPIO_Pin_5 as a push-pull GPIOB pin.
*/
void InitializeLEDs()
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE); 
    GPIO_InitTypeDef gpioStructure;
    gpioStructure.GPIO_Pin = (GPIO_Pin_4 | GPIO_Pin_5);
    gpioStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    gpioStructure.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_Init(GPIOB, &gpioStructure); // configure gpio pin 5
    GPIO_PinRemapConfig( GPIO_Remap_SWJ_NoJTRST, ENABLE);
}


/*
* Enable and initialize interrupt on timer 2
*/
void InitializeInterrupt()
{
    NVIC_InitTypeDef nvic;
    nvic.NVIC_IRQChannel = TIM2_IRQn;
    nvic.NVIC_IRQChannelPreemptionPriority = 0;
    nvic.NVIC_IRQChannelSubPriority = 1;
    nvic.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&nvic); // configure the timer 2 interrupt
}


//Initialization. Will set all motors ratio to 0%
void motorsInit()
{
  if (isInit)
    return;

  //Init structures
  GPIO_InitTypeDef GPIO_InitStructure;
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
  TIM_OCInitTypeDef  TIM_OCInitStructure;

  //Enable gpio and the timer
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO | MOTORS_GPIO_PERIF, ENABLE);
  RCC_APB1PeriphClockCmd(MOTORS_GPIO_TIM_PERIF | MOTORS_GPIO_TIM_M3_4_PERIF, ENABLE);
  // Configure the GPIO for the timer output
  GPIO_InitStructure.GPIO_Pin = (GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_8 | GPIO_Pin_9);
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(MOTORS_GPIO_PORT, &GPIO_InitStructure);

  //Remap M2-4
  GPIO_PinRemapConfig(MOTORS_REMAP , ENABLE);

  //Timer configuration
  TIM_TimeBaseStructure.TIM_Period = 7200;
  TIM_TimeBaseStructure.TIM_Prescaler = 100; 
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseInit(MOTORS_GPIO_TIM_M1_2, &TIM_TimeBaseStructure);

  TIM_TimeBaseStructure.TIM_Period = 7200;
  TIM_TimeBaseStructure.TIM_Prescaler = 100;
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseInit(MOTORS_GPIO_TIM_M3_4, &TIM_TimeBaseStructure);

  //PWM channels configuration (All identical!)
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_Pulse = 0;
  TIM_OCInitStructure.TIM_OCPolarity = MOTORS_POLARITY;

  TIM_OC3Init(MOTORS_GPIO_TIM_M3_4, &TIM_OCInitStructure);
  TIM_OC3PreloadConfig(MOTORS_GPIO_TIM_M3_4, TIM_OCPreload_Enable);

  TIM_OC4Init(MOTORS_GPIO_TIM_M3_4, &TIM_OCInitStructure);
  TIM_OC4PreloadConfig(MOTORS_GPIO_TIM_M3_4, TIM_OCPreload_Enable);

  TIM_OC3Init(MOTORS_GPIO_TIM_M1_2, &TIM_OCInitStructure);
  TIM_OC3PreloadConfig(MOTORS_GPIO_TIM_M1_2, TIM_OCPreload_Enable);

  TIM_OC4Init(MOTORS_GPIO_TIM_M1_2, &TIM_OCInitStructure);
  TIM_OC4PreloadConfig(MOTORS_GPIO_TIM_M1_2, TIM_OCPreload_Enable);

  //Enable the timer
  TIM_Cmd(MOTORS_GPIO_TIM_M1_2, ENABLE);
  TIM_Cmd(MOTORS_GPIO_TIM_M3_4, ENABLE);
  //Enable the timer PWM outputs
  TIM_CtrlPWMOutputs(MOTORS_GPIO_TIM_M1_2, ENABLE);
  TIM_CtrlPWMOutputs(MOTORS_GPIO_TIM_M3_4, ENABLE);
  // Halt timer during debug halt.
  DBGMCU_Config(MOTORS_GPIO_TIM_M1_2_DBG, ENABLE);
  DBGMCU_Config(MOTORS_GPIO_TIM_M3_4_DBG, ENABLE);
  
  isInit = true;
}



void motorsSetRatio(int id, uint16_t ratio)
{
  switch(id)
  {
    case MOTOR_M1:
      TIM_SetCompare4(MOTORS_GPIO_TIM_M1_2, C_16_TO_BITS(ratio));
      break;
    case MOTOR_M2:
      TIM_SetCompare3(MOTORS_GPIO_TIM_M1_2, C_16_TO_BITS(ratio));
      break;
    case MOTOR_M3:
      TIM_SetCompare4(MOTORS_GPIO_TIM_M3_4, C_16_TO_BITS(ratio));
      break;
    case MOTOR_M4:
      TIM_SetCompare3(MOTORS_GPIO_TIM_M3_4, C_16_TO_BITS(ratio));
      break;
  }
}    



/*
* Main Function
*/
int main()
{ 
    //initial delay to give time to program
    for(i=0; i<65535; i++)
        for(j = 0; j<100;j++);
    SystemInit();
    motorsInit();
    InitializeLEDs();
    schedule();
    
    while(1){

        if (isCalled == 1){
	    isCalled = 0;
	    if (functions & 0x01 == 1)
		detectEmergency();
	    if (functions & 0x02 == 1)
	        refreshSensorData();
	    if (functions & 0x04 == 1)
		calculateOrientation();
	    if (functions & 0x08 == 1)
		updatePid(motorSpeeds);
            if (functions & 0x10 == 1)
	        logDebugInfo();
	
            motorsSetRatio(0, motorSpeeds->m1);
            motorsSetRatio(1, motorSpeeds->m2);
	    motorsSetRatio(2, motorSpeeds->m3);
	    motorsSetRatio(3, motorSpeeds->m4);
	}
	
    }
    return(0);
}


