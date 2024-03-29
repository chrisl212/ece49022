#include <stdint.h>
#include "ui/text/text.h"
#include "ui/ui.h"
#include "sensors/sensors.h"
#include "state/state.h"
#include "stm32f0xx.h"
#include "drive.h"

#define MAX_SPEED (400)
#define LIMIT (MAX_SPEED - 100)
#define TURN_SPEED (200)
extern int16_t head;
extern int16_t desiredHead;
int collide = 0;

//Setup pot input, motor output
static void _setup_gpio() {
    RCC->AHBENR |= RCC_AHBENR_GPIOAEN;

    GPIOA->MODER |= (0x2 << (2*8)) + (0x2 << (2*9)) + (0x2 << (2*10)) + (0x2 << (2*11));

    GPIOA->AFR[1] |= 0x2 << (4*(8-8)); //Configure PA8, PA9, PA10, and PA11 as AF2 for PWM output
    GPIOA->AFR[1] |= 0x2 << (4*(9-8));
    GPIOA->AFR[1] |= 0x2 << (4*(10-8));
    GPIOA->AFR[1] |= 0x2 << (4*(11-8));
}

static void _update_speed(void){
    if (motorOut.rDir){
        TIM1->CCR1 = motorOut.rPWM; //Right motor forward
        TIM1->CCR2 = 0;
    } else{
        TIM1->CCR1 = 0;
        TIM1->CCR2 = motorOut.rPWM; //Right motor reverse
    }
    if (motorOut.lDir){
        TIM1->CCR3 = motorOut.lPWM; //Left motor forward
        TIM1->CCR4 = 0;
    }else{
        TIM1->CCR3 = 0;
        TIM1->CCR4 = motorOut.lPWM; //Left motor reverse
    }
    if (collide) {
        TIM1->CCR1 = TIM1->CCR2 = TIM1->CCR3 = TIM1->CCR4 = 0;
    }
}

//This function sets up PWM on Timer 1 for both motor outputs
static void _setup_pwm() {
    RCC->APB2ENR |= RCC_APB2ENR_TIM1EN; //turn on timer 1 clock
    TIM1->CR1 &= ~TIM_CR1_DIR; //Configure timer 1 as up counter

    //PWM Frequency 100kHz
    TIM1->PSC = 480 - 1; //set prescaler
    TIM1->ARR = 1000 - 1; //set auto reload register

    //Configure PWM Channels, Left Motor . TIM1 CH1 Right Motor . TIM1 CH 2
    TIM1->CCMR1 |= TIM_CCMR1_OC1M_2 | TIM_CCMR1_OC1M_1 | TIM_CCMR1_OC1PE; //Configure TIM1 Channel 1 for PWM mode 1 w/ preload enabled AIN1
    TIM1->CCMR1 |= TIM_CCMR1_OC2M_2 | TIM_CCMR1_OC2M_1 | TIM_CCMR1_OC2PE; //Configure TIM1 Channel 2 for PWM mode 1 w/ preload enabled AIN2
    TIM1->CCMR2 |= TIM_CCMR2_OC3M_2 | TIM_CCMR2_OC3M_1 | TIM_CCMR2_OC3PE; //Configure TIM1 Channel 3 for PWM mode 1 w/ preload enabled BIN1
    TIM1->CCMR2 |= TIM_CCMR2_OC4M_2 | TIM_CCMR2_OC4M_1 | TIM_CCMR2_OC4PE; //Configure TIM1 Channel 3 for PWM mode 1 w/ preload enabled BIN1

    TIM1->CCER |= TIM_CCER_CC1E | TIM_CCER_CC2E | TIM_CCER_CC3E | TIM_CCER_CC4E; //Enable channels 1, 2, 3, and 4 on TIM1
    TIM1->BDTR |= TIM_BDTR_MOE; //Enables outputs
    TIM1->CCR1 = 0;
    TIM1->CCR2 = 0;
    TIM1->CCR3 = 0;
    TIM1->CCR4 = 0;
    TIM1->CR1 |= TIM_CR1_CEN; //Enable the timer
}

static void _base_PWM(int currentSpeed, int desiredSpeed){
    int error;


    error = desiredSpeed - currentSpeed;
    speedPID.integral = speedPID.integral + error;

    //speedPID.pwm = (speedPID.Kp * error) + (speedPID.Ki * speedPID.integral);
    speedPID.pwm = 500; //Run at half speed to test
    if (speedPID.pwm > 1000) {
        speedPID.pwm = 1000;
    } else if (speedPID.pwm < 0) {
        speedPID.pwm = 0;
    }
}

