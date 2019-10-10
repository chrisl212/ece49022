void setup_timer3();
void TIM3_IRQHandler();
void setup_adc();
int read_adc_channel(unsigned int channel);
void setup_gpio();
void update_speed();
void setup_pwm();

struct
{
	int integral;
	int Kp;
	int Ki;
	int pwm;
} speedPID;

struct
{
	int integral;
	int Kp;
	int Ki;
	int pwm;
} headPID;

struct
{
	int rPWM;
	int lPWM;
	int rDir;
	int lDir;
} motorOut;
