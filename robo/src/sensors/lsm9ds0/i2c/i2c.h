#ifndef I2C_H
#define I2C_H

#include <stdint.h>

typedef enum {
    I2C_OK,
    I2C_WAIT,
    I2C_READ,
    I2C_SEND,
    I2C_STOP,
    I2C_START
} i2cStatus_t;

#define I2C_WR 0
#define I2C_RD 1

int i2c_waitIdle(void);
int i2c_setup(void);
int i2c_start(uint8_t addr, uint8_t dir);
int i2c_stop(void);
int i2c_sendData(uint8_t *data, uint32_t size);
int i2c_readData(uint8_t *data, uint32_t size);
char* i2c_error(i2cStatus_t s);

#endif
