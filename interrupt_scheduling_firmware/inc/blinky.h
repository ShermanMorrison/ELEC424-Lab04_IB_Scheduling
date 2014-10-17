/* first, include the stm32f10x peripheral header files */
#include <stm32f10x_gpio.h>
#include <stm32f10x_tim.h>
#include <stm32f10x_rcc.h>
#include <misc.h>
#include <stdbool.h>
#include <scheduling.h>

#ifdef BRUSHLESS_MOTORCONTROLLER 

#define BLMC_PERIOD 0.005   // 5ms = 200Hz

#define MOTORS_PWM_PRESCALE_RAW   7200 
#define MOTORS_PWM_CNT_FOR_PERIOD 20 * BLMC_PERIOD / MOTORS_PWM_PRESCALE_RAW)
#define MOTORS_PWM_CNT_FOR_1MS    (uint32_t)(72000000 * 0.001 / MOTORS_PWM_PRESCALE_RAW)
#define MOTORS_PWM_PERIOD         MOTORS_PWM_CNT_FOR_PERIOD
#define MOTORS_PWM_BITS           11  // Only for compatibiliy
#define MOTORS_PWM_PRESCALE       (uint16_t)(MOTORS_PWM_PRESCALE_RAW - 1)
#define MOTORS_POLARITY           TIM_OCPolarity_Low
#else
//The following defines gives a PWM of 9 bits at ~140KHz for a sysclock of 72MHz
#define MOTORS_PWM_BITS     9
#define MOTORS_PWM_PERIOD   ((1<<MOTORS_PWM_BITS) - 1)
#define MOTORS_PWM_PRESCALE 0
#define MOTORS_POLARITY           TIM_OCPolarity_High
#endif


// Motors IDs define
#define MOTOR_M1  0
#define MOTOR_M2  1
#define MOTOR_M3  2
#define MOTOR_M4  3

// Test defines
#define MOTORS_TEST_RATIO         (uint16_t)(0.5*(1<<16))
#define MOTORS_TEST_ON_TIME_MS    10
#define MOTORS_TEST_DELAY_TIME_MS 50

/*** Public interface ***/

/**
 * Initialisation. Will set all motors ratio to 0%
 */
void motorsInit();

/**
 * Set the PWM ratio of the motor 'id'
 */
void motorsSetRatio(int id, uint16_t ratio);

/**
 * Get the PWM ratio of the motor 'id'. Return -1 if wrong ID.
 */
int motorsGetRatio(int id);


/* then initialize the variable to control LED state */
int led_state = 0;
int i, j;

#define MOTORS_GPIO_TIM_PERIF     RCC_APB1Periph_TIM3
#define MOTORS_GPIO_TIM_M1_2      TIM3
#define MOTORS_GPIO_TIM_M1_2_DBG  DBGMCU_TIM3_STOP
#define MOTORS_REMAP              GPIO_PartialRemap_TIM3

#define MOTORS_GPIO_TIM_M3_4_PERIF  RCC_APB1Periph_TIM4
#define MOTORS_GPIO_TIM_M3_4        TIM4
#define MOTORS_GPIO_TIM_M3_4_DBG    DBGMCU_TIM4_STOP

#define MOTORS_GPIO_PERIF         RCC_APB2Periph_GPIOB
#define MOTORS_GPIO_PORT          GPIOB
#define MOTORS_GPIO_M1            GPIO_Pin_1 // T3_CH4
#define MOTORS_GPIO_M2            GPIO_Pin_0 // T3_CH3
#define MOTORS_GPIO_M3            GPIO_Pin_9 // T4_CH4
#define MOTORS_GPIO_M4            GPIO_Pin_8 // T4_CH3

/* Utils Conversion macro */
#ifdef BRUSHLESS_MOTORCONTROLLER
  #define C_BITS_TO_16(X) (0xFFFF * (X - MOTORS_PWM_CNT_FOR_1MS) / MOTORS_PWM_CNT_FOR_1MS)
  #define C_16_TO_BITS(X) (MOTORS_PWM_CNT_FOR_1MS + ((X * MOTORS_PWM_CNT_FOR_1MS) / 0xFFFF))
#else
  #define C_BITS_TO_16(X) ((X)<<(16-MOTORS_PWM_BITS))
  #define C_16_TO_BITS(X) ((X)>>(16-MOTORS_PWM_BITS)&((1<<MOTORS_PWM_BITS)-1))
#endif

const int MOTORS[] = { MOTOR_M1, MOTOR_M2, MOTOR_M3, MOTOR_M4 };
static bool isInit = false;


int TIM3_MOTOR_STATE = 0;
int TIM4_MOTOR_STATE = 0;
