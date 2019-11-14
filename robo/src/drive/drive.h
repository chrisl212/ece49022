#ifndef DRIVE_H
#define DRIVE_H

void drive_setup(void);
void drive_move(void);
void drive_collision(int c);
void drive_start(void);
void drive_stop(void);

struct {
    int integral;
    int Kp;
    int Ki;
    int pwm;
} speedPID;

struct {
    int integral;
    int lastError;
    int cnt;
    int Kp;
    int Ki;
    int Kd;
    int pwm;
} headPID;

struct {
    int rPWM;
    int lPWM;
    int rDir;
    int lDir;
} motorOut;

#endif
