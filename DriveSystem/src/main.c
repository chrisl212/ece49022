/**
  ******************************************************************************
  * @file    main.c
  * @author  Ac6
  * @version V1.0
  * @date    01-December-2013
  * @brief   Default main function.
  ******************************************************************************
*/


#include "stm32f0xx.h"
#include "stm32f0_discovery.h"
#include "pid.h"
#include <stdio.h>


//setup TIM3 interrupt to trigger ADC every 200us
void setup_timer3(){
	RCC->APB1ENR |= RCC_APB1ENR_TIM3EN; //turn on TIM3 peripheral

	//Setup interrupt rate to 200us
	TIM3->PSC = 48 - 1; //Setup TIM3 Prescaler
	TIM3->ARR = 200 - 1; //Setup TIM3 Auto Reload Register

	TIM3->DIER |= TIM_DIER_UIE; //enable update interrupt
	NVIC->ISER[0] = 1 << TIM3_IRQn; //enable TIM3 interrupt
	TIM3->CR1 |= TIM_CR1_CEN; //enable TIM3
}



//Setup ADC for reading from pot
void setup_adc(){
	RCC->APB2ENR |= RCC_APB2ENR_ADC1EN; //Enable ADC peripheral
	RCC->CR2 |= RCC_CR2_HSI14ON; //Turn on oscillator
	while(!(RCC->CR2 & RCC_CR2_HSI14RDY));//wait for oscillator to turn on
	ADC1->CR |= ADC_CR_ADEN; //enable the ADC
	while(!(ADC1->ISR & ADC_ISR_ADRDY)); //Wait for ADC to be ready
	while((ADC1->CR & ADC_CR_ADSTART)); //Wait for ADC to be ready
}

//Read the ADC at the desired channel
int read_adc_channel(unsigned int channel){
	int adcVal;

	ADC1->CHSELR = 0; //clear channel select register
	ADC1->CHSELR |= 1 << channel; //enable channel for conversion
	while(!(ADC1->ISR & ADC_ISR_ADRDY)); //wait for ADC to be ready
	ADC1->CR |= ADC_CR_ADSTART; //start conversion
	while(!(ADC1->ISR & ADC_ISR_EOC)); //wait for conversion to be complete
	adcVal = ADC1->DR; //return value from data register

	return adcVal;
}

//Setup pot input, motor output
void setup_gpio() {
	RCC->AHBENR |= RCC_AHBENR_GPIOAEN;

	GPIOA->MODER &= ~3<<2; //Configure PA1 as the analog input from pot
	GPIOA->MODER |= 3<<2;
	GPIOA->MODER &= ~255<<16; //Configure PA8 and PA9 and PA10 and PA11 as alternate function pins
	GPIOA->MODER |= 170<<16;

	GPIOA->AFR[1] |= 2; //Configure PA8, PA9, PA10, and PA11 as AF2 for PWM output
	GPIOA->AFR[1] |= 2 << 4;
	GPIOA->AFR[1] |= 2 << 8;
	GPIOA->AFR[1] |= 2 << 12;
}

//Update the capture/compare registers on TIM1 based on pot input
void update_speed(struct Motor *motorOut){
	//int spd;

	if (motorOut->rDir){
		TIM1->CCR1 = motorOut->rPWM; //Right motor forward
		TIM1->CCR2 = 0;
	}else{
		TIM1->CCR1 = 0;
		TIM1->CCR2 = motorOut->rPWM; //Right motor reverse
	}
	if (motorOut->lDir){
		TIM1->CCR3 = motorOut->lPWM; //Left motor forward
		TIM1->CCR4 = 0;
	}else{
		TIM1->CCR3 = 0;
		TIM1->CCR4 = motorOut->lPWM; //Left motor reverse
	}


	char line1[16];
	//sprintf(line1, "Value Is: %d", spd);
	display1(line1);
}

