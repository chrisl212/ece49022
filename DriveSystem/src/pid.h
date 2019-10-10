void setup_timer3();
void TIM3_IRQHandler();
void setup_adc();
int read_adc_channel(unsigned int channel);
void setup_gpio();
void setup_pwm();

int getCurrentSpeed();
int getDesiredSpeed();
int getCurrentHeading();
int getDesiredHeading();

struct Speed
{
	int integral;
	int Kp;
	int Ki;
	int pwm;
};

struct Head
{
	int integral;
	int Kp;
	int Ki;
	int pwm;
};

struct Motor
{
	int rPWM;
	int lPWM;
	int rDir;
	int lDir;
};
void update_speed(struct Motor *motorOut);
void base_PWM(int currentSpeed, int desiredSpeed, struct Speed *speedPID);
void head_PWM(int currentHeading, int desiredHeading, struct Head *headPID);

