/* first, include the stm32f10x peripheral header files */
#include <stm32f10x_gpio.h>
#include <stm32f10x_tim.h>
#include <stm32f10x_rcc.h>
#include <misc.h>

/* then initialize the variable to control LED state */
int led_state = 0;