static void _head_PWM(int currentHeading, int desiredHeading){
    int error;

    error = desiredHeading - currentHeading;
    headPID.integral = headPID.integral + error;
    if (error < -180) {
    error += 360;
    } else if (error > 180) {
    error -= 360;
    }

    headPID.pwm = ((headPID.Kp / 1000.0) * error) + ((headPID.Ki / 1000000.0) * headPID.integral/* / headPID.cnt++*/) + ((headPID.Kd / 1000.0) * (error - headPID.lastError)); //divide by 1000 to work with ints
    headPID.pwm *= 10;

    if (headPID.pwm > LIMIT) {
        headPID.pwm = LIMIT;
    } else if (headPID.pwm < -LIMIT) {
        headPID.pwm = -LIMIT;
    }
    headPID.lastError = error;
}

static int _getCurrentSpeed(){
    return 500;
}
static int _getDesiredSpeed(){
    return 500;
}
static int _getCurrentHeading(){
    return head;
}
static int _getDesiredHeading(){
    return desiredHead;
}

void drive_move(void) {
    int currentSpeed;
    int desiredSpeed;
    int currentHeading;
    int desiredHeading;
    int rOffset;
    int lOffset;
    int error;

    //currentSpeed = _getCurrentSpeed(); //Get current speed from navigation controller
    //desiredSpeed = _getDesiredSpeed(); //Get desired speed from navigation controller
    currentHeading = _getCurrentHeading() - 180; //Get current heading from navigation controller
    desiredHeading = _getDesiredHeading() - 180; //Get desired heading from navigation controller

//    _base_PWM(currentSpeed, desiredSpeed); //calculate new speed pwm
   // _head_PWM(currentHeading, desiredHeading); //calculate new heading pwm
    
    error = currentHeading - desiredHeading;

    if (error < -180) {
        error += 360;
    } else if (error > 180) {
        error -= 360;
    }

    if(error > 85 || error < -85){
        if(error > 0){
            motorOut.rPWM = TURN_SPEED;
            motorOut.lPWM = TURN_SPEED;
            motorOut.lDir = 0;
            motorOut.rDir = 1;
        }else{
            motorOut.rPWM = TURN_SPEED;
            motorOut.lPWM = TURN_SPEED;
            motorOut.lDir = 1;
            motorOut.rDir = 0;
        }
    }else{
        _head_PWM(currentHeading, desiredHeading);
        if (headPID.pwm < 0){
            lOffset = headPID.pwm; //if heading offset is positive turn right
            rOffset = 0;
        } else {
            rOffset = -headPID.pwm; //if heading offset is negative turn left
            lOffset = 0;
        }
        
        motorOut.rPWM = speedPID.pwm + rOffset;
        motorOut.lPWM = speedPID.pwm + lOffset;
        motorOut.lDir = 1; //drive forward
        motorOut.rDir = 1; //drive forward
    }

    _update_speed();
}

static void _controller_setup(void) {
    RCC->AHBENR |= RCC_AHBENR_GPIOCEN;
    GPIOC->MODER |= 0x1 << (2*8);
    GPIOC->ODR &= ~(0x1 << 8);
}

void drive_setup(void) {
    _controller_setup();
    _setup_gpio();
    _setup_pwm();

    //initialize PID controllers
    //This will be updated when we get the velocity
    speedPID.integral = 0;
    speedPID.Kp = 0;
    speedPID.Ki = 0;
    speedPID.pwm = MAX_SPEED;

    //Mess with these values
    headPID.Kp = 900; //Kp = headPID.Kp / 1000 initially set to 9
    headPID.integral = 0;
    headPID.cnt = 0;
    headPID.lastError = 0;
    headPID.Ki = 0; //Kp = headPID.Ki / 1000 initially set to 100
    headPID.Kd = 0; //Kp = headPID.Ki / 1000 initially set to 1500
    headPID.pwm = 0;

    //enable motor driver
    GPIOC->ODR |= (0x1 << 8);
}

void drive_collision(int c) {
    int s = state_get();

    collide = c;
    if (c) {
        state_set(STATE_ERR);
        state_setErrorMessage("COLLISION!!!");
    } else {
        state_restore();
    }
    if (s != state_get()) {
        ui_draw();
    }
}

void drive_start(void) {
    collide = 0;
}

void drive_stop(void) {
    collide = 1;
}

