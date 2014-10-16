#include <lab04_tasks.h>

#define NUM_FUNCTIONS 4

#define DETECT   1
#define REFRESH  2
#define CALC     3
#define UPDATE   4
#define LOG      5


static int green_state = 0;
static int red_state = 1;
static int count = 0;
static int TIM5_IRQn = 50;
MotorSpeeds * motorSpeeds;
static int functions;
static int isCalled = 0;
static int k = 0;
static int scheduled_functions[NUM_FUNCTIONS];



