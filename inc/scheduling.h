#include <lab04_tasks.h>

static int count = 0;
static int TIM5_IRQn = 50;
MotorSpeeds * motorSpeeds;
static int functions;
static int isCalled = 0;