//This function sets up PWM on Timer 1 for both motor outputs
void setup_pwm() {
	RCC->APB2ENR |= RCC_APB2ENR_TIM1EN; //turn on timer 1 clock
	TIM1->CR1 &= ~TIM_CR1_DIR; //Configure timer 1 as up counter

	//PWM Frequency 100kHz
	TIM1->PSC = 4800 - 1; //set prescaler
	TIM1->ARR = 1000 - 1; //set auto reload register

	//Configure PWM Channels, Left Motor -> TIM1 CH1	Right Motor -> TIM1 CH 2
	TIM1->CCMR1 |= TIM_CCMR1_OC1M_2 | TIM_CCMR1_OC1M_1 | TIM_CCMR1_OC1PE; //Configure TIM1 Channel 1 for PWM mode 1 w/ preload enabled AIN1
	TIM1->CCMR1 |= TIM_CCMR1_OC2M_2 | TIM_CCMR1_OC2M_1 | TIM_CCMR1_OC2PE; //Configure TIM1 Channel 2 for PWM mode 1 w/ preload enabled AIN2
	TIM1->CCMR2 |= TIM_CCMR2_OC3M_2 | TIM_CCMR2_OC3M_1 | TIM_CCMR2_OC3PE; //Configure TIM1 Channel 3 for PWM mode 1 w/ preload enabled BIN1
	TIM1->CCMR2 |= TIM_CCMR2_OC4M_2 | TIM_CCMR2_OC4M_1 | TIM_CCMR2_OC4PE; //Configure TIM1 Channel 3 for PWM mode 1 w/ preload enabled BIN1

	TIM1->CCER |= TIM_CCER_CC1E | TIM_CCER_CC2E | TIM_CCER_CC3E | TIM_CCER_CC4E; //Enable channels 1, 2, 3, and 4 on TIM1
	TIM1->BDTR |= TIM_BDTR_MOE; //Enables outputs
	TIM1->CR1 |= TIM_CR1_CEN; //Enable the timer
}
void TIM3_IRQHandler() {
	//int spd;

	//spd = read_adc_channel(1);
	//update_speed();

	TIM3->SR &= ~TIM_SR_UIF;
}

void base_PWM(int currentSpeed, int desiredSpeed, struct Speed *speedPID){
	int error;


	error = desiredSpeed - currentSpeed;
	speedPID->integral = speedPID->integral + error;

	//speedPID.pwm = (speedPID.Kp * error) + (speedPID.Ki * speedPID.integral);
	speedPID->pwm = 500; //Run at half speed to test
	if (speedPID->pwm > 1000) speedPID->pwm = 1000;
	else if (speedPID->pwm < 0) speedPID->pwm = 0;

}

void head_PWM(int currentHeading, int desiredHeading, struct Head *headPID){
	int error;


	error = desiredHeading - currentHeading;
	headPID->integral = headPID->integral + error;

	headPID->pwm = ((headPID->Kp / 1000) * error) + ((headPID->Ki / 1000) * headPID->integral); //divide by 1000 to work with ints

	if (headPID->pwm > 500) headPID->pwm = 500;
	else if (headPID->pwm < -500) headPID->pwm = -500;

}

int getCurrentSpeed(){
	int currentSpeed;


	return currentSpeed;
}
int getDesiredSpeed(){
	int desiredSpeed;

	return desiredSpeed;
}
int getCurrentHeading(){
	int currentHeading;

	currentHeading = 500;

	return currentHeading;
}
int getDesiredHeading(){
	int desiredHeading;

	desiredHeading = 1000;

	return desiredHeading;
}
int main(void)
{
	init_lcd();

	setup_gpio();
	setup_timer3();
	setup_adc();
	setup_pwm();

	struct Speed speedPID;
	struct Head headPID;
	struct Motor motorOut;

	//initialize PID controllers
	//This will be updated when we get the velocity
	speedPID.integral = 0;
	speedPID.Kp = 0;
	speedPID.Ki = 0;
	speedPID.pwm = 0;

	//Mess with these values
	headPID.Kp = 2000; //Kp = headPID.Kp / 1000
	headPID.integral = 0;
	headPID.Ki = 1000; //Kp = headPID.Ki / 1000
	headPID.pwm = 0;

	int currentSpeed;
	int desiredSpeed;
	int currentHeading;
	int desiredHeading;

	int rOffset;
	int lOffset;


	currentSpeed = 500; //Remove after testing
	desiredSpeed = 500; //These values will be replaced when we get velocity

	while(1){
		//currentSpeed = getCurrentSpeed(); //Get current speed from navigation controller
		//desiredSpeed = getDesiredSpeed(); //Get desired speed from navigation controller
		currentHeading = getCurrentHeading(); //Get current heading from navigation controller
		desiredHeading = getDesiredHeading(); //Get desired heading from navigation controller

		base_PWM(currentSpeed, desiredSpeed, &speedPID); //calculate new speed pwm
		head_PWM(currentHeading, desiredHeading, &headPID); //calculate new heading pwm

		if (headPID.pwm > 0){
			rOffset = headPID.pwm * -1; //if heading offset is positive turn right
			lOffset = 0;
		}
		else{
			lOffset = headPID.pwm; //if heading offset is negative turn left
			rOffset = 0;
		}
		motorOut.rPWM = speedPID.pwm + rOffset;
		motorOut.lPWM = speedPID.pwm + lOffset;
		motorOut.lDir = 1; //drive forward
		motorOut.rDir = 1; //drive forward
		update_speed(&motorOut);
	}
}
